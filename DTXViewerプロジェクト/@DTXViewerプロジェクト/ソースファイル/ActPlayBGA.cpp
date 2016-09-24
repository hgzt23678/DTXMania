#include "stdafx.h"
#include "ActPlayBGA.h"
#include "DTX.h"
#include "DTXViewer.h"
#include "CTimer.h"

namespace DTXViewer {
	namespace Stage {
		namespace Play {

static const int s_nChannel[8] = {
	0x04, 0x07, 0x55, 0x56, 0x57, 0x58, 0x59, 0x60,
};

ActPlayBGA::ActPlayBGA()
{
	this->bFirstWork = true;
	this->bActivate = false;
}

bool ActPlayBGA::MyOneTimeSceneInit()
{
	// �����ɃR�[�h���L�q����BAddChild() �ȂǁB

	return Activity::MyOneTimeSceneInit();
}
bool ActPlayBGA::MyFinalCleanup()
{
	// �����ɃR�[�h���L�q����B

	return Activity::MyFinalCleanup();
}
bool ActPlayBGA::MyInitDeviceObjects()
{
	// �����ɃR�[�h���L�q����BCTexture/CSurface::InitDeviceObjects() �ȂǁB

	return Activity::MyInitDeviceObjects();
}
bool ActPlayBGA::MyDeleteDeviceObjects()
{
	// �����ɃR�[�h���L�q����B

	// ���� Activity::MyDeleteDeviceObjects() �ł́A�q�e�N�X�`���E�q�T�[�t�F�C�X�� Delete (CTexture/CSurface::Delete) ���s����B
	return Activity::MyDeleteDeviceObjects();
}
bool ActPlayBGA::MyActivate()
{
	this->bFirstWork = true;
	// �����ɃR�[�h���L�q����B
	for( int i = 0; i < 8; i++ ) {
		this->structLayer[i].pBMP = NULL;
		this->structLayer[i].pBMPTEX = NULL;
		this->structLayer[i].db�ړ��J�n����ms = INIT_TIME;
	}

	// ���� Activity::MyActivate() �ł́Athis->bActivity �̃Z�b�g�Ȃ�тɎq�T�[�t�F�C�X�E�q�e�N�X�`���� Restore (MyRestoreDeviceTexture/Surface) ���s����B
	// �Ȃ��A�q Activity �̊������͍s���Ȃ��B�i���̃��\�b�h�𔲂��Ă���s����B�j
	return Activity::MyActivate();
}
bool ActPlayBGA::MyUnActivate()
{
	// �����ɃR�[�h���L�q����B

	// ���� Activity::MyUnActivate() �ł́Athis->bActivity �̃��Z�b�g�Ȃ�тɎq�T�[�t�F�C�X�E�q�e�N�X�`���� Invalidate (MyInvalidateDeviceObjects) ���s����B
	// �Ȃ��A�qActivity �̔񊈐����͍s���Ȃ��B�i���̃��\�b�h�𔲂��Ă���s����B�j
	return Activity::MyUnActivate();
}
bool ActPlayBGA::MyRestoreDeviceTextures()
{
	// �����ɃR�[�h���L�q����B

	// ���� Activity::MyRestoreDeviceTextures() �ł́A�q�e�N�X�`���� Restore (CTexture::Restore) ���s����B
	return Activity::MyRestoreDeviceTextures();
}
bool ActPlayBGA::MyRestoreDeviceSurfaces()
{
	// �����ɃR�[�h���L�q����B

	// ���� Activity::MyRestoreDeviceSurfaces() �ł́A�q�T�[�t�F�C�X�� Restore (CSurface::Restore) ���s����B
	return Activity::MyRestoreDeviceSurfaces();
}
bool ActPlayBGA::MyInvalidateDeviceObjects()
{
	// �����ɃR�[�h���L�q����B

	// ���� Activity::MyInvalidateDeviceObjects() �ł́A�q�e�N�X�`���E�q�T�[�t�F�C�X�� Invalidate (CTexture/CSurface::Invalidate) ���s����B
	return Activity::MyRestoreDeviceSurfaces();
}
//
void ActPlayBGA::Start( int n�`�����l��, DTX::BMP *bmp, DTX::BMPTEX* bmptex, int n�J�n�T�C�YW, int n�J�n�T�C�YH, int n�I���T�C�YW, int n�I���T�C�YH, int n�摜���J�n�ʒuX, int n�摜���J�n�ʒuY, int n�摜���I���ʒuX, int n�摜���I���ʒuY, int n�\�����J�n�ʒuX, int n�\�����J�n�ʒuY, int n�\�����I���ʒuX, int n�\�����I���ʒuY, double db���ړ�����ms, double db�ړ��J�n����ms )
{
	// �`�����l���`�F�b�N
	for( int i = 0; i < 8; i++ )
	{
		if( n�`�����l�� == s_nChannel[i] )
		{
			// �Z���ɒl��ݒ�												BMP/BMPTEX	BGA		BGAPAN
			this->structLayer[i].pBMP				= bmp;				//		��		��		��
			this->structLayer[i].pBMPTEX			= bmptex;			//		��		��		��
			this->structLayer[i].sz�J�n�T�C�Y.cx	= n�J�n�T�C�YW;		//		��		��		��
			this->structLayer[i].sz�J�n�T�C�Y.cy	= n�J�n�T�C�YH;		//		��		��		��
			this->structLayer[i].sz�I���T�C�Y.cx	= n�I���T�C�YW;		//		--		--		��
			this->structLayer[i].sz�I���T�C�Y.cy	= n�I���T�C�YH;		//		--		--		��
			this->structLayer[i].pt�摜���J�n�ʒu.x	= n�摜���J�n�ʒuX;	//		--		��		��
			this->structLayer[i].pt�摜���J�n�ʒu.y	= n�摜���J�n�ʒuY;	//		--		��		��
			this->structLayer[i].pt�摜���I���ʒu.x	= n�摜���I���ʒuX;	//		--		--		��
			this->structLayer[i].pt�摜���I���ʒu.y	= n�摜���I���ʒuY;	//		--		--		��
			this->structLayer[i].pt�\�����J�n�ʒu.x	= n�\�����J�n�ʒuX;	//		--		��		��
			this->structLayer[i].pt�\�����J�n�ʒu.y	= n�\�����J�n�ʒuY;	//		--		��		��
			this->structLayer[i].pt�\�����I���ʒu.x	= n�\�����I���ʒuX;	//		--		--		��
			this->structLayer[i].pt�\�����I���ʒu.y	= n�\�����I���ʒuY;	//		--		--		��
			this->structLayer[i].db���ړ�����ms		= db���ړ�����ms;	//		--		--		��
			this->structLayer[i].db�ړ��J�n����ms	= (db�ړ��J�n����ms != INIT_TIME) ? db�ړ��J�n����ms : g_Timer.Get();
		}
	}
}
void ActPlayBGA::SkipStart( double db�ړ��J�n����ms )
{
	for( DTX::Chip* cell = g_DTX.pChip; cell != NULL; cell = cell->next )
	{
		// �J�n���������Ȃ�A���̃`�b�v�Œ����ł��؂�B
		if( cell->dbTime > db�ړ��J�n����ms ) 
			break;

		// BMP, BMPTEX, BGA, BGAPAN �Ȃ�Đ��w��
		switch( cell->BGAtype )
		{

		case BGATYPE_BMP:
			if( cell->pBMP )
				Start( cell->nChannel, cell->pBMP, NULL,
					   cell->pBMP->pSurface->dwWidth, cell->pBMP->pSurface->dwHeight, 0, 0,
					   0, 0, 0, 0,
					   0, 0, 0, 0, 0, cell->dbTime );
			break;

		case BGATYPE_BMPTEX:
			if( cell->pBMPTEX )
				Start( cell->nChannel, NULL, cell->pBMPTEX,
					   cell->pBMPTEX->pTexture->dwWidth, cell->pBMPTEX->pTexture->dwHeight, 0, 0,
					   0, 0, 0, 0,
					   0, 0, 0, 0, 0, cell->dbTime );
			break;

		case BGATYPE_BGA:
			if( cell->pBGA )
				Start( cell->nChannel, cell->pBMP, cell->pBMPTEX,
					   cell->pBGA->x2 - cell->pBGA->x1, cell->pBGA->y2 - cell->pBGA->y1, 0, 0, 
					   cell->pBGA->x1, cell->pBGA->y1, 0, 0,
					   cell->pBGA->ox, cell->pBGA->oy, 0, 0, 0, cell->dbTime );
			break;

		case BGATYPE_BGAPAN:
			if( cell->pBGAPan )
				Start( cell->nChannel, cell->pBMP, cell->pBMPTEX,
						cell->pBGAPan->sw, cell->pBGAPan->sh, cell->pBGAPan->ew, cell->pBGAPan->eh,
						cell->pBGAPan->ssx, cell->pBGAPan->ssy, cell->pBGAPan->sex, cell->pBGAPan->sey,
						cell->pBGAPan->dsx, cell->pBGAPan->dsy, cell->pBGAPan->dex, cell->pBGAPan->dey,
						cell->dbLong, cell->dbTime );
			break;
		}
	}
}
void ActPlayBGA::Stop()
{
	for( int i = 0; i < 8; i++ )
		this->structLayer[i].db�ړ��J�n����ms = INIT_TIME;
}
void ActPlayBGA::Cont( double db�ĊJ����ms )
{
	for( int i = 0; i < 8; i++ )
	{
		if(  ( this->structLayer[i].pBMP    == NULL && this->structLayer[i].pBMPTEX == NULL ) 
		  || ( this->structLayer[i].pBMP    != NULL && ( ! this->structLayer[i].pBMP->bUse    || this->structLayer[i].pBMP->pSurface    == NULL ) )
		  || ( this->structLayer[i].pBMPTEX != NULL && ( ! this->structLayer[i].pBMPTEX->bUse || this->structLayer[i].pBMPTEX->pTexture == NULL ) ) )
			continue;

		this->structLayer[i].db�ړ��J�n����ms = db�ĊJ����ms;
	}
}
void ActPlayBGA::ChangeScope( int n�`�����l��, DTX::BMP* bmp, DTX::BMPTEX* bmptex )
{
	// �`�����l���`�F�b�N
	for( int i = 0; i < 8; i++ )
	{
		if( n�`�����l�� == s_nChannel[i] )
		{
			// �Z���ɒl��ݒ�(bmp, bmptex �̂�)
			this->structLayer[i].pBMP		= bmp;
			this->structLayer[i].pBMPTEX	= bmptex;
		}
	}
}
int  ActPlayBGA::MainWork( int x, int y )
{
	for( int i = 0; i < 8; i++ )
	{
		// ���g�p�Ȃ�X�L�b�v
		if( this->structLayer[i].db�ړ��J�n����ms == INIT_TIME
			|| ( this->structLayer[i].pBMP    == NULL && this->structLayer[i].pBMPTEX == NULL ) 
			|| ( this->structLayer[i].pBMP    != NULL && ( ! this->structLayer[i].pBMP->bUse    || this->structLayer[i].pBMP->pSurface    == NULL ) )
			|| ( this->structLayer[i].pBMPTEX != NULL && ( ! this->structLayer[i].pBMPTEX->bUse || this->structLayer[i].pBMPTEX->pTexture == NULL ) ) )
			continue;

		// �p�����[�^���o�E�Z�o
		SIZE	sz�J�n�T�C�Y		= this->structLayer[i].sz�J�n�T�C�Y;
		SIZE	sz�I���T�C�Y		= this->structLayer[i].sz�I���T�C�Y;
		POINT	pt�摜���J�n�ʒu	= this->structLayer[i].pt�摜���J�n�ʒu;
		POINT	pt�摜���I���ʒu	= this->structLayer[i].pt�摜���I���ʒu;
		POINT	pt�\�����J�n�ʒu	= this->structLayer[i].pt�\�����J�n�ʒu;
		POINT	pt�\�����I���ʒu	= this->structLayer[i].pt�\�����I���ʒu;
		double	db���ړ�����ms		= this->structLayer[i].db���ړ�����ms;
		double	db�ړ��J�n����ms	= this->structLayer[i].db�ړ��J�n����ms; if( g_Timer.Get() < db�ړ��J�n����ms ) db�ړ��J�n����ms = g_Timer.Get();
		SIZE	szBGA�T�C�Y			= { BGA_W, BGA_H };
		SIZE	szBMP�T�C�Y			= { (this->structLayer[i].pBMP) ? this->structLayer[i].pBMP->pSurface->dwWidth  : this->structLayer[i].pBMPTEX->pTexture->dwImageWidth,
										(this->structLayer[i].pBMP) ? this->structLayer[i].pBMP->pSurface->dwHeight : this->structLayer[i].pBMPTEX->pTexture->dwImageHeight };

		double	db���ݎ���ms		= ( g_Timer.Get() - db�ړ��J�n����ms ) * g_DTX.fSpeed;	// �J�n����̌o�ߎ���[ms]

		// �I������ (db���ړ�����ms==0 �̂Ƃ��͏I���Ȃ��j
		if( db���ړ�����ms != 0.0 && db���ړ�����ms < db���ݎ���ms )
		{
			this->structLayer[i].pt�摜���J�n�ʒu = pt�摜���J�n�ʒu = pt�摜���I���ʒu;
			this->structLayer[i].pt�\�����J�n�ʒu = pt�\�����J�n�ʒu = pt�\�����I���ʒu;
			this->structLayer[i].sz�J�n�T�C�Y     = sz�J�n�T�C�Y     = sz�I���T�C�Y;
			this->structLayer[i].db���ړ�����ms	  = db���ړ�����ms   = 0.0;
			//db�ړ��J�n����nms	= this->structLayer[i].db�ړ��J�n����ms = INIT_TIME;		�^�C�}�������������烌�C�����̃X�L�b�v����邩��_��
		}

		/////////////
		// ���ݎ�������摜�̈�ƕ\���̈���v�Z����

		RECT rc�摜���̈�;		// �摜�̍��オ 0, 0
		RECT rc�\�����̈�;		// BGA�̈�̍��オ 0, 0

		if( db���ړ�����ms == 0.0 )
		{
			// �J�n�ʒu�E�J�n�T�C�Y�������ƕێ�
			rc�摜���̈�.left   = pt�摜���J�n�ʒu.x;
			rc�摜���̈�.top    = pt�摜���J�n�ʒu.y;
			rc�摜���̈�.right  = rc�摜���̈�.left + sz�J�n�T�C�Y.cx;
			rc�摜���̈�.bottom = rc�摜���̈�.top  + sz�J�n�T�C�Y.cy;
			
			rc�\�����̈�.left   = pt�\�����J�n�ʒu.x;
			rc�\�����̈�.top    = pt�\�����J�n�ʒu.y;
			rc�\�����̈�.right  = rc�\�����̈�.left + sz�J�n�T�C�Y.cx;
			rc�\�����̈�.bottom = rc�\�����̈�.top  + sz�J�n�T�C�Y.cy;
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
			rc�摜���̈�.left   = pt�摜���J�n�ʒu.x + (LONG)( ( pt�摜���I���ʒu.x - pt�摜���J�n�ʒu.x ) * db���� );
			rc�摜���̈�.top    = pt�摜���J�n�ʒu.y + (LONG)( ( pt�摜���I���ʒu.y - pt�摜���J�n�ʒu.y ) * db���� );
			rc�摜���̈�.right  = rc�摜���̈�.left + sz���݂̃T�C�Y.cx;
			rc�摜���̈�.bottom = rc�摜���̈�.top  + sz���݂̃T�C�Y.cy;

			// �]����̈�
			rc�\�����̈�.left   = pt�\�����J�n�ʒu.x + (LONG)( ( pt�\�����I���ʒu.x - pt�\�����J�n�ʒu.x ) * db���� );
			rc�\�����̈�.top    = pt�\�����J�n�ʒu.y + (LONG)( ( pt�\�����I���ʒu.y - pt�\�����J�n�ʒu.y ) * db���� );
			rc�\�����̈�.right  = rc�\�����̈�.left + sz���݂̃T�C�Y.cx;
			rc�\�����̈�.bottom = rc�\�����̈�.top  + sz���݂̃T�C�Y.cy;
		}

		// �]���������Ȃ��Ȃ�X�L�b�v�@ �N���b�s���O�O
		if( rc�摜���̈�.right <= 0 || rc�摜���̈�.bottom <= 0 || rc�摜���̈�.left >= szBMP�T�C�Y.cx || rc�摜���̈�.top >= szBMP�T�C�Y.cy ) continue;
		if( rc�\�����̈�.right <= 0 || rc�\�����̈�.bottom <= 0 || rc�\�����̈�.left >= szBGA�T�C�Y.cx || rc�\�����̈�.top >= szBGA�T�C�Y.cy ) continue;

		// �N���b�s���O�@ �]����
		if( rc�摜���̈�.left < 0 )	{ rc�\�����̈�.left += -rc�摜���̈�.left; rc�摜���̈�.left = 0; }
		if( rc�摜���̈�.top  < 0 )	{ rc�\�����̈�.top  += -rc�摜���̈�.top;  rc�摜���̈�.top  = 0; }
		if( rc�摜���̈�.right  > szBMP�T�C�Y.cx ) { rc�\�����̈�.right  -= rc�摜���̈�.right  - szBMP�T�C�Y.cx; rc�摜���̈�.right  = szBMP�T�C�Y.cx; }
		if( rc�摜���̈�.bottom > szBMP�T�C�Y.cy ) { rc�\�����̈�.bottom -= rc�摜���̈�.bottom - szBMP�T�C�Y.cy; rc�摜���̈�.bottom = szBMP�T�C�Y.cy; }

		// �N���b�s���O�A �]����
		if( rc�\�����̈�.left < 0 ) { rc�摜���̈�.left += -rc�\�����̈�.left; rc�\�����̈�.left = 0; }
		if( rc�\�����̈�.top  < 0 ) { rc�摜���̈�.top  += -rc�\�����̈�.top;  rc�\�����̈�.top  = 0; }
		if( rc�\�����̈�.right  > szBGA�T�C�Y.cx ) { rc�摜���̈�.right  -= rc�\�����̈�.right  - szBGA�T�C�Y.cx; rc�\�����̈�.right  = szBGA�T�C�Y.cx; }
		if( rc�\�����̈�.bottom > szBGA�T�C�Y.cy ) { rc�摜���̈�.bottom -= rc�\�����̈�.bottom - szBGA�T�C�Y.cy; rc�\�����̈�.bottom = szBGA�T�C�Y.cy; }

		// �]���������Ȃ��Ȃ�X�L�b�v�A �N���b�s���O��
		if( rc�摜���̈�.left >= rc�摜���̈�.right || rc�摜���̈�.top >= rc�摜���̈�.bottom ) continue;
		if( rc�\�����̈�.left >= rc�\�����̈�.right || rc�\�����̈�.top >= rc�\�����̈�.bottom ) continue;
		if( rc�摜���̈�.right < 0 || rc�摜���̈�.bottom < 0 || rc�摜���̈�.left > szBMP�T�C�Y.cx || rc�摜���̈�.top > szBMP�T�C�Y.cy ) continue;
		if( rc�\�����̈�.right < 0 || rc�\�����̈�.bottom < 0 || rc�\�����̈�.left > szBGA�T�C�Y.cx || rc�\�����̈�.top > szBGA�T�C�Y.cy ) continue;

		// ���Z���^�����O�́ABGAPAN �ł͈Ӗ��Ȃ��i�����j�̂Ŕp�~(2003/03/02)

		// �`��
		if( this->structLayer[i].pBMP )
		{
			this->structLayer[i].pBMP->pSurface->ColorKeyEnable( (i == 0) ? false : (g_DTX.bBlackColorKey ? true : false) );	// �ŉ��w���C���͓��߂��Ȃ�
			this->structLayer[i].pBMP->pSurface->Draw( x + rc�\�����̈�.left, y + rc�\�����̈�.top, &rc�摜���̈� );
		}
		else if( this->structLayer[i].pBMPTEX )
		{
			this->structLayer[i].pBMPTEX->pTexture->Draw( x + rc�\�����̈�.left, y + rc�\�����̈�.top, &rc�摜���̈� );
		}
	}
	return 0;
}

		}//Play
	}//Stage
}//DTXViewer
