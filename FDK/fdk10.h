
// fdk9.h
// FDK9 ���g���A�v���͂���� include ����B

#pragma once

#include "stdafx.h"

// libcmt.lib ���g���i�ÓI�����N�j
#pragma comment(linker, "/NODEFAULTLIB:libc.lib")		// �V���O���X���b�h
#pragma comment(linker, "/NODEFAULTLIB:libcd.lib")
#pragma comment(linker, "/NODEFAULTLIB:msvcrt.lib")		// �}���`�X���b�hDLL
#pragma comment(linker, "/NODEFAULTLIB:msvcrtd.lib")

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dinput.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msacm32.lib")
#pragma comment(lib, "version.lib")		// GetDXVersion.cpp �Ŏg�p
#pragma comment(lib, "xadec.lib")
#pragma comment(lib, "libjpeg.lib")
#ifdef _DEBUG
 #pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
 #pragma comment(lib, "d3dxd.lib")
 #pragma comment(lib, "zlibd.lib")
 #pragma comment(lib, "libpngd.lib")
 #pragma comment(lib, "ogg_static_d.lib")
 #pragma comment(lib, "vorbis_static_d.lib")
 #pragma comment(lib, "vorbisfile_static_d.lib")
 #pragma comment(lib, "fdk10d.lib")
#else
 #pragma comment(linker, "/NODEFAULTLIB:libcmtd.lib")
 #pragma comment(lib, "d3dx.lib")
 #pragma comment(lib, "zlib.lib")
 #pragma comment(lib, "libpng.lib")
 #pragma comment(lib, "ogg_static.lib")
 #pragma comment(lib, "vorbis_static.lib")
 #pragma comment(lib, "vorbisfile_static.lib")
 #pragma comment(lib, "fdk10.lib")
#endif
