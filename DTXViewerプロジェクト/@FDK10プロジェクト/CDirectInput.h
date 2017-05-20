/*
	CDirectInput �N���X

	DirectInput �̃��b�p�[�N���X�B�L�[�{�[�h�A�W���C�X�e�B�b�N�������B�i�}�E�X�͖��Ή��B�j
		�E�L�[�{�[�h		�c�@�W���L�[�{�[�h�̂݁i�Q��ڈȍ~�͖����j
		�E�W���C�X�e�B�b�N	�c�@�������iInit() �j���ɃA�^�b�`�ς݂̂��̂̂ݑΏہBInit() ��ɃA�^�b�`���ꂽ���͖̂����B
								�t�H�[�X�t�B�[�h�o�b�N�Ɋւ���T�|�[�g�͂Ȃ��B
	�y�g�����z

	(1) CD3DApplication::OneTimeSceneInit() �ŁAInit() ���Ăяo���B
		����ɂ��A�ȉ��̂悤�ȏ������������s����B
			�EDirectInput �𐶐����A�E�B���h�E�Ɗ֘A�t����B
			�E�W���L�[�{�[�h�A�W���}�E�X�A�A�^�b�`�ς݃W���C�X�e�B�b�N��񋓂��A�f�o�C�X�𐶐�����B
			�@�e�f�o�C�X�Ƃ��A�������x���� Foreground | NonExclusive | NoWinKey �ŌŒ�B
	
	(2) �K���Ȏ����ŁAPoll() ���Ăяo���B
		����ɂ��A�e�f�o�C�X�̌��݂̏�ԁA�Ȃ�тɑO���Poll() �ȍ~�ɔ��������C�x���g�f�[�^�� CDirectInput �����̔z��ϐ��Ɏ�荞�܂��B

	(3) �e�f�o�C�X�̓��̓f�[�^���擾����B
		���̓f�[�^�ɂ́A���̂Q��ނ�����B
			�E�u�ԏ��	�c�@Poll() �����s���ꂽ�u�Ԃ́A�e�f�o�C�X�̏�ԁB
			�E�C�x���g	�c�@�O���Poll() �ȍ~�ɔ��������C�x���g�f�[�^�B�i�{�^����Up, Down �Ȃǁj

			���C�x���g�f�[�^�������ꍇ�ɂ́A�t�H�[�J�X�̏�����Ď擾�ɒ��ӂ���B
			�i��FSHIFT�L�[Down�C�x���g�擾��Ƀt�H�[�J�X�������A�ʃA�v�����SHIFT�L�[��������A�Ăуt�H�[�J�X���߂����ꍇ�A
			�@�@�@�A�v���� SHIFT �L�[�� Up �C�x���g���擾�����˂Ă��܂��B�j
			�@�� WM_ACTIVATE ���t�F�b�`���A�K�؂ȃ��Z�b�g�Ȃǂ��s�����ƁB
				�iCDirectInput �N���X�ł́AWM_ACTIVATE �Ɋւ���C�x���g�͉����s��Ȃ��B�j


		IsAxis�n�́ACDirectInput �����ŏu�ԏ�Ԃ��`�F�b�N���ăC�x���g�����o����]���̂����ł̎����B
		�@nAxis=0(��),1(�E),2(��),3(��),4(�O),5(��)

	(4) CD3DApplication::FinalCleanup() �ŁATerm() ���Ăяo���B
		����ɂ��ADirectInput �֌W�̊e�I�u�W�F�N�g����������B

	�yAcquire�ɂ��āz

	�E�{�N���X�ł́APoll() �̍ŏ��ɏ�� Acquire �����s���A����Ɏ��s�����牽�������A�҂���B
	�@�iAcquire �\�ɂȂ�܂ŃE�F�C�g������͂��Ȃ��B�j
	�E�{�N���X�ł� WM_ACTIVATE �Ɋւ��鏈���͉����s��Ȃ��̂ŁA�C�x���g�f�[�^�ŏ�ԊǗ�����K�v������A�v���͎����ŏ������邱�ƁB
		
*/

#pragma once

namespace FDK {
	namespace IO {

#define	MAX_JOYSTICKS		32			// ���o����ő�̃W���C�X�e�B�b�N��
#define	MAX_DIEVENT_BUFFER	32			// ���̓C�x���g�̃o�b�t�@�T�C�Y

class CDirectInput
{
public:
	HRESULT	Init( HWND hWnd );								// ������
	void	Term();											// �I������
	bool	Poll();											// ��ԍX�V�i���͂̃|�[�����O�j

	bool	IsKeyDown( int nDIK );							// �u�ԏ��(Down)�擾�G�L�[��������Ԃł���� true ��Ԃ��B(nDIK ... 0�`255 (DIK_...; dinput.h�Q��))
	bool	IsKeyUp( int nDIK );							// �u�ԏ��(Up)�擾�G�L�[���񉟉���Ԃł���� true ��Ԃ��B(nDIK ... 0�`255 (DIK_...; dinput.h�Q��))
	bool	IsKeyPushDown( int nDIK );						// �C�x���g(Down)�擾�G�L�[���񉟉����牟����Ԃ֕ϑJ����� true ��Ԃ��B(nDIK ... 0�`255 (DIK_...; dinput.h�Q��))
	bool	IsKeyPullUp( int nDIK );						// �C�x���g(Up)�擾�G�L�[����������񉟉���Ԃ֕ϑJ����� true ��Ԃ��B(nKey ... 0�`255 (DIK_...; dinput.h�Q��))
	
	bool	IsButtonDown( int nID, int nButton );			// �u�ԏ�Ԏ擾�G�{�^����������Ԃł���� true ��Ԃ��B(nID ... JoyStick �ԍ�(0..), nButton ... 0..127)
	bool	IsButtonUp( int nID, int nButton );				// �u�ԏ�Ԏ擾�G�{�^���������Ԃł���� true ��Ԃ��B(nID ... JoyStick �ԍ�(0..), nButton ... 0..127)
	bool	IsButtonPushDown( int nID, int nButton );		// �C�x���g�擾�G�{�^�����񉟉����牟����Ԃ֕ϑJ����� true ��Ԃ��B(nID ... JoyStick �ԍ�(0..), nButton ... 0..127)
	bool	IsButtonPullUp( int nID, int nButton );			// �C�x���g�擾�G�{�^������������񉟉���Ԃ֕ϑJ����� true ��Ԃ��B(nID ... JoyStick �ԍ�(0..), nButton ... 0..127)

	bool	IsAxisDown( int nID, int nAxis );				// ����������Ԃł���� true ��Ԃ��B(nID ... JoyStick �ԍ�(0..), nAxis ... 0(��),1(�E),2(��),3(��),4(�O),5(��))
	bool	IsAxisUp( int nID, int nAxis );					// ���������Ԃł���� true ��Ԃ��B(nID ... JoyStick �ԍ�(0..), nAxis ... 0(��),1(�E),2(��),3(��),4(�O),5(��))
	bool	IsAxisPushDown( int nID, int nAxis );			// ����������牟����Ԃ֕ϑJ����� true ��Ԃ��B(nID ... JoyStick �ԍ�(0..), nAxis ... 0(��),1(�E),2(��),3(��),4(�O),5(��))
	bool	IsAxisPullUp( int nID, int nAxis );				// ����������������Ԃ֕ϑJ����� true ��Ԃ��B(nID ... JoyStick �ԍ�(0..), nAxis ... 0(��),1(�E),2(��),3(��),4(�O),5(��))
	
	int		GetJoysticksNum()	 { return this->nJoySticks; }	// �W���C�X�e�B�b�N�̐�
	LPDIJOYSTATE2 GetJoyState( int nID );					// �u�ԏ�Ԏ擾�G���݂� DIJOYSTATE2 ��Ԃ��B(nID ... JoyStick �ԍ�(0..))
	HRESULT	GetJoyProperty( int nID, REFGUID rguidProp, LPDIPROPHEADER pdiph );	// �f�o�C�X���̎擾
public:
	CDirectInput();
	virtual ~CDirectInput();

protected:
	LPDIRECTINPUT7			pDI;							// DirectInput �I�u�W�F�N�g

	// �L�[�{�[�h
	LPDIRECTINPUTDEVICE7	pDIDKeyboard;
	BYTE					byKeyState[ 256 ];				// �u�ԏ�ԋL���p
	bool					bKeyPushDown[ 256 ];			// �C�x���g�L���p�G�L�[�� PushDown ������ true	�� Poll() ���ɍX�V�B
	bool					bKeyPullUp[ 256 ];				// �C�x���g�L���p�G�L�[�� PullUp ������ true		��       �V

	// �W���C�X�e�B�b�N
	int						nJoySticks;						// ���o���ꂽ�W���C�X�e�B�b�N�̐�
	bool					m_bNeedCleanupCOM;				// COM ���������������Ă���� true
	struct JoyStickInfo {
		DIDEVICEINSTANCE		instance;					// Joystick �C���X�^���X
		LPDIRECTINPUTDEVICE7	pDID;						// Joystick �f�o�C�X (NULL �Ȃ琶�����ɃG���[�����������Ƃ����������̃f�o�C�X�͎g���Ȃ��j
		DIJOYSTATE2				state;						// �u�ԏ�ԋL���p
		bool					bPushDown[ 128 ];			// �C�x���g�L���p�G�{�^���� PushDown ������ true
		bool					bPullUp[ 128 ];				// �C�x���g�L���p�G�{�^���� PullUp   ������ true
		LONG					lMin[3],lMax[3];			// �l��([]=x,y,z��)
		bool					bAxisDown[2][6];			// ����ԋL���p
		int						nAxisDownIndex;				// �t���b�v�p
	} m_JoyStickInfo[ MAX_JOYSTICKS ];

protected:
	HRESULT	InitKeyboard( HWND hWnd );						// �L�[�{�[�h�f�o�C�X�̏�����
	HRESULT	InitJoySticks( HWND hWnd );						// �W���C�X�e�B�b�N�f�o�C�X�̏�����
	bool	Poll�L�[�{�[�h();								// �L�[�{�[�h��ԍX�V�i���͂̃|�[�����O�j
	bool	Poll�W���C�X�e�B�b�N();							// �W���C�X�e�B�b�N��ԍX�V�i���͂̃|�[�����O�j

public:
	// �W���C�X�e�B�b�N�񋓃R�[���o�b�N�֐�;
	// �����ł� DIDEVICEINSTANCE ���Ђ�����R�s�[���邾���B
	// �f�o�C�X�����Ȃǂ̕K�v�ȏ�����InitJoysticks() �̒��ł��B
	BOOL	EnumJoySticksCallback( LPCDIDEVICEINSTANCE lpddi );
};

	}//IO
}//FDK

using namespace FDK::IO;
