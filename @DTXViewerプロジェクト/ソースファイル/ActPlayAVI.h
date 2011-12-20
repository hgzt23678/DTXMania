#pragma once

#include "Activity.h"
#include "CSurface.h"
#include "DTX.h"
#include "CStage.h"

namespace DTXViewer {
	namespace Stage {
		namespace Play {

class ActPlayAVI : public Activity
{
public:
	void	Start( int n�`�����l���ԍ�, DTX::AVI* pAVI, int n�J�n�T�C�YW, int n�J�n�T�C�YH, int n�I���T�C�YW, int n�I���T�C�YH, int n�摜���J�n�ʒuX, int n�摜���J�n�ʒuY, int n�摜���I���ʒuX, int n�摜���I���ʒuY, int n�\�����J�n�ʒuX, int n�\�����J�n�ʒuY, int n�\�����I���ʒuX, int n�\�����I���ʒuY, double db���ړ�����ms, double db�ړ��J�n����ms );
	void	SkipStart( double db�ړ��J�n����ms );
	void	Stop();
	void	Cont( double db�ĊJ����ms );

	ActPlayAVI();
	bool	MyOneTimeSceneInit();
	bool	MyActivate();
	bool	MyUnActivate();
	bool	MyFinalCleanup();
	bool	MyInitDeviceObjects();
	bool	MyRestoreDeviceTextures();
	bool	MyRestoreDeviceSurfaces();
	bool	MyInvalidateDeviceObjects();
	bool	MyDeleteDeviceObjects();
	int		MainWork( int x, int y );

protected:
	CSurface	sfAVI;		// AVI�`��p�T�[�t�F�C�X

	struct _structLayer
	{
		DTX::AVI*	pAVI;
		int		n�J�n�T�C�YW, n�J�n�T�C�YH;
		int		n�I���T�C�YW, n�I���T�C�YH;
		int		n�摜���J�n�ʒuX, n�摜���J�n�ʒuY;		// BMP�摜�����΍��W
		int		n�摜���I���ʒuX, n�摜���I���ʒuY;		// BMP�摜�����΍��W
		int		n�\�����J�n�ʒuX, n�\�����J�n�ʒuY;		// DTXMania��BGA�̈摊�΍��W
		int		n�\�����I���ʒuX, n�\�����I���ʒuY;		// DTXMania��BGA�̈摊�΍��W
		double	db���ړ�����ms;	
		double	db�ړ��J�n����ms;
		DWORD	dw�O��\�������t���[���ԍ�;
	}
	structLayer[ MAX_AVI_LAYER ];

};
		}//Play
	}//Stage
}//DTXViewer
