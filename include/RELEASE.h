#include <linux/types.h>
#include <nano-X.h>

typedef unsigned char __UINT8;
typedef short unsigned int __UINT16;
typedef unsigned int __UINT32;
typedef unsigned int __BOOLEAN;

typedef enum API_STATE_T {
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

typedef struct API_MUSIC_INFO {
	__UINT32                   duration;             /*     0     4 */
	__UINT32                   samplerRate;          /*     4     4 */
	__UINT32                   numOfFrames;          /*     8     4 */
	__UINT32                   bitRate;              /*    12     4 */
	__BOOLEAN                  bVBR;                 /*    16     4 */

} API_MUSIC_INFO_T;

typedef struct API_MOVIE_INFO {
	__UINT32                   duration;             /*     0     4 */
	__UINT32                   hres;                 /*     4     4 */
	__UINT32                   vres;                 /*     8     4 */
	__UINT32                   vcodec;               /*    12     4 */
	__UINT32                   acodec;               /*    16     4 */

} API_MOVIE_INFO_T;

extern API_STATE_T API_EME_GetMusicInfo(char * pszFilePath, API_MUSIC_INFO_T * pMusicInfo);
extern API_STATE_T API_EME_GetMovieInfo(char * pszFilePath, API_MOVIE_INFO_T * pMovieInfo);

/* EME */

typedef struct EME_RECT {
	__UINT16                   w;                    /*     0     2 */
	__UINT16                   h;                    /*     2     2 */
	__UINT16                   x;                    /*     4     2 */
	__UINT16                   y;                    /*     6     2 */
} EME_RECT_T;

//extern API_STATE_T API_EME_PreviewDivx(char * pszFilePath, EME_RECT_T rect);
//extern API_STATE_T API_EME_DecodePreviewImage(char * pszFilePath, int x, int y, int width, int height, __BOOLEAN bDisplayImageDirectly, __UINT32 timeStamp);
//extern API_STATE_T API_EME_DecodePreviewImageUsingTask(char * pszFilePath, int x, int y, int width, int height, __BOOLEAN bDisplayImageDirectly, __UINT32 timeStamp);

/* UI MUTIL */


struct tagUI_WIN_COORD {
	GR_COORD                   resolution[3];
};

typedef struct tagUI_WIN_COORD UI_COORD_T;

enum tagUI_MSG_RESULT_T {
	MSG_ERROR = 4294967295,
	MSG_HANDLED = 0,
	MSG_NOT_HANDLED = 1,
};

typedef enum tagUI_MSG_RESULT_T UI_MSG_RESULT_T; 

enum tagUI_MSG_BOX_OPTION_T {
	MSG_BOX_HCENTER = 2,
	MSG_BOX_HLEFT = 4,
	MSG_BOX_HRIGHT = 8,
	MSG_BOX_VCENTER = 16,
	MSG_BOX_VTOP = 32,
	MSG_BOX_VBOTTOM = 64,
	MSG_BOX_OKCANCEL = 128,
	MSG_BOX_YESNO = 256,
	MSG_BOX_ENTER = 512,
	MSG_BOX_CLOSE = 1024,
	MSG_BOX_NO_CLOSE = 2048,
	MSG_BOX_ICON_ERROR = 4096,
	MSG_BOX_ICON_EXCLAMATION = 8192,
	MSG_BOX_ICON_QUESTION = 16384,
	MSG_BOX_ICON_INFO = 32768,
	MSG_BOX_ICON_SIMPLINK = 65536,
	MSG_BOX_HPOPUP = 131072,
	MSG_BOX_VPOPUP = 262144,
	MSG_BOX_TRANSPARENT = 524288,
	MSG_BOX_OPAQUE = 1048576,
	MSG_BOX_FOCUS_YES = 2097152,
	MSG_BOX_VIDEOADVANCESAVE = 4194304,
	MSG_BOX_EMF = 8388608,
	MSG_BOX_NO_KEY = 16777216,
	MSG_BOX_FIT_SIZE = 33554432,
};

typedef enum tagUI_MSG_BOX_OPTION_T UI_MSG_BOX_OPTION_T; 

struct tagUI_STRING_T {
	__UINT8 *                  psz;
	int                        nLength;
};

typedef struct tagUI_STRING_T UI_STRING_T;

enum tagUI_NAVMAP_OPTION {
	NAV_VERT = 1,
	NAV_HORIZ = 2,
	NAV_VERT_CIRCULAR = 4,
	NAV_HORIZ_CIRCULAR = 8,
};

typedef enum tagUI_NAVMAP_OPTION UI_NAVMAP_OPTION_T;

struct tagUI_NAVMAP {
	int                        nColumn;
	int                        nRow;
	int                        xCur;
	int                        yCur;
	UI_NAVMAP_OPTION_T         option;
	GR_WINDOW_ID               wids[1];
};

typedef struct tagUI_NAVMAP UI_NAVMAP_T;

typedef UI_MSG_RESULT_T (*BUTTON_CALLBACK_T)(GR_WINDOW_ID, __UINT32);
typedef UI_MSG_RESULT_T (*UI_WIN_HANDLER_T)(GR_WINDOW_ID, void *, __UINT32, __UINT32 *);

struct tagUI_MSGBOX_INFO_T {
	__UINT32                   id;
	UI_MSG_BOX_OPTION_T        option;
	UI_STRING_T                szMsg;
	UI_NAVMAP_T *              pNavMap;
	__UINT32                   timeOut;
	UI_WIN_HANDLER_T           pfnWinHandler;
	__BOOLEAN                  bDrawBG;
	UI_COORD_T                 winX;
	UI_COORD_T                 winY;
	UI_COORD_T                 winWidth;
	UI_COORD_T                 winHeight;
	UI_COORD_T                 bottomHeight;
	UI_COORD_T                 iconX;
	UI_COORD_T                 iconY;
	UI_COORD_T                 textX;
	UI_COORD_T                 textY;
	UI_COORD_T                 textWidth;
	UI_COORD_T                 buttonX;
	UI_COORD_T                 buttonY;
	UI_COORD_T                 buttonWidth;
	UI_COORD_T                 buttonHeight;
	UI_COORD_T                 buttonHGap;
	UI_COORD_T                 buttonVGap;
	UI_COORD_T                 lineGap;
	UI_COORD_T                 lineLowGap;
	UI_COORD_T                 simplinkLogoX;
	UI_COORD_T                 simplinkLogoY;
};

typedef struct tagUI_MSGBOX_INFO_T UI_MSGBOX_INFO_T;

extern GR_WINDOW_ID UI_MUTIL_CreateMsgBox(GR_WINDOW_ID pid, __UINT32 id, char * pszMsg, int nStrLength, 
				UI_MSG_BOX_OPTION_T option, BUTTON_CALLBACK_T pfnCallBack, UI_WIN_HANDLER_T pfnWinHandler, 
				__UINT32 nTimeOut);

extern UI_MSG_RESULT_T _MUTIL_MsgBoxDrawHandler(GR_WINDOW_ID wid, UI_MSGBOX_INFO_T * pMsgBoxInfo);
extern UI_MSG_RESULT_T _MUTIL_MsgBoxKeyHandler(GR_WINDOW_ID wid, UI_MSGBOX_INFO_T * pMsgBoxInfo, __UINT32 key);
extern UI_MSG_RESULT_T _MUTIL_MsgBoxMsgHandler(GR_WINDOW_ID wid, void * pWinData, __UINT32 msg, __UINT32 * param);
