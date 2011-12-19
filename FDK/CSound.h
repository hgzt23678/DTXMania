
// CSound - �T�E���h�N���X

#pragma once

namespace FDK {
	namespace Sound {

enum SoundType
{
	SOUNDTYPE_UNKNOWN,
	SOUNDTYPE_WAV,
	SOUNDTYPE_XA,
	SOUNDTYPE_MP3,
	SOUNDTYPE_OGG
};

class CSound
{
public:
	void	Init();														// ������
	virtual void	Term();												// �I��

	void	Play( bool bLoop=false );									// ���t�J�n
	void	Stop();														// ���t��~
	void	Pause();													// �ꎞ��~
	void	Cont();														// ���݈ʒu�im_dwCurrentPosition�j���牉�t�ĊJ
	void	Cont( double dbContTime );									// �w�肵���������牉�t�ĊJ
	bool	IsPlay();													// �Đ����Ȃ� true

	void	SetPan( long lPan=0 );										// �p���|�b�g�̐ݒ�ilPan = -100[��]�`0[��]�`100[�E]�j
	void	SetVolume( long lVolume=100 );								// ���ʂ̐ݒ�ilVolume = 0�`100 [%]�j
	void	SetSpeed( float fSpeed=1.0f )	{ this->fSpeed = fSpeed; }	// �Đ��X�s�[�h�̐ݒ�
	void	SetPitch( float fPitch=1.0f )	{ this->fPitch = fPitch; }	// �s�b�`�i���g���{���j�̐ݒ�
	DWORD	GetFrequency();												// �T�E���h�̎��g����Ԃ��B
	DWORD	SetFrequency( DWORD dwFreq );								// �T�E���h�̎��g����ݒ肵�A�ݒ�O�̒l��Ԃ��B
	double	GetTotalTime();												// �����t����[ms]���v�Z���ĕԂ��B
	void	SetPosition( DWORD dwNewPosition );							// �w��ʒu[byte]�ֈړ�����B
	DWORD	GetPositionFromTime( double dbTime );						// ��������ʒu���擾����B

	LPDIRECTSOUNDBUFFER GetDirectSoundBuffer() { return this->pDSBuffer; }
	DWORD				GetDirectSoundBufferSize() { return this->dwDSBufferSize; }

	HRESULT	DuplicateFromSound( LPDIRECTSOUND8 pDS, CSound* srcSound );			// �����̐���
	HRESULT	CreateFromFile( LPDIRECTSOUND8 pDS, LPCTSTR filename );				// �t�@�C������T�E���h�𐶐�����
	HRESULT	CreateFromMemory( LPDIRECTSOUND8 pDS, BYTE* pData, DWORD dwSize );	// �������f�[�^����T�E���h�𐶐�����

public:
	CSound();

public:
	SoundType	m_SoundType;

protected:
	LPDIRECTSOUNDBUFFER	pDSBuffer;
	DWORD				dwDSBufferSize;
	TCHAR				strFileName[_MAX_PATH];
	long				lVolume;
	long				lPan;
	float				fSpeed;
	float				fPitch;
	bool				bLoop;
	int					nPause;
	DWORD				dwCurrentPosition;	// Pause() ���̈ʒu

protected:
	// �T�E���h�o�b�t�@�� Lost ���Ă��邩�ۂ��m�F���ALost ���Ă���Ȃ畜������B
	HRESULT	RestoreBuffer( LPDIRECTSOUNDBUFFER pDSB, BOOL* pbWasRestored );

	HRESULT	DecodeFromMP3( LPDIRECTSOUND8 pDS, BYTE* pSrcData, DWORD dwSrcSize );	// MP3 �f�[�^�̃f�R�[�h
	HRESULT	DecodeFromXA(  LPDIRECTSOUND8 pDS, BYTE* pSrcData, DWORD dwSrcSize );	// XA �f�[�^�̃f�R�[�h
	HRESULT	DecodeFromWAV( LPDIRECTSOUND8 pDS, BYTE* pSrcData, DWORD dwSrcSize );	// WAV �f�[�^�̃f�R�[�h
	HRESULT	DecodeFromOgg( LPDIRECTSOUND8 pDS, BYTE* pSrcData, DWORD dwSrcSize );	// Ogg �f�[�^�̃f�R�[�h

	// �T�E���h�o�b�t�@���쐬���A�����փf�[�^���������ށB
	HRESULT CreateAndCopyBuffer( LPDIRECTSOUND8 pDS, WAVEFORMATEX* pwfx, BYTE* pData, DWORD dwSize );
};

	}//Sound
}//FDK

using namespace FDK::Sound;
