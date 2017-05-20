#include "stdafx.h"
#include "CCounter.h"
#include "CTimer.h"

namespace FDK {
	namespace General {

CCounter::CCounter()
{
	this->pTimer = NULL;
	this->nValue = 0;
	this->dbTimer = INIT_TIME;
	this->n�J�n�l = 0;
	this->n�I���l = 0;
	this->db�Ԋu  = 0.0;
}
void CCounter::Start( int n�J�n�l, int n�I���l, int n�Ԋums, CTimer* pTimer )
{
	this->n�J�n�l = n�J�n�l;
	this->n�I���l = n�I���l;
	this->db�Ԋu  = (double) n�Ԋums;
	this->pTimer  = pTimer;

	_ASSERT( this->pTimer );

	this->dbTimer = this->pTimer->Get();
	this->nValue  = n�J�n�l;
}
void CCounter::Step()
{
	if( this->dbTimer == INIT_TIME )
		return;

	_ASSERT( this->pTimer );

	double dbNow = this->pTimer->Get();
	
	if( dbNow < this->dbTimer )
		this->dbTimer = dbNow;

	while( dbNow - this->dbTimer >= this->db�Ԋu )
	{
		if( ++this->nValue > this->n�I���l )
			this->nValue = this->n�I���l;
		this->dbTimer += db�Ԋu;
	}
}
void CCounter::StepLoop()
{
	_ASSERT( this->pTimer );
	
	if( this->dbTimer == INIT_TIME )
		return;

	double dbNow = this->pTimer->Get();
	
	if( dbNow < this->dbTimer )
		this->dbTimer = dbNow;

	while( dbNow - this->dbTimer >= this->db�Ԋu )
	{
		if( ++this->nValue > this->n�I���l )
			this->nValue = this->n�J�n�l;
		this->dbTimer += db�Ԋu;
	}
}
void CCounter::Stop()
{
	this->dbTimer = INIT_TIME;
}
bool CCounter::b�I���l�ɒB����()
{
	return ( this->nValue == this->n�I���l ) ? true : false;
}
bool CCounter::b���쒆()
{
	return ( this->dbTimer != INIT_TIME )? true : false;
}
	}//General
}//FDK
