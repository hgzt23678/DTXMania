#include "stdafx.h"
#include "CDirectInput.h"
#include "FDKError.h"

namespace FDK {
	namespace IO {

// JOYSTICK�R�[���o�b�N�֐�
static BOOL CALLBACK EnumJoySticksCallback( LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef ) {
	return ((CDirectInput*) pvRef)->EnumJoySticksCallback( lpddi );
}
CDirectInput::CDirectInput()
{
	// COM �̏�����
	m_bNeedCleanupCOM	= SUCCEEDED( CoInitialize( NULL ) ) ? true : false;
	this->pDI				= NULL;
	this->pDIDKeyboard		= NULL;
	this->nJoySticks		= 0;
}

CDirectInput::~CDirectInput()
{
	// COM �̏I���iCOM �̏������Ɏ��s���Ă���ꍇ�̓X�L�b�v�j
	if( m_bNeedCleanupCOM )
		CoUninitialize();
}

HRESULT CDirectInput::Init( HWND hWnd )
{
	HRESULT hr;

	// DirectInput7 �̐���
	if( FAILED( hr = DirectInputCreateEx( GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput7, (void**)&this->pDI, NULL ) ) )
		return hr;	// ���s

	// �L�[�{�[�h�f�o�C�X�̏�����
	if( FAILED( hr = InitKeyboard( hWnd ) ) )
	{
		Term();
		return hr;	// ���s
	}

	// �W���C�X�e�B�b�N�f�o�C�X�̏�����
	if( FAILED( hr = InitJoySticks( hWnd ) ) )
	{
		Term();
		return hr;	//	���s
	}

	return S_OK;
}

HRESULT CDirectInput::InitKeyboard( HWND hWnd )
{
	HRESULT hr;

	// �����ϐ��̏�����
	for( int i = 0; i < 256; i++ )
	{
		this->byKeyState[i] = 0x00;
		this->bKeyPushDown[i] = false;
		this->bKeyPullUp[i] = false;
	}

	// �L�[�{�[�h�f�o�C�X�̐���
	if( FAILED( hr = this->pDI->CreateDeviceEx( GUID_SysKeyboard, IID_IDirectInputDevice7, (LPVOID*)&this->pDIDKeyboard, NULL ) ) )
	{
		this->pDIDKeyboard = NULL;
		return hr;		// ���s
	}
	
	// �L�[�{�[�h�̃f�[�^�`���̐ݒ�
	if( FAILED( hr = this->pDIDKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
	{
		SAFE_RELEASE( this->pDIDKeyboard );
		return hr;		// ���s
	}

	// �L�[�{�[�h�̋������x���̐ݒ�
	// WindowNT�œ��삳����ɂ́A������ DISCL_FOREGROUND | DISCL_NONEXCLUSIVE �łȂ��Ă͂����Ȃ��B
	if( FAILED( hr = this->pDIDKeyboard->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY ) ) )
	{
		SAFE_RELEASE( this->pDIDKeyboard );
		return hr;		// ���s
	}

	// �L�[�{�[�h�̃C�x���g�擾�p�o�b�t�@�T�C�Y MAX_DIEVENT_BUFFER ��ݒ�B
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize       = sizeof( DIPROPDWORD );
	dipdw.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = MAX_DIEVENT_BUFFER;
	if( FAILED( hr = this->pDIDKeyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
	{
		SAFE_RELEASE( this->pDIDKeyboard );
		return hr;		// ���s
	}

	// Acquire
	//this->pDIDKeyboard->Acquire();		Acquire �͍s�킸�ɏI���B�i�� Init() ��Ƀv���p�e�B��ݒ�ł���B�j

	return S_OK;
}

HRESULT CDirectInput::InitJoySticks( HWND hWnd )
{
	HRESULT hr;

	// �W���C�X�e�B�b�N�̗�
	this->nJoySticks = 0;
	if( FAILED( hr = this->pDI->EnumDevices( DIDEVTYPE_JOYSTICK, FDK::IO::EnumJoySticksCallback, (void*)this, DIEDFL_ATTACHEDONLY ) ) )
		return hr;	// ���s

	// ���ꂼ��̃W���C�X�e�B�b�N�f�o�C�X�ɂ���...
	for( int i = 0; i < this->nJoySticks; i++ )
	{
		// ������
		ZeroMemory( (LPVOID) &m_JoyStickInfo[i].state, sizeof( DIJOYSTATE2 ) );
		for( int j = 0; j < 128; j++ ) {
			m_JoyStickInfo[i].bPushDown[j] = false;
			m_JoyStickInfo[i].bPullUp[j] = false;
		}
		for( int j = 0; j < 3; j++ )
			m_JoyStickInfo[i].lMin[j] = m_JoyStickInfo[i].lMax[j] = 0;
		
		for( int j = 0; j < 6; j++ ) {
			m_JoyStickInfo[i].bAxisDown[0][j] = false;
			m_JoyStickInfo[i].bAxisDown[1][j] = false;
		}
		m_JoyStickInfo[i].nAxisDownIndex = 0;

		// �W���C�X�e�B�b�N�f�o�C�X�̐���
		if( FAILED( hr = this->pDI->CreateDeviceEx( m_JoyStickInfo[i].instance.guidInstance, IID_IDirectInputDevice7, (LPVOID*)&(m_JoyStickInfo[i].pDID), NULL ) ) )
		{
			m_JoyStickInfo[i].pDID = NULL;
			continue;		// ���f�������̃W���C�X�e�B�b�N�֑J��
		}

		// �W���C�X�e�B�b�N�̃f�[�^�`���̐ݒ�
		if( FAILED( hr = m_JoyStickInfo[i].pDID->SetDataFormat( &c_dfDIJoystick2 ) ) )
		{
			SAFE_RELEASE( m_JoyStickInfo[i].pDID );
			continue;		// ���f�������̃W���C�X�e�B�b�N�֑J��
		}

		// �W���C�X�e�B�b�N�̋������x���̐ݒ�
		if( FAILED( hr = m_JoyStickInfo[i].pDID->SetCooperativeLevel( hWnd, DISCL_BACKGROUND | DISCL_EXCLUSIVE ) ) )
		{
			SAFE_RELEASE( m_JoyStickInfo[i].pDID );
			continue;		// ���f�������̃W���C�X�e�B�b�N�֑J��
		}

		// �C�x���g�擾�p�o�b�t�@�T�C�Y�̐ݒ�
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize       = sizeof( DIPROPDWORD );
		dipdw.diph.dwHeaderSize = sizeof( DIPROPHEADER );
		dipdw.diph.dwObj        = 0;
		dipdw.diph.dwHow        = DIPH_DEVICE;
		dipdw.dwData            = MAX_DIEVENT_BUFFER;
		if( FAILED( hr = m_JoyStickInfo[i].pDID->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
		{
			SAFE_RELEASE( m_JoyStickInfo[i].pDID );
			continue;		// ���f�������̃W���C�X�e�B�b�N�֑J��
		}
 
		// X,Y,Z���̒l��̎擾
		for( int j = 0; j < 3; j++ )
		{
			static const DWORD dwObj[3] = { DIJOFS_X, DIJOFS_Y, DIJOFS_Z };
			DIPROPRANGE dipr;
			dipr.diph.dwSize = sizeof( DIPROPRANGE );
			dipr.diph.dwHeaderSize = sizeof( DIPROPHEADER );
			dipr.diph.dwObj = dwObj[j];
			dipr.diph.dwHow = DIPH_BYOFFSET;
			dipr.lMin = dipr.lMax = 0;
			if( SUCCEEDED( m_JoyStickInfo[i].pDID->GetProperty( DIPROP_RANGE, &dipr.diph ) ) ) {
				m_JoyStickInfo[i].lMin[j] = dipr.lMin;
				m_JoyStickInfo[i].lMax[j] = dipr.lMax;
			}
		}

		// Acquire
		//m_JoyStickInfo[i].pDID->Acquire();	//		Acquire �͍s��Ȃ��B�i�� Init() ��Ƀv���p�e�B��ݒ�ł���B�j
	}

	return S_OK;
}

BOOL CDirectInput::EnumJoySticksCallback( LPCDIDEVICEINSTANCE lpddi )
{
	if( this->nJoySticks >= MAX_JOYSTICKS )
		return DIENUM_STOP;		// ��������

	CopyMemory( &m_JoyStickInfo[ this->nJoySticks ].instance, lpddi, sizeof( DIDEVICEINSTANCE ) );
	this->nJoySticks ++;

	return DIENUM_CONTINUE;
}

bool CDirectInput::Poll()
{
	bool bRet = true;
	
	if( ! Poll�L�[�{�[�h() )
		bRet = false;

	if(  ! Poll�W���C�X�e�B�b�N() )
		bRet = false;

	return bRet;
}


bool CDirectInput::Poll�L�[�{�[�h()
{
	// ��ԃN���A
	for( int i = 0; i < 256; i++ ) 
	{
		this->bKeyPullUp[i] = false;
		this->bKeyPushDown[i] = false;
	}

	if( ! this->pDIDKeyboard )
		return true;	// ��������

	// Acquire ���Ă݂�
	HRESULT hr = this->pDIDKeyboard->Acquire();
	if( hr != DI_OK && hr != S_FALSE )		// ��������� DI_OK, �擾�ς݂Ȃ� S_FALSE ���Ԃ�B
		return false;

	// �|�[�����O
	if( FAILED( this->pDIDKeyboard->Poll() ) )
		return false;

	// 1.��Ԏ擾
	if( FAILED( this->pDIDKeyboard->GetDeviceState( 256, this->byKeyState ) ) )
		ZeroMemory( this->byKeyState, 256 );		// ��Ԏ擾�Ɏ��s������SOFF����

	// 2.�C�x���g�擾
	DIDEVICEOBJECTDATA ddod[ MAX_DIEVENT_BUFFER ];
	DWORD dwItems = MAX_DIEVENT_BUFFER;
	if( SUCCEEDED( this->pDIDKeyboard->GetDeviceData( sizeof( DIDEVICEOBJECTDATA ), ddod, &dwItems, 0 ) ) )
	{
		for( DWORD i = 0; i < dwItems; i++ )
		{
			if( ( ddod[i].dwData & 0xFF ) == 0 )
				this->bKeyPullUp[ ddod[i].dwOfs ] = true;		// �L�[�������ꂽ
			else
				this->bKeyPushDown[ ddod[i].dwOfs ] = true;		// �L�[�������ꂽ
		}
	}
	return true;
}

bool CDirectInput::Poll�W���C�X�e�B�b�N()
{
	for( int i = 0; i < this->nJoySticks; i++ )
	{
		// ��ԃN���A
		for( int j = 0; j < 128; j++ ) 
		{
			m_JoyStickInfo[i].bPushDown[j] = false;
			m_JoyStickInfo[i].bPullUp[j] = false;
		}

		if( ! m_JoyStickInfo[i].pDID )
			continue;

		// Acquire ���Ă݂�
		HRESULT hr = m_JoyStickInfo[i].pDID->Acquire();
		if( hr != DI_OK && hr != S_FALSE )		// ��������� DI_OK, �擾�ς݂Ȃ� S_FALSE ���Ԃ�B
			continue;

		// �|�[�����O
		if( FAILED( m_JoyStickInfo[i].pDID->Poll() ) )
			continue;

		// 1. ��Ԏ擾
		if( FAILED( m_JoyStickInfo[i].pDID->GetDeviceState( sizeof( DIJOYSTATE2 ), &m_JoyStickInfo[i].state ) ) )
			ZeroMemory( &m_JoyStickInfo[i].state, sizeof(DIJOYSTATE2) );			// ��Ԏ擾�Ɏ��s������S�{�^��OFF����
		m_JoyStickInfo[i].nAxisDownIndex = 1 - m_JoyStickInfo[i].nAxisDownIndex;	// ����Ԃ̃C�x���g�`�F�b�N
		for( int k = 0; k < 6; k++ )
			m_JoyStickInfo[i].bAxisDown[ m_JoyStickInfo[i].nAxisDownIndex ][ k ] = IsAxisDown( i, k );

		// 2.�C�x���g�擾
		DIDEVICEOBJECTDATA ddod[ MAX_DIEVENT_BUFFER ];
		DWORD dwItems = MAX_DIEVENT_BUFFER;
		if( SUCCEEDED( m_JoyStickInfo[i].pDID->GetDeviceData( sizeof( DIDEVICEOBJECTDATA ), ddod, &dwItems, 0 ) ) )
		{
			for( DWORD ev = 0; ev < dwItems; ev++ )
			{
				for( int k = 0; k < 128; k++ )
				{
					if( ddod[ ev ].dwOfs == DIJOFS_BUTTON( k ) )
					{
						if( ( ddod[ ev ].dwData & 0xFF ) == 0 )
							m_JoyStickInfo[i].bPullUp[ k ] = true;			// �{�^���������ꂽ
						else
							m_JoyStickInfo[i].bPushDown[ k ] = true;		// �{�^���������ꂽ
					}
				}
			}
		}
	}
	return true;
}

void CDirectInput::Term()
{
	// �L�[�{�[�h�f�o�C�X�̉��
	if( this->pDIDKeyboard )
	{
		this->pDIDKeyboard->Unacquire();
		SAFE_RELEASE( this->pDIDKeyboard );
	}

	// �W���C�X�e�B�b�N�f�o�C�X�̉��
	for( int i = 0; i < this->nJoySticks; i++ )
	{
		if( m_JoyStickInfo[i].pDID )
		{
			m_JoyStickInfo[i].pDID->Unacquire();
			SAFE_RELEASE( m_JoyStickInfo[i].pDID );
		}
	}
	this->nJoySticks = 0;

	// DirectInput7 �̉��
	SAFE_RELEASE( this->pDI );
}

bool CDirectInput::IsKeyDown( int nDIK )
{
	return ( nDIK >= 0 && nDIK < 256 && (this->byKeyState[ nDIK ] & 0x80) != 0x00 );
}

bool CDirectInput::IsKeyUp( int nDIK )
{
	return ( nDIK >= 0 && nDIK < 256 && (this->byKeyState[ nDIK ] & 0x80) == 0x00 );
}

bool CDirectInput::IsKeyPushDown( int nDIK )
{
	return ( nDIK >= 0 && nDIK < 256 && this->bKeyPushDown[ nDIK ] );
}

bool CDirectInput::IsKeyPullUp( int nKey )
{
	return ( nKey >= 0 && nKey < 256 && this->bKeyPullUp[ nKey ] );
}

bool CDirectInput::IsButtonDown( int nID, int nButton )
{
	if( nID < 0 || nID >= this->nJoySticks || nButton < 0 || nButton > 127 || m_JoyStickInfo[ nID ].pDID == NULL )
		return false;

	return ( ( m_JoyStickInfo[ nID ].state.rgbButtons[ nButton ] & 0x80 ) != 0x00 );
}

bool CDirectInput::IsButtonUp( int nID, int nButton )
{
	if( nID < 0 || nID >= this->nJoySticks || nButton < 0 || nButton > 127 || m_JoyStickInfo[ nID ].pDID == NULL )
		return false;

	return ( ( m_JoyStickInfo[ nID ].state.rgbButtons[ nButton ] & 0x80 ) == 0x00 );
}

bool CDirectInput::IsButtonPushDown( int nID, int nButton )
{
	return ( nID >= 0 && nID < this->nJoySticks && nButton >= 0 && nButton < 128 && m_JoyStickInfo[ nID ].bPushDown[ nButton ] );
}

bool CDirectInput::IsButtonPullUp( int nID, int nButton )
{
	return ( nID >= 0 && nID < this->nJoySticks && nButton >= 0 && nButton < 128 && m_JoyStickInfo[ nID ].bPullUp[ nButton ] );
}

bool CDirectInput::IsAxisDown( int nID, int nAxis )
{
	if( nID < 0 || nID >= this->nJoySticks || nAxis < 0 || nAxis > 5 || m_JoyStickInfo[ nID ].pDID == NULL )
		return false;

	if( nAxis == 0 && m_JoyStickInfo[ nID ].state.lX < ( 3 * m_JoyStickInfo[ nID ].lMin[0] +     m_JoyStickInfo[ nID ].lMax[0] ) / 4 ) return true;
	if( nAxis == 1 && m_JoyStickInfo[ nID ].state.lX > (     m_JoyStickInfo[ nID ].lMin[0] + 3 * m_JoyStickInfo[ nID ].lMax[0] ) / 4 ) return true;
	if( nAxis == 2 && m_JoyStickInfo[ nID ].state.lY < ( 3 * m_JoyStickInfo[ nID ].lMin[1] +     m_JoyStickInfo[ nID ].lMax[1] ) / 4 ) return true;
	if( nAxis == 3 && m_JoyStickInfo[ nID ].state.lY > (     m_JoyStickInfo[ nID ].lMin[1] + 3 * m_JoyStickInfo[ nID ].lMax[1] ) / 4 ) return true;
	if( nAxis == 4 && m_JoyStickInfo[ nID ].state.lZ < ( 3 * m_JoyStickInfo[ nID ].lMin[2] +     m_JoyStickInfo[ nID ].lMax[2] ) / 4 ) return true;
	if( nAxis == 5 && m_JoyStickInfo[ nID ].state.lZ > (     m_JoyStickInfo[ nID ].lMin[2] + 3 * m_JoyStickInfo[ nID ].lMax[2] ) / 4 ) return true;
	return false;
}

bool CDirectInput::IsAxisUp( int nID, int nAxis )
{
	if( nID < 0 || nID >= this->nJoySticks || nAxis < 0 || nAxis > 5 || m_JoyStickInfo[ nID ].pDID == NULL )
		return false;

	if( nAxis == 0 && m_JoyStickInfo[ nID ].state.lX >= ( 3 * m_JoyStickInfo[ nID ].lMin[0] +     m_JoyStickInfo[ nID ].lMax[0] ) / 4 ) return true;
	if( nAxis == 1 && m_JoyStickInfo[ nID ].state.lX <= (     m_JoyStickInfo[ nID ].lMin[0] + 3 * m_JoyStickInfo[ nID ].lMax[0] ) / 4 ) return true;
	if( nAxis == 2 && m_JoyStickInfo[ nID ].state.lY >= ( 3 * m_JoyStickInfo[ nID ].lMin[1] +     m_JoyStickInfo[ nID ].lMax[1] ) / 4 ) return true;
	if( nAxis == 3 && m_JoyStickInfo[ nID ].state.lY <= (     m_JoyStickInfo[ nID ].lMin[1] + 3 * m_JoyStickInfo[ nID ].lMax[1] ) / 4 ) return true;
	if( nAxis == 4 && m_JoyStickInfo[ nID ].state.lZ >= ( 3 * m_JoyStickInfo[ nID ].lMin[2] +     m_JoyStickInfo[ nID ].lMax[2] ) / 4 ) return true;
	if( nAxis == 5 && m_JoyStickInfo[ nID ].state.lZ <= (     m_JoyStickInfo[ nID ].lMin[2] + 3 * m_JoyStickInfo[ nID ].lMax[2] ) / 4 ) return true;
	return false;
}

bool CDirectInput::IsAxisPushDown( int nID, int nAxis )
{
	if( nID < 0 || nID >= this->nJoySticks || nAxis < 0 || nAxis > 5 || m_JoyStickInfo[ nID ].pDID == NULL )
		return false;

	return     ! m_JoyStickInfo[ nID ].bAxisDown[ 1 - m_JoyStickInfo[ nID ].nAxisDownIndex ][ nAxis ]
			&&   m_JoyStickInfo[ nID ].bAxisDown[     m_JoyStickInfo[ nID ].nAxisDownIndex ][ nAxis ];
}

bool CDirectInput::IsAxisPullUp( int nID, int nAxis )
{
	if( nID < 0 || nID >= this->nJoySticks || nAxis < 0 || nAxis > 5 || m_JoyStickInfo[ nID ].pDID == NULL )
		return false;

	return       m_JoyStickInfo[ nID ].bAxisDown[ 1 - m_JoyStickInfo[ nID ].nAxisDownIndex ][ nAxis ]
			&& ! m_JoyStickInfo[ nID ].bAxisDown[     m_JoyStickInfo[ nID ].nAxisDownIndex ][ nAxis ];
}

LPDIJOYSTATE2 CDirectInput::GetJoyState( int nID )
{
	if( nID < 0 || nID >= this->nJoySticks || m_JoyStickInfo[ nID ].pDID == NULL )
		return NULL;

	return &(m_JoyStickInfo[ nID ].state);
}

HRESULT	CDirectInput::GetJoyProperty( int nID, REFGUID rguidProp, LPDIPROPHEADER pdiph )
{
	if( nID < 0 || nID >= this->nJoySticks || m_JoyStickInfo[ nID ].pDID == NULL )
		return S_FALSE;

	return m_JoyStickInfo[ nID ].pDID->GetProperty( rguidProp, pdiph );
}

	}//IO
}//FDK