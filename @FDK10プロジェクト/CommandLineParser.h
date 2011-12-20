#pragma once

namespace FDK {
	namespace General {

class CommandLineParser
{
public:
	void Init();
	void Init( LPCTSTR str��͂���R�}���h���C���� );
	int GetItemNum();			// �A�C�e������Ԃ�
	LPCTSTR Get( int index );	// index �Ԗڂ̃A�C�e����Ԃ��i index = 0�` GetItemNum()-1 �j�B�Ԃ����͓̂����� tstring �� c_str() �Ȃ̂ŁA�����Ԃ͈����Ȃ����Ƃɒ��ӁB
	void Term();

	CommandLineParser();

protected:

	// (1) '-' �Ŏn�܂鍀�ڂ̓I�v�V�����Ƃ݂Ȃ��B���̏ꍇ�AstrItem �ɂ͐擪�� '-' �������܂܂��B�i��F"-N1"�j
	// (2) ���ڊԂ͋󔒕���( SPC, TAB )�ŋ�؂���B�S�pSPC�͋󔒂Ƃ݂Ȃ��Ȃ��B
	// (3) �������A�_�u���N�H�[�g����Ă���ꍇ�͂�����P�̍��ڂƂ݂Ȃ��B

	struct CmdLineItem {
		tstring		strItem;
		CmdLineItem	*prev, *next;
	} *pFirstItem, *pLastItem;

	int n�A�C�e����;
};

	}//General
}//FDK