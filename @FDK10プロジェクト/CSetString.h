//
// CSetString
//   �������p�̏W���B�����̕�����ɃC���f�b�N�X�ԍ��ŃA�N�Z�X�ł���B
//   ������폜��̃K�x�[�W�R���N�V�����͍s��Ȃ��B
//   �P�o�b�t�@�̃T�C�Y(4096)�𒴂��镶����������Ȃ��B
//
#pragma once

namespace FDK {
	namespace General {

class CSetString
{
public:
	CSetString();
	CSetString( DWORD dw�����o�b�t�@�T�C�Ybytes );
	virtual ~CSetString();

	int  n�v�f��();
	bool t�ǉ�( LPTSTR p�ǉ����镶���� );							// �ǉ��ɐ���������true��Ԃ�
	bool t�폜( int n�폜���镶�����0����n�܂�C���f�b�N�X );		// �폜�ɐ���������true��Ԃ�
	LPCTSTR p�w��Ԗڂ̕������Ԃ�( int n0����n�܂�C���f�b�N�X );

protected:
	struct Index {
		LPTSTR	p������;
		Index	*prev, *next;
	} *pIndexFirst, *pIndexLast;

	struct StringBuffer {
		LPTSTR	p������o�b�t�@;
		DWORD	dw�o�b�t�@�T�C�Y;
		DWORD	dw���ݎg�p�\�Ȏc��̃T�C�Y;
		StringBuffer	*prev, *next;
	} *pStringBufferFirst, *pStringBufferLast;

	StringBuffer* p�V����StringBuffer���쐬���ĕԂ�( DWORD dw�o�b�t�@�T�C�Y = 4096 );
	bool b��������o�b�t�@�Ɋi�[�\( LPTSTR p�i�[������������, StringBuffer* p�i�[�������o�b�t�@ );
};
	}//General
}//FDK

using namespace FDK::General;
