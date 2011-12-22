// stdafx.h
// FDK �Ɨ��p�A�v���̗����ŕs�ϓI�ȁi�߂����ɏC������Ȃ��j���́B
// FDK �����̃N���X�� stdafx.h ������ include ���A���p�A�v���� fdk10.h �� include ����B

#pragma once

#define WIN32_LEAN_AND_MEAN		// Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B
#define _CRTDBG_MAP_ALLOC		// malloc �� free �̃f�o�b�O�ł��g���i_DEBUG���̂݁j

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <malloc.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <msacm.h>
#include <vfw.h>
#include <tchar.h>
#include <process.h>
#include <time.h>
#include <direct.h>			// for _tmkdir(), etc.
#include <winnls32.h>		// for WINNLSEnableIME()
#include <crtdbg.h>			// for _ASSERT(), malloc_dbg, free_dbg

#define	DIRECTINPUT_VERSION 0x0700

#include <ddraw.h>
#include <d3d.h>
#include <d3dx.h>
#include <dinput.h>
#include <dsound.h>

#ifndef bDEBUG
 #ifdef _DEBUG
  #define bDEBUG	true
 #else
  #define bDEBUG	false
 #endif
#endif

#pragma warning( disable:4312 )		// ���傫���^�ւ̕ϊ��x����}��

#define APPENDLIST(f,l,c)	 {if(l==NULL)f=l=c;else{(l)->next=c;(c)->prev=l;l=c;}}
#define	REMOVELIST(f,l,c)	 {if((c)->prev==NULL)f=(c)->next;else(c)->prev->next=(c)->next; if((c)->next==NULL)l=(c)->prev;else(c)->next->prev=(c)->prev;}
#define SAFE_DELETE(p)       { if(p) { delete (p);       (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);     (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release();   (p)=NULL; } }
#define	SAFE_FREE(p)		 { if(p) { free((void*)(p)); (p)=NULL; } }
#define ABS(a)				 (((a)<0)?-(a):(a))
#define	MIN(a,b)			 (((a)<(b))?(a):(b))
#define	MAX(a,b)			 (((a)>(b))?(a):(b))
#define SKIPSP(p)			{while(*p==_T(' ')||*p==_T('\t'))p++;}
#define SKIPDEC(p)			{while((*p>=_T('0')&&*p<=_T('9'))||*p==_T('-')||*p==_T('+'))p++;}
#define SKIPHEX(p)			{while((*p>=_T('0')&&*p<=_T('9'))||(*p>=_T('a')&&*p<=_T('f'))||(*p>=_T('A')&&*p<=_T('F'))||*p==_T('-')||*p==_T('+'))p++;}
#define SKIPZEX(p)			{while((*p>=_T('0')&&*p<=_T('9'))||(*p>=_T('a')&&*p<=_T('z'))||(*p>=_T('A')&&*p<=_T('Z'))||*p==_T('-')||*p==_T('+'))p++;}

#ifndef PI
#define PI			3.1415926535898
#endif

// STL �֌W
#include <string>
using namespace std;
typedef basic_string<TCHAR> tstring;
#pragma warning( disable:4786 )		// STL�̌x������

// �L�[���s�[�g�����}�N���@
// m ... ��ԕϐ�(int; 0..2; �����l 0)
// t ... �^�C�}�l(double; �������s�v)
// i ... �L�[��������Ă���Ȃ� true �ɂȂ������
// f ... �L�[�����֐�
#define KEYREPEAT(m,t,i,f) \
if(i){\
	switch(m){\
	case 0: f;m=1;t=g_Timer.Get();break;\
	case 1: if(g_Timer.Get()-t>200.0){f;t=g_Timer.Get();m=2;}break;\
	case 2: if(g_Timer.Get()-t>30.0){f;t=g_Timer.Get();}break;\
    }\
}else{\
	m=0;\
}

// �L�[���s�[�g�����}�N���A CCounter�Ή���; ���{����CCounter�̎g�����ł͂Ȃ��̂Œ���
// c ... CCtimer �ϐ�
// i ... �L�[��������Ă���Ȃ� true �ɂȂ������
// f ... �L�[�����֐�
#define KEYREPEATCT(c,i,f) \
if(i){\
	switch(c.nValue){\
	case 0: f; c.nValue=1; c.dbTimer=g_Timer.Get();break;\
	case 1: if(g_Timer.Get()-c.dbTimer > 200.0){f; c.dbTimer=g_Timer.Get(); c.nValue=2;}break;\
	case 2: if(g_Timer.Get()-c.dbTimer > 30.0) {f; c.dbTimer=g_Timer.Get();}break;\
    }\
}else{\
	c.nValue=0;\
}

// �A�j���i�s�}�N���@ �J�E���g�ϐ����[�v�^�C�v�ic = b��h, b��h, ...�j
// c ... �J�E���g�ϐ�(int�G�����l b�j
// t ... �^�C�}�ϐ�(double)
// b ... �J�E���g�̍ŏ��l
// h ... �J�E���g�̍ő�l
// i ... �C���^�[�o��(ms�j
#define COUNTLOOP(c,t,b,h,i) \
if( t != INIT_TIME ) {\
	if( g_Timer.Get() < t ) t = g_Timer.Get();\
	while( g_Timer.Get() - t >= (double) i ) {\
		if( ++c > h ) c = b;\
		t += (double) i;\
	}\
}

// �A�j���i�s�}�N���A �J�E���g�ϐ�����Œ�~�^�C�v�ic = b��h,h,h,h,h,....�j
#define COUNT(c,t,b,h,i) \
if( t != INIT_TIME ) {\
	if( g_Timer.Get() < t ) t = g_Timer.Get();\
	while( g_Timer.Get() - t >= (double) i ) {\
		if (++c > h) c = h;\
		t += (double) i;\
	}\
}

