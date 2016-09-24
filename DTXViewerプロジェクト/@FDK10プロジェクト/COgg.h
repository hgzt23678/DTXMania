#pragma once

#include "vorbis/vorbisfile.h"

namespace FDK {
	namespace Sound {

class COgg
{
public:
	bool	bEOF;		// �\�[�X�f�[�^��EOF�ɒB������true

public:
	bool	Init( LPBYTE pOggRawData, DWORD dwOggRawDataSize );		// Ogg�������C���[�W����̃I�[�v��
	void	Term();													// �I������
	size_t	GetDestSize();											// �ϊ���ɕK�v�ȃo�b�t�@�̃T�C�Y�𓾂�B
	bool	ConvertToWav( LPBYTE pDestBuf, size_t szDestBufSize );	// Wave �֕ϊ�����B
	bool	GetFormat( WAVEFORMATEX* pwfx );						// WAV�T�E���h�t�H�[�}�b�g��ݒ肵�ĕԂ��B
	void	RawSeek( long pos );
	void	PcmSeek( long pos );
	COgg();

public:
	static size_t	read( void* ptr, size_t size, size_t nmemb, void* datasource );
	static int		seek( void* datasource, ogg_int64_t offset, int whence );
	static int		close( void* datasource );
	static long		tell( void *datasource );

protected:
	bool	bOpened;			// ����Open���im_vf, m_vi ���L���j
	LPBYTE	pOggData;			// Ogg���f�[�^�ւ̃|�C���^�i���̃N���X�ł̓������Ǘ��͍s��Ȃ��j
	DWORD	dwTotalSize;		// Ogg���f�[�^�̑��T�C�Y(bytes); �R���X�g���N�^�� dwOggRawDataSize ��ێ�
	DWORD	dwCurrentPos;		// ���ɓǂݏo�����f�[�^�̈ʒu(0..m_dwTotalSize; m_dwTotalSize �̈ʒu�� EOF)

	OggVorbis_File	vf;
	vorbis_info*	vi;

};

	}//Sound
}//FDK

using namespace FDK::Sound;
