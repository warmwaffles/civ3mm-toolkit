#ifndef DEFINITIONS_H__CIV3FLCEDIT__INCLUDED_
#define DEFINITIONS_H__CIV3FLCEDIT__INCLUDED_

///////////////////////////////////////////////////////////////
// REGISTRY KEYS

#define REG_OPTIONS "Options"
#define REG_LAST_OPENED "Last Opend"

#define REG_MENU "Menu"
#define REG_MENU_STYLE "Style"
#define REG_MENU_SELECTION "Selection"
#define REG_MENU_GLOOM "Gloom"

#define REG_MAX_MRU "Max MRU"
#define REG_LOAD_LAST_DOC "Load Doc"
#define REG_MRU_FLC "Recent FLC Files"
#define REG_MRU_C3F "Recent C3F Files"
#define REG_CHECK_FILE_TYPE "Check File Type"
#define REG_CONTROL_BAR "CtrlBars\\CtrlBar"
#define REG_SHOW_TOOLBAR "Show ToolBar"
#define REG_SHOW_STATUSBAR "Show StatusBar"
#define REG_SHOW_DIALOGBAR "Show DialogBar"
#define REG_FLOATING "IsFloating"
#define REG_ALWAYS_ON_TOP "Always on top"
#define REG_PAL_TYPE "Pal Type"
#define REG_DEF_PAL_TYPE "Alpha Pal"
#define REG_MAKE_PALETTE "Make Pal File"
#define REG_LAST_PAL_TYPE "Pal32 Type"
#define REG_LAST_PAL_PATH "Pal32 Path"
#define REG_PAL_DIR "Pal Dir"
#define REG_ANIM_TYPE "Anim Type"

#define REG_FILE_TYPE "FileType"
#define REG_INIT_PATH "Path"
#define REG_FRM_RESIZE "Resize"

#define REG_LAST_PAL "PalDir"
#define REG_FILTER_PAL_IMP "PalFilterImp"
#define REG_FILTER_PAL_EXP "PalFilterExp"
#define REG_FILTER_FLCEDIT "FlcEditFilter"

#define REG_BGD_TYPE "Bgd Type"
#define REG_BGD_COLOR "Custom Color"
#define REG_BGD_IMG "Custom Img"

#define REG_DIRECTION "Direction"
#define REG_ALPHA_BLEND "AlphaBlend"

#define REG_SB_OPTIONS "SbOptions"
#define REG_SF_INFO_DLG "Dlg FlcInfo"

#define REG_EXP_OPEN "ExpOpen"
#define REG_EXP_ASK "ExpAsk"
#define REG_IGNORE_OFFSET "Ignore Offset"

#define REG_PREVIEW "Preview"
#define REG_WAV_DLG "Dlg Wav"
#define REG_WAV_NON_STOP "Wav NonStop"
#define REG_FILTER "Filter"

#define REG_COL_DEPTH "ColDepth"

#define REG_CURRENT_TAB "CurTab"

//////////////////////////////////////////////////////////////////////////
// For 'Export FLC' & 'New FLC' dialog boxes
#define REG_SAVE_SETTINGS "Save"
#define REG_LEADER_HEAD "LeaderHead"
#define REG_DELAY "Delay"
#define REG_FRM_COUNT "FrmCount"
#define REG_FRM_W "Width"
#define REG_FRM_H "Height"
#define REG_OFFSET_X "XOffset"
#define REG_OFFSET_Y "YOffset"
#define REG_DIR_COUNT "DirCount"
#define REG_PALETTE "Palette"
#define REG_SEL_PAL_PATH "SelPalPath"
#define REG_SEL_PAL "SelPal"
#define REG_TRIM_LEFT "TrimLeft"
#define REG_TRIM_RIGHT "TrimRight"
#define REG_TRIM_TOP "TrimTop"
#define REG_TRIM_BOTTOM "TrimBottom"
#define REG_USE_SIZE "UseSize"
#define REG_USE_FRM_COUNT "UseFrmCount"
#define REG_USE_OFFSET "UseOffset"
#define REG_USE_DELAY "UseDelay"
#define REG_USE_TRIM "UseTrim"
#define REG_USE_NAME "UseName"
#define REG_USE_DIR_COUNT "UseDirCount"
#define REG_LOCK "Lock"

///////////////////////////////////////////////////////////////
// GLOBAL DEFINITIONS

// Chunks
#define COLOR_256 4L // 256-level color palette info
#define DELTA_FLC 7L // Word-oriented delta compression
#define BLACK 13L    // Entire frame is color index 0 (full black frame)
#define BYTE_RUN 15L // Full image, byte run length compression
#define FLI_COPY 16L // No compression

#define FRAME_TYPE 0xF1FA // Frame chunk

#define WND_W 240 // maximal frame width for animation
#define WND_H 240 // maximal frame height for animation

#define WND_ACX 2 // additional preview frame width
#define WND_ACY 4 // additional preview frame height

#define WND_CX 242 // preview frame width (WND_W + WND_ACX)
#define WND_CY 244 // preview frame height (WND_H + WND_ACY)

#define DIRECTIONS_MAX 8 // maximal directions count

#define CIV_COLOR 6     // Order of the civilization color in the palette
#define COLOR8 256L     // Number of colors
#define FLC_TYPE 0xAF12 // For standard FLC files (with an 8-bit colour depth)

#define IDX_MAX 255 // last color index in palette (0-255)

#define BITS_8 8   // 256 color mode
#define BITS_24 24 // 24 bits color mode

#define PCX_TYPE 0 // PCX files type
#define BMP_TYPE 1 // BMP files type
#define MAX_TYPE 2 // maximal supported types count

#define TIMER_PLAYER_ID 1
#define TIMER_PLAYER_DELAY 50

#define TIMER_REWIND_ID 2
#define TIMER_REWIND_DELAY 100

#define C3F_EXT "c3f" // C3F files
#define FLC_EXT "flc" // FLC files
#define ACT_EXT "act" // ACT palette files
#define PAL_EXT "pal" // PAL palette files
#define BMP_EXT "bmp" // Windows Bitmap files
#define PCX_EXT "pcx" // ZSoft PCX files

#define GRID_SIZE 1 // size of the grig lines

#define FLI_MAX_RUN 127    // Longest run for byte-oriented compression
#define MAX_FRM_COUNT 1000 // Maximal value of frames per direction

#define FRM_DELAY_MIN 10   // minimal delay between frame in milliseconds
#define FRM_DELAY_MAX 1000 // maximal delay between frame in milliseconds

// Default transparent color
//static COLORREF RGB_TRANSPARENT = RGB(255, 0, 255);
#define RGB_TRANSPARENT 0xFF00FF
#define RGB_ALPHA_ERROR 0x01FFFF

#ifndef SAFE_FREE
#define SAFE_FREE(ptr)                                                                                                                     \
    if (ptr != NULL) {                                                                                                                     \
        free(ptr);                                                                                                                         \
        ptr = NULL;                                                                                                                        \
    };
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(ptr)                                                                                                                  \
    if (ptr) {                                                                                                                             \
        (ptr)->Release();                                                                                                                  \
        ptr = NULL;                                                                                                                        \
    }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(ptr)                                                                                                                   \
    if (ptr) {                                                                                                                             \
        delete (ptr);                                                                                                                      \
        (ptr) = NULL;                                                                                                                      \
    };
#endif

#ifndef SAFE_DELETE_MULTI
#define SAFE_DELETE_MULTI(ptr)                                                                                                             \
    if (ptr) {                                                                                                                             \
        delete[] ptr;                                                                                                                      \
        ptr = NULL;                                                                                                                        \
    }
#endif

///////////////////////////////////////////////////////////////
// GLOBAL ENUMS

enum EPalType
{
    PalGame = 0,
    PalRes  = 1,
    PalDir  = 2
};

enum EPaletteFileType
{
    PalACT  = 0, // Adobe Color Table
    PalMS   = 1, // Microsoft Palette
    PalJASC = 2, // Jasc Animation Shop
    PalErr  = 3  // an invalid format
};

enum ELastPalType
{
    PalCurrent   = 0,
    PalHardcoded = 1,
    PalSelective = 2,
    PalError     = 3
};

enum EDirection
{
    DirAll = 0,
    DirSW  = 1,
    DirS   = 2,
    DirSE  = 3,
    DirE   = 4,
    DirNE  = 5,
    DirN   = 6,
    DirNW  = 7,
    DirW   = 8
};

enum EBdg
{
    BgdDefault = 0,
    BgdCustom  = 1,
    BgdSea     = 2,
    BgdLand    = 3,
    BgdGrid    = 4,
    BgdImg     = 5
};

enum EFramePos
{
    FrmFirst    = 0,
    FrmPrevious = 1,
    FrmNext     = 2,
    FrmLast     = 3
};

enum EBitsPerPixel
{
    BitsPerPixel_8  = 0,
    BitsPerPixel_24 = 1
};

enum EChunkType
{
    ChNull      = 0x00, // initial value
    ChDELTA_FLC = 0x01, // Word-oriented delta compression
    ChBYTE_RUN  = 0x02, // Full image, byte run length compression
    ChCOLOR_256 = 0x04, // 256-level color palette info
    ChBLACK     = 0x08, // Entire frame is color index 0 (full black frame)
    ChFLI_COPY  = 0x10, // No compression
    ChDefault   = 0x20  // all other chunks
};

///////////////////////////////////////////////////////////////
// GLOBAL TYPEDEFS

#pragma pack(1)
typedef struct _FrameHeader
{                     // Offset   Size
    DWORD Size;       //      0      4 - Total size of chunk
    WORD Type;        //      4      2 - Chunk identifier
    WORD Count;       //      6      2 - Number of subchunks in this chunk
    BYTE Reserved[8]; //      8      8 - Not used (Set to 00h)
} FRAMEHEADER;        // Total size: 16 bytes
/*
typedef struct _DeltaFlcChunk
{
	CHUNKHEADER Header;				// Header for this chunk
	WORD NumberOfLines;				// Number of encoded lines in chunk
	struct _Line					// Encoded line (one/'NumberOfLines')
	{
		WORD PacketCount;			// Packet count, skip count, or last byte value
									// Additional WORDs of data may appear in this location
		struct _Packet				// Encoded packet (one per 'Count')
		{
			BYTE SkipCount;			// Number of pixels to skip
			BYTE PacketType;		// Type of encoding used on this packet
			WORD PixelData[];		// Pixel data for this packet
		} Packet[NumberOfPackets];
	} Lines[NumberOfLines];
} DELTAFLCCHUNK;

typedef struct _ByteRunChunk
{
	CHUNKHEADER Header;				// Header for this chunk
	BYTE PixelData[];				// RLE pixel data
} BYTERUNCHUNK;

typedef struct _ColormapChunk
{
	CHUNKHEADER Header;				// Header for this chunk
	WORD NumberOfElements;			// Number of color elements in map
	struct _ColorElement			// Color element (one per NumberOfElements)
	{
		BYTE SkipCount;				// Color index skip count
		BYTE ColorCount;			// Number of colors in this element
		struct _ColorComponent		// Color component (one /'ColorCount')
		{
			BYTE Red;				// Red component color
			BYTE Green;				// Green component color
			BYTE Blue;				// Blue component color
		} ColorComponents[ColorCount];
	} ColorElements[NumberOfElements];
} COLORMAPCHUNK;
*/
typedef struct _ChunkHeader
{               // Offset   Size
    DWORD Size; //      0      4
    WORD Type;  //      4      2
} CHUNKHEADER;  // Total size: 6 bytes

typedef struct tagCHUNK
{
    EChunkType type; // chunk's type
    DWORD pos;       // start position
    DWORD size;      // size in bytes

    tagCHUNK()
    {
        type = ChNull;
        pos  = 0;
        size = 0;
    }
} CHUNK;
typedef CHUNK FAR* LPCHUNK;

typedef struct tagFRAME
{
    WORD id;         // frame number
    DWORD pos;       // start position
    DWORD size;      // size in bytes
    CHUNK chunks[5]; // frame chunks
    WORD numch;      // chunks count

    tagFRAME()
    {
        id    = 0;
        pos   = 0;
        size  = 0;
        numch = 0;
    }
} FRAME;
typedef FRAME FAR* LPFRAME;

typedef struct _RGBQUADPAL
{
    RGBQUAD pal[COLOR8];
} RGBQUADPAL; // Total size: 1024 bytes

#define RGBQUADPAL_SIZE sizeof(RGBQUADPAL)

// Old struct for C3F Storyboards:
typedef struct _C3FSTRUCT_OLD
{                    // Offset   Size	Description
    BYTE Name[64];   //  0		64		Image file name
    WORD Width;      // 64		 2		Image width
    WORD Height;     // 66		 2		Image height
    BYTE FrmWidth;   // 67		 1		Frame width
    BYTE FrmHeight;  // 68		 1		Frame height
    BYTE FrmCount;   // 69		 1		Frames per direction
    BYTE DirCount;   // 70		 1		Directions number
    WORD Delay;      // 71		 2		Frame delay (ms)
    BYTE XOffset;    // 73		 1		Left offset
    BYTE YOffset;    // 74		 1		Top offset
    BYTE FileType;   // 75		 1		File type
    BYTE PalType;    // 76		 1		Palette type
    BYTE LeaderHead; // 77		 1		Animation type (unit or leaderhead)
} C3FSTRUCT_OLD;     // Total size: 79 bytes

// New struct for C3F Storyboards (from v.3,1,7,100):
// Width & Height is UINT instead of WORD
// FrmCount is WORD instead of BYTE
typedef struct _C3FSTRUCT
{                    // Offset   Size	Description
    BYTE Name[64];   //  0		64		Image file name
    UINT Width;      // 64		 4		Image width
    UINT Height;     // 68		 4		Image height
    BYTE FrmWidth;   // 72		 1		Frame width
    BYTE FrmHeight;  // 73		 1		Frame height
    WORD FrmCount;   // 74		 2		Frames per direction
    BYTE DirCount;   // 76		 1		Directions number
    WORD Delay;      // 77		 2		Frame delay (ms)
    BYTE XOffset;    // 79		 1		Left offset
    BYTE YOffset;    // 80		 1		Top offset
    BYTE FileType;   // 81		 1		File type
    BYTE PalType;    // 82		 1		Palette type
    BYTE LeaderHead; // 83		 1		Animation type (unit or leaderhead)
} C3FSTRUCT;         // Total size: 84 bytes
#pragma pack()

/*
	Mike Breitkreutz
	mbreitkreutz@firaxis.com
	Tools Manager
	FIRAXIS Games

	The FLC format was created by Autodesk. You can find some information on the format here: http://www.compuphase.com/flic.htm. 
	The reserved3 field of the FLIC_HEADER is used for custom Civ3 data.
	The following email has some information about this data.
	The name of our "custom" FLC format is FlicAnim and the header is the FlicAnimHeader. 
	The FlicAnimHeader structure is 28 bytes long and consists of 10 fields. 
	The remaining 12 bytes of the reserved field are set to 0:

	DWORD	size: Set to the size of the FlicAnimHeader structure (currently 28 bytes or 0x001C)
	int		flags: This field is used internally and by our FlicAnim creator and should be set to 0.
	WORD	num_anims: This is equivalent to the number of directions stored in the FlicAnim. This value can
	be calculated by dividing the total number of frames in the FLC (from the FLC header)
	by the anim_length (see below).
	WORD	anim_length: This is the number of frames in each direction. Note that each direction must have the same
	number of frames.
	WORD	x_offset: The pixel offset from the left edge of the image to the clipped sprite (see below).
	WORD	y_offset: The pixel offset from the top edge of the image to the clipped sprite (see below).
	WORD	xs_orig: The pixel width of the non-clipped sprite (see below).
	WORD	ys_orig: The pixel height of the non-clipped sprite (see below).
	DWORD	anim_time: This is the frame rate of the animation. It can be calculated with the following formula:
	anim_length * 1000 / fps
	where fps is the frames per second at which the FLC is supposed to
	be played. The value is in milliseconds. I'm not entirely sure if this is used.
	int		directions:
	Bit flags representing which directions are present. If a bit is set, that direction is present.
	Bit 0: southwest
	Bit 1: south
	Bit 2: southeast
	Bit 3: east
	Bit 4: northeast
	Bit 5: north
	Bit 6: northwest
	Bit 7: west

	I believe this was done so that you could have an animation with 4 directions and set bits
	1, 3, 5, and 7 so that you have just the cardinal directions (for example). However, I don't
	think this will work for units in Civ3.
  
	Note:
	DWORDs are 4 BYTES and are unsigned.
	WORDs are 2 BYTES and are unsigned.
	ints are 4 BYTES and they are signed.
	
	Clipping:
	Our FlicAnim program calculates the smallest bounding box needed to contain all of the frames of the animation. The upper-left corner of this bounding box is stored as x_offset and y_offset. The xs_orig and ys_orig are the original height and width of the image (not taking the bounding box into account). I believe all of our units are 240x240 which is why you found these fields to always be 240.
	  
	FLC header:
	The standard FLC header has the expected data except the following:
	The Size field should contain the total number of frames in the file (num_anims * anim_length). The actual number of frames will be Size + 1 because all FLC files contain a ring frame for looping purposes.
	The Speed field should be set to 4.
	The Creator field should be set to 0xF1F1F2F2, which desginates the FLICSPRITE_CREATOR we use to make the FlicAnims. (Note that the FLC specs state that this should be 0x464C4942 if not created by Animator Pro - I'm not sure why this info was ignored).
		
	Palette:
	Most of our unit FLCs contain a bug wherein the FLI_COLOR_256 chunks contain an incorrect size. As the game knows that all of our palettes are 256 colors, it doesn't cause a problem. It does, however, cause problems in other programs that attempt to load our FlicAnims. Our most recently created units should contain the proper size information.
		  
	Compression:
	I'm fairly certain there is a bug in the FLI_WORD_LC (also referred to as FLI_SS2) compression used for many of the frames as well. I've read differing descriptions of the format that explain the skip count. In some descriptions, the skip count represents the number of 2-pixel words to skip while in others, it is the number of bytes to skip. Our format treats the skip count as the number of bytes to skip.		
*/

struct FlicAnimHeader
{
    DWORD size;        // Set to the size of the FlicAnimHeader structure (currently 28 bytes or 0x001C)
    int flags;         // This field is used internally and by our FlicAnim creator and should be set to 0.
    WORD num_anims;    // This is equivalent to the number of directions stored in the FlicAnim.
    WORD anim_length;  // This is the number of frames in each direction.
    WORD x_offset;     // The pixel offset from the left edge of the image to the clipped sprite.
    WORD y_offset;     // The pixel offset from the top edge of the image to the clipped sprite.
    WORD xs_orig;      // The pixel width of the non-clipped sprite.
    WORD ys_orig;      // The pixel height of the non-clipped sprite.
    DWORD anim_time;   // This is the frame rate of the animation.
    int directions;    // Bit flags representing which directions are present.
    BYTE reserved[12]; // The remaining 12 bytes of the reserved field are set to 0.
};                     // 40 bytes total

struct FlcHeader
{
    DWORD size;         // Size of FLIC including this header
    WORD type;          // File type 0xAF11, 0xAF12, 0xAF30, 0xAF44, ...
    WORD frames;        // Number of frames in first segment
    WORD width;         // FLIC width in pixels
    WORD height;        // FLIC height in pixels
    WORD depth;         // Bits per pixel (usually 8)
    WORD flags;         // Set to zero or to three
    DWORD speed;        // Delay between frames
    WORD reserved1;     // Set to zero
    DWORD created;      // Date of FLIC creation (FLC only)
    DWORD creator;      // Serial number or compiler id (FLC only)
    DWORD updated;      // Date of FLIC update (FLC only)
    DWORD updater;      // Serial number (FLC only), see creator
    WORD aspect_dx;     // Width of square rectangle (FLC only)
    WORD aspect_dy;     // Height of square rectangle (FLC only)
    BYTE reserved2[36]; // Set to zero (why in the specification it has 24 bytes?)
    DWORD oframe1;      // Offset to frame 1 (FLC only)
    DWORD oframe2;      // Offset to frame 2 (FLC only)
    //	BYTE	reserved3[40];	// here is a FlicAnimHeader structure placement
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(DEFINITIONS_H__CIV3FLCEDIT__INCLUDED_)