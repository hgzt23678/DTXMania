#pragma once

namespace DTXMania {
	namespace General {

class DirectXVersion
{
public:
	DWORD	dwMajor;		// ���W���[�ԍ�
	DWORD	dwMinor;		// �}�C�i�[�ԍ�
	DWORD	dwAppx;			// Appendix ('a'�`'z' �� 0�`25)

public:
	void tDirectX�̃o�[�W���������擾����();

public:
	DirectXVersion();
};

	}//General
}//DTXMania

using namespace DTXMania::General;
