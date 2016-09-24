#pragma once

#include "Activity.h"
#include "DTX.h"

namespace DTXViewer {
	namespace Stage {
		namespace Play {

class ActPlayBGA : public Activity
{
public:
	void	Start( int n�`�����l��, DTX::BMP *bmp, DTX::BMPTEX* bmptex,	int n�J�n�T�C�YW, int n�J�n�T�C�YH, int n�I���T�C�YW, int n�I���T�C�YH, int n�摜���J�n�ʒuX, int n�摜���J�n�ʒuY, int n�摜���I���ʒuX, int n�摜���I���ʒuY, int n�\�����J�n�ʒuX, int n�\�����J�n�ʒuY, int n�\�����I���ʒuX, int n�\�����I���ʒuY, double db���ړ�����ms, double db�ړ��J�n����ms=INIT_TIME );
	void	SkipStart( double db�ړ��J�n����ms );
	void	Stop();
	void	Cont( double db�ĊJ����ms );
	void	ChangeScope( int n�`�����l��, DTX::BMP* bmp, DTX::BMPTEX* bmptex );		// bmp, bmptex .. �V�����摜�i�����ꂩ����NULL�j 

	ActPlayBGA();
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
	struct _structLayer {
		DTX::BMP		*pBMP;			// BMP�ւ̃|�C���^			�i�� pBMP | pBMPTEX �̂ǂ��炩���� NULL�j
		DTX::BMPTEX		*pBMPTEX;		// BMPTEX �ւ̃|�C���^
		SIZE	sz�J�n�T�C�Y;
		SIZE	sz�I���T�C�Y;
		POINT	pt�摜���J�n�ʒu;		// BMP�摜�����΍��W
		POINT	pt�摜���I���ʒu;
		POINT	pt�\�����J�n�ʒu;		// DTXMania��BGA�̈摊�΍��W
		POINT	pt�\�����I���ʒu;
		double	db���ړ�����ms;
		double	db�ړ��J�n����ms;
	} structLayer[ 8 ];					// �ő�8�d�˂܂ŉ\�B�i�������Y���̂��̂قǉ��ʂɕ`��j
};
		}//Play
	}//Stage
}//DTXViewer
