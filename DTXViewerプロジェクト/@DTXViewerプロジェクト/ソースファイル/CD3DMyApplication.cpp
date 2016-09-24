#include "stdafx.h"
// FDK includes
#include "Activity.h"
#include "Debug.h"
#include "FDKError.h"
#include "CTimer.h"
#include "CSoundManager.h"
#include "CDirectInput.h"
#include "CTexture.h"
// DTXViewer includes
#include "../resource.h"
#include "DTXViewer.h"
#include "Version.h"
#include "DirectXVersion.h"
#include "CD3DMyApplication.h"
#include "CStagePlay.h"

namespace DTXViewer {
	namespace General {

static CD3DMyApplication* s_pApp = NULL;
LRESULT CALLBACK MyWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( s_pApp ) return s_pApp->WndProc( hWnd, uMsg, wParam, lParam );
	return 0;
}
//
CD3DMyApplication::CD3DMyApplication() : CD3DApplication()
{
	s_pApp = this;
}
HRESULT	CD3DMyApplication::Create( HINSTANCE hInst, LPCTSTR strCmdLine )
{
	this->hInstance		= hInst;

	// �������̓o�^
	{
		this->b�S��ʃ��[�h			= false;
		this->nWidth				= 640;
		this->nHeight				= 480;
		this->nBpp					= ( g_bFullcolor ) ? 32 : 16;
		this->b�����A������			= false;
		this->dw�`��Ԋu			= 2;
		this->strWindowTitle		= _T("DTXViewer" VERSION );
		this->strWindowClass		= _T("DTXVIEWERCLASS" );
		this->dw�E�B���h�E�X�^�C��	= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX;
	}
	//
	// �E�B���h�E�̐V�K����
	{
		WNDCLASS wndClass = {
			0, MyWndProc, 0, 0, hInst,
			LoadIcon( this->hInstance, MAKEINTRESOURCE( IDI_DTXVIEWER ) ),
			LoadCursor( NULL, IDC_ARROW ),
			(HBRUSH)GetStockObject( WHITE_BRUSH ),
			NULL, this->strWindowClass
		};
		RegisterClass( &wndClass );

		RECT rc = { 0, 0, this->nWidth, this->nHeight };
		::AdjustWindowRect( &rc, this->dw�E�B���h�E�X�^�C��, FALSE );

		this->hWnd = CreateWindow(
			this->strWindowClass, this->strWindowTitle,
			(this->b�S��ʃ��[�h) ? this->dw�S��ʃX�^�C�� : this->dw�E�B���h�E�X�^�C��, 
			CW_USEDEFAULT, CW_USEDEFAULT,
			(rc.right-rc.left), (rc.bottom-rc.top), 0L,
			NULL, hInst, 0L );	

		if( this->hWnd == NULL ) {
			Debug::Msg( _T("Error: ���C���E�B���h�E�̍쐬�Ɏ��s���܂����B(%08x)\n"), GetLastError() );
			return E_FAIL;
		}
	}
	//
	// DirectX �����^�C���̃o�[�W�����`�F�b�N
	{
		DirectXVersion	dxver;

		dxver.tDirectX�̃o�[�W���������擾����();
		if( dxver.dwMajor < 7 ) {
			Debug::MsgDialog( this->hWnd, _T("DTXViewer runtime error"), _T("Error: DTXViewer �̎��s�ɂ́ADirectX 7.0a �ȏ�̃����^�C�����C�u�������K�v�ł��B\n") );
			return E_FAIL;
		}
	}
	//
	// �e�N���X�̌Ăяo��
	{
		HRESULT hr;
		if( FAILED( hr = CD3DApplication::Create( hInst, strCmdLine ) ) )
			return hr;
	}
	//
	// �ŏ��̃X�e�[�W�̐ݒ�;
	// ��LCreate()�Ńg�b�v���x��Activity �� RestoreDeviceObjects�܂Ŋ�������̂ŁA
	// �����ł悤�₭�g�b�v���x��Activity�i�ŏ��̃X�e�[�W�j�� Activate �ł���B
	{
		g_StagePlay.Activate();
		g_Timer.Flush();
	}
	return S_OK;
}
HRESULT CD3DMyApplication::OneTimeSceneInit()
{
	HRESULT hr;

	this->b���t�J�n���ɃE�B���h�E��O�ʂɎ����Ă��� = true;

	// �����̏�����
	{
		srand( (unsigned) time( NULL ) );
	}
	//
	// �^�C�}�̏�����;
	// CD3DApplication �̐������� CD3CMyApplication::timer �� Init() ����Ă��܂��Ă���̂ŁA
	// �^�C�}��ʂ��w�肷�邽�߂ɁA��x Term() ���� Init() ���Ȃ����B
	{
		g_Timer.Term();
		g_Timer.Init( TIMERTYPE_TIMEGTTIME );
	}
	//
	// DirectInput �̏�����
	{
		if( FAILED( hr = g_DirectInput.Init( this->hWnd ) ) )
		{
			Debug::Msg( _T("Error: DirectInput �̏������Ɏ��s���܂����B(%s)\n"), HRMSG(hr) );
			return hr;
		}
	}
	//
	// �g�b�v���x��Activity �̓o�^�Ə�����
	{
		this->actTopLevel.AddChild( &g_StagePlay );
		this->actTopLevel.OneTimeSceneInit();
	}
	//
	return S_OK;
}
HRESULT CD3DMyApplication::FinalCleanup()
{
	// �g�b�v���x��Activity �̏I��
	{
		this->actTopLevel.FinalCleanup();
	}
	//
	// DirectInput �̏I��
	{
		g_DirectInput.Term();
	}
	//
	// �^�C�}�̏I��
	{
		g_Timer.Term();
	}
	//
	return S_OK;
}
HRESULT CD3DMyApplication::InitDeviceObjects()
{
	// �e�N�X�`���t�H�[�}�b�g�̎��O���������O�擾
	CTexture::EnumTextureFormats();

	// �g�b�v���x�� Activity �̃f�o�C�X�ˑ��I�u�W�F�N�g�̏�����
	this->actTopLevel.InitDeviceObjects();

	return S_OK;
}
HRESULT CD3DMyApplication::DeleteDeviceObjects()
{
	// �g�b�v���x�� Activity �̃f�o�C�X�ˑ��I�u�W�F�N�g�̔j��
	this->actTopLevel.DeleteDeviceObjects();

	return S_OK;
}
HRESULT CD3DMyApplication::RestoreDeviceObjects()
{
	// �g�b�v���x�� Activity �̃f�o�C�X�ˑ��I�u�W�F�N�g�̍\�z
	this->actTopLevel.RestoreDeviceTextures();
	this->actTopLevel.RestoreDeviceSurfaces();
	
	return S_OK;
}
HRESULT CD3DMyApplication::InvalidateDeviceObjects()
{
	// �g�b�v���x�� Activity �̃f�o�C�X�ˑ��I�u�W�F�N�g�̉��
	this->actTopLevel.InvalidateDeviceObjects();

	return S_OK;
}

//
LRESULT CD3DMyApplication::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// �K�v����΂����Ƀ��b�Z�[�W�n���h�����O�R�[�h��ǉ�����B
	switch( uMsg )
	{
	case WM_COPYDATA:
		{
			if( g_bAction )		// ���ɃA�N�V�����҂��Ȃ��ɂ������������
				Action();

			COPYDATASTRUCT* pcds = (COPYDATASTRUCT*) lParam;		// ��M������� g_strCmdLine �ɃR�s�[
			lstrcpyn( g_strCmdLine, (LPCTSTR)pcds->lpData, COMLINESIZE );
			//SetWindowPos( m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE );	// �E�B���h�E���őO�ʂ�
			//SetWindowPos( m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE );
			g_bAction = true;		// �A�N�V���������s����
			break;
		}

	case WM_SYSKEYDOWN:
		if( wParam == VK_F4 )	// Al4 + F4
			SendMessage( this->hWnd, WM_CLOSE, 0, 0 );
		return 0;

	case WM_CLOSE:
		break;	// break �� �����Đe�N���X�̏�����
	}

	return CD3DApplication::WndProc( hWnd, uMsg, wParam, lParam );
}
//	
void	CD3DMyApplication::t�o�b�N�T�[�t�F�C�X���N���A����()
{
	if( this->pddsBackBuffer == NULL )
		return;

	DDBLTFX ddbltfx;
	::ZeroMemory( &ddbltfx, sizeof( ddbltfx ) );
	ddbltfx.dwSize = sizeof( ddbltfx );
	ddbltfx.dwFillColor = 0;
	this->pddsBackBuffer->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
}
bool	CD3DMyApplication::Render()
{
	// �߂�l: true:�A�v���I��, false:�p��

	// ��ԍX�V
	g_Timer.Flush();
	g_DirectInput.Poll();

	// �V�[���`��
	CTexture::BeginScene();
	this->t�o�b�N�T�[�t�F�C�X���N���A����();
	if( g_StagePlay.MainWork() == -1 )
	{
		g_StagePlay.UnActivate();
		CTexture::EndScene();
		return true;		// �A�v���I��
	}
	CTexture::EndScene();

	// �A�N�V��������
	if( g_bAction )
		this->Action();

	return false;
}
//
void CD3DMyApplication::Action()
{
	Debug::Out( _T("��M������ = [%s]\n"), g_strCmdLine );

	// ������Ԃ̏�����
	g_DTX.SetWAVCache( true );
	g_DTX.SetBMPCache( true );

	// �R�}���h���C���������X�g
	struct CmdList {
		TCHAR	str[ _MAX_PATH ];
		CmdList	*prev, *next;
	} *cmdList, *cmdListLast, *cell;
	cmdList = cmdListLast = cell = NULL;

	Debug::Out( _T("��M������ = [%s]\n"), g_strCmdLine );

	// g_strCmdList ���R�}���h���C���������X�g�icmdList�j�ɕ������Ċi�[����B
	TCHAR* p = g_strCmdLine;
	while( *p != _T('\0') )
	{
		while( *p==_T(' ') || *p==_T('\t') )
			p++;

		// (1) �I�v�V�����̏ꍇ
		if( *p == _T('-') )
		{
			cell = new CmdList();
			int i = 0;
			while( *p != _T(' ') && *p != _T('\t') && *p != _T('\0') )
				cell->str[i++] = *p++;
			cell->str[i] = _T('\0');
		}

		// (2) �t�@�C�����̏ꍇ
		else
		{
			cell = new CmdList();
			int i = 0;
			while( *p != _T('\0') )
			{
				if( *p == _T('-') && i > 0 && (*(p-1) == _T(' ') || *(p-1) == _T('\t') ) ) 	// �I�v�V������������I�[
					break;
				cell->str[i++] = *p++;
			}
			while( i > 0 && ( cell->str[i-1] == _T(' ') || cell->str[i-1] == _T('\t') ) )
				i--;	// �����̋󔒂͑S�폜
			cell->str[i] = _T('\0');

			// �_�u���N�H�e�[�V����("...")�ň͂܂�Ă���ꍇ�͂͂���
			if( cell->str[0] == _T('\"') )
			{
				int i = 1;
				while( cell->str[i] != _T('\"') && cell->str[i] != _T('\0') ) {
					cell->str[i-1] = cell->str[i];
					i++;
				}
				cell->str[i-1] = _T('\0');
			}
		}

		// (1)(2)�ǂ�������X�g�֐ڑ�
		cell->prev = cell->next = NULL;
		APPENDLIST( cmdList, cmdListLast, cell );

		Debug::Out( _T("��M�Z��:�u%s�v\n"), cell->str );
	}

	//
	// �������X�g�̒��Ƀt�@�C����������΂���� g_strFileName �ɃR�s�[����B
	// �t�@�C��������������ꍇ�́A�ŏ��̎w��̂ݗL���Ƃ���B
	for( cell = cmdList; cell != NULL; cell=cell->next )
	{
		if( cell->str[0] != _T('-') )
		{
			lstrcpyn( g_strFileName, cell->str, _MAX_PATH );
			break;	// �ŏ��̂��̂����L���B
		}
	}

	//
	// �������X�g���̃I�v�V���������ԂɃ}�[�N����B
	bool b���t�J�n = false;
	int  n���t�J�n�ʒu = -1;
	for( cell = cmdList; cell != NULL; cell=cell->next )
	{
		switch( cell->str[1] )
		{

		//////////////////
		// -N[num] �c num���߂���̍Đ��J�n

		case _T('N'):
		case _T('n'): {
			
			// num �̎擾
			TCHAR* p = cell->str+2;
			int n���� = 1;
			while( *p == _T('-') ) { n���� *= -1; p++; }	// �}�C�i�X���l��
			n���t�J�n�ʒu = atoi( p ) * n����;
			b���t�J�n = true;	// ���ۂ̉��t�͂��� switch �𔲂������ƂŁc�i���t�O�ɑS���̃I�v�V�������������邽�߁j
			break;
		}

		//////////////////
		// -S �c �Đ���~
		case _T('S'):
		case _T('s'):
			g_StagePlay.Stop();
			break;

/*		//////////////////
		// -R �c �Đ��ĊJ		��������
		case _T('R'):
		case _T('r'):
			g_StagePlay.Replay();
			break;
*/
		//////////////////
		// -W[0|1] �c WAV�L���b�V��
		//   -W0 �� WAV�L���b�V��OFF, ���̑��� ON�B�ȗ����� -W1�B
		case _T('W'):
		case _T('w'):
			g_DTX.SetWAVCache( ( cell->str[2] == _T('0') ) ? false : true );
			break;

		//////////////////
		// -B[0|1] �c BMP�L���b�V��
		//   -B0 �� BMP�L���b�V��OFF, ���̑��� ON�B�ȗ����� -B1�B
		case _T('B'):
		case _T('b'):
			g_DTX.SetBMPCache( ( cell->str[2] == _T('0') ) ? false : true );
			break;

		//////////////////
		// -P[0|1]
		//   -P0 �őO�ʂɂ����Ă��Ȃ��A-P1 �ł����Ă���B�ȗ����� -P1�B
		case _T('P'):
		case _T('p'):
			this->b���t�J�n���ɃE�B���h�E��O�ʂɎ����Ă��� = ( cell->str[2] == _T('0') ) ? false : true;
			break;

		default:
			break;
		}
	}
	//
	// ���t�J�n�H
	if( b���t�J�n )
	{
		if( this->b���t�J�n���ɃE�B���h�E��O�ʂɎ����Ă��� )
			this->t�E�B���h�E��O�ʂɂ����Ă���();

		if( g_StagePlay.Load( g_strFileName ) )
			g_StagePlay.Play( n���t�J�n�ʒu );
	}
	//
	// �R�}���h���C���������X�g�̉��
	cell = cmdList;
	while( cell != NULL )
	{
		CmdList *next = cell->next;
		delete cell;
		cell = next;
	}

	// ��������
	g_bAction = false;
}
void	CD3DMyApplication::t�E�B���h�E��O�ʂɂ����Ă���()
{
	SetWindowPos( this->hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE );
	SetWindowPos( this->hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE );
}
	}//General
}//DTXViewer
