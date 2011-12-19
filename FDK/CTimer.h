#pragma once

// CTimer: �ėp�^�C�}
//    ���i�� CTimerEx ���g���ׂ��B

namespace FDK {
	namespace General {

#define	INIT_TIME	(-1.0)

#define	TIMERTYPE_PERFORMANCECOUNTER	0	// �����x�^�C�}
#define TIMERTYPE_TIMEGTTIME			1	// �}���`���f�B�A�^�C�}
#define TIMERTYPE_GETTICKCOUNT			2	// �����^�C�}
#define TIMERTYPE_UNKNOWN				99	// �s��

class CTimer
{
public:

public:
	void	Init( int timerType=TIMERTYPE_UNKNOWN );	// ������
	void	Term();										// �I������
	void	Reset();									// �^�C�}�����Z�b�g����BPAUSE �͂��ׂĉ��������B
	double	Get();										// ���ݎ����̎擾�BPAUSE���Ȃ�PAUSE���_�̎������Ԃ����B
	void	Set( double dbTime );						// ���ݎ����̐ݒ�BPAUSE���Ȃ�PAUSE���_�̎������ύX�����B
	void	Pause();									// �ꎞ��~�B
	void	Restart();									// �ꎞ��~��̍ĊJ�B
	double	GetSysTime()	{return GetTime();}		// ��Ύ����i�V�X�e���N���ォ��̌o�ߎ����j��Ԃ��B
	int		GetTimerType() { return timerType; }

public:
	CTimer() {};

protected:
	double	dbOffsetTime;		//	�I�t�Z�b�g�l
	double	dbPauseTime;		//	Pause�������Ƃ���Time
	int		nPaused;			//	pause���Ȃ�0���傫����
	UINT	uTimerID;			//  �^�C�}���荞��ID

protected:
	static int				nRefCount;		// �Q�Ɖ�
	static int				timerType;		// �d�l����^�C�}
	static TIMECAPS			dwTimeCaps;		// timeGetTime �p
	static LARGE_INTEGER	liFrequency;		// �����x�^�C�}�̉𑜓x

protected:
	// �V�X�e���N����̌o�ߎ��Ԃ��~���b�ŕԂ��B
	static double GetTime();

	// �����x�^�C�}�̎g�p�ۂ��m�F���A�g����Ȃ�ݒ肷��B
	// �g����Ȃ� true�A�g���Ȃ��Ȃ� false ��Ԃ��B
	static bool CheckAndSetA_HighPerformanceTimer();
	
	// �}���`���f�B�A�^�C�}�̎g�p�ۂ��m�F���A�g����Ȃ�ݒ肷��B
	// �g����Ȃ� true�A�g���Ȃ��Ȃ� false ��Ԃ��B
	static bool CheckAndSetB_MultimediaTimer();
	
	// �����^�C�}�̐ݒ���s���B
	// ��� true ��Ԃ��B
	static bool SetC_OldTimer();
};

/////////////////////////////
// CTimerEx: 
//    (1) Reset(), Set(time) ����̌o�ߎ���(ms)�� Get() �Ŏ擾�B
//    (2) Flush() ���Ă΂Ȃ����莞���͍X�V����Ȃ��B
//    (3) Set(time) �Ŏ�����ݒ�ł���B
//    (4) Pause(), Restart() �ňꎞ��~�E�������ł���B
//    (5) GetSysTime() �Ő�Ύ������擾�ł���B
//    (6) SetTimer(proc), KillTimer() �Ń^�C�}���荞�݂̐ݒ�Ɖ������ł���B
class CTimerEx : public CTimer
{
public:
	void   Init( int timerType=TIMERTYPE_UNKNOWN )
		{CTimer::Init(timerType); Reset();}
	void   Reset()				{Flush();dbOffsetTime=dbPauseTime=dbTimeGetTime;nPaused=0;}
	double Get()				{return (nPaused>0)? dbPauseTime-dbOffsetTime : dbTimeGetTime-dbOffsetTime;}
	double GetReal()			{return CTimer::Get();}
	void   Set( double dbTime )	{if(nPaused>0){dbOffsetTime=dbPauseTime-dbTime;}else{dbOffsetTime=dbTimeGetTime-dbTime;}}
	void   Pause()				{if(nPaused++ == 0){dbPauseTime=dbTimeGetTime;}}
	void   Restart()			{if(--nPaused == 0){dbOffsetTime+=dbTimeGetTime-dbPauseTime;}}

	void Flush()				{dbTimeGetTime=CTimer::GetTime();}
	double db�O��Reset��������()		{return dbOffsetTime;}

protected:
	double dbTimeGetTime;			//	�O��Flush��������
};

	}//General
}//FDK

using namespace FDK::General;
