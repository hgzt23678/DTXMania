#pragma once

#include "CTimer.h"
#include "CDirectInput.h"
#include "DTX.h"
#include "CStagePlay.h"
#include "CD3DMyApplication.h"

#define	COMLINESIZE		2048

namespace DTXViewer {
	namespace General {

extern CTimerEx				g_Timer;
extern CDirectInput			g_DirectInput;
extern DTX					g_DTX;
extern CD3DMyApplication	g_App;
extern CStagePlay			g_StagePlay;
extern tstring				g_strModulePath;
extern bool					g_bAction;						// true �Ȃ�A���̃t�F�[�Y�� Action() �����s�����B
extern TCHAR				g_strCmdLine[ COMLINESIZE ];	// �R�}���h���C�������i��M������j
extern TCHAR				g_strFileName[ _MAX_PATH ];		// ��M������Ŏw�肳�ꂽ�t�@�C����
extern bool					g_bFullcolor;					// 32bpp �Ȃ� true

	}//General
}//DTXViewer
extern INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow );
