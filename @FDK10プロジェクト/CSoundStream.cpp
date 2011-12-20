#include "stdafx.h"
#include "CSoundStream.h"
#include "FDKError.h"
#include "CFileMM.h"
#include "xadec.h"
#include "vorbis/vorbisfile.h"
#include "COgg.h"

namespace FDK {
	namespace Sound {

CSoundStream::CSoundStream()
{
	this->pDSBuffer = NULL;
}
//
void CSoundStream::Init()
{
	SAFE_RELEASE( this->pDSBuffer );
	this->strFileName.clear();
	this->b�Ō�ɏ������񂾃o�b�t�@ = true;		// �㔼
	this->b�Đ��� = false;
	this->soundType = SOUNDTYPE_UNKNOWN;
	this->l���݂̉��� = 0;
	this->bLoop = false;
	this->n�񃋁[�v���̖����Đ��� = 0;
	this->fSpeed = 1.0f;
	this->fPitch = 1.0f;
	this->nPause = 0;
	this->dwCurrentPosition = 0;
}
void CSoundStream::Term()
{
	switch( this->soundType )
	{
	case SOUNDTYPE_OGG:
		this->ogg.Term();
		break;
	}
	this->Stop();
	this->file.Term();
	Init();
}
void CSoundStream::Play( bool bLoop )
{
	if( this->pDSBuffer )
	{
		switch( this->soundType )
		{
		case SOUNDTYPE_OGG:
			this->t���̃u���b�N��ϊ����ăo�b�t�@�ɃZ�b�g����_OGG();
			break;
		}
		this->pDSBuffer->SetVolume( this->l���݂̉��� );
		this->pDSBuffer->Play( 0, 0, DSBPLAY_LOOPING );
		this->b�Đ��� = true;
		this->bLoop = bLoop;
		this->n�񃋁[�v���̖����Đ��� = 0;
	}
}
void CSoundStream::Stop()
{
	if( this->b�Đ��� && this->pDSBuffer )
	{
		this->pDSBuffer->Stop();
		this->b�Đ��� = false;
	}
}
void CSoundStream::Pause()
{
	// ������
}
void CSoundStream::Cont()
{
	// ������
}
void CSoundStream::Cont( double dbContTime )
{
	// ������
}
void CSoundStream::SetVolume( long lVolume )
{
	if( lVolume == 0 )
		lVolume = 1;		// Vol = 0 �͂Ȃ�������100%�ɂȂ��Ă��܂�����...

	this->l���݂̉��� = (long)(( 20.0 * log10( lVolume / 100.0 ) ) * 100.0);

	if( this->b�Đ��� )
		this->pDSBuffer->SetVolume( this->l���݂̉��� );	// ���t���Ȃ瑦���f
}
void CSoundStream::SetPosition( DWORD dwNewPosition )
{
	// ������
}
DWORD CSoundStream::GetPositionFromTime( double dbTime )
{
	// ������
	return 0;
}
void CSoundStream::StreamWork()
{
	if( ! this->pDSBuffer || ! this->b�Đ��� ) 
		return;

	DWORD dw���t�ʒu, dw�������݈ʒu;
	if( FAILED( this->pDSBuffer->GetCurrentPosition( &dw���t�ʒu, &dw�������݈ʒu ) ) )
		return;

	switch( this->soundType )
	{
	case SOUNDTYPE_OGG:
		if( ( dw���t�ʒu <  this->wfx.nAvgBytesPerSec && ! this->b�Ō�ɏ������񂾃o�b�t�@ )
		 || ( dw���t�ʒu >= this->wfx.nAvgBytesPerSec &&   this->b�Ō�ɏ������񂾃o�b�t�@ ) )
		{
			if( this->ogg.bEOF )
			{
				if( this->bLoop ) {
					this->ogg.RawSeek( 0 );		// �ŏ����烋�[�v�Đ�
					this->ogg.bEOF = false;
					this->t���̃u���b�N��ϊ����ăo�b�t�@�ɃZ�b�g����_OGG();
				}
				else if( ++ this->n�񃋁[�v���̖����Đ��� >= 2 )		// ���[�v���Ȃ��Ƃ��́AEOF �ɂȂ��Ă���Q��o�b�t�@��ʉ߂�����Đ����~����B
					this->Stop();										// EOF ����͂܂����t���I����ĂȂ��A��~�ł��Ȃ����߁B
				else
					this->t���̃u���b�N��ϊ����ăo�b�t�@�ɃZ�b�g����_OGG();
			}
			else
				this->t���̃u���b�N��ϊ����ăo�b�t�@�ɃZ�b�g����_OGG();
		}
		break;
	}
}
HRESULT	CSoundStream::CreateFromFile( LPDIRECTSOUND8 pDS, LPCTSTR filename )
{
	HRESULT hr = S_OK;
	
	if( ! pDS )
		return FDKERR_DirectSound��NULL;
	
	// �t�@�C���̓ǂݍ���
	if( ! this->file.Load( filename ) )
	{
		this->file.Term();
		return FDKERR_�t�@�C���̓ǂݍ��ݎ��s;
	}
	
	// �t�@�C���f�[�^����̃T�E���h�o�b�t�@�쐬
	if( FAILED( hr = CreateFromMemory( pDS, file.GetData(), file.GetSize() ) ) )
	{
		this->file.Term();
		return FDKERR_�T�E���h�o�b�t�@�̍쐬�Ɏ��s;
	}

	this->strFileName = filename;
	return hr;
}
HRESULT	CSoundStream::CreateFromMemory( LPDIRECTSOUND8 pDS, BYTE* pData, DWORD dwSize )
{
	HRESULT hr = S_OK;

	if( ! pData || dwSize == 0 )
		return E_INVALIDARG;

	if( ! pDS )
		return FDKERR_DirectSound��NULL;

	if( this->pDSBuffer )
		Term();	// ���p�� �� ��ɏI���������s��

	// �T�E���h�̎��ʂƃT�E���h�o�b�t�@�̍쐬
	if( FAILED( hr = this->t�T�E���h�̎��ʂƃT�E���h�o�b�t�@�̍쐬_OGG( pDS, pData, dwSize ) ) )
//	if( FAILED( hr = this->t�T�E���h�̎��ʂƃT�E���h�o�b�t�@�̍쐬_XA(  pDS, pData, dwSize ) ) )
//	if( FAILED( hr = this->t�T�E���h�̎��ʂƃT�E���h�o�b�t�@�̍쐬_WAV( pDS, pData, dwSize ) ) )
//	if( FAILED( hr = this->t�T�E���h�̎��ʂƃT�E���h�o�b�t�@�̍쐬_MP3( pDS, pData, dwSize ) ) )
		return FDKERR_�T�E���h�o�b�t�@�̍쐬�Ɏ��s;		// ���T�|�[�g�f�[�^

	return hr;
}
HRESULT	CSoundStream::t�T�E���h�̎��ʂƃT�E���h�o�b�t�@�̍쐬_OGG( LPDIRECTSOUND8 pDS, BYTE* pData, DWORD dwSize )
{
	HRESULT hr = S_OK;

	// �`�F�b�N
	if( ! this->ogg.Init( pData, dwSize ) )
		return FDKERR_SoundType������;		// Ogg ����Ȃ�

	// WAVEFORMATEX �̎擾
	if( ! this->ogg.GetFormat( &this->wfx ) )
	{
		this->ogg.Term();
		return FDKERR_SoundType������;		// �擾���s
	}

	// �T�E���h�o�b�t�@�̍쐬
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
	dsbd.dwSize          = sizeof(DSBUFFERDESC);
	dsbd.dwFlags         = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLFREQUENCY;
	dsbd.dwBufferBytes   = this->wfx.nAvgBytesPerSec * 2;		// �Q�b��
	dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
	dsbd.lpwfxFormat     = &this->wfx;
	if( FAILED( hr = pDS->CreateSoundBuffer( &dsbd, &this->pDSBuffer, NULL ) ) )
		return FDKERR_�T�E���h�o�b�t�@�̍쐬�Ɏ��s;

	this->soundType = SOUNDTYPE_OGG;
	return S_OK;
}
bool	CSoundStream::t���̃u���b�N��ϊ����ăo�b�t�@�ɃZ�b�g����_OGG()
{
	LPBYTE  p�������ݐ�;
	DWORD	dw�������݃o�C�g��;
	
	if( this->b�Ō�ɏ������񂾃o�b�t�@ )
	{
		// �O�������b�N
		if( FAILED( this->pDSBuffer->Lock( 0, this->wfx.nAvgBytesPerSec, (void**)&p�������ݐ�, &dw�������݃o�C�g��, NULL, NULL, 0 ) ) )
			return false;	// ���b�N���s
	}
	else
	{
		// �㔼�����b�N
		if( FAILED( this->pDSBuffer->Lock( this->wfx.nAvgBytesPerSec, this->wfx.nAvgBytesPerSec, (void**)&p�������ݐ�, &dw�������݃o�C�g��, NULL, NULL, 0 ) ) )
			return false;	// ���b�N���s
	}
	
	this->ogg.ConvertToWav( p�������ݐ�, dw�������݃o�C�g�� );
	
	this->pDSBuffer->Unlock( p�������ݐ�, dw�������݃o�C�g��, NULL, 0 );
	this->b�Ō�ɏ������񂾃o�b�t�@ = ! this->b�Ō�ɏ������񂾃o�b�t�@;

	return true;
}
	}//Sound
}//FDK
