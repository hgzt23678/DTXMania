#pragma once

#include "CSound.h"
#include "CSoundStream.h"
#include "CSet.h"
#include "CSetString.h"

namespace FDK {
	namespace Sound {

class CSoundManager
{
public:
	HRESULT	Init( HWND hWnd, DWORD dwCoopLevel=DSSCL_PRIORITY );	// ������
	void	Term();									// �I������

	void	t�X�g���[���T�E���h�̍Đ�����();		// �X�g���[���T�E���h���g���ꍇ�̓��C�����[�`������p���I�ɌĂяo�����ƁB

	static double	dbMP3�Đ��x������ms;

	//-----------------------------------------
	// �L���b�V�����Ή��T�E���h

	CSound*			p�L���b�V������Ȃ��T�E���h�𐶐����ĕԂ�( LPCTSTR filename );					// �g���I������玩���� delete ���邱�ƁB

	CSoundStream*	p�L���b�V������Ȃ��X�g���[���T�E���h�𐶐����ĕԂ�( LPCTSTR filename );		// ������ delete ���Ȃ����ƁB
	void			t�L���b�V������Ȃ��X�g���[���T�E���h���폜����( CSoundStream* pSoundStream );
	
	//-----------------------------------------
	// �L���b�V���Ή��T�E���h

	void	t�g�p�t���O���N���A�������Z�����폜����();								// STEP1
	CSound*	p�L���b�V���Ή��T�E���h���쐬���ĕԂ�( LPCTSTR filename );				// STEP2
	void	t���g�p�T�E���h���폜����();											// STEP3

	//-----------------------------------------
	// ���̑�

	// �v���C�}���o�b�t�@�̃t�H�[�}�b�g��ݒ�/�擾����B(��: SetPrimaryBufferFormat( 2, 22050, 16 ); )
	HRESULT SetPrimaryBufferFormat( DWORD dwPrimaryChannels, DWORD dwPrimaryFreq, DWORD dwPrimaryBitRate );
	HRESULT GetPrimaryBufferFormat( DWORD* pdwPrimaryChannels, DWORD* pdwPrimaryFreq, DWORD* pdwPrimaryBitRate );

	static LPDIRECTSOUND8	GetDirectSound() { return CSoundManager::pDS; }

	CSoundManager();
	virtual ~CSoundManager();

protected:
	static int				n�C���X�^���X��;	// CSoundManager ������邲�ƂɂP���Z
	static LPDIRECTSOUND8	pDS;				// DirectSound

	struct SoundList {
		bool		b�g�p����;
		bool		b�����ł���;
		TCHAR		str�t�@�C����[_MAX_PATH];
		FILETIME	ft�ŏI�X�V����;
		CSound*		pSound;
		SoundList	*prev, *next;
	} *soundList, *soundListLast;

	CSet<CSoundStream>	setSoundStream;			// CreateSoundStreamFromFile() �ō��ꂽ�X�g���[���T�E���h�̏W���B

protected:
	// g_SampleMP3[] ����͂��AMP3��DECODE�ɉ� ms �����邩���߂� dbMP3�Đ��x������ms �Ɋi�[����B
	void tMP3�x�����Ԃ��v�Z���L������();
};

	}//Sound
}//FDK

using namespace FDK::Sound;
