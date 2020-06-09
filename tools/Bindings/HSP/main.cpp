
//
//		HSP3.0 plugin sample
//		onion software/onitama 2004/9
//

#include "LuminoHSP.h"

void DrawLine( BMSCR *bm, int x1, int y1, int x2, int y2, int color );

 /*------------------------------------------------------------*/
/*
		controller
*/
/*------------------------------------------------------------*/

static void newcmd2( void )
{
	//		newcmd2 命令の処理
	//
	//		(newcmd2 p1,p2)
	//		p1で指定した変数に0～(p2-1)までの乱数を代入
	//		HSP 2.61までのrnd命令と同じ動作
	//
	PVal *pval;
	APTR aptr;
	aptr = code_getva( &pval );						// 変数の取得(PValとAPTRポインタ)
	p1=code_getdi(100);								// 整数値を取得(デフォルト100)
	if (p1==0) puterror( HSPERR_DIVIDED_BY_ZERO );	// 0ならばエラー
	p2 = rand()%p1;									// 乱数を発生させる
	code_setva( pval, aptr, HSPVAR_FLAG_INT, &p2 );		// 変数に値を代入(整数値)
}


static void newcmd3( void )
{
	//		newcmd3 命令の処理
	//
	//		(newcmd3 "ファイル名",p1)
	//		"ファイル名"で指定したファイルにp1の内容を書き出す
	//		p1に指定される型は、整数、実数、文字列のいずれかで構わない
	//
	char *p;
	char pathname[_MAX_PATH];
	char out[64];
	int chk;

	p = code_gets();								// 文字列を取得
	strncpy( pathname, p, _MAX_PATH-1 );			// 取得した文字列をコピー

	chk = code_getprm();							// パラメーターを取得(型は問わない)
	if ( chk<=PARAM_END ) {
		return;										// パラメーター省略時の処理
	}

	int type = mpval->flag;							// パラメーターの型を取得
	switch( type ) {
	case HSPVAR_FLAG_STR:								// パラメーターが文字列だった時
		{
		char *str = (char *)mpval->pt;
		sprintf( out,"%s\n",str );
		break;
		}
	case HSPVAR_FLAG_DOUBLE:									// パラメーターが実数だった時
		{
		double *ptr = (double *)mpval->pt;
		sprintf( out,"%f\n",*ptr );
		break;
		}
	case HSPVAR_FLAG_INT:									// パラメーターが整数だった時
		{
		int *ptr = (int *)mpval->pt;
		sprintf( out,"%d\n",*ptr );
		break;
		}
	default:
		puterror( HSPERR_TYPE_MISMATCH );			// サポートしていない型ならばエラー
	}

	//	文字列outをファイルに追記で書き出す
	//
	FILE *fp;
	fp = fopen( pathname, "a" );
	fputs( out, fp );
	fclose( fp );
}


static void newcmd4( void )
{
	//		newcmd4 命令の処理
	//
	//		(newcmd4 x1,y1,x2,y2,color)
	//		(x1,y1)から(x2,y2)までcolorで指定した色の線を引く
	//
	BMSCR *bm;

	p1 = code_getdi(0);		// パラメータ1:数値
	p2 = code_getdi(0);		// パラメータ2:数値
	p3 = code_getdi(0);		// パラメータ3:数値
	p4 = code_getdi(0);		// パラメータ4:数値
	p5 = code_getdi(0);		// パラメータ5:数値

	bm = (BMSCR *)getbmscr( active_window );	// 現在のBMSCRを取得
	DrawLine( bm, p1, p2, p3, p4, p5 );
	bms_send( bm, 0, 0, bm->sx, bm->sy );		// 全画面を更新
}

/*------------------------------------------------------------*/

static int cmdfunc( int cmd )
{
	//		実行処理 (命令実行時に呼ばれます)
	//
	code_next();							// 次のコードを取得(最初に必ず必要です)

	switch( cmd ) {							// サブコマンドごとの分岐

	case 0x00:								// newcmd

		p1 = code_getdi( 123 );		// 整数値を取得(デフォルト123)
		stat = p1;					// システム変数statに代入
		break;

	case 0x01:								// newcmd2
		newcmd2();
		break;

	case 0x02:								// newcmd3
		newcmd3();
		break;

	case 0x03:								// newcmd4
		newcmd4();
		break;

	default:
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
	}
	return RUNMODE_RUN;
}


/*------------------------------------------------------------*/

static int ref_ival;						// 返値のための変数

static void *reffunc( int *type_res, int cmd )
{
	//		関数・システム変数の実行処理 (値の参照時に呼ばれます)
	//
	//			'('で始まるかを調べる
	//
	if ( *type != TYPE_MARK ) puterror( HSPERR_INVALID_FUNCPARAM );
	if ( *val != '(' ) puterror( HSPERR_INVALID_FUNCPARAM );
	code_next();


	switch( cmd ) {							// サブコマンドごとの分岐

	case 0x00:								// newcmd

		p1 = code_geti();				// 整数値を取得(デフォルトなし)
		ref_ival = p1 * 2;				// 返値をivalに設定
		break;

	default:
		puterror( HSPERR_UNSUPPORTED_FUNCTION );
	}

	//			'('で終わるかを調べる
	//
	if ( *type != TYPE_MARK ) puterror( HSPERR_INVALID_FUNCPARAM );
	if ( *val != ')' ) puterror( HSPERR_INVALID_FUNCPARAM );
	code_next();

	*type_res = HSPVAR_FLAG_INT;			// 返値のタイプを整数に指定する
	return (void *)&ref_ival;
}


/*------------------------------------------------------------*/

static int termfunc( int option )
{
	//		終了処理 (アプリケーション終了時に呼ばれます)
	//
	return 0;
}

/*------------------------------------------------------------*/

static int eventfunc( int event, int prm1, int prm2, void *prm3 )
{
	//		イベント処理 (HSPイベント発生時に呼ばれます)
	//
	switch( event ) {
	case HSPEVENT_GETKEY:
		{
		int *ival;
		ival = (int *)prm3;
		*ival = 123;
		return 1;
		}
	}
	return 0;
}

/*------------------------------------------------------------*/
/*
		interface
*/
/*------------------------------------------------------------*/

int WINAPI DllMain (HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
	//		DLLエントリー (何もする必要はありません)
	//
	return TRUE;
}


EXPORT void WINAPI hsp3cmdinit( HSP3TYPEINFO *info )
{
	//		プラグイン初期化 (実行・終了処理を登録します)
	//
	hsp3sdk_init( info );			// SDKの初期化(最初に行なって下さい)
	info->cmdfunc = cmdfunc;		// 実行関数(cmdfunc)の登録
	info->reffunc = reffunc;		// 参照関数(reffunc)の登録
	info->termfunc = termfunc;		// 終了関数(termfunc)の登録

	/*
	//	イベントコールバックを発生させるイベント種別を設定する
    info->option = HSPEVENT_ENABLE_GETKEY;
	info->eventfunc = eventfunc;	// イベント関数(eventfunc)の登録
	*/
}

/*------------------------------------------------------------*/

/*
EXPORT BOOL WINAPI test1( PVAL2 *pval, int p1, int p2, int p3 )
{
	//	DLL test1 (type$83)
	//		指定した変数に、「文字列テスト」を代入する。
	//		変数の型が数値型だった場合も、文字列型にする。
	//
	char *pt;
	if (pval->flag==4) pval->flag=2;	// 文字列型に
	pt=pval->pt;
	strcpy( pt,"文字列テスト" );
	return 0;
}


EXPORT BOOL WINAPI test2( HSPEXINFO *hei, int p1, int p2, int p3 )
{
	//	DLL test2 (type$202)
	//		test2 変数,"ファイル名"
	//		指定したテキストファイルを変数バッファに読み込む。
	//		ファイルサイズに合わせて変数バッファを拡張する。
	//		packfileで指定したファイルも読み込み可能。
	//
	int cursize, newsize;
	PVAL2 *pval;
	char *ep1;
	char *ep2;
	ep1 = (char *)hei->HspFunc_prm_getv();	// パラメータ1:変数
	pval = *hei->pval;						// 変数のPVAL2ポインタを取得
	ep2 = hei->HspFunc_prm_gets();			// パラメータ2:文字列
	if ( *hei->er ) return *hei->er;		// エラーチェック
	cursize = valsize( pval );				// 変数のメモリサイズを取得
	newsize = hei->HspFunc_fsize( ep2 );	// ファイルのサイズを取得
	if ( newsize<0 ) return -1;				// ファイルがない場合は終了
	if ( newsize>cursize ) {
		//	ファイル読み込みに必要なバッファを確保
		hei->HspFunc_val_realloc( pval,newsize,0 );
	}
	hei->HspFunc_fread( ep2, pval->pt, newsize, 0 );	// ファイル読み込み
	return 0;
}


EXPORT BOOL WINAPI test3( BMSCR *bm, int p1, int p2, char *p3 )
{
	//	DLL test3 (type$32)
	//		test3 p1,p2
	//		パレットのp1～p2までにグレースケールのグラデーションを
	//		設定する。
	//
	int a,n,len,grade;
	int x1,x2,px;
	char *pt;
	len=p2-p1;
	x1=p1<<8;x2=p2<<8;px=(255<<8)/len;
	for(a=0;a<len;a++) {
		n=a+p1;								// n=パレット番号
		pt=p3+(n*3);						// パレットアドレス計算
		grade=x1>>8;
		pt[0]=grade;						// Rの輝度
		pt[1]=grade;						// Gの輝度
		pt[2]=grade;						// Bの輝度
		x1+=px;
	}
	bms_setpal( bm, bm->pals, p3 );
	bms_update( bm );
	return 0;
}


EXPORT BOOL WINAPI test4( HSPEXINFO *hei, int p1, int p2, int p3 )
{
	//	DLL test4 (type$202)
	//		test4 変数,数値1,数値2,数値3,数値4,数値5,数値6
	//		多数のパラメーターを受け取るテスト。
	//		数値1～6までの合計を変数aに代入。
	//
	int ep2,ep3,ep4,ep5,ep6,ep7;
	int *ep1;
	ep1 = (int *)hei->HspFunc_prm_getv();	// パラメータ1:変数
	ep2 = hei->HspFunc_prm_getdi(0);		// パラメータ2:数値
	ep3 = hei->HspFunc_prm_getdi(0);		// パラメータ3:数値
	ep4 = hei->HspFunc_prm_getdi(0);		// パラメータ4:数値
	ep5 = hei->HspFunc_prm_getdi(0);		// パラメータ5:数値
	ep6 = hei->HspFunc_prm_getdi(0);		// パラメータ6:数値
	ep7 = hei->HspFunc_prm_getdi(0);		// パラメータ7:数値
	if ( *hei->er ) return *hei->er;		// エラーチェック
	*ep1 = ep2+ep3+ep4+ep5+ep6+ep7;
	return 0;
}


EXPORT BOOL WINAPI test5( HSPEXINFO *hei, int p1, int p2, int p3 )
{
	//	DLL test5 (type$202)
	//		test5 X1座標,Y1座標,X2座標,Y2座標,RGB値
	//
	BMSCR *bm;
	int ep1,ep2,ep3,ep4,ep5;
	ep1 = hei->HspFunc_prm_getdi(0);		// パラメータ1:数値
	ep2 = hei->HspFunc_prm_getdi(0);		// パラメータ2:数値
	ep3 = hei->HspFunc_prm_getdi(0);		// パラメータ3:数値
	ep4 = hei->HspFunc_prm_getdi(0);		// パラメータ4:数値
	ep5 = hei->HspFunc_prm_getdi(0);		// パラメータ5:数値
	if ( *hei->er ) return *hei->er;		// エラーチェック
	bm = (BMSCR *)hei->HspFunc_getbmscr( *hei->actscr );	// 現在のBMSCRを取得
	DrawLine( bm, ep1, ep2, ep3, ep4, ep5 );
	return 0;
}


EXPORT BOOL WINAPI test6( HSPEXINFO *hei, int p1, int p2, int p3 )
{
	//	DLL test6 (type$202)
	//		test6 prm
	//
	int type,val;
	int *strsize;
	char *es1;
	char *refstr;
	int ep1;
	strsize = hei->strsize;
	refstr = hei->refstr;
	type = *hei->nptype;
	val = *hei->npval;
	switch( type ) {
	case 1:
		ep1 = hei->HspFunc_prm_getdi(0);		// パラメータ1:数値
		*strsize = ep1;
		break;
	case 2:
		es1 = hei->HspFunc_prm_gets();			// パラメータ1:文字列
		strcpy( refstr, es1 );
		break;
	case 4:
		ep1 = hei->HspFunc_prm_getv();			// パラメータ1:変数
		*strsize = ep1;
		break;
	default:
		return 2;
	}
	return -type;
}
*/

/*------------------------------------------------------------*/
/*
		(おまけ) Line Draw Routines (フルカラー用)
*/
/*------------------------------------------------------------*/

void DrawLine( BMSCR *bm, int x1, int y1, int x2, int y2, int color )
{
	char *lpDest;		// 描画画面のバッファ
	int nDestWByte;		// 描画座標幅byte数
	int nDestWidth;		// 描画座標幅
	int nDestHeight;	// 描画座標高さ
	int DestSize;		// 描画画面バッファのサイズ
    int i, x, y, dx, dy, addx, addy;
	short color1;
	unsigned char color2;
	unsigned char *up;
	char *dwDest;
    int cnt = 0;

	lpDest = (char *)bm->pBit;
	nDestWidth = bm->sx;
	nDestWByte = bm->sx*3;
	nDestHeight = bm->sy;
	DestSize = bm->sx*bm->sy*3;

	dwDest = lpDest + ((nDestHeight-1)*nDestWByte);
	color1 = (short)(color & 0xffff);
	color2 = (unsigned char)( (color & 0xff0000 ) >>16 );

    dx = x2 - x1;
    if (dx < 0){ addx = -1; dx = -dx; } else addx = 1;
    dy = y2 - y1;
    if (dy < 0){ addy = -1; dy = -dy; } else addy = 1;
    x = x1; y = y1;

    if (dx > dy){
        for (i = 0; i < dx; ++i){
			if ((y>=0)&&(y<nDestHeight)&&(x>=0)&&(x<nDestWidth)) {
				up = (unsigned char *)(dwDest - nDestWByte*y + x*3 );
				*(short *)up = color1; up[2] = color2;
			}
            cnt += dy; x += addx;
            if (cnt >= dx){ cnt-=dx; y+=addy; }
        }
    }
    else{
        for (i = 0; i < dy; ++i){
			if ((y>=0)&&(y<nDestHeight)&&(x>=0)&&(x<nDestWidth)) {
				up = (unsigned char *)(dwDest - nDestWByte*y + x*3 );
				*(short *)up = color1; up[2] = color2;
			}
            cnt += dx; y += addy;
            if (cnt >= dy){ cnt-=dy; x+=addx; }
        }
    }
	return;
}


/*----------------------------------------------------------------*/
