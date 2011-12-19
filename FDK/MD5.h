
/*
 * MD5 - MD5������̍쐬
 *
 *�y�T�v�z
 *  �C�ӂ̃��b�Z�[�W�i�o�C�i���j��MD5������i16�i��32�����j���쐬����B
 *  �������A�{�R�[�h�ł́i��𔲂��Ă���̂Łj���b�Z�[�W�͍ő� 2^32 �o�C�g
 *  �܂ł����Ή����Ă��Ȃ��B
 *
 *�y�g�����z
 *  (2) MD5::Init() ���Ăяo���B
 *  (3) ���񂩂ɂ킯�� MD5::Update() ���Ăяo���A���b�Z�[�W�����ׂď�������B
 *  (4) ���ׂď������I������� MD5::Final() ���Ăяo���A�ŏI�I�� MD5 ��������󂯎��B
 */

#pragma once

namespace FDK {
	namespace General {

typedef USHORT	UINT2;		// UINT2: �����Ȃ�16bit����
typedef	ULONG	UINT4;		// UINT4: �����Ȃ�32bit����

class MD5
{
public:
	void	Init();										// ������
	void 	Update( UCHAR* input, ULONG input_len );	// ���b�Z�[�W�̏���

	// �Ō�̃��b�Z�[�W�ϊ��B�쐬���ꂽ MD5 �R�[�h�i128bit�j��16�i���\�L��
	// �ϊ�����������i32�����{'\0'�j�� outbuf �Ɋi�[�����B
	// (��outbuf[] �͍Œ�33�����ȏ���邱�ƁB�j
	void	Final( TCHAR outbuf[] );

	static BYTE		CalcSum( TCHAR* hash );				// �w�肳�ꂽ�n�b�V���̃T�����v�Z���ĕԂ��B
	static void		PutBool( BYTE** p, bool b );		// bool �o��
	static void		PutInt( BYTE** p, int n );			// int �o��
	static void		PutInt64( BYTE** p, __int64 n );	// __int64 �o��
	static void		PutFloat( BYTE** p, float f );		// float �o��
	static void		PutString( BYTE** p, TCHAR* str );	// ������o��

public:
	MD5() {}
	virtual ~MD5() {}

protected:
	void	Transform();		// ���̓o�b�t�@��ϊ��������A���̌��ʂ� m_state[4] �Ɋi�[����B

protected:
	ULONG	m_input_len;		// ���͂̑��T�C�Y
	UINT4	m_state[4];			// �o��(A,B,C,D)
	UCHAR	m_buffer[64+1];		// ���̓o�b�t�@(16���[�h)
	ULONG	m_buflen;			// ���̓o�b�t�@�̗L���o�C�g��

};

	}//General
}//FDK

using namespace FDK::General;
