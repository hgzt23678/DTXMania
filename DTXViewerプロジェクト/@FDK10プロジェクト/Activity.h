
//
// Name: Activity �N���X
// Desc: ������ԂƓ����摜��ێ����A�i�s�ƕ`����s�����̂̊�{�N���X�B
//
// �E������ԂƂ́A�i�s�ɂ�������ԕϐ��A�����ē����摜�Ȃǂ������B�i�摜�͏�Ԃ̈ꕔ�j
// �E�����摜�Ƃ́A�e�N�X�`����T�[�t�F�C�X�������B
//
// �EActivity �̔h���N���X�́AMy... ���\�b�h�Ȃ�т� MainWork() ���\�b�h���I�[�o�[���C�h����B
// �E�P�� Activity �́A�����̎qActivity�A�q�e�N�X�`����ێ����邱�Ƃ��ł���B
//     �EAddChild() / RemoveChild() �Œǉ��폜
//   �@�EActivity�A�e�N�X�`���� AddChild() �́AMyOneTimeSceneInit() ���ōs�����ƁB
//   �@�EMy...���\�b�h�ł́A�qActivity �̑���������s��Ȃ����ƁB
//
// �E�A�v���P�[�V�����iCD3DApplication �܂��͂��̔h���j���ۗL���� Activity ���A
//  �u�g�b�v���x�� Activity�v�ƌĂԁB
//
// �E�A�v���P�[�V�����́A�ȉ��̃^�C�~���O�Ńg�b�v���x��Activity ���������邱�ƁB
//     �EOneTimeSceneInit() ��			���@���ׂẴg�b�v���x��Activity �� OneTimeSceneInit() ���Ăяo���B
//     �EInitDeviceObjects() ��			���@�@�@�@�@�@�@�@�@�V�@�@�@�@�@�@�@InitDeviceObjects() ���Ăяo���B
//     �ERestoreDeviceObjects() ��		���@�@�@�@�@�@�@�@�@�V�@�@�@�@�@�@�@RestoreDeviceTextures(), RestoreDeviceSurfaces() �̏��ɌĂяo���B
//     �EInvalidateDeviceObjects() ��	���@�@�@�@�@�@�@�@�@�V�@�@�@�@�@�@�@InvalidateDeviceObjects() ���Ăяo���B
//     �EDeleteDeviceObjects() ��		���@�@�@�@�@�@�@�@�@�V�@�@�@�@�@�@�@DeleteDeviceObjects() ���Ăяo���B
//     �EFinalCleanup() ��				���@�@�@�@�@�@�@�@�@�V�@�@�@�@�@�@�@FinalCleanup() ���Ăяo���B
//     �EActivity ��L������������		���@�@�@�@�@�@�@�@�@�V�@�@�@�@�@�@�@Activate() ���Ăяo���B
//     �EActivity �𖳌�����������		���@�@�@�@�@�@�@�@�@�V�@�@�@�@�@�@�@UnActivate() ���Ăяo���B
//

#pragma once

#include "CTexture.h"
#include "CSurface.h"
#include "CSet.h"

namespace FDK {
	namespace AppBase {

class Activity
{
public:
	void	AddChild( Activity* c ) 	{Activities.insert(c);}		// �qActivity�̒ǉ�
	void	AddChild( CTexture* c ) 	{Textures.insert(c);}		// �qTexture�̒ǉ�
	void	AddChild( CSurface* c ) 	{Surfaces.insert(c);}		// �qSurface�̒ǉ�
	void	RemoveChild( Activity* c )	{Activities.erase(c);}		// �qActivity�̍폜
	void	RemoveChild( CTexture* c )	{Textures.erase(c);}		// �qTexture�̍폜
	void	RemoveChild( CSurface* c )	{Surfaces.erase(c);}		// �qSurface�̍폜

	virtual bool	OneTimeSceneInit();								// MyObeTimeSceneInit()�{�q�W���̂��ׂĂ� OneTimeSceneInit() ���Ăяo���B
	virtual bool	Activate();										// MyActivate()�{�q�W���̂��ׂĂ� Activate() ���Ăяo���B
	virtual bool	UnActivate();									// MyUnActivate()�{�q�W���̂��ׂĂ� UnActivate() ���Ăяo���B
	virtual bool	FinalCleanup();									// MyFinalCleanup()�{�q�W���̂��ׂĂ� FinalCleanup() ���Ăяo���B
	virtual bool	InitDeviceObjects();							// MyInitDeviceObjects()�{�q�W���̂��ׂĂ� InitDeviceObjects() ���Ăяo���B
	virtual bool	RestoreDeviceTextures();						// MyRestoreDeviceTextures()�{�q�W���̂��ׂĂ� RestoreDeviceTextures() ���Ăяo���B
	virtual bool	RestoreDeviceSurfaces();						// MyRestoreDeviceSurfaces()�{�q�W���̂��ׂĂ� RestoreDeviceSurfaces() ���Ăяo���B
	virtual bool	InvalidateDeviceObjects();						// MyInvalidateDeviceObjects()�{�q�W���̂��ׂĂ� InvalidateDeviceObjects() ���Ăяo���B
	virtual bool	DeleteDeviceObjects();							// MyDeleteDeviceObjects()�{�q�W���̂��ׂĂ� DeleteDeviceObjects() ���Ăяo���B

	virtual int		MainWork()				{return 0;}				// �i�s�ƕ`��
	bool			IsActive()				{return bActivate;}		// ���݊�������ԂȂ�true��Ԃ��B

protected:
	virtual bool	MyOneTimeSceneInit()	{return true;}			// ������
	virtual bool	MyActivate();									// �������G �@MyActivate() �� MyRestoreDevice�`() �����s����B
	virtual bool	MyUnActivate();									// �񊈐����G MyUnActivate() �� MyInvalidateDeviceObjects() �����s����B
	virtual bool	MyFinalCleanup()		{return true;}			// �I������
	virtual bool	MyInitDeviceObjects()	{return true;}			// �摜�̓o�^
	virtual bool	MyRestoreDeviceTextures();						// �q�e�N�X�`���摜�̍\�z
	virtual bool	MyRestoreDeviceSurfaces();						// �q�T�[�t�F�C�X�摜�̍\�z
	virtual bool	MyInvalidateDeviceObjects();					// �摜�̖�����
	virtual bool	MyDeleteDeviceObjects();						// �摜�̍폜

public:
	Activity();

protected:
	bool			bActivate;			// ��������ԂȂ�true
	bool			bFirstWork;

	CSet<Activity>	Activities;			// Activitiy �q�W��
	CSet<CTexture>	Textures;				// CTexture �q�W��
	CSet<CSurface>	Surfaces;				// CSurface �q�W��
};
	}//AppBase
}//FDK

using namespace FDK::AppBase;
