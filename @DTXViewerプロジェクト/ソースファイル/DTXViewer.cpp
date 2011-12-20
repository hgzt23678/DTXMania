#include "stdafx.h"
// FDK includes
#include "CTimer.h"
#include "CDirectInput.h"
#include "DTX.h"
#include "Debug.h"
#include "CommandLineParser.h"
// DTXViewer includes
#include "DTXViewer.h"
#include "CD3dMyApplication.h"

namespace DTXViewer {
	namespace General {

CTimerEx			g_Timer;
CDirectInput		g_DirectInput;
DTX					g_DTX;
CD3DMyApplication	g_App;
CStagePlay			g_StagePlay;
tstring				g_strModulePath;
bool				g_bAction;						// true �Ȃ�A���̃t�F�[�Y�� Action() �����s�����B
TCHAR				g_strCmdLine[ COMLINESIZE ];	// �R�}���h���C�������i��M������j
TCHAR				g_strFileName[ _MAX_PATH ];		// ��M������Ŏw�肳�ꂽ�t�@�C����
bool				g_bFullcolor;					// 32bpp �Ȃ� true

	}//General
}//DTXViewer

static void s_tEXE�̂���p�X���擾�������ֈړ�����()
{
	TCHAR buf[ _MAX_PATH ];
	TCHAR strDrive[ _MAX_DRIVE ];
	TCHAR strDir[ _MAX_DIR ];

	::GetModuleFileName( NULL, buf, _MAX_PATH );
	_tsplitpath_s( buf, strDrive, _MAX_DRIVE, strDir, _MAX_DIR, NULL, 0, NULL, 0 );

	g_strModulePath  = strDrive;
	g_strModulePath += strDir;		// strDir �̖����ɂ� / ���t���Ă���
	::SetCurrentDirectory( g_strModulePath.c_str() );

	//	Debug::Out( _T("Init(); ���W���[���p�X = \"%s\"\n"), g_strModulePath.c_str() );
	//	--> DEBUG ���� c:\ �ɏo�͂���Ă��܂��B(g_Config �O������B) RELEASE ���͑��v�� �c���Ȃ��悤���B
}
//
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow )
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );	// �v���O�����I�����Ɏ����I�� _CrtDumpMemoryLeaks() ���Ăяo���悤�ɂ���ݒ�B 	
	_CrtSetBreakAlloc( 0 );												// �ȉ��̊֐��Ń����������񐔂��w�肷��΁A�����Ƀu���[�N�|�C���g���ݒ肳���B

	g_bFullcolor = false;

	s_tEXE�̂���p�X���擾�������ֈړ�����();

	Debug::Init( _T("trace.txt"), _T("DTXViewer.txt") );	// �f�o�b�O�֐��������i�J�����g�t�H���_���ړ����Ă���ĂԂ��Ɓj
	Debug::CutOut( true );									// trace.txt �̏o�̓J�b�g
	Debug::CutOutConsole( false );							// �f�o�b�O�o�͂̏o�̓J�b�g
	Debug::CutMsg( false );									// DTXViewer.txt �̏o�̓J�b�g

	// ��M���b�Z�[�W�F�Ȃ�
	g_bAction = false;

	// �R�}���h���C���� g_strCmdLine[] �փR�s�[
	lstrcpyn( g_strCmdLine, lpCmdLine, COMLINESIZE );
	Debug::Out(_T("�N���R�}���h���C���F\"%s\"\n"), g_strCmdLine );

	// ���� DTXV ���N�����Ă��邩�T���A�N�����Ă����� COPYDATASTRUCT ��
	// �����ɃR�}���h���C����������ہX�����ďI������B
	HWND hDTXV;
	if( ( hDTXV = FindWindow( "DTXVIEWERCLASS", NULL ) ) )		// �N���X���Ō���
	{
		// ������𑗐M
	    COPYDATASTRUCT cds;
	    cds.dwData = 0;
		cds.lpData = (void*) g_strCmdLine;
		cds.cbData = lstrlen( g_strCmdLine ) + 1; //  �I�[�� '\0' ������
		SendMessage( hDTXV, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds );
		Debug::Out(_T("DTXV �����݂��Ă���̂ŁA�R�}���h���C���u%s�v�𑗐M���܂����B\n"), g_strCmdLine );
		return 0;		// �I��
	}

	// ���Ȃ������̂Ŏ����ŏ�������B
	g_bAction = true;

	// ��ɃR�}���h���C������͂��A'-F' �I�v�V���������邩�ǂ����𒲂ׂ�B
	CommandLineParser clp;
	clp.Init( g_strCmdLine );
	int num = clp.GetItemNum();
	for( int i = 0; i < num; i++ ) {
		if( strncmp( _T("-F"), clp.Get( i ), _MAX_PATH ) == 0 || strncmp( _T("-f"), clp.Get( i ), _MAX_PATH ) == 0 ) {
			g_bFullcolor = true;
			break;
		}
	}
	clp.Term();

	// �E�B���h�E�쐬�̂̂����C�����[�v��
	if( FAILED( g_App.Create( hInst, (LPCTSTR)lpCmdLine ) ) )
		return 0;

	return g_App.Run();
}
