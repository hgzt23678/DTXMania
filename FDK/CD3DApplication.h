#pragma once

#include "CD3DDisplay.h"
#include "CTimer.h"

namespace FDK {
	namespace AppBase {

class CD3DApplication
{
public:
	CD3DApplication();

	// �N��
	virtual HRESULT Create( HINSTANCE hInst, LPCTSTR strCmdLine );		// ������
	virtual LRESULT WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	// �E�B���h�E���b�Z�[�W�n���h���B
	INT		Run();														// �X���b�h���[�v

	// Create() ���Ăяo���O�Ɉȉ��̃����o�ϐ���ݒ�\�B
	static HWND hWnd;								// �E�B���h�E�n���h���iNULL�Ȃ�f�t�H���g�E�B���h�E���쐬�����j
	static int	nBpp;								// �r�b�g�[�x�i16 or 32�j
	bool		b�S��ʃ��[�h;						// �S��ʃ��[�h�Ȃ� true
	int			nWidth, nHeight;					// �E�B���h�E�T�C�Y�A�[�x
	bool		b�����A������;						// �t���b�v�𐂒��A������������Ȃ�true
	DWORD		dw�`��Ԋu;							// Present ���̃X���[�v��[ms]�i�f�t�H���g 3 ms�G�C�ӂ̃^�C�~���O�ŕύX�\�j
    LPCTSTR		strWindowTitle;						// �A�v���E�B���h�E�̃^�C�g����
    LPCTSTR		strWindowClass;						// �A�v���E�B���h�E�̃N���X��

	// ���̑�
	static LPDIRECTDRAW7			pDD;				// DirectDraw7
	static LPDIRECTDRAWSURFACE7		pddsFrontBuffer;	// �t�����g�o�b�t�@
	static LPDIRECTDRAWSURFACE7		pddsBackBuffer;		// �o�b�N�o�b�t�@
	static LPDIRECT3D7				pD3D;				// Direct3D7
	static LPDIRECT3DDEVICE7		pD3DDevice;			// Direct3DDevice7
	static CD3DDisplay				D3DDisplay;			// �A�_�v�^,�f�o�C�X,���[�h�Ǘ�
	HINSTANCE						hInstance;			// �C���X�^���X
	bool							bReady;				// �`�揀������������ true
	bool							bActive;			// �E�B���h�E���A�N�e�B�u�Ȃ� true�A�ŏ���or�S���B��Ă���ꍇ�� false
	CTimer							timer;				// Flip|Blt�p�^�C�}
	int								nFPS;				// Frame / sec,

	static void	GetProjectionParam( float* d, float* t, float* a ) { *d=100.0f; *t=24.0f/180.0f*(float)PI; *a=1.0f; }

protected:
	HRESULT t3D���̍\�z();					// �R�c���̍\�z�iCreate(), ForceWindowed()����Ăяo�����j
	HRESULT t3D���̕`��();					// �R�c�̕`�揈��
	void	t3D���̔j��();					// �R�c���̔j��
	HRESULT t3D���̍č\�z( bool b�V�S��ʃ��[�h );	// �X���b�h�쓮�̏ꍇ�́A�����ɗ���܂łɃN���e�B�J���Z�N�V�����ŃX���b�h������~�߂Ă������ƁB

	virtual HRESULT OneTimeSceneInit()				{ return S_OK; }	// �ŏ��P�񂾂��̏������i�E�B���h�E�ADD/D3D��������j
	virtual HRESULT InitDeviceObjects()				{ return S_OK; }	// �f�o�C�X�ˑ��I�u�W�F�N�g�̓�����ԏ�����
	virtual HRESULT RestoreDeviceObjects()			{ return S_OK; }	// �@�@�@�@�@�@�V�@�@�@�@�@�@�T�[�t�F�C�X�\�z
	virtual HRESULT InvalidateDeviceObjects()		{ return S_OK; }	// �@�@�@�@�@�@�V�@�@�@�@�@�@�T�[�t�F�C�X�j���i������Ԉێ��j
	virtual HRESULT DeleteDeviceObjects()			{ return S_OK; }	// �@�@�@�@�@�@�V�@�@�@�@�@�@������Ԕj��
	virtual HRESULT FinalCleanup()					{ return S_OK; }	// �Ō�P�񂾂��̏I�������iWM_CLOSE���ŌĂ΂��j
	virtual bool	Render()						{ return false; }	// �V�[���̐i�s���`��it3D���̕`��()����Ăяo�����; true��Ԃ��ƃA�v���I���j
	virtual	HRESULT	tFlip�܂���Blt();

	virtual void tWaitAMoment�̕`��();		// �f�o�C�X�ؑ֎��� Wait a moment �摜�̕`��i�K�v����Ύ�������j

	void	t�S��ʁE�E�B���h�E���[�h��؂�ւ���();							// �E�B���h�E�X���b�h�łȂ���ΐ؂肩�����Ȃ��̂ŁA�Q�[���X���b�h����͌Ă΂Ȃ����ƁB
	void	t���݂̃E�B���h�E�̃E�B���h�E�̈�ƃN���C�A���g�̈���擾����();	// �ǂ�����X�N���[�����W�Ŏ擾����B
	HRESULT	t�����������g��Ȃ��E�F�C�g��Blt( LPRECT prc�N���C�A���g�̈� );
	void	tFPS���Z�o����();

	DWORD	dw�E�B���h�E�X�^�C��;		// ���[�h�ؑ֗p�ɃE�B���h�E�X�^�C����ۑ�
	DWORD	dw�S��ʃX�^�C��;			// ���[�h�ؑ֗p�ɑS��ʃX�^�C����ۑ�
	RECT	rc�E�B���h�E�̈�;			// ���[�h�ؑ֗p�ɃE�B���h�E�̈��ۑ�
	RECT	rc�N���C�A���g�̈�;			// ���[�h�ؑ֗p�ɃN���C�A���g�̈��ۑ�

	bool	bScreenSaverEnable;			// �N�����A�X�N���[���Z�[�o���L���������Ȃ�true
	EXECUTION_STATE	exeState;			// ���j�^�d���ݒ�ۑ�
};

	}//AppBase
}//FDK

using namespace FDK::AppBase;
