#pragma once

namespace FDK {
	namespace MIDI {

#define MAX_MIDIIN		8			// �ő� MIDI In �f�o�C�X��

class CMIDIInput
{
public:
	bool Init();								// ������
	void Term();								// �I������

	void Poll();								// MIDI���͂̃m�[�h��ԃ|�[�����O
	int  IsNotePushDown( UINT uID, int nKey );	// �w��m�[�h�����񉟉����ꂽ����Ԃ�
	int  GetDeviceNum() {return this->nDevices;}

	CMIDIInput();

public:
	static int nDevices;					// MIDI���̓f�o�C�X��
	static CRITICAL_SECTION cs[ MAX_MIDIIN ];		// MidiInProc() �Ƃ̓����p
	static struct Info	{
		UINT	uID;						// �f�o�C�X�ԍ�
		TCHAR	strName[MAXPNAMELEN];		// �f�o�C�X��
		HMIDIIN hMidiIn;					// �f�o�C�X�n���h��
		BYTE	byUpdate[3][256];			// 0:MIDI���X�V�t���O, 1:Poll���X�V�t���O
		int		nBuf;						// �t���b�v�p
	} m_Info[ MAX_MIDIIN ];

};

	}//MIDI
}//FDK

using namespace FDK::MIDI;
