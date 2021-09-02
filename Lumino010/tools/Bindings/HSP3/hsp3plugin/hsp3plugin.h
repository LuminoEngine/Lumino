
//
//	hsp3debug.cpp header
//
#ifndef __hsp3plugin_h
#define __hsp3plugin_h

//	VC++�̏ꍇ
#ifdef __cplusplus
#define EXPORT extern "C" __declspec (dllexport)
#else
#define EXPORT __declspec (dllexport)
#endif

#include "hsp3debug.h"			// hsp3 error code
#include "hsp3struct.h"			// hsp3 core define
#include "hspwnd.h"				// hsp3 windows define

extern int p1,p2,p3,p4,p5,p6;
extern int *type;
extern int *val;
extern PVal *mpval;		// Master PVal pointer
extern HSPCTX *ctx;		// Current Context
extern HSPEXINFO *exinfo;	// Info for Plugins

void hsp3sdk_init( HSP3TYPEINFO *info );
int code_getprm( void );
void bms_send( BMSCR *bm, int x, int y, int sx, int sy );

#define code_next exinfo->HspFunc_prm_next
#define puterror exinfo->HspFunc_puterror
#define code_event exinfo->HspFunc_hspevent

#define code_geti exinfo->HspFunc_prm_geti
#define code_getdi exinfo->HspFunc_prm_getdi
#define code_gets exinfo->HspFunc_prm_gets
#define code_getds exinfo->HspFunc_prm_getds

#define getbmscr exinfo->HspFunc_getbmscr
#define getobj exinfo->HspFunc_getobj
#define setobj exinfo->HspFunc_setobj
#define addobj exinfo->HspFunc_addobj
#define getproc exinfo->HspFunc_getproc
#define seekproc exinfo->HspFunc_seekproc

#define code_getlb exinfo->HspFunc_prm_getlb
#define code_getpval exinfo->HspFunc_prm_getpval
#define code_getva exinfo->HspFunc_prm_getva
#define code_setva exinfo->HspFunc_prm_setva
#define hspmalloc exinfo->HspFunc_malloc
#define hspfree exinfo->HspFunc_free
#define hspexpand exinfo->HspFunc_expand

#define addirq exinfo->HspFunc_addirq
#define registvar exinfo->HspFunc_registvar
#define code_setpc exinfo->HspFunc_setpc
#define code_call exinfo->HspFunc_call

#define code_getd exinfo->HspFunc_prm_getd
#define code_getdd exinfo->HspFunc_prm_getdd

#define stat ctx->stat
#define active_window (*exinfo->actscr)

#endif
