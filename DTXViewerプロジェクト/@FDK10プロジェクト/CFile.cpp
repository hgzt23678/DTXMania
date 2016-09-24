#include "stdafx.h"
#include "CFile.h"

namespace FDK {
	namespace General {

CFile::CFile()
{
	this->pData = NULL;
	Init();
}

CFile::~CFile()
{
	Init();
}

void	CFile::Init()
{
	SAFE_FREE( this->pData );
	this->strFileName.clear();
	this->strExt[0]						 = _T('\0');
	this->ftLastWriteTime.dwLowDateTime  = 0;
	this->ftLastWriteTime.dwHighDateTime = 0;
	this->dwFileSizeHigh				 = 0;
	this->dwFileSizeLow					 = 0;
}

void	CFile::Term()
{
	Init();
}

bool	CFile::Load( LPCTSTR filename )
{
	// ������
	Init();
	this->strFileName = filename;

	// �t�@�C�����̎擾
	GetExt( filename, this->strExt );
	WIN32_FILE_ATTRIBUTE_DATA fileInfo;
	if( ! ::GetFileAttributesEx( filename, GetFileExInfoStandard, &fileInfo))
		return false;		// ���s

	this->ftLastWriteTime.dwLowDateTime  = fileInfo.ftLastWriteTime.dwLowDateTime;
	this->ftLastWriteTime.dwHighDateTime = fileInfo.ftLastWriteTime.dwHighDateTime;
	this->dwFileSizeHigh = fileInfo.nFileSizeHigh;
	this->dwFileSizeLow  = fileInfo.nFileSizeLow;

	// �t�@�C���̓ǂݍ���
	HANDLE hFile = ::CreateFile( filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
		return false;

	if( this->dwFileSizeLow != 0 )		// 32bit�܂ł̃T�C�Y(2GB)�����ǂ܂Ȃ����ǂ������ (High�͖���)
	{
		DWORD dwReadSize = 0;
		this->pData = (BYTE*) malloc( this->dwFileSizeLow );
	
		if( ! ::ReadFile( hFile, (void*)this->pData, this->dwFileSizeLow, &dwReadSize, NULL ) || dwReadSize != this->dwFileSizeLow )
		{
			CloseHandle( hFile );
			return false;
		}
	}

	// �t�@�C�������
	CloseHandle( hFile );
	return true;
}

void	CFile::GetExt( LPCTSTR filename, LPTSTR ext )
{
	TCHAR strFullPath[_MAX_PATH], strExt[_MAX_EXT], *p;

	if( ! ::GetFullPathName( filename, _MAX_PATH, strFullPath, NULL ) )
	{
		ext[0] = _T('\0');
		return;
	}
	
	_tsplitpath_s( strFullPath, NULL, 0, NULL, 0, NULL, 0, strExt, _MAX_EXT );
	p = &(strExt[0]);
	if( *p == _T('.') )
		p++;

	lstrcpyn( ext, p, _MAX_EXT );
}

	}//General
}//FDK