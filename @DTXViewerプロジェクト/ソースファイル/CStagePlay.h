#pragma once

// FDK includes
#include "DTX.h"
#include "CSurface.h"
#include "CTexture.h"
#include "CCounter.h"
// App includes
#include "CStage.h"
#include "ActPlayAVI.h"
#include "ActPlayBGA.h"

namespace DTXViewer {
	namespace Stage {
		namespace Play {

enum EPlayPart
{
	PLAYPART_UNKNOWN	= -1,
	PLAYPART_DRUMS		= 0,
	PLAYPART_GUITAR		= 1,
	PLAYPART_BASS		= 2
};

class CStagePlay : public CStage
{
public:
	bool	Load( LPCTSTR strFileName );
	void	Play( int nPart );
	void	Stop();

	virtual int		MainWork();
	CStagePlay();

protected:
	struct _struct���t��� {
		bool			b���t��;
		DTX::Chip*		p���݂̃g�b�vChip;
		int				n���݂̏��ߔԍ��E�f�o�b�O�\���p;
		double			db���݂�BPM�E�f�o�b�O�\���p;
		double			db�J�n����ms;
		int				n�Ō�ɍĐ�����BGM�̎�WAV�ԍ�[50];	// -1=���Đ�
		int				n�Ō�ɍĐ�����HH��WAV�ԍ�;			// LC�͖��֌W, -1=���Đ�
		int				n�Ō�ɍĐ�����HH�̃`�����l���ԍ�;	// LC�͖��֌W,  0=���Đ�
		int				n�Ō�ɍĐ������M�^�[��WAV�ԍ�;		// -1=���Đ�
		int				n�Ō�ɍĐ������x�[�X��WAV�ԍ�;		// -1=���Đ�
	} struct���t���;

	CSurface		sfMap;
	CTexture		txNumbers;

	ActPlayAVI		actAVI;
	ActPlayBGA		actBGA;

	CCounter	ctNowLoading�\��;
	bool		bLoading�v������;
	tstring		strFileName;
	bool		bPlaying�v������;
	int			n�J�n���ߔԍ�;

	void	SkipStart( double db�J�n����ms );
	void	ProcessCell( DTX::Chip* pChip, bool bSkip );
	void	t�����`��( int rx, int by, int num );
	void	t�T�E���h�Đ�( DTX::Chip* pChip, EPlayPart part, long l����=DTX_PLAYVOLUME );
	bool	tDTX�t�@�C���̓ǂݍ���();
	void	t�Đ��J�n();
	void	t���t��������������();

	virtual bool	MyOneTimeSceneInit();
	virtual bool	MyActivate();
	virtual bool	MyUnActivate();
	virtual bool	MyFinalCleanup();
	virtual bool	MyInitDeviceObjects();
	virtual bool	MyRestoreDeviceTextures();
	virtual bool	MyRestoreDeviceSurfaces();
	virtual bool	MyInvalidateDeviceObjects();
	virtual bool	MyDeleteDeviceObjects();
};

		}//Play
	}//Stage
}//DTXViewer

using namespace DTXViewer::Stage::Play;
