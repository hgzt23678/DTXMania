#pragma once

#include "Activity.h"

namespace DTXViewer {
	namespace Stage {

enum EStage 
{
	STAGE_IDLE,				// �������Ȃ�
	STAGE_PLAY				// ���t��ʁi�h�����A�M�^�[�j
};
enum EPhase
{
	// ����
	PHASE_COMMON_�ʏ���,
	PHASE_COMMON_�t�F�[�h�C��,
	PHASE_COMMON_�t�F�[�h�A�E�g,
	PHASE_COMMON_�I�����,

	// StagePlay
	PHASE_PLAY_IDLE,
	PHASE_PLAY_LOADING1,	// "NowLoading" �\�����ԁi�܂��ǂݍ��܂��j
	PHASE_PLAY_LOADING2,	// �ǂݍ��݊J�n
	PHASE_PLAY_PLAYING1,	// ���t�J�n�҂�
	PHASE_PLAY_PLAYING2		// ���t��
};

static const int	BAR_Y			= 480-38;
static const int	BGA_X			= 361;
static const int	BGA_Y			=   2;
static const int	BGA_W			= 278;
static const int	BGA_H			= 355;
static const int	MAX_AVI_LAYER	= 1;

class CStage : public Activity
{
public:
	EStage	stageID;
	LPTSTR	str�X�e�[�W��;

	EPhase	phaseID;			// ���݂̃t�F�[�Y
	bool	bFirstWork;			// Activate() ��܂� MainWork() ���Ă΂�ĂȂ��Ȃ� true

public:
	CStage(){}
};

	}//Stage
}//DTXViewer

using namespace DTXViewer::Stage;
