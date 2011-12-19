#pragma once

namespace FDK {
	namespace AppBase {

//--------------------------------------------------------------------------
// Name: D3DModeInfo
// Desc: ���[�h���
//--------------------------------------------------------------------------

typedef DDSURFACEDESC2	D3DModeInfo;

//--------------------------------------------------------------------------
// Name: D3DDeviceInfo
// Desc: �f�o�C�X���i�R�c�̎g������̂̂݁j
//--------------------------------------------------------------------------

enum D3DAPPDEV {
	D3DAPPDEV_HALTnL,
	D3DAPPDEV_HAL,
	D3DAPPDEV_HEL,
	D3DAPPDEV_REF
};

struct D3DDeviceInfo
{
	// �f�o�C�X���
	GUID			guidDevice;				// GUID
	GUID*			pDeviceGUID;			// guidDevice �ւ̃|�C���^�i���邢��NULL�j
	D3DAPPDEV		type;					// D3DAPPDEV_...
	bool			bHardware;				// type �� HAL or HALTnL �Ȃ� true
	D3DDEVICEDESC7	ddDeviceDesc;			// �f�o�C�X�\��
	TCHAR			strDesc[40];			// �f�o�C�X��
	bool			bStereoCompatible;		// �����t���b�s���O���K�v�Ȃ� true
	
	// ���[�h���X�g�iD3DAdapterInfo::modes[] ���炱�̂R�c�f�C�o�X�ɑΉ����Ă�����̂̂ݔ����������X�g�j
	D3DModeInfo		modes[150];				// ���[�h�z��
	DWORD			dwNumModes;				// ���[�h��
	
	// �X�e�[�^�X
	bool			bWindowed;				// ���݂̃E�B���h�E�^�S��ʃ��[�h
	D3DModeInfo*	pMode;					// ���݂̃��[�h�i�S��ʎ��j
};

//--------------------------------------------------------------------------
// Name: D3DAdapterInfo
// Desc: �A�_�v�^���iDirectDraw���j
//--------------------------------------------------------------------------

struct D3DAdapterInfo 
{
	// �A�_�v�^���
	GUID			guidAdapter;			// GUID
	GUID*			pAdapterGUID;			// guidAdapter �ւ̃|�C���^�i���邢��NULL�j
	TCHAR			strDriver[MAX_DDDEVICEID_STRING];	// �A�_�v�^��
	TCHAR			strDesc[40];			// �A�_�v�^�̐���
	DWORD			dwTotalMemory;			// �S�e��
	DDCAPS			ddHALCaps;				// HAL �̔\��
	DDCAPS			ddHELCaps;				// HEL �̔\��
	bool			bDesktopCompatible;		// �f�X�N�g�b�v�݊��Ȃ� true

	// DirectDraw �̑Ή�����S���[�h�i�S�f�o�C�X�̍ŏ����{���j
	D3DModeInfo		modes[150];				// ���[�h���X�g
	DWORD			dwNumModes;				// ���[�h��

	// �f�o�C�X���X�g
	D3DDeviceInfo	devices[5];				// �f�o�C�X�z��
	DWORD			dwNumDevices;			// �f�o�C�X��

	// �X�e�[�^�X
	D3DDeviceInfo*	pDevice;				// ���݂̃f�o�C�X
};

//--------------------------------------------------------------------------
// Name: CD3DDisplay
// Desc: �A�_�v�^�^�f�o�C�X�^���[�h�̊Ǘ��N���X
//--------------------------------------------------------------------------

class CD3DDisplay
{
// ���唼�̃����o���R�[���o�b�N�֐�����A�N�Z�X����邽�߁Apublic �ɂȂ��Ă���B
public:
	D3DAdapterInfo	adapters[20];			// �A�_�v�^�z��
	DWORD			dwNumAdapters;			// �A�_�v�^��
	D3DAdapterInfo*	pAdapter;				// ���݂̃A�_�v�^

public:
	HRESULT	EnumerateDevices();							// �A�_�v�^�^�f�o�C�X�^���[�h�̗񋓁B
	bool	FindMode( DWORD w, DWORD h, DWORD bpp );	// ���݂̃f�o�C�X����w�肳�ꂽ���[�h�iw�~h�~bpp�j���������A������Ԃ��Z�b�g����B

	virtual HRESULT	ConfirmDevice( DDCAPS* pddHALCaps, D3DDEVICEDESC7* pd3dd )	{ return S_OK; }    // �R�c�f�o�C�X�̔��f�i�I�[�o�[���C�h�p�j

public:
	CD3DDisplay();
	virtual ~CD3DDisplay() {};
};

	}//AppBase
}//FDK

using namespace FDK::AppBase;
