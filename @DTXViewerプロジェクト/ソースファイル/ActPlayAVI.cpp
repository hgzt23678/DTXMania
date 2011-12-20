#include "stdafx.h"
#include "ActPlayAVI.h"
#include "CSurface.h"
#include "DTX.h"
#include "CStage.h"
#include "DTXViewer.h"

namespace DTXViewer {
	namespace Stage {
		namespace Play {

ActPlayAVI::ActPlayAVI()
{
	this->bFirstWork = true;
	this->bActivate = false;
}

bool ActPlayAVI::MyOneTimeSceneInit()
{
	// �����ɃR�[�h���L�q����BAddChild() �ȂǁB
	AddChild( &this->sfAVI );

	return Activity::MyOneTimeSceneInit();
}
bool ActPlayAVI::MyFinalCleanup()
{
	// �����ɃR�[�h���L�q����B

	return Activity::MyFinalCleanup();
}
bool ActPlayAVI::MyInitDeviceObjects()
{
	// �����ɃR�[�h���L�q����BCTexture/CSurface::InitDeviceObjects() �ȂǁB
	this->sfAVI.InitDeviceObjects( _T("AVI"), BGA_W, BGA_H );

	return Activity::MyInitDeviceObjects();
}
bool ActPlayAVI::MyDeleteDeviceObjects()
{
	// �����ɃR�[�h���L�q����B

	// ���� Activity::MyDeleteDeviceObjects() �ł́A�q�e�N�X�`���E�q�T�[�t�F�C�X�� Delete (CTexture/CSurface::Delete) ���s����B
	return Activity::MyDeleteDeviceObjects();
}
bool ActPlayAVI::MyActivate()
{
	this->bFirstWork = true;
	// �����ɃR�[�h���L�q����B
	for( int i = 0; i < MAX_AVI_LAYER; i++ )
	{
		this->structLayer[i].pAVI						= NULL;
		this->structLayer[i].db�ړ��J�n����ms			=INIT_TIME;
		this->structLayer[i].dw�O��\�������t���[���ԍ�	= 0;
	}

	// ���� Activity::MyActivate() �ł́Athis->bActivity �̃Z�b�g�Ȃ�тɎq�T�[�t�F�C�X�E�q�e�N�X�`���� Restore (MyRestoreDeviceTexture/Surface) ���s����B
	// �Ȃ��A�q Activity �̊������͍s���Ȃ��B�i���̃��\�b�h�𔲂��Ă���s����B�j
	return Activity::MyActivate();
}
bool ActPlayAVI::MyUnActivate()
{
	// �����ɃR�[�h���L�q����B

	// ���� Activity::MyUnActivate() �ł́Athis->bActivity �̃��Z�b�g�Ȃ�тɎq�T�[�t�F�C�X�E�q�e�N�X�`���� Invalidate (MyInvalidateDeviceObjects) ���s����B
	// �Ȃ��A�qActivity �̔񊈐����͍s���Ȃ��B�i���̃��\�b�h�𔲂��Ă���s����B�j
	return Activity::MyUnActivate();
}
bool ActPlayAVI::MyRestoreDeviceTextures()
{
	// �����ɃR�[�h���L�q����B

	// ���� Activity::MyRestoreDeviceTextures() �ł́A�q�e�N�X�`���� Restore (CTexture::Restore) ���s����B
	return Activity::MyRestoreDeviceTextures();
}
bool ActPlayAVI::MyRestoreDeviceSurfaces()
{
	// �����ɃR�[�h���L�q����B

	// ���� Activity::MyRestoreDeviceSurfaces() �ł́A�q�T�[�t�F�C�X�� Restore (CSurface::Restore) ���s����B
	return Activity::MyRestoreDeviceSurfaces();
}
bool ActPlayAVI::MyInvalidateDeviceObjects()
{
	// �����ɃR�[�h���L�q����B

	// ���� Activity::MyInvalidateDeviceObjects() �ł́A�q�e�N�X�`���E�q�T�[�t�F�C�X�� Invalidate (CTexture/CSurface::Invalidate) ���s����B
	return Activity::MyRestoreDeviceSurfaces();
}
//
void ActPlayAVI::Start( int n�`�����l���ԍ�, DTX::AVI* pAVI, int n�J�n�T�C�YW, int n�J�n�T�C�YH, int n�I���T�C�YW, int n�I���T�C�YH, int n�摜���J�n�ʒuX, int n�摜���J�n�ʒuY, int n�摜���I���ʒuX, int n�摜���I���ʒuY, int n�\�����J�n�ʒuX, int n�\�����J�n�ʒuY, int n�\�����I���ʒuX, int n�\�����I���ʒuY, double db���ړ�����ms, double db�ړ��J�n����ms )
{
	// �`�����l���`�F�b�N
	if( n�`�����l���ԍ� != 0x54 )
		return;

	// �Z���ɒl��ݒ�
	int i = 0;
	this->structLayer[i].pAVI	= pAVI;
	this->structLayer[i].n�J�n�T�C�YW	= n�J�n�T�C�YW;
	this->structLayer[i].n�J�n�T�C�YH	= n�J�n�T�C�YH;
	this->structLayer[i].n�摜���J�n�ʒuX	= n�摜���J�n�ʒuX;
	this->structLayer[i].n�摜���J�n�ʒuY	= n�摜���J�n�ʒuY;
	this->structLayer[i].n�\�����J�n�ʒuX	= n�\�����J�n�ʒuX;
	this->structLayer[i].n�\�����J�n�ʒuY	= n�\�����J�n�ʒuY;
	this->structLayer[i].db���ړ�����ms		= db���ړ�����ms;
	this->structLayer[i].db�ړ��J�n����ms	= ( db�ړ��J�n����ms != INIT_TIME ) ? db�ړ��J�n����ms : g_Timer.Get();
	this->structLayer[i].dw�O��\�������t���[���ԍ� = 0;
}
void ActPlayAVI::SkipStart( double db�ړ��J�n����ms )
{
	for( DTX::Chip* cell = g_DTX.pChip; cell != NULL; cell = cell->next )
	{
		// �J�n���������Ȃ�A���̃`�b�v�Œ����ł��؂�B
		if( cell->dbTime > db�ړ��J�n����ms ) 
			break;

		// AVI, AVIPAN �Ȃ�Đ��w��
		switch( cell->AVItype )
		{

		case AVITYPE_AVI:
			if( cell->pAVI )
				this->Start(
					cell->nChannel, cell->pAVI,
					BGA_W, BGA_H, 0, 0,			// sw, sh, ew, eh		; �̈�   �����T�C�Y�A�I���T�C�Y
					0, 0, 0, 0,					// ssx, ssy, sex, sey	; �摜�� �����ʒu�A�ŏI�ʒu
					0, 0, 0, 0,					// dsx, dsy, dex, dey	; �\���� �����ʒu�A�ŏI�ʒu
					0, cell->dbTime );			// len, start
			break;

		case AVITYPE_AVIPAN:
			if( cell->pAVIPan )
				this->Start(
					cell->nChannel, cell->pAVI,
					cell->pAVIPan->sw, cell->pAVIPan->sh, cell->pAVIPan->ew, cell->pAVIPan->eh,		// �̈�   �����T�C�Y�A�I���T�C�Y
					cell->pAVIPan->ssx, cell->pAVIPan->ssy, cell->pAVIPan->sex, cell->pAVIPan->sey,	// �摜�� �����ʒu�A�ŏI�ʒu
					cell->pAVIPan->dsx, cell->pAVIPan->dsy, cell->pAVIPan->dex, cell->pAVIPan->dey,	// �\���� �����ʒu�A�ŏI�ʒu
					cell->dbLong, cell->dbTime );													// len, start
			break;
		}
	}
}
void ActPlayAVI::Stop()
{
	for( int i = 0; i < MAX_AVI_LAYER; i++ ) {
		if( this->structLayer[i].pAVI && this->structLayer[i].pAVI->bUse )
			this->structLayer[i].db�ړ��J�n����ms = INIT_TIME;
	}
}
void ActPlayAVI::Cont( double db�ĊJ���� )
{
	for( int i = 0; i < MAX_AVI_LAYER; i++ ) {
		if( this->structLayer[i].pAVI && this->structLayer[i].pAVI->bUse )
			this->structLayer[i].db�ړ��J�n����ms = db�ĊJ����;
	}
}
int  ActPlayAVI::MainWork( int x, int y )
{
	// AVI �w��Ȃ��Ȃ牽�����Ȃ�
	if( ! g_DTX.bUseAVI )
		return 0;

	// �i�s���`��
	for( int i = 0; i < MAX_AVI_LAYER; i++ )
	{
		// ���g�p�Ȃ�X�L�b�v
		if( this->structLayer[i].db�ړ��J�n����ms == INIT_TIME )
			continue;
		if( ! this->structLayer[i].pAVI || ! this->structLayer[i].pAVI->bUse )
			continue;

		// �p�����[�^���o�E�Z�o
		DTX::AVI*	 pAVI		= this->structLayer[i].pAVI;
		SIZE szAVI�T�C�Y;		pAVI->avi._�t���[���T�C�Y���擾����( &(szAVI�T�C�Y.cx), &(szAVI�T�C�Y.cy) );
		SIZE szBGA�T�C�Y		= { BGA_W, BGA_H };
		SIZE sz�J�n�T�C�Y		= { this->structLayer[i].n�J�n�T�C�YW, this->structLayer[i].n�J�n�T�C�YH };
		SIZE sz�I���T�C�Y		= { this->structLayer[i].n�I���T�C�YW, this->structLayer[i].n�I���T�C�YH };
		POINT pt�摜���J�n�ʒu	= { this->structLayer[i].n�摜���J�n�ʒuX, this->structLayer[i].n�摜���I���ʒuY };
		POINT pt�摜���I���ʒu	= { this->structLayer[i].n�摜���I���ʒuX, this->structLayer[i].n�摜���I���ʒuY };
		POINT pt�\�����J�n�ʒu	= { this->structLayer[i].n�\�����J�n�ʒuX, this->structLayer[i].n�\�����J�n�ʒuY };
		POINT pt�\�����I���ʒu	= { this->structLayer[i].n�\�����I���ʒuX, this->structLayer[i].n�\�����I���ʒuY };
		double db���ړ�����ms	= this->structLayer[i].db���ړ�����ms;
		double db�ړ��J�n����ms	= this->structLayer[i].db�ړ��J�n����ms; if( g_Timer.Get() < db�ړ��J�n����ms ) db�ړ��J�n����ms = g_Timer.Get();

		double db���ݎ���ms		= ( g_Timer.Get() - db�ړ��J�n����ms ) * g_DTX.fSpeed;	// �J�n����̌o�ߎ���[ms]
		DWORD  dw�t���[���ԍ�	= pAVI->avi._��������t���[���ԍ���Ԃ�( db���ݎ���ms );

		// �I������idb���ړ�����ms==0 �̂Ƃ���AVI�Đ��I���܂Łj
		if( db���ړ�����ms != 0.0 && db���ړ�����ms < db���ݎ���ms )
		{
			this->structLayer[i].db���ړ�����ms   = 0;
			this->structLayer[i].db�ړ��J�n����ms = INIT_TIME;	// �Đ��I��
			return 0;
		}

		// ���ݍ��W�̌v�Z�G�ȉ��̂Q�ϐ����Z�o����B
		RECT rc�摜���̈�;		// �摜�̍��オ 0, 0
		RECT rc�\�����̈�;		// BGA�̈�̍��オ 0, 0

		if( db���ړ�����ms == 0.0 )
		{
			// �����ʒu�E�����T�C�Y�������ƕێ�
			SetRect( &rc�摜���̈�,
				pt�摜���J�n�ʒu.x,
				pt�摜���J�n�ʒu.y,
				pt�摜���J�n�ʒu.x + sz�J�n�T�C�Y.cx,
				pt�摜���J�n�ʒu.y + sz�J�n�T�C�Y.cy );
			
			SetRect( &rc�\�����̈�,
				pt�\�����J�n�ʒu.x,
				pt�\�����J�n�ʒu.y,
				pt�\�����J�n�ʒu.x + sz�J�n�T�C�Y.cx,
				pt�\�����J�n�ʒu.y + sz�J�n�T�C�Y.cy );
		}
		else
		{
			// db����; �J�n���I�� �̂Ƃ� 0.0 �� 1.0
			double db���� = db���ݎ���ms / db���ړ�����ms;

			// ���݂̃T�C�Y�i�摜�E�\�����ʁj
			SIZE sz���݂̃T�C�Y = {
				sz�J�n�T�C�Y.cx + (LONG)( ( sz�I���T�C�Y.cx - sz�J�n�T�C�Y.cx ) * db���� ),
				sz�J�n�T�C�Y.cy + (LONG)( ( sz�I���T�C�Y.cy - sz�J�n�T�C�Y.cy ) * db���� )
			};

			// �]�����̈�
			SetRect( &rc�摜���̈�,
				(LONG)( ( pt�摜���I���ʒu.x - pt�摜���J�n�ʒu.x ) * db���� ),
				(LONG)( ( pt�摜���I���ʒu.y - pt�摜���J�n�ʒu.y ) * db���� ),
				(LONG)( ( pt�摜���I���ʒu.x - pt�摜���J�n�ʒu.x ) * db���� ) + sz���݂̃T�C�Y.cx,
				(LONG)( ( pt�摜���I���ʒu.y - pt�摜���J�n�ʒu.y ) * db���� ) + sz���݂̃T�C�Y.cy );

			// �]����̈�
			SetRect( &rc�\�����̈�,
				(LONG)( ( pt�\�����I���ʒu.x - pt�\�����J�n�ʒu.x ) * db���� ),
				(LONG)( ( pt�\�����I���ʒu.y - pt�\�����J�n�ʒu.y ) * db���� ),
				(LONG)( ( pt�\�����I���ʒu.x - pt�\�����J�n�ʒu.x ) * db���� ) + sz���݂̃T�C�Y.cx,
				(LONG)( ( pt�\�����I���ʒu.y - pt�\�����J�n�ʒu.y ) * db���� ) + sz���݂̃T�C�Y.cy );

			// �]���������Ȃ��Ȃ�X�L�b�v�@ �N���b�s���O�O
			if( rc�摜���̈�.right <= 0 || rc�摜���̈�.bottom <= 0 || rc�摜���̈�.left >= szAVI�T�C�Y.cx || rc�摜���̈�.top >= szAVI�T�C�Y.cy ) continue;
			if( rc�\�����̈�.right <= 0 || rc�\�����̈�.bottom <= 0 || rc�\�����̈�.left >= szBGA�T�C�Y.cx || rc�\�����̈�.top >= szBGA�T�C�Y.cy ) continue;

			// �N���b�s���O�@ �]����
			if( rc�摜���̈�.left < 0 )	{ rc�\�����̈�.left += -rc�摜���̈�.left; rc�摜���̈�.left = 0; }
			if( rc�摜���̈�.top  < 0 )	{ rc�\�����̈�.top  += -rc�摜���̈�.top;  rc�摜���̈�.top  = 0; }
			if( rc�摜���̈�.right  > szAVI�T�C�Y.cx ) { rc�\�����̈�.right  -= rc�摜���̈�.right  - szAVI�T�C�Y.cx; rc�摜���̈�.right  = szAVI�T�C�Y.cx; }
			if( rc�摜���̈�.bottom > szAVI�T�C�Y.cy ) { rc�\�����̈�.bottom -= rc�摜���̈�.bottom - szAVI�T�C�Y.cy; rc�摜���̈�.bottom = szAVI�T�C�Y.cy; }

			// �N���b�s���O�A �]����
			if( rc�\�����̈�.left < 0 ) { rc�摜���̈�.left += -rc�\�����̈�.left; rc�\�����̈�.left = 0; }
			if( rc�\�����̈�.top  < 0 ) { rc�摜���̈�.top  += -rc�\�����̈�.top;  rc�\�����̈�.top  = 0; }
			if( rc�\�����̈�.right  > szBGA�T�C�Y.cx ) { rc�摜���̈�.right  -= rc�\�����̈�.right  - szBGA�T�C�Y.cx; rc�\�����̈�.right  = szBGA�T�C�Y.cx; }
			if( rc�\�����̈�.bottom > szBGA�T�C�Y.cy ) { rc�摜���̈�.bottom -= rc�\�����̈�.bottom - szBGA�T�C�Y.cy; rc�\�����̈�.bottom = szBGA�T�C�Y.cy; }

			// �]���������Ȃ��Ȃ�X�L�b�v�A �N���b�s���O��
			if( rc�摜���̈�.left >= rc�摜���̈�.right || rc�摜���̈�.top >= rc�摜���̈�.bottom ) continue;
			if( rc�\�����̈�.left >= rc�\�����̈�.right || rc�\�����̈�.top >= rc�\�����̈�.bottom ) continue;
			if( rc�摜���̈�.right < 0 || rc�摜���̈�.bottom < 0 || rc�摜���̈�.left > szAVI�T�C�Y.cx || rc�摜���̈�.top > szAVI�T�C�Y.cy ) continue;
			if( rc�\�����̈�.right < 0 || rc�\�����̈�.bottom < 0 || rc�\�����̈�.left > szBGA�T�C�Y.cx || rc�\�����̈�.top > szBGA�T�C�Y.cy ) continue;
		}
	
		// �`��
		this->structLayer[i].dw�O��\�������t���[���ԍ� = dw�t���[���ԍ�;

		HDC hdc;
		int ret = 0;
		
		this->sfAVI.Clear();
		
		if( SUCCEEDED( this->sfAVI.GetDC( &hdc ) ) )
		{
			ret = pAVI->avi._�w�莞���̃t���[����DC�֕`�悷��( g_App.hWnd, hdc, db���ݎ���ms, 
				rc�摜���̈�.left, rc�摜���̈�.top, rc�摜���̈�.right - rc�摜���̈�.left, rc�摜���̈�.bottom - rc�摜���̈�.top,
				rc�\�����̈�.left, rc�\�����̈�.top, rc�\�����̈�.right - rc�\�����̈�.left, rc�\�����̈�.bottom - rc�\�����̈�.top );
			this->sfAVI.ReleaseDC( hdc );
			this->sfAVI.Draw( x, y );
		}
		if( ret != 0 )
		{
			// �Đ�����
			this->structLayer[i].db�ړ��J�n����ms = INIT_TIME;
		}
	}
	return 0;
}

		}//Play
	}//Stage
}//DTXViewer
