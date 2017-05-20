#include "stdafx.h"
#include "CMIDIInput.h"

namespace FDK {
	namespace MIDI {

// static �����o
CRITICAL_SECTION CMIDIInput::cs[ MAX_MIDIIN ];
CMIDIInput::Info CMIDIInput::m_Info[ MAX_MIDIIN ];
int CMIDIInput::nDevices = 0;


// MIDI���̓R�[���o�b�N
void CALLBACK MidiInProc( HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2 )
{
	if( wMsg != MIM_DATA )
		return;

	int nStatus		=  dwParam1        & 0xf0;	// Status
	int nKey		= (dwParam1 >> 8)  & 0xff;	// data1
	int nVelocity	= (dwParam1 >> 16) & 0xff;	// data2

	int i;
	for( i = 0; i < CMIDIInput::nDevices; i++ )
	{
		if( hMidiIn == CMIDIInput::m_Info[i].hMidiIn )
			break;
	}
	if( i >= CMIDIInput::nDevices )
		return;

	EnterCriticalSection( &(CMIDIInput::cs[i]) );

	switch( nStatus )
	{
	case 0x90:	// Note On or Off
		if( nVelocity > 0 )
		{
			if( ++CMIDIInput::m_Info[i].byUpdate[0][nKey] == 255 )
				CMIDIInput::m_Info[i].byUpdate[0][nKey] = 0;
		}
		break;

	case 0x80:	// Note Off
		break;
	}
	
	LeaveCriticalSection( &(CMIDIInput::cs[i]) );
}

CMIDIInput::CMIDIInput()
{
	this->nDevices = 0;
}


bool CMIDIInput::Init()
{
	// ������
	this->nDevices = 0;
	for( int i = 0; i < MAX_MIDIIN; i++ )
	{
		m_Info[i].uID = 0;
		m_Info[i].strName[0] = _T('\0');
		m_Info[i].hMidiIn = NULL;
		ZeroMemory( m_Info[i].byUpdate[0], 256 );
		ZeroMemory( m_Info[i].byUpdate[1], 256 );
		ZeroMemory( m_Info[i].byUpdate[2], 256 );
		m_Info[i].nBuf = 0;
		InitializeCriticalSection( &(this->cs[i]) );
	}

	// �f�o�C�X���̎擾
	this->nDevices = midiInGetNumDevs();
	if( this->nDevices < 0 )
		return true;		// MIDI ���̓f�o�C�X�Ȃ�

	if( this->nDevices > MAX_MIDIIN )
		this->nDevices = MAX_MIDIIN;	// MIDI ���̓f�o�C�X�����ő吔(%d)�𒴂����@���@����ȏ�̃f�o�C�X�͖���

	// �R�[���o�b�N��t�J�n
	for( int i = 0; i < this->nDevices; i++ )
	{
		MMRESULT mr = ::midiInOpen( (LPHMIDIIN)&m_Info[i].hMidiIn, i, (DWORD_PTR)MidiInProc, 0, CALLBACK_FUNCTION );

		if( mr == MMSYSERR_NOERROR && m_Info[i].hMidiIn != NULL )
		{
			// ��t�J�n
			::midiInStart( m_Info[i].hMidiIn );
			::midiInGetID( m_Info[i].hMidiIn, &m_Info[i].uID );

			// �f�o�C�X���擾
			MIDIINCAPS micaps;
			::ZeroMemory( &micaps, sizeof(micaps) );
			::midiInGetDevCaps( m_Info[i].uID, &micaps, sizeof(micaps) );
			lstrcpyn( m_Info[i].strName, micaps.szPname, 32 );
		}
		else
			m_Info[i].hMidiIn = NULL;	// MIDI���̓f�o�C�X�̎�t�J�n�Ɏ��s �� ���̃f�o�C�X�𖳎�
	}
	return true;
}

void CMIDIInput::Term()
{
	for( int i = 0; i < this->nDevices; i++ )
	{
		if( m_Info[i].hMidiIn != NULL )
		{
			midiInStop(  m_Info[i].hMidiIn );
			midiInReset( m_Info[i].hMidiIn );
			midiInClose( m_Info[i].hMidiIn );
			m_Info[i].hMidiIn = NULL;
		}
		DeleteCriticalSection( &(this->cs[i]) );
	}

	this->nDevices = 0;
}

void CMIDIInput::Poll()
{
	for( int i = 0; i < this->nDevices; i++ )
	{
		::EnterCriticalSection( &(this->cs[i]) );
		m_Info[i].nBuf = 1 - m_Info[i].nBuf;
		::CopyMemory( m_Info[i].byUpdate[ m_Info[i].nBuf+1 ], m_Info[i].byUpdate[0], 256 );
		::LeaveCriticalSection( &(this->cs[i]) );
	}
}

int  CMIDIInput::IsNotePushDown( UINT uID, int nKey )
{
	if( nKey < 0 || nKey > 255 )
		return 0;

	for( int i = 0; i < this->nDevices; i++ )
	{
		if( m_Info[i].uID == uID )
		{
			EnterCriticalSection( &(this->cs[i]) );
			int n = m_Info[i].byUpdate[m_Info[i].nBuf+1][nKey] - m_Info[i].byUpdate[1-m_Info[i].nBuf+1][nKey];
			LeaveCriticalSection( &(this->cs[i]) );
			if( n < 0 )
				n += 255;
			return n;
		}
	}
	return 0;
}

	}//MIDI
}//FDK