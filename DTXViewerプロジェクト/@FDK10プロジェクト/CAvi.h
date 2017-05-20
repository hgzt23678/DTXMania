
// CAvi, CAviExpandBuf - AVI�Đ��N���X

#pragma once

#pragma comment(lib, "vfw32.lib")

namespace FDK {
	namespace Graphics {

class CAvi
{
public:
	void _������();
	void _���();

	HRESULT Open( LPCTSTR fname );									// �t�@�C�����I�[�v������B
	HRESULT Open( tstring &fname ) {return Open( fname.c_str() );}	// �t�@�C�����I�[�v������B
	void    Close();												// �t�@�C�����N���[�Y����B

	DWORD _��������t���[���ԍ���Ԃ�( double time );				// time ... ����(CTimer�\�L)

	// �t���[���T�C�Y�̎擾�iOpen��ɗL���j
	void _�t���[���T�C�Y���擾����( LONG* plWidth, LONG* plHeight );

	// �w�莞���̃t���[���� DC �֕`�悷��B
	// �@hWnd ... �E�B���h�E�n���h��
	// �@hDC .... �f�o�C�X�R���e�N�X�g
	// �@time ... ����
	// �@sx, sy ... �]�������W
	// �@sw, sh ... �]�����T�C�Y
	// �@dx, dy ... �]������W
	// �@dw, dh ... �]����T�C�Y
	// �߂�l: CAVI_����I�� ..... ����I��
	// �@�@�@�@CAVI_�Đ��I�� ..... �Đ��I���i���Đ����Ԃ�� time �̕����傫���j
	// �@�@�@�@CAVI_�G���[�I�� ... �G���[�I��
	int _�w�莞���̃t���[����DC�֕`�悷��( HWND hWnd, HDC hDC, double time, int sx=0, int sy=0, int sw=-1, int sh=-1, int dx=0, int dy=0, int dw=-1, int dh=-1 );
	static const int CAVI_����I�� = 0;
	static const int CAVI_�Đ��I�� = 1;
	static const int CAVI_�G���[�I�� = -1;

public:
	CAvi();
	virtual ~CAvi();

protected:

	static int n�Q�Ɛ�;		// ���݂̃C���X�^���X��

	PAVIFILE	 hFile;		// AVI�̃t�@�C���n���h��
	PAVISTREAM	 hAvi;		// AVI�̃X�g���[���n���h��
	PGETFRAME	 hFrame;	// GetFrame�I�u�W�F�N�g
	HDRAWDIB	 hDrawDib;

	DWORD dw���[�g;			// ����̃��[�g [�T���v��/�b]
	DWORD dw�X�P�[��;		// ����̃X�P�[�� [�T���v��/�t���[��]
	DWORD dw���t���[����;	// ����̃t���[����
	LONG  lFrameWidth;		// ����̉���
	LONG  lFrameHeight;		// ����̏c��

	bool bErrorAtDrawFrame;	// Draw���ɃG���[������������true�ɂȂ�B
};

	}//Graphics
}//FDK

using namespace FDK::Graphics;
