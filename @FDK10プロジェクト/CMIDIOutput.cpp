#include "stdafx.h"
#include "CMIDIOutput.h"

namespace FDK {
	namespace MIDI {

bool CMIDIOutput::Init()
{
	// �g�p���Ȃ�I���������ɍs��
	if( this->bInit )
		Term();

	// ������Ԃ̏�����
	this->bInit = false;
	this->nDevices = 0;
	this->hMidiOut = NULL;

	// �f�o�C�X���̎擾
	if( ( this->nDevices = midiOutGetNumDevs() ) == 0 )
	{
		this->bInit = true;
		return true;	// MIDI�o�̓f�o�C�X�͂P�����o�ł��Ȃ�����
	}

	// MIDI_MAPPER �f�o�C�X�̃I�[�v��
	MMRESULT mmr;
	if( ( mmr = ::midiOutOpen( &this->hMidiOut, MIDI_MAPPER, (DWORD)NULL, (DWORD)NULL, CALLBACK_NULL ) ) != MMSYSERR_NOERROR )
		return true;	// �f�t�H���g��MIDI�o�̓f�o�C�X�̃I�[�v���Ɏ��s

	// �f�o�C�X���̎擾
	MIDIOUTCAPS mocaps;
	::midiOutGetDevCaps( MIDI_MAPPER, &mocaps, sizeof(mocaps) );

	// ����
	this->bInit = true;
	return true;
}

void CMIDIOutput::Term()
{
	if (! this->bInit) 
		return;	// ����������ĂȂ�

	if( this->hMidiOut )
	{
		::midiOutClose( this->hMidiOut );
		this->hMidiOut = NULL;
	}
	this->bInit = false;
}

void CMIDIOutput::SendMsg( DWORD dwMsg )
{
	if( this->bInit && this->hMidiOut )
		::midiOutShortMsg( this->hMidiOut, dwMsg );
}

void CMIDIOutput::SendMsg( BYTE byState, BYTE byData1, BYTE byData2 )
{
	SendMsg( (DWORD)byState | (((DWORD)byData1) << 8) | ((DWORD)byData2) << 16 );
}

	}//MIDI
}//FDK