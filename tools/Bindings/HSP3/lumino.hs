
;============================================================
; Lumino ヘルプファイル
;============================================================

%dll
Lumino

%ver
0.9.0

%date
2020/12/7

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
LNVector3_GetX

%group
LNVector3
%prm
vector3, outReturn
[in]  vector3   : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_SetX

%group
LNVector3
%prm
vector3, value
[in] vector3 : instance
[in] value   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_GetY

%group
LNVector3
%prm
vector3, outReturn
[in]  vector3   : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_SetY

%group
LNVector3
%prm
vector3, value
[in] vector3 : instance
[in] value   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_GetZ

%group
LNVector3
%prm
vector3, outReturn
[in]  vector3   : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_SetZ

%group
LNVector3
%prm
vector3, value
[in] vector3 : instance
[in] value   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



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
[in] vector3 : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] vector3 : instance
[in] x       : 
[in] y       : 
[in] z       : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in]  vector3 : instance
[out] outX    : 
[out] outY    : 
[out] outZ    : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in]  vector3   : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in]  vector3   : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] vector3 : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in]  vec       : 
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector4_GetX

%group
LNVector4
%prm
vector4, outReturn
[in]  vector4   : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector4_SetX

%group
LNVector4
%prm
vector4, value
[in] vector4 : instance
[in] value   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector4_GetY

%group
LNVector4
%prm
vector4, outReturn
[in]  vector4   : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector4_SetY

%group
LNVector4
%prm
vector4, value
[in] vector4 : instance
[in] value   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector4_GetZ

%group
LNVector4
%prm
vector4, outReturn
[in]  vector4   : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector4_SetZ

%group
LNVector4
%prm
vector4, value
[in] vector4 : instance
[in] value   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector4_GetW

%group
LNVector4
%prm
vector4, outReturn
[in]  vector4   : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector4_SetW

%group
LNVector4
%prm
vector4, value
[in] vector4 : instance
[in] value   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] vector4 : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] vector4 : instance
[in] x       : 
[in] y       : 
[in] z       : 
[in] w       : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNQuaternion_GetX

%group
LNQuaternion
%prm
quaternion, outReturn
[in]  quaternion : instance
[out] outReturn  : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNQuaternion_SetX

%group
LNQuaternion
%prm
quaternion, value
[in] quaternion : instance
[in] value      : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNQuaternion_GetY

%group
LNQuaternion
%prm
quaternion, outReturn
[in]  quaternion : instance
[out] outReturn  : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNQuaternion_SetY

%group
LNQuaternion
%prm
quaternion, value
[in] quaternion : instance
[in] value      : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNQuaternion_GetZ

%group
LNQuaternion
%prm
quaternion, outReturn
[in]  quaternion : instance
[out] outReturn  : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNQuaternion_SetZ

%group
LNQuaternion
%prm
quaternion, value
[in] quaternion : instance
[in] value      : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNQuaternion_GetW

%group
LNQuaternion
%prm
quaternion, outReturn
[in]  quaternion : instance
[out] outReturn  : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNQuaternion_SetW

%group
LNQuaternion
%prm
quaternion, value
[in] quaternion : instance
[in] value      : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] quaternion : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] quaternion : instance
[in] x          : 
[in] y          : 
[in] z          : 
[in] w          : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] quaternion : instance
[in] axis       : 
[in] r          : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
axis が単位ベクトルでなければ正規化してから計算を行います。
%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMatrix_GetRow0

%group
LNMatrix
%prm
matrix, outReturn
[in]  matrix    : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMatrix_SetRow0

%group
LNMatrix
%prm
matrix, value
[in] matrix : instance
[in] value  : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMatrix_GetRow1

%group
LNMatrix
%prm
matrix, outReturn
[in]  matrix    : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMatrix_SetRow1

%group
LNMatrix
%prm
matrix, value
[in] matrix : instance
[in] value  : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMatrix_GetRow2

%group
LNMatrix
%prm
matrix, outReturn
[in]  matrix    : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMatrix_SetRow2

%group
LNMatrix
%prm
matrix, value
[in] matrix : instance
[in] value  : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMatrix_GetRow3

%group
LNMatrix
%prm
matrix, outReturn
[in]  matrix    : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMatrix_SetRow3

%group
LNMatrix
%prm
matrix, value
[in] matrix : instance
[in] value  : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] matrix : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] matrix : instance
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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColor_GetR

%group
LNColor
%prm
color, outReturn
[in]  color     : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColor_SetR

%group
LNColor
%prm
color, value
[in] color : instance
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColor_GetG

%group
LNColor
%prm
color, outReturn
[in]  color     : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColor_SetG

%group
LNColor
%prm
color, value
[in] color : instance
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColor_GetB

%group
LNColor
%prm
color, outReturn
[in]  color     : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColor_SetB

%group
LNColor
%prm
color, value
[in] color : instance
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColor_GetA

%group
LNColor
%prm
color, outReturn
[in]  color     : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColor_SetA

%group
LNColor
%prm
color, value
[in] color : instance
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] color : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] color        : instance
[in] r_           : 
[in] g_           : 
[in] b_           : 
[in] a_(1.000000) : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColorTone_GetR

%group
LNColorTone
%prm
colortone, outReturn
[in]  colortone : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColorTone_SetR

%group
LNColorTone
%prm
colortone, value
[in] colortone : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColorTone_GetG

%group
LNColorTone
%prm
colortone, outReturn
[in]  colortone : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColorTone_SetG

%group
LNColorTone
%prm
colortone, value
[in] colortone : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColorTone_GetB

%group
LNColorTone
%prm
colortone, outReturn
[in]  colortone : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColorTone_SetB

%group
LNColorTone
%prm
colortone, value
[in] colortone : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColorTone_GetS

%group
LNColorTone
%prm
colortone, outReturn
[in]  colortone : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColorTone_SetS

%group
LNColorTone
%prm
colortone, value
[in] colortone : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] colortone : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] colortone : instance
[in] r_        : 
[in] g_        : 
[in] b_        : 
[in] s_        : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPoint_GetX

%group
LNPoint
%prm
point, outReturn
[in]  point     : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPoint_SetX

%group
LNPoint
%prm
point, value
[in] point : instance
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPoint_GetY

%group
LNPoint
%prm
point, outReturn
[in]  point     : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPoint_SetY

%group
LNPoint
%prm
point, value
[in] point : instance
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] point : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] point : instance
[in] x_    : 
[in] y_    : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSize_GetWidth

%group
LNSize
%prm
size, outReturn
[in]  size      : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSize_SetWidth

%group
LNSize
%prm
size, value
[in] size  : instance
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSize_GetHeight

%group
LNSize
%prm
size, outReturn
[in]  size      : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSize_SetHeight

%group
LNSize
%prm
size, value
[in] size  : instance
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] size : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] size : instance
[in] w    : 
[in] h    : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRect_GetX

%group
LNRect
%prm
rect, outReturn
[in]  rect      : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRect_SetX

%group
LNRect
%prm
rect, value
[in] rect  : instance
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRect_GetY

%group
LNRect
%prm
rect, outReturn
[in]  rect      : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRect_SetY

%group
LNRect
%prm
rect, value
[in] rect  : instance
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRect_GetWidth

%group
LNRect
%prm
rect, outReturn
[in]  rect      : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRect_SetWidth

%group
LNRect
%prm
rect, value
[in] rect  : instance
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRect_GetHeight

%group
LNRect
%prm
rect, outReturn
[in]  rect      : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRect_SetHeight

%group
LNRect
%prm
rect, value
[in] rect  : instance
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] rect : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] rect   : instance
[in] x      : 
[in] y      : 
[in] width  : 
[in] height : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in]  rect      : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] rect : instance
[in] size : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in]  rect      : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNThickness_GetLeft

%group
LNThickness
%prm
thickness, outReturn
[in]  thickness : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNThickness_SetLeft

%group
LNThickness
%prm
thickness, value
[in] thickness : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNThickness_GetTop

%group
LNThickness
%prm
thickness, outReturn
[in]  thickness : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNThickness_SetTop

%group
LNThickness
%prm
thickness, value
[in] thickness : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNThickness_GetRight

%group
LNThickness
%prm
thickness, outReturn
[in]  thickness : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNThickness_SetRight

%group
LNThickness
%prm
thickness, value
[in] thickness : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNThickness_GetBottom

%group
LNThickness
%prm
thickness, outReturn
[in]  thickness : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNThickness_SetBottom

%group
LNThickness
%prm
thickness, value
[in] thickness : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] thickness : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] thickness : instance
[in] left_     : 
[in] top_      : 
[in] right_    : 
[in] bottom_   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCornerRadius_GetTopleft

%group
LNCornerRadius
%prm
cornerradius, outReturn
[in]  cornerradius : instance
[out] outReturn    : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCornerRadius_SetTopleft

%group
LNCornerRadius
%prm
cornerradius, value
[in] cornerradius : instance
[in] value        : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCornerRadius_GetTopright

%group
LNCornerRadius
%prm
cornerradius, outReturn
[in]  cornerradius : instance
[out] outReturn    : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCornerRadius_SetTopright

%group
LNCornerRadius
%prm
cornerradius, value
[in] cornerradius : instance
[in] value        : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCornerRadius_GetBottomright

%group
LNCornerRadius
%prm
cornerradius, outReturn
[in]  cornerradius : instance
[out] outReturn    : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCornerRadius_SetBottomright

%group
LNCornerRadius
%prm
cornerradius, value
[in] cornerradius : instance
[in] value        : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCornerRadius_GetBottomleft

%group
LNCornerRadius
%prm
cornerradius, outReturn
[in]  cornerradius : instance
[out] outReturn    : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCornerRadius_SetBottomleft

%group
LNCornerRadius
%prm
cornerradius, value
[in] cornerradius : instance
[in] value        : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] cornerradius : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] cornerradius : instance
[in] topLeft      : 
[in] topRight     : 
[in] bottomRight  : 
[in] bottomLeft   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] object : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] object : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in]  object    : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                  : 
[in] outPromiseFailureDelegate : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outVariant : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] variant : instance
[in] value   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  variant   : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] callback           : 
[in] outZVTestDelegate1 : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] callback           : 
[in] outZVTestDelegate2 : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] callback           : 
[in] outZVTestDelegate3 : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] callback               : 
[in] outZVTestEventHandler1 : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] callback               : 
[in] outZVTestEventHandler2 : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] zvtestpromise1 : instance
[in] callback       : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] zvtestpromise1 : instance
[in] callback       : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] zvtestpromise2 : instance
[in] callback       : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] zvtestpromise2 : instance
[in] callback       : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outZVTestClass1 : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] zvtestclass1 : instance
[in] value        : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] zvtestclass1 : instance
[in] value        : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] zvtestclass1 : instance
[in] value        : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] zvtestclass1 : instance
[in] a            : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  zvtestclass1 : instance
[in]  a            : 
[in]  b            : 
[out] outReturn    : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] zvtestclass1 : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  zvtestclass1 : instance
[out] outReturn    : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  zvtestclass1 : instance
[out] outReturn    : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  zvtestclass1 : instance
[in]  handler      : 
[out] outReturn    : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] zvtestclass1 : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  zvtestclass1 : instance
[in]  handler      : 
[out] outReturn    : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] zvtestclass1 : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outZVTestEventArgs1 : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] v                   : 
[in] outZVTestEventArgs1 : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  zvtesteventargs1 : instance
[out] outReturn        : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] target        : 
[in] outAssetModel : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  assetmodel : instance
[out] outReturn  : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in]  filePath                    : 
[in]  encoding((LNEncodingType)0) : 
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
[out] outReturn                   : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] sound : instance
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  sound     : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] sound : instance
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  sound     : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] sound   : instance
[in] enabled : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  sound     : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] sound  : instance
[in] begin  : 
[in] length : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] sound : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] sound : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] sound : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] sound : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] sound        : instance
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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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


stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] position         : 
[in] distance         : 
[in] volume(1.000000) : 
[in] pitch(1.000000)  : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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


stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback             : 
[in] outTexture2DDelegate : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] texture2dpromise : instance
[in] callback         : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] texture2dpromise : instance
[in] callback         : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[out] outReturn : instance.
                     LN_GRAPHICS_API_DEFAULT
                     自動選択
                     LN_GRAPHICS_API_OPEN_GL
                     OpenGL
                     LN_GRAPHICS_API_VULKAN
                     Vulkan

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] width        : 
[in] height       : 
[in] outTexture2D : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] width        : 
[in] height       : 
[in] format       : 
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
[in] outTexture2D : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
サポートしているフォーマットは次の通りです。PNG(.png), JPG(.jpg), TGA(.tga), BMP(.bmp), GIF(.gif)
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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  filePath  : 
[in]  settings  : 
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] shader        : instance
[in] parameterName : 
[in] value         : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] shader        : instance
[in] parameterName : 
[in] value         : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] shader        : instance
[in] parameterName : 
[in] value         : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] shader        : instance
[in] parameterName : 
[in] value         : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outMaterial : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] material : instance
[in] value    : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  material  : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] material : instance
[in] value    : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] material : instance
[in] value    : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] material : instance
[in] value    : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] material : instance
[in] value    : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] material : instance
[in] value    : 
                   LN_SHADING_MODEL_DEFAULT
                   Default
                   LN_SHADING_MODEL_UNLIT
                   Unlit

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  material  : instance
[out] outReturn : instance.
                     LN_SHADING_MODEL_DEFAULT
                     Default
                     LN_SHADING_MODEL_UNLIT
                     Unlit

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] material : instance
[in] shader   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  material  : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] meshnode : instance
[in] value    : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  meshnode  : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  animationcontroller : instance
[in]  animationClip       : 
[out] outReturn           : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] animationcontroller : instance
[in] state               : 
[in] duration(0.300000)  : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  filePath  : 
[in]  settings  : 
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  meshmodel : instance
[in]  name      : 
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  meshmodel : instance
[in]  name      : 
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  meshmodel : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  meshmodel : instance
[in]  index     : 
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  meshmodel : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outMeshImportSettings : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] size                 : 
[in] outBoxCollisionShape : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] width                : 
[in] height               : 
[in] depth                : 
[in] outBoxCollisionShape : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  animationcurve : instance
[in]  time           : 
[out] outReturn      : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outKeyFrameAnimationCurve : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] keyframeanimationcurve             : instance
[in] time                               : 
[in] value                              : 
[in] rightTangentMode((LNTangentMode)0) : 
                                             LN_TANGENT_MODE_LINEAR
                                             線形補間
                                             LN_TANGENT_MODE_TANGENT
                                             接線 (速度) を使用した補間 (エルミートスプライン)
                                             LN_TANGENT_MODE_AUTO
                                             キーフレームの値を通過するなめらかな補間 (Catmull-Rom)
                                             LN_TANGENT_MODE_CONSTANT
                                             補間なし
[in] tangent(0.000000)                  : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
rightTangentMode は、新しく追加するキーフレームの右側の補間方法です。新しく追加するキーフレームの左側の保管方法は、そのひとつ前のキーフレームの右側の保管方法が設定されます。
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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] animationclip : instance
[in] value         : 
                        LN_ANIMATION_WRAP_MODE_ONCE
                        繰り返しを行わず、1度だけ再生します。
                        LN_ANIMATION_WRAP_MODE_LOOP
                        最後まで再生された後、先頭に戻ってループします。
                        LN_ANIMATION_WRAP_MODE_ALTERNATE
                        最後まで再生された後、逆方向に戻ってループします。

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  animationclip : instance
[out] outReturn     : instance.
                         LN_ANIMATION_WRAP_MODE_ONCE
                         繰り返しを行わず、1度だけ再生します。
                         LN_ANIMATION_WRAP_MODE_LOOP
                         最後まで再生された後、先頭に戻ってループします。
                         LN_ANIMATION_WRAP_MODE_ALTERNATE
                         最後まで再生された後、逆方向に戻ってループします。

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] animationclip : instance
[in] value         : 
                        LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT
                        ルートノードのみ、平行移動を有効化します。
                        LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT_Y
                        AllowTranslationOnlyRootY
                        LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION
                        すべてのノードの平行移動を有効化します。
                        LN_HIERARCHICAL_ANIMATION_MODE_DISABLE_TRANSLATION
                        平行移動を無効化します。

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  animationclip : instance
[out] outReturn     : instance.
                         LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT
                         ルートノードのみ、平行移動を有効化します。
                         LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT_Y
                         AllowTranslationOnlyRootY
                         LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION
                         すべてのノードの平行移動を有効化します。
                         LN_HIERARCHICAL_ANIMATION_MODE_DISABLE_TRANSLATION
                         平行移動を無効化します。

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outParticleEmitterModel : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] particleemittermodel : instance
[in] count                : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] particleemittermodel : instance
[in] rate                 : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] particleemittermodel : instance
[in] time                 : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] particleemittermodel : instance
[in] size                 : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] particleemittermodel : instance
[in] value                : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] particleemittermodel : instance
[in] value                : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] particleemittermodel : instance
[in] value                : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] particleemittermodel : instance
[in] value                : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] particleemittermodel : instance
[in] value                : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] particleemittermodel : instance
[in] value                : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] particleemittermodel : instance
[in] value                : 
                               LN_PARTICLE_GEOMETRY_DIRECTION_TO_VIEW
                               常に視点方向を向く
                               LN_PARTICLE_GEOMETRY_DIRECTION_TOP
                               進行方向を軸に、表面 (Y+) が視点から見えるようにする
                               LN_PARTICLE_GEOMETRY_DIRECTION_VERTICAL_BILLBOARD
                               Y 軸方向に直立し、カメラの方を向きます。
                               LN_PARTICLE_GEOMETRY_DIRECTION_HORIZONTAL_BILLBOARD
                               XZ 軸上の「床」平面と平行になります。

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] particleemittermodel : instance
[in] material             : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outParticleModel : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] particlemodel : instance
[in] value         : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  particlemodel : instance
[out] outReturn     : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] particlemodel : instance
[in] emitter       : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] visualcomponent : instance
[in] value           : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  visualcomponent : instance
[out] outReturn       : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] spritecomponent : instance
[in] texture         : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] callback                 : 
[in] outCollisionEventHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outCharacterController : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] charactercontroller : instance
[in] value               : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  charactercontroller : instance
[out] outReturn           : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] charactercontroller : instance
[in] value               : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  charactercontroller : instance
[out] outReturn           : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] charactercontroller : instance
[in] value               : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
false を指定した場合、キャラクターの想定外の自走を防止するため、速度も 0 にリセットされます。
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
[in] charactercontroller : instance
[in] value               : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
有効である場合、関連付けられているカメラを通じて、描画先となるビューの MouseGrab を取得します。つまり、マウスカーソルは非表示となり UI をポイントしてクリックする等の操作はできなくなります。
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
[in] charactercontroller : instance
[in] value               : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  charactercontroller : instance
[out] outReturn           : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] charactercontroller : instance
[in] value               : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  charactercontroller : instance
[out] outReturn           : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] charactercontroller : instance
[in] handler             : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] charactercontroller : instance
[in] handler             : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] charactercontroller : instance
[in] handler             : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] world : instance
[in] obj   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  componentlist : instance
[out] outReturn     : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  componentlist : instance
[in]  index         : 
[out] outReturn     : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outWorldObject : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] worldobject : instance
[in] pos         : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] worldobject : instance
[in] x           : 
[in] y           : 
[in] z(0.000000) : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  worldobject : instance
[out] outReturn   : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetRotationQuaternion
このオブジェクトの回転を設定します。
%group
LNWorldObject
%prm
worldobject, rot
[in] worldobject : instance
[in] rot         : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetRotation
このオブジェクトの回転をオイラー角から設定します(radian) 。回転順序は Z(Roll) > X(Pich) > Y(Yaw) です。
%group
LNWorldObject
%prm
worldobject, x, y, z
[in] worldobject : instance
[in] x           : 
[in] y           : 
[in] z           : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  worldobject : instance
[out] outReturn   : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] worldobject : instance
[in] scale       : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] worldobject : instance
[in] xyz         : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] worldobject : instance
[in] x           : 
[in] y           : 
[in] z(1.000000) : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  worldobject : instance
[out] outReturn   : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] worldobject : instance
[in] value       : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] worldobject : instance
[in] x           : 
[in] y           : 
[in] z(0.000000) : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  worldobject : instance
[out] outReturn   : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] worldobject : instance
[in] target      : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] worldobject : instance
[in] x           : 
[in] y           : 
[in] z           : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] worldobject : instance
[in] component   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] worldobject : instance
[in] component   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] worldobject : instance
[in] tag         : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] worldobject : instance
[in] tag         : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  worldobject : instance
[in]  tag         : 
[out] outReturn   : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] worldobject : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
実際の削除は、現在のフレームのアップデート処理後に行われます。削除された WorldObject は、親の World, Level, WorldObject からも除外されます。
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
[in]  worldobject : instance
[out] outReturn   : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] visualobject : instance
[in] value        : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  visualobject : instance
[out] outReturn    : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] visualobject : instance
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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] visualobject : instance
[in] value        : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  visualobject : instance
[out] outReturn    : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] environmentlight : instance
[in] enabled          : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  environmentlight : instance
[out] outReturn        : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] environmentlight : instance
[in] value            : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  environmentlight : instance
[out] outReturn        : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] environmentlight : instance
[in] value            : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  environmentlight : instance
[out] outReturn        : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  environmentlight : instance
[out] outReturn        : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] environmentlight : instance
[in] value            : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  environmentlight : instance
[out] outReturn        : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] environmentlight : instance
[in] value            : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] environmentlight : instance
[in] intensity        : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  environmentlight : instance
[out] outReturn        : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] environmentlight : instance
[in] value            : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  environmentlight : instance
[out] outReturn        : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] environmentlight : instance
[in] value            : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  environmentlight : instance
[out] outReturn        : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outDirectionalLight : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] color               : 
[in] outDirectionalLight : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] directionallight : instance
[in] enabled          : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  directionallight : instance
[out] outReturn        : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] directionallight : instance
[in] color            : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  directionallight : instance
[out] outReturn        : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] directionallight : instance
[in] intensity        : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  directionallight : instance
[out] outReturn        : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] directionallight : instance
[in] value            : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  directionallight : instance
[out] outReturn        : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] directionallight : instance
[in] value            : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  directionallight : instance
[out] outReturn        : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outPointLight : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] color         : 
[in] range         : 
[in] outPointLight : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] pointlight : instance
[in] enabled    : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  pointlight : instance
[out] outReturn  : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] pointlight : instance
[in] color      : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  pointlight : instance
[out] outReturn  : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] pointlight : instance
[in] intensity  : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  pointlight : instance
[out] outReturn  : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] pointlight : instance
[in] range      : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  pointlight : instance
[out] outReturn  : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] pointlight  : instance
[in] attenuation : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  pointlight : instance
[out] outReturn  : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outSpotLight : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] color        : 
[in] range        : 
[in] angle        : 
[in] outSpotLight : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] spotlight : instance
[in] enabled   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  spotlight : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] spotlight : instance
[in] color     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  spotlight : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] spotlight : instance
[in] intensity : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  spotlight : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] spotlight : instance
[in] range     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  spotlight : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] spotlight   : instance
[in] attenuation : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  spotlight : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] spotlight : instance
[in] angle     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  spotlight : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] spotlight : instance
[in] penumbra  : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  spotlight : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] callback        : 
[in] outTestDelegate : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outSprite : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] texture   : 
[in] outSprite : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] texture   : 
[in] width     : 
[in] height    : 
[in] outSprite : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] sprite : instance
[in] value  : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] sprite : instance
[in] value  : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] sprite : instance
[in] width  : 
[in] height : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] sprite : instance
[in] x      : 
[in] y      : 
[in] width  : 
[in] height : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] sprite   : instance
[in] callback : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outCameraOrbitControlComponent : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  point     : 
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  raycaster : instance
[in]  normalX   : 
[in]  normalY   : 
[in]  normalZ   : 
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  raycastresult : instance
[out] outReturn     : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] worldrenderview : instance
[in] value           : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  worldrenderview : instance
[out] outReturn       : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outBoxMesh : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNBoxMesh_CreateXYZ

%group
LNBoxMesh
%prm
width, height, depth, outBoxMesh
[in] width      : 
[in] height     : 
[in] depth      : 
[in] outBoxMesh : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outPlaneMesh : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNStaticMesh_Load
load
%group
LNStaticMesh
%prm
filePath, outReturn
[in]  filePath  : 
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNStaticMesh_GetModel
指定した名前の MeshContainer から、衝突判定用の Body を作成します。
%group
LNStaticMesh
%prm
staticmesh, outReturn
[in]  staticmesh : instance
[out] outReturn  : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNStaticMesh_MakeCollisionBody
指定した名前の MeshContainer から、衝突判定用の Body を作成します。
%group
LNStaticMesh
%prm
staticmesh, meshContainerName
[in] staticmesh        : instance
[in] meshContainerName : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNStaticMeshComponent_Create
init
%group
LNStaticMeshComponent
%prm
outStaticMeshComponent
[in] outStaticMeshComponent : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNStaticMeshComponent_SetModel
setModel
%group
LNStaticMeshComponent
%prm
staticmeshcomponent, model
[in] staticmeshcomponent : instance
[in] model               : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNStaticMeshComponent_MakeCollisionBody
指定した名前の MeshContainer から、衝突判定用の Body を作成します。
%group
LNStaticMeshComponent
%prm
staticmeshcomponent, meshContainerName
[in] staticmeshcomponent : instance
[in] meshContainerName   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSkinnedMeshComponent_Create
init
%group
LNSkinnedMeshComponent
%prm
outSkinnedMeshComponent
[in] outSkinnedMeshComponent : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  collision : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outTriggerBodyComponent : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] triggerbodycomponent : instance
[in] shape                : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] model              : 
[in] outParticleEmitter : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.
                     LN_LEVEL_TRANSITION_EFFECT_MODE_NONE
                     エフェクト無し
                     LN_LEVEL_TRANSITION_EFFECT_MODE_FADE_IN_OUT
                     フェードイン・フェードアウト
                     LN_LEVEL_TRANSITION_EFFECT_MODE_CROSS_FADE
                     クロスフェード

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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


stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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


stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] outLevel : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] level : instance
[in] obj   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] level : instance
[in] obj   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] level    : instance
[in] sublevel : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] level    : instance
[in] sublevel : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] level : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in]  uieventargs : instance
[out] outReturn   : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] callback                 : 
[in] outUIGeneralEventHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] callback          : 
[in] outUIEventHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] size      : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] width     : 
[in] height    : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] margin    : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] padding   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] value     : 
                    LN_UIHALIGNMENT_LEFT
                    子要素を、親のレイアウト スロットの左側に揃えて配置します。
                    LN_UIHALIGNMENT_CENTER
                    子要素を、親のレイアウト スロットの中央に揃えて配置します。
                    LN_UIHALIGNMENT_RIGHT
                    子要素を、親のレイアウト スロットの右側に揃えて配置します。
                    LN_UIHALIGNMENT_STRETCH
                    子要素を、親のレイアウト スロット全体に引き伸ばします。

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.
                     LN_UIHALIGNMENT_LEFT
                     子要素を、親のレイアウト スロットの左側に揃えて配置します。
                     LN_UIHALIGNMENT_CENTER
                     子要素を、親のレイアウト スロットの中央に揃えて配置します。
                     LN_UIHALIGNMENT_RIGHT
                     子要素を、親のレイアウト スロットの右側に揃えて配置します。
                     LN_UIHALIGNMENT_STRETCH
                     子要素を、親のレイアウト スロット全体に引き伸ばします。

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] value     : 
                    LN_UIVALIGNMENT_TOP
                    子要素を、親のレイアウト スロットの上端に揃えて配置します。
                    LN_UIVALIGNMENT_CENTER
                    子要素を、親のレイアウト スロットの中央に揃えて配置します。
                    LN_UIVALIGNMENT_BOTTOM
                    子要素を、親のレイアウト スロットの下端に揃えて配置します。
                    LN_UIVALIGNMENT_STRETCH
                    子要素を、親のレイアウト スロット全体に引き伸ばします。

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.
                     LN_UIVALIGNMENT_TOP
                     子要素を、親のレイアウト スロットの上端に揃えて配置します。
                     LN_UIVALIGNMENT_CENTER
                     子要素を、親のレイアウト スロットの中央に揃えて配置します。
                     LN_UIVALIGNMENT_BOTTOM
                     子要素を、親のレイアウト スロットの下端に揃えて配置します。
                     LN_UIVALIGNMENT_STRETCH
                     子要素を、親のレイアウト スロット全体に引き伸ばします。

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] pos       : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement   : instance
[in] x           : 
[in] y           : 
[in] z(0.000000) : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] rot       : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] x         : 
[in] y         : 
[in] z         : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] scale     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] xyz       : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] x         : 
[in] y         : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement   : instance
[in] x           : 
[in] y           : 
[in] z(0.000000) : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] value     : 
                    LN_UIVISIBILITY_VISIBLE
                    要素を表示します。
                    LN_UIVISIBILITY_HIDDEN
                    要素を表示しませんが、その要素の領域をレイアウト内に予約します。
                    LN_UIVISIBILITY_COLLAPSED
                    要素を表示しません。また、その要素の領域もレイアウト内に予約しません。

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.
                     LN_UIVISIBILITY_VISIBLE
                     要素を表示します。
                     LN_UIVISIBILITY_HIDDEN
                     要素を表示しませんが、その要素の領域をレイアウト内に予約します。
                     LN_UIVISIBILITY_COLLAPSED
                     要素を表示しません。また、その要素の領域もレイアウト内に予約しません。

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] child     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uielement : instance
[in] value     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uielement : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUITextBlock_Create
UITextBlock を作成します。
%group
LNUITextBlock
%prm
outUITextBlock
[in] outUITextBlock : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUITextBlock_CreateWithText
表示文字列を指定して、UITextBlock を作成します。
%group
LNUITextBlock
%prm
text, outUITextBlock
[in] text           : 
[in] outUITextBlock : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUITextBlock_SetText
表示文字列を設定します。
%group
LNUITextBlock
%prm
uitextblock, value
[in] uitextblock : instance
[in] value       : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUITextBlock_GetText
表示文字列を取得します。
%group
LNUITextBlock
%prm
uitextblock, outReturn
[in]  uitextblock : instance
[out] outReturn   : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outUISprite : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] texture     : 
[in] outUISprite : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uisprite : instance
[in] texture  : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uisprite : instance
[in] rect     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uisprite : instance
[in] x        : 
[in] y        : 
[in] width    : 
[in] height   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uisprite  : instance
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uisprite : instance
[in] shader   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  color     : 
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outUIMessageTextArea : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uimessagetextarea : instance
[in] value             : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uimessagetextarea : instance
[in] value             : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] outUIBoxLayout : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uiboxlayout : instance
[in] orientation : 
                      LN_UILAYOUT_ORIENTATION_HORIZONTAL
                      水平方向に配置します。
                      LN_UILAYOUT_ORIENTATION_VERTICAL
                      垂直方向に配置します。
                      LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL
                      水平方向（右から左）に配置します。
                      LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL
                      垂直方向（下から上）に配置します。

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uiboxlayout : instance
[out] outReturn   : instance.
                       LN_UILAYOUT_ORIENTATION_HORIZONTAL
                       水平方向に配置します。
                       LN_UILAYOUT_ORIENTATION_VERTICAL
                       垂直方向に配置します。
                       LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL
                       水平方向（右から左）に配置します。
                       LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL
                       垂直方向（下から上）に配置します。

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outUIStackLayout : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uistacklayout : instance
[in] orientation   : 
                        LN_UILAYOUT_ORIENTATION_HORIZONTAL
                        水平方向に配置します。
                        LN_UILAYOUT_ORIENTATION_VERTICAL
                        垂直方向に配置します。
                        LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL
                        水平方向（右から左）に配置します。
                        LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL
                        垂直方向（下から上）に配置します。

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uistacklayout : instance
[out] outReturn     : instance.
                         LN_UILAYOUT_ORIENTATION_HORIZONTAL
                         水平方向に配置します。
                         LN_UILAYOUT_ORIENTATION_VERTICAL
                         垂直方向に配置します。
                         LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL
                         水平方向（右から左）に配置します。
                         LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL
                         垂直方向（下から上）に配置します。

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outUIGridLayout : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uigridlayout : instance
[in] value        : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outUIControl : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uicontrol : instance
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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uibuttonbase : instance
[in] text         : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outUIButton : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] text        : 
[in] outUIButton : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uibutton  : instance
[in]  handler   : 
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outUIWindow : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uilistitem : instance
[in]  handler    : 
[out] outReturn  : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uilistitemscontrol : instance
[in] layout             : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] uilistitemscontrol : instance
[in] value              : 
                             LN_UILIST_SUBMIT_MODE_SINGLE
                             ゲームUI用。Hover で選択状態、シングルクリックで Submit. Hover 状態は使用されない。
                             LN_UILIST_SUBMIT_MODE_DOUBLE
                             エディタUI用。シングルクリックで選択状態、ダブルクリックで Submit.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uilistitemscontrol : instance
[out] outReturn          : instance.
                              LN_UILIST_SUBMIT_MODE_SINGLE
                              ゲームUI用。Hover で選択状態、シングルクリックで Submit. Hover 状態は使用されない。
                              LN_UILIST_SUBMIT_MODE_DOUBLE
                              エディタUI用。シングルクリックで選択状態、ダブルクリックで Submit.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] content          : 
[in] outUIListBoxItem : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outUIListBox : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  uilistbox : instance
[in]  content   : 
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] key           : 
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
                        
[in] outKeyGesture : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[out] outReturn  : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn  : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn  : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn  : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn  : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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


stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMouse_Position
マウスポインタの位置を取得します。
%group
LNMouse
%prm
outReturn
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in]  interpretercommand : instance
[out] outReturn          : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  interpretercommand : instance
[out] outReturn          : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  interpretercommand : instance
[in]  index              : 
[out] outReturn          : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outInterpreterCommandList : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] interpretercommandlist : instance
[in] code                   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] interpretercommandlist : instance
[in] code                   : 
[in] param0                 : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] interpretercommandlist : instance
[in] code                   : 
[in] param0                 : 
[in] param1                 : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] interpretercommandlist : instance
[in] code                   : 
[in] param0                 : 
[in] param1                 : 
[in] param2                 : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] interpretercommandlist : instance
[in] code                   : 
[in] param0                 : 
[in] param1                 : 
[in] param2                 : 
[in] param3                 : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] callback                      : 
[in] outInterpreterCommandDelegate : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] outInterpreter : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] interpreter : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] interpreter : instance
[in] commandList : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  interpreter : instance
[out] outReturn   : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] interpreter : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] interpreter : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] interpreter : instance
[in] name        : 
[in] handler     : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] interpreter : instance
[in] mode        : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  interpreter : instance
[out] outReturn   : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] interpreter : instance
[in] count       : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  interpreter : instance
[out] outReturn   : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetMainWindowResizable
メインウィンドウをユーザーがサイズ変更できるかどうかを指定します。(default: false)
%group
LNEngineSettings
%prm
value
[in] value : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetEngineLogEnabled
デバッグ用のログファイルの出力有無を設定します。(default: Debug ビルドの場合true、それ以外は false)
%group
LNEngineSettings
%prm
enabled
[in] enabled : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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


stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_Finalize
エンジンの終了処理を行います。
%group
LNEngine
%prm


stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_Run
指定した Application の実行を開始します。
%group
LNEngine
%prm
app
[in] app : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
この機能を呼び出した場合、Engine::initialize(), Engine::finalize(), Engine::update() を呼び出すことはできなくなります。代わりに Application::onInit(), Application::onUpdate() などを使用してください。
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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_GetCamera
デフォルトで作成されるメインの Camera です。
%group
LNEngine
%prm
outReturn
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[out] outReturn : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] outApplication : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] application : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in] application : instance

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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
[in]  application : instance
[out] outReturn   : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] color : 
[in] str   : 

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                  : 
[in] outObjectSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                           : 
[in] outEventConnectionSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                   : 
[in] outVariantSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                        : 
[in] outZVTestClass1SerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                            : 
[in] outZVTestEventArgs1SerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                       : 
[in] outSerializer2SerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                       : 
[in] outAssetObjectSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                               : 
[in] outAssetImportSettingsSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                      : 
[in] outAssetModelSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                 : 
[in] outSoundSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                   : 
[in] outTextureSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                     : 
[in] outTexture2DSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                  : 
[in] outShaderSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                      : 
[in] outRenderViewSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                    : 
[in] outMaterialSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                    : 
[in] outMeshNodeSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                               : 
[in] outAnimationControllerSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                     : 
[in] outMeshModelSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                              : 
[in] outMeshImportSettingsSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                            : 
[in] outSkinnedMeshModelSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                          : 
[in] outCollisionShapeSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                             : 
[in] outBoxCollisionShapeSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                          : 
[in] outAnimationCurveSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                                  : 
[in] outKeyFrameAnimationCurveSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                         : 
[in] outAnimationClipSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                          : 
[in] outAnimationStateSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                          : 
[in] outEffectResourceSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                                : 
[in] outParticleEmitterModelSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                         : 
[in] outParticleModelSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                     : 
[in] outComponentSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                           : 
[in] outVisualComponentSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                           : 
[in] outSpriteComponentSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                               : 
[in] outCharacterControllerSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                 : 
[in] outWorldSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                         : 
[in] outComponentListSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                       : 
[in] outWorldObjectSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                       : 
[in] outWorldObjectPreUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                    : 
[in] outWorldObjectUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                        : 
[in] outVisualObjectSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                        : 
[in] outVisualObjectPreUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                     : 
[in] outVisualObjectUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                  : 
[in] outCameraSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                  : 
[in] outCameraPreUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback               : 
[in] outCameraUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                            : 
[in] outEnvironmentLightSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                            : 
[in] outEnvironmentLightPreUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                         : 
[in] outEnvironmentLightUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                            : 
[in] outDirectionalLightSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                            : 
[in] outDirectionalLightPreUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                         : 
[in] outDirectionalLightUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                      : 
[in] outPointLightSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                      : 
[in] outPointLightPreUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                   : 
[in] outPointLightUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                     : 
[in] outSpotLightSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                     : 
[in] outSpotLightPreUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                  : 
[in] outSpotLightUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                  : 
[in] outSpriteSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                  : 
[in] outSpritePreUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback               : 
[in] outSpriteUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                                       : 
[in] outCameraOrbitControlComponentSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                     : 
[in] outRaycasterSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                         : 
[in] outRaycastResultSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                           : 
[in] outWorldRenderViewSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                   : 
[in] outBoxMeshSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                   : 
[in] outBoxMeshPreUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                : 
[in] outBoxMeshUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                     : 
[in] outPlaneMeshSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                     : 
[in] outPlaneMeshPreUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                  : 
[in] outPlaneMeshUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNStaticMeshSerializeHandler_Create

%group
LNStaticMeshSerializeHandler
%prm
callback, outStaticMeshSerializeHandler
[in] callback                      : 
[in] outStaticMeshSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNStaticMeshPreUpdateHandler_Create

%group
LNStaticMeshPreUpdateHandler
%prm
callback, outStaticMeshPreUpdateHandler
[in] callback                      : 
[in] outStaticMeshPreUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNStaticMeshUpdateHandler_Create

%group
LNStaticMeshUpdateHandler
%prm
callback, outStaticMeshUpdateHandler
[in] callback                   : 
[in] outStaticMeshUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNStaticMeshComponentSerializeHandler_Create

%group
LNStaticMeshComponentSerializeHandler
%prm
callback, outStaticMeshComponentSerializeHandler
[in] callback                               : 
[in] outStaticMeshComponentSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSkinnedMeshComponentSerializeHandler_Create

%group
LNSkinnedMeshComponentSerializeHandler
%prm
callback, outSkinnedMeshComponentSerializeHandler
[in] callback                                : 
[in] outSkinnedMeshComponentSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                     : 
[in] outCollisionSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                                : 
[in] outTriggerBodyComponentSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                           : 
[in] outParticleEmitterSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                           : 
[in] outParticleEmitterPreUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                        : 
[in] outParticleEmitterUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                 : 
[in] outLevelSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback             : 
[in] outLevelStartHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback            : 
[in] outLevelStopHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback             : 
[in] outLevelPauseHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback              : 
[in] outLevelResumeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback              : 
[in] outLevelUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                       : 
[in] outUIEventArgsSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                           : 
[in] outUILayoutElementSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                     : 
[in] outUIElementSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUITextBlockSerializeHandler_Create

%group
LNUITextBlockSerializeHandler
%prm
callback, outUITextBlockSerializeHandler
[in] callback                       : 
[in] outUITextBlockSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                    : 
[in] outUISpriteSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                  : 
[in] outUIIconSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                             : 
[in] outUIMessageTextAreaSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                         : 
[in] outUILayoutPanelSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                       : 
[in] outUIBoxLayoutSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                         : 
[in] outUIStackLayoutSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                        : 
[in] outUIGridLayoutSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                     : 
[in] outUIControlSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                        : 
[in] outUIButtonBaseSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                    : 
[in] outUIButtonSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                    : 
[in] outUIWindowSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                      : 
[in] outUIListItemSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                              : 
[in] outUIListItemsControlSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                         : 
[in] outUIListBoxItemSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                     : 
[in] outUIListBoxSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                        : 
[in] outInputGestureSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                      : 
[in] outKeyGestureSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                              : 
[in] outInterpreterCommandSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                                  : 
[in] outInterpreterCommandListSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                       : 
[in] outInterpreterSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                        : 
[in] outInterpreterUpdateWaitHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                       : 
[in] outApplicationSerializeHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                  : 
[in] outApplicationInitHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

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
[in] callback                    : 
[in] outApplicationUpdateHandler : instance.

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst

%href


