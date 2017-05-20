#include "stdafx.h"
#include "Debug.h"
#include "SerializeFileIO.h"
#include "CFile.h"
#include "CFileMM.h"

namespace FDK {
	namespace General {

void SerializeFileIO::TEST()
{
	FILE *fp;
	if( _tfopen_s( &fp, _T("SerializeFileIO�e�X�g.bin"), _T("wb") ) == 0 )
	{
		SerializeFileIO::PutDWORD( fp, 0x12345678 );
		SerializeFileIO::PutWORD( fp, 0xabcd );
		SerializeFileIO::PutString( fp, _T("�����B") );
		SerializeFileIO::PutString( fp, _T("����H") );
		LPCTSTR buf = _T("�{���ɂ����������̂ł������Ȃ��́HSoWhy?");
		SerializeFileIO::PutString( fp, buf );
		SerializeFileIO::PutBYTE( fp, 0xef );
		fclose( fp );
	}
	CFile file;
	file.Init();
	if( file.Load( _T("SerializeFileIO�e�X�g.bin") ) )
	{
		DWORD dw;
		BYTE by;
		TCHAR s[128];
		DWORD p = 0;
		dw = 0; SerializeFileIO::GetDWORD( &file, &p, &dw );
		Debug::Out(_T("%08x\n"), dw );
		dw = 0; SerializeFileIO::GetWORD( &file, &p, &dw );
		Debug::Out(_T("%08x\n"), dw );
		SerializeFileIO::GetString( &file, &p, s, 128 );
		Debug::Out(_T("%s\n"), s );
		SerializeFileIO::GetString( &file, &p, s, 3 );
		Debug::Out(_T("%s\n"), s );
		TCHAR buf[1024];
		SerializeFileIO::GetString( &file, &p, buf, 1024 );
		Debug::Out(_T("%s\n"), buf );
		dw = 0; SerializeFileIO::GetBYTE( &file, &p, &by );
		Debug::Out(_T("%08x\n"), by );
	}
}

void SerializeFileIO::PutBYTE( FILE *fp, BYTE b )
{
	_ASSERT( fp );
	fwrite( &b, 1, 1, fp );
}

bool SerializeFileIO::GetBYTE( CFile *file, DWORD *p, BYTE *var )
{
	_ASSERT( file );
	_ASSERT( p );
	_ASSERT( var );

	if( file->GetSize() - *p < 1 ) {	// ����Ȃ�
		*var = 0;
		return false;
	}

	*var = *(file->GetData() + *p);
	*p = *p + 1;
	return true;
}
bool SerializeFileIO::GetBYTE( CFileMM *file, DWORD *p, BYTE *var )
{
	_ASSERT( file );
	_ASSERT( p );
	_ASSERT( var );

	if( file->GetSize() - *p < 1 ) {	// ����Ȃ�
		*var = 0;
		return false;
	}

	*var = *(file->GetData() + *p);
	*p = *p + 1;
	return true;
}
void SerializeFileIO::PutWORD( FILE *fp, WORD w )
{
	_ASSERT( fp );

	BYTE buf[2];
	buf[0] = (BYTE)((w>>8) & 0xFF);
	buf[1] = (BYTE)(w & 0xFF);
	fwrite( buf, 2, 1, fp );
}

bool SerializeFileIO::GetWORD( CFile *file, DWORD *p, DWORD *var )
{
	_ASSERT( file );
	_ASSERT( p );
	_ASSERT( var );

	if( file->GetSize() - *p < 2 ) {	// ����Ȃ�
		*var = 0;
		return false;
	}

	LPBYTE s = file->GetData() + *p;
	*var = (((WORD)(*s))<<8) + (WORD)(*(s+1));
	*p = *p + 2;
	return true;
}

bool SerializeFileIO::GetWORD( CFileMM *file, DWORD *p, DWORD *var )
{
	_ASSERT( file );
	_ASSERT( p );
	_ASSERT( var );

	if( file->GetSize() - *p < 2 ) {	// ����Ȃ�
		*var = 0;
		return false;
	}

	LPBYTE s = file->GetData() + *p;
	*var = (((WORD)(*s))<<8) + (WORD)(*(s+1));
	*p = *p + 2;
	return true;
}

void SerializeFileIO::PutDWORD( FILE *fp, DWORD dw )
{
	_ASSERT( fp );

	BYTE buf[4];
	buf[0] = (BYTE)((dw>>24) & 0xFF);
	buf[1] = (BYTE)((dw>>16) & 0xFF);
	buf[2] = (BYTE)((dw>>8) & 0xFF);
	buf[3] = (BYTE)(dw & 0xFF);
	fwrite( buf, 4, 1, fp );
}

bool SerializeFileIO::GetDWORD( CFile *file, DWORD *p, DWORD *var )
{
	_ASSERT( file );
	_ASSERT( p );
	_ASSERT( var );

	if( file->GetSize() - *p < 4 ) {	// ����Ȃ�
		*var = 0;
		return false;
	}

	LPBYTE src = file->GetData() + *p;
	*var = (((DWORD)(*src))<<24) + (((DWORD)(*(src+1)))<<16) + (((DWORD)(*(src+2)))<<8) + (DWORD)(*(src+3));
	*p = *p + 4;
	return true;
}

bool SerializeFileIO::GetDWORD( CFileMM *file, DWORD *p, DWORD *var )
{
	_ASSERT( file );
	_ASSERT( p );
	_ASSERT( var );

	if( file->GetSize() - *p < 4 ) {	// ����Ȃ�
		*var = 0;
		return false;
	}

	LPBYTE src = file->GetData() + *p;
	*var = (((DWORD)(*src))<<24) + (((DWORD)(*(src+1)))<<16) + (((DWORD)(*(src+2)))<<8) + (DWORD)(*(src+3));
	*p = *p + 4;
	return true;
}

void SerializeFileIO::PutString( FILE *fp, LPCTSTR str )
{
	_ASSERT( fp );
	
	if( str == NULL )		// ������ NULL �Ȃ� �󕶎���("")�Ƃ݂Ȃ��ďo�͂���B
	{
		PutWORD( fp, 0 );
		PutBYTE( fp, 0x00 );
		return;
	}

	// �������inull�܂܂��j�̏o��
	WORD len = lstrlen( str );		// lstrlen() �� TCHAR �P�ʂŕ�������Ԃ�
	PutWORD( fp, len );

	// TCHAR �� BYTE[] �ɒ����� PutBYTE ����
	union {
		TCHAR	w;
		BYTE	b[4];
	} cv;

	for( int i = 0; i < len+1; i++ )
	{
		cv.w = str[i];
		for( int j = 0; j < sizeof(TCHAR); j++ )
			PutBYTE( fp, cv.b[ sizeof(TCHAR) - j - 1 ] );
	}
}

bool SerializeFileIO::GetString( CFile *file, DWORD *p�Ǐo�J�n�ʒu, LPTSTR p������i�[�o�b�t�@, DWORD n������i�[�o�b�t�@�̍ő咷 )
{
	_ASSERT( file );
	_ASSERT( p�Ǐo�J�n�ʒu );
	_ASSERT( p������i�[�o�b�t�@ );
	_ASSERT( n������i�[�o�b�t�@�̍ő咷 != 0 );

	// �������inull�܂܂��j�̎擾
	DWORD len;
	if( ! GetWORD( file, p�Ǐo�J�n�ʒu, &len ) ) {	// �擾���s
		*p������i�[�o�b�t�@ = _T('\0');
		return false;
	}

	// ������̎擾; BYTE[] �Ŏ擾���� TCHAR �ɒ���
	if( file->GetSize() - *p�Ǐo�J�n�ʒu < len ) {	// ����Ȃ�
		*p������i�[�o�b�t�@ = _T('\0');
		return false;
	}
	union {
		TCHAR w;
		BYTE  b[4];
	} cv;

	LPBYTE s = file->GetData() + *p�Ǐo�J�n�ʒu;
	for( DWORD i = 0; i < len+1; i++ )
	{
		for( int j = 0; j < sizeof(TCHAR); j++ )
		{
			BYTE by;
			if( ! GetBYTE( file, p�Ǐo�J�n�ʒu, &by ) ) {
				*(p������i�[�o�b�t�@+i) = _T('\0');
				return false;
			}
			cv.b[ sizeof(TCHAR) - j - 1 ] = by;
		}
		if( i < n������i�[�o�b�t�@�̍ő咷 -1 )
			*(p������i�[�o�b�t�@+i) = cv.w;
		else
			*(p������i�[�o�b�t�@+n������i�[�o�b�t�@�̍ő咷-1) = _T('\0');		// �ő咷���z���������͂��ׂĖ���
	}
	return true;
}

bool SerializeFileIO::GetString( CFileMM *file, DWORD *p, LPTSTR str, DWORD str_maxlen )
{
	_ASSERT( file );
	_ASSERT( p );
	_ASSERT( str );
	_ASSERT( str_maxlen != 0 );

	// �������inull�܂܂��j�̎擾
	DWORD len;
	if( ! GetWORD( file, p, &len ) ) {	// �擾���s
		*str = _T('\0');
		return false;
	}

	// ������̎擾; BYTE[] �Ŏ擾���� TCHAR �ɒ���
	if( file->GetSize() - *p < len ) {	// ����Ȃ�
		*str = _T('\0');
		return false;
	}
	union {
		TCHAR w;
		BYTE  b[4];
	} cv;

	LPBYTE s = file->GetData() + *p;
	for( DWORD i = 0; i < len+1; i++ )
	{
		for( int j = 0; j < sizeof(TCHAR); j++ )
		{
			BYTE by;
			if( ! GetBYTE( file, p, &by ) ) {
				*(str+i) = _T('\0');
				return false;
			}
			cv.b[ sizeof(TCHAR) - j - 1 ] = by;
		}
		if( i < str_maxlen -1 )
			*(str+i) = cv.w;
		else
			*(str+str_maxlen-1) = _T('\0');		// str_maxlen ���z���������͂��ׂĖ���
	}
	return true;
}

	}//General
}//FDK