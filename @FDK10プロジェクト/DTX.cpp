#include "stdafx.h"
#include "Debug.h"
#include "DTX.h"
#include "CFileMM.h"
#include "MD5.h"
#include "str2float.h"
#include "CAvi.h"

namespace FDK {
	namespace General {

#ifndef SKIPSP
#define SKIPSP(p)	{while(*p==' '||*p=='\t')p++;}
#endif
#ifndef SKIPSP_BGA
#define SKIPSP_BGA(p)	{while(*p==' '||*p=='\t'||*p==','||*p=='('||*p==')'||*p=='['||*p==']'||*p=='x'||*p=='|')p++;}
#endif
#ifndef SKIPDEC
#define SKIPDEC(p)	{while((*p>='0'&&*p<='9')||*p=='-'||*p=='+')p++;}
#endif
#ifndef SKIPHEX
#define SKIPHEX(p)	{while((*p>='0'&&*p<='9')||(*p>='a'&&*p<='f')||(*p>='A'&&*p<='F')||*p=='-'||*p=='+')p++;}
#endif
#ifndef SKIPZEX
#define SKIPZEX(p)	{while((*p>='0'&&*p<='9')||(*p>='a'&&*p<='z')||(*p>='A'&&*p<='Z')||*p=='-'||*p=='+')p++;}
#endif

static const int s_nChannelToLane[10] = { 1,2,3,4,5,7,6,1,7,0 };		// Ch�ɑΉ����郌�[���ԍ�

DTX::DTX()
{
	// Clear() ���O�ɏ��������K�v�Ȃ��̂��������ŏ���������
	this->pBPM		= this->pBPMLast	= NULL;
	this->pWave		= this->pWaveLast	= NULL;
	this->pBMP		= this->pBMPLast	= NULL;
	this->pBMPTEX	= this->pBMPTEXLast	= NULL;
	this->pBGA		= this->pBGALast	= NULL;
	this->pBGAPan	= this->pBGAPanLast	= NULL;
	this->pAVI		= this->pAVILast	= NULL;
	this->pAVIPan	= this->pAVIPanLast	= NULL;
	this->pChip		= this->pChipLast	= NULL;

	this->pTimer					= NULL;
	this->bDTXV						= false;
	this->bWAVCache					= true;
	this->bBMPCache					= true;
}

DTX::~DTX()
{
	Clear();
}

// ������
void DTX::Clear()
{
	// ������Ԃ̏����� 
	this->strFullPath.clear();
	this->strFolder.clear();
	this->strFileExt[0]			= _T('\0');
	this->scoreType				= SCORETYPE_UNKNOWN;
	this->strTitle.clear();
	this->strArtist.clear();
	this->strComment.clear();
	this->strPanel.clear();
	this->strGenre.clear();
	this->bHiddenLevel = false;
	for( int i = 0; i < 3; i++ )
		this->nLevel[i]			= 0;
	this->dbBPM					= 0.0;
	this->dbBaseBPM				= 0.0;
	this->strStage.clear();
	this->strPresound.clear();
	this->strPreimage.clear();
	this->strPremovie.clear();
	this->strBackground.clear();
	this->strBackgroundGR.clear();
	for( int i = 0; i < 7; i++ ) {
		this->strResultImage[i].clear();
		this->strResultMovie[i].clear();
	}
	this->strMIDI.clear();
	this->bMIDINote				= false;
	this->nMIDILevel			= 0;
	this->bBlackColorKey		= true;
	this->strPathWav.clear();
	this->fSpeed				= 1.0f;
	this->bHeaderOnly			= false;

	for( int i = 0; i < 10; i++)
		this->nTotalChipsD[ i ] = 0;
	this->nTotalChipsG			= 0;
	this->nTotalChipsB			= 0;
	this->bUseDrum				= false;
	this->bUseGuitar			= false;
	this->bUseBass				= false;
	this->bUseHHOpen			= false;
	this->bUseRide				= false;
	this->bUseLeftCymal			= false;
	this->strHash[0]			= _T('\0');
	// BPM���X�g�̉���Ə����� 
	{
		BPM* b = this->pBPM;
		while( b != NULL )
			{
			BPM* n = b->next;
			SAFE_DELETE( b );
			b = n;
		}
		this->pBPM = this->pBPMLast = NULL;
	}
	// WAV���X�g�̉���Ə����� 
	{
		Wave* w = this->pWave;
		while( w != NULL )
		{
			// Wave::sound[][] �̉���̓T�E���h�}�l�[�W���ōs�����߁A�����ł͉�����Ȃ��B
			Wave* n = w->next;
			SAFE_DELETE( w );
			w = n;
		}
		this->pWave = this->pWaveLast = NULL;
	}
	// BMP���X�g�̉���Ə����� 
	{
		BMP* bm = this->pBMP;
		while( bm != NULL )
		{
			// BMP::pSurface �̉���̓T�[�t�F�C�X�}�l�[�W���ōs�����߁A�����ł͉������Ȃ��B
			BMP* n = bm->next;
			SAFE_DELETE( bm );
			bm = n;
		}
		this->pBMP = this->pBMPLast = NULL;
		this->bUseBMP				= false;
		this->dwBMPWidthMax			= 0;
		this->dwBMPHeightMax		= 0;
	}
	// BMPTEX���X�g�̉���Ə����� 
	{
		BMPTEX* bmt = this->pBMPTEX;
		while( bmt != NULL )
		{
			// BMPTEX::pTexture �̉���̓e�N�X�`���}�l�[�W���ōs�����߁A�����ł͉������Ȃ��B
			BMPTEX* n = bmt->next;
			SAFE_DELETE( bmt );
			bmt = n;
		}
		this->pBMPTEX = this->pBMPTEXLast = NULL;
		this->bUseBMPTEX			= false;
		this->dwBMPTEXWidthMax		= 0;
		this->dwBMPTEXHeightMax		= 0;
	}
	// BGA���X�g�̉���Ə����� 
	{
		BGA* bg = this->pBGA;
		while( bg != NULL )
		{
			BGA* n = bg->next;
			SAFE_DELETE( bg );
			bg = n;
		}
		this->pBGA = this->pBGALast = NULL;
	}
	// BGAPAN���X�g�̉���Ə����� 
	{
		BGAPan* bp = this->pBGAPan;
		while( bp != NULL )
		{
			BGAPan* n = bp->next;
			SAFE_DELETE( bp );
			bp = n;
		}
		this->pBGAPan = this->pBGAPanLast = NULL;
	}
	// AVI���X�g�̉���Ə����� 
	{
		this->tAVI���X�g�̉���Ə�����();
	}
	// AVIPAN���X�g�̉���Ə����� 
	{
		this->tAVIPAN���X�g�̉���Ə�����();
	}
	// CHIP���X�g�̉���Ə����� 
	{
		this->tCHIP���X�g�̉���Ə�����();
	}
	// Cache OFF �̏ꍇ�́A���f�[�^�������ł��ׂĉ������B 
	if( ! this->bWAVCache ) {
		this->soundManager.t�g�p�t���O���N���A�������Z�����폜����();
		this->soundManager.t���g�p�T�E���h���폜����();
	}
	if( ! this->bBMPCache ) {
		this->textureManager.ResetCache();
		this->textureManager.RemoveUnusedTextures();
		this->surfaceManager.ResetCache();
		this->surfaceManager.RemoveUnusedSurfaces();
	}
}
void DTX::tAVI���X�g�̉���Ə�����()
{
	AVI* a = this->pAVI;
	while( a != NULL )
	{
		if( a->bUse )
			a->avi.Close();
		AVI* n = a->next;
		SAFE_DELETE( a );
		a = n;
	}
	this->pAVI = this->pAVILast = NULL;
	this->bUseAVI	= false;
}
void DTX::tAVIPAN���X�g�̉���Ə�����()
{
	AVIPan* ap = this->pAVIPan;
	while( ap != NULL )
	{
		AVIPan* n = ap->next;
		SAFE_DELETE( ap );
		ap = n;
	}
	this->pAVIPan = this->pAVIPanLast = NULL;
}
void DTX::tCHIP���X�g�̉���Ə�����()
{
	Chip* c = this->pChip;
	while( c != NULL )
	{
		Chip* n = c->next;
		SAFE_DELETE( c );
		c = n;
	}
	this->pChip = this->pChipLast = NULL;
}
// Activity
bool DTX::MyOneTimeSceneInit()
{
	// �T�E���h�}�l�[�W���̏�����
	this->soundManager.Init( CD3DApplication::hWnd, DSSCL_PRIORITY );

	// �e�N�X�`���}�l�[�W���̏�����
	this->textureManager.OneTimeSceneInit();

	// �T�[�t�F�C�X�}�l�[�W���̏�����
	this->surfaceManager.OneTimeSceneInit();


	return Activity::MyOneTimeSceneInit();
}

bool DTX::MyActivate()
{
	// ���Ɋ��������Ă��牽�����Ȃ�
	if( this->bActivate )
		return true;

	// �������\�[�X�̉��
	this->Clear();

	return Activity::MyActivate();
}

bool DTX::MyUnActivate()
{
	// ���������ĂȂ��Ȃ牽�����Ȃ�
	if( ! this->bActivate ) 
		return true;

	this->tAVI���X�g�̉���Ə�����();
	this->tAVIPAN���X�g�̉���Ə�����();
	this->tCHIP���X�g�̉���Ə�����();

	return Activity::MyUnActivate();
}

bool DTX::MyFinalCleanup()
{
	this->soundManager.Term();				// �T�E���h�}�l�[�W���̏I��
	this->textureManager.FinalCleanup();	// �e�N�X�`���}�l�[�W���̏I��
	this->surfaceManager.FinalCleanup();	// �T�[�t�F�C�X�}�l�[�W���̏I��

	return Activity::MyFinalCleanup();
}

bool DTX::MyInitDeviceObjects()
{
	this->textureManager.InitDeviceObjects();	// �e�N�X�`���}�l�[�W��
	this->surfaceManager.InitDeviceObjects();	// �T�[�t�F�C�X�}�l�[�W��

	return Activity::MyInitDeviceObjects();
}

bool DTX::MyRestoreDeviceTextures()
{
	// ���������ĂȂ��Ȃ疳��
	if( ! this->bActivate ) 
		return true;

	// �e�N�X�`���}�l�[�W���̍\�z
	this->textureManager.RestoreDeviceObjects();

	return Activity::MyRestoreDeviceTextures();
}

bool DTX::MyRestoreDeviceSurfaces()
{
	// ���������ĂȂ��Ȃ疳��
	if( ! this->bActivate ) 
		return true;

	// �T�[�t�F�C�X�}�l�[�W���̍\�z
	this->surfaceManager.RestoreDeviceObjects();

	// �����߂��L���Ȃ�A�SBMP�̃J���[�L�[�� 0xFF000000 �ɐݒ肷��B
	if( this->bBlackColorKey )
	{
		for( BMP* bmp = this->pBMP; bmp != NULL; bmp = bmp->next )
		{
			if( bmp->pSurface )
				bmp->pSurface->SetColorKey( 0xff000000 );
		}
	}

	return Activity::MyRestoreDeviceSurfaces();
}

bool DTX::MyInvalidateDeviceObjects()
{
	// ���������ĂȂ��Ȃ疳��
	if( ! this->bActivate ) 
		return true;

	this->textureManager.InvalidateDeviceObjects();		// �e�N�X�`���}�l�[�W���̉��
	this->surfaceManager.InvalidateDeviceObjects();		// �T�[�t�F�C�X�}�l�[�W���̉��

	return Activity::MyInvalidateDeviceObjects();
}

bool DTX::MyDeleteDeviceObjects()
{
	this->textureManager.DeleteDeviceObjects();		// �e�N�X�`���}�l�[�W���̔j��
	this->surfaceManager.DeleteDeviceObjects();		// �T�[�t�F�C�X�}�l�[�W���̔j��

	return Activity::MyDeleteDeviceObjects();
}

// ���t�t�@�C���̓ǂݍ���
bool DTX::Load( LPCTSTR fname, bool bHeaderOnly, double dbPlaySpeed )
{
	// (0) �O���� 
	{
		this->bHeaderOnly = bHeaderOnly;

		if( this->nMIDILevel > 2 )
			this->nMIDILevel = 2;

		// �t�@�C�����J���Ȃ���΂����ŏI�� 
		FILE *fp;
		if( _tfopen_s( &fp, fname, _T("rt") ) != 0 )
			return false;
		fclose( fp );
	}
	// (1) �t�@�C�����A�g���q�A�t�H���_����ۑ� �� strFullPath, strFolder, strFileExt 
	{
		TCHAR strDrive[ _MAX_DRIVE ];
		TCHAR strDir[_MAX_DIR ];
		if( _tsplitpath_s( fname, strDrive, _MAX_DRIVE, strDir, _MAX_DIR, NULL, 0, NULL, 0 ) != 0 )
			return false;

		// �t�@�C����
		this->strFullPath = fname;

		// �t�H���_��
		this->strFolder  = strDrive;
		this->strFolder += strDir;

		// �g���q
		CFileMM::GetExt( this->strFullPath.c_str(), this->strFileExt );
	}
	// (2) �g���q����f�[�^�^�C�v������ �� scoreType 
	{
		static struct _dataType {
			LPTSTR			ext;
			ScoreType		type;
			LPTSTR			typeName;
		} s_dataType[] = {
			{ _T("dtx"), SCORETYPE_DTX, _T("DTX") },
			{ _T("bms"), SCORETYPE_BMS, _T("BMS") },
			{ _T("bme"), SCORETYPE_BMS, _T("BMS(BME)") },
			{ _T("gda"), SCORETYPE_GDA, _T("GDA") },
			{ _T("g2d"), SCORETYPE_G2D, _T("G2D") },
			{ _T("mid"), SCORETYPE_MID, _T("MIDI") },
			{ _T("tmp"), SCORETYPE_DTX, _T("DTX") },		// for GDAC2
			{ _T(""), SCORETYPE_UNKNOWN, _T("UNKNOWN") }
		};
		for( int i = 0; i < 99; i++)
		{
			if( s_dataType[i].type == SCORETYPE_UNKNOWN )
			{
				this->scoreType = SCORETYPE_UNKNOWN;		// ���m
				break;
			}
			else if( lstrcmpi( this->strFileExt, s_dataType[i].ext ) == 0 )
			{
				this->scoreType = s_dataType[i].type;
				break;
			}
		}

		// DTXV �ł́ADTX �ȊO�͓ǂݍ��܂Ȃ�(2004.01.12)
		if( this->bDTXV && this->scoreType != SCORETYPE_DTX )
			return false;
	}
	// (3) �t�@�C����ǂݍ��� 
	switch( this->scoreType )
	{
	case SCORETYPE_DTX:
	case SCORETYPE_BMS:
	case SCORETYPE_G2D:
	case SCORETYPE_GDA:
		this->LoadDTX();
		break;

	case SCORETYPE_MID:
		this->LoadMID();
		break;
	}
	// (4) �ǂݍ��݌�̏��� 
	if( ! this->bHeaderOnly )
	{
		// (4-1) ���ߐ��Ɣ�����}������iMIDI�ȊO�j
		if( this->scoreType != SCORETYPE_MID )
			this->InsertLines();

		// (4-2) �`�b�v�̔��������𐶐�����iMIDI�ȊO�j
		if( this->scoreType != SCORETYPE_MID )
			CalcChipTime();

		// (4-3) �e�`�b�v�̔��������� dbPlaySpeed �ɂ��킹�Ē�������iDTX, MID ���ʁj
		AdujstChipTimeByPlaySpeed( dbPlaySpeed );

		// (4-4) ���`�b�v���𐔂��� �� nTotalChipsD/G/B 
		{
			for( int i = 0; i < 10; i++ )
				this->nTotalChipsD[ i ] = 0;
			this->nTotalChipsG = 0;
			this->nTotalChipsB = 0;
			
			for( Chip* cell = this->pChip; cell != NULL; cell = cell->next )
			{
				if( cell->nChannel >= 0x11 && cell->nChannel <= 0x1A )
					this->nTotalChipsD[ cell->nChannel - 0x11 ] ++;
				else if( cell->nChannel >= 0x20 && cell->nChannel <= 0x27 )
					this->nTotalChipsG ++;
				else if( cell->nChannel >= 0xA0 && cell->nChannel <= 0xA7 ) 
					this->nTotalChipsB ++;
			}
		}
		// (4-5) ���[�����ƂɎg�p����WAV��o�^����iLoadWAV()�̑O�ɍs�����ƁI�j
		if( !this->bHeaderOnly )
		{
			// (4-5-1) �r�b�g�}�X�N���� 
			WORD wWaveToLane[ 36*36 ];	// 00..ZZ = 0�`36*36-1
			for( int i = 0; i < 36*36; i++ )
				wWaveToLane[ i ] = 0;

			// (4-5-2) �S�`�b�v�𑖍����A�g�p���郌�[���̊Y���r�b�g�𗧂ĂĂ䂭�B 
			for( Chip *cp = this->pChip; cp != NULL; cp = cp->next )
			{
				switch( cp->nChannel )
				{
				case 0x11: case 0x31: case 0xB1: wWaveToLane[ cp->nParam ] |= 0x002; break;		// HH
				case 0x12: case 0x32: case 0xB2: wWaveToLane[ cp->nParam ] |= 0x004; break;		// SD
				case 0x13: case 0x33: case 0xB3: wWaveToLane[ cp->nParam ] |= 0x008; break;		// BD
				case 0x14: case 0x34: case 0xB4: wWaveToLane[ cp->nParam ] |= 0x010; break;		// HT
				case 0x15: case 0x35: case 0xB5: wWaveToLane[ cp->nParam ] |= 0x020; break;		// LT
				case 0x16: case 0x36: case 0xB6: wWaveToLane[ cp->nParam ] |= 0x080; break;		// CY
				case 0x17: case 0x37: case 0xB7: wWaveToLane[ cp->nParam ] |= 0x040; break;		// FT
				case 0x18: case 0x38: case 0xB8: wWaveToLane[ cp->nParam ] |= 0x002; break;		// HH
				case 0x19: case 0x39: case 0xB9: wWaveToLane[ cp->nParam ] |= 0x080; break;		// RD
				case 0x1A: case 0x3A: case 0xBC: wWaveToLane[ cp->nParam ] |= 0x001; break;		// LC
				case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27: case 0xBA:
					wWaveToLane[ cp->nParam ] |= 0x100; break;																	// Guitar
				case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5: case 0xA6: case 0xA7: case 0xBB:
					wWaveToLane[ cp->nParam ] |= 0x200; break;																	// Bass
				case 0x01: case 0x1F: case 0x2F: case 0xAF:
				case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67: case 0x68: case 0x69: case 0x70:
				case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77: case 0x78: case 0x79: case 0x80:
				case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87: case 0x88: case 0x89: case 0x90:
				case 0x91: case 0x92:
					wWaveToLane[ cp->nParam ] |= 0x400; break;																	// BGM
				}
			}
			// (4-5-3) �SWav�ɂ��āA�g�p���郌�[������ true �ɂ���B 
			for( Wave *wp = this->pWave; wp != NULL; wp = wp->next ) {
				for( int i = 0; i < 11; i++ )
					wp->bUseByLane[ i ] = ( wWaveToLane[ wp->vnum ] & (0x001 << i) ) ? true : false;
			}
		}
		// (4-6) WAV �̓o�^���\�z 
		this->LoadWAV();
	}
	// (4-7) �ȃf�[�^�n�b�V���̌v�Z 
	this->CalcHash();

	return true;
}
void DTX::LoadDTX()
{
	FILE *fp;
	if( _tfopen_s( &fp, this->strFullPath.c_str(), _T("rt") ) != 0 )
		return;		// ���s

	// (1) ������ 
	{
		// (1-1) �ǂݍ��ݍ�Ɨp�ϐ��̏����� 
		for( int i = 0; i < 36*36; i++ )
		{
			m_nWaveNum[i]	= -i;
			m_nWaveVol[i]	= -i;
			m_nWavePan[i]	= -10000-i;		// pan �� -100�`100�Ȃ̂ŁA�o�b�e�B���O���Ȃ��悤�l������炷�B
			m_nBPMNum[i]	= -i;
			m_nBMPNum[i]	= -i;
			m_nBMPTEXNum[i]	= -i;
			m_nBGANum[i]	= -i;
			m_nBGAPanNum[i]	= -i;
		}
		m_nWaveNumCur	= 0;
		m_nBPMNumCur	= 0;
		m_nBMPNumCur	= 0;

		// (1-2) #RESULTIMAGE/MOVIE/SOUND �p �D�揇�ʃo�b�t�@�̏����� 
		for( int i = 0; i < 7; i++ )
		{
			m_nResultImagePriority[i] = 0;
			m_nResultMoviePriority[i] = 0;
			m_nResultSoundPriority[i] = 0;
		}
	}
	// (2) �s���s����܂œǂݍ��݁��������� 
	m_nLine = 0;
	m_nRand   = 0;
	m_nSkip   = 0;
	m_bSkip[ m_nSkip ] = false;

	TCHAR* strLine = (TCHAR*) malloc( DTX_LINELEN );		// ���ǂݍ��ݍs�� malloc �Ŋm��
	
	while( _fgetts( strLine, DTX_LINELEN, fp ) != NULL )
	{
		TCHAR* p;
		int zz;

		m_nLine ++;

		// ���s�R�[�h�ƃR�����g���폜���� 
		{
			TCHAR* pLineTop = strLine;
			for( TCHAR* p = strLine; *p != _T('\0'); p++ )
			{
				// a. ';' �ȍ~�̓R�����g�Ƃ��Ė��� 
				if( *p == _T(';') )
				{
					*p = _T('\0');
			
					// ';' �̒��O�̋󔒂��폜
					if( p != pLineTop )
						p --;
					while( p != pLineTop && ( *p == _T(' ') || *p == _T('\t') ) )
					{
						*p = _T('\0');
						p --;
					}
					break;
				}

				// b. ���s�R�[�h�͍폜
				else if( *p == _T('\n') )
				{
					*p = _T('\0');
					break;
				}
			}
		}

		// �s���� # �łȂ��s�̓X�L�b�v���� 
		p = strLine;
		SKIPSP( p );
		if( *p != _T('#') )
			continue;
		p++;


		// #IF 
		if( IsCommand( &p, _T("IF") ) )
		{
			if( m_nSkip == 255 )
				continue;	// #IF �̓���q���� 255 �𒴂��� �� ���� #IF �͖���

			if( m_bSkip[ m_nSkip ] )
				m_bSkip[ ++m_nSkip ] = true;			// �X�L�b�v���Ɍ��ꂽIF�̓X�L�b�v
			else
				m_bSkip[ ++m_nSkip ] = ( _tstoi( p ) == m_nRand ) ? false : true;
			continue;
		}

		// #ENDIF 
		if( IsCommand( &p, _T("ENDIF") ) )
		{
			if( m_nSkip > 0 )	// �Ή����� #IF ������
				m_nSkip--;
			continue;
		}

		
		// ����ȍ~�́A�X�L�b�v���Ȃ疳������B
		if( m_bSkip[ m_nSkip ] ) continue;
		
		// #PATH_WAV 
		if( IsCommand( &p, _T("PATH_WAV") ) )
		{
			this->strPathWav = p;
			continue;
		}
		// #TITLE 
		if( IsCommand( &p, _T("TITLE") ) )
		{
			this->strTitle = p;
			continue;
		}
		// #ARTIST 
		if( IsCommand( &p, _T("ARTIST") ) )
		{
			this->strArtist = p;
			continue;
		}
		// #COMMENT 
		if( IsCommand( &p, _T("COMMENT") ) )
		{
			this->strComment = p;
			continue;
		}
		// #DLEVEL, #PLAYLEVEL 
		if( IsCommand( &p, _T("DLEVEL") ) || IsCommand( &p, _T("PLAYLEVEL") ) )
		{
			this->nLevel[ 0 ] = _ttoi( p );

			// ��� 100 �i�K���Ƃ݂Ȃ��B�i10�i�K�f�[�^�̃T�|�[�g�͏I���I(07.05.27)�j
			if( this->nLevel[ 0 ] <   0 ) this->nLevel[ 0 ] =   0;
			if( this->nLevel[ 0 ] > 100 ) this->nLevel[ 0 ] = 100;
			continue;
		}
		// #GLEVEL 
		if( IsCommand( &p, _T("GLEVEL") ) )
		{
			this->nLevel[ 1 ] = _ttoi( p );

			// ��� 100 �i�K���Ƃ݂Ȃ��B�i10�i�K�f�[�^�̃T�|�[�g�͏I���I(07.05.27)�j
			if( this->nLevel[ 1 ] <   0 ) this->nLevel[ 1 ] =   0;
			if( this->nLevel[ 1 ] > 100 ) this->nLevel[ 1 ] = 100;
			continue;
		}
		// #BLEVEL 
		if( IsCommand( &p, _T("BLEVEL") ) )
		{
			this->nLevel[ 2 ] = _ttoi( p );

			// ��� 100 �i�K���Ƃ݂Ȃ��B�i10�i�K�f�[�^�̃T�|�[�g�͏I���I(07.05.27)�j
			if( this->nLevel[ 2 ] <   0 ) this->nLevel[ 2 ] =   0;
			if( this->nLevel[ 2 ] > 100 ) this->nLevel[ 2 ] = 100;
			continue;
		}
		// #GENRE 
		if( IsCommand( &p, _T("GENRE") ) )
		{
			this->strGenre = p;
			continue;
		}
		// #HIDDENLEVEL 
		if( IsCommand( &p, _T("HIDDENLEVEL") ) )
		{
			this->bHiddenLevel = ((*p==_T('o') || *p==_T('O')) && (*(p+1)==_T('n') || *(p+1)==_T('N'))) ? true : false;
			continue;
		}
		// #STAGEFILE 
		if( IsCommand( &p, _T("STAGEFILE") ) )
		{
			this->strStage = p;
			continue;
		}
		// #PREVIEW 
		if( IsCommand( &p, _T("PREVIEW") ) )
		{
			this->strPresound = p;
			continue;
		}
		// #PREIMAGE 
		if( IsCommand( &p, _T("PREIMAGE") ) )
		{
			this->strPreimage = p;
			continue;
		}
		// #PREMOVIE 
		if( IsCommand( &p, _T("PREMOVIE") ) )
		{
			this->strPremovie = p;
			continue;
		}
		// #BACKGROUND_GR 
		if( IsCommand( &p, _T("BACKGROUND_GR") ) )
		{
			this->strBackgroundGR = p;
			continue;
		}
		// #BACKGROUND, #WALL 
		if( IsCommand( &p, _T("BACKGROUND") ) || IsCommand( &p, _T("WALL") ) )
		{
			this->strBackground = p;
			continue;
		}
		// #RANDOM 
		if( IsCommand( &p, _T("RANDOM") ) )
		{
			m_nRand = (rand() % _ttoi(p)) + 1;
			continue;
		}
		// #BPM 
		if( (zz = GetCommand( &p, _T("BPM") ) ) != 0 )
		{
			// #BPM: �� #BPM00: �ƌ��Ȃ��B
			if( zz < 0 ) zz = 0;

			// BPM�l���擾�B
			double bpm = str2float( p );
			if( bpm <= 0.0 )
				continue;	// #BPM �� 0 �ȉ��̒l�܂��͕s���ȕ�������w�肵�� �� ���� #BPM �𖳌�

			// #BPM: �Ȃ��\ BPM �l�Ƃ��ĕۑ�
			if( zz == 0 )
				this->dbBPM = bpm;
			
			// BPM �Z���ǉ�
			BPM *bc = new BPM();
			bc->num		= ++ m_nBPMNumCur;
			bc->vnum	= zz;
			bc->bpm		= bpm;
			bc->prev = bc->next = NULL;
			APPENDLIST( this->pBPM, this->pBPMLast, bc );

			// ������`�Ή��G
			// #BPMzz ������`�A���A����#BPM�����O�̍s�ɃI�u�W�F�N�g�L�q������Azz ���g����BPM�`�����l��������Ȃ�X�V����B
			if( m_nBPMNum[ zz ] == -zz )
			{
				for( Chip* cp = this->pChip; cp != NULL; cp=cp->next )
					if( ( GetChipParamType( cp->nChannel ) & DTXCPT_BPM ) && cp->nRealParam == -zz )
						cp->nRealParam = m_nBPMNumCur;	// BPM���ԍ�
			}
			
			// �Ō�� BPM���ԍ����X�V�B
			m_nBPMNum[ zz ] = m_nBPMNumCur;

			continue;
		}

		// bHeaderOnly = true �̎��́A�������牺�͖����B
		if( this->bHeaderOnly )	continue;

		// #MIDIFILE 
		if( IsCommand( &p, _T("MIDIFILE") ) )
		{
			this->strMIDI = p;
			continue;
		}
		// #PANEL
		TCHAR *q = p;
		if( IsCommand( &p, _T("PANEL") ) )
		{
			int isNotNum = 0;								// #26010 2011.12.23 yyagi: #PAN EL (WAV�ԍ�=EL��#PAN)��#PANEL�ƌ�����Ȃ��悤�A
			for( int i = 0; i < strlen(p); i++) {			// ���������l���ǂ����𔻒f����
				if( !_istdigit( p[i] ) && p[i] != '-' && p[i] != '+' )
				{
					isNotNum = 1;
					break;
				}
			}
			if( isNotNum )			// �������܂܂�Ă����Ȃ�#PANEL, ���l�݂̂Ȃ�#PAN
			{
				this->strPanel = p;
				continue;
			}
			p = q;
		}
		// #DTXVPLAYSPEED 
		if( IsCommand( &p, _T("DTXVPLAYSPEED") ) && this->bDTXV )
		{
			this->fSpeed = str2float( p );
			continue;
		}
		// #MIDINOTE 
		if( IsCommand( &p, _T("MIDINOTE") ) )
		{
			this->bMIDINote = ((*p==_T('o') || *p==_T('O')) && (*(p+1)==_T('n') || *(p+1)==_T('N'))) ? true : false;
			continue;
		}
		// #BLACKCOLORKEY 
		if( IsCommand( &p, _T("BLACKCOLORKEY") ) )
		{
			this->bBlackColorKey = ((*p==_T('o') || *p==_T('O')) && (*(p+1)==_T('n') || *(p+1)==_T('N'))) ? true : false;
			continue;
		}
		// #RESULTIMAGE_SS 
		if( IsCommand( &p, _T("RESULTIMAGE_SS") ) )	{ SetResultImage( 0, p, m_nResultImagePriority ); continue; }	// SS �ȏ�

		// #RESULTIMAGE_S 
		if( IsCommand( &p, _T("RESULTIMAGE_S") ) )	{ SetResultImage( 1, p, m_nResultImagePriority ); continue; }	//  S �ȏ�

		// #RESULTIMAGE_A 
		if( IsCommand( &p, _T("RESULTIMAGE_A") ) )	{ SetResultImage( 2, p, m_nResultImagePriority ); continue; }	//  A �ȏ�

		// #RESULTIMAGE_B 
		if( IsCommand( &p, _T("RESULTIMAGE_B") ) )	{ SetResultImage( 3, p, m_nResultImagePriority ); continue;	}	//  B �ȏ�

		// #RESULTIMAGE_C 
		if( IsCommand( &p, _T("RESULTIMAGE_C") ) )	{ SetResultImage( 4, p, m_nResultImagePriority ); continue; }	//  C �ȏ�

		// #RESULTIMAGE_D 
		if( IsCommand( &p, _T("RESULTIMAGE_D") ) )	{ SetResultImage( 5, p, m_nResultImagePriority ); continue;	}	//  D �ȏ�

		// #RESULTIMAGE_E 
		if( IsCommand( &p, _T("RESULTIMAGE_E") ) )	{ SetResultImage( 6, p, m_nResultImagePriority ); continue; }	//  E �ȏ�

		// #RESULTIMAGE 
		if( IsCommand( &p, _T("RESULTIMAGE") ) )
		{
			// �SRANK���������i��Ɏw�肳��Ă������̂͂��ׂď㏑������Ȃ��j
			for( int i = 0; i < 7; i++ )
				if( m_nResultImagePriority[i] == 0 )
					this->strResultImage[i] = p;
			continue;
		}
		// #RESULTMOVIE_SS 
		if( IsCommand( &p, _T("RESULTMOVIE_SS") ) ) { SetResultMovie( 0, p, m_nResultMoviePriority ); continue; }	// SS �ȏ�

		// #RESULTMOVIE_S 
		if( IsCommand( &p, _T("RESULTMOVIE_S") ) )	{ SetResultMovie( 1, p, m_nResultMoviePriority ); continue; }	//  S �ȏ�

		// #RESULTMOVIE_A 
		if( IsCommand( &p, _T("RESULTMOVIE_A") ) )	{ SetResultMovie( 2, p, m_nResultMoviePriority ); continue; }	//  A �ȏ�

		// #RESULTMOVIE_B 
		if( IsCommand( &p, _T("RESULTMOVIE_B") ) )	{ SetResultMovie( 3, p, m_nResultMoviePriority ); continue; }	//  B �ȏ�

		// #RESULTMOVIE_C 
		if( IsCommand( &p, _T("RESULTMOVIE_C") ) )	{ SetResultMovie( 4, p, m_nResultMoviePriority ); continue; }	//  C �ȏ�

		// #RESULTMOVIE_D 
		if( IsCommand( &p, _T("RESULTMOVIE_D") ) )	{ SetResultMovie( 5, p, m_nResultMoviePriority ); continue; }	//  D �ȏ�

		// #RESULTMOVIE_E 
		if( IsCommand( &p, _T("RESULTMOVIE_E") ) )	{ SetResultMovie( 6, p, m_nResultMoviePriority ); continue; }	//  E �ȏ�

		// #RESULTMOVIE 
		if( IsCommand( &p, _T("RESULTMOVIE") ) )
		{
			// �SRANK���������i��Ɏw�肳��Ă������̂͂��ׂď㏑������Ȃ��j
			for( int i = 0; i < 7; i++ )
				if( m_nResultMoviePriority[i] == 0 )
					this->strResultMovie[i] = p;
			continue;
		}
		// #RESULTSOUND_SS 
		if( IsCommand( &p, _T("RESULTSOUND_SS") ) ) { SetResultSound( 0, p, m_nResultSoundPriority ); continue; }	// SS �ȏ�

		// #RESULTSOUND_S 
		if( IsCommand( &p, _T("RESULTSOUND_S") ) )	{ SetResultSound( 1, p, m_nResultSoundPriority ); continue; }	//  S �ȏ�

		// #RESULTSOUND_A 
		if( IsCommand( &p, _T("RESULTSOUND_A") ) )	{ SetResultSound( 2, p, m_nResultSoundPriority ); continue; }	//  A �ȏ�

		// #RESULTSOUND_B 
		if( IsCommand( &p, _T("RESULTSOUND_B") ) )	{ SetResultSound( 3, p, m_nResultSoundPriority ); continue; }	//  B �ȏ�

		// #RESULTSOUND_C 
		if( IsCommand( &p, _T("RESULTSOUND_C") ) )	{ SetResultSound( 4, p, m_nResultSoundPriority ); continue; }	//  C �ȏ�

		// #RESULTSOUND_D 
		if( IsCommand( &p, _T("RESULTSOUND_D") ) )	{ SetResultSound( 5, p, m_nResultSoundPriority ); continue; }	//  D �ȏ�

		// #RESULTSOUND_E 
		if( IsCommand( &p, _T("RESULTSOUND_E") ) )	{ SetResultSound( 6, p, m_nResultSoundPriority ); continue; }	//  E �ȏ�

		// #RESULTSOUND 
		if( IsCommand( &p, _T("RESULTSOUND") ) )
		{
			// �SRANK���������i��Ɏw�肳��Ă������̂͂��ׂď㏑������Ȃ��j
			for( int i = 0; i < 7; i++ )
				if( m_nResultSoundPriority[i] == 0 )
					this->strResultSound[i] = p;
			continue;
		}
		// #BASEBPM 
		if( IsCommand( &p, _T("BASEBPM") ) )
		{
			double n = str2float( p );

			if( n < 0.0 )
				continue;	// #BASEBPM �ɕ����܂��͕s���ȕ�������w�肵�� �� ���� #BASEBPM �𖳌�

			this->dbBaseBPM = n;
			continue;
		}
		// #VOLUME, #WAVVOL 
		if( ( zz = this->GetCommand( &p, _T("VOLUME") ) ) != 0 || ( zz = this->GetCommand( &p, _T("WAVVOL") ) ) != 0 )
		{
			if( zz < 0 || zz >= 36*36 )
				continue;	// #VOLUME(WAVVOL)�̔ԍ����͈͊O

			// ���ʒl�̎擾
			int n = _ttoi( p );
			if( n > 100 ) n = 100;
			if( n <   0 ) n =   0;

			// �����Ή��G�ߋ��ɒ�`���� #WAV �̂����Avolume ������`�̂ɂ��Ċ��蓖�Ă�B
			if( this->m_nWaveVol[ zz ] == -zz )
			{
				for( Wave* wp = this->pWave; wp != NULL; wp = wp->next )
					if( wp->volume == -zz )
						wp->volume = n;
			}

			// �Ō�ɁAWAVVOL �X�^�b�N���X�V�B
			this->m_nWaveVol[ zz ] = n;
			continue;
		}
		// #PAN, #WAVPAN 
		if( ( zz = this->GetCommand( &p, _T("PAN") ) ) != 0 || ( zz = this->GetCommand( &p, _T("WAVPAN") ) ) != 0 )
		{
			if( zz < 0 || zz >= 36*36 )
				continue;	// #WAVPAN(PAN)�̔ԍ����͈͊O

			// ���l�̎擾
			int n = _ttoi( p );
			if( n >  100 ) n =  100;
			if( n < -100 ) n = -100;

			// �����Ή��G�ߋ��ɒ�`���� #WAV �̂����Apan ������`�̂ɂ��Ċ��蓖�Ă�B
			if( this->m_nWavePan[ zz ] == -10000-zz )
			{
				for( Wave* wp = this->pWave; wp != NULL; wp = wp->next )
					if( wp->pan == -10000-zz )
						wp->pan = n;
			}

			// �Ō�ɁAWAVPAN �X�^�b�N���X�V�B
			this->m_nWavePan[ zz ] = n;
			continue;
		}
		// #WAV 
		if( ( zz = this->GetCommand( &p, _T("WAV") ) ) != 0 )
		{
			if( zz < 0 || zz >= 36*36 )
				continue;	// WAV�ԍ����͈͊O

			// �Z���ǉ��i�����ł͂܂� Wave::sound[][] �� NULL �j
			Wave *wc		= new Wave();
			wc->num			= ++ m_nWaveNumCur;
			wc->vnum		= zz;
			wc->volume		= this->m_nWaveVol[ zz ];
			wc->pan			= this->m_nWavePan[ zz ];
			wc->bUse		= false;
			wc->strFileName = p;
			for( int i = 0; i < 11; i++ )
			{
				wc->bUseByLane[ i ]	= false;
				wc->nCurSound[ i ]	= -1;
				for( int j = 0; j < DTX_MAX_SOUND; j++ )
				{
					wc->sound[ i ][ j ]			= NULL;
					wc->bPlaying[ i ][ j ]		= false;
					wc->dbStartTime[ i ][ j ]	= 0;
					wc->dbPauseTime[ i ][ j ]	= 0;
				}
			}
			wc->dwBufferSize	= 0;
			wc->dbTotalTime		= 0;
			wc->prev = wc->next = NULL;
			APPENDLIST( this->pWave, this->pWaveLast, wc );	

			// #WAVzz ������`�A����
			// ����#WAV�����O�̍s�ɃI�u�W�F�N�g�L�q������Azz ���g����WAV�`�����l��������Ȃ�X�V����B
			if( this->m_nWaveNum[ zz ] == -zz )
			{
				for( Chip* cp = this->pChip; cp != NULL; cp=cp->next )
					if( ( this->GetChipParamType( cp->nChannel ) & DTXCPT_WAV ) && cp->nRealParam == -zz )
						cp->nRealParam = this->m_nWaveNumCur;	// Wave���ԍ�
			}
				
			// �Ō�� Wave���ԍ����X�V�B
			this->m_nWaveNum[ zz ] = this->m_nWaveNumCur;
			continue;
		}
		// #BMPTEX 
		if( ( zz = this->GetCommand( &p, _T("BMPTEX") ) ) != 0 )
		{
			if( zz < 0 || zz >= 36*36 )
				continue;	// #BMPTEX�ԍ����͈͊O

			// �Z���ǉ��i�����ł͂܂� BMPTEX::pTexture �� NULL �� �ݒ�͌㏈���� LoadBMP() �� �j
			BMPTEX *bm		= new BMPTEX();
			bm->num 		= zz;
			bm->bUse		= false;
			bm->strFileName = p;
			bm->pTexture	= NULL;
			bm->prev = bm->next = NULL;
			APPENDLIST( this->pBMPTEX, this->pBMPTEXLast, bm );

			// BMPTEX�g�p�t���OON
			this->bUseBMPTEX = true;
			continue;
		}
		// #BMP 
		if( ( zz = this->GetCommand( &p, _T("BMP") ) ) != 0 )
		{
			if( zz >= 36*36 )	// zz �͏ȗ��i�ȗ����Azz==-1�j
				continue;		// #BMP�ԍ����͈͊O

			// �Z���ǉ��i�����ł͂܂� BMP::pSurface �� NULL �� �ݒ�͌㏈���� LoadBMP() �� �j
			BMP *bm			= new BMP();
			bm->num 		= (zz < 0) ? 0 : zz;		// �ԍ�2���ȗ� �� #BMP00: (����BMP�摜) (2006/04/23)
			bm->bUse		= false;
			bm->strFileName = p;
			bm->pSurface	= NULL;
			bm->prev = bm->next = NULL;
			APPENDLIST( this->pBMP, this->pBMPLast, bm );

			// BMP�g�p�t���OON
			this->bUseBMP = true;
			continue;
		}
		// #BGAPAN 
		if( ( zz = this->GetCommand( &p, _T("BGAPAN") ) ) != 0 )
		{
			if( zz < 0 || zz >= 36*36 )
				continue;	// #BGAPAN�ԍ����͈͊O

			// �p�����[�^�擾
			int bmp, sw, sh, ew, eh, ssx, ssy, sex, sey, dsx, dsy, dex, dey, len;
			bmp = GetZex(p); SKIPZEX(p); SKIPSP_BGA(p);
			sw  = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			sh  = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			ew  = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			eh  = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			ssx = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			ssy = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			sex = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			sey = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			dsx = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			dsy = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			dex = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			dey = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			len = GetDec(p);
				
			// BMP�ԍ��̏ȗ��Ȃ�т� 00 �̎w��͖���
			if( bmp <= 0 )
				continue;	// �s����BMP�ԍ����w�肳��Ă��܂��B

			// �N���b�s���O
			// �� BMP�̃T�C�Y�͎��s���܂Ŕ���Ȃ��̂ŁA�N���b�s���O�͎��s���ɍs���B

			// �Z���ǉ�
			BGAPan *bc = new BGAPan();
			bc->num = zz;
			bc->bmp = bmp;
			bc->sw  =  sw;	bc->sh  =  sh;
			bc->ew  =  ew;	bc->eh  =  eh;
			bc->ssx = ssx;	bc->ssy = ssy;
			bc->sex = sex;	bc->sey = sey;
			bc->dsx = dsx;	bc->dsy = dsy;
			bc->dex = dex;	bc->dey = dey;
			bc->len = len;
			bc->prev = bc->next = NULL;
			APPENDLIST( this->pBGAPan, this->pBGAPanLast, bc );
			continue;
		}
		// #BGA 
		if( ( zz = this->GetCommand( &p, _T("BGA") ) ) != 0 )
		{
			if( zz < 0 || zz >= 36*36 )
				continue;	// #BGA�ԍ����͈͊O

			// �p�����[�^�擾
			int bmp, x1, y1, x2, y2, ox, oy;
			bmp = GetZex(p); SKIPZEX(p); SKIPSP_BGA(p);
			x1  = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			y1  = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			x2  = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			y2  = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			ox  = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			oy  = GetDec(p);

			// BMP�ԍ��̏ȗ��Ȃ�т� 00 �̎w��͖���
			if( bmp <= 0 ) 
				continue;	// �s����BMP�ԍ����w�肳��Ă���

			// ���W����
			// �� BMP�̃T�C�Y�͎��s���܂Ŕ���Ȃ��̂ŁABMP���̃N���b�s���O�͎��s���ɍs�����̂Ƃ���B
			if( x1 > x2  ) {int t=x1; x1=x2; x2=t;}		// �K��(x1,y1)��(x2,y2)�̍���ɗ���悤�ɂ���
			if( y1 > y2  ) {int t=y1; y1=y2; y2=t;}

			// �Z���ǉ�
			BGA *bc  = new BGA();
			bc->num  = zz;
			bc->bmp  = bmp;
			bc->x1   = x1;	bc->y1 = y1;
			bc->x2   = x2;	bc->y2 = y2;
			bc->ox   = ox;	bc->oy = oy;
			bc->prev = bc->next = NULL;
			APPENDLIST( this->pBGA, this->pBGALast, bc);
			continue;	
		}
		// #AVIPAN 
		if( ( zz = this->GetCommand( &p, _T("AVIPAN") ) ) != 0 )
		{
			if( zz < 0 || zz >= 36*36)
				continue;	// #AVIPAN�ԍ����͈͊O

			// �p�����[�^�擾
			int avi, sw, sh, ew, eh, ssx, ssy, sex, sey, dsx, dsy, dex, dey, len;
			avi = GetZex(p); SKIPZEX(p); SKIPSP_BGA(p);
			sw  = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			sh  = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			ew  = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			eh  = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			ssx = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			ssy = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			sex = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			sey = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			dsx = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			dsy = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			dex = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			dey = GetDec(p); SKIPDEC(p); SKIPSP_BGA(p);
			len = GetDec(p);
				
			// AVI�ԍ��̏ȗ��Ȃ�т� 00 �̎w��͖����B
			if( avi == 0 ) 
				continue;	// �s����AVI�ԍ����w�肳��Ă���

			// �N���b�s���O�G
			// �� AVI�̃T�C�Y�͎��s���܂Ŕ���Ȃ��̂ŁA�N���b�s���O�͎��s���ɍs���B

			// �Z���ǉ�
			AVIPan *ac = new AVIPan();
			ac->num = zz;
			ac->avi = avi;
			ac->sw  =  sw;	ac->sh  =  sh;
			ac->ew  =  ew;	ac->eh  =  eh;
			ac->ssx = ssx;	ac->ssy = ssy;
			ac->sex = sex;	ac->sey = sey;
			ac->dsx = dsx;	ac->dsy = dsy;
			ac->dex = dex;	ac->dey = dey;
			ac->len = len;
			ac->prev = ac->next = NULL;
			APPENDLIST( this->pAVIPan, this->pAVIPanLast, ac );
			continue;
		}
		// #AVI, #VIDEO 
		if( ( zz = this->GetCommand( &p, _T("AVI") ) ) != 0 || ( zz = this->GetCommand( &p, _T("VIDEO") ) ) != 0 )
		{
			if( zz < 0 || zz >= 36*36 )
				continue;	// #AVI(VIDEO)�ԍ����͈͊O

			// �Z���ǉ��i�����ł͂܂� AVI::avi �͏��������Ȃ��j
			AVI *ac = new AVI();
			ac->num  = zz;//++ nAVINumCur;	--> #AVIPAN �����ɂƂ��Ȃ��A�������������B(2006/5/5)
			ac->bUse = false;
			ac->strFileName = p;
			ac->prev = ac->next = NULL;
			APPENDLIST( this->pAVI, this->pAVILast, ac );

			// AVI�g�p�t���OON
			this->bUseAVI = true;
			continue;
		}

		// ���̑��F�`�b�v�z�u 
		{
			// (1) ���ߔԍ����擾 �� nPart 
			int nPart = GetDec3( p );
			p += 3;
			if( nPart < 0 )
				continue;	// ���ߔԍ����s��

			nPart ++;	// 1���߂��炷�i�ȊJ�n���� -1 ���߂���邽�߁j

			// (2) �`�����l���ԍ��擾 �� nCh 
			int nCh = GetChannel( p );
			p += 2;
			if( nCh < 0 )
				continue;	// �`�����l���ԍ����s��

			// (3) ':' �Ƌ󔒂��X�L�b�v 
			SKIPSP( p );
			if( *p == _T(':') )
			{
				p++;
				SKIPSP( p );
			}
			// (4) �g�p�t���O�G�Y��������̂� ON 
			if     ( nCh >= 0x11 && nCh <= 0x1A ) this->bUseDrum		= true;
			else if( nCh >= 0x20 && nCh <= 0x27 ) this->bUseGuitar		= true;
			else if( nCh >= 0xA0 && nCh <= 0xA7 ) this->bUseBass		= true;
			if( nCh == 0x18	)					  this->bUseHHOpen		= true;
			if( nCh == 0x19 )					  this->bUseRide		= true;
			if( nCh == 0x1A )					  this->bUseLeftCymal	= true;

			// (5) �p�����[�^�擾(ch.02�̏ꍇ): �����w�� 
			if( nCh == 0x02 )
			{
				double d = str2float( p );
				if( d <= 0.0 )
					continue;

				Chip *cell = new Chip();
				cell->dwPosition	= nPart * DTX_RESOLVE;
				cell->nChannel		= nCh;
				cell->dbParam		= d;
				this->InsertChip( cell );
				continue;
			}
			// (6) �p�����[�^�擾(ch.02�ȊO) 
			{	
				// (6-1) �I�u�W�F�N�g��̒����𐔂��� �� n���� 
				int n���� = 0;
				TCHAR* q = p;
				while( *q != _T('\n') && *q != _T('\0') && *q != _T(' ') && *q != _T('\t') )
				{
					// '_' �͖���
					if( *q == _T('_') ) {
						q++;
						continue;
					}
					if( ( *q >= _T('0') && *q <= _T('9') ) || ( *q >= _T('a') && *q <= _T('z') ) || (*q >= _T('A') && *q <= _T('Z') ) ) {
						n����++;
						q ++;
						continue;
					}
					else 
					{
						n���� = -1;
						break;		// �I�u�W�F�N�g�z�u������� 0�`9,A�`Z �ȊO�̕���(%02x)���g���Ă��� �� ���̍s�𖳎�
					}
				}
				if( ( n���� % 2 ) != 0 )
					n���� --;		// ��������Ȃ�A�Ō�̈ꌅ�͖���
				if( n���� <= 0 )
					continue;		// �����Ȃ����G���[�Ȃ疳��

				// (6-2) �I�u�W�F�N�g���Q������ Chip �� 
				for( int i = 0; i < n���� / 2; i++ )
				{
					// (6-2-1) '_' �͖��� 
					if( *p == _T('_') )
					{
						p++; i--;
						continue;
					}
					// (6-2-2) '00' �Ȃ疳�� 
					if( *p == _T('0') && *(p+1) == _T('0') ) {
						p += 2;
						continue;
					}

					// (6-2-3) zz �l�̎擾�GCh.03 �̂�16�i���A���̑���36�i�� 
					int zz = ( nCh == 0x03 ) ? GetHex( p ) : GetZex( p );
					
					// (6-2-4) �`�b�v�𐶐� 
					Chip *cc = new Chip();
					cc->dwPosition	= ( nPart * DTX_RESOLVE ) + ( DTX_RESOLVE * i) / ( n���� / 2 );
					cc->nChannel	= nCh;
					cc->nParam		= zz;
					cc->nRealParam	= zz; 
					if( nCh >= 0x11 && nCh <= 0x1A ) cc->nType = 0;		// �h�����`�����l��
					if( nCh >= 0x20 && nCh <= 0x27 ) cc->nType = 1;		// �M�^�[�`�����l��
					if( nCh >= 0xA0 && nCh <= 0xA7 ) cc->nType = 2;		// �M�^�[�`�����l��
					// (6-2-5) WAV�n�`�����l���Ȃ� zz �� WAVE���ԍ��ɕϊ����� 
					if( this->GetChipParamType( cc->nChannel ) & DTXCPT_WAV )
						cc->nRealParam = this->m_nWaveNum[ zz ];
					// (6-2-6) BPMEX�n�`�����l���Ȃ� zz �� BPM���ԍ��ɕϊ����� 
					else if( this->GetChipParamType( cc->nChannel ) & DTXCPT_BPMEX )
						cc->nRealParam = this->m_nBPMNum[ zz ];
					// (6-2-7) �t�B���C���n�`�����l���Ȃ�|�W�V�������C������ 
					else if( nCh == 0x53 )
					{
						if( zz > 0 && zz != 2 )				// �t�B���C���J�n �� �|�W�V������ 32 �����O�ɂ��炷
							cc->dwPosition -= 32;
						else if( zz == 2 )					// �t�B���C���I�� �� �|�W�V������ 32 ������ɂ��炷
							cc->dwPosition += 32;
					}
					// (6-2-8) �`�b�v�̒ǉ� 
					{
						// InsertChip() �� cc->dwPosition �Ń\�[�g���邽�߁Acc->dwPosition �̏C��������������ɌĂяo�����ƁB
						this->InsertChip( cc );
					}

					p += 2;		// ���̃I�u�W�F�N�g��	
				}
			}
		}
	}

	SAFE_FREE( strLine );

	// (3) LoadDTX() �����̌㏈�� 
	{
		// (3-1) �J�nBPM�`�b�v��ǉ�����B
		{
			// Chip���X�g�̐擪�ɁA#00008:00 �̃`�b�v��ǉ�����B�i#BPM: ���Ȃ̓��ŗL���ɂ��邽�߁j 
			BPM* pb;
			for( pb = this->pBPMLast; pb != NULL; pb = pb->prev )	// #BPM: ����������ꍇ�͌�u�D��
				if( pb->vnum == 0 )	break;
		
			// (A) #BPM: �����݂��Ȃ� �� #BPM00:120 �� BPM���X�g�̖����ɒǉ����A����BPM���ԍ��������� Chip ��ǉ�����B 
			if( pb == NULL )
			{
				pb = new BPM();
				pb->num		= ++ m_nBPMNumCur;
				pb->vnum	= 0;
				pb->bpm		= 120.0;
				pb->prev = pb->next = NULL;
				APPENDLIST( this->pBPM, this->pBPMLast, pb );
				
				Chip *cc = new Chip();
				cc->dwPosition	= 0;
				cc->nChannel	= 0x08;
				cc->nParam		= 0;
				cc->nRealParam	= pb->num;	// #BPM00: �� BPM ���ԍ�
				this->InsertChip( cc );
			}
			// (B) #BPM: �����݂��� �� ����BPM���ԍ���������Chip��ǉ�����B 
			else
			{
				Chip *cc = new Chip();
				cc->dwPosition	= 0;
				cc->nChannel	= 0x08;
				cc->nParam		= 0;
				cc->nRealParam	= pb->num;	// #BPM00: �� BPM ���ԍ�
				this->InsertChip( cc );
			}
		}
		// (3-2) WAVVOL �� WAVPAN �̏ȗ�����Ă��� Wave �ɂ��ăf�t�H���g�l��K�p����B 
		{
			for( Wave* wp = this->pWave; wp != NULL; wp = wp->next )
			{
				if( wp->volume < 0 )
					wp->volume = 100;
				if( wp->pan < -10000 )
					wp->pan = 0;
			}
		}
		// (3-3) ��ł��w�肪����ꍇ�A�ŏ��̋�ł��w��`�b�v���Ȃ̐擪�֕��ʂ���B 
		{
			for( int i = 0xB1; i <= 0xBB; i++ )
			{
				for( Chip* c = this->pChip; c != NULL; c = c->next )
				{
					if( c->nChannel == i ) 
					{
						Chip *cc = new Chip();
						cc->dwPosition	= 0;		// �擪
						cc->nChannel	= c->nChannel;
						cc->nParam		= c->nParam;
						cc->nRealParam	= c->nRealParam;
						this->InsertChip( cc );
						break;
					}
				}
			}
		}
		// (3-4) ����BMP�摜(#BMP00:)������ꍇ�ABMP�`�b�v���Ȃ̐擪�ɑ}������B 
		{
			for( BMP* pBmp = this->pBMPLast; pBmp != NULL; pBmp = pBmp->prev )
			{
				if( pBmp->num == 0 )
				{
					Chip *cc = new Chip();
					cc->dwPosition	= 0;			// �擪
					cc->nChannel	= 0x04;			// ch.04
					cc->nParam		= 0;
					cc->nRealParam	= 0;
					this->InsertChip( cc );
					break;
				}
			}
		}
	}

	fclose( fp );
}
void DTX::InsertChip( Chip *cell )
{
	// �\�F���ʒu�̏ꍇ�̃`�����l���Ԃ̗D�揇�ʁi�����������قǑO�ɑ}�������j 
	static const UCHAR byPriority[ 256 ] = {
		5,5,3,3,5,5,5,5,3,5,5,5,5,5,5,5,	// 0x	���ߐ��ύX�ABPM �͗D��
		5,7,7,7,7,7,7,7,7,7,7,5,5,5,5,5,	// 1x	�`�b�v�͌��ցi���̂��̂قǏ�ʕ`��j
		7,7,7,7,7,7,7,7,5,5,5,5,5,5,5,5,	// 2x
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,	// 3x
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,	// 4x
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,	// 5x
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,	// 6x
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,	// 7x
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,	// 8x
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,	// 9x
		7,7,7,7,7,7,7,7,5,5,5,5,5,5,5,5,	// Ax
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,	// Bx
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,	// Cx
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,	// Dx
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,	// Ex
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5		// Fx
	};

	// (1) �ŏ��̃Z���̏ꍇ �� ���X�g�̐擪�ɒǉ��B 
	if( this->pChipLast == NULL ) {
		this->pChip = this->pChipLast = cell;
		return;
	}
	// (2) �K�؂ȏꏊ�����X�g�u�Ō���v���猟�����A�}������B 
	for( Chip *t = this->pChipLast; t != NULL; t = t->prev )
	{
		if( t->dwPosition < cell->dwPosition ||
			( t->dwPosition == cell->dwPosition	&& byPriority[ t->nChannel ] <= byPriority[ cell->nChannel ] ) )
		{
			// t �̒���ɑ}������
			cell->prev = t;
			cell->next = t->next;
			if( t->next == NULL ) {
				t->next = cell;
				this->pChipLast = cell;
			} else {
				t->next->prev = cell;
				t->next = cell;
			}
			return;	// ����
		}
	}
	// (3) �擪�܂ŒB���� �� ���X�g�̐擪�ɑ}������B 
	cell->prev = NULL;
	cell->next = this->pChip;
	if( this->pChip != NULL ) {
		this->pChip->prev = cell;
		this->pChip = cell;
	} else {
		this->pChip = this->pChipLast = cell;
	}
}
void DTX::InsertMIDIChip( Chip *cell )
{
	// (1) �ŏ��̃Z���̏ꍇ �� ���X�g�̐擪�ɒǉ��B 
	if( this->pChipLast == NULL )
	{
		this->pChip = this->pChipLast = cell;
		return;
	}
	// (2) �K�؂ȏꏊ�����X�g�́u�Ō���v���猟�����A�}������B 
	for( Chip *t = this->pChipLast; t != NULL; t = t->prev )
	{
		// �ʒu�������̏ꍇ�ɂ́A0x11�`0x1A �͌��Ɏ����Ă����B�i��ŕ`�悳��遨����ɕ`�悳���j
		if( t->dbTime < cell->dbTime || ( t->dbTime == cell->dbTime && ( cell->nChannel >= 0x11 && cell->nChannel <= 0x1A ) ) )
		{
			// t �� cell �� ... �̏��ɂȂ�悤�}������
			cell->prev	= t;
			cell->next	= t->next;
			if (t->next == NULL) {
				t->next		 = cell;
				this->pChipLast = cell;
			} else {
				t->next->prev = cell;
				t->next		  = cell;
			}
			return; // ����
		}
	}
	// (3) �擪�܂ŒB���� �� ���X�g�̐擪�ɑ}������B 
	cell->prev = NULL;
	cell->next = this->pChip;
	if( this->pChip->prev != NULL ) {
		this->pChip->prev = cell;
		this->pChip = cell;
	} else {
		this->pChip = this->pChipLast = cell;
	}
}
void DTX::InsertLines()
{
	if( !this->pChip || !this->pChipLast )
		return;		// �`�b�v���Ȃ�

	double	dbBai = 1.0;	// ���ߒ��{��
	DWORD	dwLast;			// �Ō�̃`�b�v�����݂��鏬�߂́A���̎��̏��߂̐擪�ʒu
	dwLast  = this->pChipLast->dwPosition;
	dwLast += DTX_RESOLVE - ( dwLast % DTX_RESOLVE );

	// (1) ���ߐ��̑}�� 
	{
		for( DWORD pos = 0; pos <= dwLast; pos += DTX_RESOLVE )
		{
			Chip *nc = new Chip();
			nc->dwPosition	= pos;
			nc->nChannel	= 0x50;
			nc->nParam		= 36*36-1;		// �V�X�e�����u�������� nParam = zz
			InsertChip( nc );
		}
	}
	// (2) �����̑}�� 
	{
		Chip *p02 = this->pChip;	// for ch.02�i���ߒ��{���ύX�j
		Chip *pC1 = this->pChip;	// for ch.C1�i�����V�t�g�G02 ��u���ƁA���̏��ߓ��Ɍ���A�������甏�����n�܂�j
		
		for( DWORD pos = 0; pos < dwLast; pos += DTX_RESOLVE )		// pos �͏��ߒP�ʂő���
		{
			// �����V�t�g�G
			// ���̏��ߓ��ipos �` pos+DTX_RESOLVE �͈͓̔��j�� ch.C1�Z�� �����邩�ǂ������A��ǂ݂��Ċm�F����B
			// ��������Adw�J�n�ʒu �ɂ��̑��Έʒu�i���߃A�^�}����̋���[ct]�j���i�[���Ă����B
			// �Ȃ���΁Adw�J�n�ʒu �� 0 �B
			
			DWORD dw�J�n�ʒu = 0;

			while( pC1 && pC1->dwPosition < pos + DTX_RESOLVE )
			{
				if( pC1->nChannel == 0xC1 )
					dw�J�n�ʒu = pC1->dwPosition - pos;
				pC1 = pC1->next;
			}

			// ���ߒ��{���ύX���̇@�G BMS�ł̎��R���A
			// BMS,BME �ł́A�P���߂��Ƃɏ��ߒ��{���� 1.0 �ɖ߂�B

			if( this->scoreType == SCORETYPE_BMS )
				dbBai = 1.0;

			// ���ߒ��{���ύX���̇A�G Ch.02 �ł̕ύX������ΑΉ�����B
			while( p02 && p02->dwPosition <= pos )	// Ch.02 �͏��ߓ��ɂ�������Ȃ��i�� dwPosition ���K�� DTX_RESOLVE �Ŋ���؂��j
			{
				if( p02->nChannel == 0x02 )
					dbBai = p02->dbParam;
				p02 = p02->next;
			}

			// �����̑}��

			for( int n�����A�� = 0; n�����A�� < 100; n�����A��++ )	// < 100 �͖������[�v�h�~�̕ی�
			{
				DWORD dw�����ʒu = (DWORD)( ( DTX_RESOLVE * n�����A�� ) / ( 4.0 * dbBai ) );
				
				// �ʒu�����߂��z�����甲����
				if( dw�����ʒu + dw�J�n�ʒu >= DTX_RESOLVE )
					break;

				// �����`�b�v�̍쐬�Ƒ}��
				if( ( dw�����ʒu + dw�J�n�ʒu ) % DTX_RESOLVE != 0 )	// ���ߐ��Ɠ����ꏊ�ɂ͒u���Ȃ�
				{
					Chip *nc = new Chip();
					nc->dwPosition	= pos + dw�����ʒu + dw�J�n�ʒu;
					nc->nChannel	= 0x51;
					nc->nParam		= 36*36-1;	// �V�X�e�����u�������� nParam = zz
					InsertChip( nc );
				}
			}
		}
	}
	// (3) �����E���ߐ��̔�\���w��(ch.C2)������Ȃ�Ή�����B 
	{
		bool b������ = true;

		// ���ׂẴ`�b�v�ɂ���...
		for( Chip* cp = this->pChip; cp != NULL; cp = cp->next )
		{
			// ���� position �ɕ����̃`�b�v������ꍇ�́Ach.C2 ��D��I�ɓǂށB
			for( Chip* p���ʒuChip = cp; p���ʒuChip && p���ʒuChip->dwPosition == cp->dwPosition; p���ʒuChip = p���ʒuChip->next )
			{
				if( p���ʒuChip->nChannel == 0xC2 && p���ʒuChip->nParam == 1 ) b������ = true;
				if( p���ʒuChip->nChannel == 0xC2 && p���ʒuChip->nParam == 2 ) b������ = false;
			}
			
			// ���̎��ɁA�����E���ߐ��̉�������ݒ肷��B�������A���[�U���u�������́inParam!=ZZ �̂��́j�͉e�����󂯂Ȃ��B
			if( ( cp->nChannel == 0x50 || cp->nChannel == 0x51 ) && cp->nParam == 36*36-1 )
				cp->bVisible = b������;
		}
	}
}
void DTX::CalcChipTime()
{
	// MID �`���͎Z�o�s�v�iDrumMIDI ���ł��łɎ������v�Z�ς݁j
	if( this->scoreType == SCORETYPE_MID )
		return;

	double db���݂�BPM			= 120.0;	// [��/��]
	double db���݂̏��ߒ��{��	= 1.0;
	DWORD  dw��J�E���g�l		= 0;		// db���݂̏��ߒ��{��, db���݂�BPM �̍Ō�ɕω������J�E���g�l�i�P���߁� DTX_RESOLVE �J�E���g�ŌŒ�j
	double db�����			= 0;		// �@�@�@�@�@�@�@�@�@�@�@�V�@�@�@�@�@�@�@�@�@�@�@�@����[ms]
	DWORD  dw�O����				= 0;		// ���ߒ��ύX�p

	// ���ׂẴ`�b�v�ɂ���...
	for( Chip* cell = this->pChip; cell != NULL; cell = cell->next )
	{
		// (1) ���������̌v�Z �� cell->dbTime 
		cell->dbTime = (DWORD)( db����� + ((240000 / DTX_RESOLVE) * (cell->dwPosition - dw��J�E���g�l) * db���݂̏��ߒ��{��) / (db���݂�BPM * this->fSpeed) );

		// (2) ���߂��ς������A���ߒ��� 1.0 �ɖ߂��B(BMS�̂�) 
		if( this->scoreType == SCORETYPE_BMS && db���݂̏��ߒ��{�� != 1.0 && (cell->dwPosition / DTX_RESOLVE) != dw�O���� )
		{
			dw��J�E���g�l	= cell->dwPosition;
			db�����			= cell->dbTime;
			db���݂̏��ߒ��{��	= 1.0;
		}
		dw�O���� = cell->dwPosition / DTX_RESOLVE;

		// (3) �ȉ��A�`�����l�����Ƃ̌ʏ���
		switch( cell->nChannel )
		{
		// (3-1) ���ߒ��ύX 
		case 0x02: 
			{
				dw��J�E���g�l	= cell->dwPosition;
				db�����			= cell->dbTime;
				db���݂̏��ߒ��{��	= cell->dbParam;
			}
			break;

		// (3-2) BPM�ύX(1)����BPM 
		case 0x03: 
			{
				dw��J�E���g�l	= cell->dwPosition;
				db�����			= cell->dbTime;
				db���݂�BPM			= this->dbBaseBPM + (double)cell->nParam;
			}
			break;

		// (3-3) BPM�ύX(2)����BPM 
		case 0x08: 
			{
				dw��J�E���g�l = cell->dwPosition;
				db�����		 = cell->dbTime;

				for( BPM *bc = this->pBPMLast; bc != NULL; bc = bc->prev )
				{
					if( bc->num == cell->nRealParam )
					{
						db���݂�BPM = ( ( bc->vnum == 0 ) ? 0 : this->dbBaseBPM ) + bc->bpm;	// bc->vnum = 0 �̂Ƃ��i#BPM:�̂Ƃ��j�� BaseBPM �͉����Ȃ��I
						break;
					}
				}
			}
			break;

		// (3-4) BGAPAN 
		case 0x04: case 0x07:
		case 0x55: case 0x56: case 0x57: case 0x58: case 0x59: case 0x60:
			{
				for( BGAPan *bp = this->pBGAPanLast; bp != NULL; bp = bp->prev )
				{
					if( bp->num == cell->nParam )
					{
						double st = db����� + ((240000 / DTX_RESOLVE) * (cell->dwPosition           - dw��J�E���g�l) * db���݂̏��ߒ��{��) / db���݂�BPM;
						double et = db����� + ((240000 / DTX_RESOLVE) * (cell->dwPosition + bp->len - dw��J�E���g�l) * db���݂̏��ߒ��{��) / db���݂�BPM;
						cell->dbLong = et - st;
						break;
					}
				}
			}
			break;

		// (3-5) AVIPAN 
		case 0x54:
			{
				for( AVIPan *ap = this->pAVIPanLast; ap != NULL; ap = ap->prev )
				{
					if( ap->num == cell->nParam )
					{
						double st = db����� + ((240000 / DTX_RESOLVE) * (cell->dwPosition           - dw��J�E���g�l) * db���݂̏��ߒ��{��) / db���݂�BPM;
						double et = db����� + ((240000 / DTX_RESOLVE) * (cell->dwPosition + ap->len - dw��J�E���g�l) * db���݂̏��ߒ��{��) / db���݂�BPM;
						cell->dbLong = et - st;
						break;
					}
				}
			}
			break;
		}
	}
}
void DTX::AdujstChipTimeByPlaySpeed( double dbPlaySpeed )
{
	if( dbPlaySpeed <= 0 )
		return;

	// ���ׂẴ`�b�v�̔����������C��
	for( Chip* p = this->pChip; p != NULL; p = p->next )
		p->dbTime = p->dbTime / dbPlaySpeed;
}
// BMP, AVI, WAV �̓ǂݍ���
void DTX::LoadBMP()
{
	this->dwBMPWidthMax		= this->dwBMPHeightMax		= -1;
	this->dwBMPTEXWidthMax	= this->dwBMPTEXHeightMax	= -1;

	// (1) �e�}�l�[�W���̃L���b�V�����̃t���O�����Z�b�g 
	this->textureManager.ResetCache();
	this->surfaceManager.ResetCache();

	// (2) BMPTEX �̓ǂݍ��݁GVRAM �D��̂��߁ABMP ����ɓǂݍ��ށB 
	{
		int n�e�N�X�`�������� = 0;

		for( BMPTEX *btc = this->pBMPTEX; btc != NULL; btc = btc->next )
		{
			// #PATH_WAV �̎w�肪����ꍇ�͂�������A����ȊO�� DTX �̂���ꏊ����ǂݍ��ށB
			tstring path = ( this->strPathWav.empty() ) ? this->strFolder : this->strPathWav;
			path += btc->strFileName;

			// �e�N�X�`���̐���
			TCHAR name[ 32 ];
			_stprintf_s( name, 32, _T("BMPTEX(%d)�摜"), btc->num );
			if( ( btc->pTexture = this->textureManager.GetCachedTexture( name, path.c_str(), 0, 0 ) ) == NULL )
				continue;	// �쐬�Ɏ��s
			btc->bUse = true;

			// �ő啝�E�����̍X�V
			if( btc->pTexture->dwWidth  > this->dwBMPTEXWidthMax ) this->dwBMPTEXWidthMax  = btc->pTexture->dwWidth;
			if( btc->pTexture->dwHeight > this->dwBMPTEXHeightMax) this->dwBMPTEXHeightMax = btc->pTexture->dwHeight;

			n�e�N�X�`�������� ++;
		}
	}
	// (3) BMP �̓ǂݍ��� 
	{
		int n�T�[�t�F�C�X������ = 0;

		for( BMP *bc = this->pBMP; bc != NULL; bc = bc->next )
		{
			// #PATH_WAV �̎w�肪����ꍇ�͂�������A����ȊO�� DTX �̂���ꏊ����ǂݍ��ށB
			tstring path = ( this->strPathWav.empty() ) ? this->strFolder : this->strPathWav;
			path += bc->strFileName;

			// �T�[�t�F�C�X�̐���
			TCHAR name[ 32 ];
			_stprintf_s( name, 32, _T("BMP(%d)�摜"), bc->num );
			if( ( bc->pSurface = this->surfaceManager.GetCachedSurface( name, path.c_str(), 0, 0, LOADTO_VIDEO_OR_SYSTEM ) ) == NULL )
				continue;	// �쐬�Ɏ��s
			bc->bUse = true;

			// �ő啝�E�����̍X�V
			if( bc->pSurface->dwWidth  > this->dwBMPWidthMax ) this->dwBMPWidthMax  = bc->pSurface->dwWidth;
			if( bc->pSurface->dwHeight > this->dwBMPHeightMax) this->dwBMPHeightMax = bc->pSurface->dwHeight;

			n�T�[�t�F�C�X������ ++;
		}
	}
	// (4) �e�}�l�[�W���̃L���b�V������s�v�ȃ��\�[�X���폜 
	this->surfaceManager.RemoveUnusedSurfaces();
	this->textureManager.RemoveUnusedTextures();

	
	// �w�b�_�ǂݍ��݂݂̂Ȃ炱���ŏI��
	if( this->bHeaderOnly ) return;

	// (5) BMP, BMPTEX, BGA, BGAPAN �̎��O�œK�� 
	for( Chip *c = this->pChip; c != NULL; c = c->next )
	{
		// (5-1) BGA���C���`���l���`�b�v�ɁABMP/BMPTEX/BGA/BGAPAN�ւ̃|�C���^�����蓖�Ă�B 
		if( c->nChannel == 0x04 || c->nChannel == 0x07 || ( c->nChannel >= 0x55 && c->nChannel <= 0x59 ) || c->nChannel == 0x60 )
		{
			// ������ 
			c->BGAtype	= BGATYPE_UNKNOWN;
			c->pBMP		= NULL;
			c->pBMPTEX	= NULL;
			c->pBGA		= NULL;
			c->pBGAPan	= NULL;

			// (5-1-1) BGAPAN ���猟�����A����Ώ�������B 
			for( BGAPan *bgapan = this->pBGAPanLast; bgapan != NULL; bgapan = bgapan->prev )
			{
				if( bgapan->num == c->nParam )
				{
					BMPTEX* bmptex;
					for( bmptex = this->pBMPTEXLast; bmptex != NULL; bmptex = bmptex->prev )
					{
						if( bmptex->num == bgapan->bmp && bmptex->bUse )
						{
							c->BGAtype	= BGATYPE_BGAPAN;
							c->pBMPTEX	= bmptex;
							c->pBGAPan	= bgapan;
							break;
						}
					}
					if( bmptex ) break;
					BMP* bmp;
					for( bmp = this->pBMPLast; bmp != NULL; bmp = bmp->prev )
					{
						if( bmp->num == bgapan->bmp && bmp->bUse )
						{
							c->BGAtype	= BGATYPE_BGAPAN;
							c->pBMP		= bmp;
							c->pBGAPan	= bgapan;
							break;
						}
					}
					if( bmp ) break;
				}
			}
			if( c->BGAtype != BGATYPE_UNKNOWN )
				continue;

			// (5-1-2) BGA ���猟�����A����Ώ�������B 
			for( BGA *bga = this->pBGALast; bga != NULL; bga = bga->prev ) {
				if( bga->num == c->nParam ) 	{
					BMPTEX* bmptex;
					for( bmptex = this->pBMPTEXLast; bmptex != NULL; bmptex = bmptex->prev ) {
						if( bmptex->num == bga->bmp && bmptex->bUse ) {
							c->BGAtype	= BGATYPE_BGA;
							c->pBMPTEX	= bmptex;
							c->pBGA		= bga;
							break;
						}
					}
					if( bmptex ) break;
					BMP* bmp;
					for( bmp = this->pBMPLast; bmp != NULL; bmp = bmp->prev ) {
						if( bmp->num == bga->bmp && bmp->bUse ) {
							c->BGAtype	= BGATYPE_BGA;
							c->pBMP		= bmp;
							c->pBGA		= bga;
							break;
						}
					}
					if( bmp ) break;
				}
			}
			if( c->BGAtype != BGATYPE_UNKNOWN )
				continue;

			// (5-1-3) BMPTEX ���猟�����A����Ώ�������B 
			for( BMPTEX* bmptex = this->pBMPTEXLast; bmptex != NULL; bmptex = bmptex->prev ) {
				if( bmptex->num == c->nParam && bmptex->bUse ) {
					c->BGAtype	= BGATYPE_BMPTEX;
					c->pBMPTEX	= bmptex;
					break;
				}
			}
			if( c->BGAtype != BGATYPE_UNKNOWN )
				continue;

			// (5-1-4) BMP ���猟�����A����Ώ�������B 
			for( BMP* bmp = this->pBMPLast; bmp != NULL; bmp = bmp->prev ) {
				if( bmp->num == c->nParam && bmp->bUse ) {
					c->BGAtype	= BGATYPE_BMP;
					c->pBMP		= bmp;
					break;
				}
			}
		}
		// (5-2) DTX::Chip �� BGA�X�R�[�v�摜�ؑփ`�����l���Z���ɁABMP/BMPTEX �ւ̃|�C���^�����蓖�Ă�B 
		if( c->nChannel == 0xC4 || c->nChannel == 0xC7 || ( c->nChannel >= 0xD5 && c->nChannel <= 0xD9 ) || c->nChannel == 0xE0 )
		{
			// ������ 
			c->BGAtype	= BGATYPE_UNKNOWN;
			c->pBMP		= NULL;
			c->pBMPTEX	= NULL;
			c->pBGA		= NULL;
			c->pBGAPan	= NULL;

			// (5-2-1) BMPTEX ���猟�����A����Ώ�������B 
			for( BMPTEX* bmptex = this->pBMPTEXLast; bmptex != NULL; bmptex = bmptex->prev )
			{
				// ��BGA�X�R�[�v�摜�ؑփ`�����l���̏ꍇ�AnParam �� "BMP�ԍ�" �ł���ABGA, BGAPAN �̔ԍ��͖��֌W�ł���B
				if( bmptex->num == c->nParam && bmptex->bUse )
				{
					c->BGAtype	= BGATYPE_BMPTEX;
					c->pBMPTEX	= bmptex;
					break;
				}
			}
			if( c->BGAtype != BGATYPE_UNKNOWN )
				continue;

			// (5-2-2) BMP ���猟�����A����Ώ�������B 
			for( BMP* bmp = this->pBMPLast; bmp != NULL; bmp = bmp->prev )
			{
				// ��BGA�X�R�[�v�摜�ؑփ`�����l���̏ꍇ�AnParam �� "BMP�ԍ�" �ł���ABGA, BGAPAN �̔ԍ��͖��֌W�ł���B
				if( bmp->num == c->nParam && bmp->bUse ) {
					c->BGAtype	= BGATYPE_BMP;
					c->pBMP		= bmp;
					break;
				}
			}
		}
	}
}
void DTX::LoadAVI()
{
	// (1) AVI �̃I�[�v�� 
	int nAVI�I�[�v���� = 0;
	for( AVI *ac = this->pAVI; ac != NULL; ac = ac->next )
	{
		// #PATH_WAV �̎w�肪����ꍇ�͂�������A����ȊO�� DTX �̂���ꏊ����ǂݍ��ށB
		tstring path = ( this->strPathWav.empty() ) ? this->strFolder : this->strPathWav;
		path += ac->strFileName;

		// AVI ���I�[�v��
		ac->avi._������();
		ac->bUse = ( SUCCEEDED( ac->avi.Open( path ) ) ) ? true : false;		// open �ɐ��������� true

		if( ac->bUse )
			nAVI�I�[�v���� ++;
		else
			ac->avi.Close();	// ���s
	}

	// �w�b�_�ǂݍ��݂݂̂Ȃ炱���ŏI��
	if( this->bHeaderOnly ) return;

	// (2) AVIPAN �̎��O�œK�� 
	for( Chip *c = this->pChip; c != NULL; c = c->next )
	{
		// AVI�`���l���`�b�v�ɁAAVI/AVIPAN�ւ̃|�C���^�����蓖�Ă�B
		if( c->nChannel == 0x54 ) 
		{
			// ������ 
			c->AVItype	= AVITYPE_UNKNOWN;
			c->pAVI		= NULL;
			c->pAVIPan	= NULL;

			// (1) AVIPAN ���猟�����A����Ώ�������B 
			for( AVIPan *avipan = this->pAVIPanLast; avipan != NULL; avipan = avipan->prev )
			{
				if( avipan->num == c->nParam )
				{
					AVI* avi;
					for( avi = this->pAVILast; avi != NULL; avi = avi->prev )
					{
						if( avi->num == avipan->avi && avi->bUse )
						{
							c->AVItype	= AVITYPE_AVIPAN;
							c->pAVI		= avi;
							c->pAVIPan	= avipan;
							break;
						}
					}
					if( avi ) break;
				}
			}
			if( c->AVItype != AVITYPE_UNKNOWN )
				continue;

			// (2) AVI ���猟�����A����Ώ�������B 
			for( AVI* avi = this->pAVILast; avi != NULL; avi = avi->prev )
			{
				if( avi->num == c->nParam && avi->bUse )
				{
					c->AVItype	= AVITYPE_AVI;
					c->pAVI		= avi;
					break;
				}
			}
		}
	}
}
void DTX::LoadWAV()
{
	// (1) �T�E���h�}�l�[�W���̃L���b�V���̃��Z�b�g 
	this->soundManager.t�g�p�t���O���N���A�������Z�����폜����();

	// (2) �S�T�E���h��ǂݍ��� 
	for( Wave *wc = this->pWave; wc != NULL; wc = wc->next )
	{
		// path ������G#PATH_WAV �̎w�肪����ꍇ�͂�������A����ȊO�� DTX �̂���ꏊ����ǂݍ��ށB 
		tstring path = ( this->strPathWav.empty() ) ? this->strFolder : this->strPathWav;
		path += wc->strFileName;

		// �T�E���h���g�p���郌�[�����Ƃɐ�������
		for( int i = 0; i < 11; i++ )
		{
			if( ! wc->bUseByLane[i] ) continue;		// ���̃��[���Ŏg�p����Ȃ��Ȃ�X�L�b�v

			// �P���[���ɕt��MAX_SOUND �̃o�b�t�@���쐬����
			for( int j = 0; j < DTX_MAX_SOUND; j++ )
			{
				// �T�E���h�𐶐�����
				if( ( wc->sound[i][j] = this->soundManager.p�L���b�V���Ή��T�E���h���쐬���ĕԂ�( path.c_str() ) ) == NULL )
				{
					wc->bUse = false;	// �P�ł����s�����炱��WAV���̎g��Ȃ�
					break;
				}
				wc->bUse = true;

				// ���ʂ̏�����
				wc->sound[i][j]->SetVolume( 100 );

				// �����t����[ms]�̌v�Z
				wc->dwBufferSize = wc->sound[i][j]->GetDirectSoundBufferSize();
				DWORD dwSize;
				wc->sound[i][j]->GetDirectSoundBuffer()->GetFormat( NULL, 0, &dwSize );
				LPWAVEFORMATEX pWF = (LPWAVEFORMATEX) malloc( dwSize );
				wc->sound[i][j]->GetDirectSoundBuffer()->GetFormat( pWF, dwSize, NULL );
				wc->dbTotalTime = (double)( wc->dwBufferSize / ( pWF->nAvgBytesPerSec * 0.001 ) );
				SAFE_FREE( pWF );
			}
			if( ! wc->bUse )
				break;	// �ǂݍ��݂Ɏ��s�����̂Ŕ�����
		}
	}
	// (3) �T�E���h�}�l�[�W���̃L���b�V�����疢�g�p�T�E���h���폜����B 
	this->soundManager.t���g�p�T�E���h���폜����();
}
// �r������̍Đ��J�n
void DTX::SkipStart( double dbStartTime )
{
	// Wave �Ɋ֘A�t������`�b�v�� 1, BMP�֌W�� 2�B
	// �������X�L�b�v�Đ����Ȃ��Ƃ��� 0 �ɂ��Ă����B
	// ��ł��w��ȂǁA��������Ȃ�WAV�֌W�͖����B
	static const char bTarget[ 256 ] = {
		0,1,0,0,2,0,0,2,0,0,0,0,0,0,0,0,	// 0x	01: BGM / 04,07: BGA
		0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,	// 1x	11�`1A: Drums
		1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,	// 2x	20�`27: Guitar
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	// 3x
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	// 4x
		0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,	// 5x	55�`59,60: BGA
		2,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,	// 6x	61�`69,
		1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,	// 7x	70�`79,
		1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,	// 8x	80�`89,
		1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,	// 9x	90�`92: BGM
		1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,	// ax	a0�`a7: Bass
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	// bx
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	// cx
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	// dx
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	// ex
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};	// fx

	for( Chip* cell = this->pChip; cell != NULL; cell = cell->next )
	{
		// �J�n���������Ȃ�A���̃`�b�v�Œ����ł��؂�B
		if( cell->dbTime > dbStartTime ) 
			break;

		// ��������
		switch( bTarget[ cell->nChannel % 0xff ] )
		{
		case 1:
			SkipStartWave( dbStartTime, cell );		// WAVE �̃X�L�b�v�Đ�
			break;

		case 2:
			SkipStartBGA( dbStartTime, cell );		// BGA �̃X�L�b�v�Đ�
			break;
		}
	}
}

void DTX::SkipStartWave( double dbStartTime, Chip* cell )
{
	// (1) ���[���ԍ��Z�o �� l 
	int ch = cell->nChannel;
	int l;
	if( ch >= 0x11 && ch <= 0x1A )
		l = s_nChannelToLane[ ch - 0x11 ];		// Drums

	else if( ch >= 0x20 && ch <= 0x27 )
		l = 8;	// Guitar

	else if( ch >= 0xA0 && ch <= 0xA7 )
		l = 9;	// Bass

	else if( ch == 0x01 || (ch >= 0x61 && ch <= 0x69) || (ch >= 0x70 && ch <= 0x79) || (ch >= 0x80 && ch <= 0x89) || (ch >= 0x90 && ch <= 0x92) )
		l = 10;	// BGM

	else		
		return;	// ����ȊO�i�����Ȃ��j

	// (2) �Ή�����Wave������ �� wc 
	Wave* wc;
	for( wc = this->pWave; wc != NULL; wc = wc->next )
	{
		if( wc->num == cell->nRealParam && wc->bUse )
			break;
	}
	if( wc == NULL )
		return;		// �Ή�����Wave���Ȃ�����
	if( ! wc->bUseByLane[ l ] )
		return;		// ���̃��[���ł͍Đ����Ȃ�
	if( cell->dbTime + ( wc->dbTotalTime / this->fSpeed ) <= dbStartTime )
		return;		// �Đ�������Ȃ�

	// (3) �w��ʒu�Ɉړ����A�����ɍĐ����J�n����B 
	if( ++ wc->nCurSound[ l ] >= DTX_MAX_SOUND )
		wc->nCurSound[ l ] = 0;
	if( wc->sound[l][ wc->nCurSound[l] ] )
	{
		wc->sound[l][ wc->nCurSound[l] ]->SetVolume( (long)((DTX_AUTOVOLUME * wc->volume)/100.0) );
		wc->sound[l][ wc->nCurSound[l] ]->SetPan( wc->pan );
		wc->sound[l][ wc->nCurSound[l] ]->Play();
		wc->sound[l][ wc->nCurSound[l] ]->SetPosition( wc->sound[l][ wc->nCurSound[l] ]->GetPositionFromTime( dbStartTime - cell->dbTime ) );
	}
	wc->dbStartTime[l][ wc->nCurSound[l] ] = cell->dbTime;		// �Đ��J�n����
	wc->bPlaying[l][ wc->nCurSound[l] ] = true;
}
void DTX::SkipStartBGA( double dbStartTime, Chip* cell )
{
	// ������ BGA ���W���[���i�A�v�����j�Ɉˑ����邽�߁A�����ł͉������Ȃ��B�A�v�����Ŏ������邱�ƁB
}
// �`�b�v(WAV)�̍Đ��A��~
void DTX::PlayChip( Chip* pChip, int nLane, long lVol, bool bMIDIMonitor, bool bBad )
{
	// (0) ���O�`�F�b�N 
	if( this->pTimer == NULL ) return;
	if( pChip == NULL ) return;
	if( pChip->nRealParam < 0 ) return;
	if( nLane < 0 || nLane > 10 ) return;

	// (1) ���X�g�̖������猟�����A�Đ�����i����ԍ��ł̏d���o�^������ꍇ�A����̐ݒ肪�D�悳���j 
	for( Wave *wc = this->pWaveLast; wc != NULL; wc = wc->prev )
	{
		if( wc->num == pChip->nRealParam && wc->bUse && wc->bUseByLane[ nLane ] )
		{
			// (1-1) �T�E���h���[�e�[�V����
			if( ++ wc->nCurSound[ nLane ] >= DTX_MAX_SOUND )
				wc->nCurSound[ nLane ] = 0;

			// (1-2) �Đ�
			int s = wc->nCurSound[ nLane ];
			if( wc->sound[ nLane ][ s ] )
			{
				wc->sound[ nLane ][ s ]->SetVolume( (long)((lVol * wc->volume)/100.0) );
				wc->sound[ nLane ][ s ]->SetPan( wc->pan );

				if( ! bBad )
				{
					wc->sound[ nLane ][ s ]->SetPitch( 1.0f );
					wc->sound[ nLane ][ s ]->Play();
				}
				else
				{
					// BAD �͎��g���ϒ�
					wc->sound[ nLane ][ s ]->SetPitch( ( 100+ (((rand()%3)+1)*7) * (1-(rand()%2)*2) )  /100.0f );
					wc->sound[ nLane ][ s ]->Play();
				}
			}
			wc->dbStartTime[ nLane ][ s ] = this->pTimer->db�O��Reset��������() + pChip->dbTime;	// �Đ��J�n���� �c ���ݎ����ł͂Ȃ��AChip::dbTime �Ɉ�v������(FPS�x���̖h�~)(2007.6.9)
			wc->bPlaying[ nLane ][ s ] = true;

			// (1-3) pChip->dbTime �Ǝ����ԂƂ̍��𖄂߂邽�߁A�����Ȃ�WAVE�Đ��ʒu�C���B
			this->AdjustWave( wc, wc->dbStartTime[ nLane ][ s ], wc->sound[ nLane ][ s ] );

			break;
		}
	}
	// (2) MIDI Note ON �̏ꍇ�� MIDI �m�[�g�Ƃ��Ă������iDTXV �ł͖��Ή�(2004.01.12)�j 
	if( ! this->bDTXV )
	{
		DWORD dwMsg = pChip->message.dwMsg;
		if( this->bMIDINote && dwMsg != 0 )
		{
			if( bMIDIMonitor )								// bMonitor = true �̎���
				dwMsg = (dwMsg & 0x00FFFF) | 0x7F0000;		// ���ʂ�127�ɂ���
			midiOutShortMsg( this->hMidiOut, dwMsg );
		}
	}
}
void DTX::StopWave( int nWave, int nLane )
{
	if( nLane < 0 || nLane > 10 ) return;

	for( Wave *wc = this->pWaveLast; wc != NULL; wc = wc->prev )
	{
		if( wc->num == nWave && wc->bUse && wc->bUseByLane[ nLane ] )
		{
			for( int i = 0; i < DTX_MAX_SOUND; i++ ) {
				if( wc->sound[ nLane ][ i ] )
					wc->sound[ nLane ][ i ]->Stop();
				wc->bPlaying[ nLane ][ i ] = false;
			}
			wc->nCurSound[ nLane ] = -1;
			break;
		}
	}
}

void DTX::StopAllWaves()
{
	for( Wave* pw = this->pWave; pw != NULL; pw = pw->next ) {
		for( int i = 0; i < 11; i++ ) {
			for( int j = 0; j < DTX_MAX_SOUND; j++ ) {
				if( pw->bPlaying[i][j] && pw->sound[i][j] )
					pw->sound[i][j]->Stop();
			}
		}
	}
}

void DTX::PauseWaves()
{
	for( Wave *wc = this->pWaveLast; wc != NULL; wc = wc->prev ) {
		if( wc->bUse ) {
			for( int j = 0; j < 11; j++ ) {
				if( wc->bUseByLane[j] ) {
					for( int i = 0; i < DTX_MAX_SOUND; i++ ) {
						if( wc->sound[ j ][ i ] )
							wc->sound[ j ][ i ]->Pause();
						wc->bPlaying[ j ][ i ] = false;
						wc->dbPauseTime[ j ][ i ] = this->pTimer->GetSysTime();	// ������
					}
				}
			}
		}
	}
}

void DTX::ContWaves()
{
	for( Wave *wc = this->pWaveLast; wc != NULL; wc = wc->prev ) {
		if( wc->bUse ) {
			for( int j = 0; j < 11; j++ ) {
				if( wc->bUseByLane[j] ) {
					for( int i = 0; i < DTX_MAX_SOUND; i++ ) {
						if( wc->sound[ j ][ i ] ) {
							wc->sound[ j ][ i ]->Cont( wc->dbPauseTime[j][i] - wc->dbStartTime[j][i] );	// Pause() ����ĂȂ��ꍇ�͉����e���Ȃ�
							wc->dbStartTime[j][i] += this->pTimer->GetSysTime() - wc->dbPauseTime[j][i];
						}
						wc->bPlaying[ j ][ i ] = true;
					}
				}
			}
		}
	}
}

//
void DTX::AdjustWaves()
{
	for( Wave *wc = this->pWave; wc != NULL; wc = wc->next )
	{
		if( wc->bUse )
		{
			for( int j = 0; j < 11; j++ )
			{
				if( ! wc->bUseByLane[j] )
					continue;

				for( int i = 0; i < DTX_MAX_SOUND; i++ )
				{
					if( ! wc->sound[j][i] || ! wc->sound[j][i]->IsPlay() ) {
						wc->bPlaying[j][i] = false;		// �Đ�������Ȃ�
						continue;
					}

					// �Đ��ʒu�␳
					this->AdjustWave( wc, wc->dbStartTime[j][i], wc->sound[j][i] );
				}
			}
		}
	}
}
void DTX::AdjustWave( DTX::Wave* pWave, double dbStartTime, CSound* pSound )
{
	if( !pWave || dbStartTime < 0.0 || !pSound)
		return;

	// �����t���Ԃ� 5000ms �ȏ�̉��̂ݒ������� 
	if( pWave->dbTotalTime < 5000.0 )
		return;

	// ���T�E���h�̍Đ���CPU�Ƃ͖��֌W�ɐi��ł����̂ŁA
	//   �S�T�E���h�łP�� dbNow ����Ȃ��A�P�P�̃T�E���h���Ƃ� g_Timer.GetSysTime() ���Ĕ�ׂ�K�v������B(2007.6.9)
	double dbNow = this->pTimer->GetSysTime();

	if( dbNow > dbStartTime )
	{
		// �� this->fSpeed �� CSound::GetPositionFromTime() ���ōl�������̂ł����ł͂���Ȃ��B
		double dbPos = ( dbNow - dbStartTime ) * 1/*this->fSpeed*/;
		pSound->SetPosition( pSound->GetPositionFromTime( dbPos ) );
	}
}
//
void DTX::SetVolume( int nWave, long lVolume, int nLane )
{
	if( nLane < 0 || nLane > 10 ) return;

	for( Wave *wc = this->pWaveLast; wc != NULL; wc = wc->prev )		// ���X�g�̖������猟��... ����ԍ��ł̏d���o�^������ꍇ�A����̐ݒ肪�D�悳���B
	{
		if( wc->num == nWave && wc->bUse && wc->bUseByLane[ nLane ] )
		{
			int next = wc->nCurSound[ nLane ] + 1;					// ���ɔ������ׂ��T�E���h�ɂ��Đݒ�
			if( next >= DTX_MAX_SOUND )
				next = 0;
			if( wc->sound[ nLane ][ next ] )
				wc->sound[ nLane ][ next ]->SetVolume( lVolume );
			break;
		}
	}
}

void DTX::SetWAVCache( bool bON )
{
	this->bWAVCache = bON;
}
void DTX::SetBMPCache( bool bON )
{
	this->bBMPCache = bON;
}
void DTX::SetResultImage( int rank, LPCTSTR fname, int arPriority[] )
{
	if( rank < 0 || rank > 6 )
		return;

	for( int i = rank; i >= 0; i-- )
	{
		if( arPriority[i] < 7-rank )
		{
			arPriority[i] = 7-rank;
			this->strResultImage[i] = fname;
		}
	}
}

void DTX::SetResultMovie( int rank, LPCTSTR fname, int arPriority[] )
{
	if( rank < 0 || rank > 6 )
		return;

	for( int i = rank; i >= 0; i-- )
	{
		if( arPriority[i] < 7-rank )
		{
			arPriority[i] = 7-rank;
			this->strResultMovie[i] = fname;
		}
	}
}

void DTX::SetResultSound( int rank, LPCTSTR fname, int arPriority[] )
{
	if( rank < 0 || rank > 6 )
		return;

	for( int i = rank; i >= 0; i-- )
	{
		if( arPriority[i] < 7-rank )
		{
			arPriority[i] = 7-rank;
			this->strResultSound[i] = fname;
		}
	}
}
//
bool DTX::IsCommand( LPTSTR *p, LPCTSTR cmd )
{
	static TCHAR buf[256];
	int len = lstrlen( cmd );
	if( len > 255 ) len = 255;
	TCHAR *q = *p;
	
	int i = 0;
	while( *q != _T(':') && *q != _T('\n') && *q != _T(' ') && *q != _T('\t') && i < len )
		buf[i++] = *q++;
	buf[i] = _T('\0');

	if( i != len || _tcsnicmp( buf, cmd, i ) != 0 )
		return false;

	SKIPSP( q );
	if( *q == _T(':') ) {
		q++;
		SKIPSP( q );
	}
	*p = q;
	return true;
}

int  DTX::GetCommand( LPTSTR *p, LPCTSTR cmd )
{
	static TCHAR buf[256];			// �R�}���h�͍ő�256����
	int len = lstrlen( cmd );
	if( len > 255 ) len = 255;

	TCHAR *q = *p;
	int i;
	for( i = 0; *q != _T(':') && *q != _T('\n') && *q != _T(' ') && *q != _T('\t') && i < len; i++ )	// buf�ւ�cmd�Ɠ��������������R�s�[����
		buf[i] = *q++;
	buf[i] = _T('\0');

	if( lstrcmpi( buf, cmd ) != 0 ) return 0;

	int zz = GetZex( q );

	if( zz >= 0 )
		while((*q>=_T('0')&&*q<=_T('9'))||(*q>=_T('a')&&*q<=_T('z'))||(*q>=_T('A')&&*q<=_T('Z'))||*q==_T('-')||*q==_T('+')) q++;

	SKIPSP( q );
	if( *q == _T(':') )
	{
		q++;
		SKIPSP( q );
	}
	*p = q;
	return ( zz < 0 ) ? -1 : zz;
}

int  DTX::GetZex( LPCTSTR p )
{
	int num = 0;
	for( int i = 0; i < 2; i++ )
	{
		num *= 36;
		if( *p >= _T('0') && *p <= _T('9') )		num += *p - _T('0');
		else if( *p >= _T('a') && *p <= _T('z') )	num += *p - _T('a') + 10;
		else if( *p >= _T('A') && *p <= _T('Z') )	num += *p - _T('A') + 10;
		else
			return -1;	// 0�`9,A�`Z �ȊO�̕������g���Ă���
		p++;
	}
	return num;
}

int  DTX::GetHex( LPCTSTR p )
{
	int num = 0;
	for( int i = 0; i < 2; i++ )
	{
		num *= 16;
		if( *p >= _T('0') && *p <= _T('9') )		num += *p - _T('0');
		else if( *p >= _T('a') && *p <= _T('f') )	num += *p - _T('a') + 10;
		else if( *p >= _T('A') && *p <= _T('F') )	num += *p - _T('A') + 10;
		else 
			return -1;	// 0�`9,A�`F �ȊO�̕������g���Ă���
		p++;
	}
	return num;
}

int  DTX::GetDec( LPCTSTR p )
{
	int f = 1;
	if( *p == _T('-') )
	{
		f = -1;
		p++;
	}
	else if( *p == _T('+') )
	{
		f = 1;
		p++;
	}
	int num = 0;
	while( *p >= _T('0') && *p <= _T('9') )
	{
		num = num * 10 + (*p - _T('0'));
		p++;
	}
	return num * f;
}

int  DTX::GetDec3( LPCTSTR p )
{
	int num = 0;
	for( int i = 0; i < 3; i++ )
	{
		num *= 10;
		if( *p >= _T('0') && *p <= _T('9') )				num += *p - _T('0');
		else if( i == 0 && *p >= _T('a') && *p <= _T('z') )	num += *p - _T('a') + 10;
		else if( i == 0 && *p >= _T('A') && *p <= _T('Z') ) num += *p - _T('A') + 10;
		else
			return -1;	// 0�`9 �ȊO�̕������g���Ă���
		p++;
	}
	return num;
}

int  DTX::GetChannel( LPCTSTR p )
{
	// GDA, G2D �ȊO��16 �i���Q�P�^
	if( this->scoreType != SCORETYPE_GDA && this->scoreType != SCORETYPE_G2D )
		return GetHex( p );

	// GDA, G2D �͓��ꕶ����
	#define GDACH	( 5*12+1 )	//���̐��͂���Q��--------------------------------��
	static const struct ChMap {
		LPCTSTR strCh;
		int nCh;
	} chmap[ GDACH ] = {
		{"TC", 0x03}, {"BL", 0x02}, {"GS", 0x29}, {"DS", 0x30}, {"FI", 0x53},	// 1
		{"HH", 0x11}, {"SD", 0x12}, {"BD", 0x13}, {"HT", 0x14}, {"LT", 0x15},	// 2
		{"CY", 0x16}, {"G1", 0x21}, {"G2", 0x22}, {"G3", 0x23}, {"G4", 0x24},	// 3
		{"G5", 0x25}, {"G6", 0x26}, {"G7", 0x27}, {"GW", 0x28}, {"01", 0x61},	// 4
		{"02", 0x62}, {"03", 0x63}, {"04", 0x64}, {"05", 0x65}, {"06", 0x66},	// 5
		{"07", 0x67}, {"08", 0x68}, {"09", 0x69}, {"0A", 0x70}, {"0B", 0x71},	// 6
		{"0C", 0x72}, {"0D", 0x73}, {"0E", 0x74}, {"0F", 0x75}, {"10", 0x76},	// 7
		{"11", 0x77}, {"12", 0x78}, {"13", 0x79}, {"14", 0x80}, {"15", 0x81},	// 8
		{"16", 0x82}, {"17", 0x83}, {"18", 0x84}, {"19", 0x85}, {"1A", 0x86},	// 9
		{"1B", 0x87}, {"1C", 0x88}, {"1D", 0x89}, {"1E", 0x90}, {"1F", 0x91},	// 10
		{"20", 0x92}, {"B1", 0xA1}, {"B2", 0xA2}, {"B3", 0xA3}, {"B4", 0xA4},	// 11
		{"B5", 0xA5}, {"B6", 0xA6}, {"B7", 0xA7}, {"BW", 0xA8}, {"G0", 0x20},	// 12
		{"B0", 0xA0}															// +1
	};

	if( *p == _T('\0') || *(p+1) == _T('\0') )
		return -1;	// �`�����l���ԍ����Q�P�^�Ȃ�

	TCHAR buf[3];
	buf[0] = *p++;
	buf[1] = *p++;
	buf[2] = _T('\0');
	for( int i = 0; i < GDACH; i++ )
	{
		if( lstrcmpi( chmap[i].strCh, buf ) == 0 )
			return chmap[i].nCh;
	}
	return -1;
}

void DTX::SetSpeed( float fSpeed )
{
	for( Wave *w = this->pWave; w != NULL; w = w->next )
	{
		if( w->bUse )
		{
			for( int i = 0; i < 11; i++ )
			{
				if( w->bUseByLane[ i ] )
				{
					for( int j = 0; j < DTX_MAX_SOUND; j++ )
					{
						if( w->sound[ i ][ j ] != NULL )
							w->sound[ i ][ j ]->SetSpeed( fSpeed );
					}
				}
			}
		}
	}
}

void DTX::CalcHash()
{
	CFileMM file;

	// �t�@�C���̓ǂݍ���
	if( ! file.Load( this->strFullPath ) )
		return;		// ���s

	// �t�@�C�����e�̃n�b�V���l���v�Z���� this->strHash �֊i�[
	MD5 md5;
	md5.Init();
	md5.Update( file.GetData(), file.GetSize() );
	md5.Final( this->strHash );

	file.Term();
}

static int s_nChangeRGB[6][8] = {
	{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 },		// RGB �� RGB
	{ 0x00, 0x02, 0x01, 0x03, 0x04, 0x06, 0x05, 0x07 },		//        RBG
	{ 0x00, 0x01, 0x04, 0x05, 0x02, 0x03, 0x06, 0x07 },		//        GRB
	{ 0x00, 0x02, 0x04, 0x06, 0x01, 0x03, 0x05, 0x07 },		//        GBR
	{ 0x00, 0x04, 0x01, 0x05, 0x02, 0x06, 0x03, 0x07 },		//        BRG
	{ 0x00, 0x04, 0x02, 0x06, 0x01, 0x05, 0x03, 0x07 }		//        BGR
};
void DTX::Random( int nType, bool bSuper )
{
	int nPart    = -10000;	// ���ߔԍ�
	int nPattern = 0;		// ���ߒP�ʂ̓���ւ��@��

	int n�K�p�`�b�v�� = 0;

	for( Chip* cp = this->pChip; cp != NULL; cp = cp->next )
	{
		// ���߂��ς���������ւ��@����ύX
		if( (int)(cp->dwPosition / DTX_RESOLVE) != nPart ) {
			nPart = (int)(cp->dwPosition / DTX_RESOLVE);
			nPattern = rand() % 6;
		}

		// ���[������ւ�
		int ch = cp->nChannel;
		if( ( nType == 0 && ch >= 0x20 && ch <= 0x27 ) || ( nType == 1 && ch >= 0xA0 && ch <= 0xA7 ) )
		{
			if( bSuper )
				cp->nChannel = (ch & 0xF0) | ( s_nChangeRGB[ rand() % 6 ][ ch & 0x07 ] );
			else
				cp->nChannel = (ch & 0xF0) | ( s_nChangeRGB[ nPattern   ][ ch & 0x07 ] );

			n�K�p�`�b�v�� ++;
		}
	}
}

DWORD DTX::GetChipParamType( int nCh )
{
	switch( nCh )
	{
		// �o�b�N�R�[���X
		case 0x01:
			return DTXCPT_WAV;

		// BPM�ύX
		case 0x03:
			return DTXCPT_BPM;

		// BPM�ύX�i�g���j
		case 0x08:
			return DTXCPT_BPMEX;

		// �a�f�`
		case 0x04: case 0x07: case 0x55: case 0x56: case 0x57: case 0x58: case 0x59: case 0x60:
			return DTXCPT_BMP | DTXCPT_BMPTEX | DTXCPT_BGA | DTXCPT_BGAPAN;

		// �a�f�`�X�R�[�v�摜�ؑ�
		case 0xC4: case 0xC7: case 0xD5: case 0xD6: case 0xD7: case 0xD8: case 0xD9: case 0xE0:
			return DTXCPT_BMP | DTXCPT_BMPTEX;

		// �h�����p�[�g�i�ʏ�A�s���A�󂤂��j
		case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16: case 0x17: case 0x18: case 0x19: case 0x1A:
		case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: case 0x38: case 0x39: case 0x3A:
		case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7: case 0xB8: case 0xB9: case 0xBC:
			return DTXCPT_WAV;

		// �M�^�[�p�[�g�i�ʏ�A�󂤂��j
		case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: case 0x27:
		case 0xBA:
			return DTXCPT_WAV;

		// �x�[�X�p�[�g�i�ʏ�A��ł��j
		case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5: case 0xA6: case 0xA7:
		case 0xBB:
			return DTXCPT_WAV;

		// �a�f�l
		case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67: case 0x68: case 0x69: case 0x70:
		case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77: case 0x78: case 0x79: case 0x80:
		case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87: case 0x88: case 0x89: case 0x90:
		case 0x91: case 0x92:
			return DTXCPT_WAV;

		// AVI
		case 0x54:
			return DTXCPT_AVI;

		// ����
		case 0x1F: case 0x2F: case 0xAF:
			return DTXCPT_WAV;
	}
	
	return 0;
}

	}//General
}//FDK