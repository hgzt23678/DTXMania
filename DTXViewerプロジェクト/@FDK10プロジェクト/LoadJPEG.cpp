#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <setjmp.h>
#include "jpeglib.h"
#include "LoadJPEG.h"

namespace FDK {
	namespace Graphics {

// �Ǝ��̃G���[�n���h�����O���s���ꍇ�g�p
struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  // �Ǝ��̊g������
  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  (*cinfo->err->output_message) (cinfo);

  // setjmp �֖߂�
  longjmp(myerr->setjmp_buffer, 1);
}

int LoadJPEG( FILE* fp, BITMAPINFO** lppInfo, BYTE** lppBMP )
{
	BITMAPINFO* lpInfo = *lppInfo = NULL;
	BYTE* lpBMP = *lppBMP = NULL;

	// jpeg ���C�u�����ł͎��̂Q�̍\���̂��g����
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;

    JSAMPARRAY	buffer;              /* �s�o�̓o�b�t�@ */
    int			row_stride;          /* �o�̓o�b�t�@�̕����s��(bytes) */
    int			image_width;
    int			image_height;
	int			image_bpp;
	int			image_num_palette;
	RGBQUAD*	image_palette;
	UCHAR*		bmp_image;
    int			bmp_row;
    int			bmp_row_bytes;
    int			bmp_image_size;

    // �G���[�̃n���h�����O
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;

    // �ȍ~�� jpeg ���C�u�������ŃG���[���������ꍇ�A�������J�����ďI���B
    if (setjmp(jerr.setjmp_buffer)) {
        jpeg_destroy_decompress(&cinfo);
		if( lpInfo ) free( lpInfo );
		if( lpBMP ) free( lpBMP );
        return 0;
    }

	// �𓀊J�n
    jpeg_create_decompress( &cinfo );	// �\���̂̏����ݒ�
    jpeg_stdio_src( &cinfo, fp );		// �t�@�C�����̓n���h���̐ݒ�
    jpeg_read_header( &cinfo, true );	// �t�@�C���̏��w�b�_�̓Ǎ���
    jpeg_start_decompress( &cinfo );	// �𓀂̊J�n

    // DIB �̃C���[�W���擾�i�������j
    image_width			= cinfo.image_width;
    image_height		= cinfo.image_height;
	image_bpp			= 0;
	image_num_palette	= 0;
	image_palette		= NULL;

	// bpp �ƃp���b�g�̎擾
	switch( cinfo.out_color_space )
	{
	case JCS_GRAYSCALE:
		if( cinfo.num_components == 1 )
		{
			image_bpp = 8;
			image_num_palette = 256;
			image_palette = (RGBQUAD *) malloc( sizeof(RGBQUAD) * 256 );
			for( int x = 0; x < 256; x++ )
			{
				image_palette[x].rgbBlue = image_palette[x].rgbGreen = image_palette[x].rgbRed = x;
				image_palette[x].rgbReserved = 0;
			}
		}
		break;

	case JCS_RGB:
		if( cinfo.num_components == 3 )
			image_bpp = 24;
		break;

	case JCS_CMYK:
		if( cinfo.num_components == 4 )
			image_bpp = 24;					// �P�F�S�o�C�g�ł� 24bpp �Ƃ���
		break;
	}
	
	// bpp == 0 �Ȃ疢�Ή��t�H�[�}�b�g�Ƃ݂Ȃ��ċA��
	if( image_bpp == 0 )
	{
		jpeg_destroy_decompress( &cinfo );
		return 0;
	}

    // �s�o�b�t�@�̗̈�ݒ�
    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    if( !buffer ) goto SKIP1;

    // BMP ���f�[�^�̈�̏���
	if( lpBMP ) { free( lpBMP ); lpBMP = NULL; }
	bmp_row_bytes =  ( ( image_width * (image_bpp / 8) + 3 ) / 4 ) * 4;		// DIB �̍s�� DWORD ���E�ŏI���K�v������
    bmp_image_size = bmp_row_bytes * image_height;
    bmp_image = (UCHAR*) lpBMP = (BYTE*) malloc( bmp_image_size );

	// �P�s���ǂ݁A�o�b�t�@�֊i�[����
    bmp_row = image_height - 1;
    while( cinfo.output_scanline < cinfo.output_height )
	{
        int i;
        UCHAR *src, *dest;

        jpeg_read_scanlines( &cinfo, buffer, 1 );

        src = buffer[ 0 ];
        dest = &bmp_image[ bmp_row_bytes * bmp_row-- ];

		switch( cinfo.out_color_space )
		{
		case JCS_GRAYSCALE:
			memcpy( dest, src, cinfo.output_width );
			break;

		case JCS_RGB:
			for( i = 0; i < row_stride; i += 3 )
			{
				*dest++ = src[ i+2 ];
				*dest++ = src[ i+1 ];
				*dest++ = src[ i+0 ];
			}
	        for( i = 0; i < bmp_row_bytes - row_stride; i++ )      // DWORD ���E�␳(0�Ŗ��߂�)
				*dest++ = 0;
			break;

		case JCS_CMYK:
			for( i = 0; i < row_stride; i += 4 )
			{
				*dest++ = ( src[ i+2 ] * src[ i+3 ] ) / 255;
				*dest++ = ( src[ i+1 ] * src[ i+3 ] ) / 255;
				*dest++ = ( src[ i+0 ] * src[ i+3 ] ) / 255;
			}
	        for( i = 0; i < bmp_row_bytes - row_stride; i++ )      // DWORD ���E�␳(0�Ŗ��߂�)
				*dest++ = 0;
			break;
		}
    }

SKIP1:;
	// �𓀏I��
	jpeg_finish_decompress( &cinfo );
	jpeg_destroy_decompress( &cinfo );

	// BITMAP �쐬
	if( lpInfo ) { free( lpInfo ); lpInfo = NULL; }
	lpInfo = (BITMAPINFO*) malloc( sizeof( BITMAPINFOHEADER ) + (sizeof(RGBQUAD) * image_num_palette) );
    memset( &(lpInfo->bmiHeader), 0, sizeof( BITMAPINFOHEADER ) + (sizeof(RGBQUAD) * image_num_palette) );
	lpInfo->bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	lpInfo->bmiHeader.biWidth = image_width;
	lpInfo->bmiHeader.biHeight = image_height;
	lpInfo->bmiHeader.biPlanes = 1;
	lpInfo->bmiHeader.biBitCount = image_bpp;
	lpInfo->bmiHeader.biClrUsed  = image_num_palette;
	lpInfo->bmiHeader.biSizeImage = bmp_image_size;
	if( image_num_palette > 0 )
	{
		// �p���b�g������̂� 256�K���O���[�X�P�[�������i�ƌ��ߕt����j
		for( int i = 0; i < 256; i++ )
		{
			lpInfo->bmiColors[i].rgbBlue = lpInfo->bmiColors[i].rgbGreen = lpInfo->bmiColors[i].rgbRed = i;
			lpInfo->bmiColors[i].rgbReserved = 0;
		}
	}

	if( image_palette ) { free( image_palette ); image_palette = NULL; }
	*lppInfo = lpInfo;
	*lppBMP = lpBMP;
    return 1;
}
	}//Graphics
}//FDK