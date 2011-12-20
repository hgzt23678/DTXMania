#include "stdafx.h"
#include "CSetString.h"
#include "FDKError.h"
#include "Debug.h"

namespace FDK {
	namespace General {

CSetString::CSetString()
{
	this->pIndexFirst = this->pIndexLast = NULL;
	this->pStringBufferFirst = this->pStringBufferLast = this->p�V����StringBuffer���쐬���ĕԂ�();
}
CSetString::CSetString( DWORD dw�����o�b�t�@�T�C�Ybytes )
{
	if( dw�����o�b�t�@�T�C�Ybytes == 0 )
		dw�����o�b�t�@�T�C�Ybytes = 4096;

	this->pIndexFirst = this->pIndexLast = NULL;
	this->pStringBufferFirst = this->pStringBufferLast = this->p�V����StringBuffer���쐬���ĕԂ�( dw�����o�b�t�@�T�C�Ybytes );
}
CSetString::~CSetString()
{
	Index* pi = this->pIndexFirst;
	while( pi != NULL )
	{
		Index* pn = pi->next;
		SAFE_FREE( pi );
		pi = pn;
	}
	this->pIndexFirst = this->pIndexLast = NULL;

	StringBuffer* psb = this->pStringBufferFirst;
	while( psb != NULL )
	{
		StringBuffer* psn = psb->next;
		SAFE_FREE( psb->p������o�b�t�@ );
		SAFE_FREE( psb );
		psb = psn;
	}
	this->pStringBufferFirst = this->pStringBufferLast = NULL;
}
CSetString::StringBuffer* CSetString::p�V����StringBuffer���쐬���ĕԂ�( DWORD dw�o�b�t�@�T�C�Y )
{
	_ASSERT( dw�o�b�t�@�T�C�Y >= 0 );

	StringBuffer* pSB;
	if( ( pSB = (StringBuffer*) malloc( sizeof( StringBuffer ) ) ) == NULL )
		return NULL;

	if( ( pSB->p������o�b�t�@ = (LPTSTR) malloc( dw�o�b�t�@�T�C�Y ) ) == NULL ) {
		SAFE_FREE( pSB );
		return NULL;
	}

	pSB->dw�o�b�t�@�T�C�Y = dw�o�b�t�@�T�C�Y;
	pSB->dw���ݎg�p�\�Ȏc��̃T�C�Y = dw�o�b�t�@�T�C�Y;
	pSB->prev = pSB->next = NULL;

	return pSB;
}
bool CSetString::t�ǉ�( LPTSTR p�ǉ����镶���� )
{
	// �߂�l: �ǉ��ł���� true

	StringBuffer* pSB;

	// pSB �� �ǉ��Ώہi���X�g�̈�ԍŌ�j�̃o�b�t�@
	{
		if( ( pSB = this->pStringBufferLast ) == NULL )
			return false;
	}
	//
	// pSB �ɕ����񂪊i�[�ł��邱�Ƃ�ۏႷ��
	{
		if( ! this->b��������o�b�t�@�Ɋi�[�\( p�ǉ����镶����, pSB ) )
		{
			if( ( pSB = this->p�V����StringBuffer���쐬���ĕԂ�() ) == NULL )
				return false;

			APPENDLIST( this->pStringBufferFirst, this->pStringBufferLast, pSB );

			if( ! this->b��������o�b�t�@�Ɋi�[�\( p�ǉ����镶����, pSB ) )
				return false;	// �ǉ����镶���񂪃o�b�t�@�P�̃T�C�Y���ł����Ƃ��͎��s�B
		}
	}
	//
	// ������� pSB �֒ǉ�����
	{
		LPTSTR p�i�[�ʒu = pSB->p������o�b�t�@ + ( pSB->dw�o�b�t�@�T�C�Y - pSB->dw���ݎg�p�\�Ȏc��̃T�C�Y );

		// �C���f�b�N�X�쐬
		Index* pi = (Index*) malloc( sizeof(Index) );
		pi->p������ = p�i�[�ʒu;
		pi->prev = pi->next = NULL;
		APPENDLIST( this->pIndexFirst, this->pIndexLast, pi );

		// StringBuffer �֏�������
		int nLen = lstrlen( p�ǉ����镶���� );
		for( int i = 0; i < nLen; i++ )
			*p�i�[�ʒu++ = *p�ǉ����镶����++;
		*p�i�[�ʒu = _T('\0');

		pSB->dw���ݎg�p�\�Ȏc��̃T�C�Y -= nLen + 1;
	}

	return true;
}
bool CSetString::b��������o�b�t�@�Ɋi�[�\( LPTSTR p�i�[������������, StringBuffer* p�i�[�������o�b�t�@ )
{
	_ASSERT( p�i�[������������ );
	_ASSERT( p�i�[�������o�b�t�@ );

	int n�K�v�ȃT�C�Y;
	if( ( n�K�v�ȃT�C�Y = lstrlen( p�i�[������������ ) + 1 ) <= 0 )
		return false;
	
	return ( p�i�[�������o�b�t�@->dw���ݎg�p�\�Ȏc��̃T�C�Y >= (DWORD) n�K�v�ȃT�C�Y ) ? true : false;
}
bool CSetString::t�폜( int n�폜���镶�����0����n�܂�C���f�b�N�X )
{
	// �߂�l: �ǉ��ł���� true

	int n;
	if( ( n = n�폜���镶�����0����n�܂�C���f�b�N�X ) < 0 )
		return false;		// �͈͊O

	// n�Ԗڂ̃C���f�b�N�X�Z����T��
	Index* pi;
	if( ( pi = this->pIndexFirst ) == NULL )
		return false;

	while( n > 0 )
	{
		pi = pi->next;
		n --;

		if( pi == NULL )
			return false;	// �͈͊O
	}

	// �C���f�b�N�X�Z�����폜����
	REMOVELIST( this->pIndexFirst, this->pIndexLast, pi );
	SAFE_FREE( pi );

	return true;
}
int  CSetString::n�v�f��()
{
	int n = 0;
	for( Index* pi = this->pIndexFirst; pi != NULL; pi = pi->next )
		n ++;

	return n;
}
LPCTSTR CSetString::p�w��Ԗڂ̕������Ԃ�( int n0����n�܂�C���f�b�N�X )
{
	if( n0����n�܂�C���f�b�N�X < 0 )
		return NULL;	// �͈͊O

	Index* pi;
	if( ( pi = this->pIndexFirst ) == NULL )
		return NULL;

	while( n0����n�܂�C���f�b�N�X > 0 )
	{
		pi = pi->next;
		n0����n�܂�C���f�b�N�X --;

		if( pi == NULL )
			return NULL;	// �͈͊O
	}

	return pi->p������;
}
	}//General
}//FDK
