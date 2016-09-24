
// CArrayList - ���I�z��
// �@�l�^�ƎQ�ƌ^�̂Q��ނ�����i�������Ɏw��j�B

#pragma once

namespace FDK {
	namespace General {

class CArrayList
{
public:
	static const enum ARRAYLISTTYPE {
		ALT_VALUE,		// �l�^
		ALT_REFERENCE	// �Q�ƌ^
	};

public:
	// �R���X�g���N�^
	// �@Type ... �l�^ or �Q�ƌ^�B
	// �@n�v�f�T�C�Y ... �P�v�f�̃T�C�Y�B�Q�ƌ^�ł͈Ӗ��Ȃ��B
	CArrayList( ARRAYLISTTYPE Type, UINT n�v�f�T�C�Y = 0 );

	// �f�X�g���N�^
	// �@���I�z����������B
	~CArrayList();

	// �v�f��ǉ�����B
	// �@����Ȃ� S_OK ��Ԃ��B
	HRESULT t�v�f��ǉ�����( void* pEntry );

	// �v�f���폜����B
	// �@nIndex ... �v�f�̃C���f�b�N�X(0..)
    void t�v�f���폜����( UINT nIndex );

	// �v�f���擾����B
	// �@nIndex ... �v�f�̃C���f�b�N�X(0..)
    void* p�v�f���擾����( UINT nIndex );

	// �w�肵���v�f���z����ɑ��݂���Ȃ� true ��Ԃ��B
    bool b�v�f���z����ɑ��݂���( void* pEntryData );

	UINT Count()	{ return n���݂̗v�f��; }		// ���݂̗v�f����Ԃ��B
    void Clear()	{ n���݂̗v�f�� = 0; }		// ���I�z����N���A����B

protected:
	ARRAYLISTTYPE	arrayListType;			// �l�^���Q�ƌ^���H
    void*			p���I�z��;
	UINT			n�v�f�T�C�Y;				// �P�v�f�̑傫���B�Q�ƌ^�ł� sizeof(LPVOID) �ƂȂ�B
    UINT			n���݂̗v�f��;
    UINT			n���݂̓��I�z��̃T�C�Y;

private:
	HRESULT			t�V�������I�z����m�ۂ���();
};

	}//General
}//FDK

using namespace FDK::General;
