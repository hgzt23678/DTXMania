#include "stdafx.h"
#include "WindowPositions.h"

namespace FDK {
	namespace General {

#define	NAMELEN		1024		// �N���X���A�E�B���h�E���̍ő啶����

// static �Ȃ��
WindowPositions::WinPos* WindowPositions::m_pWinPos     = NULL;
WindowPositions::WinPos* WindowPositions::m_pWinPosLast = NULL;

// �Ǐ��֐��G�R�[���o�b�N
static BOOL CALLBACK EnumWindowsSaveCallbackFunc( HWND hWnd, LPARAM lParam );
static BOOL CALLBACK EnumWindowsLoadCallbackFunc( HWND hWnd, LPARAM lParam );

void WindowPositions::Clear()
{
	WinPos* p = m_pWinPos;
	while( p )
	{
		WinPos* n = p->next;
		delete p;
		p = n;
	}
	m_pWinPos = m_pWinPosLast = NULL;
}

void WindowPositions::Save()
{
	Clear();
	EnumWindows( (WNDENUMPROC) EnumWindowsSaveCallbackFunc, (LPARAM) NULL );
}

BOOL WindowPositions::EnumWindowsSaveCallbackMethod( HWND hWnd )
{
	TCHAR szWindowName[ NAMELEN ];
	TCHAR szClassName [ NAMELEN ];

	// �E�C���h�E���ƃN���X���̎擾
	GetWindowText( hWnd, szWindowName, NAMELEN );
	GetClassName ( hWnd, szClassName,  NAMELEN );

	// �����Ă���E�C���h�E������ΏۂƂ���
	if( IsWindowVisible( hWnd )
		 && GetWindow( hWnd, GW_OWNER ) == NULL
		 && lstrlen( szWindowName ) > 0
		 && lstrcmp( szClassName, "Progman" ) != 0 )
	{
		// �E�B���h�E�̈ʒu�ƃT�C�Y���擾
		WinPos* cell = new WinPos();
		cell->wp.length = sizeof( WINDOWPLACEMENT );
		GetWindowPlacement( hWnd, &(cell->wp) );
		cell->hWnd = hWnd;
		cell->prev = cell->next = NULL;
		if( cell->wp.showCmd & SW_SHOWMAXIMIZED )				// �ő剻����Ă�Ȃ�
			GetWindowRect( hWnd, &cell->rcMax );				// �T�C�Y���擾����
		else
			SetRect( &cell->rcMax, 0, 0, 0, 0 );
		
		// ���X�g�ɐڑ�
		APPENDLIST( m_pWinPos, m_pWinPosLast, cell );
	}
	return TRUE;
}

void WindowPositions::Load()
{
	EnumWindows( (WNDENUMPROC) EnumWindowsLoadCallbackFunc, (LPARAM) NULL );
}

BOOL WindowPositions::EnumWindowsLoadCallbackMethod( HWND hWnd )
{
	TCHAR szWindowName[ NAMELEN ];

	// �E�B���h�E���̎擾
	GetWindowText( hWnd, szWindowName, NAMELEN );

	// ���ׂẴE�B���h�E�ɂ���...
	for( WinPos* cell = m_pWinPos; cell != NULL; cell = cell->next )
	{
		if( cell->hWnd == hWnd )
		{
			// ���̏ꏊ�ɖ߂�
			SetWindowPlacement( hWnd, &(cell->wp) );
			if( cell->wp.showCmd == SW_SHOWMAXIMIZED )		// �ő剻����Ă��Ȃ�T�C�Y����������
			{
				int w = cell->rcMax.right  - cell->rcMax.left;
				int h = cell->rcMax.bottom - cell->rcMax.top;
				MoveWindow( hWnd, cell->rcMax.left, cell->rcMax.top, w, h, TRUE );
			}
			break;
		}
	}
	return TRUE;
}

static BOOL CALLBACK EnumWindowsSaveCallbackFunc( HWND hWnd, LPARAM lParam )
{
	return WindowPositions::EnumWindowsSaveCallbackMethod( hWnd );
}
static BOOL CALLBACK EnumWindowsLoadCallbackFunc( HWND hWnd, LPARAM lParam )
{
	return WindowPositions::EnumWindowsLoadCallbackMethod( hWnd );
}

	}//General
}//FDK