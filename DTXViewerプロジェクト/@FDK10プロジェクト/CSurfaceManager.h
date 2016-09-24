#pragma once

#include "CSurface.h"

namespace FDK {
	namespace Graphics {

class CSurfaceManager
{
public:
	// �L���b�V���̎g�p�t���O�̃��Z�b�g
	void		ResetCache();

	// �L���b�V�����ꂽ�T�[�t�F�C�X��Ԃ��B
	// �Ԃ��ꂽ�T�[�t�F�C�X�́ARestoreDeviceObjects() �܂Ŋ������Ă����ԁB
	CSurface*	GetCachedSurface( LPCTSTR name, LPCTSTR filename, DWORD width=0, DWORD height=0, LOADPLACE place=LOADTO_SYSTEM );

	// �L���b�V�����̖��g�p�T�[�t�F�C�X�̍폜
	void		RemoveUnusedSurfaces();

	HRESULT	OneTimeSceneInit();						// �������X�g�̏�����
	HRESULT	InitDeviceObjects();					// �������X�g�̊e�T�[�t�F�C�X�̏�����
	HRESULT	RestoreDeviceObjects();					// �������X�g�̊e�T�[�t�F�C�X�̍\�z
	HRESULT	InvalidateDeviceObjects();				// �������X�g�̊e�T�[�t�F�C�X�̖�����
	HRESULT	DeleteDeviceObjects();					// �������X�g�̊e�T�[�t�F�C�X�̔j��
	HRESULT	FinalCleanup();							// �������X�g�̔j��

	CSurfaceManager();

protected:
	struct SurfaceList {
		bool		bUse;							// �g�p����Ȃ� true
		TCHAR		strSurfaceName[ _MAX_PATH ];	// �T�[�t�F�C�X��
		TCHAR		strFileName[ _MAX_PATH ];		// �t�@�C����
		DWORD		width, height;					// �T�C�Y
		LOADPLACE	place;							// �����ꏊ
		FILETIME	ftLastWriteTime;				// �ŏI�X�V����
		CSurface	surface;						// �T�[�t�F�C�X
		SurfaceList	*prev, *next;					// �O�^���̃Z��
	} *pSurfaceList, *pSurfaceListLast;
};

	}//Graphics
}//FDK

using namespace FDK::Graphics;
