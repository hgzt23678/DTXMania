#pragma once

namespace FDK {
	namespace Graphics {

		// �w�肳�ꂽ JPEG �t�@�C����ǂݍ��݁ADIB �`���ɕϊ����ĕԂ��B
		// �@lppInfo ... BITMAPINFO �ւ̃A�h���X���Ԃ����B�s�v�ɂȂ�� free() ���邱�ƁB
		// �@lppBMP .... �摜�f�[�^���Ԃ����B�s�v�ɂȂ�� free() ���邱�ƁB
		extern int LoadJPEG( FILE* fp, BITMAPINFO** lppInfo, BYTE** lppBMP );

	}//Graphics
}//FDK

using namespace FDK::Graphics;
