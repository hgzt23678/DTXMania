#pragma once

#include "Activity.h"
#include "CD3DApplication.h"

namespace DTXViewer {
	namespace General {

class CD3DMyApplication : public FDK::AppBase::CD3DApplication
{
public:
	void	t�o�b�N�T�[�t�F�C�X���N���A����();

	virtual HRESULT	Create( HINSTANCE hInst, LPCTSTR strCmdLine );
	virtual HRESULT OneTimeSceneInit();					// �ŏ��P�񂾂��̏������i�E�B���h�E�ADD/D3D��������j
	virtual HRESULT InitDeviceObjects();				// �f�o�C�X�ˑ��I�u�W�F�N�g�̓�����ԏ�����
	virtual HRESULT RestoreDeviceObjects();				// �@�@�@�@�@�@�V�@�@�@�@�@�@�T�[�t�F�C�X�\�z
	virtual HRESULT InvalidateDeviceObjects();			// �@�@�@�@�@�@�V�@�@�@�@�@�@�T�[�t�F�C�X�j���i������Ԉێ��j
	virtual HRESULT DeleteDeviceObjects();				// �@�@�@�@�@�@�V�@�@�@�@�@�@������Ԕj��
	virtual HRESULT FinalCleanup();						// �Ō�P�񂾂��̏I�������iWM_CLOSE���ŌĂ΂��j
	virtual bool	Render();							// �V�[���̐i�s���`��it3D���̕`��()����Ăяo�����; true��Ԃ��ƃA�v���I���j
	virtual LRESULT WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	// �E�B���h�E���b�Z�[�W�v���V�[�W��
	CD3DMyApplication();

protected:
	Activity		actTopLevel;						// �g�b�v���x�� Activity
	bool			b���t�J�n���ɃE�B���h�E��O�ʂɎ����Ă���;

	void			Action();							// ���b�Z�[�W�̏���
	void			t�E�B���h�E��O�ʂɂ����Ă���();
};

	}//General
}//DTXViewer

using namespace DTXViewer::General;
