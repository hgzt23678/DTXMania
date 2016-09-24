
// CCounter - �A�j���p�^�C�}�ƃJ�E���^�̃Z�b�g

#pragma once

#include "CTimer.h"

namespace FDK {
	namespace General {

class CCounter
{
public:
	CCounter();
	void Start( int n�J�n�l, int n�I���l, int n�Ԋums, CTimer* pTimer );	// �A�j���J�n
	void Step();															// �J�E���g��i�߂�B�I���l�ɒB���Ă���ꍇ�͏I���l���ێ�����B
	void StepLoop();														// �J�E���g��i�߂�B�I���l�ɒB���Ă���ꍇ�͊J�n�l�ɖ߂�B
	void Stop();															// �A�j���I���i�^�C�}��~�j
	bool b�I���l�ɒB����();													// ���݂� nValue �� n�I���l �Ɠ��l�̏ꍇ�� true ��Ԃ��B
	bool b���쒆();															// ���ݓ��쒆�Ȃ� true ��Ԃ��B

public:
	double	dbTimer;	// �^�C�}�l�i���ڎQ�Ɓj
	int		nValue;		// �J�E���g�l�i���ڎQ�Ɓj

protected:
	CTimer*	pTimer;
	int		n�J�n�l;
	int		n�I���l;
	double	db�Ԋu;
};

	}//General
}//FDK

using namespace FDK::General;
