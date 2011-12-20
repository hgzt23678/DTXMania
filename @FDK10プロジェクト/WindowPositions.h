#pragma once

namespace FDK {
	namespace General {

class WindowPositions
{
public:
	static void	Save();		// �S�E�B���h�E�̈ʒu�ƃT�C�Y��ۑ�
	static void	Load();		// �S�E�B���h�E�̈ʒu�ƃT�C�Y�𕜌�
	static void	Clear();	// �����E�B���h�E��񃊃X�g�̃N���A

	static BOOL	EnumWindowsSaveCallbackMethod( HWND hWnd );
	static BOOL	EnumWindowsLoadCallbackMethod( HWND hWnd );
	
public:
	WindowPositions() 			{ m_pWinPos = m_pWinPosLast = NULL; }
	virtual ~WindowPositions()	{ Clear(); }

protected:
	static struct WinPos
	{
		HWND			hWnd;
		WINDOWPLACEMENT	wp;
		RECT			rcMax;			// �ő剻����Ă���E�B���h�E�̂ݗL��
		WinPos			*prev, *next;
	} *m_pWinPos, *m_pWinPosLast;
};

	}//General
}//FDK

using namespace FDK::General;
