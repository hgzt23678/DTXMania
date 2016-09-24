#include "stdafx.h"
#include "str2float.h"

namespace FDK {
	namespace General {

float str2float( LPCTSTR p )
{
	TCHAR buf[ 64 ];
	int nb = 0;
	bool bs = false;		// �����_�����ꂽ�� true

	while( _istdigit( *p ) || *p == _T('.') )
	{
		if( *p == _T('.') ) {
			if( bs ) break;		// �����_���Q��ȏ�ł��炻���ŏI��
			bs = true;
		}
		buf[ nb++ ] = *p++;
		if( nb >= 63 ) break;	// �����񐧌��i�ꉞ�j
	}
	buf[ nb ] = _T('\0');

	// �����ɒ����ĕԊ�
	return (float) _tstof( buf );
}
	}//General
}//FDK