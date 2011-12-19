#include "stdafx.h"
#include "CAvi.h"
#include "FDKError.h"

namespace FDK {
	namespace Graphics {

int CAvi::n�Q�Ɛ� = 0;

CAvi::CAvi()
{
	this->hFile 		= NULL;
	this->hAvi 			= NULL;
	this->hFrame		= NULL;
	this->hDrawDib		= NULL;
}
CAvi::~CAvi()
{
	_���();
}
void	CAvi::_������()
{
	this->hFile 		= NULL;
	this->hAvi 			= NULL;
	this->hFrame		= NULL;
	this->hDrawDib		= NULL;
	this->lFrameWidth	= 0;
	this->lFrameHeight	= 0;

	this->bErrorAtDrawFrame = false;

	// ���߂Ă̗��p�Ȃ�AVI���C�u����������������B
	if( this->n�Q�Ɛ�++ == 0 )
		::AVIFileInit();
}

void	CAvi::_���()
{
	// �t�@�C�����J���ꂽ�܂܂Ȃ����B
	if( this->hFile )
		this->Close();

	// CAvi �𗘗p����C���X�^���X���Ȃ��Ȃ�����AVI���C�u�������I������B
	if( --this->n�Q�Ɛ� == 0 )
		::AVIFileExit();
}

HRESULT CAvi::Open( LPCTSTR fname )
{
	HRESULT hr;

	// �t�@�C�����J���ꂽ�܂܂Ȃ����B
	if( this->hFile )
		this->Close();

	// AVI�t�@�C����ǂݎ�胂�[�h�ŃI�[�v������B
	if(( hr = ::AVIFileOpen( &this->hFile, fname, OF_READ, NULL ) ) != 0 )			// ��������� S_OK �ł͂Ȃ� 0 ���Ԃ�B�ȉ������B
		return hr;

	// �摜�X�g���[�����擾����B
	if( (hr = ::AVIFileGetStream( this->hFile, &this->hAvi, streamtypeVIDEO, 0 )) != 0 )
	{
		::AVIFileRelease( this->hFile );
		this->hFile = NULL;
		return hr;
	}

	// ��������擾����B
	AVISTREAMINFO Info;
	if( (hr = ::AVIStreamInfo( this->hAvi, &Info, sizeof(Info) )) != 0 )
	{
		::AVIStreamRelease( this->hAvi );
		::AVIFileRelease( this->hFile );
		this->hAvi = NULL;
		this->hFile = NULL;
		return FDKERR_������̎擾�Ɏ��s;
	}
	this->dw���[�g			= Info.dwRate;
	this->dw�X�P�[��		= Info.dwScale;
	this->dw���t���[����	= ::AVIStreamLength( this->hAvi );
	this->lFrameWidth		= Info.rcFrame.right  - Info.rcFrame.left;
	this->lFrameHeight		= Info.rcFrame.bottom - Info.rcFrame.top;

	// �t���[���̃I�[�v��
	if( ( this->hFrame = ::AVIStreamGetFrameOpen( this->hAvi, NULL ) ) == NULL )
	//if( ( m_hFrame = ::AVIStreamGetFrameOpen( m_hAvi, (LPBITMAPINFOHEADER) AVIGETFRAMEF_BESTDISPLAYFMT ) ) == NULL )		// ���������Əd���Ȃ�cRADEON9800�ł��c
	{
		::AVIStreamRelease( this->hAvi );
		::AVIFileRelease( this->hFile );
		this->hAvi = NULL;
		this->hFile = NULL;
		return FDKERR_����t���[���̃I�[�v���Ɏ��s;
	}

	// DrawDib �̃I�[�v��
	if( (this->hDrawDib = ::DrawDibOpen()) == NULL )
	{
		::AVIStreamGetFrameClose( this->hFrame );
		::AVIStreamRelease( this->hAvi );
		::AVIFileRelease( this->hFile );
		this->hAvi = NULL;
		this->hFile = NULL;
		return FDKERR_DRAWDIB�̃I�[�v���Ɏ��s;
	}

	return S_OK;
}

void	CAvi::Close()
{
	if( this->hDrawDib )
	{
		::DrawDibEnd( this->hDrawDib );
		::DrawDibClose( this->hDrawDib );
		this->hDrawDib = NULL;
	}
	
	if( this->hFrame )
	{
		::AVIStreamGetFrameClose( this->hFrame );
		this->hFrame = NULL;
	}

	if( this->hAvi )
	{
		::AVIStreamRelease( this->hAvi ); 
		this->hAvi = NULL;
	}

	if( this->hFile )
	{
		::AVIFileRelease( this->hFile );
		this->hFile = NULL;
	}
}

DWORD	CAvi::_��������t���[���ԍ���Ԃ�( double time )
{
	if( !this->hAvi || !this->dw���t���[���� || !this->dw���[�g || !this->dw�X�P�[�� )
		return 0;	// ����������ĂȂ�

	// �t���[���̌v�Z
	return (DWORD)(time * ((double)(this->dw���[�g) / (1000.0 * this->dw�X�P�[��)));
}

void	CAvi::_�t���[���T�C�Y���擾����( LONG* plWidth, LONG* plHeight )
{
	if( plWidth  ) *plWidth  = this->lFrameWidth;
	if( plHeight ) *plHeight = this->lFrameHeight;
}

int		CAvi::_�w�莞���̃t���[����DC�֕`�悷��( HWND hWnd, HDC hDC, double time, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh )
{
	// �G���[�����ς݂Ȃ牽�����Ȃ��ŏI��
	if( this->bErrorAtDrawFrame )
		return CAVI_�Đ��I��;
	
	// ����������ĂȂ���΃G���[�I��
	if( !this->hAvi || !this->dw���t���[���� || !this->dw���[�g || !this->dw�X�P�[�� )
	{
		this->bErrorAtDrawFrame = true;
		return CAVI_�G���[�I��;
	}

	// �w�莞������A�\�����ׂ��t���[���ԍ����v�Z
	DWORD FrameNo = _��������t���[���ԍ���Ԃ�( time );
	if( this->dw���t���[���� <= FrameNo )
		return CAVI_�Đ��I��;

	// �t���[���̎擾
	BITMAPINFOHEADER *pBmpInfo;		// DIB�w�b�_
	BYTE* pBmpData;					// DIB�f�[�^
	if(( pBmpInfo = (BITMAPINFOHEADER*) ::AVIStreamGetFrame( this->hFrame, FrameNo ) ) == NULL )
	{
		// �t���[���摜�̎擾�Ɏ��s
		this->bErrorAtDrawFrame = true;
		return CAVI_�G���[�I��;
	}

	// AVIStreamGetFrame �ŕԂ���� BITMAPINFOHEADER �ɂ� biCompression==BI_BITFIELDS ���� biColors[] �������Ă��Ȃ����߁A
	// ���e���R�s�[���Ď����� biColors[] ��ǉ�����K�v������B
	struct PACKDIB {
		BITMAPINFOHEADER	header;
		DWORD				r, g, b;
	} static pd;
	pBmpData = (LPBYTE)pBmpInfo + (WORD)(pBmpInfo->biSize);
	::CopyMemory( &(pd.header), pBmpInfo, sizeof(BITMAPINFOHEADER) );
	if( pd.header.biCompression == BI_BITFIELDS )
	{
		if( pd.header.biBitCount == 16 )
		{
			pd.r = 0xf800;	// 5
			pd.g = 0x07e0;	// 6
			pd.b = 0x001f;	// 5
		}
		else
		{
			pd.r = 0xff0000;	// 8
			pd.g = 0x00ff00;	// 8
			pd.b = 0x0000ff;	// 8
		}
	}
	pBmpInfo = &(pd.header);

	// ���k����Ă���Ȃ�W�J����
	if( pBmpInfo->biCompression != BI_RGB && pBmpInfo->biCompression != BI_BITFIELDS )
	{
		this->hDrawDib = DrawDibOpen();
		HDC hdc = ::GetDC( hWnd );
		if( ! ::DrawDibDraw( this->hDrawDib, hdc, dx, dy, dw, dh, pBmpInfo, pBmpData, sx, sy, sw, sh, DDF_DONTDRAW ) )	// �W�J�̂݁E�`��Ȃ�
		{
			// ���kDIB�̓W�J�Ɏ��s
			this->bErrorAtDrawFrame = true;
			return CAVI_�G���[�I��;
		}
		pBmpData = (LPBYTE) ::DrawDibGetBuffer( this->hDrawDib, pBmpInfo, sizeof(BITMAPINFOHEADER), 0 );
		::ReleaseDC( hWnd, hdc );
	}

	// �`��
	::DrawDibDraw( this->hDrawDib, hDC, dx, dy, dw, dh, pBmpInfo, pBmpData, sx, sy, sw, sh, 0 );

/*
	if( StretchDIBits( hDC, dx, dy, dw, dh, sx, sy, sw, sh, pBmpData, (BITMAPINFO*)(pBmpInfo), DIB_RGB_COLORS, SRCCOPY ) == GDI_ERROR )
	{
		m_bErrorAtDrawFrame = true;
		if( m_hDrawDib ) {
			DrawDibEnd( m_hDrawDib );
			DrawDibClose( m_hDrawDib );
		}
		return false;
	}
*/
	return 0;
}

	}//Graphics
}//FDK
