
#pragma once

namespace FDK {
	namespace General {

#define _FACFDK		0x0266

#define FDKERR_����������ĂȂ�				MAKE_HRESULT( 1, _FACFDK,  1 )
#define FDKERR_�A�_�v�^���Ȃ�				MAKE_HRESULT( 1, _FACFDK,  2 )
#define	FDKERR_DirectSound��NULL			MAKE_HRESULT( 1, _FACFDK,  3 )
#define	FDKERR_�T�E���h�o�b�t�@��NULL		MAKE_HRESULT( 1, _FACFDK,  4 )
#define	FDKERR_SoundType������				MAKE_HRESULT( 1, _FACFDK,  5 )
#define	FDKERR_�T�E���h�o�b�t�@�̍쐬�Ɏ��s	MAKE_HRESULT( 1, _FACFDK,  6 )
#define	FDKERR_�t�@�C���̓ǂݍ��ݎ��s		MAKE_HRESULT( 1, _FACFDK,  7 )
#define	FDKERR_DirectDraw��NULL				MAKE_HRESULT( 1, _FACFDK,  8 )
#define	FDKERR_�T�[�t�F�C�X��NULL			MAKE_HRESULT( 1, _FACFDK,  9 )
#define	FDKERR_�T�[�t�F�C�X�̍쐬�Ɏ��s		MAKE_HRESULT( 1, _FACFDK, 10 )
#define	FDKERR_DC�̎擾�Ɏ��s				MAKE_HRESULT( 1, _FACFDK, 11 )
#define FDKERR_StretchBlt�Ɏ��s				MAKE_HRESULT( 1, _FACFDK, 12 )
#define FDKERR_Blt�Ɏ��s					MAKE_HRESULT( 1, _FACFDK, 13 )
#define FDKERR_Lock�Ɏ��s					MAKE_HRESULT( 1, _FACFDK, 14 )
#define FDKERR_Caps�̎擾�Ɏ��s				MAKE_HRESULT( 1, _FACFDK, 15 )
#define	FDKERR_�e�N�X�`���t�H�[�}�b�g���Ȃ�	MAKE_HRESULT( 1, _FACFDK, 16 )
#define	FDKERR_BeginScene����ĂȂ�			MAKE_HRESULT( 1, _FACFDK, 17 )
#define	FDKERR_3D�f�o�C�X��NULL				MAKE_HRESULT( 1, _FACFDK, 18 )
#define	FDKERR_�p�����[�^��NULL				MAKE_HRESULT( 1, _FACFDK, 19 )
#define	FDKERR_DIB�̍쐬�Ɏ��s				MAKE_HRESULT( 1, _FACFDK, 20 )
#define	FDKERR_�e�N�X�`����NULL				MAKE_HRESULT( 1, _FACFDK, 21 )
#define	FDKERR_������̎擾�Ɏ��s			MAKE_HRESULT( 1, _FACFDK, 22 )
#define	FDKERR_����t���[���̃I�[�v���Ɏ��s	MAKE_HRESULT( 1, _FACFDK, 23 )
#define	FDKERR_DRAWDIB�̃I�[�v���Ɏ��s		MAKE_HRESULT( 1, _FACFDK, 24 )
#define	FDKERR_DDS�̍쐬�Ɏ��s				MAKE_HRESULT( 1, _FACFDK, 25 )
#define	FDKERR_�e�N�X�`���̍쐬�Ɏ��s		MAKE_HRESULT( 1, _FACFDK, 26 )
#define	FDKERR_DirectSound�̍쐬�Ɏ��s		MAKE_HRESULT( 1, _FACFDK, 27 )
#define	FDKERR_DirectSound�̋������x���̐ݒ�Ɏ��s	MAKE_HRESULT( 1, _FACFDK, 28 )

// HRESULT �ɊY������G���[���b�Z�[�W���擾���ĕԂ��B
extern LPCTSTR HRMSG( HRESULT hr );

// GetLastError() �œ���ꂽ�G���[�l���烁�b�Z�[�W���擾���ĕԂ��B
extern LPCTSTR LASTERRMSG();
	
// �G���[���b�Z�[�W�e�L�X�g
extern tstring g_strFDKErrMsg;

	}//General
}//FDK

using namespace FDK::General;
