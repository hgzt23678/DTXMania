#pragma once

#include "CD3DApplication.h"
#include "FDKError.h"

namespace FDK {
	namespace Graphics {
		
enum LOADPLACE {
	LOADTO_VIDEO,				// VIDEO MEMORY �ɍ쐬���A���s������G���[�B
	LOADTO_SYSTEM,				// SYSTEM MEMORY �ɍ쐬���A���s������G���[�B
	LOADTO_VIDEO_OR_SYSTEM,		// VIDEO MEMORY �ɍ쐬���A���s������ SYSTEM MEMORY �ɍ쐬���A����Ɏ��s������G���[�B
	LOADTO_NULL

	// ���ӁF
	// (1) �T�[�t�F�C�X���Ȃ�ׂ� VIDEO �ɍ��ׂ��iLOADTO_SYSTEM ���g���ƕ\�����N�\�d�����Ȃ�O���{������iATI ALL-IN-WONDER128�Ȃǁj�j�����A
	//     �T�[�t�F�C�X�ɂ̓e�N�X�`���̂悤�Ȏ����Ǘ��@�\�͂Ȃ����߁A���܂� VIDEO ���L����ƃe�N�X�`���̊��p�ł��� VIDEO ������̂�
	// �@�@�o�����X���l���邱�ƁB
	// �@�@�r�f�I������������Ȃ��ꍇ�A�e�N�X�`���͂��̎c�菭�Ȃ��̈�ł܂킳�Ȃ���΂Ȃ�Ȃ��Ȃ�B
	// (2) CSurface �� Blt ���邽�߂̃T�[�t�F�C�X�͕K�� LOADTO_VIDEO �ō쐬���邱�ƁB(SYSTEM�s��)
};

class CSurface
{
public:
	// ��T�[�t�F�C�X�쐬�̂��߂̏����ݒ�(1)�B
	// �����ł̓T�[�t�F�C�X�� ���O�A�T�C�Y�A�z�u�ꏊ ������ɋL�������邾���ŁA�f�o�C�X�֌W�̏����͈�؍s��Ȃ��B
	void InitDeviceObjects( LPCTSTR name, DWORD width, DWORD height, LOADPLACE place=LOADTO_VIDEO_OR_SYSTEM, LPDDPIXELFORMAT pFormat=NULL );

	// ��T�[�t�F�C�X�쐬�̂��߂̏����ݒ�(2)�B
	// �����ł̓T�[�t�F�C�X�� ���O�A�t�@�C�����A�T�C�Y�A�z�u�ꏊ ������ɋL�������邾���ŁA�f�o�C�X�֌W�̏����͈�؍s��Ȃ��B
	// �� width, height �Ŏw�肵���T�C�Y�ɉ摜���g��^�k������ēǂݍ��܂��B
	// �� width == 0 & height == 0 �̏ꍇ�́A�t�@�C���摜�̃T�C�Y�ɂȂ�B
	void InitDeviceObjectsFromFile( LPCTSTR name, LPCTSTR filename, DWORD width=0, DWORD height=0, LOADPLACE place=LOADTO_VIDEO_OR_SYSTEM, LPDDPIXELFORMAT pFormat=NULL );

	// �������t�@�C������T�[�t�F�C�X���쐬���邽�߂̏����ݒ�B
	// �����ł̓T�[�t�F�C�X�� ���O�A�f�[�^�A�f�[�^�T�C�Y�A�T�C�Y�A�z�u�ꏊ ������ɋL�������邾���ŁA�f�o�C�X�֌W�̏����͈�؍s��Ȃ��B
	// �� width, height �Ŏw�肵���T�C�Y�ɉ摜���g��^�k������ēǂݍ��܂��B
	// �� width == 0 & height == 0 �̏ꍇ�́A�t�@�C���摜�̃T�C�Y�ɂȂ�B
	void InitDeviceObjectsFromMemory( LPCTSTR name, BYTE* pData, DWORD dwDataSize, DWORD width=0, DWORD height=0, LOADPLACE place=LOADTO_VIDEO_OR_SYSTEM, LPDDPIXELFORMAT pFormat=NULL );

	// ���\�[�X����T�[�t�F�C�X���쐬���邽�߂̏����ݒ�B
	// �����ł̓T�[�t�F�C�X�� ���O�A�f�[�^�A�f�[�^�T�C�Y�A�T�C�Y�A�z�u�ꏊ ������ɋL�������邾���ŁA�f�o�C�X�֌W�̏����͈�؍s��Ȃ��B
	// �� width, height �Ŏw�肵���T�C�Y�ɉ摜���g��^�k������ēǂݍ��܂��B
	// �� width == 0 & height == 0 �̏ꍇ�́A�t�@�C���摜�̃T�C�Y�ɂȂ�B
	void InitDeviceObjectsFromResource( LPCTSTR name, LPCTSTR lpResourceName, LPCTSTR lpResourceType, DWORD width=0, DWORD height=0, LOADPLACE place=LOADTO_VIDEO_OR_SYSTEM, LPDDPIXELFORMAT pFormat=NULL );

	// �T�[�t�F�C�X�̓ǂݍ��݂ƍ\�z�B
	// �� width, height �Ŏw�肵���T�C�Y�ɉ摜���g��^�k������ēǂݍ��܂��B
	HRESULT	RestoreDeviceObjects();

	HRESULT	InvalidateDeviceObjects();		// �T�[�t�F�C�X�̖������i������Ԃ͈ێ��j
	HRESULT	DeleteDeviceObjects();			// �T�[�t�F�C�X�̔j��

	void	SetAlphaConst( int a );			// �Œ胿�l�̓o�^�ia = 0�`255�j
	HRESULT	SetColorKey();					// ������̐F���J���[�L�[�ɐݒ肷��
	HRESULT	SetColorKey( DWORD dwColor );	// �w��F���J���[�L�[�ɐݒ肷��
	void	ColorKeyEnable( bool f=true )	{ this->bColorKeyEnable = f; }		// �J���[�L�[�� ON/OFF�Btrue �� ON�B
	HRESULT GetDC( HDC *phdc );				// �f�o�C�X�R���e�L�X�g�̎擾
	HRESULT ReleaseDC( HDC &hdc );			// �f�o�C�X�R���e�L�X�g�̉��
	HRESULT Clear( LPRECT prc=NULL );		// �T�[�t�F�C�X�̃N���A
	
	// �w��T�[�t�F�C�X�ւ̕`��(1)
	// Appx: CTexture::m_pTexture �֕`�悷��ꍇ�́ACSurface �� LOADTO_VIDEO �ł��邱�ƁB
	// �@�@�@(Blt �͓����t�H�[�}�b�g�łȂ��Ɠ]���ł��Ȃ����ۂ��BDD_OK �͕Ԃ����B�j
	HRESULT Draw( LPDIRECTDRAWSURFACE7 pDest, int x, int y, LPCRECT pSrc=NULL );
	
	// �w��T�[�t�F�C�X�ւ̕`��(2)
	// Appx: CTexture::m_pTexture �֕`�悷��ꍇ�́ACSurface �� LOADTO_VIDEO �ł��邱�ƁB
	// �@�@�@(Blt �͓����t�H�[�}�b�g�łȂ��Ɠ]���ł��Ȃ����ۂ��BDD_OK �͕Ԃ����B�j
	HRESULT Draw( int x, int y, LPCRECT pSrc=NULL )	{
		return Draw( CD3DApplication::pddsBackBuffer, x, y, pSrc );
	}
	
	// �w��T�[�t�F�C�X�ւ̕`��(3)
	// Appx: CTexture::m_pTexture �֕`�悷��ꍇ�́ACSurface �� LOADTO_VIDEO �ł��邱�ƁB
	// �@�@�@(Blt �͓����t�H�[�}�b�g�łȂ��Ɠ]���ł��Ȃ����ۂ��BDD_OK �͕Ԃ����B�j
	HRESULT Draw( CSurface *pDest, int x, int y, LPRECT pSrc=NULL ) {
		return Draw( pDest->pSurface, x, y, pSrc );
	}
	
	// m_nAlphaConst(<255) �𔽉f�������`��i�\�t�g�����j�B
	// �]���悪�A���t�@�����ꍇ�i�e�N�X�`���Ȃǁj�A�]����̃A���t�@��
	// ��ɍő�l�ƂȂ�A�F�l�̕��� m_nAlphaConst �����f�����B
	// �������]���悪 24bit �̏ꍇ�́A�A���t�@�������Ă��Ă��T�|�[�g�͂��Ȃ��B
	HRESULT BlendDraw( LPDIRECTDRAWSURFACE7 pDest, int x, int y, LPCRECT pSrc=NULL );

	// �T�[�t�F�C�X�ւ̕�����`��(1)
	HRESULT	Text( int x, int y, HFONT hFont, LPCTSTR str, COLORREF color=RGB(255,255,255) );

	// �T�[�t�F�C�X�ւ̕�����`��(2)
	HRESULT	Text( int x, int y, HFONT hFont, tstring &str, COLORREF color=RGB(255,255,255) ) {
		return Text(x,y,hFont,str.c_str(),color);
	}
	// �T�[�t�F�C�X�ւ̕�����`��ɕK�v�ȕ��i�h�b�g�j��Ԃ�
	int	GetTextWidthDot( HFONT hFont, LPCTSTR str������ );

	// �T�[�t�F�C�X�ւ̕�����`��(3)�A���`�G�C���A�X�t��
	// ���t�H���g�͖ʐςS�{�̑傫���̂��̂��w�肷�邱�ƁI
	HRESULT	TextSharp( int x, int y, HFONT hFont, LPCTSTR str, COLORREF color=RGB(255,255,255) );

	// �T�[�t�F�C�X�T�C�Y�̎擾
	DWORD GetSurfaceSize();

	CSurface();
	virtual ~CSurface();

public:
	LPDIRECTDRAWSURFACE7	pSurface;

	int				nAlphaConst;			// �A���t�@�l�i�萔 0�`255�j
	DWORD			dwWidth;				// �T�[�t�F�C�X�̕�
	DWORD			dwHeight;				// �T�[�t�F�C�X�̍���
	LOADPLACE		place;					// �T�[�t�F�C�X�̔z�u�ꏊ
	bool			bColorKeyEnable;		// �J���[�L�[���L���Ȃ� true
	DWORD			dwColorKey;				// �J���[�L�[(32bit)
	tstring			strSurfaceName;			// �T�[�t�F�C�X���i�C�Ӂj
	tstring			strFileName;			// �t�@�C�����i�t�@�C����������Ȃ��Ȃ� "" �j
	LPCTSTR			lpResourceName;			// ���\�[�X���i���\�[�X��������Ȃ��Ȃ� NULL�j
	LPCTSTR			lpResourceType;			// ���\�[�X�^�C�v�i���\�[�X��������Ȃ��Ȃ� NULL�j
	BYTE*			pData;					// �f�[�^�ւ̃|�C���^�i����������̐�������Ȃ��Ȃ�NULL�j
	DWORD			dwDataSize;				// �f�[�^�T�C�Y�i����������̐�������Ȃ��Ȃ�0�j
	DDPIXELFORMAT	ddpfReference;			// Create���̃��t�@�����X�t�H�[�}�b�g

protected:
	// �T�[�t�F�C�X�̖��O�A�t�@�C�����A�T�C�Y�A�z�u�ꏊ������ɋL������B
	// ���̑��̃p�����[�^�̓f�t�H���g�l�ɏ���������B
	void	InitParameters( LPCTSTR name, LPCTSTR fname, LPCTSTR lpResourceName, LPCTSTR lpResourceType, BYTE* pData, DWORD dwDataSize, DWORD width, DWORD height, LOADPLACE place, LPDDPIXELFORMAT pFormat );

	// �t�@�C������̃T�[�t�F�C�X�̓ǂݍ��݂ƍ\�z�B
	// �� width, height �Ŏw�肵���T�C�Y�ɉ摜���g��^�k������ēǂݍ��܂��B
	HRESULT	RestoreDeviceObjectsFromFile();

	// �������C���[�W����̃T�[�t�F�C�X�̓ǂݍ��݂ƍ\�z�B
	// �� PNG �̂ݑΉ�
	// �� width, height �Ŏw�肵���T�C�Y�ɉ摜���g��^�k������ēǂݍ��܂��B
	HRESULT	RestoreDeviceObjectsFromFileInMemory();

	// ���\�[�X����̃T�[�t�F�C�X�̓ǂݍ��݂ƍ\�z�B
	// �� PNG �̂ݑΉ��B
	// �� width, height �Ŏw�肵���T�C�Y�ɉ摜���g��^�k������ēǂݍ��܂��B
	HRESULT	RestoreDeviceObjectsFromResource();

	// �r�b�g�}�b�v hBitmap ����T�[�t�F�C�X���\�z����B
	HRESULT	RestoreFromBitmap( HBITMAP hBitmap );

	// JPEG/PNG �f�[�^����T�[�t�F�C�X���\�z����B
	HRESULT	RestoreFromBitmap( BITMAPINFO* pbinfo, BYTE* pbdata );

	// ��̃T�[�t�F�C�X�̍쐬�G
	// �T�C�Y�im_dwWidth �~ m_dwHeight�j�̋�̃T�[�t�F�C�X���쐬����B
	HRESULT Create();

	void	BlendDraw16to16( LPDDSURFACEDESC2 pddsdSrc, LPCRECT prcSrc, LPDDSURFACEDESC2 pddsdDst, int x, int y );
	void	BlendDraw16to24( LPDDSURFACEDESC2 pddsdSrc, LPCRECT prcSrc, LPDDSURFACEDESC2 pddsdDst, int x, int y );
	void	BlendDraw16to32( LPDDSURFACEDESC2 pddsdSrc, LPCRECT prcSrc, LPDDSURFACEDESC2 pddsdDst, int x, int y );
	void	BlendDraw24to16( LPDDSURFACEDESC2 pddsdSrc, LPCRECT prcSrc, LPDDSURFACEDESC2 pddsdDst, int x, int y );
	void	BlendDraw24to24( LPDDSURFACEDESC2 pddsdSrc, LPCRECT prcSrc, LPDDSURFACEDESC2 pddsdDst, int x, int y );
	void	BlendDraw24to32( LPDDSURFACEDESC2 pddsdSrc, LPCRECT prcSrc, LPDDSURFACEDESC2 pddsdDst, int x, int y );
	void	BlendDraw32to16( LPDDSURFACEDESC2 pddsdSrc, LPCRECT prcSrc, LPDDSURFACEDESC2 pddsdDst, int x, int y );
	void	BlendDraw32to24( LPDDSURFACEDESC2 pddsdSrc, LPCRECT prcSrc, LPDDSURFACEDESC2 pddsdDst, int x, int y );
	void	BlendDraw32to32( LPDDSURFACEDESC2 pddsdSrc, LPCRECT prcSrc, LPDDSURFACEDESC2 pddsdDst, int x, int y );
	void	CopyFromBMP32x4( int x, int y, BITMAP* bmp, bool bSkipBlack );

	DWORD	BitCount( DWORD dwNum );		// dwNum �̃r�b�g'1'�̐��𐔂���B
	DWORD	ShiftCount( DWORD dwNum );		// dwNum ��LSB����0�̑�������Ԃ��B�i��F0x0020 �� 5�j

	void	LostCheck();		// �T�[�t�F�C�X�����X�g���Ă��� RestoreDeviceObject()���Ăяo���B
};

	}//Graphics
}//FDK

using namespace FDK::Graphics;
