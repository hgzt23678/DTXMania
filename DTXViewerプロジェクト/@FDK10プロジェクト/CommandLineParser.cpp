#include "stdafx.h"
#include "CommandLineParser.h"

namespace FDK {
	namespace General {

CommandLineParser::CommandLineParser()
{
	this->pFirstItem = this->pLastItem = NULL;
	this->n�A�C�e���� = 0;
}
void CommandLineParser::Init()
{
	this->Term();
}
void CommandLineParser::Init( LPCTSTR str��͂���R�}���h���C���� )
{
	LPCTSTR p = str��͂���R�}���h���C����;
	while( *p != _T('\0') )
	{
		CmdLineItem *pi;

		SKIPSP( p );

		// (1) �I�v�V�����̏ꍇ
		if( *p == _T('-') )
		{
			pi = new CmdLineItem();
			
			TCHAR buf[ _MAX_PATH ];

			int i = 0;
			while( i < _MAX_PATH - 1 && *p != _T(' ') && *p != _T('\t') && *p != _T('\0') )
				buf[i++] = *p++;
			buf[i] = _T('\0');
		
			pi->strItem = buf;
		}

		// (2) �_�u���N�H�[�g���ꂽ������̏ꍇ
		else if( *p == _T('\"') )
		{
			pi = new CmdLineItem();

			TCHAR buf[ _MAX_PATH ];
			p++;
			int i = 0;
			while( i < _MAX_PATH - 1 && *p != _T('\0') && *p != _T('\"') )
				buf[i++] = *p++;
			buf[i] = _T('\0');

			pi->strItem = buf;

			if( *p == _T('\"') )
				p++;
		}

		// (3) ���̑��̏ꍇ
		else
		{
			pi = new CmdLineItem();
			
			TCHAR buf[ _MAX_PATH ];
			int i = 0;
			while( i < _MAX_PATH - 1 && *p != _T('\0') && *p != _T(' ') && *p != _T('\t') && *p != _T('-') )
				buf[i++] = *p++;
			buf[i] = _T('\0');
			
			pi->strItem = buf;
		}

		// (1)�`(3) �̂�����̏ꍇ�ł����X�g�ɐڑ�
		pi->prev = pi->next = NULL;
		APPENDLIST( this->pFirstItem, this->pLastItem, pi );
		this->n�A�C�e����++;
	}
}
int  CommandLineParser::GetItemNum()
{
	return this->n�A�C�e����;
}
LPCTSTR CommandLineParser::Get( int index )
{
	if( index < 0 || index >= this->n�A�C�e���� )
		return NULL;

	CmdLineItem* pc = this->pFirstItem;
	while( pc != NULL && index > 0 ) {
		pc = pc->next;
		index --;
	}
	return pc->strItem.c_str();
}
void CommandLineParser::Term()
{
	CmdLineItem *pi = this->pFirstItem;
	while( pi != NULL )
	{
		CmdLineItem* next = pi->next;
		SAFE_DELETE( pi );
		pi = next;
	}
	this->pFirstItem = this->pLastItem = NULL;
	this->n�A�C�e���� = 0;
}

	}//General
}//FDK