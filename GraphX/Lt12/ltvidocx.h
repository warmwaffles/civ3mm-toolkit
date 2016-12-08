enum ControlModeEnum
{
    MODE_PLAYBACK = 0,
    MODE_EDIT     = 1,
};

enum EditFlagEnum
{
    EDIT_BOTH  = 0,
    EDIT_VIDEO = 1,
    EDIT_AUDIO = 2,
};

enum MMModeEnum
{
    MODE_PLAY   = 0,
    MODE_STOP   = 1,
    MODE_PAUSE  = 2,
    MODE_RESUME = 3,
};

enum CompressorEnums
{
    COMPRESSOR_VIDEO = 0,
    COMPRESSOR_AUDIO = 1,
};

enum RecordingEnums
{
    RECORD_MIC  = 0,
    RECORD_LINE = 1,
};

enum StreamTypeEnums
{
    STREAM_VIDEO   = 1,
    STREAM_AUDIO   = 2,
    STREAM_UNKNOWN = 3,
};

#ifndef FILE_WAV_1M08
//AudioFormatEnums
#define FILE_WAV_1M08 1
#define FILE_WAV_1S08 2
#define FILE_WAV_1M16 4
#define FILE_WAV_1S16 8
#define FILE_WAV_2M08 16
#define FILE_WAV_2S08 32
#define FILE_WAV_2M16 64
#define FILE_WAV_2S16 128
#define FILE_WAV_4M08 256
#define FILE_WAV_4S08 512
#define FILE_WAV_4M16 1024
#define FILE_WAV_4S16 2048
#endif

enum TimeFormatEnums
{
    TIME_FRAME    = 0,
    TIME_MILLISEC = 1,
};

#ifndef FILE_PCX
//VideoFileConstants
#define FILE_PCX 1
#define FILE_GIF 2
#define FILE_TIF 3
#define FILE_TGA 4
#define FILE_CMP 5
#define FILE_BMP 6
#define FROM_BUFFER 7
#define FILE_BITMAP 9
#define FILE_JFIF 10
#define FILE_JTIF 11
#define FILE_BIN 12
#define FILE_HANDLE 13
#define FILE_OS2 14
#define FILE_WMF 15
#define FILE_EPS 16
#define FILE_TIFLZW 17
#define FILE_LEAD 20
#define FILE_LEAD1JFIF 21
#define FILE_LEAD1JTIF 22
#define FILE_LEAD2JFIF 23
#define FILE_LEAD2JTIF 24
#define FILE_CCITT 25
#define FILE_LEAD1BIT 26
#define FILE_CCITT_GROUP3_1DIM 27
#define FILE_CCITT_GROUP3_2DIM 28
#define FILE_CCITT_GROUP4 29
#define FILE_LEAD_NOLOSS 30
#define FILE_CALS 50
#define FILE_MAC 51
#define FILE_IMG 52
#define FILE_MSP 53
#define FILE_WPG 54
#define FILE_RAS 55
#define FILE_PCT 56
#define FILE_PCD 57
#define FILE_DXF 58
#define FILE_AVI 59
#define FILE_WAV 60
#define FILE_FLI 61
#define FILE_CGM 62
#define FILE_EPSTIFF 63
#define FILE_EPSWMF 64
#define FILE_CMPNOLOSS 65
#define FILE_FAX_G3_1D 66
#define FILE_FAX_G3_2D 67
#define FILE_FAX_G4 68
#define FILE_WFX_G3_1D 69
#define FILE_WFX_G4 70
#define FILE_ICA_G3_1D 71
#define FILE_ICA_G3_2D 72
#define FILE_ICA_G4 73
#define FILE_OS2_2 74
#define FILE_PNG 75
#define FILE_PSD 76
#define FILE_RAWICA_G3_1D 77
#define FILE_RAWICA_G3_2D 78
#define FILE_RAWICA_G4 79
#define FILE_FPX 80
#define FILE_FPX_SINGLE_COLOR 81
#define FILE_FPX_JPEG 82
#define FILE_FPX_JPEG_QFACTOR 83
#define FILE_BMP_RLE 84
#define FILE_TIF_CMYK 85
#define FILE_TIFLZW_CMYK 86
#define FILE_TIF_PACKBITS 87
#define FILE_TIF_PACKBITS_CMYK 88
#define FILE_DICOM_GRAY 89
#define FILE_DICOM_COLOR 90
#define FILE_WIN_ICO 91
#define FILE_WIN_CUR 92
#define FILE_TIF_YCC 93
#define FILE_TIFLZW_YCC 94
#define FILE_TIF_PACKBITS_YCC 95
#define FILE_EXIF 96
#define FILE_EXIF_YCC 97
#define FILE_EXIF_JPEG 98
#define FILE_AWD 99
#define FILE_FASTEST 100
#define FILE_EXIF_JPEG_411 101
#define FILE_PBM_ASCII 102
#define FILE_PBM_BINARY 103
#define FILE_PGM_ASCII 104
#define FILE_PGM_BINARY 105
#define FILE_PPM_ASCII 106
#define FILE_PPM_BINARY 107
#define FILE_CUT 108
#define FILE_XPM 109
#define FILE_XBM 110
#define FILE_IFF_ILBM 111
#define FILE_IFF_CAT 112
#define FILE_XWD 113
#define FILE_CLP 114
#define FILE_JBIG 115
#define FILE_EMF 116
#define FILE_ICA_IBM_MMR 117
#define FILE_RAWICA_IBM_MMR 118
#define FILE_ANI 119
#define FILE_ANI_RLE 120
#define FILE_LASERDATA 121
#define FILE_INTERGRAPH_RLE 122
#define FILE_INTERGRAPH_VECTOR 123
#define FILE_DWG 124
#define FILE_DICOM_RLE_GRAY 125
#define FILE_DICOM_RLE_COLOR 126
#define FILE_DICOM_JPEG_GRAY 127
#define FILE_DICOM_JPEG_COLOR 128
#define FILE_CALS4 129
#define FILE_CALS2 130
#define FILE_CALS3 131
#define FILE_XWD10 132
#define FILE_XWD11 133
#define FILE_FLC 134
#define FILE_KDC 135
#define FILE_DRW 136
#define FILE_PLT 137
#define FILE_TIF_CMP 138
#define FILE_TIF_JBIG 139
#define FILE_TIF_DXF 140
#define FILE_TIF_UNKNOWN 141
#define FILE_SGI 142
#define FILE_SGI_RLE 143
#define FILE_VECTOR_DUMP 144
#define FILE_DWF 145
#define FILE_RAS_PDF 146
#define FILE_RAS_PDF_G3_1D 147
#define FILE_RAS_PDF_G3_2D 148
#define FILE_RAS_PDF_G4 149
#define FILE_RAS_PDF_JPEG 150
#define FILE_RAS_PDF_JPEG_422 151
#define FILE_RAS_PDF_JPEG_411 152

#define FILE_RAW 153

#define FILE_RASTER_DUMP 154
#define FILE_TIFF_CUSTOM 155

#define FILE_RAW_RGB 156
#define FILE_RAW_RLE4 157
#define FILE_RAW_RLE8 158
#define FILE_RAW_BITFIELDS 159
#define FILE_RAW_PACKBITS 160
#define FILE_RAW_JPEG 161
#define FILE_FAX_G3_1D_NOEOL 162
#define FILE_RAW_CCITT (FILE_FAX_G3_1D_NOEOL)

#define FILE_JP2 163
#define FILE_J2K 164
#define FILE_CMW 165

#define FILE_TIF_JP2 166
#define FILE_TIF_CMW 167
#define FILE_MRC 168
#define FILE_MPEG1 243
#define FILE_MPEG2 246
#endif

#ifndef MPEG_ASPECT_SQUARE
// aspect ratio information

#define MPEG_ASPECT_SQUARE 1 // square pel
#define MPEG_ASPECT_4_3 2    // 4:3
#define MPEG_ASPECT_16_9 3   // 16:9
#define MPEG_ASPECT_2_1 4    // 2.11:1
#define MPEG_ASPECT_625 8    // CCIR601 625 Line
#define MPEG_ASPECT_525 9    // CCIR601 525 Line

// frame rate code

#define MPEG_FRAMERATE_23 1 // 23.976
#define MPEG_FRAMERATE_24 2 // 24
#define MPEG_FRAMERATE_25 3 // 25
#define MPEG_FRAMERATE_29 4 // 29.97
#define MPEG_FRAMERATE_30 5 // 30

// profile id

#define MPEG_PROFILE_SIMPLE 5
#define MPEG_PROFILE_MAIN 4
#define MPEG_PROFILE_SNR 3
#define MPEG_PROFILE_SPATIAL 2
#define MPEG_PROFILE_HIGH 1

// level id

#define MPEG_LEVEL_LOW 10
#define MPEG_LEVEL_MAIN 8
#define MPEG_LEVEL_HIGH14440 6
#define MPEG_LEVEL_HIGH 4

// chroma format

#define MPEG_CHROMA_420 1 // 4:2:0
#define MPEG_CHROMA_422 2 // 4:2:2
#define MPEG_CHROMA_444 3 // 4:4:4

// video format

#define MPEG_VIDEO_COMP 0
#define MPEG_VIDEO_PAL 1
#define MPEG_VIDEO_NTSC 2
#define MPEG_VIDEO_SECAM 3
#define MPEG_VIDEO_MAC 4
#define MPEG_VIDEO_UNSP 5 // unspecified

// intra dc precision

#define MPEG_INTRA_8 0  // 8 bit
#define MPEG_INTRA_9 1  // 9 bit
#define MPEG_INTRA_10 2 // 10 bit
#define MPEG_INTRA_11 3 // 11 bit

// MPEG Audio Header Definitions - Mode Values

#define MPEG_MODE_STEREO 0
#define MPEG_MODE_JOINT_STEREO 1
#define MPEG_MODE_DUAL_CHANNEL 2
#define MPEG_MODE_MONO 3

// MPEG Audio Layer Defines

#define MPEG_LAYER_I 1
#define MPEG_LAYER_II 2
#define MPEG_LAYER_III 3

// MPEG Audio Sampling frequency

#define MPEG_FREQ_16 0 // 16000
#define MPEG_FREQ_22 1 // 22050
#define MPEG_FREQ_24 2 // 24000
#define MPEG_FREQ_32 3 // 32000
#define MPEG_FREQ_44 4 // 44100
#define MPEG_FREQ_48 5 // 48000

// MPEG Audio encoder de-emphasis

#define MPEG_DEMP_NONE 0 // none
#define MPEG_DEMP_5015 1 // 50/15 micro seconds
#define MPEG_DEMP_J17 2  // CCITT J.17
#endif

enum DialogTypeEnums
{
    MM_DLG_CONFIGURE = 1,
    MM_DLG_ABOUT     = 2,
};

// ActiveX error codes
#include "L_OcxErr.h"

// L_SUPPORT_XXX constants
#include "ltlck_.h"
