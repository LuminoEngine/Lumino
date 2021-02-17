
;============================================================
; Lumino ヘルプファイル
;============================================================

%dll
Lumino

%ver
0.10.0

%date
2021/2/11

%author
lriki

%note
lumino.as をインクルードしてください

%type
拡張命令

%url
http ://nnmy.sakura.ne.jp/



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_SetZeros
すべての要素を 0.0 に設定してインスタンスを初期化します。
%group
LNVector3
%prm
vector3
[in] vector3 : (Vector3 型の値) Vector3

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_Set
指定した値を使用してインスタンスを初期化します。
%group
LNVector3
%prm
vector3, x, y, z
[in] vector3 : (Vector3 型の値) Vector3
[in] x       : 
[in] y       : 
[in] z       : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_Get
get
%group
LNVector3
%prm
vector3, outX, outY, outZ
[in]  vector3 : (Vector3 型の値) Vector3
[out] outX    : 
[out] outY    : 
[out] outZ    : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_Length
ベクトルの長さを返します。
%group
LNVector3
%prm
vector3, outReturn
[in]  vector3   : (Vector3 型の値) Vector3
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_LengthSquared
ベクトルの長さの 2 乗を返します。
%group
LNVector3
%prm
vector3, outReturn
[in]  vector3   : (Vector3 型の値) Vector3
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_MutatingNormalize
このベクトルを正規化します。
%group
LNVector3
%prm
vector3
[in] vector3 : (Vector3 型の値) Vector3

stat : 0=成功, 負値=失敗

%inst
ベクトルの長さが 0 の場合は正規化を行いません。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_NormalizeXYZ
指定ベクトルを正規化したベクトルを返します。
%group
LNVector3
%prm
x, y, z, outReturn
[in]  x         : 
[in]  y         : 
[in]  z         : 
[out] outReturn(0) : (Vector3 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_Normalize
指定ベクトルを正規化したベクトルを返します。
%group
LNVector3
%prm
vec, outReturn
[in]  vec       : (Vector3 型の値) 
[out] outReturn(0) : (Vector3 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector4_SetZeros
すべての要素を 0.0 に設定してインスタンスを初期化します。
%group
LNVector4
%prm
vector4
[in] vector4 : (Vector4 型の値) Vector4

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector4_Set
指定した値を使用してインスタンスを初期化します。
%group
LNVector4
%prm
vector4, x, y, z, w
[in] vector4 : (Vector4 型の値) Vector4
[in] x       : 
[in] y       : 
[in] z       : 
[in] w       : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNQuaternion_SetZeros
単位クォータニオンを設定してインスタンスを初期化します。
%group
LNQuaternion
%prm
quaternion
[in] quaternion : (Quaternion 型の値) Quaternion

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNQuaternion_Set
指定した値を使用してインスタンスを初期化します。
%group
LNQuaternion
%prm
quaternion, x, y, z, w
[in] quaternion : (Quaternion 型の値) Quaternion
[in] x          : 
[in] y          : 
[in] z          : 
[in] w          : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNQuaternion_SetFromAxis
回転軸と回転角度を指定してインスタンスを初期化します。
%group
LNQuaternion
%prm
quaternion, axis, r
[in] quaternion : (Quaternion 型の値) Quaternion
[in] axis       : (Vector3 型の値) 
[in] r          : 

stat : 0=成功, 負値=失敗

%inst
axis が単位ベクトルでなければ正規化してから計算を行います。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMatrix_SetZeros
単位行列を設定してインスタンスを初期化します。
%group
LNMatrix
%prm
matrix
[in] matrix : (Matrix 型の値) Matrix

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMatrix_Set
各要素を指定してインスタンスを初期化します。
%group
LNMatrix
%prm
matrix, m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44
[in] matrix : (Matrix 型の値) Matrix
[in] m11    : 
[in] m12    : 
[in] m13    : 
[in] m14    : 
[in] m21    : 
[in] m22    : 
[in] m23    : 
[in] m24    : 
[in] m31    : 
[in] m32    : 
[in] m33    : 
[in] m34    : 
[in] m41    : 
[in] m42    : 
[in] m43    : 
[in] m44    : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColor_SetZeros
すべての要素を 0.0 で初期化します。
%group
LNColor
%prm
color
[in] color : (Color 型の値) Color

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColor_Set
各要素を指定して初期化します。
%group
LNColor
%prm
color, r_, g_, b_, a_
[in] color        : (Color 型の値) Color
[in] r_           : 
[in] g_           : 
[in] b_           : 
[in] a_(1.000000) : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColorTone_SetZeros
すべての要素を 0.0 で初期化します。
%group
LNColorTone
%prm
colortone
[in] colortone : (ColorTone 型の値) ColorTone

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColorTone_Set
各要素を指定して初期化します。
%group
LNColorTone
%prm
colortone, r_, g_, b_, s_
[in] colortone : (ColorTone 型の値) ColorTone
[in] r_        : 
[in] g_        : 
[in] b_        : 
[in] s_        : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPoint_SetZeros
すべての要素を 0 で初期化します。
%group
LNPoint
%prm
point
[in] point : (Point 型の値) Point

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPoint_Set
位置を指定して初期化します。
%group
LNPoint
%prm
point, x_, y_
[in] point : (Point 型の値) Point
[in] x_    : 
[in] y_    : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPoint_Get
各要素の値を取得します。
%group
LNPoint
%prm
point, outX, outY
[in]  point : (Point 型の値) Point
[out] outX  : 
[out] outY  : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSize_SetZeros
すべての要素を 0 で初期化します。
%group
LNSize
%prm
size
[in] size : (Size 型の値) Size

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSize_Set
幅と高さを指定して初期化します。
%group
LNSize
%prm
size, w, h
[in] size : (Size 型の値) Size
[in] w    : 
[in] h    : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRect_SetZeros
すべての要素を 0 で初期化します。
%group
LNRect
%prm
rect
[in] rect : (Rect 型の値) Rect

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRect_Set
位置とサイズを指定して初期化します。
%group
LNRect
%prm
rect, x, y, width, height
[in] rect   : (Rect 型の値) Rect
[in] x      : 
[in] y      : 
[in] width  : 
[in] height : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRect_GetLeft
左辺の x 軸の値を取得します。
%group
LNRect
%prm
rect, outReturn
[in]  rect      : (Rect 型の値) Rect
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRect_SetSize
幅と高さを設定します。
%group
LNRect
%prm
rect, size
[in] rect : (Rect 型の値) Rect
[in] size : (Size 型の値) 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRect_GetSize
幅と高さを取得します。
%group
LNRect
%prm
rect, outReturn
[in]  rect      : (Rect 型の値) Rect
[out] outReturn(0) : (Size 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNThickness_SetZeros
すべての要素を 0 で初期化します。
%group
LNThickness
%prm
thickness
[in] thickness : (Thickness 型の値) Thickness

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNThickness_Set
各辺の幅を指定して初期化します。
%group
LNThickness
%prm
thickness, left_, top_, right_, bottom_
[in] thickness : (Thickness 型の値) Thickness
[in] left_     : 
[in] top_      : 
[in] right_    : 
[in] bottom_   : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCornerRadius_SetZeros
すべての要素を 0 で初期化します。
%group
LNCornerRadius
%prm
cornerradius
[in] cornerradius : (CornerRadius 型の値) CornerRadius

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCornerRadius_Set
各要素の値を指定して初期化します。
%group
LNCornerRadius
%prm
cornerradius, topLeft, topRight, bottomRight, bottomLeft
[in] cornerradius : (CornerRadius 型の値) CornerRadius
[in] topLeft      : 
[in] topRight     : 
[in] bottomRight  : 
[in] bottomLeft   : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNObject_Release
オブジェクトの参照を開放します。
%group
LNObject
%prm
object
[in] object : Object のハンドル

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNObject_Retain
オブジェクトの参照を取得します。
%group
LNObject
%prm
object
[in] object : Object のハンドル

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNObject_GetReferenceCount
オブジェクトの参照カウントを取得します。これは内部的に使用される命令です。
%group
LNObject
%prm
object, outReturn
[in]  object    : Object のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPromiseFailureDelegate_Create

%group
LNPromiseFailureDelegate
%prm
callback, outPromiseFailureDelegate
[in]  callback                  : 
[out] outPromiseFailureDelegate(0) : 作成された PromiseFailureDelegate のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
PromiseFailureDelegate は Object のサブクラスです。PromiseFailureDelegate ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVariant_Create
init.
%group
LNVariant
%prm
outVariant
[out] outVariant(0) : 作成された Variant のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Variant は Object のサブクラスです。Variant ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVariant_SetInt
setInt
%group
LNVariant
%prm
variant, value
[in] variant : Variant のハンドル
[in] value   : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Variant は Object のサブクラスです。Variant ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVariant_GetInt
getInt
%group
LNVariant
%prm
variant, outReturn
[in]  variant   : Variant のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Variant は Object のサブクラスです。Variant ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestDelegate1_Create

%group
LNZVTestDelegate1
%prm
callback, outZVTestDelegate1
[in]  callback           : 
[out] outZVTestDelegate1(0) : 作成された ZVTestDelegate1 のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestDelegate1 は Object のサブクラスです。ZVTestDelegate1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestDelegate2_Create

%group
LNZVTestDelegate2
%prm
callback, outZVTestDelegate2
[in]  callback           : 
[out] outZVTestDelegate2(0) : 作成された ZVTestDelegate2 のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestDelegate2 は Object のサブクラスです。ZVTestDelegate2 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestDelegate3_Create

%group
LNZVTestDelegate3
%prm
callback, outZVTestDelegate3
[in]  callback           : 
[out] outZVTestDelegate3(0) : 作成された ZVTestDelegate3 のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestDelegate3 は Object のサブクラスです。ZVTestDelegate3 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestEventHandler1_Create

%group
LNZVTestEventHandler1
%prm
callback, outZVTestEventHandler1
[in]  callback               : 
[out] outZVTestEventHandler1(0) : 作成された ZVTestEventHandler1 のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestEventHandler1 は Object のサブクラスです。ZVTestEventHandler1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestEventHandler2_Create

%group
LNZVTestEventHandler2
%prm
callback, outZVTestEventHandler2
[in]  callback               : 
[out] outZVTestEventHandler2(0) : 作成された ZVTestEventHandler2 のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestEventHandler2 は Object のサブクラスです。ZVTestEventHandler2 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestPromise1_ThenWith

%group
LNZVTestPromise1
%prm
zvtestpromise1, callback
[in] zvtestpromise1 : ZVTestPromise1 のハンドル
[in] callback       : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestPromise1 は Object のサブクラスです。ZVTestPromise1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestPromise1_CatchWith

%group
LNZVTestPromise1
%prm
zvtestpromise1, callback
[in] zvtestpromise1 : ZVTestPromise1 のハンドル
[in] callback       : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestPromise1 は Object のサブクラスです。ZVTestPromise1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestPromise2_ThenWith

%group
LNZVTestPromise2
%prm
zvtestpromise2, callback
[in] zvtestpromise2 : ZVTestPromise2 のハンドル
[in] callback       : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestPromise2 は Object のサブクラスです。ZVTestPromise2 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestPromise2_CatchWith

%group
LNZVTestPromise2
%prm
zvtestpromise2, callback
[in] zvtestpromise2 : ZVTestPromise2 のハンドル
[in] callback       : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestPromise2 は Object のサブクラスです。ZVTestPromise2 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestClass1_Create
init method.
%group
LNZVTestClass1
%prm
outZVTestClass1
[out] outZVTestClass1(0) : 作成された ZVTestClass1 のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestClass1 は Object のサブクラスです。ZVTestClass1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestClass1_SetTestDelegate1
setTestDelegate1 method.
%group
LNZVTestClass1
%prm
zvtestclass1, value
[in] zvtestclass1 : ZVTestClass1 のハンドル
[in] value        : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestClass1 は Object のサブクラスです。ZVTestClass1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestClass1_SetTestDelegate2
setTestDelegate2 method.
%group
LNZVTestClass1
%prm
zvtestclass1, value
[in] zvtestclass1 : ZVTestClass1 のハンドル
[in] value        : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestClass1 は Object のサブクラスです。ZVTestClass1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestClass1_SetTestDelegate3
setTestDelegate3 method.
%group
LNZVTestClass1
%prm
zvtestclass1, value
[in] zvtestclass1 : ZVTestClass1 のハンドル
[in] value        : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestClass1 は Object のサブクラスです。ZVTestClass1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestClass1_CallTestDelegate1
callTestDelegate1 method.
%group
LNZVTestClass1
%prm
zvtestclass1, a
[in] zvtestclass1 : ZVTestClass1 のハンドル
[in] a            : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestClass1 は Object のサブクラスです。ZVTestClass1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestClass1_CallTestDelegate2
callTestDelegate2 method.
%group
LNZVTestClass1
%prm
zvtestclass1, a, b, outReturn
[in]  zvtestclass1 : ZVTestClass1 のハンドル
[in]  a            : 
[in]  b            : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestClass1 は Object のサブクラスです。ZVTestClass1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestClass1_CallTestDelegate3
callTestDelegate3 method. (create object in internal)
%group
LNZVTestClass1
%prm
zvtestclass1
[in] zvtestclass1 : ZVTestClass1 のハンドル

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestClass1 は Object のサブクラスです。ZVTestClass1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestClass1_LoadAsync
Promise test method. (static)
%group
LNZVTestClass1
%prm
filePath, outReturn
[in]  filePath  : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestClass1 は Object のサブクラスです。ZVTestClass1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestClass1_ExecuteAsync
Promise test method. (instance)
%group
LNZVTestClass1
%prm
zvtestclass1, outReturn
[in]  zvtestclass1 : ZVTestClass1 のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestClass1 は Object のサブクラスです。ZVTestClass1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestClass1_GetFilePath
Promise test method.
%group
LNZVTestClass1
%prm
zvtestclass1, outReturn
[in]  zvtestclass1 : ZVTestClass1 のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestClass1 は Object のサブクラスです。ZVTestClass1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestClass1_ConnectOnEvent1
connectOnEvent1 method.
%group
LNZVTestClass1
%prm
zvtestclass1, handler, outReturn
[in]  zvtestclass1 : ZVTestClass1 のハンドル
[in]  handler      : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestClass1 は Object のサブクラスです。ZVTestClass1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestClass1_RaiseEvent1
raiseEvent1 method.
%group
LNZVTestClass1
%prm
zvtestclass1
[in] zvtestclass1 : ZVTestClass1 のハンドル

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestClass1 は Object のサブクラスです。ZVTestClass1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestClass1_ConnectOnEvent2
connectOnEvent2 method.
%group
LNZVTestClass1
%prm
zvtestclass1, handler, outReturn
[in]  zvtestclass1 : ZVTestClass1 のハンドル
[in]  handler      : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestClass1 は Object のサブクラスです。ZVTestClass1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestClass1_RaiseEvent2
raiseEvent2 method.
%group
LNZVTestClass1
%prm
zvtestclass1
[in] zvtestclass1 : ZVTestClass1 のハンドル

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestClass1 は Object のサブクラスです。ZVTestClass1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestEventArgs1_Create
init method.
%group
LNZVTestEventArgs1
%prm
outZVTestEventArgs1
[out] outZVTestEventArgs1(0) : 作成された ZVTestEventArgs1 のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestEventArgs1 は Object のサブクラスです。ZVTestEventArgs1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestEventArgs1_CreateWithValue
init method.
%group
LNZVTestEventArgs1
%prm
v, outZVTestEventArgs1
[in]  v                   : 
[out] outZVTestEventArgs1(0) : 作成された ZVTestEventArgs1 のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestEventArgs1 は Object のサブクラスです。ZVTestEventArgs1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestEventArgs1_GetValue
value method.
%group
LNZVTestEventArgs1
%prm
zvtesteventargs1, outReturn
[in]  zvtesteventargs1 : ZVTestEventArgs1 のハンドル
[out] outReturn(0)     : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ZVTestEventArgs1 は Object のサブクラスです。ZVTestEventArgs1 ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLog_SetLevel
setLevel
%group
LNLog
%prm
level
[in] level : 
                LN_LOG_LEVEL_UNKNOWN
                
                LN_LOG_LEVEL_VERBOSE
                
                LN_LOG_LEVEL_DEBUG
                
                LN_LOG_LEVEL_INFO
                
                LN_LOG_LEVEL_WARNING
                
                LN_LOG_LEVEL_ERROR
                
                LN_LOG_LEVEL_FATAL
                
                LN_LOG_LEVEL_DISBLE
                

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLog_AllocConsole
allocConsole
%group
LNLog
%prm


stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLog_Write
write
%group
LNLog
%prm
level, tag, text
[in] level : 
                LN_LOG_LEVEL_UNKNOWN
                
                LN_LOG_LEVEL_VERBOSE
                
                LN_LOG_LEVEL_DEBUG
                
                LN_LOG_LEVEL_INFO
                
                LN_LOG_LEVEL_WARNING
                
                LN_LOG_LEVEL_ERROR
                
                LN_LOG_LEVEL_FATAL
                
                LN_LOG_LEVEL_DISBLE
                
[in] tag   : 
[in] text  : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAssetModel_Create
init
%group
LNAssetModel
%prm
target, outAssetModel
[in]  target        : 
[out] outAssetModel(0) : 作成された AssetModel のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
AssetModel は Object のサブクラスです。AssetModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAssetModel_Target
target
%group
LNAssetModel
%prm
assetmodel, outReturn
[in]  assetmodel : AssetModel のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
AssetModel は Object のサブクラスです。AssetModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAssets_SaveAssetToLocalFile
Internal
%group
LNAssets
%prm
asset, filePath
[in] asset    : 
[in] filePath : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAssets_LoadAssetFromLocalFile
Internal
%group
LNAssets
%prm
filePath, outReturn
[in]  filePath  : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAssets_LoadAsset
指定したアセットファイルを読み込み、オブジェクト生成します。
%group
LNAssets
%prm
filePath, outReturn
[in]  filePath  : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst
ファイルの拡張子は .lnasset です。ただし、filePath に指定する値は拡張子を省略可能です。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAssets_ReloadAsset
指定したアセットファイルを読み込み、作成済みのオブジェクトへ適用します。
%group
LNAssets
%prm
filePath, obj
[in] filePath : 
[in] obj      : 

stat : 0=成功, 負値=失敗

%inst
このメソッドは Lumino の型システムを使用しないオブジェクトの読み込みに使用します。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAssets_ReadAllText
readAllText
%group
LNAssets
%prm
filePath, encoding, outReturn
[in]  filePath                           : 
[in]  encoding(LN_ENCODING_TYPE_UNKNOWN) : 
                                              LN_ENCODING_TYPE_UNKNOWN
                                              不明な文字エンコーディング (判別失敗。またはバイナリファイル)
                                              LN_ENCODING_TYPE_ASCII
                                              ASCII (拡張アスキーは含まない http://www.asciitable.com/)
                                              LN_ENCODING_TYPE_UTF8
                                              UTF-8
                                              LN_ENCODING_TYPE_UTF8N
                                              UTF-8 (BOM 無し)
                                              LN_ENCODING_TYPE_SJIS
                                              日本語 (シフト JIS) -- cp932(MS932) Windows-31J ※MS実装
[out] outReturn(0)                       : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_SetVolume
この Sound の音量を設定します。
%group
LNSound
%prm
sound, value
[in] sound : Sound のハンドル
[in] value : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sound は Object のサブクラスです。Sound ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_GetVolume
この Sound の音量を取得します。
%group
LNSound
%prm
sound, outReturn
[in]  sound     : Sound のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sound は Object のサブクラスです。Sound ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_SetPitch
この Sound のピッチ (音高) を設定します。
%group
LNSound
%prm
sound, value
[in] sound : Sound のハンドル
[in] value : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sound は Object のサブクラスです。Sound ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_GetPitch
この Sound のピッチ (音高) を取得します。
%group
LNSound
%prm
sound, outReturn
[in]  sound     : Sound のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sound は Object のサブクラスです。Sound ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_SetLoopEnabled
ループ再生の有無を設定します。
%group
LNSound
%prm
sound, enabled
[in] sound   : Sound のハンドル
[in] enabled : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sound は Object のサブクラスです。Sound ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_IsLoopEnabled
ループ再生が有効かを確認します。
%group
LNSound
%prm
sound, outReturn
[in]  sound     : Sound のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sound は Object のサブクラスです。Sound ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_SetLoopRange
ループ範囲を設定します。
%group
LNSound
%prm
sound, begin, length
[in] sound  : Sound のハンドル
[in] begin  : 
[in] length : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sound は Object のサブクラスです。Sound ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_Play
この Sound の再生を開始します。
%group
LNSound
%prm
sound
[in] sound : Sound のハンドル

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sound は Object のサブクラスです。Sound ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_Stop
この Sound の再生を停止します。
%group
LNSound
%prm
sound
[in] sound : Sound のハンドル

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sound は Object のサブクラスです。Sound ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_Pause
この Sound の再生を一時停止します。
%group
LNSound
%prm
sound
[in] sound : Sound のハンドル

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sound は Object のサブクラスです。Sound ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_Resume
一時停止中の再生を再開します。
%group
LNSound
%prm
sound
[in] sound : Sound のハンドル

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sound は Object のサブクラスです。Sound ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_FadeVolume
音量のフェードを開始します。
%group
LNSound
%prm
sound, targetVolume, time, behavior
[in] sound        : Sound のハンドル
[in] targetVolume : 
[in] time         : 
[in] behavior     : 
                       LN_SOUND_FADE_BEHAVIOR_CONTINUE
                       再生を継続する
                       LN_SOUND_FADE_BEHAVIOR_STOP
                       停止する
                       LN_SOUND_FADE_BEHAVIOR_STOP_RESET
                       停止して、次の再生に備えてサウンドの音量を元の値に戻す
                       LN_SOUND_FADE_BEHAVIOR_PAUSE
                       一時停止する
                       LN_SOUND_FADE_BEHAVIOR_PAUSE_RESET
                       一時停止して、次の再生に備えてサウンドの音量を元の値に戻す

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sound は Object のサブクラスです。Sound ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAudio_PlayBGM
BGM を演奏する
%group
LNAudio
%prm
filePath, volume, pitch, fadeTime
[in] filePath           : 
[in] volume(1.000000)   : 
[in] pitch(1.000000)    : 
[in] fadeTime(0.000000) : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAudio_StopBGM
BGM の演奏を停止する
%group
LNAudio
%prm
fadeTime
[in] fadeTime(0.000000) : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAudio_PlayBGS
BGS (環境音) を演奏する
%group
LNAudio
%prm
filePath, volume, pitch, fadeTime
[in] filePath           : 
[in] volume(1.000000)   : 
[in] pitch(1.000000)    : 
[in] fadeTime(0.000000) : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAudio_StopBGS
BGS の演奏を停止する
%group
LNAudio
%prm
fadeTime
[in] fadeTime(0.000000) : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAudio_PlayME
ME (効果音楽) を演奏する
%group
LNAudio
%prm
filePath, volume, pitch
[in] filePath         : 
[in] volume(1.000000) : 
[in] pitch(1.000000)  : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAudio_StopME
ME の演奏を停止する
%group
LNAudio
%prm


stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAudio_PlaySE
SE を演奏する
%group
LNAudio
%prm
filePath, volume, pitch
[in] filePath         : 
[in] volume(1.000000) : 
[in] pitch(1.000000)  : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAudio_PlaySE3D
SE を 3D 空間上で演奏する
%group
LNAudio
%prm
filePath, position, distance, volume, pitch
[in] filePath         : 
[in] position         : (Vector3 型の値) 
[in] distance         : 
[in] volume(1.000000) : 
[in] pitch(1.000000)  : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAudio_StopSE
全ての SE の演奏を停止する
%group
LNAudio
%prm


stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNTexture2DDelegate_Create

%group
LNTexture2DDelegate
%prm
callback, outTexture2DDelegate
[in]  callback             : 
[out] outTexture2DDelegate(0) : 作成された Texture2DDelegate のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Texture2DDelegate は Object のサブクラスです。Texture2DDelegate ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNTexture2DPromise_ThenWith

%group
LNTexture2DPromise
%prm
texture2dpromise, callback
[in] texture2dpromise : Texture2DPromise のハンドル
[in] callback         : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Texture2DPromise は Object のサブクラスです。Texture2DPromise ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNTexture2DPromise_CatchWith

%group
LNTexture2DPromise
%prm
texture2dpromise, callback
[in] texture2dpromise : Texture2DPromise のハンドル
[in] callback         : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Texture2DPromise は Object のサブクラスです。Texture2DPromise ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNGraphics_GetActiveGraphicsAPI
activeGraphicsAPI
%group
LNGraphics
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。
                     LN_GRAPHICS_API_DEFAULT
                     自動選択
                     LN_GRAPHICS_API_OPEN_GL
                     OpenGL
                     LN_GRAPHICS_API_VULKAN
                     Vulkan
                     LN_GRAPHICS_API_DIRECT_X12
                     Vulkan

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNTexture2D_Create
テクスチャを作成します。ピクセルフォーマットは RGBA8 です。
%group
LNTexture2D
%prm
width, height, outTexture2D
[in]  width        : 
[in]  height       : 
[out] outTexture2D(0) : 作成された Texture2D のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Texture2D は Texture のサブクラスです。Texture2D ハンドルは Texture ハンドルとして扱うことができ、 LNTexture_ から始まる命令等で使用できます。
Texture2D は AssetObject のサブクラスです。Texture2D ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
Texture2D は Object のサブクラスです。Texture2D ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNTexture2D_CreateWithFormat
テクスチャを作成します。
%group
LNTexture2D
%prm
width, height, format, outTexture2D
[in]  width        : 
[in]  height       : 
[in]  format       : 
                        LN_TEXTURE_FORMAT_UNKNOWN
                        Unknown
                        LN_TEXTURE_FORMAT_RGBA8
                        RGBA オーダーの各要素 8bit フォーマット
                        LN_TEXTURE_FORMAT_RGB8
                        RGB オーダーの各要素 8bit フォーマット
                        LN_TEXTURE_FORMAT_RGBA16F
                        RGBA オーダーの各要素 16bit 浮動小数点フォーマット
                        LN_TEXTURE_FORMAT_RGBA32F
                        RGBA オーダーの各要素 32bit 浮動小数点フォーマット
                        LN_TEXTURE_FORMAT_R16F
                        16bit 浮動小数点フォーマット
                        LN_TEXTURE_FORMAT_R32F
                        32bit 浮動小数点フォーマット
                        LN_TEXTURE_FORMAT_R32S
                        32bit の符号あり整数フォーマット
[out] outTexture2D(0) : 作成された Texture2D のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Texture2D は Texture のサブクラスです。Texture2D ハンドルは Texture ハンドルとして扱うことができ、 LNTexture_ から始まる命令等で使用できます。
Texture2D は AssetObject のサブクラスです。Texture2D ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
Texture2D は Object のサブクラスです。Texture2D ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNTexture2D_Load
アセットからテクスチャを読み込みます。
%group
LNTexture2D
%prm
filePath, outReturn
[in]  filePath  : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst
サポートしているフォーマットは次の通りです。PNG(.png), JPG(.jpg), TGA(.tga), BMP(.bmp), GIF(.gif)

備考
--------------------
Texture2D は Texture のサブクラスです。Texture2D ハンドルは Texture ハンドルとして扱うことができ、 LNTexture_ から始まる命令等で使用できます。
Texture2D は AssetObject のサブクラスです。Texture2D ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
Texture2D は Object のサブクラスです。Texture2D ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNTexture2D_LoadEmoji
loadEmoji
%group
LNTexture2D
%prm
code, outReturn
[in]  code      : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Texture2D は Texture のサブクラスです。Texture2D ハンドルは Texture ハンドルとして扱うことができ、 LNTexture_ から始まる命令等で使用できます。
Texture2D は AssetObject のサブクラスです。Texture2D ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
Texture2D は Object のサブクラスです。Texture2D ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNShader_Load
load
%group
LNShader
%prm
filePath, settings, outReturn
[in]  filePath                 : 
[in]  settings(LN_NULL_HANDLE) : 
[out] outReturn(0)             : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Shader は AssetObject のサブクラスです。Shader ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
Shader は Object のサブクラスです。Shader ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNShader_SetFloat
浮動小数点値を設定します。
%group
LNShader
%prm
shader, parameterName, value
[in] shader        : Shader のハンドル
[in] parameterName : 
[in] value         : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Shader は AssetObject のサブクラスです。Shader ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
Shader は Object のサブクラスです。Shader ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNShader_SetVector3
ベクトル値を設定します。
%group
LNShader
%prm
shader, parameterName, value
[in] shader        : Shader のハンドル
[in] parameterName : 
[in] value         : (Vector3 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Shader は AssetObject のサブクラスです。Shader ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
Shader は Object のサブクラスです。Shader ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNShader_SetVector4
ベクトル値を設定します。
%group
LNShader
%prm
shader, parameterName, value
[in] shader        : Shader のハンドル
[in] parameterName : 
[in] value         : (Vector4 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Shader は AssetObject のサブクラスです。Shader ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
Shader は Object のサブクラスです。Shader ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNShader_SetTexture
setTexture
%group
LNShader
%prm
shader, parameterName, value
[in] shader        : Shader のハンドル
[in] parameterName : 
[in] value         : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Shader は AssetObject のサブクラスです。Shader ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
Shader は Object のサブクラスです。Shader ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMaterial_Create
init
%group
LNMaterial
%prm
outMaterial
[out] outMaterial(0) : 作成された Material のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Material は Object のサブクラスです。Material ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMaterial_SetMainTexture
mainTexture
%group
LNMaterial
%prm
material, value
[in] material : Material のハンドル
[in] value    : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Material は Object のサブクラスです。Material ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMaterial_GetMainTexture
mainTexture
%group
LNMaterial
%prm
material, outReturn
[in]  material  : Material のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Material は Object のサブクラスです。Material ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMaterial_SetColor
setColor
%group
LNMaterial
%prm
material, value
[in] material : Material のハンドル
[in] value    : (Color 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Material は Object のサブクラスです。Material ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMaterial_SetRoughness
setRoughness
%group
LNMaterial
%prm
material, value
[in] material : Material のハンドル
[in] value    : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Material は Object のサブクラスです。Material ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMaterial_SetMetallic
setMetallic
%group
LNMaterial
%prm
material, value
[in] material : Material のハンドル
[in] value    : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Material は Object のサブクラスです。Material ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMaterial_SetEmissive
setEmissive
%group
LNMaterial
%prm
material, value
[in] material : Material のハンドル
[in] value    : (Color 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Material は Object のサブクラスです。Material ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMaterial_SetShadingModel
setShadingModel
%group
LNMaterial
%prm
material, value
[in] material : Material のハンドル
[in] value    : 
                   LN_SHADING_MODEL_DEFAULT
                   Default
                   LN_SHADING_MODEL_UNLIT
                   Unlit

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Material は Object のサブクラスです。Material ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMaterial_GetShadingModel
shadingModel
%group
LNMaterial
%prm
material, outReturn
[in]  material  : Material のハンドル
[out] outReturn(0) : 結果を格納する変数。
                     LN_SHADING_MODEL_DEFAULT
                     Default
                     LN_SHADING_MODEL_UNLIT
                     Unlit

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Material は Object のサブクラスです。Material ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMaterial_SetShader
shader
%group
LNMaterial
%prm
material, shader
[in] material : Material のハンドル
[in] shader   : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Material は Object のサブクラスです。Material ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMaterial_GetShader
shader
%group
LNMaterial
%prm
material, outReturn
[in]  material  : Material のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Material は Object のサブクラスです。Material ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshNode_SetVisible
可視状態を設定します。false の場合、このメッシュの描画は行われません。(default: true)
%group
LNMeshNode
%prm
meshnode, value
[in] meshnode : MeshNode のハンドル
[in] value    : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
MeshNode は Object のサブクラスです。MeshNode ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshNode_IsVisible
可視状態を取得します。
%group
LNMeshNode
%prm
meshnode, outReturn
[in]  meshnode  : MeshNode のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
MeshNode は Object のサブクラスです。MeshNode ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAnimationController_AddClip
アニメーションクリップを追加します。 (レイヤー0 へ追加されます)
%group
LNAnimationController
%prm
animationcontroller, animationClip, outReturn
[in]  animationcontroller : AnimationController のハンドル
[in]  animationClip       : 
[out] outReturn(0)        : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
AnimationController は Object のサブクラスです。AnimationController ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAnimationController_Play
play
%group
LNAnimationController
%prm
animationcontroller, state, duration
[in] animationcontroller : AnimationController のハンドル
[in] state               : 
[in] duration(0.300000)  : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
AnimationController は Object のサブクラスです。AnimationController ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshModel_Load
load
%group
LNMeshModel
%prm
filePath, settings, outReturn
[in]  filePath                 : 
[in]  settings(LN_NULL_HANDLE) : 
[out] outReturn(0)             : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
MeshModel は Object のサブクラスです。MeshModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshModel_FindNode
findNode
%group
LNMeshModel
%prm
meshmodel, name, outReturn
[in]  meshmodel : MeshModel のハンドル
[in]  name      : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
MeshModel は Object のサブクラスです。MeshModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshModel_FindMaterial
findMaterial
%group
LNMeshModel
%prm
meshmodel, name, outReturn
[in]  meshmodel : MeshModel のハンドル
[in]  name      : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
MeshModel は Object のサブクラスです。MeshModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshModel_MaterialCount
materialCount
%group
LNMeshModel
%prm
meshmodel, outReturn
[in]  meshmodel : MeshModel のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
MeshModel は Object のサブクラスです。MeshModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshModel_Material
load
%group
LNMeshModel
%prm
meshmodel, index, outReturn
[in]  meshmodel : MeshModel のハンドル
[in]  index     : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
MeshModel は Object のサブクラスです。MeshModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshModel_GetAnimationController
animationController
%group
LNMeshModel
%prm
meshmodel, outReturn
[in]  meshmodel : MeshModel のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
MeshModel は Object のサブクラスです。MeshModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshImportSettings_Create
init
%group
LNMeshImportSettings
%prm
outMeshImportSettings
[out] outMeshImportSettings(0) : 作成された MeshImportSettings のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
MeshImportSettings は Object のサブクラスです。MeshImportSettings ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNBoxCollisionShape_Create
init
%group
LNBoxCollisionShape
%prm
size, outBoxCollisionShape
[in]  size                 : (Vector3 型の値) 
[out] outBoxCollisionShape(0) : 作成された BoxCollisionShape のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
BoxCollisionShape は CollisionShape のサブクラスです。BoxCollisionShape ハンドルは CollisionShape ハンドルとして扱うことができ、 LNCollisionShape_ から始まる命令等で使用できます。
BoxCollisionShape は Object のサブクラスです。BoxCollisionShape ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNBoxCollisionShape_CreateWHD
init
%group
LNBoxCollisionShape
%prm
width, height, depth, outBoxCollisionShape
[in]  width                : 
[in]  height               : 
[in]  depth                : 
[out] outBoxCollisionShape(0) : 作成された BoxCollisionShape のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
BoxCollisionShape は CollisionShape のサブクラスです。BoxCollisionShape ハンドルは CollisionShape ハンドルとして扱うことができ、 LNCollisionShape_ から始まる命令等で使用できます。
BoxCollisionShape は Object のサブクラスです。BoxCollisionShape ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAnimationCurve_Evaluate
指定した時間における値を評価します。
%group
LNAnimationCurve
%prm
animationcurve, time, outReturn
[in]  animationcurve : AnimationCurve のハンドル
[in]  time           : 
[out] outReturn(0)   : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
AnimationCurve は Object のサブクラスです。AnimationCurve ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNKeyFrameAnimationCurve_Create
init
%group
LNKeyFrameAnimationCurve
%prm
outKeyFrameAnimationCurve
[out] outKeyFrameAnimationCurve(0) : 作成された KeyFrameAnimationCurve のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
KeyFrameAnimationCurve は AnimationCurve のサブクラスです。KeyFrameAnimationCurve ハンドルは AnimationCurve ハンドルとして扱うことができ、 LNAnimationCurve_ から始まる命令等で使用できます。
KeyFrameAnimationCurve は Object のサブクラスです。KeyFrameAnimationCurve ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNKeyFrameAnimationCurve_AddKeyFrame
キーフレームを追加します。
%group
LNKeyFrameAnimationCurve
%prm
keyframeanimationcurve, time, value, rightTangentMode, tangent
[in] keyframeanimationcurve                   : KeyFrameAnimationCurve のハンドル
[in] time                                     : 
[in] value                                    : 
[in] rightTangentMode(LN_TANGENT_MODE_LINEAR) : 
                                                   LN_TANGENT_MODE_LINEAR
                                                   線形補間
                                                   LN_TANGENT_MODE_TANGENT
                                                   接線 (速度) を使用した補間 (エルミートスプライン)
                                                   LN_TANGENT_MODE_AUTO
                                                   キーフレームの値を通過するなめらかな補間 (Catmull-Rom)
                                                   LN_TANGENT_MODE_CONSTANT
                                                   補間なし
[in] tangent(0.000000)                        : 

stat : 0=成功, 負値=失敗

%inst
rightTangentMode は、新しく追加するキーフレームの右側の補間方法です。新しく追加するキーフレームの左側の保管方法は、そのひとつ前のキーフレームの右側の保管方法が設定されます。

備考
--------------------
KeyFrameAnimationCurve は AnimationCurve のサブクラスです。KeyFrameAnimationCurve ハンドルは AnimationCurve ハンドルとして扱うことができ、 LNAnimationCurve_ から始まる命令等で使用できます。
KeyFrameAnimationCurve は Object のサブクラスです。KeyFrameAnimationCurve ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAnimationClip_Load
load
%group
LNAnimationClip
%prm
filePath, outReturn
[in]  filePath  : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
AnimationClip は AssetObject のサブクラスです。AnimationClip ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
AnimationClip は Object のサブクラスです。AnimationClip ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAnimationClip_SetWrapMode
アニメーションの繰り返しの動作を取得します。(default: Loop)
%group
LNAnimationClip
%prm
animationclip, value
[in] animationclip : AnimationClip のハンドル
[in] value         : 
                        LN_ANIMATION_WRAP_MODE_ONCE
                        繰り返しを行わず、1度だけ再生します。
                        LN_ANIMATION_WRAP_MODE_LOOP
                        最後まで再生された後、先頭に戻ってループします。
                        LN_ANIMATION_WRAP_MODE_ALTERNATE
                        最後まで再生された後、逆方向に戻ってループします。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
AnimationClip は AssetObject のサブクラスです。AnimationClip ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
AnimationClip は Object のサブクラスです。AnimationClip ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAnimationClip_GetWrapMode
アニメーションの繰り返しの動作を取得します。
%group
LNAnimationClip
%prm
animationclip, outReturn
[in]  animationclip : AnimationClip のハンドル
[out] outReturn(0)  : 結果を格納する変数。
                         LN_ANIMATION_WRAP_MODE_ONCE
                         繰り返しを行わず、1度だけ再生します。
                         LN_ANIMATION_WRAP_MODE_LOOP
                         最後まで再生された後、先頭に戻ってループします。
                         LN_ANIMATION_WRAP_MODE_ALTERNATE
                         最後まで再生された後、逆方向に戻ってループします。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
AnimationClip は AssetObject のサブクラスです。AnimationClip ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
AnimationClip は Object のサブクラスです。AnimationClip ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAnimationClip_SetHierarchicalAnimationMode
階層構造を持つアニメーションデータの動作モード。(default: AllowTranslationOnlyRoot)
%group
LNAnimationClip
%prm
animationclip, value
[in] animationclip : AnimationClip のハンドル
[in] value         : 
                        LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT
                        ルートノードのみ、平行移動を有効化します。
                        LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT_Y
                        AllowTranslationOnlyRootY
                        LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION
                        すべてのノードの平行移動を有効化します。
                        LN_HIERARCHICAL_ANIMATION_MODE_DISABLE_TRANSLATION
                        平行移動を無効化します。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
AnimationClip は AssetObject のサブクラスです。AnimationClip ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
AnimationClip は Object のサブクラスです。AnimationClip ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAnimationClip_GetHierarchicalAnimationMode
階層構造を持つアニメーションデータの動作モード。
%group
LNAnimationClip
%prm
animationclip, outReturn
[in]  animationclip : AnimationClip のハンドル
[out] outReturn(0)  : 結果を格納する変数。
                         LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT
                         ルートノードのみ、平行移動を有効化します。
                         LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT_Y
                         AllowTranslationOnlyRootY
                         LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION
                         すべてのノードの平行移動を有効化します。
                         LN_HIERARCHICAL_ANIMATION_MODE_DISABLE_TRANSLATION
                         平行移動を無効化します。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
AnimationClip は AssetObject のサブクラスです。AnimationClip ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
AnimationClip は Object のサブクラスです。AnimationClip ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterModel_Create
init
%group
LNParticleEmitterModel
%prm
outParticleEmitterModel
[out] outParticleEmitterModel(0) : 作成された ParticleEmitterModel のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ParticleEmitterModel は Object のサブクラスです。ParticleEmitterModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterModel_SetMaxParticles
同時に表示できるパーティクルの最大数を設定します。(default: 100)
%group
LNParticleEmitterModel
%prm
particleemittermodel, count
[in] particleemittermodel : ParticleEmitterModel のハンドル
[in] count                : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ParticleEmitterModel は Object のサブクラスです。ParticleEmitterModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterModel_SetSpawnRate
1秒間に放出するパーティクルの数を設定します。(default: 1)
%group
LNParticleEmitterModel
%prm
particleemittermodel, rate
[in] particleemittermodel : ParticleEmitterModel のハンドル
[in] rate                 : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ParticleEmitterModel は Object のサブクラスです。ParticleEmitterModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterModel_SetLifeTime
パーティクルの生存時間を設定します。(default: 5.0)
%group
LNParticleEmitterModel
%prm
particleemittermodel, time
[in] particleemittermodel : ParticleEmitterModel のハンドル
[in] time                 : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ParticleEmitterModel は Object のサブクラスです。ParticleEmitterModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterModel_SetupBoxShape
setupBoxShape
%group
LNParticleEmitterModel
%prm
particleemittermodel, size
[in] particleemittermodel : ParticleEmitterModel のハンドル
[in] size                 : (Vector3 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ParticleEmitterModel は Object のサブクラスです。ParticleEmitterModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterModel_SetSize
(default: 1.0)
%group
LNParticleEmitterModel
%prm
particleemittermodel, value
[in] particleemittermodel : ParticleEmitterModel のハンドル
[in] value                : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ParticleEmitterModel は Object のサブクラスです。ParticleEmitterModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterModel_SetSizeVelocity

%group
LNParticleEmitterModel
%prm
particleemittermodel, value
[in] particleemittermodel : ParticleEmitterModel のハンドル
[in] value                : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ParticleEmitterModel は Object のサブクラスです。ParticleEmitterModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterModel_SetSizeAcceleration

%group
LNParticleEmitterModel
%prm
particleemittermodel, value
[in] particleemittermodel : ParticleEmitterModel のハンドル
[in] value                : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ParticleEmitterModel は Object のサブクラスです。ParticleEmitterModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterModel_SetForwardVelocityMin
(default: 0)
%group
LNParticleEmitterModel
%prm
particleemittermodel, value
[in] particleemittermodel : ParticleEmitterModel のハンドル
[in] value                : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ParticleEmitterModel は Object のサブクラスです。ParticleEmitterModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterModel_SetForwardVelocityMax
(default: 0)
%group
LNParticleEmitterModel
%prm
particleemittermodel, value
[in] particleemittermodel : ParticleEmitterModel のハンドル
[in] value                : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ParticleEmitterModel は Object のサブクラスです。ParticleEmitterModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterModel_SetForwardScale
進行方向に対するスケール値。通常、Z軸。ParticleGeometryDirection::ToView では Y scale (default: 1.0)
%group
LNParticleEmitterModel
%prm
particleemittermodel, value
[in] particleemittermodel : ParticleEmitterModel のハンドル
[in] value                : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ParticleEmitterModel は Object のサブクラスです。ParticleEmitterModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterModel_SetGeometryDirection
(default: ToView)
%group
LNParticleEmitterModel
%prm
particleemittermodel, value
[in] particleemittermodel : ParticleEmitterModel のハンドル
[in] value                : 
                               LN_PARTICLE_GEOMETRY_DIRECTION_TO_VIEW
                               常に視点方向を向く
                               LN_PARTICLE_GEOMETRY_DIRECTION_TOP
                               進行方向を軸に、表面 (Y+) が視点から見えるようにする
                               LN_PARTICLE_GEOMETRY_DIRECTION_VERTICAL_BILLBOARD
                               Y 軸方向に直立し、カメラの方を向きます。
                               LN_PARTICLE_GEOMETRY_DIRECTION_HORIZONTAL_BILLBOARD
                               XZ 軸上の「床」平面と平行になります。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ParticleEmitterModel は Object のサブクラスです。ParticleEmitterModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterModel_SetupSpriteModule
setupSpriteModule
%group
LNParticleEmitterModel
%prm
particleemittermodel, material
[in] particleemittermodel : ParticleEmitterModel のハンドル
[in] material             : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ParticleEmitterModel は Object のサブクラスです。ParticleEmitterModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleModel_Create
init
%group
LNParticleModel
%prm
outParticleModel
[out] outParticleModel(0) : 作成された ParticleModel のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ParticleModel は EffectResource のサブクラスです。ParticleModel ハンドルは EffectResource ハンドルとして扱うことができ、 LNEffectResource_ から始まる命令等で使用できます。
ParticleModel は AssetObject のサブクラスです。ParticleModel ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
ParticleModel は Object のサブクラスです。ParticleModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleModel_SetLoop
setLoop
%group
LNParticleModel
%prm
particlemodel, value
[in] particlemodel : ParticleModel のハンドル
[in] value         : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ParticleModel は EffectResource のサブクラスです。ParticleModel ハンドルは EffectResource ハンドルとして扱うことができ、 LNEffectResource_ から始まる命令等で使用できます。
ParticleModel は AssetObject のサブクラスです。ParticleModel ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
ParticleModel は Object のサブクラスです。ParticleModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleModel_IsLoop
setLoop
%group
LNParticleModel
%prm
particlemodel, outReturn
[in]  particlemodel : ParticleModel のハンドル
[out] outReturn(0)  : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ParticleModel は EffectResource のサブクラスです。ParticleModel ハンドルは EffectResource ハンドルとして扱うことができ、 LNEffectResource_ から始まる命令等で使用できます。
ParticleModel は AssetObject のサブクラスです。ParticleModel ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
ParticleModel は Object のサブクラスです。ParticleModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleModel_AddEmitter
setLoop
%group
LNParticleModel
%prm
particlemodel, emitter
[in] particlemodel : ParticleModel のハンドル
[in] emitter       : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ParticleModel は EffectResource のサブクラスです。ParticleModel ハンドルは EffectResource ハンドルとして扱うことができ、 LNEffectResource_ から始まる命令等で使用できます。
ParticleModel は AssetObject のサブクラスです。ParticleModel ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
ParticleModel は Object のサブクラスです。ParticleModel ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVisualComponent_SetVisible
可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
%group
LNVisualComponent
%prm
visualcomponent, value
[in] visualcomponent : VisualComponent のハンドル
[in] value           : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
VisualComponent は Component のサブクラスです。VisualComponent ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
VisualComponent は Object のサブクラスです。VisualComponent ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVisualComponent_IsVisible
可視状態を取得します。
%group
LNVisualComponent
%prm
visualcomponent, outReturn
[in]  visualcomponent : VisualComponent のハンドル
[out] outReturn(0)    : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
VisualComponent は Component のサブクラスです。VisualComponent ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
VisualComponent は Object のサブクラスです。VisualComponent ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpriteComponent_SetTexture
スプライトが表示するテクスチャを設定します。
%group
LNSpriteComponent
%prm
spritecomponent, texture
[in] spritecomponent : SpriteComponent のハンドル
[in] texture         : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
SpriteComponent は VisualComponent のサブクラスです。SpriteComponent ハンドルは VisualComponent ハンドルとして扱うことができ、 LNVisualComponent_ から始まる命令等で使用できます。
SpriteComponent は Component のサブクラスです。SpriteComponent ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
SpriteComponent は Object のサブクラスです。SpriteComponent ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCollisionEventHandler_Create

%group
LNCollisionEventHandler
%prm
callback, outCollisionEventHandler
[in]  callback                 : 
[out] outCollisionEventHandler(0) : 作成された CollisionEventHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
CollisionEventHandler は Object のサブクラスです。CollisionEventHandler ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_Create
CharacterController を作成します。
%group
LNCharacterController
%prm
outCharacterController
[out] outCharacterController(0) : 作成された CharacterController のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
CharacterController は Component のサブクラスです。CharacterController ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
CharacterController は Object のサブクラスです。CharacterController ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_SetWalkVelocity
walkVelocity
%group
LNCharacterController
%prm
charactercontroller, value
[in] charactercontroller : CharacterController のハンドル
[in] value               : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
CharacterController は Component のサブクラスです。CharacterController ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
CharacterController は Object のサブクラスです。CharacterController ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_GetWalkVelocity
walkVelocity
%group
LNCharacterController
%prm
charactercontroller, outReturn
[in]  charactercontroller : CharacterController のハンドル
[out] outReturn(0)        : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
CharacterController は Component のサブクラスです。CharacterController ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
CharacterController は Object のサブクラスです。CharacterController ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_SetVelocity
setVelocity
%group
LNCharacterController
%prm
charactercontroller, value
[in] charactercontroller : CharacterController のハンドル
[in] value               : (Vector3 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
CharacterController は Component のサブクラスです。CharacterController ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
CharacterController は Object のサブクラスです。CharacterController ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_GetVelocity
velocity
%group
LNCharacterController
%prm
charactercontroller, outReturn
[in]  charactercontroller : CharacterController のハンドル
[out] outReturn(0)        : (Vector3 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
CharacterController は Component のサブクラスです。CharacterController ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
CharacterController は Object のサブクラスです。CharacterController ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_SetInputControlEnabled
キーボードやゲームパッドによる操作の有効状態を設定します。 (default: true)
%group
LNCharacterController
%prm
charactercontroller, value
[in] charactercontroller : CharacterController のハンドル
[in] value               : 

stat : 0=成功, 負値=失敗

%inst
false を指定した場合、キャラクターの想定外の自走を防止するため、速度も 0 にリセットされます。

備考
--------------------
CharacterController は Component のサブクラスです。CharacterController ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
CharacterController は Object のサブクラスです。CharacterController ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_SetCameraControlEnabled
マウスによるカメラ操作の有効状態を設定します。 (default: true)
%group
LNCharacterController
%prm
charactercontroller, value
[in] charactercontroller : CharacterController のハンドル
[in] value               : 

stat : 0=成功, 負値=失敗

%inst
有効である場合、関連付けられているカメラを通じて、描画先となるビューの MouseGrab を取得します。つまり、マウスカーソルは非表示となり UI をポイントしてクリックする等の操作はできなくなります。

備考
--------------------
CharacterController は Component のサブクラスです。CharacterController ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
CharacterController は Object のサブクラスです。CharacterController ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_SetHeight
キャラクターの高さを設定します。この値はカプセルコライダーの高さや、カメラの注視点として使用されます。 (default: 2.0)
%group
LNCharacterController
%prm
charactercontroller, value
[in] charactercontroller : CharacterController のハンドル
[in] value               : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
CharacterController は Component のサブクラスです。CharacterController ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
CharacterController は Object のサブクラスです。CharacterController ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_GetHeight
キャラクターの高さを取得します。
%group
LNCharacterController
%prm
charactercontroller, outReturn
[in]  charactercontroller : CharacterController のハンドル
[out] outReturn(0)        : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
CharacterController は Component のサブクラスです。CharacterController ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
CharacterController は Object のサブクラスです。CharacterController ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_SetCameraRadius
キャラクターとカメラの距離を設定します。(default: 5.0)
%group
LNCharacterController
%prm
charactercontroller, value
[in] charactercontroller : CharacterController のハンドル
[in] value               : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
CharacterController は Component のサブクラスです。CharacterController ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
CharacterController は Object のサブクラスです。CharacterController ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_GetCameraRadius
キャラクターとカメラの距離を取得します。
%group
LNCharacterController
%prm
charactercontroller, outReturn
[in]  charactercontroller : CharacterController のハンドル
[out] outReturn(0)        : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
CharacterController は Component のサブクラスです。CharacterController ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
CharacterController は Object のサブクラスです。CharacterController ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_SetCollisionEnter
CollisionEnter イベントの通知を受け取るコールバックを登録します。
%group
LNCharacterController
%prm
charactercontroller, handler
[in] charactercontroller : CharacterController のハンドル
[in] handler             : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
CharacterController は Component のサブクラスです。CharacterController ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
CharacterController は Object のサブクラスです。CharacterController ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_SetCollisionLeave
CollisionLeave イベントの通知を受け取るコールバックを登録します。
%group
LNCharacterController
%prm
charactercontroller, handler
[in] charactercontroller : CharacterController のハンドル
[in] handler             : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
CharacterController は Component のサブクラスです。CharacterController ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
CharacterController は Object のサブクラスです。CharacterController ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_SetCollisionStay
CollisionStay イベントの通知を受け取るコールバックを登録します。
%group
LNCharacterController
%prm
charactercontroller, handler
[in] charactercontroller : CharacterController のハンドル
[in] handler             : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
CharacterController は Component のサブクラスです。CharacterController ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
CharacterController は Object のサブクラスです。CharacterController ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorld_Add
オブジェクトを World に追加します。
%group
LNWorld
%prm
world, obj
[in] world : World のハンドル
[in] obj   : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
World は Object のサブクラスです。World ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNComponentList_GetLength

%group
LNComponentList
%prm
componentlist, outReturn
[in]  componentlist : ComponentList のハンドル
[out] outReturn(0)  : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ComponentList は Object のサブクラスです。ComponentList ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNComponentList_GetItem

%group
LNComponentList
%prm
componentlist, index, outReturn
[in]  componentlist : ComponentList のハンドル
[in]  index         : 
[out] outReturn(0)  : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ComponentList は Object のサブクラスです。ComponentList ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_Create
WorldObject を作成します。
%group
LNWorldObject
%prm
outWorldObject
[out] outWorldObject(0) : 作成された WorldObject のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetPosition
このオブジェクトの位置を設定します。
%group
LNWorldObject
%prm
worldobject, pos
[in] worldobject : WorldObject のハンドル
[in] pos         : (Vector3 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetPositionXYZ
このオブジェクトの位置を設定します。
%group
LNWorldObject
%prm
worldobject, x, y, z
[in] worldobject : WorldObject のハンドル
[in] x           : 
[in] y           : 
[in] z(0.000000) : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_GetPosition
このオブジェクトの位置を位置を取得します。
%group
LNWorldObject
%prm
worldobject, outReturn
[in]  worldobject : WorldObject のハンドル
[out] outReturn(0) : (Vector3 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetRotation
このオブジェクトの回転を設定します。
%group
LNWorldObject
%prm
worldobject, rot
[in] worldobject : WorldObject のハンドル
[in] rot         : (Quaternion 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetRotationXYZ
このオブジェクトの回転をオイラー角から設定します(radian単位) 。回転順序は Z(Roll) > X(Pich) > Y(Yaw) です。
%group
LNWorldObject
%prm
worldobject, x, y, z
[in] worldobject : WorldObject のハンドル
[in] x           : 
[in] y           : 
[in] z           : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_GetRotation
このオブジェクトの回転を取得します。
%group
LNWorldObject
%prm
worldobject, outReturn
[in]  worldobject : WorldObject のハンドル
[out] outReturn(0) : (Quaternion 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetScale
このオブジェクトの拡大率を設定します。
%group
LNWorldObject
%prm
worldobject, scale
[in] worldobject : WorldObject のハンドル
[in] scale       : (Vector3 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetScaleS
このオブジェクトの拡大率を設定します。
%group
LNWorldObject
%prm
worldobject, xyz
[in] worldobject : WorldObject のハンドル
[in] xyz         : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetScaleXYZ
このオブジェクトの拡大率を設定します。
%group
LNWorldObject
%prm
worldobject, x, y, z
[in] worldobject : WorldObject のハンドル
[in] x           : 
[in] y           : 
[in] z(1.000000) : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_GetScale
このオブジェクトの拡大率を取得します。
%group
LNWorldObject
%prm
worldobject, outReturn
[in]  worldobject : WorldObject のハンドル
[out] outReturn(0) : (Vector3 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetCenterPoint
このオブジェクトのローカルの中心位置を設定します。
%group
LNWorldObject
%prm
worldobject, value
[in] worldobject : WorldObject のハンドル
[in] value       : (Vector3 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetCenterPointXYZ
このオブジェクトのローカルの中心位置を設定します。
%group
LNWorldObject
%prm
worldobject, x, y, z
[in] worldobject : WorldObject のハンドル
[in] x           : 
[in] y           : 
[in] z(0.000000) : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_GetCenterPoint
このオブジェクトのローカルの中心位置を取得します。
%group
LNWorldObject
%prm
worldobject, outReturn
[in]  worldobject : WorldObject のハンドル
[out] outReturn(0) : (Vector3 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_LookAt
指定した座標を向くように、オブジェクトを回転させます。
%group
LNWorldObject
%prm
worldobject, target
[in] worldobject : WorldObject のハンドル
[in] target      : (Vector3 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_LookAtXYZ
指定した座標を向くように、オブジェクトを回転させます。
%group
LNWorldObject
%prm
worldobject, x, y, z
[in] worldobject : WorldObject のハンドル
[in] x           : 
[in] y           : 
[in] z           : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_AddComponent
Component を追加します。
%group
LNWorldObject
%prm
worldobject, component
[in] worldobject : WorldObject のハンドル
[in] component   : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_RemoveComponent
Component を除外します。
%group
LNWorldObject
%prm
worldobject, component
[in] worldobject : WorldObject のハンドル
[in] component   : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_AddTag
タグを追加します。
%group
LNWorldObject
%prm
worldobject, tag
[in] worldobject : WorldObject のハンドル
[in] tag         : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_RemoveTag
タグを除外します。
%group
LNWorldObject
%prm
worldobject, tag
[in] worldobject : WorldObject のハンドル
[in] tag         : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_HasTag
指定したタグを持っているかを確認します。
%group
LNWorldObject
%prm
worldobject, tag, outReturn
[in]  worldobject : WorldObject のハンドル
[in]  tag         : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_Destroy
WorldObject を破棄します。
%group
LNWorldObject
%prm
worldobject
[in] worldobject : WorldObject のハンドル

stat : 0=成功, 負値=失敗

%inst
実際の削除は、現在のフレームのアップデート処理後に行われます。削除された WorldObject は、親の World, Level, WorldObject からも除外されます。

備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_GetComponents

%group
LNWorldObject
%prm
worldobject, outReturn
[in]  worldobject : WorldObject のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_AddInto
この WorldObject を指定した World へ追加します。省略した場合はデフォルトの World へ追加します。
%group
LNWorldObject
%prm
worldobject, world
[in] worldobject           : WorldObject のハンドル
[in] world(LN_NULL_HANDLE) : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldObject は Object のサブクラスです。WorldObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVisualObject_SetVisible
可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true)
%group
LNVisualObject
%prm
visualobject, value
[in] visualobject : VisualObject のハンドル
[in] value        : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
VisualObject は WorldObject のサブクラスです。VisualObject ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
VisualObject は Object のサブクラスです。VisualObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVisualObject_IsVisible
可視状態を取得します。
%group
LNVisualObject
%prm
visualobject, outReturn
[in]  visualobject : VisualObject のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
VisualObject は WorldObject のサブクラスです。VisualObject ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
VisualObject は Object のサブクラスです。VisualObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVisualObject_SetBlendMode2
合成方法を設定します。(default: BlendMode::Normal)
%group
LNVisualObject
%prm
visualobject, value
[in] visualobject : VisualObject のハンドル
[in] value        : 
                       LN_BLEND_MODE_NORMAL
                       通常
                       LN_BLEND_MODE_ALPHA
                       アルファブレンド (RGB をアルファブレンドし、A を加算合成)
                       LN_BLEND_MODE_ADD
                       加算合成
                       LN_BLEND_MODE_SUBTRACT
                       減算合成
                       LN_BLEND_MODE_MULTIPLY
                       乗算合成

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
VisualObject は WorldObject のサブクラスです。VisualObject ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
VisualObject は Object のサブクラスです。VisualObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVisualObject_SetOpacity
不透明度を設定します。(default: 1.0)
%group
LNVisualObject
%prm
visualobject, value
[in] visualobject : VisualObject のハンドル
[in] value        : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
VisualObject は WorldObject のサブクラスです。VisualObject ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
VisualObject は Object のサブクラスです。VisualObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVisualObject_GetOpacity
不透明度を取得します。
%group
LNVisualObject
%prm
visualobject, outReturn
[in]  visualobject : VisualObject のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
VisualObject は WorldObject のサブクラスです。VisualObject ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
VisualObject は Object のサブクラスです。VisualObject ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_SetEnabled
ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
%group
LNEnvironmentLight
%prm
environmentlight, enabled
[in] environmentlight : EnvironmentLight のハンドル
[in] enabled          : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
EnvironmentLight は WorldObject のサブクラスです。EnvironmentLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
EnvironmentLight は Object のサブクラスです。EnvironmentLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_IsEnabled
ライトの有効状態を取得します。
%group
LNEnvironmentLight
%prm
environmentlight, outReturn
[in]  environmentlight : EnvironmentLight のハンドル
[out] outReturn(0)     : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
EnvironmentLight は WorldObject のサブクラスです。EnvironmentLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
EnvironmentLight は Object のサブクラスです。EnvironmentLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_SetColor
ディレクショナルライトの光源色を設定します。(default: White)
%group
LNEnvironmentLight
%prm
environmentlight, value
[in] environmentlight : EnvironmentLight のハンドル
[in] value            : (Color 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
EnvironmentLight は WorldObject のサブクラスです。EnvironmentLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
EnvironmentLight は Object のサブクラスです。EnvironmentLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_GetColor
ディレクショナルライトの光源色を取得します。
%group
LNEnvironmentLight
%prm
environmentlight, outReturn
[in]  environmentlight : EnvironmentLight のハンドル
[out] outReturn(0)     : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
EnvironmentLight は WorldObject のサブクラスです。EnvironmentLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
EnvironmentLight は Object のサブクラスです。EnvironmentLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_SetAmbientColor
シーン全体の環境光の色を設定します。(default: 0.5, 0.5, 0.5)
%group
LNEnvironmentLight
%prm
environmentlight, value
[in] environmentlight : EnvironmentLight のハンドル
[in] value            : (Color 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
EnvironmentLight は WorldObject のサブクラスです。EnvironmentLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
EnvironmentLight は Object のサブクラスです。EnvironmentLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_GetAmbientColor
シーン全体の環境光の色を取得します。
%group
LNEnvironmentLight
%prm
environmentlight, outReturn
[in]  environmentlight : EnvironmentLight のハンドル
[out] outReturn(0)     : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
EnvironmentLight は WorldObject のサブクラスです。EnvironmentLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
EnvironmentLight は Object のサブクラスです。EnvironmentLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_GetSkyColor
空の環境光の色を取得します。
%group
LNEnvironmentLight
%prm
environmentlight, outReturn
[in]  environmentlight : EnvironmentLight のハンドル
[out] outReturn(0)     : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
EnvironmentLight は WorldObject のサブクラスです。EnvironmentLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
EnvironmentLight は Object のサブクラスです。EnvironmentLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_SetSkyColor
空の環境光の色を設定します。(default: Black)
%group
LNEnvironmentLight
%prm
environmentlight, value
[in] environmentlight : EnvironmentLight のハンドル
[in] value            : (Color 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
EnvironmentLight は WorldObject のサブクラスです。EnvironmentLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
EnvironmentLight は Object のサブクラスです。EnvironmentLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_GetGroundColor
地面の環境光の色を取得します。
%group
LNEnvironmentLight
%prm
environmentlight, outReturn
[in]  environmentlight : EnvironmentLight のハンドル
[out] outReturn(0)     : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
EnvironmentLight は WorldObject のサブクラスです。EnvironmentLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
EnvironmentLight は Object のサブクラスです。EnvironmentLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_SetGroundColor
地面の環境光の色を設定します。(default: Black)
%group
LNEnvironmentLight
%prm
environmentlight, value
[in] environmentlight : EnvironmentLight のハンドル
[in] value            : (Color 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
EnvironmentLight は WorldObject のサブクラスです。EnvironmentLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
EnvironmentLight は Object のサブクラスです。EnvironmentLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_SetIntensity
ライトの明るさを設定します。(default: 0.5)
%group
LNEnvironmentLight
%prm
environmentlight, intensity
[in] environmentlight : EnvironmentLight のハンドル
[in] intensity        : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
EnvironmentLight は WorldObject のサブクラスです。EnvironmentLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
EnvironmentLight は Object のサブクラスです。EnvironmentLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_GetIntensity
ライトの明るさを取得します。
%group
LNEnvironmentLight
%prm
environmentlight, outReturn
[in]  environmentlight : EnvironmentLight のハンドル
[out] outReturn(0)     : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
EnvironmentLight は WorldObject のサブクラスです。EnvironmentLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
EnvironmentLight は Object のサブクラスです。EnvironmentLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_SetShadowEffectiveDistance
視点からの、影を生成できる距離を指定します。 (default: 0.0f)
%group
LNEnvironmentLight
%prm
environmentlight, value
[in] environmentlight : EnvironmentLight のハンドル
[in] value            : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
EnvironmentLight は WorldObject のサブクラスです。EnvironmentLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
EnvironmentLight は Object のサブクラスです。EnvironmentLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_GetShadowEffectiveDistance
視点からの、影を生成できる距離を取得します。
%group
LNEnvironmentLight
%prm
environmentlight, outReturn
[in]  environmentlight : EnvironmentLight のハンドル
[out] outReturn(0)     : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
EnvironmentLight は WorldObject のサブクラスです。EnvironmentLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
EnvironmentLight は Object のサブクラスです。EnvironmentLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_SetShadowEffectiveDepth
光源方向からの、影を生成できる距離を指定します。 (default: 0.0f) ※これはシャドウマップの深度値の範囲となります。
%group
LNEnvironmentLight
%prm
environmentlight, value
[in] environmentlight : EnvironmentLight のハンドル
[in] value            : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
EnvironmentLight は WorldObject のサブクラスです。EnvironmentLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
EnvironmentLight は Object のサブクラスです。EnvironmentLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_GetShadowEffectiveDepth
光源方向からの、影を生成できる距離を指定します。
%group
LNEnvironmentLight
%prm
environmentlight, outReturn
[in]  environmentlight : EnvironmentLight のハンドル
[out] outReturn(0)     : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
EnvironmentLight は WorldObject のサブクラスです。EnvironmentLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
EnvironmentLight は Object のサブクラスです。EnvironmentLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_Create
既定の設定でディレクショナルライトを作成します。
%group
LNDirectionalLight
%prm
outDirectionalLight
[out] outDirectionalLight(0) : 作成された DirectionalLight のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
DirectionalLight は WorldObject のサブクラスです。DirectionalLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
DirectionalLight は Object のサブクラスです。DirectionalLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_CreateWithColor
色を指定してディレクショナルライトを作成します。
%group
LNDirectionalLight
%prm
color, outDirectionalLight
[in]  color               : (Color 型の値) 
[out] outDirectionalLight(0) : 作成された DirectionalLight のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
DirectionalLight は WorldObject のサブクラスです。DirectionalLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
DirectionalLight は Object のサブクラスです。DirectionalLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_SetEnabled
ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
%group
LNDirectionalLight
%prm
directionallight, enabled
[in] directionallight : DirectionalLight のハンドル
[in] enabled          : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
DirectionalLight は WorldObject のサブクラスです。DirectionalLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
DirectionalLight は Object のサブクラスです。DirectionalLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_IsEnabled
ライトの有効状態を取得します。
%group
LNDirectionalLight
%prm
directionallight, outReturn
[in]  directionallight : DirectionalLight のハンドル
[out] outReturn(0)     : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
DirectionalLight は WorldObject のサブクラスです。DirectionalLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
DirectionalLight は Object のサブクラスです。DirectionalLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_SetColor
ライトカラーを設定します。(default: White)
%group
LNDirectionalLight
%prm
directionallight, color
[in] directionallight : DirectionalLight のハンドル
[in] color            : (Color 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
DirectionalLight は WorldObject のサブクラスです。DirectionalLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
DirectionalLight は Object のサブクラスです。DirectionalLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_GetColor
ライトカラーを取得します。
%group
LNDirectionalLight
%prm
directionallight, outReturn
[in]  directionallight : DirectionalLight のハンドル
[out] outReturn(0)     : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
DirectionalLight は WorldObject のサブクラスです。DirectionalLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
DirectionalLight は Object のサブクラスです。DirectionalLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_SetIntensity
ライトの明るさを設定します。(default: 0.5)
%group
LNDirectionalLight
%prm
directionallight, intensity
[in] directionallight : DirectionalLight のハンドル
[in] intensity        : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
DirectionalLight は WorldObject のサブクラスです。DirectionalLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
DirectionalLight は Object のサブクラスです。DirectionalLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_GetIntensity
ライトの明るさを取得します。
%group
LNDirectionalLight
%prm
directionallight, outReturn
[in]  directionallight : DirectionalLight のハンドル
[out] outReturn(0)     : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
DirectionalLight は WorldObject のサブクラスです。DirectionalLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
DirectionalLight は Object のサブクラスです。DirectionalLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_SetShadowEffectiveDistance
視点からの、影を生成できる距離を指定します。 (default: 0.0f)
%group
LNDirectionalLight
%prm
directionallight, value
[in] directionallight : DirectionalLight のハンドル
[in] value            : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
DirectionalLight は WorldObject のサブクラスです。DirectionalLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
DirectionalLight は Object のサブクラスです。DirectionalLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_GetShadowEffectiveDistance
視点からの、影を生成できる距離を取得します。
%group
LNDirectionalLight
%prm
directionallight, outReturn
[in]  directionallight : DirectionalLight のハンドル
[out] outReturn(0)     : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
DirectionalLight は WorldObject のサブクラスです。DirectionalLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
DirectionalLight は Object のサブクラスです。DirectionalLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_SetShadowEffectiveDepth
光源方向からの、影を生成できる距離を指定します。 (default: 0.0f) ※これはシャドウマップの深度値の範囲となります。
%group
LNDirectionalLight
%prm
directionallight, value
[in] directionallight : DirectionalLight のハンドル
[in] value            : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
DirectionalLight は WorldObject のサブクラスです。DirectionalLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
DirectionalLight は Object のサブクラスです。DirectionalLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_GetShadowEffectiveDepth
光源方向からの、影を生成できる距離を指定します。
%group
LNDirectionalLight
%prm
directionallight, outReturn
[in]  directionallight : DirectionalLight のハンドル
[out] outReturn(0)     : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
DirectionalLight は WorldObject のサブクラスです。DirectionalLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
DirectionalLight は Object のサブクラスです。DirectionalLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_Create
既定の設定でポイントライトを作成します。
%group
LNPointLight
%prm
outPointLight
[out] outPointLight(0) : 作成された PointLight のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
PointLight は WorldObject のサブクラスです。PointLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
PointLight は Object のサブクラスです。PointLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_CreateWithColorAndRange
色と範囲を指定してポイントライトを作成します。
%group
LNPointLight
%prm
color, range, outPointLight
[in]  color         : (Color 型の値) 
[in]  range         : 
[out] outPointLight(0) : 作成された PointLight のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
PointLight は WorldObject のサブクラスです。PointLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
PointLight は Object のサブクラスです。PointLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_SetEnabled
ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
%group
LNPointLight
%prm
pointlight, enabled
[in] pointlight : PointLight のハンドル
[in] enabled    : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
PointLight は WorldObject のサブクラスです。PointLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
PointLight は Object のサブクラスです。PointLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_IsEnabled
ライトの有効状態を取得します。
%group
LNPointLight
%prm
pointlight, outReturn
[in]  pointlight : PointLight のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
PointLight は WorldObject のサブクラスです。PointLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
PointLight は Object のサブクラスです。PointLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_SetColor
ライトカラーを設定します。(default: White)
%group
LNPointLight
%prm
pointlight, color
[in] pointlight : PointLight のハンドル
[in] color      : (Color 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
PointLight は WorldObject のサブクラスです。PointLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
PointLight は Object のサブクラスです。PointLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_GetColor
ライトカラーを取得します。
%group
LNPointLight
%prm
pointlight, outReturn
[in]  pointlight : PointLight のハンドル
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
PointLight は WorldObject のサブクラスです。PointLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
PointLight は Object のサブクラスです。PointLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_SetIntensity
ライトの明るさを設定します。(default: 1.0)
%group
LNPointLight
%prm
pointlight, intensity
[in] pointlight : PointLight のハンドル
[in] intensity  : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
PointLight は WorldObject のサブクラスです。PointLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
PointLight は Object のサブクラスです。PointLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_GetIntensity
ライトの明るさを取得します。
%group
LNPointLight
%prm
pointlight, outReturn
[in]  pointlight : PointLight のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
PointLight は WorldObject のサブクラスです。PointLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
PointLight は Object のサブクラスです。PointLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_SetRange
ライトの影響範囲を設定します。(default: 10.0)
%group
LNPointLight
%prm
pointlight, range
[in] pointlight : PointLight のハンドル
[in] range      : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
PointLight は WorldObject のサブクラスです。PointLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
PointLight は Object のサブクラスです。PointLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_GetRange
ライトの影響範囲を取得します。
%group
LNPointLight
%prm
pointlight, outReturn
[in]  pointlight : PointLight のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
PointLight は WorldObject のサブクラスです。PointLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
PointLight は Object のサブクラスです。PointLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_SetAttenuation
ライトの減衰を設定します。(default: 1.0)
%group
LNPointLight
%prm
pointlight, attenuation
[in] pointlight  : PointLight のハンドル
[in] attenuation : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
PointLight は WorldObject のサブクラスです。PointLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
PointLight は Object のサブクラスです。PointLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_GetAttenuation
ライトの減衰を取得します。
%group
LNPointLight
%prm
pointlight, outReturn
[in]  pointlight : PointLight のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
PointLight は WorldObject のサブクラスです。PointLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
PointLight は Object のサブクラスです。PointLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_Create
既定の設定でスポットライトを作成します。
%group
LNSpotLight
%prm
outSpotLight
[out] outSpotLight(0) : 作成された SpotLight のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
SpotLight は WorldObject のサブクラスです。SpotLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
SpotLight は Object のサブクラスです。SpotLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_CreateWithColorAndRange
色と範囲を指定してスポットライトを作成します。
%group
LNSpotLight
%prm
color, range, angle, outSpotLight
[in]  color        : (Color 型の値) 
[in]  range        : 
[in]  angle        : 
[out] outSpotLight(0) : 作成された SpotLight のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
SpotLight は WorldObject のサブクラスです。SpotLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
SpotLight は Object のサブクラスです。SpotLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_SetEnabled
ライトの有効状態を設定します。false の場合、ライトはシーンに影響しません。(default: true)
%group
LNSpotLight
%prm
spotlight, enabled
[in] spotlight : SpotLight のハンドル
[in] enabled   : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
SpotLight は WorldObject のサブクラスです。SpotLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
SpotLight は Object のサブクラスです。SpotLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_IsEnabled
ライトの有効状態を取得します。
%group
LNSpotLight
%prm
spotlight, outReturn
[in]  spotlight : SpotLight のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
SpotLight は WorldObject のサブクラスです。SpotLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
SpotLight は Object のサブクラスです。SpotLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_SetColor
ライトカラーを設定します。(default: White)
%group
LNSpotLight
%prm
spotlight, color
[in] spotlight : SpotLight のハンドル
[in] color     : (Color 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
SpotLight は WorldObject のサブクラスです。SpotLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
SpotLight は Object のサブクラスです。SpotLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_GetColor
ライトカラーを取得します。
%group
LNSpotLight
%prm
spotlight, outReturn
[in]  spotlight : SpotLight のハンドル
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
SpotLight は WorldObject のサブクラスです。SpotLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
SpotLight は Object のサブクラスです。SpotLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_SetIntensity
ライトの明るさを設定します。(default: 1.0)
%group
LNSpotLight
%prm
spotlight, intensity
[in] spotlight : SpotLight のハンドル
[in] intensity : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
SpotLight は WorldObject のサブクラスです。SpotLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
SpotLight は Object のサブクラスです。SpotLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_GetIntensity
ライトの明るさを取得します。
%group
LNSpotLight
%prm
spotlight, outReturn
[in]  spotlight : SpotLight のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
SpotLight は WorldObject のサブクラスです。SpotLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
SpotLight は Object のサブクラスです。SpotLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_SetRange
ライトの影響範囲を設定します。(default: 10.0)
%group
LNSpotLight
%prm
spotlight, range
[in] spotlight : SpotLight のハンドル
[in] range     : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
SpotLight は WorldObject のサブクラスです。SpotLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
SpotLight は Object のサブクラスです。SpotLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_GetRange
ライトの影響範囲を取得します。
%group
LNSpotLight
%prm
spotlight, outReturn
[in]  spotlight : SpotLight のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
SpotLight は WorldObject のサブクラスです。SpotLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
SpotLight は Object のサブクラスです。SpotLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_SetAttenuation
ライトの減衰を設定します。(default: 1.0)
%group
LNSpotLight
%prm
spotlight, attenuation
[in] spotlight   : SpotLight のハンドル
[in] attenuation : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
SpotLight は WorldObject のサブクラスです。SpotLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
SpotLight は Object のサブクラスです。SpotLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_GetAttenuation
ライトの減衰を取得します。
%group
LNSpotLight
%prm
spotlight, outReturn
[in]  spotlight : SpotLight のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
SpotLight は WorldObject のサブクラスです。SpotLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
SpotLight は Object のサブクラスです。SpotLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_SetAngle
スポットライトのコーン角度を設定します。(ラジアン単位、default: PI / 3)
%group
LNSpotLight
%prm
spotlight, angle
[in] spotlight : SpotLight のハンドル
[in] angle     : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
SpotLight は WorldObject のサブクラスです。SpotLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
SpotLight は Object のサブクラスです。SpotLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_GetAngle
スポットライトのコーン角度を取得します。(ラジアン単位)
%group
LNSpotLight
%prm
spotlight, outReturn
[in]  spotlight : SpotLight のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
SpotLight は WorldObject のサブクラスです。SpotLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
SpotLight は Object のサブクラスです。SpotLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_SetPenumbra
スポットライトのコーン角度に対する減衰率を設定します。(0..1, default: 0)
%group
LNSpotLight
%prm
spotlight, penumbra
[in] spotlight : SpotLight のハンドル
[in] penumbra  : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
SpotLight は WorldObject のサブクラスです。SpotLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
SpotLight は Object のサブクラスです。SpotLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_GetPenumbra
スポットライトのコーン角度に対する減衰率を設定します。
%group
LNSpotLight
%prm
spotlight, outReturn
[in]  spotlight : SpotLight のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
SpotLight は WorldObject のサブクラスです。SpotLight ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
SpotLight は Object のサブクラスです。SpotLight ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNTestDelegate_Create

%group
LNTestDelegate
%prm
callback, outTestDelegate
[in]  callback        : 
[out] outTestDelegate(0) : 作成された TestDelegate のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
TestDelegate は Object のサブクラスです。TestDelegate ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSprite_Create
init
%group
LNSprite
%prm
outSprite
[out] outSprite(0) : 作成された Sprite のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sprite は VisualObject のサブクラスです。Sprite ハンドルは VisualObject ハンドルとして扱うことができ、 LNVisualObject_ から始まる命令等で使用できます。
Sprite は WorldObject のサブクラスです。Sprite ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
Sprite は Object のサブクラスです。Sprite ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSprite_CreateWithTexture
init
%group
LNSprite
%prm
texture, outSprite
[in]  texture   : 
[out] outSprite(0) : 作成された Sprite のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sprite は VisualObject のサブクラスです。Sprite ハンドルは VisualObject ハンドルとして扱うことができ、 LNVisualObject_ から始まる命令等で使用できます。
Sprite は WorldObject のサブクラスです。Sprite ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
Sprite は Object のサブクラスです。Sprite ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSprite_CreateWithTextureAndSize
init
%group
LNSprite
%prm
texture, width, height, outSprite
[in]  texture   : 
[in]  width     : 
[in]  height    : 
[out] outSprite(0) : 作成された Sprite のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sprite は VisualObject のサブクラスです。Sprite ハンドルは VisualObject ハンドルとして扱うことができ、 LNVisualObject_ から始まる命令等で使用できます。
Sprite は WorldObject のサブクラスです。Sprite ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
Sprite は Object のサブクラスです。Sprite ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSprite_SetTexture
スプライトが表示するテクスチャを設定します。
%group
LNSprite
%prm
sprite, value
[in] sprite : Sprite のハンドル
[in] value  : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sprite は VisualObject のサブクラスです。Sprite ハンドルは VisualObject ハンドルとして扱うことができ、 LNVisualObject_ から始まる命令等で使用できます。
Sprite は WorldObject のサブクラスです。Sprite ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
Sprite は Object のサブクラスです。Sprite ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSprite_SetSize
スプライトの大きさを設定します。
%group
LNSprite
%prm
sprite, value
[in] sprite : Sprite のハンドル
[in] value  : (Size 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sprite は VisualObject のサブクラスです。Sprite ハンドルは VisualObject ハンドルとして扱うことができ、 LNVisualObject_ から始まる命令等で使用できます。
Sprite は WorldObject のサブクラスです。Sprite ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
Sprite は Object のサブクラスです。Sprite ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSprite_SetSizeWH
スプライトの大きさを設定します。
%group
LNSprite
%prm
sprite, width, height
[in] sprite : Sprite のハンドル
[in] width  : 
[in] height : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sprite は VisualObject のサブクラスです。Sprite ハンドルは VisualObject ハンドルとして扱うことができ、 LNVisualObject_ から始まる命令等で使用できます。
Sprite は WorldObject のサブクラスです。Sprite ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
Sprite は Object のサブクラスです。Sprite ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSprite_SetSourceRectXYWH

%group
LNSprite
%prm
sprite, x, y, width, height
[in] sprite : Sprite のハンドル
[in] x      : 
[in] y      : 
[in] width  : 
[in] height : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sprite は VisualObject のサブクラスです。Sprite ハンドルは VisualObject ハンドルとして扱うことができ、 LNVisualObject_ から始まる命令等で使用できます。
Sprite は WorldObject のサブクラスです。Sprite ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
Sprite は Object のサブクラスです。Sprite ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSprite_SetCallerTest
test
%group
LNSprite
%prm
sprite, callback
[in] sprite   : Sprite のハンドル
[in] callback : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Sprite は VisualObject のサブクラスです。Sprite ハンドルは VisualObject ハンドルとして扱うことができ、 LNVisualObject_ から始まる命令等で使用できます。
Sprite は WorldObject のサブクラスです。Sprite ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
Sprite は Object のサブクラスです。Sprite ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCameraOrbitControlComponent_Create
CameraOrbitControlComponent を作成します。
%group
LNCameraOrbitControlComponent
%prm
outCameraOrbitControlComponent
[out] outCameraOrbitControlComponent(0) : 作成された CameraOrbitControlComponent のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
CameraOrbitControlComponent は Component のサブクラスです。CameraOrbitControlComponent ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
CameraOrbitControlComponent は Object のサブクラスです。CameraOrbitControlComponent ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRaycaster_FromScreen
メインのカメラを使用して、指定したスクリーン座標から正面に向かうレイを定義した Raycaster を取得します。
%group
LNRaycaster
%prm
point, outReturn
[in]  point     : (Point 型の値) 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Raycaster は Object のサブクラスです。Raycaster ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRaycaster_IntersectPlane
指定した向きの平面との交差判定を行います。
%group
LNRaycaster
%prm
raycaster, normalX, normalY, normalZ, outReturn
[in]  raycaster : Raycaster のハンドル
[in]  normalX   : 
[in]  normalY   : 
[in]  normalZ   : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Raycaster は Object のサブクラスです。Raycaster ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRaycastResult_GetPoint
ワールド座標上の交差点
%group
LNRaycastResult
%prm
raycastresult, outReturn
[in]  raycastresult : RaycastResult のハンドル
[out] outReturn(0)  : (Vector3 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
RaycastResult は Object のサブクラスです。RaycastResult ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldRenderView_SetGuideGridEnabled
この WorldRenderView が描画する 3D シーン上に、グリッドを表示するかどうかを設定します。
%group
LNWorldRenderView
%prm
worldrenderview, value
[in] worldrenderview : WorldRenderView のハンドル
[in] value           : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldRenderView は RenderView のサブクラスです。WorldRenderView ハンドルは RenderView ハンドルとして扱うことができ、 LNRenderView_ から始まる命令等で使用できます。
WorldRenderView は Object のサブクラスです。WorldRenderView ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldRenderView_GetGuideGridEnabled
この WorldRenderView が描画する 3D シーン上に、グリッドを表示するかどうかを取得します。
%group
LNWorldRenderView
%prm
worldrenderview, outReturn
[in]  worldrenderview : WorldRenderView のハンドル
[out] outReturn(0)    : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
WorldRenderView は RenderView のサブクラスです。WorldRenderView ハンドルは RenderView ハンドルとして扱うことができ、 LNRenderView_ から始まる命令等で使用できます。
WorldRenderView は Object のサブクラスです。WorldRenderView ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPlaneMesh_Create

%group
LNPlaneMesh
%prm
outPlaneMesh
[out] outPlaneMesh(0) : 作成された PlaneMesh のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
PlaneMesh は ShapeObject のサブクラスです。PlaneMesh ハンドルは ShapeObject ハンドルとして扱うことができ、 LNShapeObject_ から始まる命令等で使用できます。
PlaneMesh は VisualObject のサブクラスです。PlaneMesh ハンドルは VisualObject ハンドルとして扱うことができ、 LNVisualObject_ から始まる命令等で使用できます。
PlaneMesh は WorldObject のサブクラスです。PlaneMesh ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
PlaneMesh は Object のサブクラスです。PlaneMesh ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNBoxMesh_Create
各軸のサイズが 1 である BoxMesh を作成します。
%group
LNBoxMesh
%prm
outBoxMesh
[out] outBoxMesh(0) : 作成された BoxMesh のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
BoxMesh は ShapeObject のサブクラスです。BoxMesh ハンドルは ShapeObject ハンドルとして扱うことができ、 LNShapeObject_ から始まる命令等で使用できます。
BoxMesh は VisualObject のサブクラスです。BoxMesh ハンドルは VisualObject ハンドルとして扱うことができ、 LNVisualObject_ から始まる命令等で使用できます。
BoxMesh は WorldObject のサブクラスです。BoxMesh ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
BoxMesh は Object のサブクラスです。BoxMesh ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNBoxMesh_CreateWithSize

%group
LNBoxMesh
%prm
width, height, depth, outBoxMesh
[in]  width      : 
[in]  height     : 
[in]  depth      : 
[out] outBoxMesh(0) : 作成された BoxMesh のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
BoxMesh は ShapeObject のサブクラスです。BoxMesh ハンドルは ShapeObject ハンドルとして扱うことができ、 LNShapeObject_ から始まる命令等で使用できます。
BoxMesh は VisualObject のサブクラスです。BoxMesh ハンドルは VisualObject ハンドルとして扱うことができ、 LNVisualObject_ から始まる命令等で使用できます。
BoxMesh は WorldObject のサブクラスです。BoxMesh ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
BoxMesh は Object のサブクラスです。BoxMesh ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshComponent_Create
init
%group
LNMeshComponent
%prm
outMeshComponent
[out] outMeshComponent(0) : 作成された MeshComponent のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
MeshComponent は VisualComponent のサブクラスです。MeshComponent ハンドルは VisualComponent ハンドルとして扱うことができ、 LNVisualComponent_ から始まる命令等で使用できます。
MeshComponent は Component のサブクラスです。MeshComponent ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
MeshComponent は Object のサブクラスです。MeshComponent ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshComponent_SetModel
setModel
%group
LNMeshComponent
%prm
meshcomponent, model
[in] meshcomponent : MeshComponent のハンドル
[in] model         : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
MeshComponent は VisualComponent のサブクラスです。MeshComponent ハンドルは VisualComponent ハンドルとして扱うことができ、 LNVisualComponent_ から始まる命令等で使用できます。
MeshComponent は Component のサブクラスです。MeshComponent ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
MeshComponent は Object のサブクラスです。MeshComponent ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshComponent_MakeCollisionBody
指定した名前の MeshContainer から、衝突判定用の Body を作成します。
%group
LNMeshComponent
%prm
meshcomponent, meshContainerName
[in] meshcomponent     : MeshComponent のハンドル
[in] meshContainerName : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
MeshComponent は VisualComponent のサブクラスです。MeshComponent ハンドルは VisualComponent ハンドルとして扱うことができ、 LNVisualComponent_ から始まる命令等で使用できます。
MeshComponent は Component のサブクラスです。MeshComponent ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
MeshComponent は Object のサブクラスです。MeshComponent ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCollision_GetWorldObject
自分自身と衝突している他の WorldObject
%group
LNCollision
%prm
collision, outReturn
[in]  collision : Collision のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Collision は Object のサブクラスです。Collision ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNTriggerBodyComponent_Create
init
%group
LNTriggerBodyComponent
%prm
outTriggerBodyComponent
[out] outTriggerBodyComponent(0) : 作成された TriggerBodyComponent のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
TriggerBodyComponent は Component のサブクラスです。TriggerBodyComponent ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
TriggerBodyComponent は Object のサブクラスです。TriggerBodyComponent ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNTriggerBodyComponent_AddCollisionShape
addCollisionShape
%group
LNTriggerBodyComponent
%prm
triggerbodycomponent, shape
[in] triggerbodycomponent : TriggerBodyComponent のハンドル
[in] shape                : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
TriggerBodyComponent は Component のサブクラスです。TriggerBodyComponent ハンドルは Component ハンドルとして扱うことができ、 LNComponent_ から始まる命令等で使用できます。
TriggerBodyComponent は Object のサブクラスです。TriggerBodyComponent ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitter_Create
init
%group
LNParticleEmitter
%prm
model, outParticleEmitter
[in]  model              : 
[out] outParticleEmitter(0) : 作成された ParticleEmitter のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
ParticleEmitter は VisualObject のサブクラスです。ParticleEmitter ハンドルは VisualObject ハンドルとして扱うことができ、 LNVisualObject_ から始まる命令等で使用できます。
ParticleEmitter は WorldObject のサブクラスです。ParticleEmitter ハンドルは WorldObject ハンドルとして扱うことができ、 LNWorldObject_ から始まる命令等で使用できます。
ParticleEmitter は Object のサブクラスです。ParticleEmitter ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetClearMode
シーン背景のクリア方法を設定します。
%group
LNScene
%prm
value
[in] value : 
                LN_SCENE_CLEAR_MODE_NONE
                クリアしません。
                LN_SCENE_CLEAR_MODE_COLOR_AND_DEPTH
                背景色と深度バッファをクリアします。
                LN_SCENE_CLEAR_MODE_SKY
                (未実装)
                LN_SCENE_CLEAR_MODE_SKY0
                (未実装)
                LN_SCENE_CLEAR_MODE_SKY_DOME
                スカイドームでクリアします。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetSkyColor
ClearMode が SkyDome であるときに使用する、空の基本色を設定します。アルファ値は、設定した色の適用率です。
%group
LNScene
%prm
value
[in] value : (Color 型の値) 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetSkyHorizonColor
ClearMode が SkyDome であるときに使用する、地平の基本色を設定します。アルファ値は、設定した色の適用率です。
%group
LNScene
%prm
value
[in] value : (Color 型の値) 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetSkyCloudColor
ClearMode が SkyDome であるときに使用する、雲の基本色を設定します。アルファ値は、設定した色の適用率です。
%group
LNScene
%prm
value
[in] value : (Color 型の値) 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetSkyOverlayColor
ClearMode が SkyDome であるときに使用する、空全体に影響する色を設定します。アルファ値は、設定した色の適用率です。
%group
LNScene
%prm
value
[in] value : (Color 型の値) 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_GotoLevel
指定したレベルへ遷移します。既存の全てのレベルは非アクティブ化または削除されます。
%group
LNScene
%prm
level, withEffect
[in] level               : 
[in] withEffect(LN_TRUE) : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_CallLevel
現在のレベルを非アクティブ化し、指定したレベルへ遷移します。
%group
LNScene
%prm
level, withEffect
[in] level               : 
[in] withEffect(LN_TRUE) : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_ReturnLevel
現在のレベルを終了し、ひとつ前のレベルへ遷移します。
%group
LNScene
%prm
withEffect
[in] withEffect(LN_TRUE) : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_ActiveLevel
現在のアクティブなレベルを取得します。
%group
LNScene
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsTransitionEffectRunning
レベルの遷移エフェクトを実行中であるかを確認します。
%group
LNScene
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTransitionEffectMode
レベル遷移時のエフェクトの種類を設定します。
%group
LNScene
%prm
value
[in] value : 
                LN_LEVEL_TRANSITION_EFFECT_MODE_NONE
                エフェクト無し
                LN_LEVEL_TRANSITION_EFFECT_MODE_FADE_IN_OUT
                フェードイン・フェードアウト
                LN_LEVEL_TRANSITION_EFFECT_MODE_CROSS_FADE
                クロスフェード

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_TransitionEffectMode
レベル遷移時のエフェクトの種類を取得します。
%group
LNScene
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。
                     LN_LEVEL_TRANSITION_EFFECT_MODE_NONE
                     エフェクト無し
                     LN_LEVEL_TRANSITION_EFFECT_MODE_FADE_IN_OUT
                     フェードイン・フェードアウト
                     LN_LEVEL_TRANSITION_EFFECT_MODE_CROSS_FADE
                     クロスフェード

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTransitionDuration
レベルの遷移にかける時間を設定します。(Unit: 秒)
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_TransitionDuration
レベルの遷移にかける時間を取得します。(Unit: 秒)
%group
LNScene
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTransitionEffectColor
レベルの遷移モードが FadeInOut である場合に使用する色を設定します。
%group
LNScene
%prm
value
[in] value : (Color 型の値) 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_TransitionEffectColor
レベルの遷移モードが FadeInOut である場合に使用する色を取得します。
%group
LNScene
%prm
outReturn
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTransitionEffectMaskTexture
レベルの遷移エフェクトで使用するマスクテクスチャを設定します。
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_TransitionEffectMaskTexture
レベルの遷移エフェクトで使用するマスクテクスチャを取得します。
%group
LNScene
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTransitionEffectVague
レベルの遷移エフェクトの境界のあいまいさを設定します。
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_TransitionEffectVague
レベルの遷移エフェクトの境界のあいまいさを取得します。
%group
LNScene
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_StartFadeOut
フェードアウトエフェクトを開始します。
%group
LNScene
%prm


stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_StartFadeIn
フェードインエフェクトを開始します。
%group
LNScene
%prm


stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetFogStartDistance
フォグを開始するカメラからの距離を設定します。
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetFogColor
フォグのメインカラーを設定します。
%group
LNScene
%prm
value
[in] value : (Color 型の値) 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetFogDensity
フォグの濃さを設定します。
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetFogHeightDensity
高さフォグの濃さを設定します。
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetFogLowerHeight
フォグの高さの下限を設定します。
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetFogUpperHeight
フォグの高さの上限を設定します。
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetHDREnabled
HDR レンダリングの有無を設定します。 (default: false)
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsHDREnabled
HDR レンダリングの有無を取得します。
%group
LNScene
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetScreenBlendColor
画面全体へのブレンドカラーを設定します。(default: Black)
%group
LNScene
%prm
value
[in] value : (Color 型の値) 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_ScreenBlendColor
画面全体へのブレンドカラーを取得します。
%group
LNScene
%prm
outReturn
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetColorTone
setColorTone
%group
LNScene
%prm
value
[in] value : (ColorTone 型の値) 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_ColorTone
colorTone
%group
LNScene
%prm
outReturn
[out] outReturn(0) : (ColorTone 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetAntialiasEnabled
アンチエイリアスの有無を設定します。(default: false)
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsAntialiasEnabled
アンチエイリアスの有無を取得します。
%group
LNScene
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetSSREnabled
SSR (Screen Space Reflection) の有無を設定します。(default: false)
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsSSREnabled
SSR の有無を取得します。
%group
LNScene
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetSSAOEnabled
SSAO (Screen Space Ambient Occlusion) の有無を設定します。(default: false)
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsSSAOEnabled
SSAO の有無を取得します。
%group
LNScene
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetBloomEnabled
ブルームエフェクトの有無を設定します。(default: false)
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsBloomEnabled
ブルームエフェクトの有無を取得します。
%group
LNScene
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetDOFEnabled
被写界深度の有無を設定します。(default: false)
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsDOFEnabled
被写界深度の有無を取得します。
%group
LNScene
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTonemapEnabled
トーンマッピングの有無を設定します。(default: false)
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsTonemapEnabled
トーンマッピングの有無を取得します。
%group
LNScene
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetVignetteEnabled
ビネットエフェクトの有無を設定します。(default: false)
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsVignetteEnabled
ビネットエフェクトの有無を取得します。
%group
LNScene
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetGammaEnabled
ガンマ補正の有無を設定します。(default: false)
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsGammaEnabled
ガンマ補正の有無を取得します。
%group
LNScene
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTonemapExposure
Tonemap exposure
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTonemapLinearWhite
setTonemapLinearWhite
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTonemapShoulderStrength
setTonemapShoulderStrength
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTonemapLinearStrength
setTonemapLinearStrength
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTonemapLinearAngle
setTonemapLinearAngle
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTonemapToeStrength
setTonemapToeStrength
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTonemapToeNumerator
setTonemapToeNumerator
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTonemapToeDenominator
setTonemapToeDenominator
%group
LNScene
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLevel_Create
Initialize
%group
LNLevel
%prm
outLevel
[out] outLevel(0) : 作成された Level のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Level は AssetObject のサブクラスです。Level ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
Level は Object のサブクラスです。Level ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLevel_AddObject
指定した WorldObject を、この Level 子オブジェクトとして追加します。
%group
LNLevel
%prm
level, obj
[in] level : Level のハンドル
[in] obj   : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Level は AssetObject のサブクラスです。Level ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
Level は Object のサブクラスです。Level ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLevel_RemoveObject
指定した WorldObject を、この Level 子オブジェクトか除外します。
%group
LNLevel
%prm
level, obj
[in] level : Level のハンドル
[in] obj   : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Level は AssetObject のサブクラスです。Level ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
Level は Object のサブクラスです。Level ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLevel_AddSubLevel
指定した Level を、この Level の Sub-Level として追加します。
%group
LNLevel
%prm
level, sublevel
[in] level    : Level のハンドル
[in] sublevel : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Level は AssetObject のサブクラスです。Level ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
Level は Object のサブクラスです。Level ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLevel_RemoveSubLevel
指定した Level を、この Level の Sub-Level から除外します。
%group
LNLevel
%prm
level, sublevel
[in] level    : Level のハンドル
[in] sublevel : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Level は AssetObject のサブクラスです。Level ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
Level は Object のサブクラスです。Level ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLevel_RemoveAllSubLevels
すべての Sub-Level を除外します。
%group
LNLevel
%prm
level
[in] level : Level のハンドル

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Level は AssetObject のサブクラスです。Level ハンドルは AssetObject ハンドルとして扱うことができ、 LNAssetObject_ から始まる命令等で使用できます。
Level は Object のサブクラスです。Level ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Red
色の濃さ (0~9) を指定して、 Red カラーパレットの色を取得します。
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Pink
色の濃さ (0~9) を指定して、 Pink カラーパレットの色を取得します。
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Purple
色の濃さ (0~9) を指定して、 Purple カラーパレットの色を取得します。
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_DeepPurple
色の濃さ (0~9) を指定して、 DeepPurple カラーパレットの色を取得します。
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Indigo
色の濃さ (0~9) を指定して、 Indigo カラーパレットの色を取得します。
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Blue
色の濃さ (0~9) を指定して、 Blue カラーパレットの色を取得します。
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_LightBlue
色の濃さ (0~9) を指定して、 LightBlue カラーパレットの色を取得します。
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Cyan
色の濃さ (0~9) を指定して、 Cyan カラーパレットの色を取得します。
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Teal
色の濃さ (0~9) を指定して、 Teal カラーパレットの色を取得します。
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Green
色の濃さ (0~9) を指定して、 Green カラーパレットの色を取得します。
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_LightGreen
色の濃さ (0~9) を指定して、 LightGreen カラーパレットの色を取得します。
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Lime
色の濃さ (0~9) を指定して、 Lime カラーパレットの色を取得します。
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Yellow
色の濃さ (0~9) を指定して、 Yellow カラーパレットの色を取得します。
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Amber
色の濃さ (0~9) を指定して、 Amber カラーパレットの色を取得します。
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Orange
色の濃さ (0~9) を指定して、 Orange カラーパレットの色を取得します。
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_DeepOrange
色の濃さ (0~9) を指定して、 DeepOrange カラーパレットの色を取得します。
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Brown
色の濃さ (0~9) を指定して、 Brown カラーパレットの色を取得します。
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Grey
色の濃さ (0~9) を指定して、 Grey カラーパレットの色を取得します。
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Get
カラーインデックスと色の濃さ (0~9) を指定して、色を取得します。
%group
LNUIColors
%prm
hue, shades, outReturn
[in]  hue       : 
                     LN_UICOLOR_HUES_RED
                     
                     LN_UICOLOR_HUES_PINK
                     
                     LN_UICOLOR_HUES_PURPLE
                     
                     LN_UICOLOR_HUES_DEEP_PURPLE
                     
                     LN_UICOLOR_HUES_INDIGO
                     
                     LN_UICOLOR_HUES_BLUE
                     
                     LN_UICOLOR_HUES_LIGHT_BLUE
                     
                     LN_UICOLOR_HUES_CYAN
                     
                     LN_UICOLOR_HUES_TEAL
                     
                     LN_UICOLOR_HUES_GREEN
                     
                     LN_UICOLOR_HUES_LIGHT_GREEN
                     
                     LN_UICOLOR_HUES_LIME
                     
                     LN_UICOLOR_HUES_YELLOW
                     
                     LN_UICOLOR_HUES_AMBER
                     
                     LN_UICOLOR_HUES_ORANGE
                     
                     LN_UICOLOR_HUES_DEEP_ORANGE
                     
                     LN_UICOLOR_HUES_BROWN
                     
                     LN_UICOLOR_HUES_GREY
                     
                     LN_UICOLOR_HUES_BLACK
                     
                     LN_UICOLOR_HUES_WHITE
                     
[in]  shades(5) : 
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIEventArgs_Sender
イベントの発生元となった要素を取得します。
%group
LNUIEventArgs
%prm
uieventargs, outReturn
[in]  uieventargs : UIEventArgs のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIEventArgs は Object のサブクラスです。UIEventArgs ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIGeneralEventHandler_Create

%group
LNUIGeneralEventHandler
%prm
callback, outUIGeneralEventHandler
[in]  callback                 : 
[out] outUIGeneralEventHandler(0) : 作成された UIGeneralEventHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIGeneralEventHandler は Object のサブクラスです。UIGeneralEventHandler ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIEventHandler_Create

%group
LNUIEventHandler
%prm
callback, outUIEventHandler
[in]  callback          : 
[out] outUIEventHandler(0) : 作成された UIEventHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIEventHandler は Object のサブクラスです。UIEventHandler ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetSize
要素のサイズを設定します。サイズには、border と padding の幅と高さは含まれません。(例：width 10, border 10 とすると、要素の最終サイズは 20 となります)
%group
LNUIElement
%prm
uielement, size
[in] uielement : UIElement のハンドル
[in] size      : (Size 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetSizeWH
要素のサイズを設定します。サイズには、border と padding の幅と高さは含まれません。(例：width 10, border 10 とすると、要素の最終サイズは 20 となります)
%group
LNUIElement
%prm
uielement, width, height
[in] uielement : UIElement のハンドル
[in] width     : 
[in] height    : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetWidth
setWidth
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement のハンドル
[in] value     : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetWidth
width
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetHeight
setHeight
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement のハンドル
[in] value     : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetHeight
height
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetMargin
要素の margin 値 (外側の余白) を設定します。
%group
LNUIElement
%prm
uielement, margin
[in] uielement : UIElement のハンドル
[in] margin    : (Thickness 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetMargin
要素の margin 値 (外側の余白) を取得します。
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : (Thickness 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetPadding
要素の padding 値 (内側の余白) を設定します。この余白は論理ツリーの子要素のレイアウトに影響します。
%group
LNUIElement
%prm
uielement, padding
[in] uielement : UIElement のハンドル
[in] padding   : (Thickness 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetPadding
要素の padding 値 (内側の余白) を取得します。この余白は論理ツリーの子要素のレイアウトに影響します。
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : (Thickness 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetHAlignment
要素の横方向の配置方法を設定します。
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement のハンドル
[in] value     : 
                    LN_UIHALIGNMENT_LEFT
                    子要素を、親のレイアウト スロットの左側に揃えて配置します。
                    LN_UIHALIGNMENT_CENTER
                    子要素を、親のレイアウト スロットの中央に揃えて配置します。
                    LN_UIHALIGNMENT_RIGHT
                    子要素を、親のレイアウト スロットの右側に揃えて配置します。
                    LN_UIHALIGNMENT_STRETCH
                    子要素を、親のレイアウト スロット全体に引き伸ばします。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetHAlignment
要素の横方向の配置方法を取得します。
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : 結果を格納する変数。
                     LN_UIHALIGNMENT_LEFT
                     子要素を、親のレイアウト スロットの左側に揃えて配置します。
                     LN_UIHALIGNMENT_CENTER
                     子要素を、親のレイアウト スロットの中央に揃えて配置します。
                     LN_UIHALIGNMENT_RIGHT
                     子要素を、親のレイアウト スロットの右側に揃えて配置します。
                     LN_UIHALIGNMENT_STRETCH
                     子要素を、親のレイアウト スロット全体に引き伸ばします。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetVAlignment
要素の縦方向の配置方法を設定します。
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement のハンドル
[in] value     : 
                    LN_UIVALIGNMENT_TOP
                    子要素を、親のレイアウト スロットの上端に揃えて配置します。
                    LN_UIVALIGNMENT_CENTER
                    子要素を、親のレイアウト スロットの中央に揃えて配置します。
                    LN_UIVALIGNMENT_BOTTOM
                    子要素を、親のレイアウト スロットの下端に揃えて配置します。
                    LN_UIVALIGNMENT_STRETCH
                    子要素を、親のレイアウト スロット全体に引き伸ばします。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetVAlignment
要素の縦方向の配置方法を取得します。
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : 結果を格納する変数。
                     LN_UIVALIGNMENT_TOP
                     子要素を、親のレイアウト スロットの上端に揃えて配置します。
                     LN_UIVALIGNMENT_CENTER
                     子要素を、親のレイアウト スロットの中央に揃えて配置します。
                     LN_UIVALIGNMENT_BOTTOM
                     子要素を、親のレイアウト スロットの下端に揃えて配置します。
                     LN_UIVALIGNMENT_STRETCH
                     子要素を、親のレイアウト スロット全体に引き伸ばします。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetAlignments
要素の配置方法を設定します。
%group
LNUIElement
%prm
uielement, halign, valign
[in] uielement : UIElement のハンドル
[in] halign    : 
                    LN_UIHALIGNMENT_LEFT
                    子要素を、親のレイアウト スロットの左側に揃えて配置します。
                    LN_UIHALIGNMENT_CENTER
                    子要素を、親のレイアウト スロットの中央に揃えて配置します。
                    LN_UIHALIGNMENT_RIGHT
                    子要素を、親のレイアウト スロットの右側に揃えて配置します。
                    LN_UIHALIGNMENT_STRETCH
                    子要素を、親のレイアウト スロット全体に引き伸ばします。
[in] valign    : 
                    LN_UIVALIGNMENT_TOP
                    子要素を、親のレイアウト スロットの上端に揃えて配置します。
                    LN_UIVALIGNMENT_CENTER
                    子要素を、親のレイアウト スロットの中央に揃えて配置します。
                    LN_UIVALIGNMENT_BOTTOM
                    子要素を、親のレイアウト スロットの下端に揃えて配置します。
                    LN_UIVALIGNMENT_STRETCH
                    子要素を、親のレイアウト スロット全体に引き伸ばします。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetPosition
このオブジェクトの位置を設定します。
%group
LNUIElement
%prm
uielement, pos
[in] uielement : UIElement のハンドル
[in] pos       : (Vector3 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetPositionXYZ
このオブジェクトの位置を設定します。
%group
LNUIElement
%prm
uielement, x, y, z
[in] uielement   : UIElement のハンドル
[in] x           : 
[in] y           : 
[in] z(0.000000) : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetPosition
このオブジェクトの位置を位置を取得します。
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : (Vector3 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetRotation
このオブジェクトの回転を設定します。
%group
LNUIElement
%prm
uielement, rot
[in] uielement : UIElement のハンドル
[in] rot       : (Quaternion 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetEulerAngles
このオブジェクトの回転をオイラー角から設定します。(radian)
%group
LNUIElement
%prm
uielement, x, y, z
[in] uielement : UIElement のハンドル
[in] x         : 
[in] y         : 
[in] z         : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetRotation
このオブジェクトの回転を取得します。
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : (Quaternion 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetScale
このオブジェクトの拡大率を設定します。
%group
LNUIElement
%prm
uielement, scale
[in] uielement : UIElement のハンドル
[in] scale     : (Vector3 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetScaleS
このオブジェクトの拡大率を設定します。
%group
LNUIElement
%prm
uielement, xyz
[in] uielement : UIElement のハンドル
[in] xyz       : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetScaleXY
このオブジェクトの拡大率を設定します。
%group
LNUIElement
%prm
uielement, x, y
[in] uielement : UIElement のハンドル
[in] x         : 
[in] y         : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetScale
このオブジェクトの拡大率を取得します。
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : (Vector3 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetCenterPoint
このオブジェクトのローカルの中心位置を設定します。
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement のハンドル
[in] value     : (Vector3 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetCenterPointXYZ
このオブジェクトのローカルの中心位置を設定します。
%group
LNUIElement
%prm
uielement, x, y, z
[in] uielement   : UIElement のハンドル
[in] x           : 
[in] y           : 
[in] z(0.000000) : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetCenterPoint
このオブジェクトのローカルの中心位置を取得します。
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : (Vector3 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetEnabled
要素の有効状態を設定します。
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement のハンドル
[in] value     : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_IsEnabled
要素の有効状態を取得します。
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetData
任意のユーザーデータを設定します。
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement のハンドル
[in] value     : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetData
任意のユーザーデータを取得します。
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetBackgroundColor
背景の色を設定します。
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement のハンドル
[in] value     : (Color 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetBackgroundColor
背景の色を取得します。
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetBorderThickness
枠線の太さを設定します。
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement のハンドル
[in] value     : (Thickness 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetBorderThickness
枠線の太さを取得します。
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : (Thickness 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetBorderColor
枠線の色を設定します。
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement のハンドル
[in] value     : (Color 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetBorderColor
枠線の色を取得します。
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : (Color 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetCornerRadius
四角形の角の半径を設定します。
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement のハンドル
[in] value     : (CornerRadius 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetCornerRadius
四角形の角の半径を取得します。
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : (CornerRadius 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetVisibility
可視状態を設定します。(default: UIVisibility::Visible)
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement のハンドル
[in] value     : 
                    LN_UIVISIBILITY_VISIBLE
                    要素を表示します。
                    LN_UIVISIBILITY_HIDDEN
                    要素を表示しませんが、その要素の領域をレイアウト内に予約します。
                    LN_UIVISIBILITY_COLLAPSED
                    要素を表示しません。また、その要素の領域もレイアウト内に予約しません。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetVisibility
可視状態を取得します。
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : 結果を格納する変数。
                     LN_UIVISIBILITY_VISIBLE
                     要素を表示します。
                     LN_UIVISIBILITY_HIDDEN
                     要素を表示しませんが、その要素の領域をレイアウト内に予約します。
                     LN_UIVISIBILITY_COLLAPSED
                     要素を表示しません。また、その要素の領域もレイアウト内に予約しません。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetOpacity
不透明度を設定します。(default: 1.0)
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement のハンドル
[in] value     : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetOpacity
不透明度を取得します。
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_AddChild
Add element to container. 論理的な子要素として追加する。
%group
LNUIElement
%prm
uielement, child
[in] uielement : UIElement のハンドル
[in] child     : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetFocusable
入力フォーカスを得ることができるかどうかを設定します。(default: false)
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement のハンドル
[in] value     : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetFocusable
入力フォーカスを得ることができるかどうかを取得します。
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_AddInto
この UIElement を指定した UIElement へ子要素として追加します。省略した場合はデフォルトのルート UIElement へ追加します。
%group
LNUIElement
%prm
uielement, parent
[in] uielement              : UIElement のハンドル
[in] parent(LN_NULL_HANDLE) : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIElement は UILayoutElement のサブクラスです。UIElement ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIElement は Object のサブクラスです。UIElement ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIText_Create
UIText を作成します。
%group
LNUIText
%prm
outUIText
[out] outUIText(0) : 作成された UIText のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIText は UIElement のサブクラスです。UIText ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIText は UILayoutElement のサブクラスです。UIText ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIText は Object のサブクラスです。UIText ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIText_CreateWithText
表示文字列を指定して、UITextBlock を作成します。
%group
LNUIText
%prm
text, outUIText
[in]  text      : 
[out] outUIText(0) : 作成された UIText のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIText は UIElement のサブクラスです。UIText ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIText は UILayoutElement のサブクラスです。UIText ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIText は Object のサブクラスです。UIText ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIText_SetText
表示文字列を設定します。
%group
LNUIText
%prm
uitext, value
[in] uitext : UIText のハンドル
[in] value  : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIText は UIElement のサブクラスです。UIText ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIText は UILayoutElement のサブクラスです。UIText ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIText は Object のサブクラスです。UIText ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIText_GetText
表示文字列を取得します。
%group
LNUIText
%prm
uitext, outReturn
[in]  uitext    : UIText のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIText は UIElement のサブクラスです。UIText ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIText は UILayoutElement のサブクラスです。UIText ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIText は Object のサブクラスです。UIText ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUISprite_Create
UISprite を作成します。
%group
LNUISprite
%prm
outUISprite
[out] outUISprite(0) : 作成された UISprite のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UISprite は UIElement のサブクラスです。UISprite ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UISprite は UILayoutElement のサブクラスです。UISprite ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UISprite は Object のサブクラスです。UISprite ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUISprite_CreateWithTexture
テクスチャを指定して UISprite を作成します。
%group
LNUISprite
%prm
texture, outUISprite
[in]  texture     : 
[out] outUISprite(0) : 作成された UISprite のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UISprite は UIElement のサブクラスです。UISprite ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UISprite は UILayoutElement のサブクラスです。UISprite ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UISprite は Object のサブクラスです。UISprite ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUISprite_SetTexture
スプライトが表示するテクスチャを設定します。
%group
LNUISprite
%prm
uisprite, texture
[in] uisprite : UISprite のハンドル
[in] texture  : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UISprite は UIElement のサブクラスです。UISprite ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UISprite は UILayoutElement のサブクラスです。UISprite ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UISprite は Object のサブクラスです。UISprite ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUISprite_SetSourceRect
テクスチャのどの部分を表示するかを示す転送矩形を設定します。(ピクセル単位) デフォルトは Rect::Empty で、テクスチャ全体を転送することを示します。
%group
LNUISprite
%prm
uisprite, rect
[in] uisprite : UISprite のハンドル
[in] rect     : (Rect 型の値) 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UISprite は UIElement のサブクラスです。UISprite ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UISprite は UILayoutElement のサブクラスです。UISprite ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UISprite は Object のサブクラスです。UISprite ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUISprite_SetSourceRectXYWH

%group
LNUISprite
%prm
uisprite, x, y, width, height
[in] uisprite : UISprite のハンドル
[in] x        : 
[in] y        : 
[in] width    : 
[in] height   : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UISprite は UIElement のサブクラスです。UISprite ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UISprite は UILayoutElement のサブクラスです。UISprite ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UISprite は Object のサブクラスです。UISprite ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUISprite_GetSourceRect
テクスチャのどの部分を表示するかを示す転送矩形を取得します。(ピクセル単位)
%group
LNUISprite
%prm
uisprite, outReturn
[in]  uisprite  : UISprite のハンドル
[out] outReturn(0) : (Rect 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UISprite は UIElement のサブクラスです。UISprite ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UISprite は UILayoutElement のサブクラスです。UISprite ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UISprite は Object のサブクラスです。UISprite ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUISprite_SetShader
setShader
%group
LNUISprite
%prm
uisprite, shader
[in] uisprite : UISprite のハンドル
[in] shader   : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UISprite は UIElement のサブクラスです。UISprite ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UISprite は UILayoutElement のサブクラスです。UISprite ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UISprite は Object のサブクラスです。UISprite ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIIcon_LoadFontIcon
アイコン名を指定して UIIcon を作成します。
%group
LNUIIcon
%prm
iconName, outReturn
[in]  iconName  : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIIcon は UIElement のサブクラスです。UIIcon ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIIcon は UILayoutElement のサブクラスです。UIIcon ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIIcon は Object のサブクラスです。UIIcon ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIIcon_LoadFontIconWithNameSize
アイコン名・サイズ を指定して UIIcon を作成します。
%group
LNUIIcon
%prm
iconName, size, outReturn
[in]  iconName  : 
[in]  size      : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIIcon は UIElement のサブクラスです。UIIcon ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIIcon は UILayoutElement のサブクラスです。UIIcon ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIIcon は Object のサブクラスです。UIIcon ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIIcon_LoadFontIconWithNameSizeColor
アイコン名・サイズ・色 (TextColor) を指定して UIIcon を作成します。
%group
LNUIIcon
%prm
iconName, size, color, outReturn
[in]  iconName  : 
[in]  size      : 
[in]  color     : (Color 型の値) 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIIcon は UIElement のサブクラスです。UIIcon ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIIcon は UILayoutElement のサブクラスです。UIIcon ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIIcon は Object のサブクラスです。UIIcon ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIMessageTextArea_Create
init
%group
LNUIMessageTextArea
%prm
outUIMessageTextArea
[out] outUIMessageTextArea(0) : 作成された UIMessageTextArea のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIMessageTextArea は UIElement のサブクラスです。UIMessageTextArea ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIMessageTextArea は UILayoutElement のサブクラスです。UIMessageTextArea ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIMessageTextArea は Object のサブクラスです。UIMessageTextArea ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIMessageTextArea_SetText
setText
%group
LNUIMessageTextArea
%prm
uimessagetextarea, value
[in] uimessagetextarea : UIMessageTextArea のハンドル
[in] value             : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIMessageTextArea は UIElement のサブクラスです。UIMessageTextArea ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIMessageTextArea は UILayoutElement のサブクラスです。UIMessageTextArea ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIMessageTextArea は Object のサブクラスです。UIMessageTextArea ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIMessageTextArea_SetTypingSpeed
setTypingSpeed
%group
LNUIMessageTextArea
%prm
uimessagetextarea, value
[in] uimessagetextarea : UIMessageTextArea のハンドル
[in] value             : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIMessageTextArea は UIElement のサブクラスです。UIMessageTextArea ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIMessageTextArea は UILayoutElement のサブクラスです。UIMessageTextArea ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIMessageTextArea は Object のサブクラスです。UIMessageTextArea ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUI_Add
add
%group
LNUI
%prm
element
[in] element : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUI_Remove
remove
%group
LNUI
%prm
element
[in] element : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIBoxLayout_Create
init
%group
LNUIBoxLayout
%prm
outUIBoxLayout
[out] outUIBoxLayout(0) : 作成された UIBoxLayout のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIBoxLayout は UILayoutPanel のサブクラスです。UIBoxLayout ハンドルは UILayoutPanel ハンドルとして扱うことができ、 LNUILayoutPanel_ から始まる命令等で使用できます。
UIBoxLayout は UIElement のサブクラスです。UIBoxLayout ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIBoxLayout は UILayoutElement のサブクラスです。UIBoxLayout ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIBoxLayout は Object のサブクラスです。UIBoxLayout ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIBoxLayout_SetOrientation
レイアウト方向を設定します。(default: Vertical)
%group
LNUIBoxLayout
%prm
uiboxlayout, orientation
[in] uiboxlayout : UIBoxLayout のハンドル
[in] orientation : 
                      LN_UILAYOUT_ORIENTATION_HORIZONTAL
                      水平方向に配置します。
                      LN_UILAYOUT_ORIENTATION_VERTICAL
                      垂直方向に配置します。
                      LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL
                      水平方向（右から左）に配置します。
                      LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL
                      垂直方向（下から上）に配置します。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIBoxLayout は UILayoutPanel のサブクラスです。UIBoxLayout ハンドルは UILayoutPanel ハンドルとして扱うことができ、 LNUILayoutPanel_ から始まる命令等で使用できます。
UIBoxLayout は UIElement のサブクラスです。UIBoxLayout ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIBoxLayout は UILayoutElement のサブクラスです。UIBoxLayout ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIBoxLayout は Object のサブクラスです。UIBoxLayout ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIBoxLayout_GetOrientation
レイアウト方向を取得します。
%group
LNUIBoxLayout
%prm
uiboxlayout, outReturn
[in]  uiboxlayout : UIBoxLayout のハンドル
[out] outReturn(0) : 結果を格納する変数。
                       LN_UILAYOUT_ORIENTATION_HORIZONTAL
                       水平方向に配置します。
                       LN_UILAYOUT_ORIENTATION_VERTICAL
                       垂直方向に配置します。
                       LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL
                       水平方向（右から左）に配置します。
                       LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL
                       垂直方向（下から上）に配置します。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIBoxLayout は UILayoutPanel のサブクラスです。UIBoxLayout ハンドルは UILayoutPanel ハンドルとして扱うことができ、 LNUILayoutPanel_ から始まる命令等で使用できます。
UIBoxLayout は UIElement のサブクラスです。UIBoxLayout ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIBoxLayout は UILayoutElement のサブクラスです。UIBoxLayout ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIBoxLayout は Object のサブクラスです。UIBoxLayout ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIStackLayout_Create
init
%group
LNUIStackLayout
%prm
outUIStackLayout
[out] outUIStackLayout(0) : 作成された UIStackLayout のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIStackLayout は UILayoutPanel のサブクラスです。UIStackLayout ハンドルは UILayoutPanel ハンドルとして扱うことができ、 LNUILayoutPanel_ から始まる命令等で使用できます。
UIStackLayout は UIElement のサブクラスです。UIStackLayout ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIStackLayout は UILayoutElement のサブクラスです。UIStackLayout ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIStackLayout は Object のサブクラスです。UIStackLayout ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIStackLayout_SetOrientation
(default: Vertical)
%group
LNUIStackLayout
%prm
uistacklayout, orientation
[in] uistacklayout : UIStackLayout のハンドル
[in] orientation   : 
                        LN_UILAYOUT_ORIENTATION_HORIZONTAL
                        水平方向に配置します。
                        LN_UILAYOUT_ORIENTATION_VERTICAL
                        垂直方向に配置します。
                        LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL
                        水平方向（右から左）に配置します。
                        LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL
                        垂直方向（下から上）に配置します。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIStackLayout は UILayoutPanel のサブクラスです。UIStackLayout ハンドルは UILayoutPanel ハンドルとして扱うことができ、 LNUILayoutPanel_ から始まる命令等で使用できます。
UIStackLayout は UIElement のサブクラスです。UIStackLayout ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIStackLayout は UILayoutElement のサブクラスです。UIStackLayout ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIStackLayout は Object のサブクラスです。UIStackLayout ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIStackLayout_GetOrientation

%group
LNUIStackLayout
%prm
uistacklayout, outReturn
[in]  uistacklayout : UIStackLayout のハンドル
[out] outReturn(0)  : 結果を格納する変数。
                         LN_UILAYOUT_ORIENTATION_HORIZONTAL
                         水平方向に配置します。
                         LN_UILAYOUT_ORIENTATION_VERTICAL
                         垂直方向に配置します。
                         LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL
                         水平方向（右から左）に配置します。
                         LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL
                         垂直方向（下から上）に配置します。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIStackLayout は UILayoutPanel のサブクラスです。UIStackLayout ハンドルは UILayoutPanel ハンドルとして扱うことができ、 LNUILayoutPanel_ から始まる命令等で使用できます。
UIStackLayout は UIElement のサブクラスです。UIStackLayout ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIStackLayout は UILayoutElement のサブクラスです。UIStackLayout ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIStackLayout は Object のサブクラスです。UIStackLayout ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIGridLayout_Create
init
%group
LNUIGridLayout
%prm
outUIGridLayout
[out] outUIGridLayout(0) : 作成された UIGridLayout のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIGridLayout は UILayoutPanel のサブクラスです。UIGridLayout ハンドルは UILayoutPanel ハンドルとして扱うことができ、 LNUILayoutPanel_ から始まる命令等で使用できます。
UIGridLayout は UIElement のサブクラスです。UIGridLayout ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIGridLayout は UILayoutElement のサブクラスです。UIGridLayout ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIGridLayout は Object のサブクラスです。UIGridLayout ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIGridLayout_SetColumnCount
setColumnCount
%group
LNUIGridLayout
%prm
uigridlayout, value
[in] uigridlayout : UIGridLayout のハンドル
[in] value        : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIGridLayout は UILayoutPanel のサブクラスです。UIGridLayout ハンドルは UILayoutPanel ハンドルとして扱うことができ、 LNUILayoutPanel_ から始まる命令等で使用できます。
UIGridLayout は UIElement のサブクラスです。UIGridLayout ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIGridLayout は UILayoutElement のサブクラスです。UIGridLayout ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIGridLayout は Object のサブクラスです。UIGridLayout ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIGridLayout_SetRow
setRow
%group
LNUIGridLayout
%prm
element, row
[in] element : 
[in] row     : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIGridLayout は UILayoutPanel のサブクラスです。UIGridLayout ハンドルは UILayoutPanel ハンドルとして扱うことができ、 LNUILayoutPanel_ から始まる命令等で使用できます。
UIGridLayout は UIElement のサブクラスです。UIGridLayout ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIGridLayout は UILayoutElement のサブクラスです。UIGridLayout ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIGridLayout は Object のサブクラスです。UIGridLayout ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIGridLayout_SetColumn
setColumn
%group
LNUIGridLayout
%prm
element, column
[in] element : 
[in] column  : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIGridLayout は UILayoutPanel のサブクラスです。UIGridLayout ハンドルは UILayoutPanel ハンドルとして扱うことができ、 LNUILayoutPanel_ から始まる命令等で使用できます。
UIGridLayout は UIElement のサブクラスです。UIGridLayout ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIGridLayout は UILayoutElement のサブクラスです。UIGridLayout ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIGridLayout は Object のサブクラスです。UIGridLayout ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIGridLayout_SetPlacement
setPlacement
%group
LNUIGridLayout
%prm
element, row, column, rowSpan, columnSpan
[in] element       : 
[in] row           : 
[in] column        : 
[in] rowSpan(1)    : 
[in] columnSpan(1) : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIGridLayout は UILayoutPanel のサブクラスです。UIGridLayout ハンドルは UILayoutPanel ハンドルとして扱うことができ、 LNUILayoutPanel_ から始まる命令等で使用できます。
UIGridLayout は UIElement のサブクラスです。UIGridLayout ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIGridLayout は UILayoutElement のサブクラスです。UIGridLayout ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIGridLayout は Object のサブクラスです。UIGridLayout ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIControl_Create
init
%group
LNUIControl
%prm
outUIControl
[out] outUIControl(0) : 作成された UIControl のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIControl は UIElement のサブクラスです。UIControl ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIControl は UILayoutElement のサブクラスです。UIControl ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIControl は Object のサブクラスです。UIControl ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIControl_AddInlineVisual
addInlineVisual
%group
LNUIControl
%prm
uicontrol, element, layout
[in] uicontrol : UIControl のハンドル
[in] element   : 
[in] layout    : 
                    LN_UIINLINE_PLACEMENT_TOP_LEFT
                    TopLeft
                    LN_UIINLINE_PLACEMENT_TOP
                    Top
                    LN_UIINLINE_PLACEMENT_TOP_RIGHT
                    TopRight
                    LN_UIINLINE_PLACEMENT_LEFT
                    Left
                    LN_UIINLINE_PLACEMENT_CENTER
                    Center
                    LN_UIINLINE_PLACEMENT_RIGHT
                    Right
                    LN_UIINLINE_PLACEMENT_BOTTOM_LEFT
                    BottomLeft
                    LN_UIINLINE_PLACEMENT_BOTTOM
                    Bottom
                    LN_UIINLINE_PLACEMENT_BOTTOM_RIGHT
                    BottomRight

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIControl は UIElement のサブクラスです。UIControl ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIControl は UILayoutElement のサブクラスです。UIControl ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIControl は Object のサブクラスです。UIControl ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIButtonBase_SetText
set text.
%group
LNUIButtonBase
%prm
uibuttonbase, text
[in] uibuttonbase : UIButtonBase のハンドル
[in] text         : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIButtonBase は UIControl のサブクラスです。UIButtonBase ハンドルは UIControl ハンドルとして扱うことができ、 LNUIControl_ から始まる命令等で使用できます。
UIButtonBase は UIElement のサブクラスです。UIButtonBase ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIButtonBase は UILayoutElement のサブクラスです。UIButtonBase ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIButtonBase は Object のサブクラスです。UIButtonBase ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIButton_Create
init.
%group
LNUIButton
%prm
outUIButton
[out] outUIButton(0) : 作成された UIButton のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIButton は UIButtonBase のサブクラスです。UIButton ハンドルは UIButtonBase ハンドルとして扱うことができ、 LNUIButtonBase_ から始まる命令等で使用できます。
UIButton は UIControl のサブクラスです。UIButton ハンドルは UIControl ハンドルとして扱うことができ、 LNUIControl_ から始まる命令等で使用できます。
UIButton は UIElement のサブクラスです。UIButton ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIButton は UILayoutElement のサブクラスです。UIButton ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIButton は Object のサブクラスです。UIButton ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIButton_CreateWithText
表示文字列を指定して UIButton を作成します。
%group
LNUIButton
%prm
text, outUIButton
[in]  text        : 
[out] outUIButton(0) : 作成された UIButton のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIButton は UIButtonBase のサブクラスです。UIButton ハンドルは UIButtonBase ハンドルとして扱うことができ、 LNUIButtonBase_ から始まる命令等で使用できます。
UIButton は UIControl のサブクラスです。UIButton ハンドルは UIControl ハンドルとして扱うことができ、 LNUIControl_ から始まる命令等で使用できます。
UIButton は UIElement のサブクラスです。UIButton ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIButton は UILayoutElement のサブクラスです。UIButton ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIButton は Object のサブクラスです。UIButton ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIButton_ConnectOnClicked
Clicked イベントの通知を受け取るコールバックを登録します。
%group
LNUIButton
%prm
uibutton, handler, outReturn
[in]  uibutton  : UIButton のハンドル
[in]  handler   : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIButton は UIButtonBase のサブクラスです。UIButton ハンドルは UIButtonBase ハンドルとして扱うことができ、 LNUIButtonBase_ から始まる命令等で使用できます。
UIButton は UIControl のサブクラスです。UIButton ハンドルは UIControl ハンドルとして扱うことができ、 LNUIControl_ から始まる命令等で使用できます。
UIButton は UIElement のサブクラスです。UIButton ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIButton は UILayoutElement のサブクラスです。UIButton ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIButton は Object のサブクラスです。UIButton ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIWindow_Create
init
%group
LNUIWindow
%prm
outUIWindow
[out] outUIWindow(0) : 作成された UIWindow のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIWindow は UIControl のサブクラスです。UIWindow ハンドルは UIControl ハンドルとして扱うことができ、 LNUIControl_ から始まる命令等で使用できます。
UIWindow は UIElement のサブクラスです。UIWindow ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIWindow は UILayoutElement のサブクラスです。UIWindow ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIWindow は Object のサブクラスです。UIWindow ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIListItem_ConnectOnSubmit
Submit イベントの通知を受け取るコールバックを登録します。
%group
LNUIListItem
%prm
uilistitem, handler, outReturn
[in]  uilistitem : UIListItem のハンドル
[in]  handler    : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIListItem は UIControl のサブクラスです。UIListItem ハンドルは UIControl ハンドルとして扱うことができ、 LNUIControl_ から始まる命令等で使用できます。
UIListItem は UIElement のサブクラスです。UIListItem ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIListItem は UILayoutElement のサブクラスです。UIListItem ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIListItem は Object のサブクラスです。UIListItem ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIListItemsControl_SetItemsLayoutPanel
setItemsLayoutPanel
%group
LNUIListItemsControl
%prm
uilistitemscontrol, layout
[in] uilistitemscontrol : UIListItemsControl のハンドル
[in] layout             : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIListItemsControl は UIControl のサブクラスです。UIListItemsControl ハンドルは UIControl ハンドルとして扱うことができ、 LNUIControl_ から始まる命令等で使用できます。
UIListItemsControl は UIElement のサブクラスです。UIListItemsControl ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIListItemsControl は UILayoutElement のサブクラスです。UIListItemsControl ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIListItemsControl は Object のサブクラスです。UIListItemsControl ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIListItemsControl_GetItemsLayoutPanel
itemsLayoutPanel
%group
LNUIListItemsControl
%prm
uilistitemscontrol, outReturn
[in]  uilistitemscontrol : UIListItemsControl のハンドル
[out] outReturn(0)       : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIListItemsControl は UIControl のサブクラスです。UIListItemsControl ハンドルは UIControl ハンドルとして扱うことができ、 LNUIControl_ から始まる命令等で使用できます。
UIListItemsControl は UIElement のサブクラスです。UIListItemsControl ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIListItemsControl は UILayoutElement のサブクラスです。UIListItemsControl ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIListItemsControl は Object のサブクラスです。UIListItemsControl ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIListItemsControl_SetSubmitMode
UIListSubmitMode (default: Single)
%group
LNUIListItemsControl
%prm
uilistitemscontrol, value
[in] uilistitemscontrol : UIListItemsControl のハンドル
[in] value              : 
                             LN_UILIST_SUBMIT_MODE_SINGLE
                             ゲームUI用。Hover で選択状態、シングルクリックで Submit. Hover 状態は使用されない。
                             LN_UILIST_SUBMIT_MODE_DOUBLE
                             エディタUI用。シングルクリックで選択状態、ダブルクリックで Submit.

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIListItemsControl は UIControl のサブクラスです。UIListItemsControl ハンドルは UIControl ハンドルとして扱うことができ、 LNUIControl_ から始まる命令等で使用できます。
UIListItemsControl は UIElement のサブクラスです。UIListItemsControl ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIListItemsControl は UILayoutElement のサブクラスです。UIListItemsControl ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIListItemsControl は Object のサブクラスです。UIListItemsControl ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIListItemsControl_GetSubmitMode
UIListSubmitMode
%group
LNUIListItemsControl
%prm
uilistitemscontrol, outReturn
[in]  uilistitemscontrol : UIListItemsControl のハンドル
[out] outReturn(0)       : 結果を格納する変数。
                              LN_UILIST_SUBMIT_MODE_SINGLE
                              ゲームUI用。Hover で選択状態、シングルクリックで Submit. Hover 状態は使用されない。
                              LN_UILIST_SUBMIT_MODE_DOUBLE
                              エディタUI用。シングルクリックで選択状態、ダブルクリックで Submit.

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIListItemsControl は UIControl のサブクラスです。UIListItemsControl ハンドルは UIControl ハンドルとして扱うことができ、 LNUIControl_ から始まる命令等で使用できます。
UIListItemsControl は UIElement のサブクラスです。UIListItemsControl ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIListItemsControl は UILayoutElement のサブクラスです。UIListItemsControl ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIListItemsControl は Object のサブクラスです。UIListItemsControl ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIListItemsControl_ConnectOnSubmit
Submit イベントの通知を受け取るコールバックを登録します。
%group
LNUIListItemsControl
%prm
uilistitemscontrol, handler, outReturn
[in]  uilistitemscontrol : UIListItemsControl のハンドル
[in]  handler            : 
[out] outReturn(0)       : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIListItemsControl は UIControl のサブクラスです。UIListItemsControl ハンドルは UIControl ハンドルとして扱うことができ、 LNUIControl_ から始まる命令等で使用できます。
UIListItemsControl は UIElement のサブクラスです。UIListItemsControl ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIListItemsControl は UILayoutElement のサブクラスです。UIListItemsControl ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIListItemsControl は Object のサブクラスです。UIListItemsControl ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIListBoxItem_Create
init
%group
LNUIListBoxItem
%prm
content, outUIListBoxItem
[in]  content          : 
[out] outUIListBoxItem(0) : 作成された UIListBoxItem のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIListBoxItem は UIListItem のサブクラスです。UIListBoxItem ハンドルは UIListItem ハンドルとして扱うことができ、 LNUIListItem_ から始まる命令等で使用できます。
UIListBoxItem は UIControl のサブクラスです。UIListBoxItem ハンドルは UIControl ハンドルとして扱うことができ、 LNUIControl_ から始まる命令等で使用できます。
UIListBoxItem は UIElement のサブクラスです。UIListBoxItem ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIListBoxItem は UILayoutElement のサブクラスです。UIListBoxItem ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIListBoxItem は Object のサブクラスです。UIListBoxItem ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIListBox_Create
init
%group
LNUIListBox
%prm
outUIListBox
[out] outUIListBox(0) : 作成された UIListBox のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIListBox は UIListItemsControl のサブクラスです。UIListBox ハンドルは UIListItemsControl ハンドルとして扱うことができ、 LNUIListItemsControl_ から始まる命令等で使用できます。
UIListBox は UIControl のサブクラスです。UIListBox ハンドルは UIControl ハンドルとして扱うことができ、 LNUIControl_ から始まる命令等で使用できます。
UIListBox は UIElement のサブクラスです。UIListBox ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIListBox は UILayoutElement のサブクラスです。UIListBox ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIListBox は Object のサブクラスです。UIListBox ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIListBox_AddItem
UIListBoxItem を追加し、そのインスタンスを返します。
%group
LNUIListBox
%prm
uilistbox, content, outReturn
[in]  uilistbox : UIListBox のハンドル
[in]  content   : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
UIListBox は UIListItemsControl のサブクラスです。UIListBox ハンドルは UIListItemsControl ハンドルとして扱うことができ、 LNUIListItemsControl_ から始まる命令等で使用できます。
UIListBox は UIControl のサブクラスです。UIListBox ハンドルは UIControl ハンドルとして扱うことができ、 LNUIControl_ から始まる命令等で使用できます。
UIListBox は UIElement のサブクラスです。UIListBox ハンドルは UIElement ハンドルとして扱うことができ、 LNUIElement_ から始まる命令等で使用できます。
UIListBox は UILayoutElement のサブクラスです。UIListBox ハンドルは UILayoutElement ハンドルとして扱うことができ、 LNUILayoutElement_ から始まる命令等で使用できます。
UIListBox は Object のサブクラスです。UIListBox ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNKeyGesture_Create
init
%group
LNKeyGesture
%prm
key, outKeyGesture
[in]  key           : 
                         LN_KEYS_UNKNOWN
                         
                         LN_KEYS_A
                         
                         LN_KEYS_B
                         
                         LN_KEYS_C
                         
                         LN_KEYS_D
                         
                         LN_KEYS_E
                         
                         LN_KEYS_F
                         
                         LN_KEYS_G
                         
                         LN_KEYS_H
                         
                         LN_KEYS_I
                         
                         LN_KEYS_J
                         
                         LN_KEYS_K
                         
                         LN_KEYS_L
                         
                         LN_KEYS_M
                         
                         LN_KEYS_N
                         
                         LN_KEYS_O
                         
                         LN_KEYS_P
                         
                         LN_KEYS_Q
                         
                         LN_KEYS_R
                         
                         LN_KEYS_S
                         
                         LN_KEYS_T
                         
                         LN_KEYS_U
                         
                         LN_KEYS_V
                         
                         LN_KEYS_W
                         
                         LN_KEYS_X
                         
                         LN_KEYS_Y
                         
                         LN_KEYS_Z
                         
                         LN_KEYS_D0
                         
                         LN_KEYS_D1
                         
                         LN_KEYS_D2
                         
                         LN_KEYS_D3
                         
                         LN_KEYS_D4
                         
                         LN_KEYS_D5
                         
                         LN_KEYS_D6
                         
                         LN_KEYS_D7
                         
                         LN_KEYS_D8
                         
                         LN_KEYS_D9
                         
                         LN_KEYS_F1
                         
                         LN_KEYS_F2
                         
                         LN_KEYS_F3
                         
                         LN_KEYS_F4
                         
                         LN_KEYS_F5
                         
                         LN_KEYS_F6
                         
                         LN_KEYS_F7
                         
                         LN_KEYS_F8
                         
                         LN_KEYS_F9
                         
                         LN_KEYS_F10
                         
                         LN_KEYS_F11
                         
                         LN_KEYS_F12
                         
                         LN_KEYS_SPACE
                         
                         LN_KEYS_ESCAPE
                         
                         LN_KEYS_UP
                         
                         LN_KEYS_DOWN
                         
                         LN_KEYS_LEFT
                         
                         LN_KEYS_RIGHT
                         
                         LN_KEYS_LSHIFT
                         
                         LN_KEYS_RSHIFT
                         
                         LN_KEYS_LCTRL
                         
                         LN_KEYS_RCTRL
                         
                         LN_KEYS_LALT
                         
                         LN_KEYS_RALT
                         
                         LN_KEYS_TAB
                         
                         LN_KEYS_ENTER
                         
                         LN_KEYS_BACK_SPACE
                         
                         LN_KEYS_INSERT
                         
                         LN_KEYS_DELETE
                         
                         LN_KEYS_PAGE_UP
                         
                         LN_KEYS_PAGE_DOWN
                         
                         LN_KEYS_HOME
                         
                         LN_KEYS_END
                         
                         LN_KEYS_COLON
                         [非推奨 GLFWでは;]
                         LN_KEYS_SEMICOLON
                         ;	[非推奨 GLFWでは=]
                         LN_KEYS_COMMA
                         ,
                         LN_KEYS_PERIOD
                         .
                         LN_KEYS_SLASH
                         /
                         LN_KEYS_MINUS
                         -
                         LN_KEYS_BACKSLASH
                         \(バックスラッシュ) [非推奨 GLFWではNOWORD2]
                         LN_KEYS_YEN
                         \(BaskSpaceキーの左の\キー) [非推奨 GLFWではI]
                         LN_KEYS_CARET
                         ^ [非推奨 GLFWではGLFW_KEY_APOSTROPHE(')]
                         LN_KEYS_LBRACKET
                         [
                         LN_KEYS_RBRACKET
                         ]
                         LN_KEYS_ANDROID_PLUS
                         Android '+'
                         LN_KEYS_TERMINATOR
                         
[out] outKeyGesture(0) : 作成された KeyGesture のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
KeyGesture は InputGesture のサブクラスです。KeyGesture ハンドルは InputGesture ハンドルとして扱うことができ、 LNInputGesture_ から始まる命令等で使用できます。
KeyGesture は Object のサブクラスです。KeyGesture ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInput_IsPressed
ボタンが現在押されているかを判定します。
%group
LNInput
%prm
buttonName, outReturn
[in]  buttonName : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInput_IsTriggered
ボタンが新しく押された瞬間を判定します。
%group
LNInput
%prm
buttonName, outReturn
[in]  buttonName : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInput_IsTriggeredOff
ボタンが離された瞬間を判定します。
%group
LNInput
%prm
buttonName, outReturn
[in]  buttonName : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInput_IsRepeated
ボタンが新しく押された瞬間とリピート状態を判定します。
%group
LNInput
%prm
buttonName, outReturn
[in]  buttonName : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInput_GetAxisValue
指定した軸のアナログ値を取得します。
%group
LNInput
%prm
buttonName, outReturn
[in]  buttonName : 
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInput_AddBinding
ボタンのアクションマッピングを追加します。
%group
LNInput
%prm
buttonName, gesture
[in] buttonName : 
[in] gesture    : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInput_RemoveBinding
アクションマッピングを除外します。
%group
LNInput
%prm
gesture
[in] gesture : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInput_ClearBindings
指定したボタンのアクションマッピングをすべて削除します。
%group
LNInput
%prm
buttonName
[in] buttonName : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInput_ClearAllBindings
全てのアクションマッピングを削除します。
%group
LNInput
%prm


stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMouse_Pressed
ボタンが現在押されているかを判定します。
%group
LNMouse
%prm
button, outReturn
[in]  button    : 
                     LN_MOUSE_BUTTONS_NONE
                     無効または押されていないことを示す
                     LN_MOUSE_BUTTONS_LEFT
                     左ボタン
                     LN_MOUSE_BUTTONS_RIGHT
                     右ボタン
                     LN_MOUSE_BUTTONS_MIDDLE
                     中央ボタン
                     LN_MOUSE_BUTTONS_X1
                     拡張ボタン1
                     LN_MOUSE_BUTTONS_X2
                     拡張ボタン2
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMouse_Triggered
ボタンが新しく押された瞬間を判定します。
%group
LNMouse
%prm
button, outReturn
[in]  button    : 
                     LN_MOUSE_BUTTONS_NONE
                     無効または押されていないことを示す
                     LN_MOUSE_BUTTONS_LEFT
                     左ボタン
                     LN_MOUSE_BUTTONS_RIGHT
                     右ボタン
                     LN_MOUSE_BUTTONS_MIDDLE
                     中央ボタン
                     LN_MOUSE_BUTTONS_X1
                     拡張ボタン1
                     LN_MOUSE_BUTTONS_X2
                     拡張ボタン2
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMouse_TriggeredOff
ボタンが離された瞬間を判定します。
%group
LNMouse
%prm
button, outReturn
[in]  button    : 
                     LN_MOUSE_BUTTONS_NONE
                     無効または押されていないことを示す
                     LN_MOUSE_BUTTONS_LEFT
                     左ボタン
                     LN_MOUSE_BUTTONS_RIGHT
                     右ボタン
                     LN_MOUSE_BUTTONS_MIDDLE
                     中央ボタン
                     LN_MOUSE_BUTTONS_X1
                     拡張ボタン1
                     LN_MOUSE_BUTTONS_X2
                     拡張ボタン2
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMouse_Repeated
ボタンが新しく押された瞬間とリピート状態を判定します。
%group
LNMouse
%prm
button, outReturn
[in]  button    : 
                     LN_MOUSE_BUTTONS_NONE
                     無効または押されていないことを示す
                     LN_MOUSE_BUTTONS_LEFT
                     左ボタン
                     LN_MOUSE_BUTTONS_RIGHT
                     右ボタン
                     LN_MOUSE_BUTTONS_MIDDLE
                     中央ボタン
                     LN_MOUSE_BUTTONS_X1
                     拡張ボタン1
                     LN_MOUSE_BUTTONS_X2
                     拡張ボタン2
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMouse_GetPosition
マウスポインタの位置を取得します。
%group
LNMouse
%prm
outReturn
[out] outReturn(0) : (Point 型の値) 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommand_Code
コマンドの実行コードを取得します。
%group
LNInterpreterCommand
%prm
interpretercommand, outReturn
[in]  interpretercommand : InterpreterCommand のハンドル
[out] outReturn(0)       : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
InterpreterCommand は Object のサブクラスです。InterpreterCommand ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommand_ParamsCount
コマンドのパラメータ数を取得します。
%group
LNInterpreterCommand
%prm
interpretercommand, outReturn
[in]  interpretercommand : InterpreterCommand のハンドル
[out] outReturn(0)       : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
InterpreterCommand は Object のサブクラスです。InterpreterCommand ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommand_Param
コマンドのパラメータを取得します。
%group
LNInterpreterCommand
%prm
interpretercommand, index, outReturn
[in]  interpretercommand : InterpreterCommand のハンドル
[in]  index              : 
[out] outReturn(0)       : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
InterpreterCommand は Object のサブクラスです。InterpreterCommand ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommandList_Create
InterpreterCommandList を作成します。
%group
LNInterpreterCommandList
%prm
outInterpreterCommandList
[out] outInterpreterCommandList(0) : 作成された InterpreterCommandList のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
InterpreterCommandList は Object のサブクラスです。InterpreterCommandList ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommandList_AddCommand
コマンドを追加します。
%group
LNInterpreterCommandList
%prm
interpretercommandlist, code
[in] interpretercommandlist : InterpreterCommandList のハンドル
[in] code                   : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
InterpreterCommandList は Object のサブクラスです。InterpreterCommandList ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommandList_AddCommand1
コマンドと 1 つのパラメータを追加します。
%group
LNInterpreterCommandList
%prm
interpretercommandlist, code, param0
[in] interpretercommandlist : InterpreterCommandList のハンドル
[in] code                   : 
[in] param0                 : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
InterpreterCommandList は Object のサブクラスです。InterpreterCommandList ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommandList_AddCommand2
コマンドと 2 つのパラメータを追加します。
%group
LNInterpreterCommandList
%prm
interpretercommandlist, code, param0, param1
[in] interpretercommandlist : InterpreterCommandList のハンドル
[in] code                   : 
[in] param0                 : 
[in] param1                 : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
InterpreterCommandList は Object のサブクラスです。InterpreterCommandList ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommandList_AddCommand3
コマンドと 3 つのパラメータを追加します。
%group
LNInterpreterCommandList
%prm
interpretercommandlist, code, param0, param1, param2
[in] interpretercommandlist : InterpreterCommandList のハンドル
[in] code                   : 
[in] param0                 : 
[in] param1                 : 
[in] param2                 : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
InterpreterCommandList は Object のサブクラスです。InterpreterCommandList ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommandList_AddCommand4
コマンドと 4 つのパラメータを追加します。
%group
LNInterpreterCommandList
%prm
interpretercommandlist, code, param0, param1, param2, param3
[in] interpretercommandlist : InterpreterCommandList のハンドル
[in] code                   : 
[in] param0                 : 
[in] param1                 : 
[in] param2                 : 
[in] param3                 : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
InterpreterCommandList は Object のサブクラスです。InterpreterCommandList ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommandDelegate_Create

%group
LNInterpreterCommandDelegate
%prm
callback, outInterpreterCommandDelegate
[in]  callback                      : 
[out] outInterpreterCommandDelegate(0) : 作成された InterpreterCommandDelegate のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
InterpreterCommandDelegate は Object のサブクラスです。InterpreterCommandDelegate ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreter_Create
Interpreter を作成します。
%group
LNInterpreter
%prm
outInterpreter
[out] outInterpreter(0) : 作成された Interpreter のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Interpreter は Object のサブクラスです。Interpreter ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreter_Clear
コマンドリストをクリアします。実行中のコマンドリストは強制的に破棄されます。
%group
LNInterpreter
%prm
interpreter
[in] interpreter : Interpreter のハンドル

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Interpreter は Object のサブクラスです。Interpreter ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreter_Run
コマンドリストを設定し、実行を開始します。
%group
LNInterpreter
%prm
interpreter, commandList
[in] interpreter : Interpreter のハンドル
[in] commandList : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Interpreter は Object のサブクラスです。Interpreter ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreter_IsRunning
コマンドリストの実行中であるかを確認します。
%group
LNInterpreter
%prm
interpreter, outReturn
[in]  interpreter : Interpreter のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Interpreter は Object のサブクラスです。Interpreter ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreter_Update

%group
LNInterpreter
%prm
interpreter
[in] interpreter : Interpreter のハンドル

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Interpreter は Object のサブクラスです。Interpreter ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreter_Terminate

%group
LNInterpreter
%prm
interpreter
[in] interpreter : Interpreter のハンドル

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Interpreter は Object のサブクラスです。Interpreter ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreter_RegisterCommandHandler

%group
LNInterpreter
%prm
interpreter, name, handler
[in] interpreter : Interpreter のハンドル
[in] name        : 
[in] handler     : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Interpreter は Object のサブクラスです。Interpreter ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreter_SetWaitMode
setWaitMode
%group
LNInterpreter
%prm
interpreter, mode
[in] interpreter : Interpreter のハンドル
[in] mode        : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Interpreter は Object のサブクラスです。Interpreter ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreter_GetWaitMode
waitMode
%group
LNInterpreter
%prm
interpreter, outReturn
[in]  interpreter : Interpreter のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Interpreter は Object のサブクラスです。Interpreter ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreter_SetWaitCount
setWaitCount
%group
LNInterpreter
%prm
interpreter, count
[in] interpreter : Interpreter のハンドル
[in] count       : 

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Interpreter は Object のサブクラスです。Interpreter ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreter_GetWaitCount
waitCount
%group
LNInterpreter
%prm
interpreter, outReturn
[in]  interpreter : Interpreter のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Interpreter は Object のサブクラスです。Interpreter ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetMainWindowTitle
メインウィンドウのタイトル文字列を設定します。
%group
LNEngineSettings
%prm
title
[in] title : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetMainWindowSize
メインウィンドウのクライアント領域の幅と高さを設定します。(default: 640x480)
%group
LNEngineSettings
%prm
width, height
[in] width  : 
[in] height : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetMainWorldViewSize
メインウィンドウに対して作成される WorldView のサイズを設定します。(default: クライアント領域のサイズと同等)
%group
LNEngineSettings
%prm
width, height
[in] width  : 
[in] height : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetMainWindowResizable
メインウィンドウのサイズをユーザーが変更できるかどうかを指定します。(default: false)
%group
LNEngineSettings
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_AddAssetDirectory
アセットが保存されているディレクトリを登録します。
%group
LNEngineSettings
%prm
path
[in] path : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_AddAssetArchive
アセットファイルを登録します。
%group
LNEngineSettings
%prm
fileFullPath, password
[in] fileFullPath : 
[in] password     : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetFrameRate
フレームレートを設定します。(default: 60)
%group
LNEngineSettings
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetUITheme
デフォルトの UI テーマ名を設定します。
%group
LNEngineSettings
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetFontFile
デフォルトのフォントファイルを設定します。
%group
LNEngineSettings
%prm
filePath
[in] filePath : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetDebugToolEnabled
(default: Debug ビルドの場合true、それ以外は false)
%group
LNEngineSettings
%prm
enabled
[in] enabled : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetDebugMode
デバッグモードの有無を設定します。(default: Debug ビルドの場合true、それ以外は false)
%group
LNEngineSettings
%prm
enabled
[in] enabled : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetEngineLogFilePath
デバッグ用のログファイルの出力先ファイルパスを設定します。(default: Empty(実行ファイルのディレクトリへ出力))
%group
LNEngineSettings
%prm
filePath
[in] filePath : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetPriorityGPUName
優先的に使用する GPU の名前を指定します。
%group
LNEngineSettings
%prm
filePath
[in] filePath : 

stat : 0=成功, 負値=失敗

%inst
現在は DirectX12 を使用する場合のみ有効で、デフォルトの GPU では動作が不安定な場合に "Microsoft Basic Render Driver" 等を試すことができます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetDeveloperToolEnabled
setDeveloperToolEnabled
%group
LNEngineSettings
%prm
enabled
[in] enabled : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetUserMainWindow
ユーザー指定のメインウィンドウのウィンドウハンドルを設定します。
%group
LNEngineSettings
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_Initialize
エンジンの初期化処理を行います。
%group
LNEngine
%prm


stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_Terminate
エンジンの終了処理を行います。
%group
LNEngine
%prm


stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_Update

%group
LNEngine
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_GetTime
アプリケーション開始からの経過時間を取得します。この値はタイムスケールの影響を受けます。
%group
LNEngine
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_GetWorld
デフォルトで作成されるメインの World です。
%group
LNEngine
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_GetMainCamera
デフォルトで作成されるメインの Camera です。
%group
LNEngine
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_GetMainLight
デフォルトで作成されるメインの Light です。
%group
LNEngine
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_GetRenderView
デフォルトで作成されるメインの RenderView です。
%group
LNEngine
%prm
outReturn
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNApplication_Create

%group
LNApplication
%prm
outApplication
[out] outApplication(0) : 作成された Application のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Application は Object のサブクラスです。Application ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNApplication_OnInit
エンジンの初期化処理が完了した後に呼び出されます。
%group
LNApplication
%prm
application
[in] application : Application のハンドル

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Application は Object のサブクラスです。Application ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNApplication_OnUpdate
毎フレーム呼び出されます。
%group
LNApplication
%prm
application
[in] application : Application のハンドル

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Application は Object のサブクラスです。Application ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNApplication_World
デフォルトで作成されるメインの World を取得します。
%group
LNApplication
%prm
application, outReturn
[in]  application : Application のハンドル
[out] outReturn(0) : 結果を格納する変数。

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Application は Object のサブクラスです。Application ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNApplication_Run
Application の実行を開始します。
%group
LNApplication
%prm
application
[in] application : Application のハンドル

stat : 0=成功, 負値=失敗

%inst


備考
--------------------
Application は Object のサブクラスです。Application ハンドルは Object ハンドルとして扱うことができ、 LNObject_ から始まる命令等で使用できます。

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDebug_SetGuideGridEnabled
3D シーン上に、グリッドを表示するかどうかを設定します。
%group
LNDebug
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDebug_SetPhysicsDebugDrawEnabled
3D シーン上に、コリジョン及び物理演算の情報を表示するかどうかを設定します。
%group
LNDebug
%prm
value
[in] value : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDebug_Print
ウィンドウ上にデバッグ文字列を表示します。
%group
LNDebug
%prm
str
[in] str : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDebug_PrintWithTime
表示時間を指定して、ウィンドウ上にデバッグ文字列を表示します。
%group
LNDebug
%prm
time, str
[in] time : 
[in] str  : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDebug_PrintWithTimeAndColor
表示時間と文字色を指定して、ウィンドウ上にデバッグ文字列を表示します。
%group
LNDebug
%prm
time, color, str
[in] time  : 
[in] color : (Color 型の値) 
[in] str   : 

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNObjectSerializeHandler_Create

%group
LNObjectSerializeHandler
%prm
callback, outObjectSerializeHandler
[in]  callback                  : 
[out] outObjectSerializeHandler(0) : 作成された ObjectSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEventConnectionSerializeHandler_Create

%group
LNEventConnectionSerializeHandler
%prm
callback, outEventConnectionSerializeHandler
[in]  callback                           : 
[out] outEventConnectionSerializeHandler(0) : 作成された EventConnectionSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVariantSerializeHandler_Create

%group
LNVariantSerializeHandler
%prm
callback, outVariantSerializeHandler
[in]  callback                   : 
[out] outVariantSerializeHandler(0) : 作成された VariantSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestClass1SerializeHandler_Create

%group
LNZVTestClass1SerializeHandler
%prm
callback, outZVTestClass1SerializeHandler
[in]  callback                        : 
[out] outZVTestClass1SerializeHandler(0) : 作成された ZVTestClass1SerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNZVTestEventArgs1SerializeHandler_Create

%group
LNZVTestEventArgs1SerializeHandler
%prm
callback, outZVTestEventArgs1SerializeHandler
[in]  callback                            : 
[out] outZVTestEventArgs1SerializeHandler(0) : 作成された ZVTestEventArgs1SerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSerializer2SerializeHandler_Create

%group
LNSerializer2SerializeHandler
%prm
callback, outSerializer2SerializeHandler
[in]  callback                       : 
[out] outSerializer2SerializeHandler(0) : 作成された Serializer2SerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAssetObjectSerializeHandler_Create

%group
LNAssetObjectSerializeHandler
%prm
callback, outAssetObjectSerializeHandler
[in]  callback                       : 
[out] outAssetObjectSerializeHandler(0) : 作成された AssetObjectSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAssetImportSettingsSerializeHandler_Create

%group
LNAssetImportSettingsSerializeHandler
%prm
callback, outAssetImportSettingsSerializeHandler
[in]  callback                               : 
[out] outAssetImportSettingsSerializeHandler(0) : 作成された AssetImportSettingsSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAssetModelSerializeHandler_Create

%group
LNAssetModelSerializeHandler
%prm
callback, outAssetModelSerializeHandler
[in]  callback                      : 
[out] outAssetModelSerializeHandler(0) : 作成された AssetModelSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSoundSerializeHandler_Create

%group
LNSoundSerializeHandler
%prm
callback, outSoundSerializeHandler
[in]  callback                 : 
[out] outSoundSerializeHandler(0) : 作成された SoundSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNTextureSerializeHandler_Create

%group
LNTextureSerializeHandler
%prm
callback, outTextureSerializeHandler
[in]  callback                   : 
[out] outTextureSerializeHandler(0) : 作成された TextureSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNTexture2DSerializeHandler_Create

%group
LNTexture2DSerializeHandler
%prm
callback, outTexture2DSerializeHandler
[in]  callback                     : 
[out] outTexture2DSerializeHandler(0) : 作成された Texture2DSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNShaderSerializeHandler_Create

%group
LNShaderSerializeHandler
%prm
callback, outShaderSerializeHandler
[in]  callback                  : 
[out] outShaderSerializeHandler(0) : 作成された ShaderSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRenderViewSerializeHandler_Create

%group
LNRenderViewSerializeHandler
%prm
callback, outRenderViewSerializeHandler
[in]  callback                      : 
[out] outRenderViewSerializeHandler(0) : 作成された RenderViewSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMaterialSerializeHandler_Create

%group
LNMaterialSerializeHandler
%prm
callback, outMaterialSerializeHandler
[in]  callback                    : 
[out] outMaterialSerializeHandler(0) : 作成された MaterialSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshNodeSerializeHandler_Create

%group
LNMeshNodeSerializeHandler
%prm
callback, outMeshNodeSerializeHandler
[in]  callback                    : 
[out] outMeshNodeSerializeHandler(0) : 作成された MeshNodeSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAnimationControllerSerializeHandler_Create

%group
LNAnimationControllerSerializeHandler
%prm
callback, outAnimationControllerSerializeHandler
[in]  callback                               : 
[out] outAnimationControllerSerializeHandler(0) : 作成された AnimationControllerSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshModelSerializeHandler_Create

%group
LNMeshModelSerializeHandler
%prm
callback, outMeshModelSerializeHandler
[in]  callback                     : 
[out] outMeshModelSerializeHandler(0) : 作成された MeshModelSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshImportSettingsSerializeHandler_Create

%group
LNMeshImportSettingsSerializeHandler
%prm
callback, outMeshImportSettingsSerializeHandler
[in]  callback                              : 
[out] outMeshImportSettingsSerializeHandler(0) : 作成された MeshImportSettingsSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSkinnedMeshModelSerializeHandler_Create

%group
LNSkinnedMeshModelSerializeHandler
%prm
callback, outSkinnedMeshModelSerializeHandler
[in]  callback                            : 
[out] outSkinnedMeshModelSerializeHandler(0) : 作成された SkinnedMeshModelSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCollisionShapeSerializeHandler_Create

%group
LNCollisionShapeSerializeHandler
%prm
callback, outCollisionShapeSerializeHandler
[in]  callback                          : 
[out] outCollisionShapeSerializeHandler(0) : 作成された CollisionShapeSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNBoxCollisionShapeSerializeHandler_Create

%group
LNBoxCollisionShapeSerializeHandler
%prm
callback, outBoxCollisionShapeSerializeHandler
[in]  callback                             : 
[out] outBoxCollisionShapeSerializeHandler(0) : 作成された BoxCollisionShapeSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAnimationCurveSerializeHandler_Create

%group
LNAnimationCurveSerializeHandler
%prm
callback, outAnimationCurveSerializeHandler
[in]  callback                          : 
[out] outAnimationCurveSerializeHandler(0) : 作成された AnimationCurveSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNKeyFrameAnimationCurveSerializeHandler_Create

%group
LNKeyFrameAnimationCurveSerializeHandler
%prm
callback, outKeyFrameAnimationCurveSerializeHandler
[in]  callback                                  : 
[out] outKeyFrameAnimationCurveSerializeHandler(0) : 作成された KeyFrameAnimationCurveSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAnimationClipSerializeHandler_Create

%group
LNAnimationClipSerializeHandler
%prm
callback, outAnimationClipSerializeHandler
[in]  callback                         : 
[out] outAnimationClipSerializeHandler(0) : 作成された AnimationClipSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAnimationStateSerializeHandler_Create

%group
LNAnimationStateSerializeHandler
%prm
callback, outAnimationStateSerializeHandler
[in]  callback                          : 
[out] outAnimationStateSerializeHandler(0) : 作成された AnimationStateSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEffectResourceSerializeHandler_Create

%group
LNEffectResourceSerializeHandler
%prm
callback, outEffectResourceSerializeHandler
[in]  callback                          : 
[out] outEffectResourceSerializeHandler(0) : 作成された EffectResourceSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterModelSerializeHandler_Create

%group
LNParticleEmitterModelSerializeHandler
%prm
callback, outParticleEmitterModelSerializeHandler
[in]  callback                                : 
[out] outParticleEmitterModelSerializeHandler(0) : 作成された ParticleEmitterModelSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleModelSerializeHandler_Create

%group
LNParticleModelSerializeHandler
%prm
callback, outParticleModelSerializeHandler
[in]  callback                         : 
[out] outParticleModelSerializeHandler(0) : 作成された ParticleModelSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNComponentSerializeHandler_Create

%group
LNComponentSerializeHandler
%prm
callback, outComponentSerializeHandler
[in]  callback                     : 
[out] outComponentSerializeHandler(0) : 作成された ComponentSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVisualComponentSerializeHandler_Create

%group
LNVisualComponentSerializeHandler
%prm
callback, outVisualComponentSerializeHandler
[in]  callback                           : 
[out] outVisualComponentSerializeHandler(0) : 作成された VisualComponentSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpriteComponentSerializeHandler_Create

%group
LNSpriteComponentSerializeHandler
%prm
callback, outSpriteComponentSerializeHandler
[in]  callback                           : 
[out] outSpriteComponentSerializeHandler(0) : 作成された SpriteComponentSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterControllerSerializeHandler_Create

%group
LNCharacterControllerSerializeHandler
%prm
callback, outCharacterControllerSerializeHandler
[in]  callback                               : 
[out] outCharacterControllerSerializeHandler(0) : 作成された CharacterControllerSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldSerializeHandler_Create

%group
LNWorldSerializeHandler
%prm
callback, outWorldSerializeHandler
[in]  callback                 : 
[out] outWorldSerializeHandler(0) : 作成された WorldSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNComponentListSerializeHandler_Create

%group
LNComponentListSerializeHandler
%prm
callback, outComponentListSerializeHandler
[in]  callback                         : 
[out] outComponentListSerializeHandler(0) : 作成された ComponentListSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObjectSerializeHandler_Create

%group
LNWorldObjectSerializeHandler
%prm
callback, outWorldObjectSerializeHandler
[in]  callback                       : 
[out] outWorldObjectSerializeHandler(0) : 作成された WorldObjectSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObjectPreUpdateHandler_Create

%group
LNWorldObjectPreUpdateHandler
%prm
callback, outWorldObjectPreUpdateHandler
[in]  callback                       : 
[out] outWorldObjectPreUpdateHandler(0) : 作成された WorldObjectPreUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObjectUpdateHandler_Create

%group
LNWorldObjectUpdateHandler
%prm
callback, outWorldObjectUpdateHandler
[in]  callback                    : 
[out] outWorldObjectUpdateHandler(0) : 作成された WorldObjectUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVisualObjectSerializeHandler_Create

%group
LNVisualObjectSerializeHandler
%prm
callback, outVisualObjectSerializeHandler
[in]  callback                        : 
[out] outVisualObjectSerializeHandler(0) : 作成された VisualObjectSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVisualObjectPreUpdateHandler_Create

%group
LNVisualObjectPreUpdateHandler
%prm
callback, outVisualObjectPreUpdateHandler
[in]  callback                        : 
[out] outVisualObjectPreUpdateHandler(0) : 作成された VisualObjectPreUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVisualObjectUpdateHandler_Create

%group
LNVisualObjectUpdateHandler
%prm
callback, outVisualObjectUpdateHandler
[in]  callback                     : 
[out] outVisualObjectUpdateHandler(0) : 作成された VisualObjectUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCameraSerializeHandler_Create

%group
LNCameraSerializeHandler
%prm
callback, outCameraSerializeHandler
[in]  callback                  : 
[out] outCameraSerializeHandler(0) : 作成された CameraSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCameraPreUpdateHandler_Create

%group
LNCameraPreUpdateHandler
%prm
callback, outCameraPreUpdateHandler
[in]  callback                  : 
[out] outCameraPreUpdateHandler(0) : 作成された CameraPreUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCameraUpdateHandler_Create

%group
LNCameraUpdateHandler
%prm
callback, outCameraUpdateHandler
[in]  callback               : 
[out] outCameraUpdateHandler(0) : 作成された CameraUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLightSerializeHandler_Create

%group
LNEnvironmentLightSerializeHandler
%prm
callback, outEnvironmentLightSerializeHandler
[in]  callback                            : 
[out] outEnvironmentLightSerializeHandler(0) : 作成された EnvironmentLightSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLightPreUpdateHandler_Create

%group
LNEnvironmentLightPreUpdateHandler
%prm
callback, outEnvironmentLightPreUpdateHandler
[in]  callback                            : 
[out] outEnvironmentLightPreUpdateHandler(0) : 作成された EnvironmentLightPreUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLightUpdateHandler_Create

%group
LNEnvironmentLightUpdateHandler
%prm
callback, outEnvironmentLightUpdateHandler
[in]  callback                         : 
[out] outEnvironmentLightUpdateHandler(0) : 作成された EnvironmentLightUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLightSerializeHandler_Create

%group
LNDirectionalLightSerializeHandler
%prm
callback, outDirectionalLightSerializeHandler
[in]  callback                            : 
[out] outDirectionalLightSerializeHandler(0) : 作成された DirectionalLightSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLightPreUpdateHandler_Create

%group
LNDirectionalLightPreUpdateHandler
%prm
callback, outDirectionalLightPreUpdateHandler
[in]  callback                            : 
[out] outDirectionalLightPreUpdateHandler(0) : 作成された DirectionalLightPreUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLightUpdateHandler_Create

%group
LNDirectionalLightUpdateHandler
%prm
callback, outDirectionalLightUpdateHandler
[in]  callback                         : 
[out] outDirectionalLightUpdateHandler(0) : 作成された DirectionalLightUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLightSerializeHandler_Create

%group
LNPointLightSerializeHandler
%prm
callback, outPointLightSerializeHandler
[in]  callback                      : 
[out] outPointLightSerializeHandler(0) : 作成された PointLightSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLightPreUpdateHandler_Create

%group
LNPointLightPreUpdateHandler
%prm
callback, outPointLightPreUpdateHandler
[in]  callback                      : 
[out] outPointLightPreUpdateHandler(0) : 作成された PointLightPreUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLightUpdateHandler_Create

%group
LNPointLightUpdateHandler
%prm
callback, outPointLightUpdateHandler
[in]  callback                   : 
[out] outPointLightUpdateHandler(0) : 作成された PointLightUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLightSerializeHandler_Create

%group
LNSpotLightSerializeHandler
%prm
callback, outSpotLightSerializeHandler
[in]  callback                     : 
[out] outSpotLightSerializeHandler(0) : 作成された SpotLightSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLightPreUpdateHandler_Create

%group
LNSpotLightPreUpdateHandler
%prm
callback, outSpotLightPreUpdateHandler
[in]  callback                     : 
[out] outSpotLightPreUpdateHandler(0) : 作成された SpotLightPreUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLightUpdateHandler_Create

%group
LNSpotLightUpdateHandler
%prm
callback, outSpotLightUpdateHandler
[in]  callback                  : 
[out] outSpotLightUpdateHandler(0) : 作成された SpotLightUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpriteSerializeHandler_Create

%group
LNSpriteSerializeHandler
%prm
callback, outSpriteSerializeHandler
[in]  callback                  : 
[out] outSpriteSerializeHandler(0) : 作成された SpriteSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpritePreUpdateHandler_Create

%group
LNSpritePreUpdateHandler
%prm
callback, outSpritePreUpdateHandler
[in]  callback                  : 
[out] outSpritePreUpdateHandler(0) : 作成された SpritePreUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpriteUpdateHandler_Create

%group
LNSpriteUpdateHandler
%prm
callback, outSpriteUpdateHandler
[in]  callback               : 
[out] outSpriteUpdateHandler(0) : 作成された SpriteUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCameraOrbitControlComponentSerializeHandler_Create

%group
LNCameraOrbitControlComponentSerializeHandler
%prm
callback, outCameraOrbitControlComponentSerializeHandler
[in]  callback                                       : 
[out] outCameraOrbitControlComponentSerializeHandler(0) : 作成された CameraOrbitControlComponentSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRaycasterSerializeHandler_Create

%group
LNRaycasterSerializeHandler
%prm
callback, outRaycasterSerializeHandler
[in]  callback                     : 
[out] outRaycasterSerializeHandler(0) : 作成された RaycasterSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRaycastResultSerializeHandler_Create

%group
LNRaycastResultSerializeHandler
%prm
callback, outRaycastResultSerializeHandler
[in]  callback                         : 
[out] outRaycastResultSerializeHandler(0) : 作成された RaycastResultSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldRenderViewSerializeHandler_Create

%group
LNWorldRenderViewSerializeHandler
%prm
callback, outWorldRenderViewSerializeHandler
[in]  callback                           : 
[out] outWorldRenderViewSerializeHandler(0) : 作成された WorldRenderViewSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNShapeObjectSerializeHandler_Create

%group
LNShapeObjectSerializeHandler
%prm
callback, outShapeObjectSerializeHandler
[in]  callback                       : 
[out] outShapeObjectSerializeHandler(0) : 作成された ShapeObjectSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNShapeObjectPreUpdateHandler_Create

%group
LNShapeObjectPreUpdateHandler
%prm
callback, outShapeObjectPreUpdateHandler
[in]  callback                       : 
[out] outShapeObjectPreUpdateHandler(0) : 作成された ShapeObjectPreUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNShapeObjectUpdateHandler_Create

%group
LNShapeObjectUpdateHandler
%prm
callback, outShapeObjectUpdateHandler
[in]  callback                    : 
[out] outShapeObjectUpdateHandler(0) : 作成された ShapeObjectUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPlaneMeshSerializeHandler_Create

%group
LNPlaneMeshSerializeHandler
%prm
callback, outPlaneMeshSerializeHandler
[in]  callback                     : 
[out] outPlaneMeshSerializeHandler(0) : 作成された PlaneMeshSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPlaneMeshPreUpdateHandler_Create

%group
LNPlaneMeshPreUpdateHandler
%prm
callback, outPlaneMeshPreUpdateHandler
[in]  callback                     : 
[out] outPlaneMeshPreUpdateHandler(0) : 作成された PlaneMeshPreUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPlaneMeshUpdateHandler_Create

%group
LNPlaneMeshUpdateHandler
%prm
callback, outPlaneMeshUpdateHandler
[in]  callback                  : 
[out] outPlaneMeshUpdateHandler(0) : 作成された PlaneMeshUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNBoxMeshSerializeHandler_Create

%group
LNBoxMeshSerializeHandler
%prm
callback, outBoxMeshSerializeHandler
[in]  callback                   : 
[out] outBoxMeshSerializeHandler(0) : 作成された BoxMeshSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNBoxMeshPreUpdateHandler_Create

%group
LNBoxMeshPreUpdateHandler
%prm
callback, outBoxMeshPreUpdateHandler
[in]  callback                   : 
[out] outBoxMeshPreUpdateHandler(0) : 作成された BoxMeshPreUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNBoxMeshUpdateHandler_Create

%group
LNBoxMeshUpdateHandler
%prm
callback, outBoxMeshUpdateHandler
[in]  callback                : 
[out] outBoxMeshUpdateHandler(0) : 作成された BoxMeshUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshComponentSerializeHandler_Create

%group
LNMeshComponentSerializeHandler
%prm
callback, outMeshComponentSerializeHandler
[in]  callback                         : 
[out] outMeshComponentSerializeHandler(0) : 作成された MeshComponentSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCollisionSerializeHandler_Create

%group
LNCollisionSerializeHandler
%prm
callback, outCollisionSerializeHandler
[in]  callback                     : 
[out] outCollisionSerializeHandler(0) : 作成された CollisionSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNTriggerBodyComponentSerializeHandler_Create

%group
LNTriggerBodyComponentSerializeHandler
%prm
callback, outTriggerBodyComponentSerializeHandler
[in]  callback                                : 
[out] outTriggerBodyComponentSerializeHandler(0) : 作成された TriggerBodyComponentSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterSerializeHandler_Create

%group
LNParticleEmitterSerializeHandler
%prm
callback, outParticleEmitterSerializeHandler
[in]  callback                           : 
[out] outParticleEmitterSerializeHandler(0) : 作成された ParticleEmitterSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterPreUpdateHandler_Create

%group
LNParticleEmitterPreUpdateHandler
%prm
callback, outParticleEmitterPreUpdateHandler
[in]  callback                           : 
[out] outParticleEmitterPreUpdateHandler(0) : 作成された ParticleEmitterPreUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterUpdateHandler_Create

%group
LNParticleEmitterUpdateHandler
%prm
callback, outParticleEmitterUpdateHandler
[in]  callback                        : 
[out] outParticleEmitterUpdateHandler(0) : 作成された ParticleEmitterUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLevelSerializeHandler_Create

%group
LNLevelSerializeHandler
%prm
callback, outLevelSerializeHandler
[in]  callback                 : 
[out] outLevelSerializeHandler(0) : 作成された LevelSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLevelStartHandler_Create

%group
LNLevelStartHandler
%prm
callback, outLevelStartHandler
[in]  callback             : 
[out] outLevelStartHandler(0) : 作成された LevelStartHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLevelStopHandler_Create

%group
LNLevelStopHandler
%prm
callback, outLevelStopHandler
[in]  callback            : 
[out] outLevelStopHandler(0) : 作成された LevelStopHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLevelPauseHandler_Create

%group
LNLevelPauseHandler
%prm
callback, outLevelPauseHandler
[in]  callback             : 
[out] outLevelPauseHandler(0) : 作成された LevelPauseHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLevelResumeHandler_Create

%group
LNLevelResumeHandler
%prm
callback, outLevelResumeHandler
[in]  callback              : 
[out] outLevelResumeHandler(0) : 作成された LevelResumeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLevelUpdateHandler_Create

%group
LNLevelUpdateHandler
%prm
callback, outLevelUpdateHandler
[in]  callback              : 
[out] outLevelUpdateHandler(0) : 作成された LevelUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIEventArgsSerializeHandler_Create

%group
LNUIEventArgsSerializeHandler
%prm
callback, outUIEventArgsSerializeHandler
[in]  callback                       : 
[out] outUIEventArgsSerializeHandler(0) : 作成された UIEventArgsSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUILayoutElementSerializeHandler_Create

%group
LNUILayoutElementSerializeHandler
%prm
callback, outUILayoutElementSerializeHandler
[in]  callback                           : 
[out] outUILayoutElementSerializeHandler(0) : 作成された UILayoutElementSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElementSerializeHandler_Create

%group
LNUIElementSerializeHandler
%prm
callback, outUIElementSerializeHandler
[in]  callback                     : 
[out] outUIElementSerializeHandler(0) : 作成された UIElementSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUITextSerializeHandler_Create

%group
LNUITextSerializeHandler
%prm
callback, outUITextSerializeHandler
[in]  callback                  : 
[out] outUITextSerializeHandler(0) : 作成された UITextSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUISpriteSerializeHandler_Create

%group
LNUISpriteSerializeHandler
%prm
callback, outUISpriteSerializeHandler
[in]  callback                    : 
[out] outUISpriteSerializeHandler(0) : 作成された UISpriteSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIIconSerializeHandler_Create

%group
LNUIIconSerializeHandler
%prm
callback, outUIIconSerializeHandler
[in]  callback                  : 
[out] outUIIconSerializeHandler(0) : 作成された UIIconSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIMessageTextAreaSerializeHandler_Create

%group
LNUIMessageTextAreaSerializeHandler
%prm
callback, outUIMessageTextAreaSerializeHandler
[in]  callback                             : 
[out] outUIMessageTextAreaSerializeHandler(0) : 作成された UIMessageTextAreaSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUILayoutPanelSerializeHandler_Create

%group
LNUILayoutPanelSerializeHandler
%prm
callback, outUILayoutPanelSerializeHandler
[in]  callback                         : 
[out] outUILayoutPanelSerializeHandler(0) : 作成された UILayoutPanelSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIBoxLayoutSerializeHandler_Create

%group
LNUIBoxLayoutSerializeHandler
%prm
callback, outUIBoxLayoutSerializeHandler
[in]  callback                       : 
[out] outUIBoxLayoutSerializeHandler(0) : 作成された UIBoxLayoutSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIStackLayoutSerializeHandler_Create

%group
LNUIStackLayoutSerializeHandler
%prm
callback, outUIStackLayoutSerializeHandler
[in]  callback                         : 
[out] outUIStackLayoutSerializeHandler(0) : 作成された UIStackLayoutSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIGridLayoutSerializeHandler_Create

%group
LNUIGridLayoutSerializeHandler
%prm
callback, outUIGridLayoutSerializeHandler
[in]  callback                        : 
[out] outUIGridLayoutSerializeHandler(0) : 作成された UIGridLayoutSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIControlSerializeHandler_Create

%group
LNUIControlSerializeHandler
%prm
callback, outUIControlSerializeHandler
[in]  callback                     : 
[out] outUIControlSerializeHandler(0) : 作成された UIControlSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIButtonBaseSerializeHandler_Create

%group
LNUIButtonBaseSerializeHandler
%prm
callback, outUIButtonBaseSerializeHandler
[in]  callback                        : 
[out] outUIButtonBaseSerializeHandler(0) : 作成された UIButtonBaseSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIButtonSerializeHandler_Create

%group
LNUIButtonSerializeHandler
%prm
callback, outUIButtonSerializeHandler
[in]  callback                    : 
[out] outUIButtonSerializeHandler(0) : 作成された UIButtonSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIWindowSerializeHandler_Create

%group
LNUIWindowSerializeHandler
%prm
callback, outUIWindowSerializeHandler
[in]  callback                    : 
[out] outUIWindowSerializeHandler(0) : 作成された UIWindowSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIListItemSerializeHandler_Create

%group
LNUIListItemSerializeHandler
%prm
callback, outUIListItemSerializeHandler
[in]  callback                      : 
[out] outUIListItemSerializeHandler(0) : 作成された UIListItemSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIListItemsControlSerializeHandler_Create

%group
LNUIListItemsControlSerializeHandler
%prm
callback, outUIListItemsControlSerializeHandler
[in]  callback                              : 
[out] outUIListItemsControlSerializeHandler(0) : 作成された UIListItemsControlSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIListBoxItemSerializeHandler_Create

%group
LNUIListBoxItemSerializeHandler
%prm
callback, outUIListBoxItemSerializeHandler
[in]  callback                         : 
[out] outUIListBoxItemSerializeHandler(0) : 作成された UIListBoxItemSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIListBoxSerializeHandler_Create

%group
LNUIListBoxSerializeHandler
%prm
callback, outUIListBoxSerializeHandler
[in]  callback                     : 
[out] outUIListBoxSerializeHandler(0) : 作成された UIListBoxSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInputGestureSerializeHandler_Create

%group
LNInputGestureSerializeHandler
%prm
callback, outInputGestureSerializeHandler
[in]  callback                        : 
[out] outInputGestureSerializeHandler(0) : 作成された InputGestureSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNKeyGestureSerializeHandler_Create

%group
LNKeyGestureSerializeHandler
%prm
callback, outKeyGestureSerializeHandler
[in]  callback                      : 
[out] outKeyGestureSerializeHandler(0) : 作成された KeyGestureSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommandSerializeHandler_Create

%group
LNInterpreterCommandSerializeHandler
%prm
callback, outInterpreterCommandSerializeHandler
[in]  callback                              : 
[out] outInterpreterCommandSerializeHandler(0) : 作成された InterpreterCommandSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommandListSerializeHandler_Create

%group
LNInterpreterCommandListSerializeHandler
%prm
callback, outInterpreterCommandListSerializeHandler
[in]  callback                                  : 
[out] outInterpreterCommandListSerializeHandler(0) : 作成された InterpreterCommandListSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterSerializeHandler_Create

%group
LNInterpreterSerializeHandler
%prm
callback, outInterpreterSerializeHandler
[in]  callback                       : 
[out] outInterpreterSerializeHandler(0) : 作成された InterpreterSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterUpdateWaitHandler_Create

%group
LNInterpreterUpdateWaitHandler
%prm
callback, outInterpreterUpdateWaitHandler
[in]  callback                        : 
[out] outInterpreterUpdateWaitHandler(0) : 作成された InterpreterUpdateWaitHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNApplicationSerializeHandler_Create

%group
LNApplicationSerializeHandler
%prm
callback, outApplicationSerializeHandler
[in]  callback                       : 
[out] outApplicationSerializeHandler(0) : 作成された ApplicationSerializeHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNApplicationInitHandler_Create

%group
LNApplicationInitHandler
%prm
callback, outApplicationInitHandler
[in]  callback                  : 
[out] outApplicationInitHandler(0) : 作成された ApplicationInitHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNApplicationUpdateHandler_Create

%group
LNApplicationUpdateHandler
%prm
callback, outApplicationUpdateHandler
[in]  callback                    : 
[out] outApplicationUpdateHandler(0) : 作成された ApplicationUpdateHandler のハンドルを格納する変数。

stat : 0=成功, 負値=失敗

%inst


%href


