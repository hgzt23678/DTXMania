#pragma once

namespace FDK {
	namespace Graphics {

// �w�肳�ꂽ PNG �t�@�C����ǂݍ��݁ADDS �`���ɕϊ����ĕԂ��B
// �@pddsd2 ...... DDSURFACEDESC2 �̒l���ݒ肳��ĕԂ����B
// �@pppbyData ... image[][] ���Ԃ����B�s�v�ɂȂ�� free() ���邱�ƁB
// �@�@�@�@�@�@�@�@�i�s�P�ʂ� free() ���邱�ƁI�j
extern bool LoadPNGtoDDS( LPCTSTR fileName, DDSURFACEDESC2* pddsd2, BYTE*** pppbyData );
extern bool LoadPNGtoDDS( FILE* fp, DDSURFACEDESC2* pddsd2, BYTE*** pppbyData );

// �w�肳�ꂽ PNG �f�[�^����ADDS �`���ɕϊ����ĕԂ��B
// �@pData ....... PNG �f�[�^�i�t�@�C���C���[�W�j�ւ̃|�C���^
// �@dwDataSize .. PNG �f�[�^�i�t�@�C���C���[�W�j�̃T�C�Y[bytes]
// �@pddsd2 ...... DDSURFACEDESC2 �̒l���ݒ肳��ĕԂ����B
// �@pppbyData ... image[][] ���Ԃ����B�s�v�ɂȂ�� free() ���邱�ƁB
// �@�@�@�@�@�@�@�@�i�s�P�ʂ� free() ���邱�ƁI�j
extern bool LoadPNGtoDDS( BYTE* pData, DWORD dwDataSize, DDSURFACEDESC2* pddsd2, BYTE*** pppbyData );

// PNG ��ǂݍ��݁ADIB �ɕϊ����ĕԂ��B
// PNG �̃��̓s�N�Z���̐F�ƍ�������ĂȂ��Ȃ�B
extern bool LoadPNGtoDIB( LPCTSTR fileName, BITMAPINFO** ppInfo, BYTE** ppBMP );
extern bool LoadPNGtoDIB( FILE* fp, BITMAPINFO** ppInfo, BYTE** ppBMP );

// PNG ���������C���[�W����ǂݍ��݁ADIB �ɕϊ����ĕԂ��B
// PNG �̃��̓s�N�Z���̐F�ƍ�������ĂȂ��Ȃ�B
extern bool LoadPNGtoDIB( BYTE* pData, DWORD dwDataSize, BITMAPINFO** ppInfo, BYTE** ppBMP );

	}//Graphics
}//FDK

using namespace FDK::Graphics;
