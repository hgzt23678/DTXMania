#include "stdafx.h"
#include "Debug.h"
#include "CD3DDisplay.h"
#include "CTimer.h"
#include "WindowPositions.h"
#include "FDKError.h"
#include "CTexture.h"
#include "CD3DApplication.h"

namespace FDK {
	namespace AppBase {

// static �Ȃ��
LPDIRECTDRAW7			CD3DApplication::pDD = NULL;
LPDIRECTDRAWSURFACE7	CD3DApplication::pddsFrontBuffer = NULL;
LPDIRECTDRAWSURFACE7	CD3DApplication::pddsBackBuffer = NULL;
LPDIRECT3D7				CD3DApplication::pD3D = NULL;
LPDIRECT3DDEVICE7		CD3DApplication::pD3DDevice = NULL;
CD3DDisplay				CD3DApplication::D3DDisplay;
HWND					CD3DApplication::hWnd = NULL;
int						CD3DApplication::nBpp = 16;

// �Ǐ��ϐ�
static CD3DApplication* s_pD3DApp = NULL;		// WndProc�p

// static WndProc
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return ( s_pD3DApp ) ? s_pD3DApp->WndProc( hWnd, uMsg, wParam, lParam ) : DefWindowProc( hWnd, uMsg, wParam, lParam );
}
//
CD3DApplication::CD3DApplication()
{
	// �O���ϐ��̏�����
	s_pD3DApp					= this;

	// �����ϐ��̏�����
	this->nWidth				= 640;
	this->nHeight				= 480;
	this->nBpp					= 16;
	this->b�S��ʃ��[�h			= false;
	this->hWnd					= NULL;
	this->b�����A������			= true;
	this->dw�`��Ԋu			= 3;
	this->bReady				= false;
	this->bActive				= false;
    this->strWindowTitle		= _T("FDK10 Application");
    this->strWindowClass		= _T("FDK10 WindowClass");
	this->dw�E�B���h�E�X�^�C��	= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_VISIBLE;
	this->dw�S��ʃX�^�C��		= WS_POPUP | WS_VISIBLE;
	this->bScreenSaverEnable	= false;
}
HRESULT CD3DApplication::Create( HINSTANCE hInst, LPCTSTR strCmdLine )
{
	HRESULT hr;

	this->hInstance   = hInst;

	// IME �𖳌���
	{
		WINNLSEnableIME( NULL, FALSE );
	}
	//
	// �E�B���h�E�쐬���܂��Ȃ炱���ō쐬����
	{
		if( this->hWnd == NULL )
		{
			WNDCLASS wndClass = {
				0,										// �N���X�X�^�C��
				FDK::AppBase::WndProc,					// ���b�Z�[�W�v���V�[�W��
				0,										// ClsExtra
				0,										// WndExtra
				this->hInstance,						// �C���X�^���X
				NULL,									// �E�B���h�E�A�C�R��
				LoadCursor( NULL, IDC_ARROW ),			// �}�E�X�A�C�R��
				(HBRUSH)GetStockObject( WHITE_BRUSH ),	// �w�i�u���V
				NULL,									// ���j���[��
				this->strWindowClass					// �N���X��
			};
			::RegisterClass( &wndClass );

			RECT rc = { 0, 0, this->nWidth, this->nHeight };
			::AdjustWindowRect( &rc, this->dw�E�B���h�E�X�^�C��, FALSE );
			this->hWnd = ::CreateWindow(
				this->strWindowClass,
				this->strWindowTitle,
				(this->b�S��ʃ��[�h) ? this->dw�S��ʃX�^�C�� : this->dw�E�B���h�E�X�^�C��,
				CW_USEDEFAULT, CW_USEDEFAULT,
				(rc.right-rc.left), (rc.bottom-rc.top), 0L,
				NULL, this->hInstance, 0L );
		}
	}
	//
	// �E�B���h�E�̈�i�X�N���[�����W�j�̎擾�E�ۑ�
	{
		this->t���݂̃E�B���h�E�̃E�B���h�E�̈�ƃN���C�A���g�̈���擾����();
	}
	//
	// �A�_�v�^�^�f�o�C�X�^���[�h�̗�
	{
		if( FAILED( hr = this->D3DDisplay.EnumerateDevices() ) )
			return hr;	// ���s
	}
	//
	// �R�c���̍\�z
	{
		if( FAILED( hr = t3D���̍\�z() ) )
			return hr;
	}
	//
	// �A�v���̈�x�����̏�����
	{
		if( FAILED( hr = OneTimeSceneInit() ) ) {
			t3D���̔j��();
			return hr;
		}
	}
	//
	// �f�o�C�X�ˑ��I�u�W�F�N�g�̏�����
	{
		if( FAILED( hr = InitDeviceObjects() ) ) {
			t3D���̔j��();
			return hr;
		}
	}
	//
	// �f�o�C�X�ˑ��I�u�W�F�N�g�̍\�z
	{
		if( FAILED( hr = RestoreDeviceObjects() ) ) {
			t3D���̔j��();
			return hr;
		}
	}	
	//
	// ��������
	this->bReady = true;

	return S_OK;
}
HRESULT CD3DApplication::t3D���̍\�z()
{
	HRESULT hr;
	
	D3DAdapterInfo* pAdapter = this->D3DDisplay.pAdapter;
	D3DDeviceInfo*  pDevice  = pAdapter->pDevice;

	// ���j�^�̓d���ݒ���ꎞ�I�ɖ����ɂ���
	{
		this->exeState = ::SetThreadExecutionState( ES_DISPLAY_REQUIRED | ES_CONTINUOUS );
	}
	//
	// �X�N���[���Z�[�o�̗L���E�������擾���A���̌�ꎞ�I�ɖ����ɂ���
	{
		HKEY	hKeyScreenSaver = NULL;
		long	lReturn = 0;
		long	lScreenSaver = 0;
		DWORD	dwData = 0;

		if( RegOpenKeyEx( HKEY_CURRENT_USER, TEXT("Control Panel\\Desktop"), 0, KEY_QUERY_VALUE, &hKeyScreenSaver ) == ERROR_SUCCESS )
		{
			this->bScreenSaverEnable = 
				( RegQueryValueEx( hKeyScreenSaver, TEXT("SCRNSAVE.EXE"), NULL, NULL, NULL, &dwData ) == ERROR_SUCCESS ) ? true : false;
		}
		RegCloseKey( hKeyScreenSaver );
		hKeyScreenSaver = NULL;

		// �����ɂ���
		SystemParametersInfo( SPI_SETSCREENSAVEACTIVE, FALSE, 0, SPIF_SENDWININICHANGE );
	}
	//
	// �S�E�B���h�E�ʒu�̕ۑ�
	{
		WindowPositions::Save();
	}
	//
	// �^�C�}�̏�����; �^�C�}��ʂ�ύX����ꍇ�́A��x Term() ���� Init() ���Ȃ������ƁB
	{
		this->timer.Init( TIMERTYPE_TIMEGTTIME );
	}
	//
	// D3DX �̏�����
	{
		if( FAILED( hr = D3DXInitialize() ) )
			return hr;
	}
	//
	// DirectDraw7 �̍쐬
	{
		if( FAILED( hr = DirectDrawCreateEx( NULL, (VOID**)&this->pDD, IID_IDirectDraw7, NULL ) ) ) {
			D3DXUninitialize();
			return hr;
		}
	}
	//
	// �������[�h�̐ݒ�
	{
		if( FAILED( hr = this->pDD->SetCooperativeLevel( this->hWnd, this->b�S��ʃ��[�h ? (DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN) : (DDSCL_NORMAL|DDSCL_NOWINDOWCHANGES) ) ) ) 
		{
			SAFE_RELEASE( this->pDD );
			D3DXUninitialize();
			return hr;
		}
	}
	//
	// �f�B�X�v���C���[�h�̐ݒ�i�S��ʃ��[�h���̂݁j
	{
		if( this->b�S��ʃ��[�h )
		{
			if( FAILED( hr = this->pDD->SetDisplayMode( this->nWidth, this->nHeight, this->nBpp, 0, 0 ) ) )
			{
				SAFE_RELEASE( this->pDD );
				D3DXUninitialize();
				return hr;
			}
		}
	}
	//
	// �v���C�}�����o�b�N�T�[�t�F�C�X�̍쐬
	{
		// a. �S��ʃ��[�h�̏ꍇ
		if( this->b�S��ʃ��[�h )
		{
			DDSURFACEDESC2 ddsd;
			ZeroMemory( &ddsd, sizeof( ddsd ) );
			ddsd.dwSize	= sizeof( ddsd );
			ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
			ddsd.ddsCaps.dwCaps	= DDSCAPS_PRIMARYSURFACE | DDSCAPS_3DDEVICE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
			ddsd.dwBackBufferCount = 1;
			if( FAILED( hr = this->pDD->CreateSurface( &ddsd, &this->pddsFrontBuffer, NULL ) ) ) {
				D3DXUninitialize();
				SAFE_RELEASE( this->pDD );
				return hr;	// �v���C�}���T�[�t�F�C�X�̍쐬�Ɏ��s
			}
			DDSCAPS2 ddscaps;
			ZeroMemory( &ddscaps, sizeof( ddscaps ) );
			ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
			if( FAILED( hr = this->pddsFrontBuffer->GetAttachedSurface( &ddscaps, &this->pddsBackBuffer ) ) )
			{
				D3DXUninitialize();
				SAFE_RELEASE( this->pddsFrontBuffer );
				SAFE_RELEASE( this->pDD );
				return hr;	// �o�b�N�T�[�t�F�C�X�̎擾�Ɏ��s
			}
		}
		//
		// b. �E�B���h�E���[�h�̏ꍇ
		else
		{
			DDSURFACEDESC2 ddsd;
			ZeroMemory( &ddsd, sizeof( ddsd ) );
			ddsd.dwSize = sizeof( ddsd );
			ddsd.dwFlags = DDSD_CAPS;
			ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_3DDEVICE;
			if( FAILED( hr = this->pDD->CreateSurface( &ddsd, &this->pddsFrontBuffer, NULL ) ) ) {
				SAFE_RELEASE( this->pDD );
				D3DXUninitialize();
				return hr;	// �v���C�}���T�[�t�F�C�X�̍쐬�Ɏ��s
			}
			ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
			ddsd.ddsCaps.dwCaps = DDSCAPS_3DDEVICE | DDSCAPS_OFFSCREENPLAIN;
			ddsd.dwWidth = this->nWidth;
			ddsd.dwHeight = this->nHeight;
			if( FAILED( hr = this->pDD->CreateSurface( &ddsd, &this->pddsBackBuffer, NULL ) ) ) {
				SAFE_RELEASE( this->pddsFrontBuffer );
				SAFE_RELEASE( this->pDD );
				D3DXUninitialize();
				return hr;	// �o�b�N�T�[�t�F�C�X�̍쐬�Ɏ��s
			}
		}
	}
	//
	// �N���b�p�[�̍쐬�i�E�B���h�E���[�h���̂݁j
	{
		if( ! this->b�S��ʃ��[�h )
		{
			LPDIRECTDRAWCLIPPER pClipper;
			if( FAILED( hr = this->pDD->CreateClipper( 0, &pClipper, NULL ) ) )
			{
				SAFE_RELEASE( this->pddsBackBuffer );
				SAFE_RELEASE( this->pddsFrontBuffer );
				SAFE_RELEASE( this->pDD );
				D3DXUninitialize();
				return hr;	// �N���b�p�[�̍쐬�Ɏ��s
			}
			pClipper->SetHWnd( 0, this->hWnd );
			this->pddsFrontBuffer->SetClipper( pClipper );
			SAFE_RELEASE( pClipper );
		}
	}
	//
	// Direct3D �̎擾
	{
		if( FAILED( hr = this->pDD->QueryInterface( IID_IDirect3D7, (LPVOID *) &this->pD3D ) ) )
		{
			if( ! this->b�S��ʃ��[�h ) SAFE_RELEASE( this->pddsBackBuffer );
			SAFE_RELEASE( this->pddsFrontBuffer );
			SAFE_RELEASE( this->pDD );
			D3DXUninitialize();
			return hr;	// Direct3D7 �̎擾�Ɏ��s
		}
	}
	//
	// Direct3D �f�o�C�X�̍쐬
	{
		if( FAILED( hr = this->pD3D->CreateDevice( IID_IDirect3DTnLHalDevice, this->pddsBackBuffer, &this->pD3DDevice ) ) )				// TnLHAL �Ńg���C
		{
			if( FAILED( hr = this->pD3D->CreateDevice( IID_IDirect3DHALDevice, this->pddsBackBuffer, &this->pD3DDevice ) ) )			// HAL �Ńg���C
			{
				if( FAILED( hr = this->pD3D->CreateDevice( IID_IDirect3DMMXDevice, this->pddsBackBuffer, &this->pD3DDevice ) ) )		// MMX �Ńg���C
				{
					if( FAILED( hr = this->pD3D->CreateDevice( IID_IDirect3DRGBDevice, this->pddsBackBuffer, &this->pD3DDevice ) ) )	// RGB �Ńg���C
					{
						SAFE_RELEASE( this->pD3D );
						if( ! this->b�S��ʃ��[�h ) SAFE_RELEASE( this->pddsBackBuffer );
						SAFE_RELEASE( this->pddsFrontBuffer );
						SAFE_RELEASE( this->pDD );
						D3DXUninitialize();
						return hr;	// Direct3DDevice �̍쐬�Ɏ��s
					} else
						Debug::Msg( _T("Direct3D RGB �f�o�C�X���쐬���܂����B\n") );
				} else
					Debug::Msg( _T("Direct3D MMX �f�o�C�X���쐬���܂����B\n") );
			} else
				Debug::Msg( _T("Direct3D HAL �f�o�C�X���쐬���܂����B\n") );
		} else
			Debug::Msg( _T("Direct3D T&L HAL �f�o�C�X���쐬���܂����B\n") );
	}
	//
	// �f�o�C�X�Ƀr���[�|�[�g��ݒ�
	{
		D3DVIEWPORT7 vp = { 0, 0, this->nWidth, this->nHeight, 0.0f, 1.0f };
		if( FAILED( hr = this->pD3DDevice->SetViewport( &vp ) ) )
		{
			SAFE_RELEASE( this->pD3DDevice );
			SAFE_RELEASE( this->pD3D );
			if( ! this->b�S��ʃ��[�h ) SAFE_RELEASE( this->pddsBackBuffer );
			SAFE_RELEASE( this->pddsFrontBuffer );
			SAFE_RELEASE( this->pDD );
			D3DXUninitialize();
			return hr;	// �r���[�|�[�g�̐ݒ�Ɏ��s
		}
	}
	//
	// �s�ς̃����_�����O�X�e�[�^�X�̐ݒ�
	{
		this->pD3DDevice->SetRenderState( D3DRENDERSTATE_LIGHTING, FALSE );
		this->pD3DDevice->SetRenderState( D3DRENDERSTATE_ZENABLE, FALSE );
		this->pD3DDevice->SetRenderState( D3DRENDERSTATE_ANTIALIAS, FALSE );
		this->pD3DDevice->SetRenderState( D3DRENDERSTATE_TEXTUREPERSPECTIVE, TRUE );
		this->pD3DDevice->SetRenderState( D3DRENDERSTATE_ALPHATESTENABLE, TRUE );
		this->pD3DDevice->SetRenderState( D3DRENDERSTATE_ALPHAREF, 10 );
		this->pD3DDevice->SetRenderState( D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER );
		this->pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1 );
		this->pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );
		this->pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE );
		this->pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );
		this->pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_DIFFUSE );
		this->pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER,	D3DTFN_POINT );
		this->pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER,	D3DTFN_POINT );
	}
	//
	// �K�X�ύX����郌���_�����O�X�e�[�^�X�̐ݒ�
	{
		this->pD3DDevice->SetRenderState( D3DRENDERSTATE_COLORKEYENABLE, FALSE );
		this->pD3DDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );
		this->pD3DDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA );
		this->pD3DDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA );
	}
	//
	// �ˉe�s����f�t�H���g�̂��̂���ύX
	{
		float d, t, a;
		D3DXMATRIX proj;
		CD3DApplication::GetProjectionParam( &d, &t, &a );
		D3DXMatrixPerspectiveFovLH( &proj, t, 1.0f, 0.0000001f, 50.0f );
		D3DMATRIX pm = proj;
		this->pD3DDevice->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &pm );
	}
	//
	// �}�E�X�J�[�\���\��ON/OFF
	{
		if( this->b�S��ʃ��[�h ) {
			while( ShowCursor( FALSE ) >= 0 )
				Sleep(2);
		} else {
			ShowCursor( TRUE );
		}
	}

	return S_OK;
}
void	CD3DApplication::t3D���̔j��()
{
	this->bActive = false;
	this->bReady  = false;

	// �A�v�����̏I������
	{
		InvalidateDeviceObjects();
		DeleteDeviceObjects();
		FinalCleanup();
	}
	//
	// �f�B�X�v���C���[�h�̕���
	{
		if( this->b�S��ʃ��[�h )
			this->pDD->RestoreDisplayMode();
	}
	//
	// �e DirectX �I�u�W�F�N�g�̉��
	{
		this->pDD->SetCooperativeLevel( this->hWnd, DDSCL_NORMAL | DDSCL_NOWINDOWCHANGES );
		SAFE_RELEASE( this->pD3DDevice );
		SAFE_RELEASE( this->pD3D );
		if( ! this->b�S��ʃ��[�h ) SAFE_RELEASE( this->pddsBackBuffer );
		SAFE_RELEASE( this->pddsFrontBuffer );
		SAFE_RELEASE( this->pDD );
		D3DXUninitialize();
	}
	//
	// �S�E�B���h�E�̈ʒu�𕜌��i�S��ʃ��[�h���̂݁j
	{
		if( this->b�S��ʃ��[�h )
			WindowPositions::Load();
		WindowPositions::Clear();		// �������X�g���
	}
	//
	// �^�C�}�I��
	{
		this->timer.Term();
	}
	//
	// �X�N���[���Z�[�o���L���������Ȃ���Ƃɖ߂�
	{
		if( this->bScreenSaverEnable )
			SystemParametersInfo( SPI_SETSCREENSAVEACTIVE, TRUE, 0, SPIF_SENDWININICHANGE );
	}
	//
	// ���j�^�d���ݒ�̕���
	{
		::SetThreadExecutionState( this->exeState );
	}
}
HRESULT CD3DApplication::t3D���̍č\�z( bool b�V�S��ʃ��[�h )
{
	HRESULT hr;

	this->bReady = false;
	
	// ���ׂẴf�o�C�X�ˑ��I�u�W�F�N�g���������
	{
		InvalidateDeviceObjects();
	}
	//
	// ���ݑS��ʃ��[�h�Ȃ�f�B�X�v���C���[�h�𕜌�����
	{
		if( this->b�S��ʃ��[�h )
			this->pDD->RestoreDisplayMode();
	}
	//
	// �e DirectX �I�u�W�F�N�g�̉��
	{
		this->pDD->SetCooperativeLevel( this->hWnd, DDSCL_NORMAL | DDSCL_NOWINDOWCHANGES );
		SAFE_RELEASE( this->pD3DDevice );
		SAFE_RELEASE( this->pD3D );
		if( ! this->b�S��ʃ��[�h ) SAFE_RELEASE( this->pddsBackBuffer );
		SAFE_RELEASE( this->pddsFrontBuffer );
		SAFE_RELEASE( this->pDD );
		D3DXUninitialize();
	}
	//
	// �E�B���h�E�X�^�C���̐ݒ�
	{
		// �E�B���h�E�X�^�C����ύX�����ꍇ�́ASetWindowsPos() �œ����L���b�V�����N���A����K�v������B
		if( b�V�S��ʃ��[�h )
		{
			::SetWindowLongPtr( this->hWnd, GWL_STYLE, this->dw�S��ʃX�^�C�� );
			::SetWindowPos( this->hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED );
		}
		else
		{
			::SetWindowLongPtr( this->hWnd, GWL_STYLE, this->dw�E�B���h�E�X�^�C�� );
			::SetWindowPos( this->hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		}
	}
	//
	// �S�E�B���h�E�ʒu�ƃT�C�Y�̕����i�S��ʃ��[�h���̂݁j
	{
		if( this->b�S��ʃ��[�h )
		{
			// �E�B���h�E�ʒu����
			WindowPositions::Load();
			WindowPositions::Clear();

			this->t���݂̃E�B���h�E�̃E�B���h�E�̈�ƃN���C�A���g�̈���擾����();
		}
	}
	//
	// �f�o�C�X�̍\�z
	{
		bool b�ۑ� = this->b�S��ʃ��[�h;
		this->b�S��ʃ��[�h = b�V�S��ʃ��[�h;		// t3D���̍\�z() �̑O�� this->b�S��ʃ��[�h �̐ݒ肪�K�v
	
		if( FAILED( hr = this->t3D���̍\�z() ) ) {
			this->b�S��ʃ��[�h = b�ۑ�;
			return hr;
		}
	}
	//
	// �f�o�C�X�ˑ��I�u�W�F�N�g�̕����̑O�ɁAWait a moment �摜���o���Ă����B�i���������������Ƃ��̂��߁j
	{
		CTexture::BeginScene();
		this->tWaitAMoment�̕`��();
		CTexture::EndScene();
		this->tFlip�܂���Blt();
	}
	//
	// �f�o�C�X�ˑ��I�u�W�F�N�g�̕���
	{
		if( FAILED( hr = this->RestoreDeviceObjects() ) )
			return hr;
	}
	//
	// ��������
	this->bReady = true;

	return S_OK;
}
HRESULT CD3DApplication::t3D���̕`��()
{
	HRESULT hr;

	// �������x�����`�F�b�N����
	{
		if( FAILED( hr = this->pDD->TestCooperativeLevel() ) )
		{
			switch( hr )
			{
			case DDERR_EXCLUSIVEMODEALREADYSET:		// �i�������E�B���h�E���[�h�̂Ƃ��j���̃A�v�����r�����[�h�ɓ�����
			case DDERR_NOEXCLUSIVEMODE:				// �r�����������iAlt+TAB�Ȃǁj
				Sleep( 1000 );		// ���̃A�v�����r�����[�h�ɓ����Ă���̂őҋ@����B���ʂ�CPU���g��Ȃ�(2006/5/6)
				return S_OK;

			case DDERR_WRONGMODE:					// �f�B�X�v���C���[�h���ύX���ꂽ�B�S�T�[�t�F�C�X��j�����č�蒼�����K�v�B
				if( this->D3DDisplay.pAdapter->pDevice->bWindowed )
				{
					hr = this->t3D���̍č\�z( this->b�S��ʃ��[�h );
					return hr;
				}
				break;
			}
			return hr;
		}
	}
	//
	// �i�s�`��
	{
		if( this->Render() )
		{
			// Render() �� true ��Ԃ����� WM_CLOSE �𑗐M����B
			SendMessage( this->hWnd, WM_CLOSE, 0, 0 );
			return S_OK;
		}

		this->tFPS���Z�o����();
	}
	//
	// �\��
	{
		if( FAILED( hr = this->tFlip�܂���Blt() ) )
		{
			if( hr == DDERR_SURFACELOST )
			{
				// �T�[�t�F�C�X�����ׂĕ������A�č\�z�B
				this->pDD->RestoreAllSurfaces();
				return this->RestoreDeviceObjects();
			}
			else
				return hr;	// ���̑��̃G���[
		}
	}
	//
	return S_OK;
}
HRESULT CD3DApplication::tFlip�܂���Blt()
{
	HRESULT hr = S_OK;

    if( this->pddsFrontBuffer == NULL )
		return FDKERR_����������ĂȂ�;

    // �S��ʃ��[�h���F
	if( this->b�S��ʃ��[�h )
    {
		if( this->b�����A������ )
		{
			hr = this->pddsFrontBuffer->Flip( NULL, DDFLIP_WAIT );
			return hr;
		}
		else
		{
			return this->t�����������g��Ȃ��E�F�C�g��Blt( NULL );
		}
    }

    // �E�B���h�E���[�h���F
    else
	{
		return this->t�����������g��Ȃ��E�F�C�g��Blt( &this->rc�N���C�A���g�̈� );
	}

	return hr;
}
HRESULT	CD3DApplication::t�����������g��Ȃ��E�F�C�g��Blt( LPRECT prc�N���C�A���g�̈� )
{
	static bool   bBltFast���g�� = true;		// ����̂� BltFast �Ŏ����A�_���Ȃ炻��ȍ~��Blt���g���B
	static double db�O��̕`��J�n���� = INIT_TIME;

	HRESULT hr = S_OK;

	if( db�O��̕`��J�n���� != INIT_TIME )
	{
		double db�o�ߎ��� = this->timer.GetSysTime() - db�O��̕`��J�n����;
		double db�]�莞�� = (double)this->dw�`��Ԋu - db�o�ߎ���;
		if( db�]�莞�� > 0 )
			::Sleep( (DWORD)db�]�莞�� );		// �]�莞�Ԃ�������
	}

retry:
	if( bBltFast���g�� )
	{
		RECT rcBack = { 0, 0, this->nWidth, this->nHeight };
		if( FAILED( hr = this->pddsFrontBuffer->BltFast( 0, 0, this->pddsBackBuffer, &rcBack, DDBLTFAST_WAIT ) ) )
		{
			bBltFast���g�� = false;	// BltFast() �Ɏ��s�����̂ŁA�Ȍ�� Blt() ���g���B
			goto retry;
		}
	}
	else
		hr = this->pddsFrontBuffer->Blt( prc�N���C�A���g�̈�, this->pddsBackBuffer, NULL, DDBLT_WAIT, NULL );

	db�O��̕`��J�n���� = this->timer.GetSysTime();
	
	return hr;
}
//
void	CD3DApplication::t�S��ʁE�E�B���h�E���[�h��؂�ւ���()
{
	HRESULT hr;

	// ���݃E�B���h�E���[�h�Ȃ�S�E�B���h�E�̈ʒu��ۑ�����
	if( ! this->b�S��ʃ��[�h )
		WindowPositions::Save();

	// �f�o�C�X�؂�ւ�
	if( FAILED( hr = t3D���̍č\�z( ! this->b�S��ʃ��[�h ) ) )
	{
		// ���s�����猳�ɖ߂��i�P�񂾂��j
		if( FAILED( hr = t3D���̍č\�z( this->b�S��ʃ��[�h ) ) )
			return;	// ����ł��G���[���o���疳��
	}
}

void	CD3DApplication::t���݂̃E�B���h�E�̃E�B���h�E�̈�ƃN���C�A���g�̈���擾����()
{
	// �E�B���h�E�̈�̎擾
	GetWindowRect( this->hWnd, &this->rc�E�B���h�E�̈� );

	// �N���C�A���g�̈�̎擾
	GetClientRect( this->hWnd, &this->rc�N���C�A���g�̈� );
	ClientToScreen( this->hWnd, (POINT*)&this->rc�N���C�A���g�̈�.left );		// left, top	 ���X�N���[�����W��
	ClientToScreen( this->hWnd, (POINT*)&this->rc�N���C�A���g�̈�.right );		// right, bottom ���X�N���[�����W��
}

void	CD3DApplication::tWaitAMoment�̕`��()
{
/*
	�ȉ��̓T���v��

	CSurface sf;
	sf.InitDeviceObjectsFromFile( _T("Wait a moment"), _T("sysdata/ses_frame.jpg") );
	if( SUCCEEDED( sf.RestoreDeviceObjects() ) )
	{
		sf.Draw( 0, 0 );
		sf.InvalidateDeviceObjects();
	}
	sf.DeleteDeviceObjects();
*/
}

void	CD3DApplication::tFPS���Z�o����()
{
	static double fpstime = INIT_TIME;
	static int fps = 0;

	// ������
	if( fpstime == INIT_TIME )
	{
		fpstime = this->timer.GetSysTime();
		this->nFPS = fps = 0;
	}

	// �P�b�o�߂��ƂɌv�Z
	while( this->timer.GetSysTime() - fpstime >= 1000.0 )
	{
		this->nFPS = fps;
		fps = 0;
		fpstime += 1000.0;
	}
	fps ++;
}
//
INT		CD3DApplication::Run()
{
    MSG  msg;

	while( true )
    {
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			if( msg.message == WM_QUIT )
				break;
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			if( this->bReady && this->bActive )
			{
				if( FAILED( t3D���̕`��() ) ) {
					Debug::Msg( _T("3D���̕`��Ɏ��s���܂����B�I�����܂��B\n") );
					SendMessage( this->hWnd, WM_CLOSE, 0, 0 );
				}
			}
		}
	}
    return (INT)msg.wParam;
}
LRESULT CD3DApplication::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {

	case WM_CLOSE:
		this->bActive = false;
		this->t3D���̔j��();
		break;

	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;

	case WM_SIZE:
		// �E�B���h�E���S���B�ꂽ��ŏ������ꂽ�肵���� this->bActive = false �ɂ���B
		this->bActive = ( wParam == SIZE_MAXHIDE || wParam == SIZE_MINIMIZED ) ? false : true;
		break;

	case WM_MOVE:
		// �V�����̈���W���擾���Ă���
		this->t���݂̃E�B���h�E�̃E�B���h�E�̈�ƃN���C�A���g�̈���擾����();
		break;

	case WM_GETMINMAXINFO:
		// �A�v���̍ŏ��T�C�Y�̖₢���킹�ւ̉񓚁B
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
		return 0;

	case WM_NCHITTEST:
		if( this->b�S��ʃ��[�h )
			return HTCLIENT;		// ���ׂĂ��N���C�A���g�̈�
		break;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
	}//AppBase
}//FDK