#include "stdafx.h"
#include "Activity.h"

namespace FDK {
	namespace AppBase {

Activity::Activity()
{
	this->bActivate = false;
	this->bFirstWork = true;
}

bool Activity::OneTimeSceneInit()
{
	// �����{�q�W���S���ɂ��āA�G���[���o����L�����Ă����B
	// �G���[���łĂĂ��A�Ăяo���͑S���ɂ��ĂЂƒʂ�s���B
	bool bRet = true;

	// (1)����
	bRet = MyOneTimeSceneInit();
	
	// (2)�q�W��
	Activity* pa;
	this->Activities.resetList();
	while( ( pa = this->Activities.getNext() ) != NULL )
	{
		if( ! pa->OneTimeSceneInit() )
			bRet = false;
	}
	return bRet;
}

bool Activity::Activate()
{
	// �����{�q�W���S���ɂ��āA�G���[���o����L�����Ă����B
	// �G���[���łĂĂ��A�Ăяo���͑S���ɂ��ĂЂƒʂ�s���B
	bool bRet = true;

	// (1)����
	bRet = MyActivate();
	
	// (2)�q�W��
	Activity* pa;
	this->Activities.resetList();
	while( ( pa = this->Activities.getNext() ) != NULL )
	{
		if( ! pa->Activate() )
			bRet = false;
	}
	return bRet;
}

bool Activity::UnActivate()
{
	// �����{�q�W���S���ɂ��āA�G���[���o����L�����Ă����B
	// �G���[���łĂĂ��A�Ăяo���͑S���ɂ��ĂЂƒʂ�s���B
	bool bRet = true;

	// (1)����
	bRet = MyUnActivate();
	
	// (2)�q�W��
	Activity* pa;
	this->Activities.resetList();
	while( ( pa = this->Activities.getNext() ) != NULL )
	{
		if( !pa->UnActivate() )
			bRet = false;
	}
	return bRet;
}

bool Activity::FinalCleanup()
{
	// �����{�q�W���S���ɂ��āA�G���[���o����L�����Ă����B
	// �G���[���łĂĂ��A�Ăяo���͑S���ɂ��ĂЂƒʂ�s���B
	bool bRet = true;

	// (1)����
	bRet = MyFinalCleanup();
	
	// (2)�q�W��
	Activity* pa;
	this->Activities.resetList();
	while( ( pa = this->Activities.getNext() ) != NULL )
	{
		if( !pa->FinalCleanup() )
			bRet = false;
	}
	return bRet;
}

bool Activity::InitDeviceObjects()
{
	// �����{�q�W���S���ɂ��āA�G���[���o����L�����Ă����B
	// �G���[���łĂĂ��A�Ăяo���͑S���ɂ��ĂЂƒʂ�s���B
	bool bRet = true;

	// (1)����
	bRet = MyInitDeviceObjects();

	// (2)�q�W��
	Activity* pa;
	this->Activities.resetList();
	while( ( pa = this->Activities.getNext() ) != NULL )
	{
		if( !pa->InitDeviceObjects() )
			bRet = false;
	}
	return bRet;
}

bool Activity::RestoreDeviceTextures()
{
	// �����{�q�W���S���ɂ��āA�G���[���o����L�����Ă����B
	// �G���[���łĂĂ��A�Ăяo���͑S���ɂ��ĂЂƒʂ�s���B
	bool bRet = true;

	// (1)����
	bRet = MyRestoreDeviceTextures();

	// (2)�q�W��
	Activity* pa;
	this->Activities.resetList();
	while( ( pa = this->Activities.getNext() ) != NULL )
	{
		if( !pa->RestoreDeviceTextures() )
			bRet = false;
	}
	return bRet;
}

bool Activity::RestoreDeviceSurfaces()
{
	// �����{�q�W���S���ɂ��āA�G���[���o����L�����Ă����B
	// �G���[���łĂĂ��A�Ăяo���͑S���ɂ��ĂЂƒʂ�s���B
	bool bRet = true;

	// (1)����
	bRet = MyRestoreDeviceSurfaces();

	// (2)�qActivity�W��
	Activity* pa;
	this->Activities.resetList();
	while( ( pa = this->Activities.getNext() ) != NULL )
	{
		if( !pa->RestoreDeviceSurfaces() )
			bRet = false;
	}
	return bRet;
}

bool Activity::InvalidateDeviceObjects()
{
	// �����{�q�W���S���ɂ��āA�G���[���o����L�����Ă����B
	// �G���[���łĂĂ��A�Ăяo���͑S���ɂ��ĂЂƒʂ�s���B
	bool bRet = true;

	// (1)����
	bRet = MyInvalidateDeviceObjects();

	// (2)�q�W��
	Activity* pa;
	this->Activities.resetList();
	while( ( pa = this->Activities.getNext() ) != NULL )
	{
		if( ! pa->InvalidateDeviceObjects() )
			bRet = false;
	}
	return bRet;
}

bool Activity::DeleteDeviceObjects()
{
	// �����{�q�W���S���ɂ��āA�G���[���o����L�����Ă����B
	// �G���[���łĂĂ��A�Ăяo���͑S���ɂ��ĂЂƒʂ�s���B
	bool bRet = true;

	// (1)����
	bRet = MyDeleteDeviceObjects();

	// (2)�q�W��
	Activity* pa;
	this->Activities.resetList();
	while( ( pa = this->Activities.getNext() ) != NULL )
	{
		if( ! pa->DeleteDeviceObjects() )
			bRet = false;
	}
	return bRet;
}

bool Activity::MyActivate()
{
	if( this->bActivate )
		return true;		// ���łɊ��������Ă�Ȃ牽�����Ȃ�

	this->bActivate = true;		// MyResoreDeviceObjects() �̌Ăяo���̑O�ɐݒ�

	bool bTex���� = MyRestoreDeviceTextures();
	bool bSur���� = MyRestoreDeviceSurfaces();
	if( ! bTex���� || ! bSur���� )		// �G���[���o�Ă��Ђƒʂ肺��ԉ�
		this->bActivate = false;

	return this->bActivate;
}

bool Activity::MyUnActivate()
{
	if( ! this->bActivate )
		return true;		// ���������ĂȂ��Ȃ牽�����Ȃ�

	MyInvalidateDeviceObjects();

	this->bActivate = false;	// MyInvalidateDeviceObjects() �̌Ăяo���̌�ɐݒ�
	return true;
}

bool Activity::MyRestoreDeviceTextures()
{
	if( ! this->bActivate )
		return true;		// ���������ĂȂ��Ȃ牽�����Ȃ�

	// �q�e�N�X�`���̍\�z
	bool bRet = true;
	CTexture* pt;
	this->Textures.resetList();
	while( ( pt = this->Textures.getNext() ) != NULL )
	{
		if( FAILED( pt->RestoreDeviceObjects() ) )
			bRet = false;
	}
	return bRet;
}

bool Activity::MyRestoreDeviceSurfaces()
{
	if( ! this->bActivate )
		return true;		// ���������ĂȂ��Ȃ牽�����Ȃ�

	// �q�T�[�t�F�C�X�̍\�z
	bool bRet = true;
	CSurface* ps;
	this->Surfaces.resetList();
	while( ( ps = this->Surfaces.getNext() ) != NULL )
	{
		if( FAILED( ps->RestoreDeviceObjects() ) )
			bRet = false;
	}
	return bRet;
}

bool Activity::MyInvalidateDeviceObjects()
{
	if( ! this->bActivate )
		return true;		// ���������ĂȂ��Ȃ牽�����Ȃ�

	bool bRet = true;

	// �q�e�N�X�`���̖�����
	CTexture* pt;
	this->Textures.resetList();
	while( ( pt = this->Textures.getNext() ) != NULL )
	{
		if( FAILED( pt->InvalidateDeviceObjects() ) )
			bRet = false;
	}

	// �q�T�[�t�F�C�X�̖�����
	CSurface* ps;
	this->Surfaces.resetList();
	while( ( ps = this->Surfaces.getNext() ) != NULL )
	{
		if( FAILED( ps->InvalidateDeviceObjects() ) )
			bRet = false;
	}

	return bRet;
}

bool Activity::MyDeleteDeviceObjects()
{
	bool bRet = true;

	// �q�e�N�X�`���̍폜
	CTexture* pt;
	this->Textures.resetList();
	while( ( pt = this->Textures.getNext() ) != NULL )
	{
		if( FAILED( pt->DeleteDeviceObjects() ) )
			bRet = false;
	}

	// �q�T�[�t�F�C�X�̍폜
	CSurface* ps;
	this->Surfaces.resetList();
	while( ( ps = this->Surfaces.getNext() ) != NULL )
	{
		if( FAILED( ps->DeleteDeviceObjects() ) )
			bRet = false;
	}

	return bRet;
}

	}//AppBase
}//FDK