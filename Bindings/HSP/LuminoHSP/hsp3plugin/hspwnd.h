
//
//	hspwnd.cpp header
//
#ifndef __hspwnd_h
#define __hspwnd_h

//	Window Object Info
//
#define HSPOBJ_OPTION_SETFONT	0x100

#define MESSAGE_HSPOBJ	0x4000
#define HSPOBJ_LIMIT_DEFAULT	1024

#define HSPOBJ_INPUT_STR 2
#define HSPOBJ_INPUT_DOUBLE 3
#define HSPOBJ_INPUT_INT 4
#define HSPOBJ_INPUT_MULTILINE 0x100
#define HSPOBJ_INPUT_READONLY 0x200
#define HSPOBJ_INPUT_HSCROLL 0x400

#define HSPOBJ_NONE 0
#define HSPOBJ_TAB_ENABLE 1
#define HSPOBJ_TAB_DISABLE 2
#define HSPOBJ_TAB_SKIP 3

typedef struct HSP3VARSET
{
	//	PVal entry structure
	//
	int type;
	PVal *pval;
	APTR aptr;
	void *ptr;
} HSP3VARSET;

typedef struct HSPOBJINFO
{
	//		Object Info (3.0)
	//
	short	owmode;		// objectのmode
	short	option;		// objectのoption(未使用・内部オブジェクトは0)
	void	*bm;		// objectが配置されているBMSCR構造体のポインタ
	HWND	hCld;		// objectのhandle
	int		owid;		// objectのValue(汎用)
	int		owsize;		// objectの使用サイズ(汎用)

	HSP3VARSET varset;	// objectから設定される変数の情報

	//		callback function
	void	(*func_notice)( struct HSPOBJINFO *, int );
	void	(*func_objprm)( struct HSPOBJINFO *, int, void * );
	void	(*func_delete)( struct HSPOBJINFO * );

} HSPOBJINFO;


#define BMSCR_FLAG_NOUSE	0
#define BMSCR_FLAG_INUSE	1
#define BMSCR_PALMODE_FULLCOLOR	0
#define BMSCR_PALMODE_PALETTECOLOR	1

#define HSPWND_TYPE_NONE 0
#define HSPWND_TYPE_BUFFER 1
#define HSPWND_TYPE_MAIN 2
#define HSPWND_TYPE_BGSCR 3

enum {
BMSCR_SAVEPOS_MOSUEX,
BMSCR_SAVEPOS_MOSUEY,
BMSCR_SAVEPOS_MOSUEZ,
BMSCR_SAVEPOS_MOSUEW,
BMSCR_SAVEPOS_MAX,
};


typedef struct BMSCR
{
	//
	//		Window data structure
	//
	int		flag;				// used flag
	int		sx;					// X-size
	int		sy;					// Y-size
	int		palmode;			// palmode
	HDC		hdc;				// buffer HDC
	BYTE	*pBit;				// bitmap pointer
	BITMAPINFOHEADER *pbi;		// infoheader
	HBITMAP	dib;				// bitmap handle(DIB)
	HBITMAP	old;				// bitmap handle(OLD)
	RGBQUAD *pal;				// palette table
	HPALETTE hpal;				// palette handle
	HPALETTE holdpal;			// palette handle (old)
	int		pals;				// palette entries
	HWND	hwnd;				// window handle
	HINSTANCE hInst;			// Instance of program
	int		infsize;			// *pbi alloc memory size
	int		bmpsize;			// *pBit alloc memory size

	//		Window object setting
	//
	int		type;				// setting type
	int		wid;				// window ID
	short	fl_dispw;			// display window flag
	short	fl_udraw;			// update draw window
	int		wx,wy,wchg;			// actual window size x,y
	int		viewx,viewy;		// buffer view point x,y
	int		lx,ly;				// buffer view size x,y
	int		cx,cy;				// object cursor x,y
	int		ox,oy,py;			// object size x,y,py
	int		texty;				// text Y-axis size
	int		gx,gy,gmode;		// gcopy size
	HBRUSH	hbr;				// BRUSH handle
	HPEN	hpn;				// PEN handle
	HFONT	hfont;				// FONT handle
	HFONT	holdfon;			// FONT handle (old)
	COLORREF color;				// text color code
	int		textspeed;			// slow text speed
	int		cx2,cy2;			// slow text cursor x,y
	int		tex,tey;			// slow text limit x,y
	char	*prtmes;			// slow message ptr
	int		focflg;				// focus set flag
	int		objmode;			// object set mode
	LOGFONT	logfont;			// logical font
	int		style;				// extra window style
	int		gfrate;				// halftone copy rate
	int		tabmove;			// object TAB move mode
	int		sx2;				// actual bitmap X size
	SIZE	printsize;			// print,mes extent size

	//		Class depend data
	//
	int		objstyle;					// objects style
	HSPOBJINFO *mem_obj;				// Window objects
	int objmax;							// Max number of obj
	int objlimit;						// Limit number of obj
	short savepos[BMSCR_SAVEPOS_MAX];	// saved position
	void *master_hspwnd;				// Parent hspwnd class
	short	palcolor;					// Palette color code
	short	textstyle;					// Extra text style
	short	framesx, framesy;			// Window frame xy-size

	int		imgbtn;						// Custom Button Flag (-1=none)
	short	btn_x1, btn_y1;				// Custom Button Image X,Y
	short	btn_x2, btn_y2;				// Custom Button Image X,Y (press)
	short	btn_x3, btn_y3;				// Custom Button Image X,Y (mouse over)
	short	divx, divy;					// Divide value for CEL
	short	divsx, divsy;				// CEL size
	short	celofsx, celofsy;			// CEL center offset

} BMSCR;


#endif
