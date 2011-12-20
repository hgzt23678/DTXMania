#include "stdafx.h"
#include "CD3DApplication.h"
#include "CTexture.h"
#include "FDKError.h"
#include "LoadPNG.h"
#include "LoadJPEG.h"

namespace FDK {
	namespace Graphics {

// static �Ȃ��
DDPIXELFORMAT CTexture::ddpfARGB8888;
DDPIXELFORMAT CTexture::ddpfARGB4444;

// �Ǐ��ϐ�
static bool s_bEnumDone = false;		// �񋓍ς݂Ȃ� true
static bool s_bBeginScene = false;		// BeginScene() �ς݂Ȃ� true

CTexture::CTexture()
{
	this->pTexture			= NULL;

	this->nAlphaConst		= 255;
	this->bUseAddBlend		= false;
	this->bColorKeyEnable	= false;
	this->dwColorKey		= 0;
	this->dwWidth			= 0;
	this->dwHeight			= 0;
	this->dwImageHeight		= 0;
	this->dwImageWidth		= 0;
	this->fScaleW			= 1.0f;
	this->fScaleH			= 1.0f;
	this->strTextureName.clear();
	this->strFileName.clear();
	this->bSkipBlack		= true;
	this->lpResourceName	= NULL;
	this->lpResourceType	= NULL;
	this->pData				= NULL;
	this->dwDataSize		= 0;
}

CTexture::~CTexture()
{
	SAFE_RELEASE( this->pTexture );
}

HRESULT CTexture::InitDeviceObjects( TCHAR* name, DWORD width, DWORD height, bool bSkipBlack )
{
	InitParameters( name, NULL, NULL, NULL, NULL, 0, width, height, bSkipBlack );
	return S_OK;
}

HRESULT CTexture::InitDeviceObjectsFromFile( LPCTSTR name, LPCTSTR filename, DWORD width, DWORD height, bool bSkipBlack )
{
	InitParameters( name, filename, NULL, NULL, NULL, 0, width, height, bSkipBlack );
	return S_OK;
}

HRESULT CTexture::InitDeviceObjectsFromFileInMemory( TCHAR* name, BYTE* pData, DWORD dwDataSize, DWORD width, DWORD height, bool bSkipBlack )
{
	InitParameters( name, NULL, NULL, NULL, pData, dwDataSize, width, height, bSkipBlack );
	return S_OK;
}

HRESULT	CTexture::InitDeviceObjectsFromResource( TCHAR* name, LPCTSTR lpName, LPCTSTR lpType, DWORD width, DWORD height, bool bSkipBlack )
{
	InitParameters( name, NULL, lpName, lpType, NULL, 0, width, height, bSkipBlack );
	return S_OK;
}

void	CTexture::InitParameters( LPCTSTR name, LPCTSTR fname, LPCTSTR lpResourceName, LPCTSTR lpResourceType, BYTE* pData, DWORD dwDataSize, DWORD width, DWORD height, bool bSkipBlack )
{
	// ���O
	this->strTextureName.clear();
	if( name )
		this->strTextureName = name;

	this->strFileName.clear();
	if( fname )
		this->strFileName = fname;

	// ���\�[�X
	this->lpResourceName	= lpResourceName;
	this->lpResourceType	= lpResourceType;

	// �f�[�^�|�C���^�A�f�[�^�T�C�Y
	this->pData				= pData;
	this->dwDataSize		= dwDataSize;

	// �T�C�Y
	this->dwWidth			= width;
	this->dwHeight			= height;
	this->dwImageWidth		= width;
	this->dwImageHeight		= height;

	// ���̃X�L�b�v
	this->bSkipBlack		= bSkipBlack;

	// ���̑��i�f�t�H���g�l�j
	this->nAlphaConst		= 255;
	this->bUseAddBlend		= false;
	this->bColorKeyEnable	= false;
	this->dwColorKey		= 0xFF000000;
	this->fScaleW			= 1.0f;
	this->fScaleH			= 1.0f;

	SAFE_RELEASE( this->pTexture );
}

HRESULT	CTexture::RestoreDeviceObjects()
{
	HRESULT hr = D3D_OK;

	// (1) �w��t�@�C������̐���( BMP, PNG, JPEG )
	if( ! this->strFileName.empty() )
		hr = RestoreDeviceObjectsFromFile();

	// (2) ���������t�@�C������̐���( PNG �̂� )
	else if( this->pData != NULL && this->dwDataSize > 0 )
		hr = RestoreDeviceObjectsFromFileInMemory();

	// (3) ���\�[�X����̐���( PNG �̂� )
	else if( this->lpResourceName != NULL && this->lpResourceType != NULL )
		hr = RestoreDeviceObjectsFromResource();

	// (4) �󐶐�
	else if( FAILED( hr = Create() ) )
		return FDKERR_�e�N�X�`���̍쐬�Ɏ��s;

	return hr;
}

HRESULT	CTexture::RestoreDeviceObjectsFromResource()
{
	HMODULE hModule = (HMODULE)GetClassLongPtr( CD3DApplication::hWnd, GCL_HMODULE );

	HRSRC hSrc = FindResource( hModule, this->lpResourceName, this->lpResourceType );
	if( ! hSrc )
		return FDKERR_�t�@�C���̓ǂݍ��ݎ��s;

	HGLOBAL hPNG = LoadResource( hModule, hSrc );
	if( ! hPNG )
		return FDKERR_�t�@�C���̓ǂݍ��ݎ��s;

	this->dwDataSize = SizeofResource( hModule, hSrc );
	if( this->dwDataSize == 0 )
		return FDKERR_�t�@�C���̓ǂݍ��ݎ��s;

	this->pData = (BYTE*) LockResource( hPNG );
	if( ! this->pData )
		return FDKERR_�t�@�C���̓ǂݍ��ݎ��s;

	return RestoreDeviceObjectsFromFileInMemory();
}

HRESULT	CTexture::RestoreDeviceObjectsFromFileInMemory()
{
	HRESULT hr = S_OK;

	SAFE_RELEASE( this->pTexture );

	// �e�N�X�`���t�H�[�}�b�g�̗񋓂��܂��Ȃ���B
	if( ! s_bEnumDone )
		EnumTextureFormats();

	// �������C���[�W�t�@�C������̐���
	// ���e�N�X�`���� PNG �̂݃T�|�[�g
	if( this->pData  && this->dwDataSize > 0 )
	{
		DDSURFACEDESC2 ddsd;
		BYTE**	ppimage = NULL;

		// PNG ����̐���
		if( LoadPNGtoDDS( this->pData, this->dwDataSize, &ddsd, &ppimage ) )
		{
			hr = RestoreFromDDS( &ddsd, ppimage );

			for( DWORD i = 0; i < ddsd.dwHeight; i++ )
				SAFE_FREE( ppimage[i] );
			SAFE_FREE( ppimage );
		}
		else
			hr = FDKERR_DDS�̍쐬�Ɏ��s;
	}
	return hr;
}

HRESULT	CTexture::RestoreDeviceObjectsFromFile()
{
	HRESULT hr;

	SAFE_RELEASE( this->pTexture );

	// �e�N�X�`���t�H�[�}�b�g�̗񋓂��܂��Ȃ���B
	if( ! s_bEnumDone )
		EnumTextureFormats();

	// �t�@�C������̉摜�ǂݍ���
	// ���e�N�X�`���� PNG �̂݃T�|�[�g
	if( ! this->strFileName.empty() )
	{
		//-----------------------------
		// (A) BMP �̏ꍇ
		HBITMAP hSrcBmp;
		if( ( hSrcBmp = (HBITMAP) LoadImage( NULL, this->strFileName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE ) ) != NULL )
		{
			// �r�b�g�}�b�v���̎擾
			BITMAP bmSrc;
			GetObject( hSrcBmp, sizeof( BITMAP ), &bmSrc );

			// this->dwWidth, this->dwHeight ���Ƃ��� 0 �Ȃ�摜�T�C�Y�ɓ���������
			if( this->dwWidth == 0 && this->dwHeight == 0 )
			{
				this->dwWidth  = bmSrc.bmWidth;
				this->dwHeight = bmSrc.bmHeight;
				this->dwImageWidth  = bmSrc.bmWidth;
				this->dwImageHeight = bmSrc.bmHeight;
			}
			// this->dwWidth, this->dwHeight ���摜��菬�����Ȃ�摜�̃T�C�Y�ɍ��킹��
			if( this->dwWidth < (DWORD) bmSrc.bmWidth ) this->dwWidth = bmSrc.bmWidth;
			if( this->dwHeight < (DWORD) bmSrc.bmHeight ) this->dwHeight = bmSrc.bmHeight;

			// ��̃e�N�X�`�����쐬
			if( FAILED( hr = Create() ) )
			{
				DeleteObject( hSrcBmp );
				return hr;	// ���s
			}

			// ���������� RGB-32bit �� BITMAP �����
			BITMAPINFOHEADER bmi;
			ZeroMemory( &bmi, sizeof( BITMAPINFOHEADER ) );
			bmi.biSize		= sizeof( BITMAPINFOHEADER );
			bmi.biWidth		= this->dwWidth;
			bmi.biHeight	= - ((int)this->dwHeight);		// �g�b�v�_�E��Bitmap
			bmi.biPlanes	= 1;
			bmi.biBitCount	= 32;
			HDC hDstDC = CreateCompatibleDC( 0 );
			HBITMAP hDstBmp = CreateDIBSection( hDstDC, (BITMAPINFO *)&bmi, DIB_RGB_COLORS, NULL, NULL, 0 );

			// BITMAP ���� RGB-32bit BITMAP �֓]������i�g��k���Ȃ��j
			HDC hSrcDC = CreateCompatibleDC( 0 );
			HBITMAP hDstOldBmp = (HBITMAP) SelectObject( hDstDC, hDstBmp );
			HBITMAP hSrcOldBmp = (HBITMAP) SelectObject( hSrcDC, hSrcBmp );
			if( StretchBlt(
				hDstDC,									// �]���� DC
				0, 0, this->dwWidth, this->dwHeight,	// �]����̈�
				hSrcDC,									// �]���� DC
				0, 0, this->dwWidth, this->dwHeight,	// �]�����̈�
				SRCCOPY ) == 0 )
			{
				SelectObject( hDstDC, hDstOldBmp );
				SelectObject( hSrcDC, hSrcOldBmp );
				DeleteObject( hDstDC );
				DeleteObject( hSrcDC );
				DeleteObject( hDstBmp );
				DeleteObject( hSrcBmp );
				return FDKERR_StretchBlt�Ɏ��s;
			}
			SelectObject( hDstDC, hDstOldBmp );
			SelectObject( hSrcDC, hSrcOldBmp );
			DeleteObject( hSrcDC );
			DeleteObject( hSrcBmp );
			DeleteObject( hDstDC );

			// RGB-32bit BITMAP ����e�N�X�`���֓]��
			BITMAP bmDst;
			GetObject( hDstBmp, sizeof(BITMAP), &bmDst );
			CopyFromBMP32( 0, 0, &bmDst, this->bSkipBlack );
			DeleteObject( hDstBmp );

			return S_OK;
		}

		//-----------------------------
		// (B) JPEG �̏ꍇ
		FILE* fp;
		if( _tfopen_s( &fp, this->strFileName.c_str(), _T("rb") ) != 0 )
			return FDKERR_�t�@�C���̓ǂݍ��ݎ��s;

		BITMAPINFO* pbinfo = NULL;
		BYTE* pbdata = NULL;
		int h = fgetc( fp ) * 256 + fgetc( fp );
		fseek( fp, 0, SEEK_SET );
		if( h == 0xFFD8 && LoadJPEG( fp, &pbinfo, &pbdata ) != 0 )
		{
			// this->dwWidth, this->dwHeight ���Ƃ��� 0 �Ȃ�摜�T�C�Y�ɓ���������
			if( this->dwWidth == 0 && this->dwHeight == 0 ) {
				this->dwWidth  = pbinfo->bmiHeader.biWidth;
				this->dwHeight = pbinfo->bmiHeader.biHeight;
				this->dwImageWidth  = pbinfo->bmiHeader.biWidth;
				this->dwImageHeight = pbinfo->bmiHeader.biHeight;
			}
			// this->dwWidth, this->dwHeight ���摜��菬�����Ȃ�摜�̃T�C�Y�ɍ��킹��
			if( this->dwWidth < (DWORD) pbinfo->bmiHeader.biWidth ) {
				this->dwWidth = pbinfo->bmiHeader.biWidth;
				this->dwImageWidth = pbinfo->bmiHeader.biWidth;
			}
			if( this->dwHeight < (DWORD) pbinfo->bmiHeader.biHeight ) {
				this->dwHeight = pbinfo->bmiHeader.biHeight;
				this->dwImageHeight = pbinfo->bmiHeader.biHeight;
			}

			// ��̃e�N�X�`�����쐬
			if( FAILED( hr = Create() ) )
			{
				SAFE_FREE( pbinfo );
				SAFE_FREE( pbdata );
				fclose( fp );
				return hr;	// ���s
			}

			// ���������� RGB-32bit �� BITMAP �����
			BITMAPINFOHEADER bmi;
			ZeroMemory( &bmi, sizeof( BITMAPINFOHEADER ) );
			bmi.biSize		= sizeof( BITMAPINFOHEADER );
			bmi.biWidth		= this->dwWidth;
			bmi.biHeight	= this->dwHeight;		// �g�b�v�_�E��Bitmap
			bmi.biPlanes	= 1;
			bmi.biBitCount	= 32;
			HDC hDC = CreateCompatibleDC( 0 );
			HBITMAP hBmp = CreateDIBSection( hDC, (BITMAPINFO *)&bmi, DIB_RGB_COLORS, NULL, NULL, 0 );

			// JPEG-BITMAP ���� RGB-32bit BITMAP �֓]������i�g��k���Ȃ��j
			HBITMAP hOldBmp = (HBITMAP) SelectObject( hDC, hBmp );
			pbinfo->bmiHeader.biHeight = - pbinfo->bmiHeader.biHeight;	// JPEG-Bitmap ���{�g���A�b�v�Ŏw��
			if( StretchDIBits(
				hDC,							// �]���� DC
				0, 0, this->dwWidth, this->dwHeight,	// �]����̈�
				0, 0, this->dwWidth, this->dwHeight,	// �]�����̈�
				(LPVOID)pbdata, pbinfo,			// �]����DIB
				DIB_RGB_COLORS, SRCCOPY ) == GDI_ERROR )
			{
				SelectObject( hDC, hOldBmp );
				DeleteObject( hDC );
				DeleteObject( hBmp );
				SAFE_FREE( pbinfo );
				SAFE_FREE( pbdata );
				fclose( fp );
				return FDKERR_StretchBlt�Ɏ��s;
			}
			SelectObject( hDC, hOldBmp );
			DeleteDC( hDC );

			// RGB-32bit BITMAP ����e�N�X�`���֓]��
			BITMAP bmp;
			GetObject( hBmp, sizeof(BITMAP), &bmp );
			CopyFromBMP32( 0, 0, &bmp, this->bSkipBlack );
			DeleteObject( hBmp );
			SAFE_FREE( pbinfo );
			SAFE_FREE( pbdata );
			return S_OK;
		}
		else
		{
			SAFE_FREE( pbinfo );
			SAFE_FREE( pbdata );
		}

		//-----------------------------
		// (C) PNG �̏ꍇ
		DDSURFACEDESC2 ddsd;
		BYTE** ppimage = NULL;
		fclose( fp );
		if( LoadPNGtoDDS( this->strFileName.c_str(), &ddsd, &ppimage ) )
		{
			hr = RestoreFromDDS( &ddsd, ppimage );
			if( ppimage )
			{
				for( DWORD i = 0; i < ddsd.dwHeight; i++ )
					SAFE_FREE( ppimage[i] );
				SAFE_FREE( ppimage );
			}
			return hr;
		}
	}
	return S_OK;
}

HRESULT CTexture::RestoreFromDDS( DDSURFACEDESC2* pddsd, BYTE** ppimage )
{
	if( pddsd == NULL || ppimage == NULL )
		return FDKERR_�p�����[�^��NULL;

	HRESULT hr = S_OK;

	// this->dwWidth, this->dwHeight ���Ƃ��� 0 �Ȃ�摜�T�C�Y�ɓ���������
	if( this->dwWidth == 0 && this->dwHeight == 0 )
	{
		this->dwWidth  = pddsd->dwWidth;
		this->dwHeight = pddsd->dwHeight;
		this->dwImageWidth  = pddsd->dwWidth;
		this->dwImageHeight = pddsd->dwHeight;
	}

	// this->dwWidth, this->dwHeight ���摜�T�C�Y��菬�����Ȃ�摜�T�C�Y�ɓ���������
	if( this->dwWidth  < pddsd->dwWidth  ) {
		this->dwWidth  = pddsd->dwWidth;
		this->dwImageWidth  = pddsd->dwWidth;
	}
	if( this->dwHeight < pddsd->dwHeight ) {
		this->dwHeight = pddsd->dwHeight;
		this->dwImageHeight = pddsd->dwHeight;
	}

	// ��̃e�N�X�`�����쐬
	if( FAILED( hr = Create() ) )
		return hr;	// ���s

	// �e�N�X�`�������b�N
	DDSURFACEDESC2 ddsdtex;
	ZeroMemory( &ddsdtex, sizeof( DDSURFACEDESC2 ) );
	ddsdtex.dwSize = sizeof( DDSURFACEDESC2 );
	if( FAILED( hr = this->pTexture->Lock( NULL, &ddsdtex, DDLOCK_WAIT | DDLOCK_WRITEONLY, NULL ) ) )
		return FDKERR_Lock�Ɏ��s;

	// ���Ԃɓ]��
	switch( CD3DApplication::nBpp )
	{

	case 16:
		for( DWORD y = 0; y < pddsd->dwHeight; y++ )
		{
			BYTE* pDst = ((BYTE*) ddsdtex.lpSurface) + y * ddsdtex.lPitch;
			for( DWORD x = 0; x < pddsd->dwWidth; x++ )
			{
				WORD c = ( ( ppimage[y][x*4+0] << 8 ) & 0xF000 )	// A
						|( ( ppimage[y][x*4+1] << 4 ) & 0x0F00 )	// R
						|( ( ppimage[y][x*4+2]      ) & 0x00F0 )	// G
						|( ( ppimage[y][x*4+3] >> 4 ) & 0x000F );	// B
				*pDst++ = c & 0x00FF;
				*pDst++ = ( c & 0xFF00 ) >> 8;
			}
		}
		break;

	case 32:
		for( DWORD y = 0; y < pddsd->dwHeight; y++ )
		{
			BYTE* pDst = ((BYTE*) ddsdtex.lpSurface) + y * ddsdtex.lPitch;
			for( DWORD x = 0; x < pddsd->dwWidth; x++ )
			{
				*pDst++ = ppimage[y][x*4+3];	// B
				*pDst++ = ppimage[y][x*4+2];	// G
				*pDst++ = ppimage[y][x*4+1];	// R
				*pDst++ = ppimage[y][x*4+0];	// A
			}
		}
		break;
	}

	// �e�N�X�`���̃A�����b�N
	this->pTexture->Unlock( NULL );
	return S_OK;
}

HRESULT	CTexture::InvalidateDeviceObjects()
{
	SAFE_RELEASE( this->pTexture );
	return S_OK;
}

HRESULT	CTexture::DeleteDeviceObjects()
{
	SAFE_RELEASE( this->pTexture );
	return S_OK;
}

HRESULT CALLBACK TextureEnumerationCallback( DDPIXELFORMAT* pddpf, VOID* )
{
	DWORD dwFlags         = pddpf->dwFlags;
	DWORD dwFourCC        = pddpf->dwFourCC;
	DWORD dwTotalBitCount = pddpf->dwRGBBitCount;
	DWORD dwAlphaBitCount = 0;
	DWORD dwRedBitCount   = 0;
	DWORD dwGreenBitCount = 0;
	DWORD dwBlueBitCount  = 0;
	
	// �e�F�̃r�b�g���𐔂���
	DWORD mask;
	for( mask = pddpf->dwRGBAlphaBitMask; mask; mask>>=1 )
		dwAlphaBitCount += ( mask & 0x1 );
	for( mask = pddpf->dwRBitMask; mask; mask>>=1 )
		dwRedBitCount += ( mask & 0x1 );
	for( mask = pddpf->dwGBitMask; mask; mask>>=1 )
		dwGreenBitCount += ( mask & 0x1 );
	for( mask = pddpf->dwBBitMask; mask; mask>>=1 )
		dwBlueBitCount += ( mask & 0x1 );
	
	// �s���ȃt�H�[�}�b�g���`�F�b�N����
	if( ( dwFlags & DDPF_ALPHAPIXELS ) && ( dwAlphaBitCount == 0 ) )
		return DDENUMRET_OK;
	if( !( dwFlags & DDPF_ALPHAPIXELS ) && ( dwAlphaBitCount != 0 ) )
		return DDENUMRET_OK;
	if( !(dwFlags & DDPF_FOURCC ) && dwTotalBitCount == 0 )
		return DDENUMRET_OK;
	
	// ARGB8888 �� ARGB4444 �������o
	if( dwFlags & DDPF_RGB )
	{
		if( dwTotalBitCount == 32 && dwAlphaBitCount == 8 && dwRedBitCount == 8 && dwGreenBitCount == 8 && dwBlueBitCount == 8 )
			CTexture::ddpfARGB8888 = (*pddpf);
		if( dwTotalBitCount == 16 && dwAlphaBitCount == 4 && dwRedBitCount == 4 && dwGreenBitCount == 4 && dwBlueBitCount == 4 )
			CTexture::ddpfARGB4444 = (*pddpf);
		//Debug::OutFN( FNAME, _T("A%dR%dG%dB%d (%d-bit) format.\n"), dwAlphaBitCount, dwRedBitCount, dwGreenBitCount, dwBlueBitCount, dwTotalBitCount );
	}

	return DDENUMRET_OK;
}

void	CTexture::EnumTextureFormats()
{
	if( ! CD3DApplication::pD3DDevice )
		return;		// 3D�f�o�C�X������

	CTexture::ddpfARGB8888.dwSize = 0;
	CTexture::ddpfARGB4444.dwSize = 0;

	HRESULT hr = CD3DApplication::pD3DDevice->EnumTextureFormats( TextureEnumerationCallback, (LPVOID) NULL );
	if( FAILED( hr ) )
		return;		// �e�N�X�`���t�H�[�}�b�g�̗񋓂Ɏ��s

	s_bEnumDone = true;
	return;
}

HRESULT CTexture::Create()
{
	HRESULT hr;

	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof( DDSURFACEDESC2 ) );
	ddsd.dwSize				= sizeof( DDSURFACEDESC2 );
	ddsd.dwFlags			= DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_TEXTURESTAGE;
	ddsd.dwWidth			= this->dwWidth;
	ddsd.dwHeight			= this->dwHeight;
	ddsd.ddsCaps.dwCaps		= DDSCAPS_TEXTURE;
	ddsd.ddpfPixelFormat	= (CD3DApplication::nBpp == 16) ? CTexture::ddpfARGB4444 : CTexture::ddpfARGB8888;
	
	if( ddsd.ddpfPixelFormat.dwSize == 0 )
		return FDKERR_�e�N�X�`���t�H�[�}�b�g���Ȃ�;

	// �R�c�f�o�C�X�̔\�͂��擾
	D3DDEVICEDESC7 ddDesc;
	if( FAILED( hr = CD3DApplication::pD3DDevice->GetCaps( &ddDesc ) ) )
		return FDKERR_Caps�̎擾�Ɏ��s;

	// �n�[�h�E�F�A�f�o�C�X��������e�N�X�`���̊��蓖�ĊǗ���C����
	if( ddDesc.deviceGUID == IID_IDirect3DTnLHalDevice )
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
	else if( ddDesc.deviceGUID == IID_IDirect3DHALDevice )
		ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
    else
        ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;		// HEL ��������V�X�e����������ɍ��

    // �h���C�o�ɍ��킹�ăT�C�Y�𒲐�
    if( ddDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_POW2 )
    {
        for( ddsd.dwWidth  = 1; this->dwWidth  > ddsd.dwWidth;  ddsd.dwWidth  <<= 1 );
        for( ddsd.dwHeight = 1; this->dwHeight > ddsd.dwHeight; ddsd.dwHeight <<= 1 );
    }
	if( ddsd.dwWidth > ddDesc.dwMaxTextureWidth )
		ddsd.dwWidth = ddDesc.dwMaxTextureWidth;
	if( ddsd.dwHeight > ddDesc.dwMaxTextureHeight )
		ddsd.dwHeight = ddDesc.dwMaxTextureHeight;
    if( ddDesc.dpcTriCaps.dwTextureCaps & D3DPTEXTURECAPS_SQUAREONLY )
    {
        if( ddsd.dwWidth > ddsd.dwHeight ) ddsd.dwHeight = ddsd.dwWidth;
        else                               ddsd.dwWidth  = ddsd.dwHeight;
    }
	this->dwWidth  = ddsd.dwWidth;
	this->dwHeight = ddsd.dwHeight;

	// �e�N�X�`���̍쐬
	if( FAILED( hr = CD3DApplication::pDD->CreateSurface( &ddsd, &this->pTexture, NULL ) ) )
		return FDKERR_�e�N�X�`���̍쐬�Ɏ��s;

	// �e�N�X�`���̃N���A
	Clear();

	// �J���[�L�[�̐ݒ�i���j
	SetColorKey( ( CD3DApplication::nBpp == 16 ) ? 0x0000F000 : 0xFF000000 );

	return S_OK;
}

DWORD	CTexture::GetSurfaceSize()
{
	if( ! this->pTexture )
		return 0;

	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof( ddsd ) );
	ddsd.dwSize	= sizeof( ddsd );
	if( FAILED( this->pTexture->GetSurfaceDesc( &ddsd ) ) )
		return 0;

	DWORD dwSize = 0;
	for( DWORD y = 0; y < ddsd.dwHeight; y++ )
		dwSize += ddsd.lPitch;

	return dwSize;
}
HRESULT CTexture::Draw( int x, int y, LPRECT pSrc )
{
	if( this->pTexture == NULL || CD3DApplication::pddsBackBuffer == NULL )
		return FDKERR_�T�[�t�F�C�X��NULL;

	this->LostCheck();	// ���X�g���Ă���Ε�������

	// �����_�����O�X�e�[�g
	if( ! s_bBeginScene )
		BeginScene();
	SetRenderStates();
	
	// �`��
	float w = (pSrc == NULL) ? (float)this->dwWidth  : (float)(pSrc->right  - pSrc->left);
	float h = (pSrc == NULL) ? (float)this->dwHeight : (float)(pSrc->bottom - pSrc->top );
	float tu1 = (pSrc == NULL) ? 0.0f : pSrc->left / (float)this->dwWidth;
	float tv1 = (pSrc == NULL) ? 0.0f : pSrc->top / (float)this->dwHeight;
	float tu2 = (pSrc == NULL) ? 1.0f : pSrc->right / (float)this->dwWidth;
	float tv2 = (pSrc == NULL) ? 1.0f : pSrc->bottom / (float)this->dwHeight;
	
	D3DTLVERTEX vt[4];
	vt[0].sx  = (D3DVALUE) x - 0.5f;							// ����
	vt[0].sy  = (D3DVALUE) y - 0.5f;
	vt[0].sz  = (D3DVALUE) 0.0f;
	vt[0].rhw = (D3DVALUE) 1.0f;
	vt[0].color = RGBA_MAKE( 255, 255, 255, this->nAlphaConst );
	vt[0].specular = RGB_MAKE( 0, 0, 0 );
	vt[0].tu  = (D3DVALUE) tu1;
	vt[0].tv  = (D3DVALUE) tv1;
	
	vt[1].sx  = (D3DVALUE) x + w * this->fScaleW - 0.5f;			// �E��
	vt[1].sy  = (D3DVALUE) y - 0.5f;
	vt[1].sz  = (D3DVALUE) 0.0f;
	vt[1].rhw = (D3DVALUE) 1.0f;
	vt[1].color = RGBA_MAKE( 255, 255, 255, this->nAlphaConst );
	vt[1].specular = RGB_MAKE( 0, 0, 0 );
	vt[1].tu  = (D3DVALUE) tu2;
	vt[1].tv  = (D3DVALUE) tv1;
	
	vt[2].sx  = (D3DVALUE) x - 0.5f;							// ����
	vt[2].sy  = (D3DVALUE) y + h * this->fScaleH - 0.5f;
	vt[2].sz  = (D3DVALUE) 0.0f;
	vt[2].rhw = (D3DVALUE) 1.0f;
	vt[2].color = RGBA_MAKE( 255, 255, 255, this->nAlphaConst );
	vt[2].specular = RGB_MAKE( 0, 0, 0 );
	vt[2].tu  = (D3DVALUE) tu1;
	vt[2].tv  = (D3DVALUE) tv2;

	vt[3].sx  = (D3DVALUE) x + w * this->fScaleW - 0.5f;			// �E��
	vt[3].sy  = (D3DVALUE) y + h * this->fScaleH - 0.5f;
	vt[3].sz  = (D3DVALUE) 0.0f;
	vt[3].rhw = (D3DVALUE) 1.0f;
	vt[3].color = RGBA_MAKE( 255, 255, 255, this->nAlphaConst );
	vt[3].specular = RGB_MAKE( 0, 0, 0 );
	vt[3].tu  = (D3DVALUE) tu2;
	vt[3].tv  = (D3DVALUE) tv2;

	WORD pti[4] = { 0, 1, 2, 3 };
	CD3DApplication::pD3DDevice->SetTexture( 0, this->pTexture );
	CD3DApplication::pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_TLVERTEX, vt, 4, pti, 4, 0 );
	return S_OK;
}

HRESULT CTexture::Draw3D( int x, int y, LPRECT prcSrc, D3DXMATRIX *pMat )
{
	if( CD3DApplication::pD3DDevice == NULL )
		return FDKERR_3D�f�o�C�X��NULL;

	this->LostCheck();	// ���X�g���Ă���Ε�������

	RECT rc = { 0, 0, this->dwWidth, this->dwHeight };
	if( ! prcSrc ) prcSrc = &rc;

	D3DXMATRIX world;
	if( pMat )
		world = *pMat;
	else
		D3DXMatrixIdentity( &world );

	// ���݂̎ˉe�ϊ��l���擾�i�O��͕s�ρj
	float d, t, a;
	CD3DApplication::GetProjectionParam( &d, &t ,&a );

	// ���̑��p�����[�^�v�Z
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof( DDSURFACEDESC2 ) );
	ddsd.dwSize = sizeof( DDSURFACEDESC2 );
	CD3DApplication::pddsBackBuffer->GetSurfaceDesc( &ddsd );
	float devw = ddsd.dwWidth  / 2.0f;						// 320
	float devh = ddsd.dwHeight / 2.0f;						// 240
	float h = d * tan( t / 2.0f );							// ����p�̍���/2
	float w = h / a;										// ����p�̕�/2
	float hw = (prcSrc->right - prcSrc->left) / 2.0f;		// �`���`�̕�/2
	float hh = (prcSrc->bottom - prcSrc->top) / 2.0f;		// �`���`�̍���/2

	// �J�������X�v���C�g�p�ɃZ�b�g
	D3DXMATRIX view;
	D3DXMatrixLookAtLH( &view, &D3DXVECTOR3( 0.0f, 0.0f, -d ), &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ), &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
	CD3DApplication::pD3DDevice->SetTransform( D3DTRANSFORMSTATE_VIEW, (D3DMATRIX*)&view );

	// ���[���h�ϊ��o�^
	D3DXMATRIX mat, aux;
	float dx = -devw + hw + (float)x;
	float dy = -devh + hh + (float)y;
	D3DXMatrixTranslation( &aux, w * dx / devw, h * -dy / devh, 0.0f );
	mat = world * aux;		// world �K�p��� (x,y) �ֈړ��i (x,y)�͍�������W[dot] �j
	CD3DApplication::pD3DDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, (D3DMATRIX*) &mat );

	// �`��
	D3DLVERTEX tv[4];

	tv[0].x			= -(w * hw / devw);			// ����
	tv[0].y			= (h * hh / devh);
	tv[0].z			= 0.0f;
	tv[0].color		= RGBA_MAKE( 255, 255, 255, this->nAlphaConst );
	tv[0].specular	= RGB_MAKE( 0, 0, 0 );
	tv[0].tu		= (float)((float)prcSrc->left / (float)this->dwWidth);
	tv[0].tv		= (float)((float)prcSrc->top / (float)this->dwHeight);

	tv[1].x			= (w * hw / devw);			// �E��
	tv[1].y			= (h * hh / devh);
	tv[1].z			= 0.0f;
	tv[1].color		= tv[0].color;
	tv[1].specular	= RGB_MAKE( 0, 0, 0 );
	tv[1].tu		= (float)((float)prcSrc->right / (float)this->dwWidth);
	tv[1].tv		= (float)((float)prcSrc->top / (float)this->dwHeight);

	tv[2].x			= -(w * hw / devw);			// ����
	tv[2].y			= -(h * hh / devh);
	tv[2].z			= 0.0f;			
	tv[2].color		= tv[0].color;
	tv[2].specular	= RGB_MAKE( 0, 0, 0 );
	tv[2].tu		= (float)((float)prcSrc->left / (float)this->dwWidth);
	tv[2].tv		= (float)((float)prcSrc->bottom / (float)this->dwHeight);

	tv[3].x			= (w * hw / devw);			// �E��
	tv[3].y			= -(h * hh / devh);
	tv[3].z			= 0.0f;
	tv[3].color		= tv[0].color;
	tv[3].tu		= (float)((float)prcSrc->right / (float)this->dwWidth);
	tv[3].tv		= (float)((float)prcSrc->bottom / (float)this->dwHeight);

//	m_sbAddBlend = this->bUseAddBlend;
	SetRenderStates();
	WORD pti[4] = { 0, 1, 2, 3 };
	CD3DApplication::pD3DDevice->SetTexture( 0, this->pTexture );
	CD3DApplication::pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, tv, 4, pti, 4, 0 );

	return S_OK;
}

void	CTexture::BeginScene()
{
	if( ! CD3DApplication::pD3DDevice )
		return;

	CD3DApplication::pD3DDevice->BeginScene();
	s_bBeginScene = true;
}

void	CTexture::EndScene()
{
	if( ! CD3DApplication::pD3DDevice )
		return;

	CD3DApplication::pD3DDevice->EndScene();
	s_bBeginScene = false;
}

void	CTexture::SetRenderStates()
{
	CD3DApplication::pD3DDevice->SetRenderState( D3DRENDERSTATE_COLORKEYENABLE, this->bColorKeyEnable ? TRUE : FALSE );

	if( this->bUseAddBlend )
	{
		CD3DApplication::pD3DDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
		CD3DApplication::pD3DDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );	// �A���t�@�l�ŉ��Z�x�𒲐߉\
		CD3DApplication::pD3DDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE );
	}
	else
	{
		CD3DApplication::pD3DDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
		CD3DApplication::pD3DDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );	// �A���t�@�l�ŉ��Z�x�𒲐߉\
		CD3DApplication::pD3DDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );
	}
}

HRESULT CTexture::SetColorKey()
{
	if( ! this->pTexture )
		return FDKERR_�e�N�X�`����NULL;

	// ������̓_�̐F���擾
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof(ddsd) );
	ddsd.dwSize = sizeof(ddsd);
	if( SUCCEEDED( this->pTexture->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) ) )
	{
		DWORD dwBits = ddsd.ddpfPixelFormat.dwRGBBitCount;
		this->dwColorKey = (*(DWORD*)ddsd.lpSurface) & ((dwBits == 16) ? 0x0000FFFF : 0xFFFFFFFF);
		this->pTexture->Unlock( NULL );

        // �J���[�L�[�Ƃ��Đݒ�
		DDCOLORKEY ddck;
		ddck.dwColorSpaceLowValue  = this->dwColorKey;
		ddck.dwColorSpaceHighValue = this->dwColorKey;
		this->pTexture->SetColorKey( DDCKEY_SRCBLT, &ddck );
	}
	return S_OK;
}

HRESULT CTexture::SetColorKey( DWORD dwColor )
{
	if( ! this->pTexture )
		return FDKERR_�e�N�X�`����NULL;

    // �J���[�L�[�Ƃ��Đݒ�
	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue  = dwColor;
	ddck.dwColorSpaceHighValue = dwColor;
	this->pTexture->SetColorKey( DDCKEY_SRCBLT, &ddck );
	this->dwColorKey = dwColor;
	return S_OK;
}
HRESULT	CTexture::SetScale( float w, float h )
{
	this->fScaleW = w;
	this->fScaleH = h;
	return S_OK;
}

HRESULT CTexture::GetDC( HDC *phdc )
{
	if( ! this->pTexture )
		return FDKERR_�e�N�X�`����NULL;
	if( ! phdc )
		return E_INVALIDARG;

	this->LostCheck();	// ���X�g���Ă���Ε�������

	return this->pTexture->GetDC( phdc );
}

HRESULT CTexture::ReleaseDC( HDC &phdc )
{
	if( ! this->pTexture )
		return FDKERR_�e�N�X�`����NULL;

	return this->pTexture->ReleaseDC( phdc );
}

HRESULT	CTexture::SetAlphaConst( int a )
{
	if( a < 0 ) a = 0;
	if( a > 255 ) a = 255;
	this->nAlphaConst = a;
	return S_OK;
}

HRESULT CTexture::Clear( LPRECT prc )
{
	if( this->pTexture )
	{
		this->LostCheck();	// ���X�g���Ă���Ε�������

		DDBLTFX ddbltfx;
		ZeroMemory( &ddbltfx, sizeof( ddbltfx ) );
		ddbltfx.dwSize = sizeof( ddbltfx );
		ddbltfx.dwFillColor = 0;
		this->pTexture->Blt( prc, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
	}
	return S_OK;
}

DWORD	CTexture::BitCount( DWORD dwNum )
{
	DWORD dwCount;
	for( dwCount = 0; dwNum != 0; dwNum >>= 1 )
		dwCount += ( dwNum & 0x1 );
	return dwCount;
}
DWORD	CTexture::ShiftCount( DWORD dwNum )
{
	if( dwNum == 0 ) return 0;		// �[���̂Ƃ��̓[����Ԃ��B

	DWORD dwCount;
	for( dwCount = 0; (dwNum & 0x1) == 0; dwNum >>= 1 )
		dwCount ++;

	return dwCount;
}

HRESULT	CTexture::Text( int x, int y, HFONT hFont, LPCTSTR str, COLORREF color )
{
	if( ! this->pTexture )
		return FDKERR_�e�N�X�`����NULL;
		
	this->LostCheck();	// ���X�g���Ă���Ε�������

	if( ! str || *str == _T('\0') )
		return S_OK;

	// BITMAP �̍쐬
	BITMAPINFOHEADER bmi;
	ZeroMemory( &bmi, sizeof( BITMAPINFOHEADER ) );
	bmi.biSize = sizeof( BITMAPINFOHEADER );
	bmi.biWidth = (LONG) (this->dwWidth-x);
	bmi.biHeight =  - ((LONG)(this->dwHeight-y));
	bmi.biPlanes = 1;
	bmi.biBitCount = 32;

	HDC hDC = CreateCompatibleDC( 0 );
	HBITMAP hBmp = CreateDIBSection( hDC, (BITMAPINFO *)&bmi, DIB_RGB_COLORS, NULL, NULL, 0 );

	// ������DC ���擾���� BITMAP �փe�L�X�g�`��
	HBITMAP hOldBmp = (HBITMAP) SelectObject( hDC, hBmp  );
	HFONT hOldFont = (HFONT) SelectObject( hDC, hFont );
	SetTextColor( hDC, color );
	SetBkColor( hDC, RGB(0,0,0) );
	RECT rc = { 0, 0, this->dwWidth-x, this->dwHeight-y };
	DrawText( hDC, str, lstrlen(str), &rc, DT_LEFT|DT_NOPREFIX );
	SelectObject( hDC, hOldFont );
	SelectObject( hDC, hOldBmp );
	DeleteDC( hDC );

	// �r�b�g�}�b�v����e�N�X�`���֓]��
	BITMAP bmp;
	GetObject( hBmp, sizeof(BITMAP), &bmp );
	CopyFromBMP32( x, y, &bmp, true );
	DeleteObject( hBmp );

	return S_OK;
}

HRESULT	CTexture::TextSharp( int x, int y, HFONT hFont, LPCTSTR str, COLORREF color )
{
	if( ! this->pTexture )
		return FDKERR_�e�N�X�`����NULL;
		
	this->LostCheck();	// ���X�g���Ă���Ε�������

	if( ! str || *str == _T('\0') )
		return S_OK;

	// �ʐςS�{�� BITMAP ���쐬
	BITMAPINFOHEADER bmi;
	ZeroMemory( &bmi, sizeof( BITMAPINFOHEADER ) );
	bmi.biSize		= sizeof( BITMAPINFOHEADER );
	bmi.biWidth		= (    (LONG)(this->dwWidth  - x) ) * 2;
	bmi.biHeight	= ( - ((LONG)(this->dwHeight - y)) ) * 2;
	bmi.biPlanes	= 1;
	bmi.biBitCount	= 32;
	HDC hDC = CreateCompatibleDC( 0 );
	HBITMAP hBmp = CreateDIBSection( hDC, (BITMAPINFO *)&bmi, DIB_RGB_COLORS, NULL, NULL, 0 );

	// ������DC ���g���� BITMAP �փe�L�X�g�`��
	HBITMAP hOldBmp = (HBITMAP) SelectObject( hDC, hBmp  );
	HFONT hOldFont  = (HFONT) SelectObject( hDC, hFont );
	SetTextColor( hDC, color );
	SetBkColor( hDC, RGB(0,0,0) );
	RECT rc = { 0, 0, (this->dwWidth-x)*2, (this->dwHeight-y)*2 };
	DrawText( hDC, str, lstrlen(str), &rc, DT_LEFT|DT_NOPREFIX );
	SelectObject( hDC, hOldFont );
	SelectObject( hDC, hOldBmp );
	DeleteDC( hDC );

	// �r�b�g�}�b�v����e�N�X�`���֓]��
	BITMAP bmp;
	GetObject( hBmp, sizeof(BITMAP), &bmp );
	CopyFromBMP32x4( x, y, &bmp, true );
	DeleteObject( hBmp );

	return S_OK;
}
int		CTexture::GetTextWidthDot( HFONT hFont, LPCTSTR str������ )
{
	HDC hdc = ::CreateCompatibleDC( 0 );
	HFONT hOldFont = (HFONT) ::SelectObject( hdc, hFont );
	SIZE size;
	::GetTextExtentPoint32( hdc, str������, lstrlen(str������), &size );
	::SelectObject( hdc, hOldFont );
	::DeleteDC( hdc );
	return (int) size.cx;
}
void	CTexture::CopyFromBMP32( int x, int y, BITMAP* bmp, bool bSkipBlack )
{
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof( DDSURFACEDESC2 ) );
	ddsd.dwSize = sizeof( DDSURFACEDESC2 );
	if( SUCCEEDED( this->pTexture->Lock( NULL, &ddsd, DDLOCK_WAIT|DDLOCK_WRITEONLY, NULL ) ) )
	{
		BYTE* src_line = (BYTE*) bmp->bmBits;
		BYTE* dst_line = (BYTE*) ddsd.lpSurface;

		if( ddsd.ddpfPixelFormat.dwRGBBitCount == 32 )
		{
			for( int ly = 0; ly < (LONG)this->dwHeight; ly++ )
			{
				if( ly >= y )
				{
					DWORD* src = (DWORD*) src_line;
					DWORD* dst = (DWORD*) dst_line;
					for( int lx = 0; lx < (LONG)this->dwWidth; lx++ )
					{
						if( lx >= x ) 
						{
							DWORD c = *src & 0x00FFFFFF;
							if( ! bSkipBlack || c != 0x00000000 )		// �F 0 ���X�L�b�v�H
								*dst = c | 0xFF000000;					// �����̃��͏�� 255
							src ++;
						}
						dst ++;
					}
					src_line += bmp->bmWidthBytes;
				}
				dst_line += ddsd.lPitch;
			}
		}
		else if ( ddsd.ddpfPixelFormat.dwRGBBitCount == 16 )
		{
			for( int ly = 0; ly < (LONG)this->dwHeight; ly++ )
			{
				if( ly >= y )
				{
					DWORD* src = (DWORD*) src_line;
					WORD*  dst = (WORD*)  dst_line;
					for( int lx = 0; lx < (LONG)this->dwWidth; lx++ )
					{
						if( lx >= x ) 
						{
							WORD rgb = (WORD)(((*src & 0x00FF0000) >> 20) << 8)
									 | (WORD)(((*src & 0x0000FF00) >> 12) << 4)
									 | (WORD)( (*src & 0x000000FF) >>  4);
							if( ! bSkipBlack || rgb != 0x0000 )			// �F 0 ���X�L�b�v�H
								*dst = rgb | 0xF000;					// �����̃��͏�� 255(15)
							src ++;
						}
						dst ++;
					}
					src_line += bmp->bmWidthBytes;
				}
				dst_line += ddsd.lPitch;
			}
		}

		this->pTexture->Unlock( NULL );
	}
}

void	CTexture::CopyFromBMP32x4( int x, int y, BITMAP* bmp, bool bSkipBlack )
{
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof( DDSURFACEDESC2 ) );
	ddsd.dwSize = sizeof( DDSURFACEDESC2 );
	if( SUCCEEDED( this->pTexture->Lock( NULL, &ddsd, DDLOCK_WAIT|DDLOCK_WRITEONLY, NULL ) ) )
	{
		BYTE* src_line = (BYTE*) bmp->bmBits;
		BYTE* dst_line = (BYTE*) ddsd.lpSurface;

		// A. �]����e�N�X�`���� 32bpp �̏ꍇ
		if( ddsd.ddpfPixelFormat.dwRGBBitCount == 32 )
		{
			for( int ly = 0; ly < (LONG)this->dwHeight; ly++ )
			{
				if( ly >= y )
				{
					DWORD* src = (DWORD*) src_line;
					DWORD* dst = (DWORD*) dst_line;
					for( int lx = 0; lx < (LONG)this->dwWidth; lx++ )
					{
						if( lx >= x ) 
						{
							DWORD s, s1[4], s2[4], s3[4];
							s = *src;
							s1[0] = s & 0x00FF0000;
							s2[0] = s & 0x0000FF00;
							s3[0] = s & 0x000000FF;
							s = *(src + 1);
							s1[1] = s & 0x00FF0000;
							s2[1] = s & 0x0000FF00;
							s3[1] = s & 0x000000FF;
							s = *((DWORD*)(((BYTE*)src) + bmp->bmWidthBytes));
							s1[2] = s & 0x00FF0000;
							s2[2] = s & 0x0000FF00;
							s3[2] = s & 0x000000FF;
							s = *((DWORD*)(((BYTE*)src) + bmp->bmWidthBytes) + 1);
							s1[3] = s & 0x00FF0000;
							s2[3] = s & 0x0000FF00;
							s3[3] = s & 0x000000FF;

							DWORD A = 255;	// �A���t�@�͏��255

							bool bAllBlack = false;		// bSkipBlack ���ɃX�L�b�v���邩�ۂ��́A�S�̐F���u���ׂč��v���u�ЂƂł����łȂ��v�������Ō��߂�B
							for( int i = 0; i < 4; i++ ) {
								if( ! ( s1[i] == 0 && s2[i] == 0 && s3[i] == 0 ) ) {
									bAllBlack = true;
									break;
								}
							}
						
							if( ! ( bSkipBlack && bAllBlack == 0 ) )	// �S�̐F�����ׂč� ���� bSkipBlack=true �Ȃ�]�����Ȃ��B
							{
								DWORD c1 = ((( s1[0] + s1[1] + s1[2] + s1[3] ) >> 2) & ddsd.ddpfPixelFormat.dwRBitMask );
								DWORD c2 = ((( s2[0] + s2[1] + s2[2] + s2[3] ) >> 2) & ddsd.ddpfPixelFormat.dwGBitMask );
								DWORD c3 = ((( s3[0] + s3[1] + s3[2] + s3[3] ) >> 2) & ddsd.ddpfPixelFormat.dwBBitMask );
								DWORD c4 = (A << 24);
								*dst = c1 | c2 | c3 | c4;
							}
							src += 2;
						}
						dst ++;
					}
					src_line += bmp->bmWidthBytes * 2;
				}
				dst_line += ddsd.lPitch;
			}
		}
		// B. �]����e�N�X�`���� 16bpp �̏ꍇ
		else if ( ddsd.ddpfPixelFormat.dwRGBBitCount == 16 )
		{
			// �]����}�X�N�̂��ꂼ��̃r�b�g���ƃV�t�g�������߂�
			int nBit��R = this->BitCount( ddsd.ddpfPixelFormat.dwRBitMask );
			int nBit��G = this->BitCount( ddsd.ddpfPixelFormat.dwGBitMask );
			int nBit��B = this->BitCount( ddsd.ddpfPixelFormat.dwBBitMask );
			int nBit��A = this->BitCount( ddsd.ddpfPixelFormat.dwRGBAlphaBitMask );
			int nShift��R = this->ShiftCount( ddsd.ddpfPixelFormat.dwRBitMask );
			int nShift��G = this->ShiftCount( ddsd.ddpfPixelFormat.dwGBitMask );
			int nShift��B = this->ShiftCount( ddsd.ddpfPixelFormat.dwBBitMask );
			int nShift��A = this->ShiftCount( ddsd.ddpfPixelFormat.dwRGBAlphaBitMask );

			for( int ly = 0; ly < (LONG)this->dwHeight; ly++ )
			{
				if( ly >= y )
				{
					DWORD* src = (DWORD*) src_line;
					WORD*  dst = (WORD*)  dst_line;
					for( int lx = 0; lx < (LONG)this->dwWidth; lx++ )
					{
						if( lx >= x ) 
						{
							DWORD s;
							DWORD s1[4], s2[4], s3[4];
							s = *src;
							s1[0] = (s & 0x00FF0000) >> (16 + (8 - nBit��R));
							s2[0] = (s & 0x0000FF00) >> ( 8 + (8 - nBit��G));
							s3[0] = (s & 0x000000FF) >> ( 0 + (8 - nBit��B));
							s = *(src + 1);
							s1[1] = (s & 0x00FF0000) >> (16 + (8 - nBit��R));
							s2[1] = (s & 0x0000FF00) >> ( 8 + (8 - nBit��G));
							s3[1] = (s & 0x000000FF) >> ( 0 + (8 - nBit��B));
							s = *((DWORD*)(((BYTE*)src) + bmp->bmWidthBytes));
							s1[2] = (s & 0x00FF0000) >> (16 + (8 - nBit��R));
							s2[2] = (s & 0x0000FF00) >> ( 8 + (8 - nBit��G));
							s3[2] = (s & 0x000000FF) >> ( 0 + (8 - nBit��B));
							s = *((DWORD*)(((BYTE*)src) + bmp->bmWidthBytes) + 1);
							s1[3] = (s & 0x00FF0000) >> (16 + (8 - nBit��R));
							s2[3] = (s & 0x0000FF00) >> ( 8 + (8 - nBit��G));
							s3[3] = (s & 0x000000FF) >> ( 0 + (8 - nBit��B));

							WORD A = (WORD) ddsd.ddpfPixelFormat.dwRGBAlphaBitMask;		// �A���t�@�͏�ɍő�

							bool bAllBlack = false;		// bSkipBlack ���ɃX�L�b�v���邩�ۂ��́A�S�̐F���u���ׂč��v���u�ЂƂł����łȂ��v�������Ō��߂�B
							for( int i = 0; i < 4; i++ ) {
								if( ! ( s1[i] == 0 && s2[i] == 0 && s3[i] == 0 ) ) {
									bAllBlack = true;
									break;
								}
							}
						
							if( ! ( bSkipBlack && bAllBlack == 0 ) )	// �S�̐F�����ׂč� ���� bSkipBlack=true �Ȃ�]�����Ȃ��B
							{
								WORD c1 = (WORD)(((( s1[0] + s1[1] + s1[2] + s1[3] ) >> 2) << nShift��R ) & ddsd.ddpfPixelFormat.dwRBitMask );
								WORD c2 = (WORD)(((( s2[0] + s2[1] + s2[2] + s2[3] ) >> 2) << nShift��G ) & ddsd.ddpfPixelFormat.dwGBitMask );
								WORD c3 = (WORD)(((( s3[0] + s3[1] + s3[2] + s3[3] ) >> 2) << nShift��B ) & ddsd.ddpfPixelFormat.dwBBitMask );
								*dst = c1 | c2 | c3 | A;
							}
							src += 2;
						}
						dst ++;
					}
					src_line += bmp->bmWidthBytes * 2;
				}
				dst_line += ddsd.lPitch;
			}
		}

		this->pTexture->Unlock( NULL );
	}
}

void	CTexture::LostCheck()
{
	if( this->pTexture != NULL && this->pTexture->IsLost() == DDERR_SURFACELOST )
		this->RestoreDeviceObjects();
}
	}//Graphics
}//FDK