#pragma once

namespace FDK {
	namespace General {

class CFile
{
public:
	void	Init();		// ������
	void	Term();		// �I������

	bool	Load( LPCTSTR filename );										// �t�@�C���̓ǂݍ���
	bool	Load( tstring &filename )	{return Load( filename.c_str() );}	// �t�@�C���̓ǂݍ���

	LPBYTE GetData()			{return this->pData;}				// �t�@�C���f�[�^�ւ̃|�C���^��Ԃ�
	DWORD  GetSize()			{return this->dwFileSizeLow;}		// �t�@�C���T�C�Y�̎擾
	FILETIME GetLastWriteTime()	{return this->ftLastWriteTime;}	// �ŏI�X�V������Ԃ�

	static void GetExt( LPCTSTR filename, LPTSTR ext );		// �g���q�𒲂ׂ� ext[_MAX_EXT] �Ɋi�[�i��F"txt")

public:
	CFile();
	virtual ~CFile();

protected:
	tstring		strFileName;				// �t�@�C����
	TCHAR		strExt[_MAX_EXT];			// �g���q
	BYTE*		pData;						// �f�[�^
	FILETIME	ftLastWriteTime;			// �ŏI�X�V����
	DWORD		dwFileSizeHigh;				// �t�@�C���T�C�Y�i���32�r�b�g�j
	DWORD		dwFileSizeLow;				// �t�@�C���T�C�Y�i����32�r�b�g�j
};

	}//General
}//FDK

using namespace FDK::General;
