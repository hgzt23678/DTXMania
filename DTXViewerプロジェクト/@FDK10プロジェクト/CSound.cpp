#include "stdafx.h"
#include "CSound.h"
#include "CSoundManager.h"
#include "FDKError.h"
#include "CFileMM.h"
#include "xadec.h"
#include "vorbis/vorbisfile.h"
#include "COgg.h"
#include "Debug.h"

namespace FDK {
	namespace Sound {

CSound::CSound()
{
	this->pDSBuffer = NULL;
}
void	CSound::Init()
{
	SAFE_RELEASE( this->pDSBuffer );
	this->dwDSBufferSize = 0;
	this->strFileName[0] = _T('\0');
	this->lVolume = DSBVOLUME_MAX;
	this->lPan = 0;
	this->fSpeed = 1.0f;
	this->fPitch = 1.0f;
	this->bLoop = false;
	this->nPause = 0;
	this->dwCurrentPosition = 0;
}
void	CSound::Term()
{
	Init();
}
HRESULT CSound::DuplicateFromSound( LPDIRECTSOUND8 pDS, CSound* srcSound )
{
	HRESULT hr;

	// �����`�F�b�N
	if( ! pDS )
		return FDKERR_DirectSound��NULL;
	if( ! srcSound )
		return FDKERR_�T�E���h�o�b�t�@��NULL;
	if( this->pDSBuffer )
		Term();		// ���p���Ȃ̂Ő�ɏI���������s��

	// �T�E���h�̕���
	if( FAILED( hr = pDS->DuplicateSoundBuffer( srcSound->GetDirectSoundBuffer(), &this->pDSBuffer ) ) )
		return FDKERR_�T�E���h�o�b�t�@�̍쐬�Ɏ��s;
	
	// �T�E���h�o�b�t�@�T�C�Y�̎擾
	this->dwDSBufferSize = srcSound->GetDirectSoundBufferSize();

	return S_OK;
}
HRESULT	CSound::CreateFromFile( LPDIRECTSOUND8 pDS, LPCTSTR filename )
{
	HRESULT hr;

	// �����`�F�b�N
	if( ! pDS )
		return FDKERR_DirectSound��NULL;

	// �t�@�C���̓ǂݍ���
	CFileMM file;
	if( ! file.Load( filename ) )
	{
		file.Term();
		return FDKERR_�t�@�C���̓ǂݍ��ݎ��s;
	}

	// �t�@�C���f�[�^����̃T�E���h�o�b�t�@�쐬
	if( FAILED( hr = CreateFromMemory( pDS, file.GetData(), file.GetSize() ) ) )
	{
		file.Term();
		return FDKERR_�T�E���h�o�b�t�@�̍쐬�Ɏ��s;
	}

	lstrcpyn( this->strFileName, filename, _MAX_PATH );
	file.Term();

	return S_OK;
}
HRESULT	CSound::CreateFromMemory( LPDIRECTSOUND8 pDS, BYTE* pData, DWORD dwSize )
{
	HRESULT hr;

	// �����`�F�b�N
	if( ! pData || dwSize == 0)
		return E_INVALIDARG;
	
	if( ! pDS )
		return FDKERR_DirectSound��NULL;
	
	if( this->pDSBuffer )
		Term();	// ���p���Ȃ̂ŁA��ɏI���������s��

	// �T�E���h�̃f�R�[�h
	if( FAILED( hr = DecodeFromOgg( pDS, pData, dwSize ) ) )
	if( FAILED( hr = DecodeFromXA(  pDS, pData, dwSize ) ) )
	if( FAILED( hr = DecodeFromMP3( pDS, pData, dwSize ) ) ) 
	if( FAILED( hr = DecodeFromWAV( pDS, pData, dwSize ) ) )
		return FDKERR_�T�E���h�o�b�t�@�̍쐬�Ɏ��s;		// ���T�|�[�g�f�[�^

	return hr;
}

HRESULT CSound::CreateAndCopyBuffer( LPDIRECTSOUND8 pDS, WAVEFORMATEX* pwfx, BYTE* pData, DWORD dwSize )
{
	HRESULT hr;

	this->dwDSBufferSize = dwSize;

	// �T�E���h�o�b�t�@�̐���
    DSBUFFERDESC dsbd;
    ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
	dsbd.dwSize          = sizeof(DSBUFFERDESC);
	dsbd.dwFlags         = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLFREQUENCY | DSBCAPS_LOCDEFER;
	dsbd.dwBufferBytes   = dwSize;
	dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
	dsbd.lpwfxFormat     = pwfx;
	if( FAILED( hr = pDS->CreateSoundBuffer( &dsbd, &this->pDSBuffer, NULL ) ) )
		return FDKERR_�T�E���h�o�b�t�@�̍쐬�Ɏ��s;

	// �T�E���h�o�b�t�@�� Lost �`�F�b�N
	if( FAILED( hr = RestoreBuffer( this->pDSBuffer, NULL ) ) )
		return hr;

	// �f�R�[�h�f�[�^���T�E���h�o�b�t�@�ɓ]��
	VOID* pDSLockedBuffer		= NULL;
	DWORD dwDSLockedBufferSize	= 0;
    if( FAILED( hr = this->pDSBuffer->Lock( 0, dwSize, &pDSLockedBuffer, &dwDSLockedBufferSize, NULL, NULL, DSBLOCK_ENTIREBUFFER ) ) )
		return FDKERR_Lock�Ɏ��s;
	else
	{
		CopyMemory( pDSLockedBuffer, pData, dwSize );
		this->pDSBuffer->Unlock( pDSLockedBuffer, dwDSLockedBufferSize, NULL, 0 );
	}
	return S_OK;
}

HRESULT CSound::RestoreBuffer( LPDIRECTSOUNDBUFFER pDSB, BOOL* pbWasRestored )
{
    HRESULT hr;

	// �����`�F�b�N
    if( pDSB == NULL )
        return CO_E_NOTINITIALIZED;
	
	if( pbWasRestored ) *pbWasRestored = FALSE;

	// �X�e�[�^�X�� Lost �`�F�b�N
    DWORD dwStatus;
    if( FAILED( hr = pDSB->GetStatus( &dwStatus ) ) )
        return hr;

    if( dwStatus & DSBSTATUS_BUFFERLOST )
    {
		// �T�E���h�o�b�t�@�� Lost ���Ă���̂ŕ�������B
		// �A�v���P�[�V�����͊����������΂���̏ꍇ�A�܂� DirectSound �͐��䌠��
		// �n���Ă���Ă��Ȃ����߁A�o�b�t�@�� Restore �����s���邱�Ƃ�����B
		// ���������Ȃ�����ADirectSound �����䌠�������܂ŃX���[�v����B
		while( ( hr = pDSB->Restore() ) == DSERR_BUFFERLOST )
			Sleep( 100 );

		// �������ł̓o�b�t�@�̕��������B�f�[�^�̓��e�͌Ăяo�����ŕʓr�������邱�ƁB

		if( pbWasRestored )	*pbWasRestored = TRUE;
	}
	return S_OK;
}

HRESULT	CSound::DecodeFromXA(  LPDIRECTSOUND8 pDS, BYTE* pSrcData, DWORD dwSrcSize )
{
	XAHEADER xah;
	HXASTREAM hxas;
	XASTREAMHEADER xash;
	UCHAR *ps;
	ULONG dlen;

	// �w�b�_�`�F�b�N
	CopyMemory( &xah, pSrcData, sizeof(XAHEADER) );
	if( xah.id != _XAID )
		return FDKERR_SoundType������;		// XA �ł͂Ȃ�����

	ps = (UCHAR *)pSrcData + sizeof(XAHEADER);
	WAVEFORMATEX* pwfx = (WAVEFORMATEX*) malloc( sizeof(WAVEFORMATEX) );
	hxas = xaDecodeOpen( &xah, pwfx );
	if( hxas == NULL )
		return FDKERR_SoundType������;		// XA �ł͂Ȃ�����

	// �f�R�[�h��T�C�Y�̎擾
	if( ! xaDecodeSize( hxas, xah.nDataLen, &dlen ) )
	{
		xaDecodeClose( hxas );
		return FDKERR_SoundType������;		// XA �ł͂Ȃ������H
	}

	// �f�R�[�h��T�C�Y���̃��������m��
	BYTE* pDestData = (LPBYTE) malloc( dlen );
	if( ! pDestData )
	{
		xaDecodeClose( hxas );
		return E_OUTOFMEMORY;		// �������s��
	}

	// XA �� WAV �ϊ�
	xash.pSrc = ps;
	xash.nSrcLen = xah.nDataLen;
	xash.nSrcUsed = 0;
	xash.pDst = (UCHAR *) pDestData;
	xash.nDstLen = dlen;
	xash.nDstUsed = 0;
	if( ! xaDecodeConvert( hxas, &xash ) )
	{
		xaDecodeClose( hxas );
		return FDKERR_SoundType������;		// XA �ł͂Ȃ������H
	}

	// XA �ϊ�����
	xaDecodeClose( hxas );

	// ���̕ϊ���T�C�Y�̌v�Z
//	lpSrcInfo->dwPCMSize = dlen;		// xadec �̃o�O
	DWORD dwTrueLen = xah.nSamples * xah.nChannels * 2;

	// �쐬�Ə�������
	HRESULT hr;
	if( FAILED( hr = CreateAndCopyBuffer( pDS, pwfx, pDestData, dwTrueLen ) ) )
	{
		SAFE_FREE( pwfx );
		SAFE_FREE( pDestData );
		return hr;
	}
	SAFE_FREE( pwfx );
	SAFE_FREE( pDestData );

	m_SoundType = SOUNDTYPE_XA;

	return S_OK;
}

HRESULT	CSound::DecodeFromMP3( LPDIRECTSOUND8 pDS, BYTE* pSrcData, DWORD dwSrcSize )
{
	// �w�b�_�`�F�b�N
	if( dwSrcSize <= 128 )
		return FDKERR_SoundType������;		// MP3����Ȃ�

	//  ID3v2�^�O�����Ă���Ȃ�΁A�ǂݔ�΂�
	if( (pSrcData[0] == 'I') && (pSrcData[1] == 'D') && (pSrcData[2] == '3') )
	{
		DWORD dwID3Size = pSrcData[9] + (pSrcData[8]<<7) + (pSrcData[7]<<14) + (pSrcData[6]<<21);
		if( pSrcData[3] >= 0x04)
		{
			if( pSrcData[5] & 0x10 )
				dwID3Size += 20; // ID3v2.4.0�ȍ~�E�t�b�^����
			else
				dwID3Size += 10; // ID3v2.4.0�ȍ~�E�t�b�^�Ȃ�
		}
		else
			dwID3Size += 10;	 // ID3v2.3.0�ȑO�E�t�b�^�Ȃ�
		
		if( dwSrcSize <= dwID3Size + 128 )
			return FDKERR_SoundType������;		// MP3����Ȃ�

		pSrcData += dwID3Size;
		dwSrcSize -= dwID3Size;
	}

	//	MP3�`�F�b�N
	if( pSrcData[0] != 0xff || (pSrcData[1] & 0xf0) != 0xf0 )
		return FDKERR_SoundType������;		// MP3����Ȃ�


	static const int anBitrate[2][3][16] = {
		{
			// MPEG-1
			{ 0,32,64,96,128,160,192,224,256,288,320,352,384,416,448,0 },	//	32000Hz(layer1)
			{ 0,32,48,56, 64, 80, 96,112,128,160,192,224,256,320,384,0 },	//	44100Hz(layer2)
			{ 0,32,40,48, 56, 64, 80, 96,112,128,160,192,224,256,320,0 },	//	48000Hz(layer3)
		},
		{
			// MPEG-2, 2.5
			{ 0,32,48,56, 64, 80, 96,112,128,144,160,176,192,224,256,0 },	//	32000Hz(layer1)
			{ 0, 8,16,24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160,0 },	//	44100Hz(layer2)
			{ 0, 8,16,24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160,0 },	//	48000Hz(layer3)
			},
	};
	static const int anFreq[2][4] = {
		{ 44100,48000,32000,0 },
		{ 22050,24000,16000,0 },
	};

	// ���C���[�ԍ��̃`�F�b�N
	int nLayer = 4-((pSrcData[1] >> 1) & 3);
	if( nLayer == 4 )
		return FDKERR_SoundType������;			// MP3 ����Ȃ������H
	
	int nMpeg		= ((pSrcData[1] & 8) == 0) ? 1 : 0;
	int nBitrate	= anBitrate[ nMpeg ][ nLayer-1 ][ pSrcData[2]>>4 ];
	int nFreq		= anFreq[ nMpeg ][ (pSrcData[2] >> 2) & 3];
	int nChannel	= ((pSrcData[3] >> 6) == 3) ? 1 : 2;
	int nFrameSize	= 144000 * nBitrate / nFreq;

	// MP3�̃^�O��p�ӂ���
	MPEGLAYER3WAVEFORMAT wfxMP3;
	ZeroMemory( &wfxMP3, sizeof(MPEGLAYER3WAVEFORMAT) );
	wfxMP3.wfx.cbSize			= MPEGLAYER3_WFX_EXTRA_BYTES;
	wfxMP3.wfx.wFormatTag		= WAVE_FORMAT_MPEGLAYER3;
	wfxMP3.wfx.nChannels		= nChannel;
	wfxMP3.wfx.nSamplesPerSec	= nFreq;
	wfxMP3.wfx.nAvgBytesPerSec	= nBitrate * 1000 / 8;
	wfxMP3.wfx.nBlockAlign		= 1;
	wfxMP3.wfx.wBitsPerSample	= 0;
	wfxMP3.wID					= MPEGLAYER3_ID_MPEG;
	wfxMP3.fdwFlags				= MPEGLAYER3_FLAG_PADDING_OFF;
	wfxMP3.nBlockSize			= nFrameSize;
	wfxMP3.nFramesPerBlock		= 1;
	wfxMP3.nCodecDelay			= 0x0571;

	//	ID3�^�O�����Ă���Ȃ�΁A���̕������O����
	if( (pSrcData[ dwSrcSize-128 ] == 'T') && (pSrcData[ dwSrcSize-127 ] == 'A') && (pSrcData[ dwSrcSize-126 ] == 'G') )
		dwSrcSize-= 128;

	// �\�[�X���� PCM �ɕϊ��\�� codec �����邩���ׁAwfxDest ��ݒ�
	MMRESULT mmr;
	WAVEFORMATEX wfxDest;
	wfxDest.wFormatTag = WAVE_FORMAT_PCM;
	if( ( mmr = acmFormatSuggest( NULL, (WAVEFORMATEX*)(&wfxMP3), &wfxDest, sizeof(WAVEFORMATEX), ACM_FORMATSUGGESTF_WFORMATTAG ) ) != 0 )
		return FDKERR_SoundType������;		// Error: CODEC ���Ȃ�����

	BYTE* pDestData = NULL;
	DWORD dwDestSize = 0;

	// ACM ���I�[�v������
	HACMSTREAM hAcm;
	if( ( mmr = acmStreamOpen( &hAcm, NULL, (WAVEFORMATEX*)(&wfxMP3), &wfxDest, NULL, NULL, NULL, ACM_STREAMOPENF_NONREALTIME ) ) != 0 )
		return FDKERR_SoundType������;		// Error: �I�[�v�����s

	// �ϊ���̃T�C�Y���擾���AdwDestSize �ɐݒ�
	if( ( mmr = acmStreamSize( hAcm, dwSrcSize, &dwDestSize, ACM_STREAMSIZEF_SOURCE ) ) != 0 )
	{
		acmStreamClose( hAcm, NULL );
		return FDKERR_SoundType������;		// Error: �T�C�Y�擾���s
	}
	if( dwDestSize == 0 )
	{
		acmStreamClose( hAcm, NULL );
		return FDKERR_SoundType������;		// Error: �ϊ���̃T�C�Y���[��
	}

	// PCM�o�b�t�@�̊m��
	pDestData = (LPBYTE) malloc( dwDestSize );
	if( ! pDestData ) 
	{
		acmStreamClose( hAcm, NULL );
		return E_OUTOFMEMORY;				// Error: �������s��
	}

	// �ϊ��p�w�b�_����
	ACMSTREAMHEADER	acmHeader;
	ZeroMemory( &acmHeader, sizeof(acmHeader) );
	acmHeader.cbStruct		= sizeof(acmHeader);
	acmHeader.pbSrc			= pSrcData;
	acmHeader.cbSrcLength	= dwSrcSize;
	acmHeader.pbDst			= pDestData;
	acmHeader.cbDstLength	= dwDestSize;
	if( ( mmr = acmStreamPrepareHeader( hAcm, &acmHeader, NULL ) ) != 0 )
	{
		acmStreamUnprepareHeader( hAcm, &acmHeader, NULL );
		acmStreamClose( hAcm, NULL );
		SAFE_FREE( pDestData );
		return FDKERR_SoundType������;		// ACM�w�b�_�o�^���s
	}

	// �ϊ�
	if( ( mmr = acmStreamConvert( hAcm, &acmHeader, NULL ) ) != 0 )
	{
		acmStreamUnprepareHeader( hAcm, &acmHeader, NULL );
		acmStreamClose( hAcm, NULL );
		SAFE_FREE( pDestData );
		return FDKERR_SoundType������;		// ACM�ϊ����s
	}

	// �^�̕ϊ���T�C�Y���擾
	dwDestSize = acmHeader.cbDstLengthUsed;

	// ACM �����
	acmStreamUnprepareHeader( hAcm, &acmHeader, NULL );
	acmStreamClose( hAcm, NULL );

	// �쐬�Ə�������
	HRESULT hr;
	if( FAILED( hr = CreateAndCopyBuffer( pDS, &wfxDest, pDestData, dwDestSize ) ) )
	{
		SAFE_FREE( pDestData );
		return hr;
	}
	SAFE_FREE( pDestData );
	
	m_SoundType = SOUNDTYPE_MP3;

	return S_OK;
}

HRESULT	CSound::DecodeFromWAV( LPDIRECTSOUND8 pDS, BYTE* pSrcData, DWORD dwSrcSize )
{
	MMRESULT mmr;

	// �`�F�b�N
	MMIOINFO mmio;
	ZeroMemory( &mmio, sizeof(MMIOINFO) );
	mmio.pchBuffer = (LPSTR)pSrcData;
	mmio.fccIOProc = FOURCC_MEM;
	mmio.cchBuffer = dwSrcSize;

	HMMIO hmmio;
	if( ( hmmio = mmioOpen( NULL, &mmio, MMIO_READ ) ) == NULL )	// ����������MMIO�I�[�v��
		return FDKERR_SoundType������;		// RIFF �ł͂Ȃ�����

	// WAVE�`�����N�ֈړ�
	MMCKINFO ckiParent;
	ckiParent.fccType = mmioFOURCC('W','A','V','E');
	if( ( mmr = mmioDescend( hmmio, &ckiParent, NULL, MMIO_FINDRIFF ) ) != 0 )
	{
		mmioClose( hmmio, 0 );
		return FDKERR_SoundType������;		// WAVE�`�����N�ړ����s
	}

	// WAVE��fmt�`�����N�ւ̈ړ�
	MMCKINFO ckiChild;
	ckiChild.ckid = mmioFOURCC('f','m','t',' ');
	if( ( mmr = mmioDescend( hmmio, &ckiChild, &ckiParent, MMIO_FINDCHUNK ) ) != 0 )
	{
		mmioClose( hmmio, 0 );
		return FDKERR_SoundType������;		// fmt �`�����N�ړ����s
	}

	// WAVEFORMATEX ���擾���Afmt�`�����N���甲����
	WAVEFORMATEX* pw = (WAVEFORMATEX *) (((LPBYTE)pSrcData) + ((int)mmioSeek( hmmio, 0, SEEK_CUR )) );
	if( ( mmr = mmioAscend( hmmio, &ckiChild, 0 ) ) != MMSYSERR_NOERROR )
	{
		mmioClose( hmmio, 0 );
		return FDKERR_SoundType������;		// fmt �`�����N�E�o���s
	}

	// WAVEFORMATEX ��V���� malloc �����̈�ɕ��ʂ���
	WORD wfxsize = sizeof(WAVEFORMATEX) + ( ( pw->wFormatTag != WAVE_FORMAT_PCM ) ? pw->cbSize : 0 );
	WAVEFORMATEX* pwfx = (WAVEFORMATEX*) malloc( wfxsize );
	if( ! pwfx )
	{
		mmioClose( hmmio, 0 );
		return E_OUTOFMEMORY;				// �������s��
	}
	CopyMemory( pwfx, pw, wfxsize );

	// data�`�����N�ֈړ�
	ckiChild.ckid = mmioFOURCC('d','a','t','a');
	if( ( mmr = mmioDescend( hmmio, &ckiChild, &ckiParent, MMIO_FINDCHUNK ) ) != 0 )
	{
		mmioClose( hmmio, 0 );
		SAFE_FREE( pwfx );
		return FDKERR_SoundType������;		// data�`�����N�ւ̈ړ����s
	}

	// �T�C�Y�ƃ|�C���^���擾���AMMIO�����
	BYTE* pDestData = (LPBYTE) ((LPBYTE)pSrcData + mmioSeek( hmmio, 0, SEEK_CUR ) );
	DWORD dwDestSize = ckiChild.cksize;
	mmioClose( hmmio, 0 );

	/* �����܂łŁApwfx, pDestData, dwDestSize �� RiffWAV �̓��e������ꂽ�B*/

	// WAVE �t�H�[�}�b�g�łȂ��ꍇ�� ACM �Ńf�R�[�h
	if( pwfx->wFormatTag != WAVE_FORMAT_PCM )
	{
		WAVEFORMATEX* pSrcWfx	= pwfx;
		BYTE* pSrcData			= pDestData;
		DWORD dwSrcSize			= dwDestSize;

		// �\�[�X���� PCM �ɕϊ��\�� codec �����邩���ׁAwfxDest ��ݒ�
		WAVEFORMATEX wfxDest;
		wfxDest.wFormatTag = WAVE_FORMAT_PCM;
		if( ( mmr = acmFormatSuggest( NULL, pSrcWfx, &wfxDest, sizeof(WAVEFORMATEX), ACM_FORMATSUGGESTF_WFORMATTAG ) ) != 0 )
		{
			SAFE_FREE( pSrcWfx );
			return FDKERR_SoundType������;		// CODEC ���Ȃ�����
		}


		// ACM ���I�[�v������
		HACMSTREAM hAcm;
		if( ( mmr = acmStreamOpen( &hAcm, NULL, pSrcWfx, &wfxDest, NULL, NULL, NULL, ACM_STREAMOPENF_NONREALTIME ) ) != 0 )
		{
			SAFE_FREE( pSrcWfx );
			return FDKERR_SoundType������;		// ACM�̃I�[�v���Ɏ��s
		}

		// �ϊ���̃T�C�Y���擾���AdwDestSize �ɐݒ�
		if( ( mmr = acmStreamSize( hAcm, dwSrcSize, &dwDestSize, ACM_STREAMSIZEF_SOURCE ) ) != 0 )
		{
			acmStreamClose( hAcm, NULL );
			SAFE_FREE( pSrcWfx );
			return FDKERR_SoundType������;		// �ϊ���̃T�C�Y�̎擾�Ɏ��s
		}
		if( dwDestSize == 0 )
		{
			acmStreamClose( hAcm, NULL );
			SAFE_FREE( pSrcWfx );
			return FDKERR_SoundType������;		// �ϊ���̃T�C�Y���[��
		}

		// �ϊ���t�H�[�}�b�g�̎擾���ݒ�
		pwfx = (WAVEFORMATEX*) malloc( sizeof(WAVEFORMATEX) );
		if( ! pwfx )
		{
			acmStreamClose( hAcm, NULL );
			SAFE_FREE( pSrcWfx );
			return E_OUTOFMEMORY;				// �������s��
		}
		CopyMemory( pwfx, &wfxDest, sizeof(WAVEFORMATEX) );

		// PCM�o�b�t�@�̊m��
		pDestData = (LPBYTE) malloc( dwDestSize );
		if( ! pDestData )
		{
			acmStreamClose( hAcm, NULL );
			SAFE_FREE( pSrcWfx );
			return E_OUTOFMEMORY;				// �������s��
		}

		// �ϊ��p�w�b�_����
		ACMSTREAMHEADER	acmHeader;
		ZeroMemory( &acmHeader, sizeof(acmHeader) );
		acmHeader.cbStruct		= sizeof(acmHeader);
		acmHeader.pbSrc			= pSrcData;
		acmHeader.cbSrcLength	= dwSrcSize;
		acmHeader.pbDst			= pDestData;
		acmHeader.cbDstLength	= dwDestSize;
		if( ( mmr = acmStreamPrepareHeader( hAcm, &acmHeader, NULL ) ) != 0 )
		{
			acmStreamUnprepareHeader( hAcm, &acmHeader, NULL );
			acmStreamClose( hAcm, NULL );
			SAFE_FREE( pSrcWfx );
			SAFE_FREE( pwfx );
			SAFE_FREE( pDestData );
			return FDKERR_SoundType������;		// ACM�w�b�_�o�^���s
		}

		// �ϊ�
		if( ( mmr = acmStreamConvert( hAcm, &acmHeader, NULL ) ) != 0 )
		{
			acmStreamUnprepareHeader( hAcm, &acmHeader, NULL );
			acmStreamClose( hAcm, NULL );
			SAFE_FREE( pSrcWfx );
			SAFE_FREE( pwfx );
			SAFE_FREE( pDestData );
			return FDKERR_SoundType������;		// ACM�ϊ����s
		}

		// �^�̕ϊ���T�C�Y���擾
		dwDestSize = acmHeader.cbDstLengthUsed;

		// ACM �����
		acmStreamUnprepareHeader( hAcm, &acmHeader, NULL );
		acmStreamClose( hAcm, NULL );

		// �쐬�Ə�������
		HRESULT hr;
		if( FAILED( hr = CreateAndCopyBuffer( pDS, pwfx, pDestData, dwDestSize ) ) )
		{
			SAFE_FREE( pwfx );
			SAFE_FREE( pSrcWfx );
			SAFE_FREE( pDestData );
			return hr;
		}
		SAFE_FREE( pwfx );
		SAFE_FREE( pSrcWfx );
		SAFE_FREE( pDestData );
	}
	else
	{
		// �쐬�Ə�������
		HRESULT hr;
		if( FAILED( hr = CreateAndCopyBuffer( pDS, pwfx, pDestData, dwDestSize ) ) )
		{
			SAFE_FREE( pwfx );
			return hr;
		}
		SAFE_FREE( pwfx );
	}

	m_SoundType = SOUNDTYPE_WAV;

	return S_OK;
}

HRESULT	CSound::DecodeFromOgg( LPDIRECTSOUND8 pDS, BYTE* pSrcData, DWORD dwSrcSize )
{
	COgg ogg;

	// �`�F�b�N
	if( ! ogg.Init( pSrcData, dwSrcSize ) )
		return FDKERR_SoundType������;		// Ogg ����Ȃ�

	// �f�R�[�h�o�b�t�@�̊m��
	size_t szDestDataSize = ogg.GetDestSize();
	if( szDestDataSize == 0 )
		return FDKERR_SoundType������;

	LPBYTE pDestData = (LPBYTE) malloc( szDestDataSize );
	if( ! pDestData )
	{
		ogg.Term();
		return E_OUTOFMEMORY;				// �������s��
	}
	
	// �f�R�[�h
	if( ! ogg.ConvertToWav( pDestData, szDestDataSize ) )
	{
		ogg.Term();
		SAFE_FREE( pDestData );
		return FDKERR_SoundType������;		// �ϊ����s
	}

	// WAVE�t�H�[�}�b�g�̎擾
	WAVEFORMATEX wfx;
	if( ! ogg.GetFormat( &wfx ) )
	{
		ogg.Term();
		SAFE_FREE( pDestData );
		return FDKERR_SoundType������;		// �擾���s
	}

	// �T�E���h�̍쐬�Ə�������
	HRESULT hr;
	if( FAILED( hr = CreateAndCopyBuffer( pDS, &wfx, pDestData, (DWORD)szDestDataSize ) ) )
	{
		ogg.Term();
		SAFE_FREE( pDestData );
		return hr;
	}

	// ����
	SAFE_FREE( pDestData );
	ogg.Term();

	m_SoundType = SOUNDTYPE_OGG;

	return S_OK;
}

void	CSound::Play( bool bLoop )
{
	HRESULT hr;

	// �`�F�b�N
	if( this->pDSBuffer == NULL )
		return;		// �Z�J���_���o�b�t�@������

	// �T�E���h�o�b�t�@�̃��X�g�A�`�F�b�N
	BOOL bRestored = FALSE;
    if( FAILED( hr = this->RestoreBuffer( this->pDSBuffer, &bRestored ) ) )
		return;		// �T�E���h�o�b�t�@�̕����Ɏ��s
	if( bRestored )
	{
		Term();
		TCHAR filename[_MAX_PATH];
		lstrcpyn( filename, this->strFileName, _MAX_PATH );
		if( FAILED( hr = CreateFromFile( CSoundManager::GetDirectSound(), filename ) ) )
			return;	// �T�E���h�o�b�t�@�̕����Ɏ��s
	}

	// �Đ��J�n�ʒu�̌���
	DWORD dwStartPos = (m_SoundType == SOUNDTYPE_MP3) ? GetPositionFromTime( CSoundManager::dbMP3�Đ��x������ms ) : 0;

	// �Đ��J�[�\��������
	DWORD dwStatus;
	this->pDSBuffer->SetCurrentPosition( dwStartPos );
	this->pDSBuffer->GetStatus( &dwStatus );
	if( dwStatus & DSBSTATUS_PLAYING )
	{
		this->pDSBuffer->Stop();
		this->pDSBuffer->SetCurrentPosition( dwStartPos );
	}

	// �Đ�
	DWORD dwFreq;
	this->pDSBuffer->SetFrequency( DSBFREQUENCY_ORIGINAL );
	this->pDSBuffer->GetFrequency( &dwFreq );
	this->pDSBuffer->SetFrequency( (DWORD)(dwFreq * this->fSpeed * this->fPitch ) );
	this->pDSBuffer->SetVolume( this->lVolume );
	this->pDSBuffer->SetPan( this->lPan );
	this->pDSBuffer->Play( 0, 0, (bLoop) ? DSBPLAY_LOOPING : 0 );

	this->bLoop = bLoop;
	this->nPause = 0;
}

void	CSound::Stop()
{
	if( this->pDSBuffer )
	{
		this->pDSBuffer->Stop();
		this->pDSBuffer->SetCurrentPosition( 0 );
		this->nPause = 0;
	}
}

void	CSound::Pause()
{
	if( this->pDSBuffer && IsPlay() )
	{
		this->pDSBuffer->GetCurrentPosition( &this->dwCurrentPosition, NULL );
		this->pDSBuffer->Stop();
		this->nPause ++;
	}
}

void	CSound::Cont()
{
	if( this->nPause == 0 )
		return;
	this->nPause --;

	HRESULT hr;

	// �`�F�b�N
	if( this->pDSBuffer == NULL )
		return;	// �Z�J���_���o�b�t�@������

	// �T�E���h�o�b�t�@�̃��X�g�A�`�F�b�N
	BOOL bRestored = FALSE;
    if( FAILED( hr = RestoreBuffer( this->pDSBuffer, &bRestored ) ) )
		return;	// �T�E���h�o�b�t�@�̕����Ɏ��s
	if( bRestored )
	{
		Term();
		TCHAR	filename[_MAX_PATH];
		lstrcpyn( filename, this->strFileName, _MAX_PATH );
		if( FAILED( hr = CreateFromFile( CSoundManager::GetDirectSound(), filename ) ) )
			return;	// �T�E���h�o�b�t�@�̕����Ɏ��s
	}

	// �Đ��J�n�ʒu�̌���
	DWORD dwStartPos = this->dwCurrentPosition;

	// �Đ��J�[�\��������
	DWORD dwStatus;
	this->pDSBuffer->SetCurrentPosition( dwStartPos );
	this->pDSBuffer->GetStatus( &dwStatus );
	if( dwStatus & DSBSTATUS_PLAYING )
	{
		this->pDSBuffer->Stop();
		this->pDSBuffer->SetCurrentPosition( dwStartPos );
	}

	// �Đ�
	DWORD dwFreq;
	this->pDSBuffer->SetFrequency( DSBFREQUENCY_ORIGINAL );
	this->pDSBuffer->GetFrequency( &dwFreq );
	this->pDSBuffer->SetFrequency( (DWORD)(dwFreq * this->fSpeed * this->fPitch ) );
	this->pDSBuffer->SetVolume( this->lVolume );
	this->pDSBuffer->SetPan( this->lPan );
	this->pDSBuffer->Play( 0, 0, (this->bLoop) ? DSBPLAY_LOOPING : 0 );
}

void	CSound::Cont( double dbContTime )
{
	// �J�[�\���ړ�(this->dwCurrentPosition���ړ�)
	SetPosition( GetPositionFromTime( dbContTime ) );
	
	// this->dwCurrentPosition ����ĊJ
	Cont();
}

void	CSound::SetPosition( DWORD dwNewPosition )
{
	if( ! this->pDSBuffer )
		return;

	// �u���b�N���E�ɂ��낦��
	DWORD dwSize;
	this->pDSBuffer->GetFormat( NULL, 0, &dwSize );
	WAVEFORMATEX* pWF = (LPWAVEFORMATEX) malloc( dwSize );
	this->pDSBuffer->GetFormat( pWF, dwSize, NULL );
	dwNewPosition -= dwNewPosition % pWF->nBlockAlign;

	// �J�[�\�����ړ�
	DSBCAPS dsbc;
	ZeroMemory( &dsbc, sizeof(dsbc) );
	dsbc.dwSize = sizeof(DSBCAPS);
	this->pDSBuffer->GetCaps( &dsbc );
	if( dwNewPosition < dsbc.dwBufferBytes )
		this->pDSBuffer->SetCurrentPosition( dwNewPosition );

	this->dwCurrentPosition = dwNewPosition;	// �ꉞ���f...
	free( pWF );
}

DWORD	CSound::GetPositionFromTime( double dbTime )
{
	if( ! this->pDSBuffer )
		return 0;

	// ���g���X�s�[�h������
	dbTime = dbTime * this->fSpeed * this->fPitch;

	// MP3 �Ȃ�AdwTime �ɒx�����Ԃ����Z�B
	if( m_SoundType == SOUNDTYPE_MP3 ) 
		dbTime += CSoundManager::dbMP3�Đ��x������ms;

	// dbTime [ms] ���� dwCurPos [byte] ���Z�o
	DWORD dwCurPos, dwSize;
	this->pDSBuffer->GetFormat( NULL, 0, &dwSize );
	WAVEFORMATEX* pWF = (LPWAVEFORMATEX) malloc( dwSize );
	this->pDSBuffer->GetFormat( pWF, dwSize, NULL );
	dwCurPos = (DWORD)(dbTime * 0.001 * (pWF->nSamplesPerSec/* * this->fSpeedw*/) * pWF->nBlockAlign);	//pWF->nAvgBytesPerSec �͏������l��Ԃ����Ƃ�����A�v�Z������ł̓I�[�o�[�t���[����̂Œ���

	// �u���b�N���E�ɂ��낦��
	dwCurPos -= dwCurPos % pWF->nBlockAlign;

	free( pWF );
	return dwCurPos;
}

void	CSound::SetVolume( long lVolume )
{
	if( lVolume == 0 )
		lVolume = 1;		// Vol = 0 �͂Ȃ�������100%�ɂȂ��Ă��܂�����...

	this->lVolume = (long)(( 20.0 * log10( lVolume / 100.0 ) ) * 100.0);

	// ���t���Ȃ瑦���f
	if( IsPlay() )
		this->pDSBuffer->SetVolume( this->lVolume );
}

void	CSound::SetPan( long lPan )
{
	if( lPan == 0 ) this->lPan = 0;
	else if( lPan == -100 ) this->lPan = DSBPAN_LEFT;
	else if( lPan ==  100 ) this->lPan = DSBPAN_RIGHT;
	else if( lPan < 0 ) this->lPan = (long)( (20.0 * log10((lPan+100)/100.0)) * 100.0 );
	else this->lPan = (long)( (-20.0 * log10((100-lPan)/100.0)) * 100.0 );

	// ���t���Ȃ瑦���f
	if( IsPlay() )
		this->pDSBuffer->SetPan( lPan );
}

bool	CSound::IsPlay()
{
	if( ! this->pDSBuffer )
		return false;

	DWORD dwStatus;
	this->pDSBuffer->GetStatus( &dwStatus );
	return dwStatus & DSBSTATUS_PLAYING;
}

DWORD	CSound::GetFrequency()
{
	if( ! this->pDSBuffer )
		return 0;

	DWORD dwFreq;
	this->pDSBuffer->GetFrequency( &dwFreq );
	return dwFreq;
}

DWORD	CSound::SetFrequency( DWORD dwFreq )
{
	if( ! this->pDSBuffer )
		return 0;

	DWORD dwOldFreq;
	this->pDSBuffer->GetFrequency( &dwOldFreq );
	this->pDSBuffer->SetFrequency( dwFreq );
	return dwOldFreq;
}

double	CSound::GetTotalTime()
{
	if( this->pDSBuffer == NULL )
		return 0.0;

	// WAVEFORMATEX �擾
	DWORD dwSize;
	this->pDSBuffer->GetFormat( NULL, 0, &dwSize );
	LPWAVEFORMATEX pWF = (LPWAVEFORMATEX) malloc( dwSize );
	this->pDSBuffer->GetFormat( pWF, dwSize, NULL );

	// �����Ԃ̌v�Z
	double dbTotalTime = (double)( this->dwDSBufferSize / ( pWF->nAvgBytesPerSec * 0.001 ) );

	free( pWF );
	return dbTotalTime;
}

	}//Sound
}//FDK