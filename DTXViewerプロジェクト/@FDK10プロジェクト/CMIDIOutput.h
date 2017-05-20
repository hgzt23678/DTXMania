#pragma once

namespace FDK {
	namespace MIDI {

class CMIDIOutput
{
public:
	bool Init();	// ������
	void Term();	// �I������
	
	// ���b�Z�[�W�̑��M�@
	// �@dwMsg ... data2<<16|data1<<8|status
	void SendMsg( DWORD dwMsg );

	// Desc: ���b�Z�[�W�̑��M�A
	void SendMsg( BYTE byState, BYTE byData1, BYTE byData2 );

	CMIDIOutput() {
		this->bInit=false;
	};

public:
	HMIDIOUT hMidiOut;

protected:
	bool	 bInit;			// ����������Ă���� true
	int		 nDevices;		// MIDI Out �f�o�C�X��
};

	}//MIDI
}//FDK

using namespace FDK::MIDI;
