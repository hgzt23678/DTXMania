#include "stdafx.h"
#include "CArrayList.h"

namespace FDK {
	namespace General {

CArrayList::CArrayList( CArrayList::ARRAYLISTTYPE Type, UINT n�v�f�T�C�Y )
{
    this->arrayListType				= Type;
    this->p���I�z��					= NULL;
	this->n�v�f�T�C�Y				= (Type == ALT_REFERENCE) ? sizeof(LPVOID) : n�v�f�T�C�Y;
    this->n���݂̗v�f��				= 0;
	this->n���݂̓��I�z��̃T�C�Y	= 0;
}

CArrayList::~CArrayList()
{
    if( ! this->p���I�z�� )
        delete[] this->p���I�z��;
}

HRESULT CArrayList::t�v�f��ǉ�����( void* pEntry )
{
    if( this->n�v�f�T�C�Y == 0 )
        return E_FAIL;	// ����������ĂȂ�

	// �z����I�[�o�[�t���[����Ȃ�V�����̈���m�ۂ���B
	if( ! this->p���I�z�� || this->n���݂̗v�f�� + 1 > this->n���݂̓��I�z��̃T�C�Y )
	{
		HRESULT hr = this->t�V�������I�z����m�ۂ���();
		if( FAILED(hr) )
			return hr;
	}

	// ���I�z��ɐV�v�f��ǉ�����B
	switch( this->arrayListType )
	{
	case ALT_REFERENCE:
        *(((void**)this->p���I�z��) + this->n���݂̗v�f��) = pEntry;
		break;

	case ALT_VALUE:
        CopyMemory( (BYTE*)this->p���I�z�� + (this->n���݂̗v�f�� * this->n�v�f�T�C�Y), pEntry, this->n�v�f�T�C�Y );
		break;
	}
    this->n���݂̗v�f��++;

    return S_OK;
}

void CArrayList::t�v�f���폜����( UINT nIndex )
{
	if( this->n���݂̗v�f�� <= 0 )
		return;

    this->n���݂̗v�f�� --;

	BYTE* pData = (BYTE*)this->p���I�z�� + (nIndex * this->n�v�f�T�C�Y);
    MoveMemory( pData, pData + this->n�v�f�T�C�Y, ( this->n���݂̗v�f�� - nIndex) * this->n�v�f�T�C�Y );
}

void* CArrayList::p�v�f���擾����( UINT nIndex )
{
	switch( this->arrayListType )
	{
		case ALT_REFERENCE:
	        return *(((void**)this->p���I�z��) + nIndex);

		case ALT_VALUE:
	        return (BYTE*)this->p���I�z�� + (nIndex * this->n�v�f�T�C�Y);
	}

	return NULL;
}

bool CArrayList::b�v�f���z����ɑ��݂���( void* pEntryData )
{
    for( UINT i = 0; i < this->n���݂̗v�f��; i++ )
    {
		switch( this->arrayListType )
		{
		case ALT_REFERENCE:
			if( this->p�v�f���擾����(i) == pEntryData )
                return true;
			break;

		case ALT_VALUE:
			if( memcmp( this->p�v�f���擾����(i), pEntryData, this->n�v�f�T�C�Y ) == 0 )
                return true;
			break;
		}
    }
    return false;
}

HRESULT CArrayList::t�V�������I�z����m�ۂ���()
{
	// �V�����z��̃T�C�Y�͂���܂ł̂Q�{
	UINT �V�������I�z��̃T�C�Y = ( this->n���݂̓��I�z��̃T�C�Y == 0 ) ? 16 : this->n���݂̓��I�z��̃T�C�Y * 2;

	// �V�z��̃��������m��
	void* �V�������I�z�� = new BYTE[ �V�������I�z��̃T�C�Y * this->n�v�f�T�C�Y ];
	if( ! �V�������I�z�� )
		return E_OUTOFMEMORY;	// ���s

	// ���z�񂪂���Ȃ璆�g���R�s�[����
	if( this->p���I�z��  )
	{
		CopyMemory( �V�������I�z��, this->p���I�z��, this->n���݂̗v�f�� * this->n�v�f�T�C�Y );
		delete[] this->p���I�z��;
	}

	// �ڍs����
	this->p���I�z��               = �V�������I�z��;
	this->n���݂̓��I�z��̃T�C�Y = �V�������I�z��̃T�C�Y;

	return S_OK;
}

	}//General
}//FDK