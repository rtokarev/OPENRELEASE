#ifndef _RELEASE_H_
#define _RELEASE_H_

typedef unsigned char __UINT8;
typedef short unsigned int __UINT16;
typedef unsigned int __UINT32;

typedef unsigned char U8;
typedef short unsigned int U16;
typedef unsigned int U32;

typedef unsigned int __BOOLEAN;

typedef enum {
	API_OK = 0,
	OK = 0,
	API_ERROR = 4294967295,
	API_NOT_OK = 4294967295,
	NOT_OK = 4294967295,
	PARAMETER_ERROR = 4294967294,
	API_INVALID_PARAMS = 4294967294,
	INVALID_PARAMS = 4294967294,
	API_NOT_ENOUGH_RESOURCE = 4294967293,
	NOT_ENOUGH_RESOURCE = 4294967293,
	API_NOT_SUPPORTED = 4294967292,
	NOT_SUPPORTED = 4294967292,
	API_NOT_PERMITTED = 4294967291,
	NOT_PERMITTED = 4294967291,
	API_TIMEOUT = 4294967290,
	TIMEOUT = 4294967290,
	NO_DATA_RECEIVED = 4294967289,
	DN_BUF_OVERFLOW = 4294967288,
	IPM_OK = 0,
	IPM_FAIL = 4294967295,
	IPM_ERR_ARG = 4294967295,
	IPM_ERR_INTERFACE_DOWN = 4294967294,
	IPM_ERR_INVALID_GATEWAY = 4294967293,
	IPM_ERR_EXISTENT_IP = 4294967292,
	IPM_ERR_NO_DHCP_RESPONSE = 4294967291,
} API_STATE_T;

typedef API_STATE_T DTV_STATUS_T;

typedef struct {
	__UINT16 w;
	__UINT16 h;
	__UINT16 x;
	__UINT16 y;
} EME_RECT_T;

enum EMF_PLAYING_STATUS {
	STOPPED = 0,
	PAUSED = 1,
	PLAYING = 2,
};

typedef enum EMF_PLAYING_STATUS EMF_PLAYING_STATUS_T;

/* mhal_gop.h */

/// Define GOP HW Type
typedef enum
{
    /// GOP 0 (4 Gwin Support)
    E_GOP4G_0 = 0,
    /// GOP 1 (2 Gwin Support)
    E_GOP2G_1 = 1,
    /// GOP SIGNAL (1 Gwin Support which can be used for cursor)
    E_GOP1G = 2,
} GOP_HW_Type;

/// Define GWIN color format.
typedef enum
{
    /// Color format RGB555 and Blink.
    E_GOP_COLOR_RGB555_BLINK    =0,
    /// Color format RGB565.
    E_GOP_COLOR_RGB565          =1,
    /// Color format ARGB4444.
    E_GOP_COLOR_ARGB4444        =2,
    /// Color format 555.
    E_GOP_COLOR_RGB555          =3,
    /// Color format I8 (256-entry palette).
    E_GOP_COLOR_I8              =4,
    /// Color format ARGB8888.
    E_GOP_COLOR_ARGB8888        =5,
    /// Color format ARGB1555.
    E_GOP_COLOR_ARGB1555        =6,
    /// Color format I2.
    E_GOP_COLOR_I2              =7,
    /// Color format YUV422_VU7Y8, Packet format: V7Y8,U7Y8,......
    E_GOP_COLOR_YUV422_VU7Y8    =8,
    /// Color format YUV422_VU8Y8. Packet format: V8Y8,U8Y8,......
    E_GOP_COLOR_YUV422_VU8Y8    =9,
    /// Color format I1
    E_GOP_COLOR_I1              =10,
    /// Invalid color format.
    E_GOP_COLOR_INVALID
} GOPColorType;
// mhal_gop.c:1944
// In T2, we added ARGB1555 format, so I
// remap [0: RGB1555 / Blink mode] into [6: ARGB1555].
// thus upper layer code is no need to change.
// Samuel Huang: 20080711


/// Declare the Gwin information.
typedef struct
{
    /// Usage of GWIN
    U16 bUsed;
    /// Display horizontal start position in pixel
    U32 u32DispHStart;
    /// Display vertical start position in pixel
    U32 u32DispVStart;
    /// Display width in pixel
    U32 u32DispWidth;
    /// Display height in pixel
    U32 u32DispHeight;
    /// DRAM block start address in byte
    U32 u32DRAMRBlkStart;
    /// DRAM block horizontal size in pixel
    U32 u32DRAMRBlkHSize;
    /// DRAM block vertical size in pixel
    U32 u32DRAMRBlkVSize;
    /// Source horizontal start position in pixel
    U32 u32SrcHStart;
    /// Source vertical start position in pixel
    U32 u32SrcVStart;
    /// color depth in bytes
    U32 u32ColorDepth;
    /// Color Format
    GOPColorType eColorType;
    /// Specify whether Gwin is enabled.
    U32 u32Enable;
} GopGwinInfo;

/* gfxosd_ddi.h */

typedef enum {
    GFXOSD_VOSD_0 = 0,
    GFXOSD_VOSD_1 = 1,
    GFXOSD_VOSD_2 = 2,
    GFXOSD_VOSD_3 = 3,
    GFXOSD_NUM_OF_VOSD = 4,
} GFXOSD_VOSD_ID_T;

typedef enum {
    GFXOSD_0 = 0,
    GFXOSD_1 = 1,
    GFXOSD_2 = 2,
} GFXOSD_ID_T;

typedef enum {
    GFXOSD_ARGB8888 = 0,
    GFXOSD_ARGB4444 = 1,
    GFXOSD_ARGB1555 = 2,
    GFXOSD_8BPP_PAL = 3,
} GFXOSD_FORMAT_T;

typedef enum {
    GFXOSD_8BPP = 0,
    GFXOSD_16BPP = 1,
    GFXOSD_32BPP = 2,
} GFXOSD_PIXELDEPTH_T;

typedef struct {
    __BOOLEAN                  bEnable;
    GFXOSD_ID_T                osdId;
    __UINT16                   viewX;
    __UINT16                   viewY;
    __UINT16                   viewWidth;
    __UINT16                   viewHeight;
    __UINT16                   dispX;
    __UINT16                   dispY;
    __UINT16                   dispWidth;
    __UINT16                   dispHeight;
    GFXOSD_FORMAT_T            format;
    GFXOSD_PIXELDEPTH_T        pxlDepth;
    __UINT16                   width;
    __UINT16                   height;
    __UINT16                   stride;
    __UINT8                    alphaValue;
    void *                     pAddr;
} GFXOSD_VOSD_INFO_T;

typedef enum {
    GE_FMT_I1 = 0,
    GE_FMT_I2 = 1,
    GE_FMT_I4 = 2,
    GE_FMT_I8 = 4,
    GE_FMT_1ABFGBG12355 = 7,
    GE_FMT_RGB565 = 8,
    GE_FMT_ARGB1555 = 9,
    GE_FMT_ARGB4444 = 10,
    GE_FMT_YUV422 = 14,
    GE_FMT_ARGB8888 = 15,
} GE_Buffer_Format;

typedef struct {
    unsigned int               u32Addr;
    unsigned int               u32Width;
    unsigned int               u32Height;
    unsigned int               u32Pitch;
    GE_Buffer_Format           u32ColorFmt;
} GE_BUFFER_INFO;

typedef struct
{
    GFXOSD_VOSD_ID_T           vosdId;
    GFXOSD_VOSD_INFO_T         info;
    int                        bChanged;
    int                        dispX;
    int                        dispY;
    int                        dispWidth;
    int                        dispHeight;
    int                        bPaletteChanged;
    unsigned int               palette[256];
    void *                     pAddr;
    GE_BUFFER_INFO             bufInfo;
} GFXOSD_VOSDDEF_T;


typedef struct {
    long unsigned int          bEnable:1;
    long unsigned int          bUpdate:1;
    long unsigned int          bUseDoubleBuffer:1;
    long unsigned int          doubleBufferIndex:1;
    GFXOSD_FORMAT_T            format;
    GFXOSD_PIXELDEPTH_T        pxlDepth;
    __UINT16                   width;
    __UINT16                   height;
    int                        numOfVOSDs;
    GFXOSD_VOSDDEF_T *         pVOSDDef;
    __UINT32                   wid;
    GOP_HW_Type                gopType;
    int                        stride;
    void *                     pBuffer;
    GE_BUFFER_INFO *           pFrontBufInfo;
    GE_BUFFER_INFO *           pBackBufInfo;
    GE_BUFFER_INFO             bufferInfo[2];
    int                        stretchX;
    int                        stretchY;
    __UINT16                   viewX;
    __UINT16                   viewY;
    __UINT16                   viewWidth;
    __UINT16                   viewHeight;
    __UINT16                   dispX;
    __UINT16                   dispY;
    __UINT16                   dispWidth;
    __UINT16                   dispHeight;
    __UINT8                    alphaValue;
} GFXOSD_DEF_T;

#endif
