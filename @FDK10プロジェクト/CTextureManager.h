#pragma once

#include "CTexture.h"

namespace FDK {
	namespace Graphics {

class CTextureManager
{
public:
	// �L���b�V���̎g�p�t���O�̃��Z�b�g
	void ResetCache();
	
	// �L���b�V�����ꂽ�e�N�X�`����Ԃ��B
	// �Ԃ��ꂽ�e�N�X�`���́ARestoreDeviceObjects() �܂Ŋ������Ă����ԁB
	CTexture* GetCachedTexture( LPCTSTR name, LPCTSTR filename, DWORD width=0, DWORD height=0 );

	// �L���b�V�����̖��g�p�e�N�X�`���̍폜
	void RemoveUnusedTextures();

	HRESULT	OneTimeSceneInit();						// �������X�g�̏�����
	HRESULT	InitDeviceObjects();					// �������X�g�̊e�e�N�X�`���̏�����
	HRESULT	RestoreDeviceObjects();					// �������X�g�̊e�e�N�X�`���̍\�z
	HRESULT	InvalidateDeviceObjects();				// �������X�g�̊e�e�N�X�`���̖�����
	HRESULT	DeleteDeviceObjects();					// �������X�g�̊e�e�N�X�`���̔j��
	HRESULT	FinalCleanup();							// �������X�g�̔j��

public:
	CTextureManager();
	virtual ~CTextureManager() {};

protected:
	struct TextureList {
		bool		bUse;							// �g�p����Ȃ� true
		TCHAR		strTextureName[ _MAX_PATH ];	// �e�N�X�`����
		TCHAR		strFileName[ _MAX_PATH ];		// �t�@�C����
		DWORD		width, height;					// �T�C�Y
		FILETIME	ftLastWriteTime;				// �ŏI�X�V����
		CTexture	texture;						// �e�N�X�`��
		TextureList	*prev, *next;					// �O�^���̃Z��
	} *pTextureList, *pTextureListLast;
};

	}//Graphics
}//FDK

using namespace FDK::Graphics;
