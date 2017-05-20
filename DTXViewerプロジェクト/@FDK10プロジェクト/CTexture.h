#pragma once

namespace FDK {
	namespace Graphics {

#define MAX_NUM_FORMATS		64		// �񋓂���ő�̃s�N�Z���t�H�[�}�b�g��

class CTexture
{
public:
	// ��e�N�X�`���쐬�̂��߂̏����ݒ�(1)�B
	// �����ł̓e�N�X�`���� ���O�A�T�C�Y�A�z�u�ꏊ ������ɋL�������邾���ŁA�f�o�C�X�֌W�̏����͈�؍s��Ȃ��B
	HRESULT InitDeviceObjects( TCHAR* name, DWORD width, DWORD height, bool bSkipBlack=true );
	
	// ��e�N�X�`���쐬�̂��߂̏����ݒ�(2)�B
	// �����ł̓e�N�X�`���� ���O�A�t�@�C�����A�T�C�Y�A�z�u�ꏊ ������ɋL�������邾���ŁA�f�o�C�X�֌W�̏����͈�؍s��Ȃ��B
	// �� width, height �Ŏw�肵���T�C�Y�ɉ摜���g��^�k������ēǂݍ��܂��B
	// �� width == 0 & height == 0 �̏ꍇ�́A�t�@�C���摜�̃T�C�Y�ɂȂ�B
	HRESULT InitDeviceObjectsFromFile( LPCTSTR name, LPCTSTR filename, DWORD width=0, DWORD height=0, bool bSkipBlack=true );

	// �������t�@�C������e�N�X�`�����쐬���邽�߂̏����ݒ�B
	// �����ł̓e�N�X�`���� ���O�A�f�[�^�A�f�[�^�T�C�Y�A�T�C�Y�A�z�u�ꏊ ������ɋL�������邾���ŁA�f�o�C�X�֌W�̏����͈�؍s��Ȃ��B
	// �� width, height �Ŏw�肵���T�C�Y�ɉ摜���g��^�k������ēǂݍ��܂��B
	// �� width == 0 & height == 0 �̏ꍇ�́A�t�@�C���摜�̃T�C�Y�ɂȂ�B
	HRESULT InitDeviceObjectsFromFileInMemory( TCHAR* name, BYTE* pData, DWORD dwDataSize, DWORD width=0, DWORD height=0, bool bSkipBlack=true );

	// ���\�[�X����e�N�X�`�����쐬���邽�߂̏����ݒ�B
	// �����ł̓e�N�X�`���� ���O�A�f�[�^�A�f�[�^�T�C�Y�A�T�C�Y�A�z�u�ꏊ ������ɋL�������邾���ŁA�f�o�C�X�֌W�̏����͈�؍s��Ȃ��B
	// �� width, height �Ŏw�肵���T�C�Y�ɉ摜���g��^�k������ēǂݍ��܂��B
	// �� width == 0 & height == 0 �̏ꍇ�́A�t�@�C���摜�̃T�C�Y�ɂȂ�B
	HRESULT	InitDeviceObjectsFromResource( TCHAR* name, LPCTSTR lpResourceName, LPCTSTR lpResourceType, DWORD width=0, DWORD height=0, bool bSkipBlack=true );

	// �e�N�X�`���̓ǂݍ��݂ƍ\�z�B
	// �� width, height �Ŏw�肵���T�C�Y�ɉ摜���g��^�k������ēǂݍ��܂��B
	HRESULT	RestoreDeviceObjects();

	HRESULT	InvalidateDeviceObjects();		// �e�N�X�`���̖������B
	HRESULT	DeleteDeviceObjects();			// �e�N�X�`���̔j��

	HRESULT	SetAlphaConst( int a );													// �Œ胿�l�̓o�^(a = 0�`255)
	HRESULT	AddBlendEnable( bool f=true )	{ this->bUseAddBlend = f; return S_OK; }
	HRESULT	SetColorKey();															// ������̐F���J���[�L�[�ɂ���B
	HRESULT	SetColorKey( DWORD dwColor );											// �w��F���J���[�L�[�ɐݒ肷��B
	void	ColorKeyEnable( bool f=true )	{ this->bColorKeyEnable = f; }			// �J���[�L�[�� ON/OFF�Btrue �� ON�B
	HRESULT GetDC( HDC *phdc );														// �f�o�C�X�R���e�L�X�g�̎擾
	HRESULT ReleaseDC( HDC &hdc );													// �f�o�C�X�R���e�L�X�g�̉��
	HRESULT	SetScale( float w, float h );											// �X�P�[���̕ύX
	void	GetScale( float* w, float* h ) { *w=this->fScaleW; *h=this->fScaleH; }	// �X�P�[���̎擾

	HRESULT Clear( LPRECT prc=NULL );												// �T�[�t�F�C�X�̃N���A
	HRESULT Draw( int x, int y, LPRECT pSrc=NULL );									// �Q�c�`��F�o�b�N�o�b�t�@�ւ̏������݂����T�|�[�g���Ȃ�
	HRESULT Draw3D( int x, int y, LPRECT prcSrc, D3DXMATRIX *pMat );				// �R�c��Ԃւ̕`��F�o�b�N�o�b�t�@�ւ̏������݂����T�|�[�g���Ȃ�

	// �e�N�X�`���ւ̕�����̕`��(1)
	HRESULT	Text( int x, int y, HFONT hFont, LPCTSTR str, COLORREF color=RGB(255,255,255) );
	int		GetTextWidthDot( HFONT hFont, LPCTSTR str������ );

	// �e�N�X�`���ւ̕�����̕`��(2)�A���`�G�C���A�X�t��
	// ���t�H���g�͖ʐςS�{�̑傫���̂��̂��w�肷�邱�ƁI
	// ���A���t�@�͏��255�Ȃ̂Œ��ӂ��邱�ƁI�i�X�L�b�v����鍕�����͏����j
	HRESULT	TextSharp( int x, int y, HFONT hFont, LPCTSTR str, COLORREF color=RGB(255,255,255) );

	// �e�N�X�`���T�[�t�F�C�X�T�C�Y�̎擾
	DWORD	GetSurfaceSize();

	static void EnumTextureFormats();	// �e�N�X�`���t�H�[�}�b�g���X�g�̍쐬
	static void BeginScene();			// �`��J�n
	static void EndScene();				// �`��I��

	CTexture();
	virtual ~CTexture();

public:
	LPDIRECTDRAWSURFACE7	pTexture;

	int				nAlphaConst;
	bool			bUseAddBlend;
	bool			bColorKeyEnable;
	DWORD			dwColorKey;
	DWORD			dwWidth;		// �e�N�X�`���̎��T�C�Y
	DWORD			dwHeight;	
	DWORD			dwImageWidth;	// �ǂݍ��݌��摜�̃T�C�Y
	DWORD			dwImageHeight;
	float			fScaleW;		// �������{��
	float			fScaleH;		// �c�����{��
	tstring			strTextureName;					// �e�N�X�`�����i�C�Ӂj
	tstring			strFileName;					// �t�@�C�����i�t�@�C����������Ȃ��Ȃ� "" �j
	bool			bSkipBlack;		// �摜�\�z��(RestoreDevice�`) �ɁA�摜�̍�(0)���X�L�b�v���邩(BMP,JPEG�̂ݗL��)
	LPCTSTR			lpResourceName;					// ���\�[�X���i���\�[�X��������Ȃ��Ȃ� NULL�j
	LPCTSTR			lpResourceType;					// ���\�[�X�^�C�v�i���\�[�X��������Ȃ��Ȃ� NULL�j
	BYTE*			pData;							// �f�[�^�ւ̃|�C���^�i����������̐�������Ȃ��Ȃ�NULL�j
	DWORD			dwDataSize;						// �f�[�^�T�C�Y�i����������̐�������Ȃ��Ȃ�0�j

	static DDPIXELFORMAT	ddpfARGB8888;				// A8R8G8B8 (32-bit) �t�H�[�}�b�g�BdwSize = 0 �Ȃ�g���Ȃ��B
	static DDPIXELFORMAT	ddpfARGB4444;				// A4R4G4B4 (16-bit) �t�H�[�}�b�g�BdwSize = 0 �Ȃ�g���Ȃ��B

protected:
	// ��̃e�N�X�`���̍쐬
	HRESULT Create();
	
	// �����_�����O�X�e�[�^�X�̐ݒ�
	void	SetRenderStates();

	// RGB-32bit �� BITMAP ����e�N�X�`���փR�s�[����B
	// �e�N�X�`���̃A���t�@�� 0xFF �ƂȂ�B
	// �@bSkipBlack ... true �Ȃ獕�F�͕`�悹���X�L�b�v����B
	void	CopyFromBMP32( int x, int y, BITMAP* bmp, bool bSkipBlack=true );

	// RGB-32bit �̂S�{�ʐ� BITMAP ����e�N�X�`���֏k���R�s�[����B
	// �k���́A2x2�̂S�h�b�g���P�h�b�g�ϊ��ōs����B
	// ���񂽂߁A�e�N�X�`���̃A���t�@�� 0%, 25%, 50%, 75%, 100% �̂T��ނƂȂ�B
	// �@bSkipBlack ... true �Ȃ獕�F�͕`�悹���X�L�b�v����B
	void	CopyFromBMP32x4( int x, int y, BITMAP* bmp, bool bSkipBlack=true );

	// �e�N�X�`���̖��O�A�t�@�C�����A�T�C�Y�A�z�u�ꏊ������ɋL������B
	// ���̑��̃p�����[�^�̓f�t�H���g�l�ɏ���������B
	void	InitParameters( LPCTSTR name, LPCTSTR fname, LPCTSTR lpResourceName, LPCTSTR lpResourceType, BYTE* pData, DWORD dwDataSize, DWORD width, DWORD height, bool bSkipBlack );

	// �t�@�C������̃e�N�X�`���̍\�z�B
	// �� width, height �Ŏw�肵���T�C�Y�ɉ摜���g��^�k������ēǂݍ��܂��B
	HRESULT	RestoreDeviceObjectsFromFile();

	// �������C���[�W����̃e�N�X�`���̓ǂݍ��݂ƍ\�z
	// �� PNG �̂ݑΉ�
	// �� width, height �Ŏw�肵���T�C�Y�ɉ摜���g��^�k������ēǂݍ��܂��B
	HRESULT	RestoreDeviceObjectsFromFileInMemory();

	// ���\�[�X����̃e�N�X�`���̓ǂݍ��݂ƍ\�z
	// �� PNG �̂ݑΉ�
	// �� width, height �Ŏw�肵���T�C�Y�ɉ摜���g��^�k������ēǂݍ��܂��B
	HRESULT	RestoreDeviceObjectsFromResource();

	// DDS ����e�N�X�`�����\�z����B
	HRESULT RestoreFromDDS( DDSURFACEDESC2* pddsd, BYTE** ppimage );

	DWORD	BitCount( DWORD dwNum );		// dwNum �̃r�b�g'1'�̐��𐔂���B
	DWORD	ShiftCount( DWORD dwNum );		// dwNum ��LSB����0�̑�������Ԃ��B�i��F0x0020 �� 5�j

	void LostCheck();		// �T�[�t�F�C�X�����X�g���Ă���ꍇ�� RestoreDeviceObjest() ���Ăяo���B
};

	}//Graphics
}//FDK

using namespace FDK::Graphics;
