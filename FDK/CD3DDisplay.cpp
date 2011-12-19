#include "stdafx.h"
#include "CD3DDisplay.h"
#include "FDKError.h"

namespace FDK {
	namespace AppBase {

// Direct3D �f�o�C�X�񋓗p�̃R�[���o�b�N�֐��B
static HRESULT WINAPI DeviceEnumCallback( TCHAR* strDesc, TCHAR* strName, D3DDEVICEDESC7* pDesc, VOID* pContext )
{
	CD3DDisplay* app		= (CD3DDisplay*) pContext;
	D3DAdapterInfo* adapter	= app->pAdapter;
	D3DDeviceInfo* device	= adapter->pDevice;

	// �f�o�C�X�����R�s�[����B
	ZeroMemory( device, sizeof(D3DDeviceInfo) );
	device->guidDevice			= pDesc->deviceGUID;
	device->pDeviceGUID			= &device->guidDevice;
	device->bHardware			= (pDesc->dwDevCaps & D3DDEVCAPS_HWRASTERIZATION) ? true : false;
	memcpy( &device->ddDeviceDesc, pDesc, sizeof(D3DDEVICEDESC7) );
	if( adapter->pAdapterGUID )
		lstrcpyn( device->strDesc, adapter->strDesc, 39 );							// strDesc[]
	else
		lstrcpyn( device->strDesc, strName, 39 );
	if( device->bHardware )															// type
		device->type = ( device->guidDevice == IID_IDirect3DTnLHalDevice ) ? D3DAPPDEV_HALTnL : D3DAPPDEV_HAL;
	else
		device->type = ( device->guidDevice == IID_IDirect3DRefDevice ) ? D3DAPPDEV_REF : D3DAPPDEV_HEL;
	device->bStereoCompatible = false;					// bStereoCompatible�i��ōă`�F�b�N����j
	device->dwNumModes = 0;								// dwNumModes
	device->bWindowed = adapter->bDesktopCompatible;	// bWindowed�i�����ŃE�B���h�E�ɂȂ��Ȃ�true�j
	device->pMode = &(device->modes[0]);				// pMode�i���640x480�̃��[�h��ݒ肷��j

	// �d������G�Z�J���_���A�_�v�^�ȍ~�ɂ��ẮAHAL/HALTnL �̂݃��X�g�A�b�v����B
    if( adapter->pAdapterGUID != NULL && ! device->bHardware )
		return D3DENUMRET_OK;
	
    // �A�v���ɂ��̃f�o�C�X�̋��^�s���𔻒f������B
    if( FAILED( app->ConfirmDevice( &adapter->ddHALCaps, &device->ddDeviceDesc ) ) )
		return D3DENUMRET_OK;
	
	// DirectDraw �̑S���[�h���X�g�iAdapter��modes[]�j�̂����ADirect3D �f�o�C�X�ƌ݊����̂��郂�[�h�𔲐��������X�g���쐬����B
    for( DWORD i=0; i<adapter->dwNumModes; i++ )
    {
        DDSURFACEDESC2 ddsdDDMode	= adapter->modes[i];
        DWORD dwDDDepth				= ddsdDDMode.ddpfPixelFormat.dwRGBBitCount;			// �A�_�v�^�̐[�x
        DWORD dwD3DDepths			= device->ddDeviceDesc.dwDeviceRenderBitDepth;		// D3D�f�o�C�X�̐[�x

		// Direct3D �f�o�C�X�̃��[�h�ƌ݊����̂��郂�[�h������F�߂�B
        if( ( ( dwDDDepth == 32 ) && ( dwD3DDepths & DDBD_32 ) ) ||
            ( ( dwDDDepth == 24 ) && ( dwD3DDepths & DDBD_24 ) ) ||
            ( ( dwDDDepth == 16 ) && ( dwD3DDepths & DDBD_16 ) ) )
        {
			// ���[�h���f�o�C�X���[�h�̃��X�g�ɃR�s�[����B
            device->modes[device->dwNumModes++] = ddsdDDMode;

			// �X�e���I�t���b�s���O�`�F�[���ł��邩���L�^����B�i���[�h���P�ł������Ȃ� trur �ɂȂ�j
            if( ddsdDDMode.ddsCaps.dwCaps2 & DDSCAPS2_STEREOSURFACELEFT )
                device->bStereoCompatible = true;
        }
    }

	// �f�o�C�X�����[�h���P���T�|�[�g���Ȃ��Ȃ炱���Ŏߕ��B
    if( device->dwNumModes == 0 )
        return D3DENUMRET_OK;

	// �f�t�H���g�̑S��ʃ��[�h�Ƃ��� 640x480x16 ���[�h����������B
	for( DWORD i=0; i < device->dwNumModes; i++ )
	{
		if( ( device->modes[i].dwWidth == 640 ) &&
			( device->modes[i].dwHeight == 480 ) &&
			( device->modes[i].ddpfPixelFormat.dwRGBBitCount == 16 ) )
		{
			device->pMode = &( device->modes[i] );
			break;
		}
	}

	// �f�o�C�X��F�߁A�߂�B
    if( adapter->dwNumDevices < 4 )
	{
		adapter->dwNumDevices++;
		adapter->pDevice++;
	}

    return D3DENUMRET_OK;
}

// DirectDraw �̃t���X�N���[�����[�h�񋓗p�̃R�[���o�b�N�֐��B
static HRESULT WINAPI ModeEnumCallback( DDSURFACEDESC2* pddsd, VOID* pContext )
{
	D3DAdapterInfo* adapter = (D3DAdapterInfo*) pContext;
	adapter->modes[ adapter->dwNumModes ] = (*pddsd);
	if( adapter->dwNumModes < 149 ) adapter->dwNumModes++;
    return DDENUMRET_OK;
}

// �f�B�X�v���C���[�h���Ƀ\�[�g���邽�߂̃R�[���o�b�N�֐��B
static int SortModesCallback( const VOID* arg1, const VOID* arg2 )
{
    DDSURFACEDESC2* p1 = (DDSURFACEDESC2*)arg1;
    DDSURFACEDESC2* p2 = (DDSURFACEDESC2*)arg2;

    if( p1->dwWidth < p2->dwWidth )
        return -1;
    if( p1->dwWidth > p2->dwWidth )
        return +1;

    if( p1->dwHeight < p2->dwHeight )
        return -1;
    if( p1->dwHeight > p2->dwHeight )
        return +1;

    if( p1->ddpfPixelFormat.dwRGBBitCount < p2->ddpfPixelFormat.dwRGBBitCount )
        return -1;
    if( p1->ddpfPixelFormat.dwRGBBitCount > p2->ddpfPixelFormat.dwRGBBitCount )
        return +1;

    return 0;
}

// �A�_�v�^�񋓗p�̃R�[���o�b�N�֐��B
static BOOL WINAPI AdapterEnumCallback( GUID* pGUID, TCHAR* strDesc, TCHAR* strName, VOID* pContext )
{
    LPDIRECTDRAW7	pDD;
    LPDIRECT3D7		pD3D;
    HRESULT			hr;
	TCHAR			strErrorMsg[2048];
	
	CD3DDisplay* app		= (CD3DDisplay*) pContext;
	D3DAdapterInfo* adapter = app->pAdapter;
	
    // GUID ���w�肵�� DirectDraw ���쐬����B
    hr = DirectDrawCreateEx( pGUID, (VOID**)&pDD, IID_IDirectDraw7, NULL );
	if( FAILED(hr) )
	{
		D3DXGetErrorString( hr, 2048, strErrorMsg );
        return D3DENUMRET_OK;	// ���s
    }
	
    // D3D�f�o�C�X��񋓂��邽�߂ɁADirect3D ���擾����B
    hr = pDD->QueryInterface( IID_IDirect3D7, (VOID**)&pD3D );
    if( FAILED(hr) )
	{
        pDD->Release();
		D3DXGetErrorString( hr, 2048, strErrorMsg );
        return D3DENUMRET_OK;	// ���s
	}

	// �A�_�v�^���\���̂փf�[�^���R�s�[����B
	ZeroMemory( adapter, sizeof(D3DAdapterInfo) );
    if( pGUID )
	{
        adapter->guidAdapter = (*pGUID);				// �񋓂��I���� GUID �������ɂȂ�̂ŁAGUID �̖{�̂��ƍT���Ă����B
        adapter->pAdapterGUID = &adapter->guidAdapter;	// �Ƃ����Ă� GUID �� NULL �̏ꍇ������̂ŁA���ʂ̂��߂� GUID �ւ̃|�C���^���p�ӂ���B
    }
	DDDEVICEIDENTIFIER2 dddi;
	pDD->GetDeviceIdentifier( &dddi, 0 );
	lstrcpyn( adapter->strDriver, dddi.szDescription, 512 );	// strDriver[]
	lstrcpyn( adapter->strDesc, strDesc, 39 );					// strDesc[]
	DWORD dwTotal, dwFree;
	DDSCAPS2 ddsc;
	ZeroMemory( &ddsc, sizeof(ddsc) );
	ddsc.dwCaps = DDSCAPS_VIDEOMEMORY;
	pDD->GetAvailableVidMem( &ddsc, &dwTotal, &dwFree );
	adapter->dwTotalMemory = dwTotal;							// dwTotalMemory
	adapter->ddHALCaps.dwSize = sizeof(DDCAPS);
    adapter->ddHELCaps.dwSize = sizeof(DDCAPS);
    pDD->GetCaps( &adapter->ddHALCaps, &adapter->ddHELCaps );	// ddHALCaps, ddHELCaps
    if( adapter->ddHALCaps.dwCaps2 & DDCAPS2_CANRENDERWINDOWED )	// �E�B���h�E���[�h�̕`�悪�ł��A
        if( adapter->pAdapterGUID == NULL )							// �h���C�o�� GUID �� NULL �ł���Ȃ�
            adapter->bDesktopCompatible = true;						// �f�X�N�g�b�v�݊��A�_�v�^�ł���B

	adapter->dwNumModes = 0;						// dwNumModes
	adapter->dwNumDevices = 0;						// dwNumDevices
	adapter->pDevice = &(adapter->devices[0]);		// pDevice
	
	// DirectDraw �ŗ��p�\�ȑS���[�h��񋓂��A�\�[�g���ē����ɋL�����Ă����B
	// �� ���̃��[�h���X�g���W���Ƃ��āA�e�R�c�f�o�C�X�ŗ��p�\�ȃ��[�h���s�b�N�A�b�v����B
    pDD->EnumDisplayModes( 0, NULL, adapter, ModeEnumCallback );
    qsort( adapter->modes, adapter->dwNumModes, sizeof(DDSURFACEDESC2), SortModesCallback );

	// �R�c�f�o�C�X�����ׂė񋓂���B
    pD3D->EnumDevices( DeviceEnumCallback, app );

	// �f�t�H���g�f�o�C�X��I������B
	int nRank = -1;
	for( DWORD d=0; d<adapter->dwNumDevices; d++ )
	{
		if( adapter->devices[d].type == D3DAPPDEV_HALTnL )						// �ŗD��� HALTnL
		{
			nRank = 4;
			adapter->pDevice = &( adapter->devices[d] );
		}
		else if( adapter->devices[d].type == D3DAPPDEV_HAL && nRank < 4 )		// ���� HAL
		{
			nRank = 3;
			adapter->pDevice = &( adapter->devices[d] );
		}
		else if( adapter->devices[d].type == D3DAPPDEV_HEL && nRank < 3 )		// ���� HEL
		{
			nRank = 2;
			adapter->pDevice = &( adapter->devices[d] );
		}
		else
		{
			nRank = 1;
			adapter->pDevice = &( adapter->devices[d] );						// �Ō�ɂ��̑�
		}
	}
	if( nRank < 0 )
		adapter->pDevice = &( adapter->devices[0] );
	
	// �|�����Ď��̃A�_�v�^�ցB
    pD3D->Release();
    pDD->Release();
	if( app->dwNumAdapters < 19 )
	{
		app->dwNumAdapters ++;
		app->pAdapter ++;
	}

	return DDENUMRET_OK;
}

CD3DDisplay::CD3DDisplay()
{
	this->pAdapter = &(this->adapters[0]);
	this->dwNumAdapters = 0;
}

HRESULT CD3DDisplay::EnumerateDevices()
{
	// �񋓂���
    DirectDrawEnumerate( AdapterEnumCallback, this );

	if( this->dwNumAdapters == 0 )
		return FDKERR_�A�_�v�^���Ȃ�;

	// m_pAdapter �Ƀf�t�H���g�̃A�_�v�^�ւ̃|�C���^���Z�b�g����B
	for( DWORD a = 0; a < this->dwNumAdapters; a++ )
	{
		if( this->adapters[a].bDesktopCompatible )
		{
			this->pAdapter = &(this->adapters[a]);
			break;
		}
	}
	return S_OK;
}

bool	CD3DDisplay::FindMode( DWORD w, DWORD h, DWORD bpp )
{
	D3DAdapterInfo* a = this->pAdapter;
	D3DDeviceInfo*  d = a->pDevice;
	
	for( DWORD i = 0; i < d->dwNumModes; i++ )
	{
		if( ( d->modes[i].dwWidth == w ) &&
			( d->modes[i].dwHeight == h ) &&
			( d->modes[i].ddpfPixelFormat.dwRGBBitCount == bpp ) )
		{
			d->pMode = &( d->modes[i] );
			return true;		// ��������
		}
	}
	return false;	// �Ȃ�����
}


	}//AppBase
}//FDK
