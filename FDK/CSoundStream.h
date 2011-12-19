
// CSoundStream - �X�g���[���Ή��T�E���h
//   Play() �̂��ƁA���C�����[�`������ StreamWork() ���p���I�ɌĂяo�����ƁB
//   �X�g���[���Ή����ĂȂ��T�E���h�� CreateFrom�c() �Ŏ��s����̂Ŕ��ʂł���B

#pragma once

#include "CFileMM.h"
#include "COgg.h"
#include "CSound.h"

namespace FDK {
	namespace Sound {

class CSoundStream 
{
public:
	CSoundStream();
	void	Init();				// ������
	void	Term();				// �I��

	void	Play( bool bLoop=false );
	void	Stop();
	void	Pause();
	void	Cont();														// ���݈ʒu�im_dwCurrentPosition�j���牉�t�ĊJ
	void	Cont( double dbContTime );									// �w�肵���������牉�t�ĊJ
	void	SetVolume( long lVolume=100 );			// ���ʂ̐ݒ�ilVolume = 0�`100 [%]�j
	void	SetPosition( DWORD dwNewPosition );							// �w��ʒu[byte]�ֈړ�����B
	DWORD	GetPositionFromTime( double dbTime );						// ��������ʒu���擾����B
	void	SetSpeed( float fSpeed=1.0f )	{ this->fSpeed = fSpeed; }		// �Đ��X�s�[�h�̐ݒ�
	void	SetPitch( float fPitch=1.0f )	{ this->fPitch = fPitch; }		// �s�b�`�̐ݒ�
	void	StreamWork();
	bool	b�Đ���;

	HRESULT	CreateFromFile( LPDIRECTSOUND8 pDS, LPCTSTR filename );				// �t�@�C������T�E���h�𐶐�����
	HRESULT	CreateFromMemory( LPDIRECTSOUND8 pDS, BYTE* pData, DWORD dwSize );	// �������f�[�^����T�E���h�𐶐�����i�ďo������������f�[�^�͉�����Ȃ����Ɓj

protected:
	LPDIRECTSOUNDBUFFER	pDSBuffer;
	tstring				strFileName;
	bool				b�Ō�ɏ������񂾃o�b�t�@;		// false=�O��, true=�㔼
	long				l���݂̉���;
	float				fSpeed;
	float				fPitch;
	bool				bLoop;
	int					nPause;
	DWORD				dwCurrentPosition;	// Pause() ���̈ʒu
	SoundType			soundType;
	CFileMM				file;
	WAVEFORMATEX		wfx;
	int					n�񃋁[�v���̖����Đ���;
	COgg				ogg;

protected:
	HRESULT	t�T�E���h�̎��ʂƃT�E���h�o�b�t�@�̍쐬_OGG( LPDIRECTSOUND8 pDS, BYTE* pData, DWORD dwSize );
	bool	t���̃u���b�N��ϊ����ăo�b�t�@�ɃZ�b�g����_OGG();
//	HRESULT	t�T�E���h�̎��ʂƃT�E���h�o�b�t�@�̍쐬_MP3( LPDIRECTSOUND8 pDS, BYTE* pData, DWORD dwSize );
//	HRESULT	t�T�E���h�̎��ʂƃT�E���h�o�b�t�@�̍쐬_WAV( LPDIRECTSOUND8 pDS, BYTE* pData, DWORD dwSize );
//	HRESULT	t�T�E���h�̎��ʂƃT�E���h�o�b�t�@�̍쐬_XA(  LPDIRECTSOUND8 pDS, BYTE* pData, DWORD dwSize );
};

	}//Sound
}//FDK

using namespace FDK::Sound;
