#include "stdafx.h"
#include "vorbis/vorbisfile.h"
#include "COgg.h"

namespace FDK {
	namespace Sound {

static const WORD	s_bps = 2;	// byte / sample; �Œ�l

COgg::COgg()
{
	this->bOpened		= false;

	this->pOggData		= NULL;
	this->dwTotalSize	= 0;
	this->dwCurrentPos	= 0;

	this->vi = NULL;
}


bool	COgg::Init( LPBYTE pOggRawData, DWORD dwOggRawDataSize )
{
	if( ! pOggRawData )
		return false;	// �f�[�^�|�C���^��NULL

	if( this->bOpened )
		return false;	// ���ɃI�[�v���ς�

	this->pOggData		= pOggRawData;
	this->dwTotalSize	= dwOggRawDataSize;
	this->dwCurrentPos	= 0;

	// ogg �̃������C���[�W����̃R�[���o�b�N�t���I�[�v��
	static ov_callbacks s_callbackFunctions = {
		COgg::read,
		COgg::seek,
		COgg::close,
		COgg::tell
	};

	if( ov_open_callbacks( (void*)this, &this->vf, (char*)pOggRawData, (long)dwOggRawDataSize, s_callbackFunctions ) < 0 ) 
	{
		//Debug::OutFN( FNAME, _T("ov_open() �Ɏ��s���܂����B\n") );		�w�b�_�`�F�b�N�����˂Ă�̂ŃG���[���b�Z�[�W�͏o���Ȃ��B
		return false;
	}

	// ogg �̏��擾
	if( ( this->vi = ov_info( &this->vf, -1 ) ) == NULL )
	{
		ov_clear( &this->vf );
		//Debug::OutFN( FNAME, _T("ov_info() �Ɏ��s���܂����B\n") );		�w�b�_�`�F�b�N�����˂Ă�̂ŃG���[���b�Z�[�W�͏o���Ȃ��B
		return false;
	}

	this->bOpened = true;
	this->bEOF = false;
	return true;
}
size_t	COgg::GetDestSize()
{
	if( ! this->bOpened )
		return 0;	// �I�[�v������ĂȂ�

	_ASSERT( this->vi );

	// �f�R�[�h��̃T�C�Y���v�Z
	double dbDestSize = ceil( this->vi->channels * this->vi->rate * ov_time_total( &this->vf, -1) * s_bps );
	if( dbDestSize < 0.0 )
	{
		ov_clear( &this->vf );
		return 0;	// ���s
	}
	return (size_t)dbDestSize;
}

bool	COgg::ConvertToWav( LPBYTE pDestBuf, size_t szDestBufSize )
{
	if( ! pDestBuf || szDestBufSize <= 0 )
		return false;	// �p�����[�^���s��

	if( this->bEOF )
		return true;	// ���ł� EOF ���B����

	// �f�R�[�h�G
	// �P��ł��ׂĂ��f�R�[�h�����킯�ł͂Ȃ����Ƃɒ��ӁB�i�P��ɂ� 256�`4096 bytes�炵���j
	long lDecodedSize = 0;
	int  nCurrent = 0;
	while( szDestBufSize > (size_t)lDecodedSize )
	{
		long ret = ov_read(
			&this->vf,								// OggVobis_File
			(char*)(pDestBuf + lDecodedSize),		// �o�b�t�@�G�������ވʒu
			(int)(szDestBufSize - lDecodedSize),	// �o�b�t�@�̃T�C�Y
			0,										// �r�b�O�G���f�B�A���Ȃ� 1 �ɂ���ix86 �� 0�j
			s_bps,									// �o�̓f�[�^�̗ʎq���r�b�g�����o�C�g�P�ʂŎw�肷��B��F16bit�ʎq���̏ꍇ��2�B
			1,										// �o�̓f�[�^�̕����`���B0:�����Ȃ��A1:��������BDirectSound �ł́A8bit�Ȃ�unsigned, 16bit�Ȃ�signed �ƂȂ�B
			&nCurrent );							// �_���r�b�g�X�g���[���̃|�C���^�B

		if( ret < 0 )
			return false;	// ogg �̃f�R�[�h���ɃG���[������
		else if( ret == 0 )
		{
			// eof �ɓ��B�������܂��������݃o�b�t�@���]���Ă���Ώ�������������
			if( szDestBufSize > (size_t)lDecodedSize )
				ZeroMemory( (void*)(pDestBuf + lDecodedSize), (int)(szDestBufSize - lDecodedSize) );
			this->bEOF = true;
			break;
		}

		lDecodedSize += ret;
	}
	return true;
}

bool	COgg::GetFormat( WAVEFORMATEX* pwfx )
{
	if( ! pwfx )
		return false;	// �p�����[�^���s��

	if( ! this->bOpened )
		return false;	// ogg ���I�[�v������ĂȂ�

	ZeroMemory( pwfx, sizeof( WAVEFORMATEX ) );
	pwfx->cbSize				= 0;
	pwfx->wFormatTag			= WAVE_FORMAT_PCM;
	pwfx->nChannels				= this->vi->channels;
	pwfx->nSamplesPerSec		= this->vi->rate;
	pwfx->nAvgBytesPerSec		= this->vi->rate * this->vi->channels * s_bps;
	pwfx->nBlockAlign			= this->vi->channels * s_bps;
	pwfx->wBitsPerSample		= s_bps * 8;
	return true;
}

void	COgg::Term()
{
	if( this->bOpened )
	{
		ov_clear( &this->vf );
		this->bOpened = false;
	}
}

void	COgg::RawSeek( long pos )
{
	if( ! this->bOpened )
		return;	// ogg ���I�[�v������ĂȂ�

	ov_raw_seek( &this->vf, pos );
}
void	COgg::PcmSeek( long pos )
{
	if( ! this->bOpened )
		return;	// ogg ���I�[�v������ĂȂ�

	ov_pcm_seek( &this->vf, pos );
}
size_t	COgg::read( void* ptr, size_t size, size_t nmemb, void* datasource )
{
	if( ! datasource || ! ptr )
		return 0;	// EOF; �p�����[�^���s��

	COgg* pOgg = (COgg*)datasource;
	_ASSERT( pOgg->pOggData );

	if( pOgg->dwCurrentPos >= pOgg->dwTotalSize )
		return 0;	// EOF

	size_t	readsize = size * nmemb;
	if( pOgg->dwCurrentPos + readsize > pOgg->dwTotalSize )
		readsize = pOgg->dwTotalSize - pOgg->dwCurrentPos;

	memcpy( ptr, pOgg->pOggData + pOgg->dwCurrentPos, readsize );
	pOgg->dwCurrentPos += (DWORD) readsize;

	return readsize;
}

int		COgg::seek( void* datasource, ogg_int64_t offset, int whence )
{
	_ASSERT( datasource );
	
	COgg* pOgg = (COgg*)datasource;

	switch( whence )
	{
	// a. �ŏ��̈ʒu����
	case SEEK_SET:
		if( offset < 0 )
			pOgg->dwCurrentPos = 0;
		else
		{
			pOgg->dwCurrentPos = (DWORD) offset;
			if( pOgg->dwCurrentPos > pOgg->dwTotalSize )
				pOgg->dwCurrentPos = pOgg->dwTotalSize;
		}
		break;

	// b. �Ō�̈ʒu����
	case SEEK_END:
		if( offset > 0 )
			pOgg->dwCurrentPos = pOgg->dwTotalSize;
		else if( -offset > pOgg->dwTotalSize )
			pOgg->dwCurrentPos = 0;
		else
			pOgg->dwCurrentPos = (DWORD)(pOgg->dwTotalSize + offset);
		break;

	// c. ���݂̈ʒu����
	case SEEK_CUR:
		if( pOgg->dwCurrentPos + offset > pOgg->dwTotalSize )
			pOgg->dwCurrentPos = pOgg->dwTotalSize;
		else if( pOgg->dwCurrentPos < -offset )
			pOgg->dwCurrentPos = 0;
		else
			pOgg->dwCurrentPos = (DWORD)(pOgg->dwCurrentPos + offset);
		break;

	default:
		return -1;	// ���s
	}

	return 0;	// ����
}

int		COgg::close( void* datasource )
{
	_ASSERT( datasource );
	COgg* pOgg = (COgg*)datasource;
	
	// ���ɉ������Ȃ�
	
	return 0;
}

long	COgg::tell( void *datasource )
{
	_ASSERT( datasource );
	COgg* pOgg = (COgg*)datasource;

	return pOgg->dwCurrentPos;
}

	}//Sound
}//FDK