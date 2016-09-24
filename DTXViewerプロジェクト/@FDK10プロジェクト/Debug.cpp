#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <crtdbg.h>
#include <locale.h>
#include <direct.h>
#include "Debug.h"

namespace FDK {
	namespace General {

bool	Debug::m_b�������� = false;
int		Debug::m_n�s���� = 0;
bool	Debug::m_b�o�͒�~Out = false;
bool	Debug::m_b�o�͒�~OutConsole = false;
bool	Debug::m_b�o�͒�~Msg = false;
bool	Debug::m_b���߂Ă�TraceLog�o�� = true;
bool	Debug::m_b���߂Ă�ReportLog�o�� = true;
TCHAR	Debug::m_strTraceLog[ _MAX_PATH ];
TCHAR	Debug::m_strReportLog[ _MAX_PATH ];

void Debug::Init( LPCTSTR strTraceLog, LPCTSTR strReportLog )
{
	// �t�@�C�����̍쐬
	tstring trace = _T("log\\");
	trace += strTraceLog;
	lstrcpyn( m_strTraceLog, trace.c_str(), _MAX_PATH );

	tstring report = _T("log\\");
	report += strReportLog;
	lstrcpyn( m_strReportLog, report.c_str(), _MAX_PATH );

	// log �t�H���_�����쐬�Ȃ���B
	_tmkdir( _T("log") );

	m_b�������� = true;
}

void Debug::Out( LPCTSTR format, ...)
{
	if( ! m_b�������� )
		return;

	if( ! format )
	{
		OutputDebugString( "No output format string !!\n" );
		return;
	}

	TCHAR strMsg[ MSGLEN ];

	// ���b�Z�[�W�����
	va_list	args;
	va_start( args, format );
	_vstprintf_s( strMsg, MSGLEN, format, args );
	va_end( args );

	// �o��
	OutWithHeadIndex( strMsg );
}

void Debug::OutFN( LPCTSTR funcname, LPCTSTR format, ...)
{
	if( ! m_b�������� )
		return;

	if( ! format )
	{
		OutputDebugString( "No output format string !!\n" );
		return;
	}

	if( ! funcname )
		funcname = _T("(no function name)");

	// ���b�Z�[�W�����
	TCHAR strMsg[ MSGLEN ];
	va_list	args;
	va_start( args, format );
	_vstprintf_s( strMsg, MSGLEN, format, args );
	va_end( args );

	// �֐�����������
	tstring buf = funcname;
	buf += _T("; ");
	buf += strMsg;

	// �o��
	OutWithHeadIndex( buf.c_str() );
}

void Debug::OutRaw( LPCTSTR format, ...)
{
	if( ! m_b�������� )
		return;

	if( ! format )
	{
		OutputDebugString( "No output format string !!\n" );
		return;
	}

	TCHAR strMsg[ MSGLEN ];

	// ���b�Z�[�W�����
	va_list	args;
	va_start( args, format );
	_vstprintf_s( strMsg, MSGLEN, format, args );
	va_end( args );

	// �o��
	OutWithoutHeadIndex( strMsg );
}

void Debug::Msg( LPCTSTR format, ...)
{
	if( ! m_b�������� || m_b�o�͒�~Msg )
		return;

	if( ! format )
	{
		OutputDebugString( "No output format string !!\n" );
		return;
	}

	TCHAR strMsg[ MSGLEN ];

	// ���b�Z�[�W�����
	va_list	args;
	va_start( args, format );
	_vstprintf_s( strMsg, MSGLEN, format, args );
	va_end( args );

	// �t�@�C���֏o��
	FILE *fp;
	if( _tfopen_s( &fp, m_strReportLog, m_b���߂Ă�ReportLog�o�� ?_T("wt"):_T("at") ) == 0 )
	{
		_ftprintf_s( fp, _T("%s"), strMsg );
		fclose( fp );
	}
	m_b���߂Ă�ReportLog�o�� = false;

	// Out�n�֏o��
	Out( _T("%s"), strMsg );
}

void Debug::MsgFN( LPCTSTR funcname, LPCTSTR format, ...)
{
	if( ! m_b�������� || m_b�o�͒�~Msg )
		return;

	if( ! format )
	{
		OutputDebugString( "No output format string !!\n" );
		return;
	}

	TCHAR strMsg[ MSGLEN ];

	// ���b�Z�[�W�����
	va_list	args;
	va_start( args, format );
	_vstprintf_s( strMsg, MSGLEN, format, args );
	va_end( args );

	// �֐�����������
	tstring buf = funcname;
	buf += _T("; ");
	buf += strMsg;

	// �t�@�C���֏o��
	FILE *fp;
	if( _tfopen_s( &fp, m_strReportLog, m_b���߂Ă�ReportLog�o�� ?_T("wt"):_T("at") ) == 0 )
	{
		_ftprintf_s( fp, _T("%s"), buf.c_str() );
		fclose( fp );
	}
	m_b���߂Ă�ReportLog�o�� = false;

	// Out�n�֏o��
	Out( _T("%s"), buf.c_str() );
}

void Debug::MsgDialog( HWND hWnd, LPCTSTR title, LPCTSTR format, ... )
{
	if( ! m_b�������� )
		return;

	if( !hWnd || !title || !format )
	{
		OutputDebugString( "MsgDialog parameter error !!\n" );
		return;
	}

	// ���b�Z�[�W�����
	TCHAR strMsg[ MSGLEN ];
	va_list	args;
	va_start( args, format );
	_vstprintf_s( strMsg, MSGLEN, format, args );
	va_end( args );

	// �t�@�C���֏o��
	FILE *fp;
	if( _tfopen_s( &fp, m_strReportLog, m_b���߂Ă�ReportLog�o�� ?_T("wt"):_T("at") ) == 0 )
	{
		_ftprintf_s( fp, _T("%s"), strMsg );
		fclose( fp );
	}
	m_b���߂Ă�ReportLog�o�� = false;

	// �_�C�A���O�o��
	MessageBox( hWnd, strMsg, title, MB_OK );

	// Out�n�֏o��
	Out( _T("%s: %s"), title, strMsg );
}

void Debug::OutWithHeadIndex( LPCTSTR str�o�͂��镶���� )
{
	if( ! m_b�������� || ! str�o�͂��镶���� )
		return;

	tstring buf;

	// �C���f�b�N�X�t��������̍쐬
	time_t �o�͎���;
	TCHAR str����[32];

	time( &�o�͎��� );
	_tctime_s( str����, 32, &�o�͎��� );

	buf = str����;
	buf.erase( 24, 1 );	// ���s�R�[�h���폜
	buf += _T("| ");
	for( int i = 0; i < m_n�s����; i++ )
		buf += _T("+--");
	if( m_n�s���� > 0 )
		buf += _T(" ");
	buf += str�o�͂��镶����;

	// �o��
	OutWithoutHeadIndex( buf.c_str() );
}

void Debug::OutWithoutHeadIndex( LPCTSTR str�o�͂��镶���� )
{
	if( ! m_b�������� || ! str�o�͂��镶���� )
		return;

	// �f�o�b�N�R���\�[���ւ̏o��
	if( ! m_b�o�͒�~OutConsole )
		OutputDebugString( str�o�͂��镶���� );

	// �g���[�X�t�@�C���ւ̏o��
	if( bDEBUG && ! m_b�o�͒�~Out )
	{
		FILE *fp;
		if( _tfopen_s( &fp, m_strTraceLog, m_b���߂Ă�TraceLog�o�� ?_T("wt"):_T("at") ) == 0 )
		{
			_ftprintf_s( fp, _T("%s"), str�o�͂��镶���� );
			fclose( fp );
		}
		m_b���߂Ă�TraceLog�o�� = false;
	}
}

void Debug::IndexUp()
{
	m_n�s����++;
}

void Debug::IndexDown()
{
	if( m_n�s���� > 0 ) m_n�s����--;
}

void Debug::METHOD_START( LPCTSTR methodname, LPCSTR format, ...)
{
	// ���b�Z�[�W�����
	TCHAR strMsg[ MSGLEN ];
	va_list	args;
	va_start( args, format );
	_vstprintf_s( strMsg, MSGLEN, format, args );
	va_end( args );

	// �o�́��C���f�b�N�XUP
	OutFN( methodname, _T("%s"), strMsg );
	IndexUp();
}

void Debug::METHOD_END( LPCTSTR methodname )
{
	IndexDown();
	tstring msg = _T("����(");
	msg += methodname;
	msg += _T(")\n");
	Out( msg.c_str() );
}

void Debug::FUNCTION_START( LPCTSTR funcname, LPCSTR format, ...)
{
	// ���b�Z�[�W�����
	TCHAR strMsg[ MSGLEN ];
	va_list	args;
	va_start( args, format );
	_vstprintf_s( strMsg, MSGLEN, format, args );
	va_end( args );

	// �o�́��C���f�b�N�XUP
	OutFN( funcname, _T("%s"), strMsg );
	IndexUp();
}

void Debug::FUNCTION_END( LPCTSTR funcname )
{
	IndexDown();
	tstring msg = _T("����(");
	msg += funcname;
	msg += _T(")\n");
	Out( msg.c_str() );
}

void Debug::WORK_START( LPCTSTR format, ...)
{
	// ���b�Z�[�W�����
	TCHAR strMsg[ MSGLEN ];
	va_list	args;
	va_start( args, format );
	_vstprintf_s( strMsg, MSGLEN, format, args );
	va_end( args );

	// �o�́��C���f�b�N�XUP
	Out( _T("%s"), strMsg );
	IndexUp();
}

void Debug::WORK_END()
{
	IndexDown();
	Out( _T("����\n") );
}

	}//General
}//FDK
