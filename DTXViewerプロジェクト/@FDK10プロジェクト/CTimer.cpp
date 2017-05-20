#include "stdafx.h"
#include "CTimer.h"

namespace FDK {
	namespace General {

// static �����o
int					CTimer::nRefCount = 0;
int					CTimer::timerType = TIMERTYPE_GETTICKCOUNT;
TIMECAPS			CTimer::dwTimeCaps;
LARGE_INTEGER		CTimer::liFrequency;
//double				CTimerEx::dbTimeGetTime = 0;

void	CTimer::Init( int timerType )
{
	// �ŏ��̃C���X�^���X�Ȃ�A�S�C���X�^���X���ʂ̏��������s���B
	if( this->nRefCount == 0 )
	{
		// �g�p����^�C�}�����肷��
		switch( timerType )
		{
		// �`�D�����x�^�C�}
		// �c�D����`
		case TIMERTYPE_PERFORMANCECOUNTER:
		case TIMERTYPE_UNKNOWN:
			if( CheckAndSetA_HighPerformanceTimer() ) break;
			if( CheckAndSetB_MultimediaTimer() ) break;
			SetC_OldTimer();
			break;

		// �a�D�}���`���f�B�A�^�C�}
		case TIMERTYPE_TIMEGTTIME:
			if( CheckAndSetB_MultimediaTimer() ) break;
			if( CheckAndSetA_HighPerformanceTimer() ) break;
			SetC_OldTimer();
			break;

		// ���̑��D�����^�C�}
		default:
			SetC_OldTimer();
			break;
		}
	}

	Reset();		// �^�C�}���Z�b�g
	this->nRefCount++;	// �Q�ƃJ�E���g����
}

bool	CTimer::CheckAndSetA_HighPerformanceTimer()
{
	if( ::QueryPerformanceFrequency( &liFrequency ) )
	{
		CTimer::timerType = TIMERTYPE_PERFORMANCECOUNTER;
		return true;
	}
	return false;
}

bool	CTimer::CheckAndSetB_MultimediaTimer()
{
	if( ::timeGetDevCaps( &dwTimeCaps, sizeof( TIMECAPS ) ) == TIMERR_NOERROR && dwTimeCaps.wPeriodMin < 10 )
	{
		CTimer::timerType = TIMERTYPE_TIMEGTTIME;
		::timeBeginPeriod( dwTimeCaps.wPeriodMin );
		return true;
	}
	return false;
}

bool	CTimer::SetC_OldTimer()
{
	CTimer::timerType = TIMERTYPE_GETTICKCOUNT;
	dwTimeCaps.wPeriodMax = 1;	// ���x1ms�i�̂���ɂȂ�j
	return true;
}

void	CTimer::Term()
{
	// �Q�ƃJ�E���g����
	this->nRefCount --;

	// �C���X�^���X���Ȃ��Ȃ�����I������
	if( this->nRefCount == 0 )
	{
		if( this->timerType == TIMERTYPE_TIMEGTTIME )
			::timeEndPeriod( this->dwTimeCaps.wPeriodMin );
	}
}

void	CTimer::Reset()
{
	this->dbOffsetTime = this->dbPauseTime = GetTime();
	this->nPaused = 0;
}

double	CTimer::Get()
{
	return ( this->nPaused > 0 ) ? (this->dbPauseTime - this->dbOffsetTime) : (GetTime() - this->dbOffsetTime);
}

void	CTimer::Set( double dbTime )
{
	this->dbOffsetTime = ( this->nPaused > 0 ) ? (this->dbPauseTime - dbTime) : (GetTime() - dbTime);
}

void	CTimer::Pause()
{
	if ( this->nPaused++ == 0 )
		this->dbPauseTime = GetTime();
}

void	CTimer::Restart()
{
	if ( --this->nPaused == 0)
		this->dbOffsetTime += GetTime() - this->dbPauseTime;
}

double	CTimer::GetTime()
{
	switch( CTimer::timerType )
	{
	case TIMERTYPE_GETTICKCOUNT:
		return (double) ::GetTickCount();

	case TIMERTYPE_TIMEGTTIME:
		return (double) ::timeGetTime();

	case TIMERTYPE_PERFORMANCECOUNTER:
		{
			double dbNow = 0.0;
			if( liFrequency.QuadPart != 0 )
			{
				LARGE_INTEGER liNowTime;
				::QueryPerformanceCounter( &liNowTime );
				dbNow = liNowTime.QuadPart / ( liFrequency.QuadPart / 1000.0 );
			}
			return dbNow;
		}
	}
	return 0.0;
}

	}//General
}//FDK