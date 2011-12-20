#include "stdafx.h"
#include "Debug.h"
#include "CSurfaceManager.h"

namespace FDK {
	namespace Graphics {

CSurfaceManager::CSurfaceManager()
{
	this->pSurfaceList = this->pSurfaceListLast = NULL;
}

void	  CSurfaceManager::ResetCache()
{
	for( SurfaceList* sc = this->pSurfaceList; sc != NULL; sc = sc->next )
		sc->bUse = false;
}

CSurface* CSurfaceManager::GetCachedSurface( LPCTSTR name, LPCTSTR filename, DWORD width, DWORD height, LOADPLACE place )
{
	SurfaceList* cell;

	// �t�@�C���̍ŏI�X�V�����̎擾�G
	// �@fileInfo �� �w�肳�ꂽ�t�@�C���̍ŏI�X�V����
	WIN32_FILE_ATTRIBUTE_DATA fileInfo;
	if( ! ::GetFileAttributesEx( filename, GetFileExInfoStandard, &fileInfo) )
		return NULL;	// ���s

	// �L���b�V�����X�g�̒��ɓ���̂��̂����邩���ׂ�
	for( cell = this->pSurfaceList; cell != NULL; cell = cell->next )
	{
		if( lstrcmp( name, cell->strSurfaceName ) == 0 && lstrcmp( filename, cell->strFileName ) == 0 
			&& ( fileInfo.ftLastWriteTime.dwLowDateTime  == cell->ftLastWriteTime.dwLowDateTime )
			&& ( fileInfo.ftLastWriteTime.dwHighDateTime == cell->ftLastWriteTime.dwHighDateTime )
			&& ( cell->width == width && cell->height == height && cell->place == place ) )
		{
			cell->bUse = true;			// �������̂ł�����g��
			return &cell->surface;
		}
	}

	// ���ł̐V�K�쐬
	cell = new SurfaceList();
	cell->surface.InitDeviceObjectsFromFile( name, filename, width, height, place );
	if( FAILED( cell->surface.RestoreDeviceObjects() ) )
	{
		cell->surface.DeleteDeviceObjects();
		SAFE_DELETE( cell );
		return NULL;		// �\�z�Ɏ��s
	}
	cell->bUse = true;
	lstrcpyn( cell->strFileName, filename, _MAX_PATH );
	lstrcpyn( cell->strSurfaceName, name, _MAX_PATH );
	cell->width = width;
	cell->height = height;
	cell->place = place;
	cell->ftLastWriteTime = fileInfo.ftLastWriteTime;
	cell->prev = cell->next = NULL;
	APPENDLIST( this->pSurfaceList, this->pSurfaceListLast, cell );
	return &cell->surface;
}

void	  CSurfaceManager::RemoveUnusedSurfaces()
{
	SurfaceList* sc = this->pSurfaceList;
	while( sc != NULL )
	{
		SurfaceList* next = sc->next;
		if( ! sc->bUse )
		{
			REMOVELIST( this->pSurfaceList, this->pSurfaceListLast, sc );
			sc->surface.DeleteDeviceObjects();
			SAFE_DELETE( sc );
		}
		sc = next;
	}
}

HRESULT	  CSurfaceManager::OneTimeSceneInit()
{
	SurfaceList *s = this->pSurfaceList;
	while( s != NULL )
	{
		SurfaceList *next = s->next;
		s->surface.DeleteDeviceObjects();
		SAFE_DELETE( s );
		s = next;
	}
	this->pSurfaceList = this->pSurfaceListLast = NULL;
	return S_OK;
}

HRESULT	  CSurfaceManager::FinalCleanup()
{
	SurfaceList *s = this->pSurfaceList;
	while( s != NULL )
	{
		SurfaceList *next = s->next;
		s->surface.DeleteDeviceObjects();
		SAFE_DELETE( s );
		s = next;
	}
	this->pSurfaceList = this->pSurfaceListLast = NULL;
	return S_OK;
}

HRESULT	  CSurfaceManager::InitDeviceObjects()
{
	for( SurfaceList* s = this->pSurfaceList; s != NULL; s = s->next )
		s->surface.InitDeviceObjectsFromFile( s->strSurfaceName, s->strFileName, s->width, s->height, s->place );
	return S_OK;
}

HRESULT	  CSurfaceManager::RestoreDeviceObjects()
{
	for( SurfaceList* s = this->pSurfaceList; s != NULL; s = s->next )
		s->surface.RestoreDeviceObjects();
	return S_OK;
}

HRESULT	  CSurfaceManager::InvalidateDeviceObjects()
{
	for( SurfaceList* s = this->pSurfaceList; s != NULL; s = s->next )
		s->surface.InvalidateDeviceObjects();
	return S_OK;
}

HRESULT	  CSurfaceManager::DeleteDeviceObjects()
{
	for( SurfaceList* s = this->pSurfaceList; s != NULL; s = s->next )
		s->surface.DeleteDeviceObjects();

	return S_OK;
}

	}//Graphics
}//FDK