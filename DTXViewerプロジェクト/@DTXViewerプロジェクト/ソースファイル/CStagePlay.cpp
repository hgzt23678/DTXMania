#include "stdafx.h"
// FDK includes
#include "Debug.h"
#include "DTX.h"
// App includes
#include "../resource.h"
#include "DTXViewer.h"
#include "CStage.h"
#include "CStagePlay.h"
#include "ActPlayAVI.h"
#include "ActPlayBGA.h"

namespace DTXViewer {
	namespace Stage {
		namespace Play {

static RECT s_rcChips[28] = {
	{ 640,2,651,5 }, { 651,2,662,5 },												// 0:BPM, 1:BPx
	{ 662,2,673,5 }, { 673,2,684,5 }, { 684,2,695,5 }, { 695,2,706,3 },				// 2:HH, 3:SD, 4:BD, 5:HT
	{ 706,2,717,5 }, { 717,2,728,5 }, { 728,2,739,5 },								// 6:LT, 7:FT, 8;CY
	{ 739,2,746,5 },																// 9:FI
	{ 746,2,757,5 }, { 757,2,768,5 }, { 768,2,779,5 },								// 10:BGM, 11:BGA, 12:AVI
	{ 779,2,790,5 }, { 790,2,801,5 }, { 801,2,812,5 }, { 812,2,819,3 },				// 13:R1, 14:G1, 15:B1. 16:W1
	{ 819,2,830,5 }, { 830,2,841,5 }, { 841,2,852,5 }, { 852,2,859,3 },				// 17:R1, 18:G1, 19:B1. 20:W1
	{ 859,2,870,5 }, { 870,2,881,5 }, { 881,2,892,5 }, { 892,2,903,3 }, { 903,2,914,5 },	// 21�`25: SE1�`5
	{ 879,2,914,5 },																// 26: GuitarOPEN
	{ 914,2,925,5 }																	// 27: LC
};
static RECT s_rcRing  = { 640, 5, 640+12, 5+12 };									// HHO,RD �̗�
static RECT s_rcLine  = { 640, 0, 640+244, 1 };										// ���ߐ�
static RECT s_rcSubLine  = { 640, 1, 640+244, 2 };									// ����
static RECT s_rcNums[10] = {
	{   0,   0,  18,  31 },	{  18,   0,  36,  31 },	{  36,   0,  54,  31 },	{  54,   0,  72,  31 },	{  72,   0,  90,  31 },
	{   0,  31,  18,  62 },	{  18,  31,  36,  62 },	{  36,  31,  54,  62 },	{  54,  31,  72,  62 },	{  72,  31,  90,  62 }
};
static const int s_n�`�����l��0Ato���[��07[10] = { 1,2,3,4,5,7,6,1,7,0 };

CStagePlay::CStagePlay()
{
	this->str�X�e�[�W�� = _T("���t���");
	this->stageID = STAGE_PLAY;
	this->phaseID = PHASE_COMMON_�ʏ���;
	this->bFirstWork = true;
	this->bActivate = false;
}
//
bool CStagePlay::Load( LPCTSTR strFileName )
{
	this->bLoading�v������ = true;
	this->strFileName = strFileName;

	return true;
}
void CStagePlay::Play( int nPart )
{
	this->bPlaying�v������ = true;
	this->n�J�n���ߔԍ� = nPart;
	if( this->n�J�n���ߔԍ� < 0 ) this->n�J�n���ߔԍ� = 0;
}
void CStagePlay::Stop()
{
	this->struct���t���.b���t�� = false;

	g_DTX.StopAllWaves();
	this->actAVI.Stop();			// AVI ��~
	this->actBGA.Stop();			// BGA ��~
}
void CStagePlay::SkipStart( double db�J�n����ms )
{
	DTX::Chip* pChip;
	for( pChip = g_DTX.pChip; pChip != NULL; pChip = pChip->next )
	{
		if( pChip->dbTime >= db�J�n����ms )
			break;

		this->ProcessCell( pChip, true );
	}
	this->struct���t���.p���݂̃g�b�vChip = pChip;
}
//
int	 CStagePlay::MainWork()
{
	// �߂�l: 0=�p��, 1=���t�I��, -1=�A�v���I��

	// �w�i�`��
	RECT rc = { 0, 0, 640, 480 };
	this->sfMap.Draw( 0, 0, &rc );

	if( this->bLoading�v������ )
	{
		this->bLoading�v������ = false;
		this->ctNowLoading�\��.Start( 0, 50, 1, &g_Timer );
		this->phaseID = PHASE_PLAY_LOADING1;
	}

	switch( this->phaseID )
	{
	case PHASE_PLAY_IDLE:
		break;

	case PHASE_PLAY_LOADING1: {
		RECT rcNowLoading = { 640, 17, 640+229, 17+40 };
		this->sfMap.Draw( 176-225/2, 180-41/2, &rcNowLoading );

		this->ctNowLoading�\��.Step();
		if( this->ctNowLoading�\��.b�I���l�ɒB����() )
			this->phaseID = PHASE_PLAY_LOADING2;
		break;
	}
	case PHASE_PLAY_LOADING2:
		if( this->tDTX�t�@�C���̓ǂݍ���() )
			this->phaseID = PHASE_PLAY_PLAYING1;
		else {
			this->phaseID = PHASE_PLAY_IDLE;
			this->bPlaying�v������ = false;
		}
		break;

	case PHASE_PLAY_PLAYING1:
		if( this->bPlaying�v������ ) {
			this->t�Đ��J�n();
			this->phaseID = PHASE_PLAY_PLAYING2;
		}
		break;

	case PHASE_PLAY_PLAYING2:
		if( this->struct���t���.b���t�� )
		{
			// BGA�̈���N���A
			{
				if( g_DTX.bUseAVI || g_DTX.bUseBMP || g_DTX.bUseBMPTEX )
				{
					DDBLTFX ddbfx;
					ZeroMemory( &ddbfx, sizeof( DDBLTFX ) );
					ddbfx.dwSize = sizeof( DDBLTFX );
					ddbfx.dwFillColor = 0xFF000000;
					RECT rc = { BGA_X, BGA_Y, BGA_X+BGA_W, BGA_Y+BGA_H };
					g_App.pddsBackBuffer->Blt( &rc, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbfx );
				}
			}
			//
			// �`�b�v�i�s�`��
			{
				if( this->struct���t���.p���݂̃g�b�vChip == NULL ) {
					this->Stop();
					this->phaseID = PHASE_PLAY_IDLE;
					return 1;	// ���t�I��
				}

				// �X�N���[�����x[dot/ms] �Z�o
				static const double BPM = 150.0;			// BPM = �P���߂̒������A150 BPM ��
				static const double LEN = 264.0;			// LEN = 264 �h�b�g
				static const double s_dbScrollSpeed = 2.0;	// ���ʃX�N���[�����x�i�Œ�j
				double dbDotPerMS_D = ( s_dbScrollSpeed * 0.5       ) * ( BPM / 4.0 ) * LEN / 60000.0;
				double dbDotPerMS_G = ( s_dbScrollSpeed * 0.5 * 0.5 ) * ( BPM / 4.0 ) * LEN / 60000.0;
				double dbDotPerMS_B = ( s_dbScrollSpeed * 0.5 * 0.5 ) * ( BPM / 4.0 ) * LEN / 60000.0;

				// �g�b�v�Z�����珇�ԂɁA��ʂɓ��邾���`��B
				for( DTX::Chip* cell = this->struct���t���.p���݂̃g�b�vChip; cell != NULL; cell = cell->next )
				{
					// �o�[����̋���[dot] �Z�o
					cell->nDotFromBarD = (long)( (cell->dbTime - g_Timer.Get() ) * dbDotPerMS_D );	// �h�����֌W�A���̑�
					cell->nDotFromBarG = (long)( (cell->dbTime - g_Timer.Get() ) * dbDotPerMS_G );	// �M�^�[�֌W
					cell->nDotFromBarB = (long)( (cell->dbTime - g_Timer.Get() ) * dbDotPerMS_B );	// �x�[�X�֌W
				
					if( MIN( MIN( cell->nDotFromBarD, cell->nDotFromBarG ), cell->nDotFromBarB ) > 480 )
						break;	// �h�����A�M�^�[�A�x�[�X�̂����A��ԒZ����������ʕ����傫���Ȃ炻���ŏI���B
				
					// �g�b�v�Z������ւ��`�F�b�N�G
					// ��ʉ��[��艺�Ȃ�g�b�v�Z������ւ��i�q�b�g���Ė����Ȃ�܂�����ւ��Ȃ��j�B
					// ���邢�́A�q�b�g���ĂȂ��Ă��A-400�𒴂����狭���I�ɓ���ւ���i�\�h��j
					if( cell == this->struct���t���.p���݂̃g�b�vChip )
					{
						if( ( cell->nDotFromBarD < -65 && cell->bHit ) || cell->nDotFromBarD < -400 )		// nDotFromBar �̓h�����ł��M�^�[�ł�����
						{
							this->struct���t���.p���݂̃g�b�vChip = this->struct���t���.p���݂̃g�b�vChip->next;
							continue;
						}
					}

					// �`�����l�����Ƃɏ���
					this->ProcessCell( cell, false );
				}
			}
			//
			// AVI �i�s�`��
			{
				if( g_DTX.bUseAVI )
					this->actAVI.MainWork( BGA_X, BGA_Y );
			}
			//
			// BGA �i�s�`��
			{
				if( g_DTX.bUseBMP || g_DTX.bUseBMPTEX )
					this->actBGA.MainWork( BGA_X, BGA_Y );
			}
		}
		break;
	}

	if( g_DirectInput.IsKeyPushDown( DIK_ESCAPE ) )
		return -1;		// �A�v���I��

	return 0;
}
bool CStagePlay::tDTX�t�@�C���̓ǂݍ���()
{
	this->Stop();

	// ���t��������������
	{
		this->t���t��������������();
		g_DTX.Clear();
	}
	//
	// DTX�̓ǂݍ���
	{
		if( ! g_DTX.Load( this->strFileName ) )
		{
			Debug::Out( _T("DTX�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B(%s)\n"), this->strFileName.c_str() );
			return false;
		}
		g_DTX.LoadBMP();
		g_DTX.LoadAVI();
	}
	//
	// �Đ����x�̐ݒ�
	{
		g_DTX.SetSpeed( g_DTX.fSpeed );	// #DTXVPLAYSPEED: 
	}
	//
	// �E�B���h�E�^�C�g�����u�Ȗ� [���]�v�ɕύX
	{
		TCHAR buf[ 1024 ];
		_stprintf_s( buf, 1024, _T("%s [%s]"), g_DTX.strTitle.c_str(), g_DTX.strArtist.c_str() );
		::SetWindowText( g_App.hWnd, buf );
	}
	//
	return true;
}
void CStagePlay::t�Đ��J�n()
{
	this->Stop();

	// ���ߔԍ� nPart ����Đ��J�n�����ƍĐ��J�n�`�b�v���Z�o;
	// �Đ��J�n�����͏��ߐ��� dbTime ����擾�B�i����\�̔{���̈ʒu�ɂ͕K���I�u�W�F�N�g�i���ߐ��j������Ƃ����O��j
	{
		this->struct���t���.db�J�n����ms = INIT_TIME;
		this->struct���t���.p���݂̃g�b�vChip = NULL;

		for( DTX::Chip* cell = g_DTX.pChip; cell != NULL; cell = cell->next )
		{
			if( cell->dwPosition >= (DWORD)( (this->n�J�n���ߔԍ�+1) * DTX_RESOLVE ) )
			{
				this->struct���t���.db�J�n����ms = cell->dbTime;
				break;
			}
		}
	}
	//
	// ���t�J�n
	{
		if( this->struct���t���.db�J�n����ms != INIT_TIME )
		{
			Debug::Out( _T("���t�J�n���� = %e [ms]\n"), this->struct���t���.db�J�n����ms );

			// �^�C�}���Z�b�g���`��J�n
			g_Timer.Reset();
			g_Timer.Set( this->struct���t���.db�J�n����ms );				// �Đ��J�n�����Z�b�g
			g_DTX.SkipStart( this->struct���t���.db�J�n����ms );			// �S�`�b�v�ɂ��� dwStartTime �ɍĐ������ۂ��������A�Đ����Ȃ�K�؂Ȍ��Ɋe�Đ��J�[�\�����ړ����čĐ�
			this->actAVI.SkipStart( this->struct���t���.db�J�n����ms );	// AVI �̉��t�J�n
			this->actBGA.SkipStart( this->struct���t���.db�J�n����ms );	// BGA �̉��t�J�n
			this->SkipStart( this->struct���t���.db�J�n����ms );			// �`�b�v�̉��t�J�n(actBGA����ł��邱��; ChangeScope�̂���)
			this->struct���t���.b���t�� = true;

			// �܂� Loading �O�̉\���������̂ŁA�����ł� this->phaseID �͂�����Ȃ��B
		}
	}
}
void CStagePlay::ProcessCell( DTX::Chip* pChip, bool bSkip )
{
	switch( pChip->nChannel )
	{
	// �o�b�N�R�[���X
	case 0x01: {

		// �`�b�v�`��
		if( ! pChip->bHit && ! bSkip )
			this->sfMap.Draw( 146, BAR_Y-pChip->nDotFromBarD-1, &s_rcChips[10] );

		// �������ĂȂ���Δ�������
		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarD < 0 ) )
		{
			pChip->bHit = true;
			if( ! bSkip )
				g_DTX.PlayChip( pChip, 10, DTX_AUTOVOLUME );
		}
		break;
	}
	// BPM�ύX
	case 0x03: {
		
		// �\��
		if( ! bSkip && ! pChip->bHit && pChip->bVisible )
			this->sfMap.Draw( 2, BAR_Y-pChip->nDotFromBarD-1, &s_rcChips[0] );

		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarD < 0 ) )
		{
			// �\���p BPM �̍X�V
			this->struct���t���.db���݂�BPM�E�f�o�b�O�\���p = pChip->nParam * g_DTX.fSpeed + g_DTX.dbBaseBPM;
			pChip->bHit = true;
		}
		break;
	}
	// BPM�ύX�i�g���j
	case 0x08: {

		if( ! bSkip && ! pChip->bHit && pChip->bVisible )
			this->sfMap.Draw( 14, BAR_Y-pChip->nDotFromBarD-1, &s_rcChips[1] );

		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarD < 0 ) )
		{
			pChip->bHit = true;
			
			for( DTX::BPM* bc = g_DTX.pBPMLast; bc != NULL; bc = bc->prev )
			{
				if( bc->num == pChip->nRealParam )
				{
					// �\���p BPM �̍X�V
					this->struct���t���.db���݂�BPM�E�f�o�b�O�\���p = bc->bpm * g_DTX.fSpeed + g_DTX.dbBaseBPM;
					break;
				}
			}
		}
		break;
	}
	// BGA
	case 0x04: case 0x07: case 0x55: case 0x56: case 0x57: case 0x58: case 0x59: case 0x60: {
	
		if( ! bSkip && ! pChip->bHit && pChip->bVisible )
			this->sfMap.Draw( 158, BAR_Y-pChip->nDotFromBarD-1, &s_rcChips[11] );

		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarD < 0 ) )
		{
			pChip->bHit = true;
			if( bSkip ) break;

			switch( pChip->BGAtype )
			{
			case BGATYPE_BMP: {
				
				if( pChip->pBMP ) 
				{
					// len = 0 �Ȃ�]�������̂ݗL���B�i�]������� 0 �ł����B�j
					this->actBGA.Start( pChip->nChannel, pChip->pBMP, NULL,
						pChip->pBMP->pSurface->dwWidth, pChip->pBMP->pSurface->dwHeight, 0, 0,
						0, 0, 0, 0,
						0, 0, 0, 0,
						0.0 );
				}
				break;
			}
			case BGATYPE_BMPTEX: {
				
				if( pChip->pBMPTEX )
				{
					// len = 0 �Ȃ�]�������̂ݗL���B�i�]������� 0 �ł����B�j
					this->actBGA.Start( pChip->nChannel, NULL, pChip->pBMPTEX,
						pChip->pBMPTEX->pTexture->dwWidth, pChip->pBMPTEX->pTexture->dwHeight, 0, 0,
						0, 0, 0, 0,
						0, 0, 0, 0,
						0.0 );
				}
				break;
			}
			case BGATYPE_BGA: {
				
				if( pChip->pBGA && ( pChip->pBMP || pChip->pBMPTEX ) )
				{
					// len = 0 �Ȃ�]�������̂ݗL���B�i�]������� 0 �ł����B�j
					this->actBGA.Start( pChip->nChannel, pChip->pBMP, pChip->pBMPTEX,
						pChip->pBGA->x2 - pChip->pBGA->x1, pChip->pBGA->y2 - pChip->pBGA->y1, 0, 0, 
						pChip->pBGA->x1, pChip->pBGA->y1, 0, 0,
						pChip->pBGA->ox, pChip->pBGA->oy, 0, 0,
						0.0 );
				}
				break;
			}
			case BGATYPE_BGAPAN: {
				
				if( pChip->pBGAPan && ( pChip->pBMP || pChip->pBMPTEX ) )
				{
					this->actBGA.Start( pChip->nChannel, pChip->pBMP, pChip->pBMPTEX,
						pChip->pBGAPan->sw, pChip->pBGAPan->sh, pChip->pBGAPan->ew, pChip->pBGAPan->eh,
						pChip->pBGAPan->ssx, pChip->pBGAPan->ssy, pChip->pBGAPan->sex, pChip->pBGAPan->sey,
						pChip->pBGAPan->dsx, pChip->pBGAPan->dsy, pChip->pBGAPan->dex, pChip->pBGAPan->dey,
						pChip->dbLong );
                }
				break;
			}
			}
		}
		break;
	}
	// BGA�X�R�[�v�摜�؂�ւ�
	case 0xC4: case 0xC7: case 0xD5: case 0xD6: case 0xD7: case 0xD8: case 0xD9: case 0xE0: {

		if( ! bSkip && ! pChip->bHit && pChip->bVisible )
			this->sfMap.Draw( 158, BAR_Y-pChip->nDotFromBarD-1, &s_rcChips[11] );

		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarD < 0 ) )
		{
			pChip->bHit = true;

			if( pChip->BGAtype == BGATYPE_BMP || pChip->BGAtype == BGATYPE_BMPTEX )
			{
				static const int nCh[2][8] = {
					{ 0xC4, 0xC7, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xE0 },
					{ 0x04, 0x07, 0x55, 0x56, 0x57, 0x58, 0x59, 0x60 }
				};
				for( int i = 0; i < 8; i++)
					if( nCh[0][i] == pChip->nChannel )
						this->actBGA.ChangeScope( nCh[1][i], pChip->pBMP, pChip->pBMPTEX );
			}
		}
		break;
	}
	// �h�����`�b�v
	case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16: case 0x17: case 0x18: case 0x19: case 0x1A: {

		int l = s_n�`�����l��0Ato���[��07[ pChip->nChannel - 0x11 ];

		// �������ĂȂ���Δ�������
		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarD < 0 ) ) {
			if( ! bSkip )
				this->t�T�E���h�Đ�( pChip, PLAYPART_DRUMS, DTX_AUTOVOLUME );
			pChip->bHit   = true;
		}

		// �`�b�v�̕`��
		if( ! pChip->bHit && ! bSkip && pChip->bVisible )
		{
			// �`�b�v�`��
			this->sfMap.Draw( 33+12*l, BAR_Y-pChip->nDotFromBarD-1, &s_rcChips[ (pChip->nChannel!=0x1A)? 1+l : 27 ] );

			// HHO,RD�̓����O���`��
			if( pChip->nChannel == 0x18 || pChip->nChannel == 0x19 )
				this->sfMap.Draw( 33+12*l, BAR_Y-pChip->nDotFromBarD-5, &s_rcRing );
		}
		break;
	}
	// �h�����`�b�v�i�s���j
	case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: case 0x38: case 0x39: case 0x3A: {
		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarD < 0 ) )
			pChip->bHit = true;		// �������Ȃ�
		break;
	}
	// �h�����`�b�v�i�󂤂��w��j
	case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7: case 0xB8: case 0xB9: case 0xBC: {
		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarD < 0 ) )
			pChip->bHit = true;		// �������Ȃ�
		break;
	}
	// �M�^�[�`�b�v
	case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27: {

		// �������ĂȂ���Δ�������
		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarG < 0 ) )
		{
			if( ! bSkip )
				this->t�T�E���h�Đ�( pChip, PLAYPART_GUITAR, DTX_AUTOVOLUME );
			pChip->bHit = true;
		}

		// �`�b�v�̕`��
		if( ! bSkip && ! pChip->bHit && pChip->bVisible )
		{
			int y = 38 + pChip->nDotFromBarG-1;
			if( y >= 0 && y < 480 )
			{
				if( pChip->nChannel == 0x20 )	// OPEN
					this->sfMap.Draw( 188, y, &s_rcChips[26] );
				else
				{								// RGB
					if( pChip->nChannel & 0x04 ) this->sfMap.Draw( 188,      y, &s_rcChips[13] );
					if( pChip->nChannel & 0x02 ) this->sfMap.Draw( 188+12,   y, &s_rcChips[14] );
					if( pChip->nChannel & 0x01 ) this->sfMap.Draw( 188+12*2, y, &s_rcChips[15] );
				}
			}
		}
		break;
	}		   
	// �M�^�[�`�b�v�i�󂤂��w��j
	case 0xBA: {
		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarG < 0 ) )
			pChip->bHit = true;		// �������Ȃ�
		break;
	}
	// �x�[�X�`�b�v
	case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5: case 0xA6: case 0xA7: {

		// �������ĂȂ���Δ�������
		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarB < 0 ) )
		{
			if( ! bSkip )
				this->t�T�E���h�Đ�( pChip, PLAYPART_BASS, DTX_AUTOVOLUME );
			pChip->bHit = true;
		}

		// �`�b�v�̕`��
		if( ! bSkip && ! pChip->bHit && pChip->bVisible )
		{
			int y = 38 + pChip->nDotFromBarB-1;
			if( y >= 0 && y < 480 )
			{
				if( pChip->nChannel == 0xA0 )	// OPEN
					this->sfMap.Draw( 234, y, &s_rcChips[26] );
				else
				{								// RGB
					if( pChip->nChannel & 0x04 ) this->sfMap.Draw( 234,      y, &s_rcChips[17] );
					if( pChip->nChannel & 0x02 ) this->sfMap.Draw( 234+12,   y, &s_rcChips[18] );
					if( pChip->nChannel & 0x01 ) this->sfMap.Draw( 234+12*2, y, &s_rcChips[19] );
				}
			}
		}
		break;
	}
	// �x�[�X�`�b�v�i�󂤂��w��j
	case 0xBB: {
		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarB < 0 ) )
			pChip->bHit = true;		// �������Ȃ�
		break;
	}
	// �M�^�[Wiling
	case 0x28: {
		// �q�b�g���Ă��Ȃ���΃q�b�g��������
		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarG < 0 ) )
			pChip->bHit = true;

		// �`�b�v�`��
		if( ! bSkip && ! pChip->bHit && pChip->bVisible )
			this->sfMap.Draw( 224, 38+pChip->nDotFromBarG, &s_rcChips[16] );
		break;
	}
	// �x�[�XWiling
	case 0xA8: {
		// �q�b�g���Ă��Ȃ���΃q�b�g��������
		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarB < 0 ) )
			pChip->bHit = true;

		// �`�b�v�`��
		if( ! bSkip && ! pChip->bHit && pChip->bVisible )
			this->sfMap.Draw( 270, 38+pChip->nDotFromBarB, &s_rcChips[20] );
		break;
	}
	// BGM
	case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67: case 0x68: case 0x69: case 0x70:
	case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77: case 0x78: case 0x79: case 0x80:
	case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87: case 0x88: case 0x89: case 0x90:
	case 0x91: case 0x92: {

		// �`�b�v�`��i0x61�`0x65�܂ł����j
		if( ! bSkip && ! pChip->bHit && pChip->bVisible && pChip->nChannel >= 0x61 && pChip->nChannel <= 0x65 )
			this->sfMap.Draw( 290+(pChip->nChannel-0x61)*12, BAR_Y-pChip->nDotFromBarD-1, &s_rcChips[21+pChip->nChannel-0x61] );

		// �������ĂȂ���Δ�������
		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarD < 0 ) )
		{
			if( ! bSkip ) {
				g_DTX.StopWave( this->struct���t���.n�Ō�ɍĐ�����BGM�̎�WAV�ԍ�[ pChip->nChannel-0x61 ], 10 );
				g_DTX.PlayChip( pChip, 10, DTX_AUTOVOLUME );
				this->struct���t���.n�Ō�ɍĐ�����BGM�̎�WAV�ԍ�[ pChip->nChannel-0x61 ] = pChip->nRealParam;
			}
			pChip->bHit = true;
		}
		break;
	}
	// ���ߐ�
	case 0x50: {
			
		// part = ���ߔԍ�(�\������ԍ�+1)
		int part = (int)( pChip->dwPosition / 384 );

		// �q�b�g�������ĂȂ���΂���
		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarD < 0 ) )
		{
			pChip->bHit = true;

			// �\���p���ߔԍ��̍X�V
			this->struct���t���.n���݂̏��ߔԍ��E�f�o�b�O�\���p = part - 1;

			// Wave�Đ��ʒu�����␳
			if( ! bSkip )
				g_DTX.AdjustWaves();
		}
		// ���ߐ��\��
		if( ! bSkip && ! pChip->bHit && pChip->bVisible )
		{
			// BPM/BPx ���[�����ߐ��\��
			RECT rc;
			SetRect( &rc, s_rcLine.left, s_rcLine.top, s_rcLine.left + 23, s_rcLine.top + 1 );
			this->sfMap.Draw( 2, BAR_Y-pChip->nDotFromBarD, &rc );

			// �h�������[�����ߐ��\��
			SetRect( &rc, s_rcLine.left, s_rcLine.top, s_rcLine.left + 103, s_rcLine.top + 1 );
			this->sfMap.Draw( 33, BAR_Y-pChip->nDotFromBarD, &rc );

			// BGM/BGA/AVI ���[�����ߐ��\��
			SetRect( &rc, s_rcLine.left, s_rcLine.top, s_rcLine.left + 34, s_rcLine.top + 1 );
			this->sfMap.Draw( 146, BAR_Y-pChip->nDotFromBarD, &rc );

			// �M�^�[���[�����ߐ��\��
			SetRect( &rc, s_rcLine.left, s_rcLine.top, s_rcLine.left + 45, s_rcLine.top + 1 );
			this->sfMap.Draw( 187, 38+pChip->nDotFromBarG, &rc );

			// �x�[�X���[�����ߐ��\��
			SetRect( &rc, s_rcLine.left, s_rcLine.top, s_rcLine.left + 45, s_rcLine.top + 1 );
			this->sfMap.Draw( 233, 38+pChip->nDotFromBarB, &rc );

			// SE1-5 ���[�����ߐ��\��
			SetRect( &rc, s_rcLine.left, s_rcLine.top, s_rcLine.left + 61, s_rcLine.top + 1 );
			this->sfMap.Draw( 289, BAR_Y-pChip->nDotFromBarD, &rc );
		}
		// ���ߔԍ��\��
		if( ! bSkip && ! pChip->bHit ) 
		{
			this->txNumbers.SetScale( 0.5f, 0.5f );
			this->t�����`��( 125, BAR_Y-pChip->nDotFromBarD, part-1 );	// ���ߐ��̉�
			
			this->txNumbers.SetScale( 1.0f, 1.0f );
			this->t�����`��( 640, 480, this->struct���t���.n���݂̏��ߔԍ��E�f�o�b�O�\���p );	// ��ʉE���̂ł�����
		}
		break;
	}
	// ����
	case 0x51: {
		
		// �q�b�g�������ĂȂ���΂���
		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarD < 0 ) )
			pChip->bHit = true;

		// ���[�������\��
		if( ! bSkip && ! pChip->bHit && pChip->bVisible )
		{
			// BPM/BPx ���[�������\��
			RECT rc;
			SetRect( &rc, s_rcSubLine.left, s_rcSubLine.top, s_rcSubLine.left + 23, s_rcSubLine.top + 1 );
			this->sfMap.Draw( 2, BAR_Y-pChip->nDotFromBarD, &rc );

			// �h�������[�������\��
			SetRect( &rc, s_rcSubLine.left, s_rcSubLine.top, s_rcSubLine.left + 103, s_rcSubLine.top + 1 );
			this->sfMap.Draw( 33, BAR_Y-pChip->nDotFromBarD, &rc );

			// BGM/BGA/AVI ���[�������\��
			SetRect( &rc, s_rcSubLine.left, s_rcSubLine.top, s_rcSubLine.left + 34, s_rcSubLine.top + 1 );
			this->sfMap.Draw( 146, BAR_Y-pChip->nDotFromBarD, &rc );

			// SE1-5 ���[�������\��
			SetRect( &rc, s_rcSubLine.left, s_rcSubLine.top, s_rcSubLine.left + 61, s_rcSubLine.top + 1 );
			this->sfMap.Draw( 289, BAR_Y-pChip->nDotFromBarD, &rc );
		}
		break;
	}
	// MIDI �h�����R�[���X
	case 0x52: {
		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarD < 0 ) )
			pChip->bHit = true;		// �������Ȃ�
		break;
	}
	// �t�B���C��
	case 0x53: {
		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarD < 0 ) )
			pChip->bHit = true;		// �������Ȃ�
		break;
	}
	// AVI
	case 0x54: {
		
		if( ! pChip->bHit && ( bSkip || pChip->nDotFromBarD < 0 ) )
		{
			pChip->bHit = true;

			switch( pChip->AVItype )
			{
			case AVITYPE_AVI:
				if( pChip->pAVI )
				{
					// len = 0 �Ȃ�]�������̂ݗL��
					this->actAVI.Start( pChip->nChannel, pChip->pAVI,
						BGA_W, BGA_H, 0, 0,			// sw, sh, ew, eh		; �̈�   �����T�C�Y�A�I���T�C�Y
						0, 0, 0, 0,					// ssx, ssy, sex, sey	; �摜�� �����ʒu�A�ŏI�ʒu
						0, 0, 0, 0,					// dsx, dsy, dex, dey	; �\���� �����ʒu�A�ŏI�ʒu
						0, pChip->dbTime );			// len, start
				}
				break;

			case AVITYPE_AVIPAN:
				if( pChip->pAVIPan )
				{
					this->actAVI.Start( pChip->nChannel, pChip->pAVI,
						pChip->pAVIPan->sw, pChip->pAVIPan->sh, pChip->pAVIPan->ew, pChip->pAVIPan->eh,		// �̈�   �����T�C�Y�A�I���T�C�Y
						pChip->pAVIPan->ssx, pChip->pAVIPan->ssy, pChip->pAVIPan->sex, pChip->pAVIPan->sey,	// �摜�� �����ʒu�A�ŏI�ʒu
						pChip->pAVIPan->dsx, pChip->pAVIPan->dsy, pChip->pAVIPan->dex, pChip->pAVIPan->dey,	// �\���� �����ʒu�A�ŏI�ʒu
						pChip->dbLong, pChip->dbTime );													// len, start
				}
				break;
			}
		}
		break;
	}
	// ���m
	default: {
		if( pChip->bHit && ( bSkip || pChip->nDotFromBarD < 0 ) )
			pChip->bHit = true;
		break;
	}
	}
}
void CStagePlay::t�T�E���h�Đ�( DTX::Chip* pChip, EPlayPart part, long l���� )
{
	if( ! pChip )
		return;

	switch( part )
	{

	// �h�����F
	// HHC, HHO �ɂ��ẮA�O�ɔ������������~���Ă���Đ�����BLC �̏ꍇ�͖��֌W�B
	// �������A�O�̉��ƍ���̉��������Ƃ� HHO �̏ꍇ�͒�~���Ȃ��B(HHO���[���΍�j
	case PLAYPART_DRUMS:
		if( pChip->nChannel >= 0x11 && pChip->nChannel <= 0x1A )
		{
			int nLane = s_n�`�����l��0Ato���[��07[ pChip->nChannel - 0x11 ];

			if( nLane == 1 && ( pChip->nChannel == 0x11 || ( pChip->nChannel == 0x18 && this->struct���t���.n�Ō�ɍĐ�����HH�̃`�����l���ԍ� != 0x18 ) ) )
			{
				g_DTX.StopWave( this->struct���t���.n�Ō�ɍĐ�����HH��WAV�ԍ�, nLane );
				this->struct���t���.n�Ō�ɍĐ�����HH��WAV�ԍ�			= pChip->nRealParam;
				this->struct���t���.n�Ō�ɍĐ�����HH�̃`�����l���ԍ�	= pChip->nChannel;
			}
			g_DTX.PlayChip( pChip, nLane, l���� );
		}
		break;

	// �M�^�[�F
	// �O�̉����~���Ă��甭��
	case PLAYPART_GUITAR:
		g_DTX.StopWave( this->struct���t���.n�Ō�ɍĐ������M�^�[��WAV�ԍ�, 8 );
		g_DTX.PlayChip( pChip, 8, l���� );
		this->struct���t���.n�Ō�ɍĐ������M�^�[��WAV�ԍ� = pChip->nRealParam;
		break;

	// �x�[�X�F
	// �O�̉����~���Ă��甭��
	case PLAYPART_BASS:
		g_DTX.StopWave( this->struct���t���.n�Ō�ɍĐ������x�[�X��WAV�ԍ�, 9 );
		g_DTX.PlayChip( pChip, 9, l���� );
		this->struct���t���.n�Ō�ɍĐ������x�[�X��WAV�ԍ� = pChip->nRealParam;
		break;
	}
}
void CStagePlay::t�����`��( int rx, int by, int num )
{
	// �����͕\�����Ȃ�
	if( num < 0 ) return;

	// ���l�������Ƃɕ����i�U���܂ŁG����ȏ�͖����j
	int pn[ 6 ];
	for( int i = 0; i < 6; i++ )
	{
		int n = num % 10;
		num = (num - n) / 10;
		pn[ 5-i ] = n;
	}

	// �摜�̍��v�T�C�Y�̎Z�o
	int w = 0;
	int h = 0;
	float fw, fh;
	bool bf = true;		// �ŏ��̂O�͖���

	this->txNumbers.GetScale( &fw, &fh );
	
	for( int i = 0; i < 6; i++ )
	{
		if( pn[i] == 0 && bf && i < 5 ) continue;
		w += (int)( (s_rcNums[ pn[i] ].right - s_rcNums[ pn[i] ].left) * fw );
		h = (int)( MAX( h, (s_rcNums[ pn[i] ].bottom - s_rcNums[ pn[i] ].top) ) * (fh+0.2f) );
		bf = false;
	}

	// �`��
	int x = rx - w;
	int y = by - h;
	bf = true;
	for( int i = 0; i < 6; i++ )
	{
		if( pn[i] == 0 && bf && i < 5 ) continue;
		this->txNumbers.Draw( x, y, &s_rcNums[ pn[i] ] );
		x += (int)( (s_rcNums[ pn[i] ].right - s_rcNums[ pn[i] ].left) * fw );
		bf = false;
	}
}
void CStagePlay::t���t��������������()
{
	this->struct���t���.b���t�� = false;
	this->struct���t���.p���݂̃g�b�vChip = NULL;
	this->struct���t���.n���݂̏��ߔԍ��E�f�o�b�O�\���p = 0;
	this->struct���t���.db���݂�BPM�E�f�o�b�O�\���p = 0.0;
	this->struct���t���.db�J�n����ms = INIT_TIME;
	for( int i = 0; i < 50; i++ )
		this->struct���t���.n�Ō�ɍĐ�����BGM�̎�WAV�ԍ�[i] = -1;
	this->struct���t���.n�Ō�ɍĐ�����HH��WAV�ԍ�			= -1;
	this->struct���t���.n�Ō�ɍĐ�����HH�̃`�����l���ԍ�	= 0;
	this->struct���t���.n�Ō�ɍĐ������M�^�[��WAV�ԍ�		= -1;
	this->struct���t���.n�Ō�ɍĐ������x�[�X��WAV�ԍ�		= -1;
}
//
bool CStagePlay::MyOneTimeSceneInit()
{
	// �����ɃR�[�h���L�q����BAddChild() �ȂǁB
	AddChild( &this->sfMap );
	AddChild( &this->txNumbers );
	AddChild( &g_DTX );
	AddChild( &this->actAVI );

	g_DTX.bDTXV = true;
	g_DTX.SetTimer( &g_Timer );

	return CStage::MyOneTimeSceneInit();
}
bool CStagePlay::MyFinalCleanup()
{
	// �����ɃR�[�h���L�q����B

	return CStage::MyFinalCleanup();
}
bool CStagePlay::MyInitDeviceObjects()
{
	// �����ɃR�[�h���L�q����BCTexture/CSurface::InitDeviceObjects() �ȂǁB
	this->sfMap.InitDeviceObjectsFromResource( _T("�}�b�v"), MAKEINTRESOURCE(IDR_PNG_MAP), _T("PNG") );
	this->sfMap.ColorKeyEnable();
	this->txNumbers.InitDeviceObjectsFromResource( _T("����"), MAKEINTRESOURCE(IDR_PNG_NUMBERS), _T("PNG") );

	return CStage::MyInitDeviceObjects();
}
bool CStagePlay::MyDeleteDeviceObjects()
{
	// �����ɃR�[�h���L�q����B

	// ���� CStage::MyDeleteDeviceObjects() �ł́A�q�e�N�X�`���E�q�T�[�t�F�C�X�� Delete (CTexture/CSurface::Delete) ���s����B
	return CStage::MyDeleteDeviceObjects();
}
bool CStagePlay::MyActivate()
{
	this->bFirstWork = true;
	// �����ɃR�[�h���L�q����B
	this->t���t��������������();

	this->phaseID = PHASE_PLAY_IDLE;
	this->bLoading�v������ = false;
	this->bPlaying�v������ = false;
	this->n�J�n���ߔԍ� = 0;

	// ���� CStage::MyActivate() �ł́Athis->bActivity �̃Z�b�g�Ȃ�тɎq�T�[�t�F�C�X�E�q�e�N�X�`���� Restore (MyRestoreDeviceTexture/Surface) ���s����B
	// �Ȃ��A�q Activity �̊������͍s���Ȃ��B�i���̃��\�b�h�𔲂��Ă���s����B�j
	return CStage::MyActivate();
}
bool CStagePlay::MyUnActivate()
{
	// �����ɃR�[�h���L�q����B

	// ���� CStage::MyUnActivate() �ł́Athis->bActivity �̃��Z�b�g�Ȃ�тɎq�T�[�t�F�C�X�E�q�e�N�X�`���� Invalidate (MyInvalidateDeviceObjects) ���s����B
	// �Ȃ��A�qActivity �̔񊈐����͍s���Ȃ��B�i���̃��\�b�h�𔲂��Ă���s����B�j
	return CStage::MyUnActivate();
}
bool CStagePlay::MyRestoreDeviceTextures()
{
	// �����ɃR�[�h���L�q����B

	// ���� CStage::MyRestoreDeviceTextures() �ł́A�q�e�N�X�`���� Restore (CTexture::Restore) ���s����B
	return CStage::MyRestoreDeviceTextures();
}
bool CStagePlay::MyRestoreDeviceSurfaces()
{
	// �����ɃR�[�h���L�q����B

	// ���� CStage::MyRestoreDeviceSurfaces() �ł́A�q�T�[�t�F�C�X�� Restore (CSurface::Restore) ���s����B
	return CStage::MyRestoreDeviceSurfaces();
}
bool CStagePlay::MyInvalidateDeviceObjects()
{
	// �����ɃR�[�h���L�q����B

	// ���� CStage::MyInvalidateDeviceObjects() �ł́A�q�e�N�X�`���E�q�T�[�t�F�C�X�� Invalidate (CTexture/CSurface::Invalidate) ���s����B
	return CStage::MyRestoreDeviceSurfaces();
}
		}//Play
	}//Stage
}//DTXViewer
