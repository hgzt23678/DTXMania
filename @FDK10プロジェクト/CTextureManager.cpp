#include "stdafx.h"
#include "CTextureManager.h"

namespace FDK {
	namespace Graphics {

CTextureManager::CTextureManager()
{
	this->pTextureList = this->pTextureListLast = NULL;
}

void	  CTextureManager::ResetCache()
{
	for( TextureList* sc = this->pTextureList; sc != NULL; sc = sc->next )
		sc->bUse = false;
}
CTexture* CTextureManager::GetCachedTexture( LPCTSTR name, LPCTSTR filename, DWORD width, DWORD height )
{
	TextureList* cell;

	// �t�@�C���̍ŏI�X�V�����̎擾�G
	// �@fileInfo �� �w�肳�ꂽ�t�@�C���̍ŏI�X�V����
	WIN32_FILE_ATTRIBUTE_DATA fileInfo;
	if( ! GetFileAttributesEx( filename, GetFileExInfoStandard, &fileInfo) )
		return NULL;	// ���s

	// �L���b�V�����X�g�̒��ɓ���̂��̂����邩���ׂ�
	for( cell = this->pTextureList; cell != NULL; cell = cell->next )
	{
		if( lstrcmp( name, cell->strTextureName ) == 0 && lstrcmp( filename, cell->strFileName ) == 0 
			&& ( fileInfo.ftLastWriteTime.dwLowDateTime  == cell->ftLastWriteTime.dwLowDateTime )
			&& ( fileInfo.ftLastWriteTime.dwHighDateTime == cell->ftLastWriteTime.dwHighDateTime )
			&& ( cell->width == width && cell->height == height ) )
		{
			cell->bUse = true;
			return &cell->texture;			// �L���b�V���ɂ������̂ł�������p����
		}
	}

	// ���ł̐V�K�쐬
	cell = new TextureList();
	if( FAILED( cell->texture.InitDeviceObjectsFromFile( name, filename, width, height ) )
		|| FAILED( cell->texture.RestoreDeviceObjects() ) )
	{
		cell->texture.DeleteDeviceObjects();
		SAFE_DELETE( cell );
		return NULL;		// �\�z�Ɏ��s
	}
	cell->bUse = true;
	lstrcpyn( cell->strFileName, filename, _MAX_PATH );
	lstrcpyn( cell->strTextureName, name, _MAX_PATH );
	cell->width = width;
	cell->height = height;
	cell->ftLastWriteTime = fileInfo.ftLastWriteTime;
	cell->prev = cell->next = NULL;
	APPENDLIST( this->pTextureList, this->pTextureListLast, cell );
	return &cell->texture;
}

void	  CTextureManager::RemoveUnusedTextures()
{
	TextureList* sc = this->pTextureList;
	while( sc != NULL )
	{
		TextureList* next = sc->next;
		if( ! sc->bUse )
		{
			REMOVELIST( this->pTextureList, this->pTextureListLast, sc );
			sc->texture.DeleteDeviceObjects();
			SAFE_DELETE( sc );
		}
		sc = next;
	}
}

HRESULT	  CTextureManager::OneTimeSceneInit()
{
	TextureList *s = this->pTextureList;
	while( s != NULL )
	{
		TextureList *next = s->next;
		s->texture.DeleteDeviceObjects();
		SAFE_DELETE( s );
		s = next;
	}
	this->pTextureList = this->pTextureListLast = NULL;
	return S_OK;
}

HRESULT	  CTextureManager::FinalCleanup()
{
	TextureList *s = this->pTextureList;
	while( s != NULL )
	{
		TextureList *next = s->next;
		s->texture.DeleteDeviceObjects();
		SAFE_DELETE( s );
		s = next;
	}
	this->pTextureList = this->pTextureListLast = NULL;
	return S_OK;
}

HRESULT	  CTextureManager::InitDeviceObjects()
{
	for( TextureList* s = this->pTextureList; s != NULL; s = s->next )
		s->texture.InitDeviceObjectsFromFile( s->strTextureName, s->strFileName, s->width, s->height );
	return S_OK;
}

HRESULT	  CTextureManager::RestoreDeviceObjects()
{
	for( TextureList* s = this->pTextureList; s != NULL; s = s->next )
		s->texture.RestoreDeviceObjects();
	return S_OK;
}

HRESULT	  CTextureManager::InvalidateDeviceObjects()
{
	for( TextureList* s = this->pTextureList; s != NULL; s = s->next )
		s->texture.InvalidateDeviceObjects();
	return S_OK;
}

HRESULT	  CTextureManager::DeleteDeviceObjects()
{
	for( TextureList* s = this->pTextureList; s != NULL; s = s->next )
		s->texture.DeleteDeviceObjects();
	return S_OK;
}

	}//Graphics
}//FDK