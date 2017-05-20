// �N���X���
/*

	DTX �N���X

	���t�t�@�C���ɑΉ�����N���X�B�ȉ��̏������B

		(1) �I�u�W�F�N�g���

			Chip		�I�u�W�F�N�g���X�g

		(2) �w�b�_���@�i���e�����j

			�^�C�g��	������
			���x���l	���l
			etc.,

		(3) �w�b�_���A�i���X�g�����\�[�X�j�F

			BPM			BPM�i���l�j�̃��X�g
			Wave 		�������X�g�G���\�[�X�{�̂� CSoundManager �ŊǗ����A�{�\���̂ł̓|�C���^�������Ǘ�����B�j
			BMP 		�Q�c�摜���X�g�G���\�[�X�{�̂� CSurfaceManager �ŊǗ����A�{�\���̂ł̓|�C���^�������Ǘ�����B�j
			BMPTEX		�R�c�摜���X�g�F���\�[�X�{�̂� CTextureManager �ŊǗ����A�{�\���̂ł̓|�C���^�������Ǘ�����B�j
			BGA			���l��{BMP|BMPTEX}�ւ̎Q�Ƃ̃��X�g
			BGAPan		���l��{BMP|BMPTEX}�ւ̎Q�Ƃ̃��X�g
			AVI			���惊�X�g�FCAvi �����

		(4) Wave �ɑ΂���e�탁�\�b�h�F

			WAV�i�P���j�̍Đ��J�n�A��~�A���ʐݒ�A�Đ����x�ݒ�A�Đ��ʒu�␳
			WAV�i�S���j�̒��r�Đ��A��~�A�ꎞ��~�A�ĊJ

		(5) ���\�[�X�f�[�^���L���b�V�����邽�߂̊e Manager �B

			CSoundManager		�����iCSound�j�̃L���b�V��
			CSurfaceManager		�Q�c�摜�iCSurface�j�̃L���b�V��
			CTextureManager		�R�c�摜�iCTexture�j�̃L���b�V��

	�{�N���X�� Activity �N���X����p�����Ă���B
		�� DTX �� Activity �̃��[���ɏ]���A�K�؂ȃ^�C�~���O�� Init, Restore, Invalidate, Delete �ɘA�����邱�ƁB
		�@�i�ǂ��ւ� AddChild ����Ȃ��ꍇ�͓��ɒ��ӁB�j
		�� �������A�{�N���X�̎����� UnActivate() ��ɂ��A�N�Z�X�����̂ŁAUnActivate() �ł͉��������A
		   �ʓr�����������Z�b�g�i���X�g����Ȃǁj���邽�߂̃��\�b�h Clear() ����������B
*/
// �g����
/*
	�y�g�����z

	(0) ���������s���B
			�EDrumMIDI ���C�u�����̏�����
			�E�^�C�}�iCTimerEx�j�AMIDI Out�iHMIDOUT�j�̎��O�ݒ�iSet�`���\�b�h�Q�Ɓj

	(1) OneTimeSceneInit() ���Ăяo���B
			�E�e Manager �������������B

	(2) Activate() ���Ăяo���B
			�E������ Clear() ���Ăяo����A������񂪃��Z�b�g�����B

	(3) Load() �ŉ��t�t�@�C����ǂݍ��ށB
			�EMIDI�t�@�C����ǂݍ��ޏꍇ�́A���O�� DTX::m_nMIDILevel ��ݒ肷�邱�ƁB
			�E�w�b�_���@�i���e�����j�����ǂݍ��݂����ꍇ�́AbHeaderOnly = true �ɂ���B

	(4) LoadBMP(), LoadAVI() �Ńf�o�C�X�ˑ��I�u�W�F�N�g�Ɠ�����\�z����B
			�ELoad() ��Ɏ��s���邱�ƁB

	(5) ���t�O�� Random() �ŃM�^�[�`�b�v�� RANDOM ���H��K�p����B�i�I�v�V�����j
			�ELoad() ��Ɏ��s���邱�ƁB

	(6) ���t�܂��͉��t���̕\���B�A�v������ DTX �̊e public �����o�Ɏ��R�ɃA�N�Z�X����B

	(7) UnActivate() ���Ăяo���B
			�E�󃁃\�b�h�B�����s���Ȃ��B

	(8) Clear() ���Ăяo���A�����������Z�b�g����B
			�EDTX::bWAVCache, DTX::bBMPCache �� false �̏ꍇ�A�e Manager �̎����f�[�^�ɂ��ẮA�����ł͉������Ȃ��B
			 �i�e Manager ���ꂼ��̃��C�t�T�C�N���ɏ]���B�j
			 true �̏ꍇ�͂����Ŏ��f�[�^���N���A����B

	(9) FinalCleanup() ���Ăяo���B
			�E�e Manager ���I������B�����ł͓������ɂ��Ă͉������Ȃ��̂ŁA���O�� Clear() ���Ă������ƁB

	���̑��AActivity ���[���ɏ]���āA���L���\�b�h���Ăяo���B
		�EMyInitDeviceObjects()				�e Manager �ɐڑ��B
		�EMyRestoreDeviceObjects()			����
		�EMyInvalidateDeviceObjects()		����
		�EMyDeleteDeviceObjects()			����
*/

#pragma once

#include "Activity.h"
#include "CTimer.h"
#include "CSound.h"
#include "CSoundManager.h"
#include "CSurfaceManager.h"
#include "CTextureManager.h"
#include "CAvi.h"
#include "CD3DApplication.h"

namespace FDK {
	namespace General {

// ���p�����[�^��`
#define	DTX_TEXTLEN		1024		// �����񃁃��o�̍ő咷
#define	DTX_LINELEN		32768		// �ǂݍ��ݗp������i���t�t�@�C�����̂P�s�j�̍ő咷
#define	DTX_MAX_SOUND	2			// �T�E���h���Ƃ̍ő哯��������
#define	DTX_RESOLVE		384			// �P���߂̉𑜓x
#define	DTX_PLAYVOLUME	100			// �蓮���t�p�[�g�̊����[��]
#define	DTX_AUTOVOLUME	80			// �������t�p�[�g�̊����[��]

// ScoreType ��` - ���t�f�[�^�̎��
enum ScoreType {
	SCORETYPE_UNKNOWN,
	SCORETYPE_DTX,
	SCORETYPE_BMS,
	SCORETYPE_GDA,
	SCORETYPE_G2D,
	SCORETYPE_MID
};

// BGAType ��` - BGA�R�}���h���ʗp
enum BGAType {
	BGATYPE_UNKNOWN,
	BGATYPE_BMP,
	BGATYPE_BMPTEX,
	BGATYPE_BGA,
	BGATYPE_BGAPAN
};

// AVIType ��` - AVI�R�}���h���ʗp
enum AVIType {
	AVITYPE_UNKNOWN,
	AVITYPE_AVI,
	AVITYPE_AVIPAN
};

// DTXCPT_xxx ��` - DTX::Chip �̈���(nParam=zz)�̎�� (DWORD)
#define DTXCPT_WAV		(1 << 0)
#define	DTXCPT_WAVVOL	(1 << 1)
#define	DTXCPT_WAVPAN	(1 << 2)
#define	DTXCPT_BPM		(1 << 3)
#define	DTXCPT_BPMEX	(1 << 4)
#define	DTXCPT_BMP		(1 << 5)
#define	DTXCPT_BMPTEX	(1 << 6)
#define	DTXCPT_BGA		(1 << 7)
#define	DTXCPT_BGAPAN	(1 << 8)
#define	DTXCPT_AVI		(1 << 9)

class DTX : public Activity
{
public:
	void	Clear();	// ������Ԃ̏������A���\�[�X(WAV,BMP)�̉���A�������X�g�̃N���A

	void	SetTimer( CTimerEx* pc ) {this->pTimer = pc;}
	void	SetMIDIOut( HMIDIOUT hMidiOut ) {this->hMidiOut = hMidiOut;}
	void	SetWAVCache( bool bON );
	void	SetBMPCache( bool bON );

	// ���t�t�@�C���̓ǂݍ��݁G
	// Activate()��ɌĂяo�����ƁB
	// �@fname ,........ �t�@�C�����i�t���p�X�j
	// �@bHeaderOnly ... �w�b�_�݂̂̓ǂݍ��݂Ȃ�true
	// �@dbSpeed ....... ���t���x�iBPM�ɏ悶�鐔�l�G�f�t�H���g1.0�j
	bool	Load( LPCTSTR fname, bool bHeaderOnly=false, double dbPlaySpeed=1.0 );

	// ���t�t�@�C���̓ǂݍ��݁G
	// Activate()��ɌĂяo�����ƁB
	// �@fname ,........ �t�@�C�����i�t���p�X�j
	// �@bHeaderOnly ... �w�b�_�݂̂̓ǂݍ��݂Ȃ�true
	// �@dbSpeed ....... ���t���x�iBPM�ɏ悶�鐔�l�G�f�t�H���g1.0�j
	//   bLv100 ........ 10�ȉ��̃��x����10�{����ꍇ��true
	bool	Load( tstring &fname, bool bHeaderOnly=false, double dbPlaySpeed=1.0 )
			{ return Load( fname.c_str(), bHeaderOnly, dbPlaySpeed ); }
	
	// BMP, BMPTEX �̓o�^�B
	// Load()��Ɏ��s���邱�ƁB
	// �����ł� m_TextureManager|m_SurfaceManager.CreateCachedSurface() ���g�p����B
	void	LoadBMP();
	
	// AVI �t�@�C���̓ǂݍ���
	// Load()��Ɏ��s���邱�ƁB
	// �����ł́ACAvi::Init() �� CAvi::Open() �܂ł��s���B
	void	LoadAVI();
	
	// �M�^�[�^�x�[�X�ɂ��āA�����_����K�p����B
	// �@nType = 0:Guitar, 1:Bass
	// �@bSuper = true: SUPER RANDOM, false: RANDOM
	void	Random( int nType, bool bSuper=false );

	class	Chip;

	// �S�`�b�v��WAVE/BGA�ɂ��� dwStartTime ���_�ōĐ������ۂ��������A
	// �Đ����Ȃ�K�؂Ȍ�����Đ����J�n����B
	void	SkipStart( double dbStartTime );

	// WAVE �̔������ʂ̐ݒ�
	// �@nLane = ���[���ԍ��iDrums=0�`6, Guitar=7, Bass=8, BGM=9�j
	// �@lVolume = ���� 0..100 [%]
	void	SetVolume( int nWave, long lVolume=DTX_AUTOVOLUME, int nLane=9 );
	
	//
	// ���݂�WAV���X�g�ɂ��邷�ׂĂ�Wave�̍Đ��X�s�[�h��ύX����B
	void	SetSpeed( float fSpeed=1.0f );

	// ���ׂĂ�WAVE�̎��Ԓ����G
	// �J�n�������画�f���āA�Đ��J�[�\�������݂���ׂ��������ʒu�Ɉړ�����B
	void 	AdjustWaves();

	//
	// �ȃf�[�^�n�b�V�����v�Z���� m_strHash �Ɋi�[����B
	void	CalcHash();
	
	// �w�肳�ꂽChip�� nParam �� Wave ���ԍ��Ƃ݂Ȃ��Ĕ���
	// �@pChip = Chip �ւ̃|�C���^
	// �@nLane = ���[���ԍ��iDrums=0�`7, Guitar=8, Bass=9, BGM=10�j
	// �@lVol  = ��{����[%]�iDTX::Wave.volume �𔽉f����O�̉��ʁj
	// �@bMonitor = MIDI�̉���UP�Ȃ�true
	// �@bBad  = ���������炷�Ȃ� true
	void 	PlayChip( Chip* pChip, int nLane=10, long lVol=DTX_AUTOVOLUME, bool bMIDIMonitor=false, bool bBad=false );

	// WAVE �̔�����~
	// �@nWave = Wave�ԍ�
	// �@nLane = ���[���ԍ��iDrums=0�`6, Guitar=7, Bass=8, BGM=9�j
	void	StopWave( int nWave, int nLane );

	void	StopAllWaves();			// ���ׂĂ�WAVE�̔�����~
	void	PauseWaves();			// �S Wave �̔����ꎞ��~
	void	ContWaves();			// �S Wave �̔����ĊJ

	bool	MyOneTimeSceneInit();				// �������G�e Manager �̏�����
	bool	MyActivate();						// �������GClear() ���Ăяo������
	bool	MyUnActivate();						// �񊈐����GWAV,BMP,BMPTEX �ȊO�̃��\�[�X�iAVI, Chip�Ȃǁj�̉���i������ Clear()���ł��s���邪�A�ꉞ�����ł��c�B�j
	bool	MyFinalCleanup();					// �I�������G�e Manager �̉��
	bool	MyInitDeviceObjects();				// �f�o�C�X�ˑ��I�u�W�F�N�g�̏�����
	bool	MyRestoreDeviceTextures();			// �f�o�C�X�ˑ��I�u�W�F�N�g�̍\�z�i�e�N�X�`���j�B���O�� SetDirect3DDevice() �� Direct3D �f�o�C�X��o�^���Ă������ƁB
	bool	MyRestoreDeviceSurfaces();			// �f�o�C�X�ˑ��I�u�W�F�N�g�̍\�z�i�T�[�t�F�C�X�j�B���O�� SetDirect3DDevice() �� Direct3D �f�o�C�X��o�^���Ă������ƁB
	bool	MyInvalidateDeviceObjects();		// �f�o�C�X�ˑ��I�u�W�F�N�g�̉��
	bool	MyDeleteDeviceObjects();			// Desc: �f�o�C�X�ˑ��I�u�W�F�N�g�̔j��

	DTX();
	virtual ~DTX();

public:
	bool			bDTXV;						// DTXV �ł̗��p�Ȃ�A��������� true �ɂ���B
	tstring			strFullPath;				// �t�@�C�����i�t���p�X�j�i��F"C:\DTXFiles\50000t\50000t.gda")
	tstring			strFolder;					// �t�@�C���p�X�i��F"C:\DTXFiles\50000t\")
	TCHAR			strFileExt[ _MAX_EXT ];		// �t�@�C���̊g���q�i��: "gda"�j
	ScoreType		scoreType;					// �g���q�ɑΉ�����X�R�A�^�C�v
	tstring			strTitle;					// �^�C�g����
	tstring			strArtist;					// �A�[�e�B�X�g��
	tstring			strComment;					// �R�����g
	tstring			strPanel;					// �p�l���R�����g
	tstring			strGenre;					// �W������
	bool			bHiddenLevel;				// ���x����s���Ƃ��ĕ\������ꍇ�� true �B
	int				nLevel[ 3 ];				// ���x��(��:1-100:��)�i���F�ő�l�͐������ĂȂ��j(0:Drums, 1:Guitar, 2:Bass)
	double			dbBPM;						// BPM�l �� #BPM: �̒l������B
	double			dbBaseBPM;					// ��{BPM�l
	tstring			strStage;					// Stage�t�@�C����
	tstring			strPresound;				// �v���r���[���t�@�C����
	tstring			strPreimage;				// �v���r���[�摜�t�@�C����
	tstring			strPremovie;				// �v���r���[����t�@�C����
	tstring			strBackground;				// �w�i�t�@�C�����i�h������ʁj
	tstring			strBackgroundGR;			// �w�i�t�@�C�����i�f�q��ʁj
	tstring			strResultImage[7];			// ���U���g�摜�t�@�C�����irank SS,S,A,B,C,D,E �̏��ɂV�j
	tstring			strResultMovie[7];			// ���U���g����t�@�C�����irank SS,S,A,B,C,D,E �̏��ɂV�j
	tstring			strResultSound[7];			// ���U���g���t�@�C�����irank SS,S,A,B,C,D,E �̏��ɂV�j
	tstring			strMIDI;					// MIDI�t�@�C����
	bool			bMIDINote;					// MIDI�m�[�g
	int				nMIDILevel;					// MIDI���x��(0:EASY, 1:REAL, 2:RAW;  MID�ł̂ݗL��,����0)	--> MyActivate() �̌� ���� Load() �̑O�ɐݒ肷�邱�ƁI
	int				nMIDIMap;					// MIDI�}�b�v�iDM_MIDIMAP_...; DrumMIDI.h �Q�Ɓj
	bool			bBlackColorKey;				// BMP �摜�̍��l�𓧉߂���Ȃ�true
	tstring			strPathWav;					// WAV,BMP,etc. �̑��݂���t�H���_�i#PATH_WAV �w��j
	float			fSpeed;						// �Đ����x�i�f�t�H���g�F1.0f�j(DTXV��p�j
	bool			bHeaderOnly;				// Header �����̓ǂݍ��݂Ȃ� true

	// BPM �\����
	struct BPM {
		int		num;								// BPM���ԍ�
		int		vnum;								// �\�L���BPM�ԍ�(00�`ZZ; 00 ��#BPM: �̒l)
		double	bpm;								// BPM�l
		BPM		*prev, *next;						// �O��̃Z��
	} *pBPM, *pBPMLast;

	// WAVE �\����
	struct Wave {
		int		num;								// WAV���ԍ�
		int		vnum;								// �\�L���WAV�ԍ�(01�`ZZ)
		int		volume;								// ����(0�`100[%])
		int		pan;								// �p��(-100(��)�`0�`100(�E))
		bool	bUse;								// ���ꂪ false �Ȃ�� open �Ɏ��s�������Ƃ��Ӗ�����B
		tstring	strFileName;						// WAV�t�@�C����
		// �ȉ��A[0..7]:�h�����A[8]:�M�^�[�A[9]:�x�[�X�A[10]:BGM
		bool	bUseByLane[ 11 ];					// �e���[���ł���Wave���g����Ȃ�true�B���ꂪ false �Ȃ�A���Ƃ� bUse=true �ł� sound[] �͎g���Ȃ��B([0..7]:�h�����A[8]:�M�^�[�A[9]:�x�[�X�A[10]:BGM)
		int		nCurSound[ 11 ];					// ���ݍĐ����̃T�E���h�ԍ�(0..MAX_SOUND-1; �����l��-1) ([0..7]:�h�����A[8]:�M�^�[�A[9]:�x�[�X�A[10]:BGM)
		CSound*	sound[ 11 ][ DTX_MAX_SOUND ];		// �T�E���h�i�P���[���ɂ�MAX_SOUND���B �j([0..7]:�h�����A[8]:�M�^�[�A[9]:�x�[�X�A[10]:BGM)
		bool	bPlaying[ 11 ][ DTX_MAX_SOUND ];	// �Đ����Ȃ� true([0..7]:�h�����A[8]:�M�^�[�A[9]:�x�[�X�A[10]:BGM)
		double	dbStartTime[ 11 ][ DTX_MAX_SOUND ];	// �Đ��J�n����([0..7]:�h�����A[8]:�M�^�[�A[9]:�x�[�X�A[10]:BGM)
		double	dbPauseTime[ 11 ][ DTX_MAX_SOUND ];	// �ꎞ��~����([0..7]:�h�����A[8]:�M�^�[�A[9]:�x�[�X�A[10]:BGM)
		DWORD	dwBufferSize;						// �T�E���h�o�b�t�@�̃T�C�Y
		double	dbTotalTime;						// �T�E���h�o�b�t�@�̑����t����[ms]
		Wave	*prev, *next;						// �O��̃Z��
	} *pWave, *pWaveLast;

	// BMP �\����
	struct BMP {
		int			num;							// BMP�ԍ�(00�`ZZ; 00 �͏����摜)
		bool		bUse;							// ���ꂪ false �Ȃ�� open �Ɏ��s�������Ƃ��Ӗ�����B
		tstring		strFileName;					// BMP�t�@�C����
		CSurface*	pSurface;						// �摜
		BMP			*prev, *next;					// �O��̃Z��
	} *pBMP, *pBMPLast;
	bool	bUseBMP;							// BMP���P�ł�����Ȃ�true
	DWORD	dwBMPWidthMax;						// BPM �̍ő啝
	DWORD	dwBMPHeightMax;						// BPM �̍ő卂

	// BMPTEX �\����
	struct BMPTEX {
		int			num;							// BMPTEX�ԍ�(01�`ZZ)
		bool		bUse;							// ���ꂪ false �Ȃ�� open �Ɏ��s�������Ƃ��Ӗ�����B
		tstring		strFileName;					// BMPTEX�t�@�C����
		CTexture*	pTexture;						// �摜
		BMPTEX		*prev, *next;					// �O��̃Z��
	} *pBMPTEX, *pBMPTEXLast;
	bool	bUseBMPTEX;							// BMPTEX���P�ł�����Ȃ�true
	DWORD	dwBMPTEXWidthMax;					// BPMTEX �̍ő啝
	DWORD	dwBMPTEXHeightMax;					// BPMTEX �̍ő卂

	// BGA �\����
	struct BGA {
		int		num;								// BGA�ԍ�(01�`ZZ)
		int		bmp;								// BMP�ԍ�(01�`ZZ)
		int		x1, y1;								// BGA�̍�����W(BMP�摜�����΍��W)
		int		x2, y2;								// BGA�̉E�����W(        �V       )
		int		ox, oy;								// BGA�̕\���ʒu(DTXMania��BGA�̈摊��)
		BGA		*prev, *next;						// �O��̃Z��
	} *pBGA, *pBGALast;

	// BGAPan �\����
	struct BGAPan {
		int		num;								// BGAPAN�ԍ�(01�`ZZ)
		int		bmp;								// BMP�ԍ�(01�`ZZ)
		int		sw, sh;								// �J�n�]���T�C�Y
		int		ew, eh;								// �I���]���T�C�Y
		int		ssx, ssy;							// �摜���J�n���W(BMP�摜�����΍��W)
		int		sex, sey;							// �摜���I�����W(        �V       )
		int		dsx, dsy;							// �\�����J�n�ʒu(DTXMania��BGA�̈摊�΍��W)
		int		dex, dey;							// �\�����I���ʒu(            �V           )
		int		len;								// �J�n�ʒu����I���ʒu�ւ̈ړ��ɂ����鎞��[ct]�i�P���߁�PART_RESOLV ct�j
		BGAPan	*prev, *next;						// �O��̃Z��
	} *pBGAPan, *pBGAPanLast;

	// AVI �\����
	struct AVI {
		int		num;								// AVI�ԍ�(01�`ZZ)
		bool	bUse;								// ���ꂪ false �Ȃ� open �Ɏ��s�������Ƃ��Ӗ�����B
		tstring	strFileName;						// AVI�t�@�C����
		CAvi	avi;								// AVI�X�g���[��
		AVI		*prev, *next;						// �O��̃Z��
	} *pAVI, *pAVILast;
	bool bUseAVI;								// AVI���P�ł�����Ȃ�true

	// AVIPan �\����
	struct AVIPan {
		int		num;								// AVIPAN�ԍ�(01�`ZZ)
		int		avi;								// AVI�ԍ�(01�`ZZ)
		int		sw, sh;								// �J�n�]���T�C�Y
		int		ew, eh;								// �I���]���T�C�Y
		int		ssx, ssy;							// �摜���J�n���W(AVI�摜�����΍��W)
		int		sex, sey;							// �摜���I�����W(        �V       )
		int		dsx, dsy;							// �\�����J�n�ʒu(DTXMania��BGA�̈摊�΍��W)
		int		dex, dey;							// �\�����I���ʒu(            �V           )
		int		len;								// �J�n�ʒu����I���ʒu�ւ̈ړ��ɂ����鎞��[ct]�i�P���߁�PART_RESOLV ct�j
		AVIPan	*prev, *next;						// �O��̃Z��
	} *pAVIPan, *pAVIPanLast;

	// Chip �N���X
	class Chip
	{												// ��CHIP ���X�g
	public:
		DWORD	dwPosition;							// �����ʒu�i�擪����̐�Έʒu�G�P���߂̒����͏��ߒ��{���Ɋւ�炸 PART_RESOLV �ŌŒ�j; MID�̏ꍇ�A���ߐ��E�����ȊO������ 0
		int		nChannel;							// �`�����l���ԍ�
		int		nParam;								// [������] Wave,BMP,BGA,BGAPAN,AVI,BPM �ԍ�(01�`ZZ)
		int		nRealParam;							//          Wave,AVI,BPM �̎��ԍ�(1�`...����)
		double	dbParam;							// [������] �p�����[�^�l(ch.02)
		Chip	*prev, *next;						// �O��̃Z��
		double	dbTime;								// ��������[ms]�iMID �ł� g_dmCHIP_Event ����擾�j

		// MID��p; g_dmCHIP_Event ����擾�inChannel ���h�����`�b�v�̎��̂ݗL���j
		union {              
			DWORD dwMsg;							// DWORD�A�N�Z�X�p�C�x���g�f�[�^
			BYTE bData[4];							// BYTE�A�N�Z�X�p�C�x���g�f�[�^ [0]:�X�e�[�^�X
		} message;									// ���b�Z�[�W�ϊ��p���p��

		// BMP,BGA,BGAPAN ����p;
		// (1) nChannel = BGA���C���`���l��(04,07,55�`59,60) �̏ꍇ�F
		//   BGAType==BGATYPE_BMP    �̏ꍇ�ApBMP �������L���B�i���R��NULL�j
		//            BGATYPE_BMPTEX �̏ꍇ�ApBMPTEX �������L���B�i���R��NULL�j
		//            BGATYPE_BGA    �̏ꍇ�A(pBMP|pBMPTEX)��pBGA���L���B�i���Q��NULL�j
		//	          BGATYPE_BGAPAN �̏ꍇ�A(pBMP|pBMPTEX)��pBGAPAN���L���B�i���Q��NULL�j
		// (2) nChannel = BGA�X�R�[�v�摜�ؑփ`�����l��(C4,C7,D5�`D9,E0) �̏ꍇ�F
		//   BGAType==BGATYPE_BMP    �̏ꍇ�ApBMP �������L���B�i���R��NULL�j
		//            BGATYPE_BMPTEX �̏ꍇ�ApBMPTEX �������L���B�i���R��NULL�j
		//   BGAType �� BGATYPE_BGA|BGAPAN �̏ꍇ�͂��肦�Ȃ��i�����j�B
		BGAType		BGAtype;						//
		BMP*		pBMP;							//
		BMPTEX*		pBMPTEX;						// 
		BGA*		pBGA;							//
		BGAPan*		pBGAPan;						//

		// AVI,AVIPAN ����p�F
		// nChannel = AVI�`���l��(54) �̏ꍇ�A
		//   AVIType==AVITYPE_AVI    �̏ꍇ�ApAVI �������L���B�ipAVIPan��NULL�j
		//            AVITYPE_AVIPAN �̏ꍇ�ApAVI, pAVIPan �̗������L���B
		AVIType		AVItype;						//
		AVI*		pAVI;							//
		AVIPan*		pAVIPan;						//

		// BGAPAN/AVIPAN ����p
		double		dbLong;							// ���ړ�����

		// �ȉ��͉��t����p
		bool	bHit;								// �����ςȂ�true;
		bool	bVisible;							// �s���Ȃ�true;
		long	nDotFromBarD;						// �o�[����̋���[dot] �P�`�b�v�ɂ��A���̂R��S���v�Z����B
		long	nDotFromBarG;						// �o�[����̋���[dot]
		long	nDotFromBarB;						// �o�[����̋���[dot]
		int		nType;								// 0:Drum, 1:Guitar, 2:Bass, -1:Other

	public:
		Chip() {
			dwPosition 		= 0;
			nChannel		= -1;
			nParam			= 0;
			nRealParam		= 0;
			dbParam			= 0.0;
			prev = next		= NULL;
			dbTime			= 0;
			message.dwMsg	= 0;
			dbLong			= 0;
			BGAtype			= BGATYPE_UNKNOWN;
			pBMP			= NULL;
			pBMPTEX			= NULL;
			pBGA			= NULL;
			pBGAPan			= NULL;
			AVItype			= AVITYPE_UNKNOWN;
			pAVI			= NULL;
			pAVIPan			= NULL;
			bHit			= false;
			bVisible		= true;
			nDotFromBarD	= 500;
			nDotFromBarG	= 500;
			nDotFromBarB	= 500;
			nType			= -1;
		}
	} *pChip, *pChipLast;
	
	int		nTotalChipsD[ 10 ];					// �S�`�b�v���i�h�����j; AutoPlay �̃`�b�v���܂ށi��Auto���ۂ��̃t���O�̓A�v���ˑ��ł��� FDK �ɂ͂Ȃ��j
	int		nTotalChipsG;						// �S�`�b�v���i�M�^�[�j; ����
	int		nTotalChipsB;						// �S�`�b�v���i�x�[�X�j; ����
	bool	bUseDrum;							// �h�����p�[�g�Ƀ`�b�v������Ȃ� true
	bool	bUseGuitar;							// �M�^�[�p�[�g�Ƀ`�b�v������Ȃ� true
	bool	bUseBass;							// �x�[�X�p�[�g�Ƀ`�b�v������Ȃ� true
	bool	bUseHHOpen;							// HHOpen �`�b�v������Ȃ� true
	bool	bUseRide;							// Ride �`�b�v������Ȃ� true
	bool	bUseLeftCymal;						// LeftCymbal �`�b�v������Ȃ� true
	TCHAR	strHash[33];						// �ȃf�[�^�n�b�V��

protected:
	void			LoadDTX();					// DTX �`���X�R�A�t�@�C���̓ǂݍ���
	virtual void	LoadMID() {};				// MIDI �`���X�R�A�t�@�C���̓ǂݍ��݁i�I�[�o�[���C�h�p�j

	// WAV �̓ǂݍ���
	// �����ł� m_pSoundManager.CreateCachedSound() �̂ݎg�p����B
	void	LoadWAV();
	
	void	InsertChip( Chip *cell );			// �ʒu�idwPosition�j�������ɂȂ�悤�ɁA�K�؂ȏꏊ�� Chip ��}������B
	void	InsertMIDIChip( Chip *cell );		// �ʒu�idwTime�j�������ɂȂ�悤�ɁA�K�؂ȏꏊ�� cell ��}������BMIDI�p�B
	void	InsertLines();						// ���ߐ��A�����̑}��

	// �`�b�v�̔��������̎Z�o�B
	// �����ł́A���t���x�iBPM�ɏ悶�銄���j�� 1.0 �ŌŒ�Ƃ��A
	// ��� AdjustChipTimeByPlaySpeed() �Ŕ��f������B�iDTX �� MID �ŋ��ʂ̂��߁j
	void	CalcChipTime();

	// �S�`�b�v�̔����������AdbPlaySpeed �ɂ��킹�ďC������B
	// �@dbSpeed ... ���t���x�iBPM�ɏ悶�鐔�l�G�f�t�H���g1.0�j
	void	AdujstChipTimeByPlaySpeed( double dbPlaySpeed=1.0 );

	// �w�肳�ꂽ�`�����l�� nCh �� nParam (zz) �������w�肷����̂ł��邩��Ԃ��B
	// Ret:  �ȉ��̃}�X�N�̘_���ւ�Ԃ��B
	//       DTXCPT_WAV
	//       DTXCPT_BPM
	//       DTXCPT_BPMEX
	//       DTXCPT_BMP
	//       DTXCPT_BMPTEX
	//       DTXCPT_BGA
	//       DTXCPT_BGAPAN
	//       DTXCPT_AVI
	DWORD	GetChipParamType( int nCh );

	void	AdjustWave( DTX::Wave* pWave, double dbStartTime, CSound* pSound );		// Wave �P�ʂł̍Đ��ʒu��������

	void	SkipStartWave( double dbStartTime, Chip* cell );	// �w��WAVE�ɂ��� dbStartTime ���_����Đ����J�n����B
	void	SkipStartBGA( double dbStartTime, Chip* cell );		// �S�`�b�v��BGA�ɂ��� dbStartTime ���_����Đ����J�n����B

	// �D�揇�ʂ����Ȃ���A�K�؂� m_strResultImage[] �Ƀt�@�C������ݒ肷��B
	// �@rank ... 0:SS,S,A,B,C,D,E:6
	void	SetResultImage( int rank, LPCTSTR fname, int arPriority[] );
	
	// �D�揇�ʂ����Ȃ���A�K�؂� m_strResultMovie[] �Ƀt�@�C������ݒ肷��B
	// �@rank ... 0:SS,S,A,B,C,D,E:6
	void	SetResultMovie( int rank, LPCTSTR fname, int arPriority[] );

	// �D�揇�ʂ����Ȃ���A�K�؂� m_strResultSound[] �Ƀt�@�C������ݒ肷��B
	// �@rank ... 0:SS,S,A,B,C,D,E:6
	void	SetResultSound( int rank, LPCTSTR fname, int arPriority[] );

	// �R�}���h�������r
	bool	IsCommand( LPTSTR *p, LPCTSTR cmd );

	// �R�}���h�`�F�b�N
	// (1) ������ p ����w�肳�ꂽ�R�}���h�����񂪑���������@���@�����Ȃ��Ȃ� 0 ��Ԃ�
	// (2) ���̌�� [��][':'][��] ������΃X�L�b�v����
	// (3) ���̌��36�i��������Q�P�^�����������f�@���@�����Ȃ��Ȃ� -1 ��Ԃ�
	// (4) (3)�ŋL�������l��߂�l�Ƃ��ĕԂ�
	int		GetCommand( LPTSTR *p, LPCTSTR cmd );

	int		GetZex( LPCTSTR p );					// 36�i���Q�P�^�̎擾
	int		GetHex( LPCTSTR p );					// 16�i���Q�P�^�̎擾
	int		GetDec( LPCTSTR p );					// 10�i���i�������j�̎擾
	int		GetDec3( LPCTSTR p );					// 10�i���R���̎擾�i1���ڂ� 0�`Z �܂őΉ� (2002/11/03)�j
	int		GetChannel( LPCTSTR p );				// �`�����l���ԍ��̎擾

protected:
	CSoundManager		soundManager;				// �T�E���h�}�l�[�W��
	CSurfaceManager		surfaceManager;				// �T�[�t�F�C�X�}�l�[�W��
	CTextureManager		textureManager;				// �e�N�X�`���}�l�[�W��
	CTimerEx*			pTimer;						// CTimerEx �ւ̃|�C���^
	HMIDIOUT			hMidiOut;					// MIDI �o��
	bool				bWAVCache;					// WAV���L���b�V������ꍇ�� true
	bool				bBMPCache;					// BMP���L���b�V������ꍇ�� true

private:
	void	tAVI���X�g�̉���Ə�����();
	void	tAVIPAN���X�g�̉���Ə�����();
	void	tCHIP���X�g�̉���Ə�����();

	// �ȉ��͍�Ɨp�ϐ��i LoadDTX() �p�j
	int		m_nLine;						// ���ݏ������̍s�ԍ�
	int		m_nResultImagePriority[7];		// #RESULTIMAGE �p �D�揇�ʃo�b�t�@
	int		m_nResultMoviePriority[7];		// #RESULTMOVIE �p �D�揇�ʃo�b�t�@
	int		m_nResultSoundPriority[7];		// #RESULTSOUND �p �D�揇�ʃo�b�t�@
	int		m_nWaveNum[36*36];				// #WAV �p�̎��ԍ��X�^�b�N
	int		m_nWaveNumCur;					// ���݂�#WAV ���ԍ�
	int		m_nWaveVol[36*36];				// #WAVVOL �p�X�^�b�N[36*36]
	int		m_nWavePan[36*36];				// #WAVPAN �p�X�^�b�N[36*36]
	int		m_nBPMNum[36*36];				// #BPM �p�̎��ԍ��X�^�b�N
	int		m_nBPMNumCur;					// ���݂�#BPM ���ԍ�
	int		m_nBMPNum[36*36];				// #BMP �p�̎��ԍ��X�^�b�N
	int		m_nBMPTEXNum[36*36];			// #BMPTEX �p�̎��ԍ��X�^�b�N
	int		m_nBGANum[36*36];				// #BGA �p�̎��ԍ��X�^�b�N
	int		m_nBGAPanNum[36*36];			// #BGAPAN �p�̎��ԍ��X�^�b�N
	int		m_nBMPNumCur;					// ���݂�#BMP,BMPTEX,BGA,BGAPAN ���ԍ��i�S�ŋ��ʁj
	int		m_nRand;						// ���݂̗���
	bool	m_bSkip[256];					// IF�`ENDIF���X�L�b�v����Ȃ� true�i�ő�255�l�X�g�j
	int		m_nSkip;						// IF�̃l�X�g���ibSkip[] �̈����l�j
};

	}//General
}//FDK

using namespace FDK::General;
