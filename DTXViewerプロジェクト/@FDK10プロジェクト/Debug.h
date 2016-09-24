
// Debug - �f�o�b�O���O
//
// ���O�ɂ͈ȉ��̂Q������B
// �@1.�g���[�X�t�@�C��(Out�n) ... �f�o�b�O�R���\�[���{Trace�t�@�C���ɏo�́B
// �@2.���|�[�g�t�@�C��(Msg�n) ... Out�n�{Report�t�@�C���ɏo�́B

#pragma once

#include <tchar.h>
#include <string>
#include <stdlib.h>

#ifndef tstring
using namespace std;
typedef basic_string<TCHAR> tstring;
#endif

namespace FDK {
	namespace General {

#ifndef bDEBUG
 #ifdef _DEBUG
  #define bDEBUG	TRUE
 #else
  #define bDEBUG	FALSE
 #endif
#endif

#ifndef FNAME
# ifdef _UNICODE
# define FNAME	WFunction(__FUNCTION__)
# else
# define FNAME	_T(__FUNCTION__)
# endif
#endif

class Debug
{

public:
	// �������B
	// �@strTraceLog ... �g���[�X���O�t�@�C����
	// �@strReportLog .. ���|�[�g���O�t�@�C����
	static void Init( LPCTSTR strTraceLog, LPCTSTR strReportLog );

	static void IndexUp();		// �s���󔒂��P���₷
	static void IndexDown();	// �s���󔒂��P���炷

	// �g���[�X���O�̏o�́i���t�A�C���f�b�N�X�t�j
	// �@format ... �o�͕�����t�H�[�}�b�g
	static void Out( LPCTSTR format, ...);

	// �֐����t���g���[�X���O�̏o�́i���t�A�C���f�b�N�X�t�j
	// �@funcname ... �֐���
	// �@format ..... �o�͕�����t�H�[�}�b�g
	static void OutFN( LPCTSTR funcname, LPCTSTR format, ...);

	// �w�b�_�i���t�A�C���f�b�N�X�j�Ȃ��g���[�X���O�̏o��
	// �@format ... �o�͕�����t�H�[�}�b�g
	static void OutRaw( LPCSTR format, ...);

	// ���|�[�g�t�@�C���̏o��
	// �@format ... �o�͕�����t�H�[�}�b�g
	static void Msg( LPCTSTR format, ...);

	// �֐��������|�[�g�t�@�C���̏o��
	// �@funcname ... �֐���
	// �@format ..... �o�͕�����t�H�[�}�b�g
	static void MsgFN( LPCTSTR funcname, LPCTSTR format, ...);

	// ���b�Z�[�W�_�C�A���O(OK|CANCEL)�\��
	// �@hWnd ....... �E�B���h�E�n���h��
	// �@title ...... �E�B���h�E�̃^�C�g��
	// �@format ..... �o�͕�����t�H�[�}�b�g
	static void MsgDialog( HWND hWnd, LPCTSTR title, LPCTSTR format, ... );

	static void CutOut( bool b = true )			{ m_b�o�͒�~Out=b;}		// Out() �n�̃t�@�C���o�͂��~�Ȃ�true
	static void CutOutConsole( bool b = true )	{ m_b�o�͒�~OutConsole=b;}	// Out() �n�̃R���\�[���o�͂��~�Ȃ�true
	static void CutMsg( bool b = true )			{ m_b�o�͒�~Msg=b;}		// Msg() �n���o�͒�~�Ȃ�true

	static void METHOD_START( LPCTSTR methodname, LPCSTR format, ...);		// ���\�b�h�J�n
	static void METHOD_END( LPCTSTR methodname );							// ���\�b�h�I��
	static void FUNCTION_START( LPCTSTR funcname, LPCSTR format, ...);		// �֐��J�n
	static void FUNCTION_END( LPCTSTR funcname );							// �֐��I��
	static void WORK_START( LPCTSTR format, ...);							// ���[�N�J�n
	static void WORK_END();													// ���[�N�I��

public:
	Debug() {}	// �R���X�g���N�^

protected:
	// ��������w��Ώۂ֏o�͂���B�w�b�_����B
	static void	OutWithHeadIndex( LPCTSTR str�o�͂��镶���� );

	// ��������w��Ώۂ֏o�͂���B�w�b�_�Ȃ��B
	static void	OutWithoutHeadIndex( LPCTSTR str�o�͂��镶���� );

private:
	static const int MSGLEN = 2048;
	static bool m_b��������;
	static int	m_n�s����;
	static bool	m_b�o�͒�~Out;
	static bool	m_b�o�͒�~OutConsole;
	static bool	m_b�o�͒�~Msg;
	static bool	m_b���߂Ă�TraceLog�o��;
	static bool	m_b���߂Ă�ReportLog�o��;
	static TCHAR m_strTraceLog[ _MAX_PATH ];
	static TCHAR m_strReportLog[ _MAX_PATH ];
};

	}//General
}//FDK

using namespace FDK::General;
