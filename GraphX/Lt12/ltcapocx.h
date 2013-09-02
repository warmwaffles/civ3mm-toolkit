  enum CapDisplayEnums
  {
    DISPLAY_NONE = 0, 
    DISPLAY_PREVIEW = 1, 
    DISPLAY_OVERLAY = 2
  };

  
  enum CaptureFlagsEnums
  {
    DLG_VIDCOMP = 3, 
    DLG_VIDDISPLAY = 5, 
    DLG_VIDFORMAT = 6, 
    DLG_VIDSOURCE = 7, 
    HAS_OVERLAY = 9, 
    AUD_SUPPORT = 17,
    DLG_AUDFORMAT = 18,
  };

#ifndef FILE_PCX
//VideoFileConstants
 #define FILE_PCX                1
 #define FILE_GIF                2
 #define FILE_TIF                3
 #define FILE_TGA                4
 #define FILE_CMP                5
 #define FILE_BMP                6
 #define FROM_BUFFER             7
 #define FILE_BITMAP             9
 #define FILE_JFIF               10
 #define FILE_JTIF               11
 #define FILE_BIN                12
 #define FILE_HANDLE             13
 #define FILE_OS2                14
 #define FILE_WMF                15
 #define FILE_EPS                16
 #define FILE_TIFLZW             17
 #define FILE_LEAD               20
 #define FILE_LEAD1JFIF          21
 #define FILE_LEAD1JTIF          22
 #define FILE_LEAD2JFIF          23
 #define FILE_LEAD2JTIF          24
 #define FILE_CCITT              25
 #define FILE_LEAD1BIT           26
 #define FILE_CCITT_GROUP3_1DIM  27
 #define FILE_CCITT_GROUP3_2DIM  28
 #define FILE_CCITT_GROUP4       29
 #define FILE_LEAD_NOLOSS        30
 #define FILE_CALS               50
 #define FILE_MAC                51
 #define FILE_IMG                52
 #define FILE_MSP                53
 #define FILE_WPG                54
 #define FILE_RAS                55
 #define FILE_PCT                56
 #define FILE_PCD                57
 #define FILE_DXF                58
 #define FILE_AVI                59
 #define FILE_WAV                60
 #define FILE_FLI                61
 #define FILE_CGM                62
 #define FILE_EPSTIFF            63
 #define FILE_EPSWMF             64
 #define FILE_CMPNOLOSS          65
 #define FILE_FAX_G3_1D          66
 #define FILE_FAX_G3_2D          67
 #define FILE_FAX_G4             68
 #define FILE_WFX_G3_1D          69
 #define FILE_WFX_G4             70
 #define FILE_ICA_G3_1D          71
 #define FILE_ICA_G3_2D          72
 #define FILE_ICA_G4             73
 #define FILE_OS2_2              74
 #define FILE_PNG                75
 #define FILE_PSD                76
 #define FILE_RAWICA_G3_1D       77
 #define FILE_RAWICA_G3_2D       78
 #define FILE_RAWICA_G4          79
 #define FILE_FPX                80
 #define FILE_FPX_SINGLE_COLOR   81
 #define FILE_FPX_JPEG           82
 #define FILE_FPX_JPEG_QFACTOR   83
 #define FILE_BMP_RLE            84
 #define FILE_TIF_CMYK           85
 #define FILE_TIFLZW_CMYK        86
 #define FILE_TIF_PACKBITS       87
 #define FILE_TIF_PACKBITS_CMYK  88
 #define FILE_DICOM_GRAY         89
 #define FILE_DICOM_COLOR        90
 #define FILE_WIN_ICO            91
 #define FILE_WIN_CUR            92
 #define FILE_TIF_YCC            93
 #define FILE_TIFLZW_YCC         94
 #define FILE_TIF_PACKBITS_YCC   95
 #define FILE_EXIF               96
 #define FILE_EXIF_YCC           97
 #define FILE_EXIF_JPEG          98
 #define FILE_AWD                99
 #define FILE_FASTEST            100
 #define FILE_PBM_ASCII          102
 #define FILE_PBM_BINARY         103
 #define FILE_PGM_ASCII          104
 #define FILE_PGM_BINARY         105
 #define FILE_PPM_ASCII          106
 #define FILE_PPM_BINARY         107
 #define FILE_CUT                108
 #define FILE_XPM                109
 #define FILE_XBM                110
 #define FILE_IFF_ILBM           111
 #define FILE_IFF_CAT            112
 #define FILE_XWD                113
 #define FILE_CLP                114
 #define FILE_JBIG               115
 #define FILE_EMF                116
 #define FILE_ICA_IBM_MMR        117 /* MO:DCA IBM MMR CCITT G32D */
 #define FILE_RAWICA_IBM_MMR     118 /* IOCA IBM MMR CCITT G32D */
 #define FILE_ANI                119
 #define FILE_ANI_RLE            120
 #define FILE_LASERDATA          121   /* LaserData CCITT G4 */
 #define FILE_INTERGRAPH_RLE     122   /* Intergraph RLE */
 #define FILE_INTERGRAPH_VECTOR  123   /* Intergraph Vector */
 #define FILE_DWG                124
 
 #define FILE_DICOM_RLE_GRAY     125
 #define FILE_DICOM_RLE_COLOR    126
 #define FILE_DICOM_JPEG_GRAY    127
 #define FILE_DICOM_JPEG_COLOR   128
 
 #define FILE_CALS4              129
 #define FILE_CALS2              130
 #define FILE_CALS3              131
 #define FILE_XWD10              132 
 #define FILE_XWD11              133 
 #define FILE_FLC                134   
 #define FILE_KDC                135   
 #define FILE_DRW                136   
 #define FILE_PLT                137
 #define FILE_TIF_CMP            138
 #define FILE_TIF_JBIG           139
 #define FILE_TIF_DXF            140
 #define FILE_TIF_UNKNOWN        141
 #define FILE_SGI                142
 #define FILE_SGI_RLE            143
 #define FILE_VECTOR_DUMP        144
 #define FILE_DWF                145
 #define FILE_RAS_PDF            146
 #define FILE_RAS_PDF_G3_1D      147
 #define FILE_RAS_PDF_G3_2D      148
 #define FILE_RAS_PDF_G4         149
 #define FILE_RAS_PDF_JPEG       150
 #define FILE_RAS_PDF_JPEG_422   151
 #define FILE_RAS_PDF_JPEG_411   152

 #define FILE_RAW                153

 #define FILE_RASTER_DUMP        154
 #define FILE_TIFF_CUSTOM        155

 #define FILE_RAW_RGB            156
 #define FILE_RAW_RLE4           157
 #define FILE_RAW_RLE8           158
 #define FILE_RAW_BITFIELDS      159
 #define FILE_RAW_PACKBITS       160
 #define FILE_RAW_JPEG           161
 #define FILE_FAX_G3_1D_NOEOL    162
 #define FILE_RAW_CCITT          (FILE_FAX_G3_1D_NOEOL)

 #define FILE_JP2                163
 #define FILE_J2K                164
 #define FILE_CMW                165

 #define FILE_TIF_JP2            166
 #define FILE_TIF_CMW            167
 #define FILE_MRC                168

 #define FILE_MPEG1              243
 #define FILE_MPEG2              246
#endif

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
  
  enum FeedSoundFlagsEnums
  {
    CAP_FEED_SAVEDATA = 1, 
    CAP_FEED_PLAYDATA = 2, 
    CAP_FEED_SAVEONERROR = 4
  };

  
  enum FeedSoundMsgEnums
  {
    CAP_FEED_EMPTY = 0, 
    CAP_FEED_CLOSED = 1
  };

  
  enum MixerEnums
  {
    ENUM_AUX = 1, 
    ENUM_WAVE_IN = 2, 
    ENUM_WAVE_OUT = 4, 
    ENUM_MIDI_IN = 8, 
    ENUM_MIDI_OUT = 16, 
    ENUM_ALL = 4095
  };

  
  enum MixerDestination
  {
    DST_UNDEFINED = 0, 
    DST_DIGITAL = 1, 
    DST_LINE = 2, 
    DST_MONITOR = 3, 
    DST_SPEAKERS = 4, 
    DST_HEADPHONES = 5, 
    DST_TELEPHONE = 6, 
    DST_WAVEIN = 7, 
    DST_VOICEIN = 8
  };

  
  enum MixerSource
  {
    SRC_UNDEFINED = 0, 
    SRC_DIGITAL = 1, 
    SRC_LINE = 2, 
    SRC_MICROPHONE = 3, 
    SRC_SYNTHESIZER = 4, 
    SRC_COMPACTDISC = 5, 
    SRC_TELEPHONE = 6, 
    SRC_PCSPEAKER = 7, 
    SRC_WAVEOUT = 8, 
    SRC_AUXILIARY = 9, 
    SRC_ANALOG = 10
  };

  
  enum MixerTarget
  {
    TARGET_UNDEFINED = 0, 
    TARGET_WAVEOUT = 1, 
    TARGET_WAVEIN = 2, 
    TARGET_MIDIOUT = 3, 
    TARGET_MIDIIN = 4, 
    TARGET_AUX = 5
  };

  enum WaveFormats
  {
     CAP_WAVE_FORMAT_UNKNOWN    = 0x0000  , 
     CAP_WAVE_FORMAT_PCM        = 0x0001  , 
     CAP_WAVE_FORMAT_ADPCM      = 0x0002  , 
     CAP_WAVE_FORMAT_IBM_CVSD   = 0x0005  , 
     CAP_WAVE_FORMAT_ALAW       = 0x0006  ,
     CAP_WAVE_FORMAT_MULAW      = 0x0007  ,
     CAP_WAVE_FORMAT_OKI_ADPCM  = 0x0010  , 
     CAP_WAVE_FORMAT_DVI_ADPCM  = 0x0011  , 
     CAP_WAVE_FORMAT_IMA_ADPCM  = (CAP_WAVE_FORMAT_DVI_ADPCM) , 
     CAP_WAVE_FORMAT_MEDIASPACE_ADPCM   = 0x0012  , 
     CAP_WAVE_FORMAT_SIERRA_ADPCM       = 0x0013  , 
     CAP_WAVE_FORMAT_G723_ADPCM = 0x0014  , 
     CAP_WAVE_FORMAT_DIGISTD    = 0x0015  , 
     CAP_WAVE_FORMAT_DIGIFIX    = 0x0016  ,
     CAP_WAVE_FORMAT_DIALOGIC_OKI_ADPCM = 0x0017  , 
     CAP_WAVE_FORMAT_YAMAHA_ADPCM       = 0x0020  , 
     CAP_WAVE_FORMAT_SONARC     = 0x0021  ,
     CAP_WAVE_FORMAT_DSPGROUP_TRUESPEECH        = 0x0022  ,
     CAP_WAVE_FORMAT_ECHOSC1    = 0x0023  , 
     CAP_WAVE_FORMAT_AUDIOFILE_AF36     = 0x0024  , 
     CAP_WAVE_FORMAT_APTX       = 0x0025  , 
     CAP_WAVE_FORMAT_AUDIOFILE_AF10     = 0x0026  ,
     CAP_WAVE_FORMAT_DOLBY_AC2  = 0x0030  , 
     CAP_WAVE_FORMAT_GSM610     = 0x0031  , 
     CAP_WAVE_FORMAT_ANTEX_ADPCME       = 0x0033  ,
     CAP_WAVE_FORMAT_CONTROL_RES_VQLPC  = 0x0034  , 
     CAP_WAVE_FORMAT_DIGIREAL   = 0x0035  , 
     CAP_WAVE_FORMAT_DIGIADPCM  = 0x0036  , 
     CAP_WAVE_FORMAT_CONTROL_RES_CR10   = 0x0037  , 
     CAP_WAVE_FORMAT_NMS_VBXADPCM       = 0x0038  , 
     CAP_WAVE_FORMAT_CS_IMAADPCM = 0x0039 , 
     CAP_WAVE_FORMAT_G721_ADPCM = 0x0040  , 
     CAP_WAVE_FORMAT_MPEG       = 0x0050  , 
     CAP_WAVE_FORMAT_CREATIVE_ADPCM     = 0x0200  , 
     CAP_WAVE_FORMAT_CREATIVE_FASTSPEECH8       = 0x0202  , 
     CAP_WAVE_FORMAT_CREATIVE_FASTSPEECH10      = 0x0203  , 
     CAP_WAVE_FORMAT_FM_TOWNS_SND       = 0x0300  , 
     CAP_WAVE_FORMAT_OLIGSM     = 0x1000  , 
     CAP_WAVE_FORMAT_OLIADPCM   = 0x1001  , 
     CAP_WAVE_FORMAT_OLICELP    = 0x1002  , 
     CAP_WAVE_FORMAT_OLISBC     = 0x1003  , 
     CAP_WAVE_FORMAT_OLIOPR     = 0x1004  , 
  };

   enum AudioListConstants
   {
      CAP_AUDIO_CAPTURE = 0,
      CAP_AUDIO_RECORD,
      CAP_AUDIO_FEED,
      CAP_AUDIO_MAX,         // the first invalid index
   };

   enum AudioFlags
   {
      CAP_AUDIO_CHOOSE_INPUT  = 0x00800000,
      CAP_AUDIO_CHOOSE_OUTPUT = 0x01000000,
   };

// ActiveX error codes
#include "L_OcxErr.h"

// L_SUPPORT_XXX constants
#include "ltlck_.h"
