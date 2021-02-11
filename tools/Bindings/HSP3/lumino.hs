
;============================================================
; Lumino �w���v�t�@�C��
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
lumino.as ���C���N���[�h���Ă�������

%type
�g������

%url
http ://nnmy.sakura.ne.jp/



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_SetZeros
���ׂĂ̗v�f�� 0.0 �ɐݒ肵�ăC���X�^���X�����������܂��B
%group
LNVector3
%prm
vector3
[in] vector3 : (Vector3 �^�̒l) Vector3

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_Set
�w�肵���l���g�p���ăC���X�^���X�����������܂��B
%group
LNVector3
%prm
vector3, x, y, z
[in] vector3 : (Vector3 �^�̒l) Vector3
[in] x       : 
[in] y       : 
[in] z       : 

stat : 0=����, ���l=���s

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
[in]  vector3 : (Vector3 �^�̒l) Vector3
[out] outX    : 
[out] outY    : 
[out] outZ    : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_Length
�x�N�g���̒�����Ԃ��܂��B
%group
LNVector3
%prm
vector3, outReturn
[in]  vector3   : (Vector3 �^�̒l) Vector3
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_LengthSquared
�x�N�g���̒����� 2 ���Ԃ��܂��B
%group
LNVector3
%prm
vector3, outReturn
[in]  vector3   : (Vector3 �^�̒l) Vector3
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_MutatingNormalize
���̃x�N�g���𐳋K�����܂��B
%group
LNVector3
%prm
vector3
[in] vector3 : (Vector3 �^�̒l) Vector3

stat : 0=����, ���l=���s

%inst
�x�N�g���̒����� 0 �̏ꍇ�͐��K�����s���܂���B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_NormalizeXYZ
�w��x�N�g���𐳋K�������x�N�g����Ԃ��܂��B
%group
LNVector3
%prm
x, y, z, outReturn
[in]  x         : 
[in]  y         : 
[in]  z         : 
[out] outReturn(0) : (Vector3 �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector3_Normalize
�w��x�N�g���𐳋K�������x�N�g����Ԃ��܂��B
%group
LNVector3
%prm
vec, outReturn
[in]  vec       : (Vector3 �^�̒l) 
[out] outReturn(0) : (Vector3 �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector4_SetZeros
���ׂĂ̗v�f�� 0.0 �ɐݒ肵�ăC���X�^���X�����������܂��B
%group
LNVector4
%prm
vector4
[in] vector4 : (Vector4 �^�̒l) Vector4

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVector4_Set
�w�肵���l���g�p���ăC���X�^���X�����������܂��B
%group
LNVector4
%prm
vector4, x, y, z, w
[in] vector4 : (Vector4 �^�̒l) Vector4
[in] x       : 
[in] y       : 
[in] z       : 
[in] w       : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNQuaternion_SetZeros
�P�ʃN�H�[�^�j�I����ݒ肵�ăC���X�^���X�����������܂��B
%group
LNQuaternion
%prm
quaternion
[in] quaternion : (Quaternion �^�̒l) Quaternion

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNQuaternion_Set
�w�肵���l���g�p���ăC���X�^���X�����������܂��B
%group
LNQuaternion
%prm
quaternion, x, y, z, w
[in] quaternion : (Quaternion �^�̒l) Quaternion
[in] x          : 
[in] y          : 
[in] z          : 
[in] w          : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNQuaternion_SetFromAxis
��]���Ɖ�]�p�x���w�肵�ăC���X�^���X�����������܂��B
%group
LNQuaternion
%prm
quaternion, axis, r
[in] quaternion : (Quaternion �^�̒l) Quaternion
[in] axis       : (Vector3 �^�̒l) 
[in] r          : 

stat : 0=����, ���l=���s

%inst
axis ���P�ʃx�N�g���łȂ���ΐ��K�����Ă���v�Z���s���܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMatrix_SetZeros
�P�ʍs���ݒ肵�ăC���X�^���X�����������܂��B
%group
LNMatrix
%prm
matrix
[in] matrix : (Matrix �^�̒l) Matrix

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMatrix_Set
�e�v�f���w�肵�ăC���X�^���X�����������܂��B
%group
LNMatrix
%prm
matrix, m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44
[in] matrix : (Matrix �^�̒l) Matrix
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

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColor_SetZeros
���ׂĂ̗v�f�� 0.0 �ŏ��������܂��B
%group
LNColor
%prm
color
[in] color : (Color �^�̒l) Color

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColor_Set
�e�v�f���w�肵�ď��������܂��B
%group
LNColor
%prm
color, r_, g_, b_, a_
[in] color        : (Color �^�̒l) Color
[in] r_           : 
[in] g_           : 
[in] b_           : 
[in] a_(1.000000) : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColorTone_SetZeros
���ׂĂ̗v�f�� 0.0 �ŏ��������܂��B
%group
LNColorTone
%prm
colortone
[in] colortone : (ColorTone �^�̒l) ColorTone

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNColorTone_Set
�e�v�f���w�肵�ď��������܂��B
%group
LNColorTone
%prm
colortone, r_, g_, b_, s_
[in] colortone : (ColorTone �^�̒l) ColorTone
[in] r_        : 
[in] g_        : 
[in] b_        : 
[in] s_        : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPoint_SetZeros
���ׂĂ̗v�f�� 0 �ŏ��������܂��B
%group
LNPoint
%prm
point
[in] point : (Point �^�̒l) Point

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPoint_Set
�ʒu���w�肵�ď��������܂��B
%group
LNPoint
%prm
point, x_, y_
[in] point : (Point �^�̒l) Point
[in] x_    : 
[in] y_    : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPoint_Get
�e�v�f�̒l���擾���܂��B
%group
LNPoint
%prm
point, outX, outY
[in]  point : (Point �^�̒l) Point
[out] outX  : 
[out] outY  : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSize_SetZeros
���ׂĂ̗v�f�� 0 �ŏ��������܂��B
%group
LNSize
%prm
size
[in] size : (Size �^�̒l) Size

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSize_Set
���ƍ������w�肵�ď��������܂��B
%group
LNSize
%prm
size, w, h
[in] size : (Size �^�̒l) Size
[in] w    : 
[in] h    : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRect_SetZeros
���ׂĂ̗v�f�� 0 �ŏ��������܂��B
%group
LNRect
%prm
rect
[in] rect : (Rect �^�̒l) Rect

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRect_Set
�ʒu�ƃT�C�Y���w�肵�ď��������܂��B
%group
LNRect
%prm
rect, x, y, width, height
[in] rect   : (Rect �^�̒l) Rect
[in] x      : 
[in] y      : 
[in] width  : 
[in] height : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRect_GetLeft
���ӂ� x ���̒l���擾���܂��B
%group
LNRect
%prm
rect, outReturn
[in]  rect      : (Rect �^�̒l) Rect
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRect_SetSize
���ƍ�����ݒ肵�܂��B
%group
LNRect
%prm
rect, size
[in] rect : (Rect �^�̒l) Rect
[in] size : (Size �^�̒l) 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRect_GetSize
���ƍ������擾���܂��B
%group
LNRect
%prm
rect, outReturn
[in]  rect      : (Rect �^�̒l) Rect
[out] outReturn(0) : (Size �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNThickness_SetZeros
���ׂĂ̗v�f�� 0 �ŏ��������܂��B
%group
LNThickness
%prm
thickness
[in] thickness : (Thickness �^�̒l) Thickness

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNThickness_Set
�e�ӂ̕����w�肵�ď��������܂��B
%group
LNThickness
%prm
thickness, left_, top_, right_, bottom_
[in] thickness : (Thickness �^�̒l) Thickness
[in] left_     : 
[in] top_      : 
[in] right_    : 
[in] bottom_   : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCornerRadius_SetZeros
���ׂĂ̗v�f�� 0 �ŏ��������܂��B
%group
LNCornerRadius
%prm
cornerradius
[in] cornerradius : (CornerRadius �^�̒l) CornerRadius

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCornerRadius_Set
�e�v�f�̒l���w�肵�ď��������܂��B
%group
LNCornerRadius
%prm
cornerradius, topLeft, topRight, bottomRight, bottomLeft
[in] cornerradius : (CornerRadius �^�̒l) CornerRadius
[in] topLeft      : 
[in] topRight     : 
[in] bottomRight  : 
[in] bottomLeft   : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNObject_Release
�I�u�W�F�N�g�̎Q�Ƃ��J�����܂��B
%group
LNObject
%prm
object
[in] object : Object �̃n���h��

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNObject_Retain
�I�u�W�F�N�g�̎Q�Ƃ��擾���܂��B
%group
LNObject
%prm
object
[in] object : Object �̃n���h��

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNObject_GetReferenceCount
�I�u�W�F�N�g�̎Q�ƃJ�E���g���擾���܂��B����͓����I�Ɏg�p����閽�߂ł��B
%group
LNObject
%prm
object, outReturn
[in]  object    : Object �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outPromiseFailureDelegate(0) : �쐬���ꂽ PromiseFailureDelegate �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
PromiseFailureDelegate �� Object �̃T�u�N���X�ł��BPromiseFailureDelegate �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outVariant(0) : �쐬���ꂽ Variant �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Variant �� Object �̃T�u�N���X�ł��BVariant �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] variant : Variant �̃n���h��
[in] value   : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Variant �� Object �̃T�u�N���X�ł��BVariant �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  variant   : Variant �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Variant �� Object �̃T�u�N���X�ł��BVariant �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outZVTestDelegate1(0) : �쐬���ꂽ ZVTestDelegate1 �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestDelegate1 �� Object �̃T�u�N���X�ł��BZVTestDelegate1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outZVTestDelegate2(0) : �쐬���ꂽ ZVTestDelegate2 �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestDelegate2 �� Object �̃T�u�N���X�ł��BZVTestDelegate2 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outZVTestDelegate3(0) : �쐬���ꂽ ZVTestDelegate3 �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestDelegate3 �� Object �̃T�u�N���X�ł��BZVTestDelegate3 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outZVTestEventHandler1(0) : �쐬���ꂽ ZVTestEventHandler1 �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestEventHandler1 �� Object �̃T�u�N���X�ł��BZVTestEventHandler1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outZVTestEventHandler2(0) : �쐬���ꂽ ZVTestEventHandler2 �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestEventHandler2 �� Object �̃T�u�N���X�ł��BZVTestEventHandler2 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] zvtestpromise1 : ZVTestPromise1 �̃n���h��
[in] callback       : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestPromise1 �� Object �̃T�u�N���X�ł��BZVTestPromise1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] zvtestpromise1 : ZVTestPromise1 �̃n���h��
[in] callback       : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestPromise1 �� Object �̃T�u�N���X�ł��BZVTestPromise1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] zvtestpromise2 : ZVTestPromise2 �̃n���h��
[in] callback       : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestPromise2 �� Object �̃T�u�N���X�ł��BZVTestPromise2 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] zvtestpromise2 : ZVTestPromise2 �̃n���h��
[in] callback       : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestPromise2 �� Object �̃T�u�N���X�ł��BZVTestPromise2 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outZVTestClass1(0) : �쐬���ꂽ ZVTestClass1 �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestClass1 �� Object �̃T�u�N���X�ł��BZVTestClass1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] zvtestclass1 : ZVTestClass1 �̃n���h��
[in] value        : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestClass1 �� Object �̃T�u�N���X�ł��BZVTestClass1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] zvtestclass1 : ZVTestClass1 �̃n���h��
[in] value        : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestClass1 �� Object �̃T�u�N���X�ł��BZVTestClass1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] zvtestclass1 : ZVTestClass1 �̃n���h��
[in] value        : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestClass1 �� Object �̃T�u�N���X�ł��BZVTestClass1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] zvtestclass1 : ZVTestClass1 �̃n���h��
[in] a            : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestClass1 �� Object �̃T�u�N���X�ł��BZVTestClass1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  zvtestclass1 : ZVTestClass1 �̃n���h��
[in]  a            : 
[in]  b            : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestClass1 �� Object �̃T�u�N���X�ł��BZVTestClass1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] zvtestclass1 : ZVTestClass1 �̃n���h��

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestClass1 �� Object �̃T�u�N���X�ł��BZVTestClass1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestClass1 �� Object �̃T�u�N���X�ł��BZVTestClass1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  zvtestclass1 : ZVTestClass1 �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestClass1 �� Object �̃T�u�N���X�ł��BZVTestClass1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  zvtestclass1 : ZVTestClass1 �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestClass1 �� Object �̃T�u�N���X�ł��BZVTestClass1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  zvtestclass1 : ZVTestClass1 �̃n���h��
[in]  handler      : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestClass1 �� Object �̃T�u�N���X�ł��BZVTestClass1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] zvtestclass1 : ZVTestClass1 �̃n���h��

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestClass1 �� Object �̃T�u�N���X�ł��BZVTestClass1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  zvtestclass1 : ZVTestClass1 �̃n���h��
[in]  handler      : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestClass1 �� Object �̃T�u�N���X�ł��BZVTestClass1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] zvtestclass1 : ZVTestClass1 �̃n���h��

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestClass1 �� Object �̃T�u�N���X�ł��BZVTestClass1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outZVTestEventArgs1(0) : �쐬���ꂽ ZVTestEventArgs1 �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestEventArgs1 �� Object �̃T�u�N���X�ł��BZVTestEventArgs1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outZVTestEventArgs1(0) : �쐬���ꂽ ZVTestEventArgs1 �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestEventArgs1 �� Object �̃T�u�N���X�ł��BZVTestEventArgs1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  zvtesteventargs1 : ZVTestEventArgs1 �̃n���h��
[out] outReturn(0)     : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ZVTestEventArgs1 �� Object �̃T�u�N���X�ł��BZVTestEventArgs1 �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
                

stat : 0=����, ���l=���s

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


stat : 0=����, ���l=���s

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

stat : 0=����, ���l=���s

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
[out] outAssetModel(0) : �쐬���ꂽ AssetModel �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
AssetModel �� Object �̃T�u�N���X�ł��BAssetModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  assetmodel : AssetModel �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
AssetModel �� Object �̃T�u�N���X�ł��BAssetModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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

stat : 0=����, ���l=���s

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
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAssets_LoadAsset
�w�肵���A�Z�b�g�t�@�C����ǂݍ��݁A�I�u�W�F�N�g�������܂��B
%group
LNAssets
%prm
filePath, outReturn
[in]  filePath  : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst
�t�@�C���̊g���q�� .lnasset �ł��B�������AfilePath �Ɏw�肷��l�͊g���q���ȗ��\�ł��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAssets_ReloadAsset
�w�肵���A�Z�b�g�t�@�C����ǂݍ��݁A�쐬�ς݂̃I�u�W�F�N�g�֓K�p���܂��B
%group
LNAssets
%prm
filePath, obj
[in] filePath : 
[in] obj      : 

stat : 0=����, ���l=���s

%inst
���̃��\�b�h�� Lumino �̌^�V�X�e�����g�p���Ȃ��I�u�W�F�N�g�̓ǂݍ��݂Ɏg�p���܂��B

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
                                              �s���ȕ����G���R�[�f�B���O (���ʎ��s�B�܂��̓o�C�i���t�@�C��)
                                              LN_ENCODING_TYPE_ASCII
                                              ASCII (�g���A�X�L�[�͊܂܂Ȃ� http://www.asciitable.com/)
                                              LN_ENCODING_TYPE_UTF8
                                              UTF-8
                                              LN_ENCODING_TYPE_UTF8N
                                              UTF-8 (BOM ����)
                                              LN_ENCODING_TYPE_SJIS
                                              ���{�� (�V�t�g JIS) -- cp932(MS932) Windows-31J ��MS����
[out] outReturn(0)                       : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_SetVolume
���� Sound �̉��ʂ�ݒ肵�܂��B
%group
LNSound
%prm
sound, value
[in] sound : Sound �̃n���h��
[in] value : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sound �� Object �̃T�u�N���X�ł��BSound �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_GetVolume
���� Sound �̉��ʂ��擾���܂��B
%group
LNSound
%prm
sound, outReturn
[in]  sound     : Sound �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sound �� Object �̃T�u�N���X�ł��BSound �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_SetPitch
���� Sound �̃s�b�` (����) ��ݒ肵�܂��B
%group
LNSound
%prm
sound, value
[in] sound : Sound �̃n���h��
[in] value : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sound �� Object �̃T�u�N���X�ł��BSound �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_GetPitch
���� Sound �̃s�b�` (����) ���擾���܂��B
%group
LNSound
%prm
sound, outReturn
[in]  sound     : Sound �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sound �� Object �̃T�u�N���X�ł��BSound �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_SetLoopEnabled
���[�v�Đ��̗L����ݒ肵�܂��B
%group
LNSound
%prm
sound, enabled
[in] sound   : Sound �̃n���h��
[in] enabled : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sound �� Object �̃T�u�N���X�ł��BSound �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_IsLoopEnabled
���[�v�Đ����L�������m�F���܂��B
%group
LNSound
%prm
sound, outReturn
[in]  sound     : Sound �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sound �� Object �̃T�u�N���X�ł��BSound �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_SetLoopRange
���[�v�͈͂�ݒ肵�܂��B
%group
LNSound
%prm
sound, begin, length
[in] sound  : Sound �̃n���h��
[in] begin  : 
[in] length : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sound �� Object �̃T�u�N���X�ł��BSound �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_Play
���� Sound �̍Đ����J�n���܂��B
%group
LNSound
%prm
sound
[in] sound : Sound �̃n���h��

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sound �� Object �̃T�u�N���X�ł��BSound �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_Stop
���� Sound �̍Đ����~���܂��B
%group
LNSound
%prm
sound
[in] sound : Sound �̃n���h��

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sound �� Object �̃T�u�N���X�ł��BSound �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_Pause
���� Sound �̍Đ����ꎞ��~���܂��B
%group
LNSound
%prm
sound
[in] sound : Sound �̃n���h��

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sound �� Object �̃T�u�N���X�ł��BSound �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_Resume
�ꎞ��~���̍Đ����ĊJ���܂��B
%group
LNSound
%prm
sound
[in] sound : Sound �̃n���h��

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sound �� Object �̃T�u�N���X�ł��BSound �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSound_FadeVolume
���ʂ̃t�F�[�h���J�n���܂��B
%group
LNSound
%prm
sound, targetVolume, time, behavior
[in] sound        : Sound �̃n���h��
[in] targetVolume : 
[in] time         : 
[in] behavior     : 
                       LN_SOUND_FADE_BEHAVIOR_CONTINUE
                       �Đ����p������
                       LN_SOUND_FADE_BEHAVIOR_STOP
                       ��~����
                       LN_SOUND_FADE_BEHAVIOR_STOP_RESET
                       ��~���āA���̍Đ��ɔ����ăT�E���h�̉��ʂ����̒l�ɖ߂�
                       LN_SOUND_FADE_BEHAVIOR_PAUSE
                       �ꎞ��~����
                       LN_SOUND_FADE_BEHAVIOR_PAUSE_RESET
                       �ꎞ��~���āA���̍Đ��ɔ����ăT�E���h�̉��ʂ����̒l�ɖ߂�

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sound �� Object �̃T�u�N���X�ł��BSound �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAudio_PlayBGM
BGM �����t����
%group
LNAudio
%prm
filePath, volume, pitch, fadeTime
[in] filePath           : 
[in] volume(1.000000)   : 
[in] pitch(1.000000)    : 
[in] fadeTime(0.000000) : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAudio_StopBGM
BGM �̉��t���~����
%group
LNAudio
%prm
fadeTime
[in] fadeTime(0.000000) : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAudio_PlayBGS
BGS (����) �����t����
%group
LNAudio
%prm
filePath, volume, pitch, fadeTime
[in] filePath           : 
[in] volume(1.000000)   : 
[in] pitch(1.000000)    : 
[in] fadeTime(0.000000) : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAudio_StopBGS
BGS �̉��t���~����
%group
LNAudio
%prm
fadeTime
[in] fadeTime(0.000000) : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAudio_PlayME
ME (���ʉ��y) �����t����
%group
LNAudio
%prm
filePath, volume, pitch
[in] filePath         : 
[in] volume(1.000000) : 
[in] pitch(1.000000)  : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAudio_StopME
ME �̉��t���~����
%group
LNAudio
%prm


stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAudio_PlaySE
SE �����t����
%group
LNAudio
%prm
filePath, volume, pitch
[in] filePath         : 
[in] volume(1.000000) : 
[in] pitch(1.000000)  : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAudio_PlaySE3D
SE �� 3D ��ԏ�ŉ��t����
%group
LNAudio
%prm
filePath, position, distance, volume, pitch
[in] filePath         : 
[in] position         : (Vector3 �^�̒l) 
[in] distance         : 
[in] volume(1.000000) : 
[in] pitch(1.000000)  : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAudio_StopSE
�S�Ă� SE �̉��t���~����
%group
LNAudio
%prm


stat : 0=����, ���l=���s

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
[out] outTexture2DDelegate(0) : �쐬���ꂽ Texture2DDelegate �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Texture2DDelegate �� Object �̃T�u�N���X�ł��BTexture2DDelegate �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] texture2dpromise : Texture2DPromise �̃n���h��
[in] callback         : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Texture2DPromise �� Object �̃T�u�N���X�ł��BTexture2DPromise �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] texture2dpromise : Texture2DPromise �̃n���h��
[in] callback         : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Texture2DPromise �� Object �̃T�u�N���X�ł��BTexture2DPromise �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outReturn(0) : ���ʂ��i�[����ϐ��B
                     LN_GRAPHICS_API_DEFAULT
                     �����I��
                     LN_GRAPHICS_API_OPEN_GL
                     OpenGL
                     LN_GRAPHICS_API_VULKAN
                     Vulkan
                     LN_GRAPHICS_API_DIRECT_X12
                     Vulkan

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNTexture2D_Create
�e�N�X�`�����쐬���܂��B�s�N�Z���t�H�[�}�b�g�� RGBA8 �ł��B
%group
LNTexture2D
%prm
width, height, outTexture2D
[in]  width        : 
[in]  height       : 
[out] outTexture2D(0) : �쐬���ꂽ Texture2D �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Texture2D �� Texture �̃T�u�N���X�ł��BTexture2D �n���h���� Texture �n���h���Ƃ��Ĉ������Ƃ��ł��A LNTexture_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Texture2D �� AssetObject �̃T�u�N���X�ł��BTexture2D �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Texture2D �� Object �̃T�u�N���X�ł��BTexture2D �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNTexture2D_CreateWithFormat
�e�N�X�`�����쐬���܂��B
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
                        RGBA �I�[�_�[�̊e�v�f 8bit �t�H�[�}�b�g
                        LN_TEXTURE_FORMAT_RGB8
                        RGB �I�[�_�[�̊e�v�f 8bit �t�H�[�}�b�g
                        LN_TEXTURE_FORMAT_RGBA16F
                        RGBA �I�[�_�[�̊e�v�f 16bit ���������_�t�H�[�}�b�g
                        LN_TEXTURE_FORMAT_RGBA32F
                        RGBA �I�[�_�[�̊e�v�f 32bit ���������_�t�H�[�}�b�g
                        LN_TEXTURE_FORMAT_R16F
                        16bit ���������_�t�H�[�}�b�g
                        LN_TEXTURE_FORMAT_R32F
                        32bit ���������_�t�H�[�}�b�g
                        LN_TEXTURE_FORMAT_R32S
                        32bit �̕������萮���t�H�[�}�b�g
[out] outTexture2D(0) : �쐬���ꂽ Texture2D �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Texture2D �� Texture �̃T�u�N���X�ł��BTexture2D �n���h���� Texture �n���h���Ƃ��Ĉ������Ƃ��ł��A LNTexture_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Texture2D �� AssetObject �̃T�u�N���X�ł��BTexture2D �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Texture2D �� Object �̃T�u�N���X�ł��BTexture2D �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNTexture2D_Load
�A�Z�b�g����e�N�X�`����ǂݍ��݂܂��B
%group
LNTexture2D
%prm
filePath, outReturn
[in]  filePath  : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst
�T�|�[�g���Ă���t�H�[�}�b�g�͎��̒ʂ�ł��BPNG(.png), JPG(.jpg), TGA(.tga), BMP(.bmp), GIF(.gif)

���l
--------------------
Texture2D �� Texture �̃T�u�N���X�ł��BTexture2D �n���h���� Texture �n���h���Ƃ��Ĉ������Ƃ��ł��A LNTexture_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Texture2D �� AssetObject �̃T�u�N���X�ł��BTexture2D �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Texture2D �� Object �̃T�u�N���X�ł��BTexture2D �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Texture2D �� Texture �̃T�u�N���X�ł��BTexture2D �n���h���� Texture �n���h���Ƃ��Ĉ������Ƃ��ł��A LNTexture_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Texture2D �� AssetObject �̃T�u�N���X�ł��BTexture2D �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Texture2D �� Object �̃T�u�N���X�ł��BTexture2D �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outReturn(0)             : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Shader �� AssetObject �̃T�u�N���X�ł��BShader �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Shader �� Object �̃T�u�N���X�ł��BShader �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNShader_SetFloat
���������_�l��ݒ肵�܂��B
%group
LNShader
%prm
shader, parameterName, value
[in] shader        : Shader �̃n���h��
[in] parameterName : 
[in] value         : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Shader �� AssetObject �̃T�u�N���X�ł��BShader �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Shader �� Object �̃T�u�N���X�ł��BShader �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNShader_SetVector3
�x�N�g���l��ݒ肵�܂��B
%group
LNShader
%prm
shader, parameterName, value
[in] shader        : Shader �̃n���h��
[in] parameterName : 
[in] value         : (Vector3 �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Shader �� AssetObject �̃T�u�N���X�ł��BShader �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Shader �� Object �̃T�u�N���X�ł��BShader �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNShader_SetVector4
�x�N�g���l��ݒ肵�܂��B
%group
LNShader
%prm
shader, parameterName, value
[in] shader        : Shader �̃n���h��
[in] parameterName : 
[in] value         : (Vector4 �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Shader �� AssetObject �̃T�u�N���X�ł��BShader �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Shader �� Object �̃T�u�N���X�ł��BShader �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] shader        : Shader �̃n���h��
[in] parameterName : 
[in] value         : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Shader �� AssetObject �̃T�u�N���X�ł��BShader �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Shader �� Object �̃T�u�N���X�ł��BShader �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outMaterial(0) : �쐬���ꂽ Material �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Material �� Object �̃T�u�N���X�ł��BMaterial �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] material : Material �̃n���h��
[in] value    : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Material �� Object �̃T�u�N���X�ł��BMaterial �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  material  : Material �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Material �� Object �̃T�u�N���X�ł��BMaterial �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] material : Material �̃n���h��
[in] value    : (Color �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Material �� Object �̃T�u�N���X�ł��BMaterial �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] material : Material �̃n���h��
[in] value    : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Material �� Object �̃T�u�N���X�ł��BMaterial �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] material : Material �̃n���h��
[in] value    : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Material �� Object �̃T�u�N���X�ł��BMaterial �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] material : Material �̃n���h��
[in] value    : (Color �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Material �� Object �̃T�u�N���X�ł��BMaterial �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] material : Material �̃n���h��
[in] value    : 
                   LN_SHADING_MODEL_DEFAULT
                   Default
                   LN_SHADING_MODEL_UNLIT
                   Unlit

stat : 0=����, ���l=���s

%inst


���l
--------------------
Material �� Object �̃T�u�N���X�ł��BMaterial �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  material  : Material �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B
                     LN_SHADING_MODEL_DEFAULT
                     Default
                     LN_SHADING_MODEL_UNLIT
                     Unlit

stat : 0=����, ���l=���s

%inst


���l
--------------------
Material �� Object �̃T�u�N���X�ł��BMaterial �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] material : Material �̃n���h��
[in] shader   : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Material �� Object �̃T�u�N���X�ł��BMaterial �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  material  : Material �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Material �� Object �̃T�u�N���X�ł��BMaterial �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshNode_SetVisible
����Ԃ�ݒ肵�܂��Bfalse �̏ꍇ�A���̃��b�V���̕`��͍s���܂���B(default: true)
%group
LNMeshNode
%prm
meshnode, value
[in] meshnode : MeshNode �̃n���h��
[in] value    : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
MeshNode �� Object �̃T�u�N���X�ł��BMeshNode �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshNode_IsVisible
����Ԃ��擾���܂��B
%group
LNMeshNode
%prm
meshnode, outReturn
[in]  meshnode  : MeshNode �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
MeshNode �� Object �̃T�u�N���X�ł��BMeshNode �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAnimationController_AddClip
�A�j���[�V�����N���b�v��ǉ����܂��B (���C���[0 �֒ǉ�����܂�)
%group
LNAnimationController
%prm
animationcontroller, animationClip, outReturn
[in]  animationcontroller : AnimationController �̃n���h��
[in]  animationClip       : 
[out] outReturn(0)        : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
AnimationController �� Object �̃T�u�N���X�ł��BAnimationController �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] animationcontroller : AnimationController �̃n���h��
[in] state               : 
[in] duration(0.300000)  : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
AnimationController �� Object �̃T�u�N���X�ł��BAnimationController �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outReturn(0)             : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
MeshModel �� Object �̃T�u�N���X�ł��BMeshModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  meshmodel : MeshModel �̃n���h��
[in]  name      : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
MeshModel �� Object �̃T�u�N���X�ł��BMeshModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  meshmodel : MeshModel �̃n���h��
[in]  name      : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
MeshModel �� Object �̃T�u�N���X�ł��BMeshModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  meshmodel : MeshModel �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
MeshModel �� Object �̃T�u�N���X�ł��BMeshModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  meshmodel : MeshModel �̃n���h��
[in]  index     : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
MeshModel �� Object �̃T�u�N���X�ł��BMeshModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  meshmodel : MeshModel �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
MeshModel �� Object �̃T�u�N���X�ł��BMeshModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outMeshImportSettings(0) : �쐬���ꂽ MeshImportSettings �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
MeshImportSettings �� Object �̃T�u�N���X�ł��BMeshImportSettings �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  size                 : (Vector3 �^�̒l) 
[out] outBoxCollisionShape(0) : �쐬���ꂽ BoxCollisionShape �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
BoxCollisionShape �� CollisionShape �̃T�u�N���X�ł��BBoxCollisionShape �n���h���� CollisionShape �n���h���Ƃ��Ĉ������Ƃ��ł��A LNCollisionShape_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
BoxCollisionShape �� Object �̃T�u�N���X�ł��BBoxCollisionShape �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outBoxCollisionShape(0) : �쐬���ꂽ BoxCollisionShape �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
BoxCollisionShape �� CollisionShape �̃T�u�N���X�ł��BBoxCollisionShape �n���h���� CollisionShape �n���h���Ƃ��Ĉ������Ƃ��ł��A LNCollisionShape_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
BoxCollisionShape �� Object �̃T�u�N���X�ł��BBoxCollisionShape �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAnimationCurve_Evaluate
�w�肵�����Ԃɂ�����l��]�����܂��B
%group
LNAnimationCurve
%prm
animationcurve, time, outReturn
[in]  animationcurve : AnimationCurve �̃n���h��
[in]  time           : 
[out] outReturn(0)   : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
AnimationCurve �� Object �̃T�u�N���X�ł��BAnimationCurve �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outKeyFrameAnimationCurve(0) : �쐬���ꂽ KeyFrameAnimationCurve �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
KeyFrameAnimationCurve �� AnimationCurve �̃T�u�N���X�ł��BKeyFrameAnimationCurve �n���h���� AnimationCurve �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAnimationCurve_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
KeyFrameAnimationCurve �� Object �̃T�u�N���X�ł��BKeyFrameAnimationCurve �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNKeyFrameAnimationCurve_AddKeyFrame
�L�[�t���[����ǉ����܂��B
%group
LNKeyFrameAnimationCurve
%prm
keyframeanimationcurve, time, value, rightTangentMode, tangent
[in] keyframeanimationcurve                   : KeyFrameAnimationCurve �̃n���h��
[in] time                                     : 
[in] value                                    : 
[in] rightTangentMode(LN_TANGENT_MODE_LINEAR) : 
                                                   LN_TANGENT_MODE_LINEAR
                                                   ���`���
                                                   LN_TANGENT_MODE_TANGENT
                                                   �ڐ� (���x) ���g�p������� (�G���~�[�g�X�v���C��)
                                                   LN_TANGENT_MODE_AUTO
                                                   �L�[�t���[���̒l��ʉ߂���Ȃ߂炩�ȕ�� (Catmull-Rom)
                                                   LN_TANGENT_MODE_CONSTANT
                                                   ��ԂȂ�
[in] tangent(0.000000)                        : 

stat : 0=����, ���l=���s

%inst
rightTangentMode �́A�V�����ǉ�����L�[�t���[���̉E���̕�ԕ��@�ł��B�V�����ǉ�����L�[�t���[���̍����̕ۊǕ��@�́A���̂ЂƂO�̃L�[�t���[���̉E���̕ۊǕ��@���ݒ肳��܂��B

���l
--------------------
KeyFrameAnimationCurve �� AnimationCurve �̃T�u�N���X�ł��BKeyFrameAnimationCurve �n���h���� AnimationCurve �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAnimationCurve_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
KeyFrameAnimationCurve �� Object �̃T�u�N���X�ł��BKeyFrameAnimationCurve �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
AnimationClip �� AssetObject �̃T�u�N���X�ł��BAnimationClip �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
AnimationClip �� Object �̃T�u�N���X�ł��BAnimationClip �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAnimationClip_SetWrapMode
�A�j���[�V�����̌J��Ԃ��̓�����擾���܂��B(default: Loop)
%group
LNAnimationClip
%prm
animationclip, value
[in] animationclip : AnimationClip �̃n���h��
[in] value         : 
                        LN_ANIMATION_WRAP_MODE_ONCE
                        �J��Ԃ����s�킸�A1�x�����Đ����܂��B
                        LN_ANIMATION_WRAP_MODE_LOOP
                        �Ō�܂ōĐ����ꂽ��A�擪�ɖ߂��ă��[�v���܂��B
                        LN_ANIMATION_WRAP_MODE_ALTERNATE
                        �Ō�܂ōĐ����ꂽ��A�t�����ɖ߂��ă��[�v���܂��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
AnimationClip �� AssetObject �̃T�u�N���X�ł��BAnimationClip �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
AnimationClip �� Object �̃T�u�N���X�ł��BAnimationClip �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAnimationClip_GetWrapMode
�A�j���[�V�����̌J��Ԃ��̓�����擾���܂��B
%group
LNAnimationClip
%prm
animationclip, outReturn
[in]  animationclip : AnimationClip �̃n���h��
[out] outReturn(0)  : ���ʂ��i�[����ϐ��B
                         LN_ANIMATION_WRAP_MODE_ONCE
                         �J��Ԃ����s�킸�A1�x�����Đ����܂��B
                         LN_ANIMATION_WRAP_MODE_LOOP
                         �Ō�܂ōĐ����ꂽ��A�擪�ɖ߂��ă��[�v���܂��B
                         LN_ANIMATION_WRAP_MODE_ALTERNATE
                         �Ō�܂ōĐ����ꂽ��A�t�����ɖ߂��ă��[�v���܂��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
AnimationClip �� AssetObject �̃T�u�N���X�ł��BAnimationClip �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
AnimationClip �� Object �̃T�u�N���X�ł��BAnimationClip �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAnimationClip_SetHierarchicalAnimationMode
�K�w�\�������A�j���[�V�����f�[�^�̓��샂�[�h�B(default: AllowTranslationOnlyRoot)
%group
LNAnimationClip
%prm
animationclip, value
[in] animationclip : AnimationClip �̃n���h��
[in] value         : 
                        LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT
                        ���[�g�m�[�h�̂݁A���s�ړ���L�������܂��B
                        LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT_Y
                        AllowTranslationOnlyRootY
                        LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION
                        ���ׂẴm�[�h�̕��s�ړ���L�������܂��B
                        LN_HIERARCHICAL_ANIMATION_MODE_DISABLE_TRANSLATION
                        ���s�ړ��𖳌������܂��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
AnimationClip �� AssetObject �̃T�u�N���X�ł��BAnimationClip �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
AnimationClip �� Object �̃T�u�N���X�ł��BAnimationClip �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNAnimationClip_GetHierarchicalAnimationMode
�K�w�\�������A�j���[�V�����f�[�^�̓��샂�[�h�B
%group
LNAnimationClip
%prm
animationclip, outReturn
[in]  animationclip : AnimationClip �̃n���h��
[out] outReturn(0)  : ���ʂ��i�[����ϐ��B
                         LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT
                         ���[�g�m�[�h�̂݁A���s�ړ���L�������܂��B
                         LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT_Y
                         AllowTranslationOnlyRootY
                         LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION
                         ���ׂẴm�[�h�̕��s�ړ���L�������܂��B
                         LN_HIERARCHICAL_ANIMATION_MODE_DISABLE_TRANSLATION
                         ���s�ړ��𖳌������܂��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
AnimationClip �� AssetObject �̃T�u�N���X�ł��BAnimationClip �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
AnimationClip �� Object �̃T�u�N���X�ł��BAnimationClip �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outParticleEmitterModel(0) : �쐬���ꂽ ParticleEmitterModel �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ParticleEmitterModel �� Object �̃T�u�N���X�ł��BParticleEmitterModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterModel_SetMaxParticles
�����ɕ\���ł���p�[�e�B�N���̍ő吔��ݒ肵�܂��B(default: 100)
%group
LNParticleEmitterModel
%prm
particleemittermodel, count
[in] particleemittermodel : ParticleEmitterModel �̃n���h��
[in] count                : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ParticleEmitterModel �� Object �̃T�u�N���X�ł��BParticleEmitterModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterModel_SetSpawnRate
1�b�Ԃɕ��o����p�[�e�B�N���̐���ݒ肵�܂��B(default: 1)
%group
LNParticleEmitterModel
%prm
particleemittermodel, rate
[in] particleemittermodel : ParticleEmitterModel �̃n���h��
[in] rate                 : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ParticleEmitterModel �� Object �̃T�u�N���X�ł��BParticleEmitterModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterModel_SetLifeTime
�p�[�e�B�N���̐������Ԃ�ݒ肵�܂��B(default: 5.0)
%group
LNParticleEmitterModel
%prm
particleemittermodel, time
[in] particleemittermodel : ParticleEmitterModel �̃n���h��
[in] time                 : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ParticleEmitterModel �� Object �̃T�u�N���X�ł��BParticleEmitterModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] particleemittermodel : ParticleEmitterModel �̃n���h��
[in] size                 : (Vector3 �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ParticleEmitterModel �� Object �̃T�u�N���X�ł��BParticleEmitterModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] particleemittermodel : ParticleEmitterModel �̃n���h��
[in] value                : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ParticleEmitterModel �� Object �̃T�u�N���X�ł��BParticleEmitterModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] particleemittermodel : ParticleEmitterModel �̃n���h��
[in] value                : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ParticleEmitterModel �� Object �̃T�u�N���X�ł��BParticleEmitterModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] particleemittermodel : ParticleEmitterModel �̃n���h��
[in] value                : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ParticleEmitterModel �� Object �̃T�u�N���X�ł��BParticleEmitterModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] particleemittermodel : ParticleEmitterModel �̃n���h��
[in] value                : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ParticleEmitterModel �� Object �̃T�u�N���X�ł��BParticleEmitterModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] particleemittermodel : ParticleEmitterModel �̃n���h��
[in] value                : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ParticleEmitterModel �� Object �̃T�u�N���X�ł��BParticleEmitterModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNParticleEmitterModel_SetForwardScale
�i�s�����ɑ΂���X�P�[���l�B�ʏ�AZ���BParticleGeometryDirection::ToView �ł� Y scale (default: 1.0)
%group
LNParticleEmitterModel
%prm
particleemittermodel, value
[in] particleemittermodel : ParticleEmitterModel �̃n���h��
[in] value                : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ParticleEmitterModel �� Object �̃T�u�N���X�ł��BParticleEmitterModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] particleemittermodel : ParticleEmitterModel �̃n���h��
[in] value                : 
                               LN_PARTICLE_GEOMETRY_DIRECTION_TO_VIEW
                               ��Ɏ��_����������
                               LN_PARTICLE_GEOMETRY_DIRECTION_TOP
                               �i�s���������ɁA�\�� (Y+) �����_���猩����悤�ɂ���
                               LN_PARTICLE_GEOMETRY_DIRECTION_VERTICAL_BILLBOARD
                               Y �������ɒ������A�J�����̕��������܂��B
                               LN_PARTICLE_GEOMETRY_DIRECTION_HORIZONTAL_BILLBOARD
                               XZ ����́u���v���ʂƕ��s�ɂȂ�܂��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ParticleEmitterModel �� Object �̃T�u�N���X�ł��BParticleEmitterModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] particleemittermodel : ParticleEmitterModel �̃n���h��
[in] material             : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ParticleEmitterModel �� Object �̃T�u�N���X�ł��BParticleEmitterModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outParticleModel(0) : �쐬���ꂽ ParticleModel �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ParticleModel �� EffectResource �̃T�u�N���X�ł��BParticleModel �n���h���� EffectResource �n���h���Ƃ��Ĉ������Ƃ��ł��A LNEffectResource_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
ParticleModel �� AssetObject �̃T�u�N���X�ł��BParticleModel �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
ParticleModel �� Object �̃T�u�N���X�ł��BParticleModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] particlemodel : ParticleModel �̃n���h��
[in] value         : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ParticleModel �� EffectResource �̃T�u�N���X�ł��BParticleModel �n���h���� EffectResource �n���h���Ƃ��Ĉ������Ƃ��ł��A LNEffectResource_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
ParticleModel �� AssetObject �̃T�u�N���X�ł��BParticleModel �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
ParticleModel �� Object �̃T�u�N���X�ł��BParticleModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  particlemodel : ParticleModel �̃n���h��
[out] outReturn(0)  : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ParticleModel �� EffectResource �̃T�u�N���X�ł��BParticleModel �n���h���� EffectResource �n���h���Ƃ��Ĉ������Ƃ��ł��A LNEffectResource_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
ParticleModel �� AssetObject �̃T�u�N���X�ł��BParticleModel �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
ParticleModel �� Object �̃T�u�N���X�ł��BParticleModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] particlemodel : ParticleModel �̃n���h��
[in] emitter       : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
ParticleModel �� EffectResource �̃T�u�N���X�ł��BParticleModel �n���h���� EffectResource �n���h���Ƃ��Ĉ������Ƃ��ł��A LNEffectResource_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
ParticleModel �� AssetObject �̃T�u�N���X�ł��BParticleModel �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
ParticleModel �� Object �̃T�u�N���X�ł��BParticleModel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVisualComponent_SetVisible
����Ԃ�ݒ肵�܂��Bfalse �̏ꍇ�A�R���|�[�l���g�̕`��͍s���܂���B(default: true)
%group
LNVisualComponent
%prm
visualcomponent, value
[in] visualcomponent : VisualComponent �̃n���h��
[in] value           : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
VisualComponent �� Component �̃T�u�N���X�ł��BVisualComponent �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
VisualComponent �� Object �̃T�u�N���X�ł��BVisualComponent �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVisualComponent_IsVisible
����Ԃ��擾���܂��B
%group
LNVisualComponent
%prm
visualcomponent, outReturn
[in]  visualcomponent : VisualComponent �̃n���h��
[out] outReturn(0)    : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
VisualComponent �� Component �̃T�u�N���X�ł��BVisualComponent �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
VisualComponent �� Object �̃T�u�N���X�ł��BVisualComponent �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpriteComponent_SetTexture
�X�v���C�g���\������e�N�X�`����ݒ肵�܂��B
%group
LNSpriteComponent
%prm
spritecomponent, texture
[in] spritecomponent : SpriteComponent �̃n���h��
[in] texture         : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
SpriteComponent �� VisualComponent �̃T�u�N���X�ł��BSpriteComponent �n���h���� VisualComponent �n���h���Ƃ��Ĉ������Ƃ��ł��A LNVisualComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
SpriteComponent �� Component �̃T�u�N���X�ł��BSpriteComponent �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
SpriteComponent �� Object �̃T�u�N���X�ł��BSpriteComponent �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outCollisionEventHandler(0) : �쐬���ꂽ CollisionEventHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
CollisionEventHandler �� Object �̃T�u�N���X�ł��BCollisionEventHandler �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_Create
CharacterController ���쐬���܂��B
%group
LNCharacterController
%prm
outCharacterController
[out] outCharacterController(0) : �쐬���ꂽ CharacterController �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
CharacterController �� Component �̃T�u�N���X�ł��BCharacterController �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
CharacterController �� Object �̃T�u�N���X�ł��BCharacterController �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] charactercontroller : CharacterController �̃n���h��
[in] value               : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
CharacterController �� Component �̃T�u�N���X�ł��BCharacterController �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
CharacterController �� Object �̃T�u�N���X�ł��BCharacterController �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  charactercontroller : CharacterController �̃n���h��
[out] outReturn(0)        : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
CharacterController �� Component �̃T�u�N���X�ł��BCharacterController �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
CharacterController �� Object �̃T�u�N���X�ł��BCharacterController �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] charactercontroller : CharacterController �̃n���h��
[in] value               : (Vector3 �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
CharacterController �� Component �̃T�u�N���X�ł��BCharacterController �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
CharacterController �� Object �̃T�u�N���X�ł��BCharacterController �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  charactercontroller : CharacterController �̃n���h��
[out] outReturn(0)        : (Vector3 �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
CharacterController �� Component �̃T�u�N���X�ł��BCharacterController �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
CharacterController �� Object �̃T�u�N���X�ł��BCharacterController �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_SetInputControlEnabled
�L�[�{�[�h��Q�[���p�b�h�ɂ�鑀��̗L����Ԃ�ݒ肵�܂��B (default: true)
%group
LNCharacterController
%prm
charactercontroller, value
[in] charactercontroller : CharacterController �̃n���h��
[in] value               : 

stat : 0=����, ���l=���s

%inst
false ���w�肵���ꍇ�A�L�����N�^�[�̑z��O�̎�����h�~���邽�߁A���x�� 0 �Ƀ��Z�b�g����܂��B

���l
--------------------
CharacterController �� Component �̃T�u�N���X�ł��BCharacterController �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
CharacterController �� Object �̃T�u�N���X�ł��BCharacterController �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_SetCameraControlEnabled
�}�E�X�ɂ��J��������̗L����Ԃ�ݒ肵�܂��B (default: true)
%group
LNCharacterController
%prm
charactercontroller, value
[in] charactercontroller : CharacterController �̃n���h��
[in] value               : 

stat : 0=����, ���l=���s

%inst
�L���ł���ꍇ�A�֘A�t�����Ă���J������ʂ��āA�`���ƂȂ�r���[�� MouseGrab ���擾���܂��B�܂�A�}�E�X�J�[�\���͔�\���ƂȂ� UI ���|�C���g���ăN���b�N���铙�̑���͂ł��Ȃ��Ȃ�܂��B

���l
--------------------
CharacterController �� Component �̃T�u�N���X�ł��BCharacterController �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
CharacterController �� Object �̃T�u�N���X�ł��BCharacterController �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_SetHeight
�L�����N�^�[�̍�����ݒ肵�܂��B���̒l�̓J�v�Z���R���C�_�[�̍�����A�J�����̒����_�Ƃ��Ďg�p����܂��B (default: 2.0)
%group
LNCharacterController
%prm
charactercontroller, value
[in] charactercontroller : CharacterController �̃n���h��
[in] value               : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
CharacterController �� Component �̃T�u�N���X�ł��BCharacterController �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
CharacterController �� Object �̃T�u�N���X�ł��BCharacterController �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_GetHeight
�L�����N�^�[�̍������擾���܂��B
%group
LNCharacterController
%prm
charactercontroller, outReturn
[in]  charactercontroller : CharacterController �̃n���h��
[out] outReturn(0)        : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
CharacterController �� Component �̃T�u�N���X�ł��BCharacterController �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
CharacterController �� Object �̃T�u�N���X�ł��BCharacterController �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_SetCameraRadius
�L�����N�^�[�ƃJ�����̋�����ݒ肵�܂��B(default: 5.0)
%group
LNCharacterController
%prm
charactercontroller, value
[in] charactercontroller : CharacterController �̃n���h��
[in] value               : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
CharacterController �� Component �̃T�u�N���X�ł��BCharacterController �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
CharacterController �� Object �̃T�u�N���X�ł��BCharacterController �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_GetCameraRadius
�L�����N�^�[�ƃJ�����̋������擾���܂��B
%group
LNCharacterController
%prm
charactercontroller, outReturn
[in]  charactercontroller : CharacterController �̃n���h��
[out] outReturn(0)        : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
CharacterController �� Component �̃T�u�N���X�ł��BCharacterController �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
CharacterController �� Object �̃T�u�N���X�ł��BCharacterController �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_SetCollisionEnter
CollisionEnter �C�x���g�̒ʒm���󂯎��R�[���o�b�N��o�^���܂��B
%group
LNCharacterController
%prm
charactercontroller, handler
[in] charactercontroller : CharacterController �̃n���h��
[in] handler             : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
CharacterController �� Component �̃T�u�N���X�ł��BCharacterController �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
CharacterController �� Object �̃T�u�N���X�ł��BCharacterController �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_SetCollisionLeave
CollisionLeave �C�x���g�̒ʒm���󂯎��R�[���o�b�N��o�^���܂��B
%group
LNCharacterController
%prm
charactercontroller, handler
[in] charactercontroller : CharacterController �̃n���h��
[in] handler             : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
CharacterController �� Component �̃T�u�N���X�ł��BCharacterController �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
CharacterController �� Object �̃T�u�N���X�ł��BCharacterController �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCharacterController_SetCollisionStay
CollisionStay �C�x���g�̒ʒm���󂯎��R�[���o�b�N��o�^���܂��B
%group
LNCharacterController
%prm
charactercontroller, handler
[in] charactercontroller : CharacterController �̃n���h��
[in] handler             : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
CharacterController �� Component �̃T�u�N���X�ł��BCharacterController �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
CharacterController �� Object �̃T�u�N���X�ł��BCharacterController �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorld_Add
�I�u�W�F�N�g�� World �ɒǉ����܂��B
%group
LNWorld
%prm
world, obj
[in] world : World �̃n���h��
[in] obj   : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
World �� Object �̃T�u�N���X�ł��BWorld �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  componentlist : ComponentList �̃n���h��
[out] outReturn(0)  : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ComponentList �� Object �̃T�u�N���X�ł��BComponentList �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  componentlist : ComponentList �̃n���h��
[in]  index         : 
[out] outReturn(0)  : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ComponentList �� Object �̃T�u�N���X�ł��BComponentList �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_Create
WorldObject ���쐬���܂��B
%group
LNWorldObject
%prm
outWorldObject
[out] outWorldObject(0) : �쐬���ꂽ WorldObject �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetPosition
���̃I�u�W�F�N�g�̈ʒu��ݒ肵�܂��B
%group
LNWorldObject
%prm
worldobject, pos
[in] worldobject : WorldObject �̃n���h��
[in] pos         : (Vector3 �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetPositionXYZ
���̃I�u�W�F�N�g�̈ʒu��ݒ肵�܂��B
%group
LNWorldObject
%prm
worldobject, x, y, z
[in] worldobject : WorldObject �̃n���h��
[in] x           : 
[in] y           : 
[in] z(0.000000) : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_GetPosition
���̃I�u�W�F�N�g�̈ʒu���ʒu���擾���܂��B
%group
LNWorldObject
%prm
worldobject, outReturn
[in]  worldobject : WorldObject �̃n���h��
[out] outReturn(0) : (Vector3 �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetRotation
���̃I�u�W�F�N�g�̉�]��ݒ肵�܂��B
%group
LNWorldObject
%prm
worldobject, rot
[in] worldobject : WorldObject �̃n���h��
[in] rot         : (Quaternion �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetRotationXYZ
���̃I�u�W�F�N�g�̉�]���I�C���[�p����ݒ肵�܂�(radian�P��) �B��]������ Z(Roll) > X(Pich) > Y(Yaw) �ł��B
%group
LNWorldObject
%prm
worldobject, x, y, z
[in] worldobject : WorldObject �̃n���h��
[in] x           : 
[in] y           : 
[in] z           : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_GetRotation
���̃I�u�W�F�N�g�̉�]���擾���܂��B
%group
LNWorldObject
%prm
worldobject, outReturn
[in]  worldobject : WorldObject �̃n���h��
[out] outReturn(0) : (Quaternion �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetScale
���̃I�u�W�F�N�g�̊g�嗦��ݒ肵�܂��B
%group
LNWorldObject
%prm
worldobject, scale
[in] worldobject : WorldObject �̃n���h��
[in] scale       : (Vector3 �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetScaleS
���̃I�u�W�F�N�g�̊g�嗦��ݒ肵�܂��B
%group
LNWorldObject
%prm
worldobject, xyz
[in] worldobject : WorldObject �̃n���h��
[in] xyz         : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetScaleXYZ
���̃I�u�W�F�N�g�̊g�嗦��ݒ肵�܂��B
%group
LNWorldObject
%prm
worldobject, x, y, z
[in] worldobject : WorldObject �̃n���h��
[in] x           : 
[in] y           : 
[in] z(1.000000) : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_GetScale
���̃I�u�W�F�N�g�̊g�嗦���擾���܂��B
%group
LNWorldObject
%prm
worldobject, outReturn
[in]  worldobject : WorldObject �̃n���h��
[out] outReturn(0) : (Vector3 �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetCenterPoint
���̃I�u�W�F�N�g�̃��[�J���̒��S�ʒu��ݒ肵�܂��B
%group
LNWorldObject
%prm
worldobject, value
[in] worldobject : WorldObject �̃n���h��
[in] value       : (Vector3 �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetCenterPointXYZ
���̃I�u�W�F�N�g�̃��[�J���̒��S�ʒu��ݒ肵�܂��B
%group
LNWorldObject
%prm
worldobject, x, y, z
[in] worldobject : WorldObject �̃n���h��
[in] x           : 
[in] y           : 
[in] z(0.000000) : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_GetCenterPoint
���̃I�u�W�F�N�g�̃��[�J���̒��S�ʒu���擾���܂��B
%group
LNWorldObject
%prm
worldobject, outReturn
[in]  worldobject : WorldObject �̃n���h��
[out] outReturn(0) : (Vector3 �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_LookAt
�w�肵�����W�������悤�ɁA�I�u�W�F�N�g����]�����܂��B
%group
LNWorldObject
%prm
worldobject, target
[in] worldobject : WorldObject �̃n���h��
[in] target      : (Vector3 �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_LookAtXYZ
�w�肵�����W�������悤�ɁA�I�u�W�F�N�g����]�����܂��B
%group
LNWorldObject
%prm
worldobject, x, y, z
[in] worldobject : WorldObject �̃n���h��
[in] x           : 
[in] y           : 
[in] z           : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_AddComponent
Component ��ǉ����܂��B
%group
LNWorldObject
%prm
worldobject, component
[in] worldobject : WorldObject �̃n���h��
[in] component   : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_RemoveComponent
Component �����O���܂��B
%group
LNWorldObject
%prm
worldobject, component
[in] worldobject : WorldObject �̃n���h��
[in] component   : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_AddTag
�^�O��ǉ����܂��B
%group
LNWorldObject
%prm
worldobject, tag
[in] worldobject : WorldObject �̃n���h��
[in] tag         : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_RemoveTag
�^�O�����O���܂��B
%group
LNWorldObject
%prm
worldobject, tag
[in] worldobject : WorldObject �̃n���h��
[in] tag         : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_HasTag
�w�肵���^�O�������Ă��邩���m�F���܂��B
%group
LNWorldObject
%prm
worldobject, tag, outReturn
[in]  worldobject : WorldObject �̃n���h��
[in]  tag         : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_Destroy
WorldObject ��j�����܂��B
%group
LNWorldObject
%prm
worldobject
[in] worldobject : WorldObject �̃n���h��

stat : 0=����, ���l=���s

%inst
���ۂ̍폜�́A���݂̃t���[���̃A�b�v�f�[�g������ɍs���܂��B�폜���ꂽ WorldObject �́A�e�� World, Level, WorldObject ��������O����܂��B

���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  worldobject : WorldObject �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_AddInto
���� WorldObject ���w�肵�� World �֒ǉ����܂��B�ȗ������ꍇ�̓f�t�H���g�� World �֒ǉ����܂��B
%group
LNWorldObject
%prm
worldobject, world
[in] worldobject           : WorldObject �̃n���h��
[in] world(LN_NULL_HANDLE) : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldObject �� Object �̃T�u�N���X�ł��BWorldObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVisualObject_SetVisible
����Ԃ�ݒ肵�܂��Bfalse �̏ꍇ�A�R���|�[�l���g�̕`��͍s���܂���B(default: true)
%group
LNVisualObject
%prm
visualobject, value
[in] visualobject : VisualObject �̃n���h��
[in] value        : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
VisualObject �� WorldObject �̃T�u�N���X�ł��BVisualObject �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
VisualObject �� Object �̃T�u�N���X�ł��BVisualObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVisualObject_IsVisible
����Ԃ��擾���܂��B
%group
LNVisualObject
%prm
visualobject, outReturn
[in]  visualobject : VisualObject �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
VisualObject �� WorldObject �̃T�u�N���X�ł��BVisualObject �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
VisualObject �� Object �̃T�u�N���X�ł��BVisualObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVisualObject_SetBlendMode2
�������@��ݒ肵�܂��B(default: BlendMode::Normal)
%group
LNVisualObject
%prm
visualobject, value
[in] visualobject : VisualObject �̃n���h��
[in] value        : 
                       LN_BLEND_MODE_NORMAL
                       �ʏ�
                       LN_BLEND_MODE_ALPHA
                       �A���t�@�u�����h (RGB ���A���t�@�u�����h���AA �����Z����)
                       LN_BLEND_MODE_ADD
                       ���Z����
                       LN_BLEND_MODE_SUBTRACT
                       ���Z����
                       LN_BLEND_MODE_MULTIPLY
                       ��Z����

stat : 0=����, ���l=���s

%inst


���l
--------------------
VisualObject �� WorldObject �̃T�u�N���X�ł��BVisualObject �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
VisualObject �� Object �̃T�u�N���X�ł��BVisualObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVisualObject_SetOpacity
�s�����x��ݒ肵�܂��B(default: 1.0)
%group
LNVisualObject
%prm
visualobject, value
[in] visualobject : VisualObject �̃n���h��
[in] value        : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
VisualObject �� WorldObject �̃T�u�N���X�ł��BVisualObject �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
VisualObject �� Object �̃T�u�N���X�ł��BVisualObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNVisualObject_GetOpacity
�s�����x���擾���܂��B
%group
LNVisualObject
%prm
visualobject, outReturn
[in]  visualobject : VisualObject �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
VisualObject �� WorldObject �̃T�u�N���X�ł��BVisualObject �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
VisualObject �� Object �̃T�u�N���X�ł��BVisualObject �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_SetEnabled
���C�g�̗L����Ԃ�ݒ肵�܂��Bfalse �̏ꍇ�A���C�g�̓V�[���ɉe�����܂���B(default: true)
%group
LNEnvironmentLight
%prm
environmentlight, enabled
[in] environmentlight : EnvironmentLight �̃n���h��
[in] enabled          : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
EnvironmentLight �� WorldObject �̃T�u�N���X�ł��BEnvironmentLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
EnvironmentLight �� Object �̃T�u�N���X�ł��BEnvironmentLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_IsEnabled
���C�g�̗L����Ԃ��擾���܂��B
%group
LNEnvironmentLight
%prm
environmentlight, outReturn
[in]  environmentlight : EnvironmentLight �̃n���h��
[out] outReturn(0)     : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
EnvironmentLight �� WorldObject �̃T�u�N���X�ł��BEnvironmentLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
EnvironmentLight �� Object �̃T�u�N���X�ł��BEnvironmentLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_SetColor
�f�B���N�V���i�����C�g�̌����F��ݒ肵�܂��B(default: White)
%group
LNEnvironmentLight
%prm
environmentlight, value
[in] environmentlight : EnvironmentLight �̃n���h��
[in] value            : (Color �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
EnvironmentLight �� WorldObject �̃T�u�N���X�ł��BEnvironmentLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
EnvironmentLight �� Object �̃T�u�N���X�ł��BEnvironmentLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_GetColor
�f�B���N�V���i�����C�g�̌����F���擾���܂��B
%group
LNEnvironmentLight
%prm
environmentlight, outReturn
[in]  environmentlight : EnvironmentLight �̃n���h��
[out] outReturn(0)     : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
EnvironmentLight �� WorldObject �̃T�u�N���X�ł��BEnvironmentLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
EnvironmentLight �� Object �̃T�u�N���X�ł��BEnvironmentLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_SetAmbientColor
�V�[���S�̂̊����̐F��ݒ肵�܂��B(default: 0.5, 0.5, 0.5)
%group
LNEnvironmentLight
%prm
environmentlight, value
[in] environmentlight : EnvironmentLight �̃n���h��
[in] value            : (Color �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
EnvironmentLight �� WorldObject �̃T�u�N���X�ł��BEnvironmentLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
EnvironmentLight �� Object �̃T�u�N���X�ł��BEnvironmentLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_GetAmbientColor
�V�[���S�̂̊����̐F���擾���܂��B
%group
LNEnvironmentLight
%prm
environmentlight, outReturn
[in]  environmentlight : EnvironmentLight �̃n���h��
[out] outReturn(0)     : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
EnvironmentLight �� WorldObject �̃T�u�N���X�ł��BEnvironmentLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
EnvironmentLight �� Object �̃T�u�N���X�ł��BEnvironmentLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_GetSkyColor
��̊����̐F���擾���܂��B
%group
LNEnvironmentLight
%prm
environmentlight, outReturn
[in]  environmentlight : EnvironmentLight �̃n���h��
[out] outReturn(0)     : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
EnvironmentLight �� WorldObject �̃T�u�N���X�ł��BEnvironmentLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
EnvironmentLight �� Object �̃T�u�N���X�ł��BEnvironmentLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_SetSkyColor
��̊����̐F��ݒ肵�܂��B(default: Black)
%group
LNEnvironmentLight
%prm
environmentlight, value
[in] environmentlight : EnvironmentLight �̃n���h��
[in] value            : (Color �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
EnvironmentLight �� WorldObject �̃T�u�N���X�ł��BEnvironmentLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
EnvironmentLight �� Object �̃T�u�N���X�ł��BEnvironmentLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_GetGroundColor
�n�ʂ̊����̐F���擾���܂��B
%group
LNEnvironmentLight
%prm
environmentlight, outReturn
[in]  environmentlight : EnvironmentLight �̃n���h��
[out] outReturn(0)     : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
EnvironmentLight �� WorldObject �̃T�u�N���X�ł��BEnvironmentLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
EnvironmentLight �� Object �̃T�u�N���X�ł��BEnvironmentLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_SetGroundColor
�n�ʂ̊����̐F��ݒ肵�܂��B(default: Black)
%group
LNEnvironmentLight
%prm
environmentlight, value
[in] environmentlight : EnvironmentLight �̃n���h��
[in] value            : (Color �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
EnvironmentLight �� WorldObject �̃T�u�N���X�ł��BEnvironmentLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
EnvironmentLight �� Object �̃T�u�N���X�ł��BEnvironmentLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_SetIntensity
���C�g�̖��邳��ݒ肵�܂��B(default: 0.5)
%group
LNEnvironmentLight
%prm
environmentlight, intensity
[in] environmentlight : EnvironmentLight �̃n���h��
[in] intensity        : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
EnvironmentLight �� WorldObject �̃T�u�N���X�ł��BEnvironmentLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
EnvironmentLight �� Object �̃T�u�N���X�ł��BEnvironmentLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_GetIntensity
���C�g�̖��邳���擾���܂��B
%group
LNEnvironmentLight
%prm
environmentlight, outReturn
[in]  environmentlight : EnvironmentLight �̃n���h��
[out] outReturn(0)     : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
EnvironmentLight �� WorldObject �̃T�u�N���X�ł��BEnvironmentLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
EnvironmentLight �� Object �̃T�u�N���X�ł��BEnvironmentLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_SetShadowEffectiveDistance
���_����́A�e�𐶐��ł��鋗�����w�肵�܂��B (default: 0.0f)
%group
LNEnvironmentLight
%prm
environmentlight, value
[in] environmentlight : EnvironmentLight �̃n���h��
[in] value            : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
EnvironmentLight �� WorldObject �̃T�u�N���X�ł��BEnvironmentLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
EnvironmentLight �� Object �̃T�u�N���X�ł��BEnvironmentLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_GetShadowEffectiveDistance
���_����́A�e�𐶐��ł��鋗�����擾���܂��B
%group
LNEnvironmentLight
%prm
environmentlight, outReturn
[in]  environmentlight : EnvironmentLight �̃n���h��
[out] outReturn(0)     : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
EnvironmentLight �� WorldObject �̃T�u�N���X�ł��BEnvironmentLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
EnvironmentLight �� Object �̃T�u�N���X�ł��BEnvironmentLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_SetShadowEffectiveDepth
������������́A�e�𐶐��ł��鋗�����w�肵�܂��B (default: 0.0f) ������̓V���h�E�}�b�v�̐[�x�l�͈̔͂ƂȂ�܂��B
%group
LNEnvironmentLight
%prm
environmentlight, value
[in] environmentlight : EnvironmentLight �̃n���h��
[in] value            : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
EnvironmentLight �� WorldObject �̃T�u�N���X�ł��BEnvironmentLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
EnvironmentLight �� Object �̃T�u�N���X�ł��BEnvironmentLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEnvironmentLight_GetShadowEffectiveDepth
������������́A�e�𐶐��ł��鋗�����w�肵�܂��B
%group
LNEnvironmentLight
%prm
environmentlight, outReturn
[in]  environmentlight : EnvironmentLight �̃n���h��
[out] outReturn(0)     : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
EnvironmentLight �� WorldObject �̃T�u�N���X�ł��BEnvironmentLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
EnvironmentLight �� Object �̃T�u�N���X�ł��BEnvironmentLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_Create
����̐ݒ�Ńf�B���N�V���i�����C�g���쐬���܂��B
%group
LNDirectionalLight
%prm
outDirectionalLight
[out] outDirectionalLight(0) : �쐬���ꂽ DirectionalLight �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
DirectionalLight �� WorldObject �̃T�u�N���X�ł��BDirectionalLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
DirectionalLight �� Object �̃T�u�N���X�ł��BDirectionalLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_CreateWithColor
�F���w�肵�ăf�B���N�V���i�����C�g���쐬���܂��B
%group
LNDirectionalLight
%prm
color, outDirectionalLight
[in]  color               : (Color �^�̒l) 
[out] outDirectionalLight(0) : �쐬���ꂽ DirectionalLight �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
DirectionalLight �� WorldObject �̃T�u�N���X�ł��BDirectionalLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
DirectionalLight �� Object �̃T�u�N���X�ł��BDirectionalLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_SetEnabled
���C�g�̗L����Ԃ�ݒ肵�܂��Bfalse �̏ꍇ�A���C�g�̓V�[���ɉe�����܂���B(default: true)
%group
LNDirectionalLight
%prm
directionallight, enabled
[in] directionallight : DirectionalLight �̃n���h��
[in] enabled          : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
DirectionalLight �� WorldObject �̃T�u�N���X�ł��BDirectionalLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
DirectionalLight �� Object �̃T�u�N���X�ł��BDirectionalLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_IsEnabled
���C�g�̗L����Ԃ��擾���܂��B
%group
LNDirectionalLight
%prm
directionallight, outReturn
[in]  directionallight : DirectionalLight �̃n���h��
[out] outReturn(0)     : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
DirectionalLight �� WorldObject �̃T�u�N���X�ł��BDirectionalLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
DirectionalLight �� Object �̃T�u�N���X�ł��BDirectionalLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_SetColor
���C�g�J���[��ݒ肵�܂��B(default: White)
%group
LNDirectionalLight
%prm
directionallight, color
[in] directionallight : DirectionalLight �̃n���h��
[in] color            : (Color �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
DirectionalLight �� WorldObject �̃T�u�N���X�ł��BDirectionalLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
DirectionalLight �� Object �̃T�u�N���X�ł��BDirectionalLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_GetColor
���C�g�J���[���擾���܂��B
%group
LNDirectionalLight
%prm
directionallight, outReturn
[in]  directionallight : DirectionalLight �̃n���h��
[out] outReturn(0)     : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
DirectionalLight �� WorldObject �̃T�u�N���X�ł��BDirectionalLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
DirectionalLight �� Object �̃T�u�N���X�ł��BDirectionalLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_SetIntensity
���C�g�̖��邳��ݒ肵�܂��B(default: 0.5)
%group
LNDirectionalLight
%prm
directionallight, intensity
[in] directionallight : DirectionalLight �̃n���h��
[in] intensity        : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
DirectionalLight �� WorldObject �̃T�u�N���X�ł��BDirectionalLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
DirectionalLight �� Object �̃T�u�N���X�ł��BDirectionalLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_GetIntensity
���C�g�̖��邳���擾���܂��B
%group
LNDirectionalLight
%prm
directionallight, outReturn
[in]  directionallight : DirectionalLight �̃n���h��
[out] outReturn(0)     : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
DirectionalLight �� WorldObject �̃T�u�N���X�ł��BDirectionalLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
DirectionalLight �� Object �̃T�u�N���X�ł��BDirectionalLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_SetShadowEffectiveDistance
���_����́A�e�𐶐��ł��鋗�����w�肵�܂��B (default: 0.0f)
%group
LNDirectionalLight
%prm
directionallight, value
[in] directionallight : DirectionalLight �̃n���h��
[in] value            : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
DirectionalLight �� WorldObject �̃T�u�N���X�ł��BDirectionalLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
DirectionalLight �� Object �̃T�u�N���X�ł��BDirectionalLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_GetShadowEffectiveDistance
���_����́A�e�𐶐��ł��鋗�����擾���܂��B
%group
LNDirectionalLight
%prm
directionallight, outReturn
[in]  directionallight : DirectionalLight �̃n���h��
[out] outReturn(0)     : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
DirectionalLight �� WorldObject �̃T�u�N���X�ł��BDirectionalLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
DirectionalLight �� Object �̃T�u�N���X�ł��BDirectionalLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_SetShadowEffectiveDepth
������������́A�e�𐶐��ł��鋗�����w�肵�܂��B (default: 0.0f) ������̓V���h�E�}�b�v�̐[�x�l�͈̔͂ƂȂ�܂��B
%group
LNDirectionalLight
%prm
directionallight, value
[in] directionallight : DirectionalLight �̃n���h��
[in] value            : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
DirectionalLight �� WorldObject �̃T�u�N���X�ł��BDirectionalLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
DirectionalLight �� Object �̃T�u�N���X�ł��BDirectionalLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDirectionalLight_GetShadowEffectiveDepth
������������́A�e�𐶐��ł��鋗�����w�肵�܂��B
%group
LNDirectionalLight
%prm
directionallight, outReturn
[in]  directionallight : DirectionalLight �̃n���h��
[out] outReturn(0)     : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
DirectionalLight �� WorldObject �̃T�u�N���X�ł��BDirectionalLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
DirectionalLight �� Object �̃T�u�N���X�ł��BDirectionalLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_Create
����̐ݒ�Ń|�C���g���C�g���쐬���܂��B
%group
LNPointLight
%prm
outPointLight
[out] outPointLight(0) : �쐬���ꂽ PointLight �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
PointLight �� WorldObject �̃T�u�N���X�ł��BPointLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
PointLight �� Object �̃T�u�N���X�ł��BPointLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_CreateWithColorAndRange
�F�Ɣ͈͂��w�肵�ă|�C���g���C�g���쐬���܂��B
%group
LNPointLight
%prm
color, range, outPointLight
[in]  color         : (Color �^�̒l) 
[in]  range         : 
[out] outPointLight(0) : �쐬���ꂽ PointLight �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
PointLight �� WorldObject �̃T�u�N���X�ł��BPointLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
PointLight �� Object �̃T�u�N���X�ł��BPointLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_SetEnabled
���C�g�̗L����Ԃ�ݒ肵�܂��Bfalse �̏ꍇ�A���C�g�̓V�[���ɉe�����܂���B(default: true)
%group
LNPointLight
%prm
pointlight, enabled
[in] pointlight : PointLight �̃n���h��
[in] enabled    : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
PointLight �� WorldObject �̃T�u�N���X�ł��BPointLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
PointLight �� Object �̃T�u�N���X�ł��BPointLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_IsEnabled
���C�g�̗L����Ԃ��擾���܂��B
%group
LNPointLight
%prm
pointlight, outReturn
[in]  pointlight : PointLight �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
PointLight �� WorldObject �̃T�u�N���X�ł��BPointLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
PointLight �� Object �̃T�u�N���X�ł��BPointLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_SetColor
���C�g�J���[��ݒ肵�܂��B(default: White)
%group
LNPointLight
%prm
pointlight, color
[in] pointlight : PointLight �̃n���h��
[in] color      : (Color �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
PointLight �� WorldObject �̃T�u�N���X�ł��BPointLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
PointLight �� Object �̃T�u�N���X�ł��BPointLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_GetColor
���C�g�J���[���擾���܂��B
%group
LNPointLight
%prm
pointlight, outReturn
[in]  pointlight : PointLight �̃n���h��
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
PointLight �� WorldObject �̃T�u�N���X�ł��BPointLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
PointLight �� Object �̃T�u�N���X�ł��BPointLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_SetIntensity
���C�g�̖��邳��ݒ肵�܂��B(default: 1.0)
%group
LNPointLight
%prm
pointlight, intensity
[in] pointlight : PointLight �̃n���h��
[in] intensity  : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
PointLight �� WorldObject �̃T�u�N���X�ł��BPointLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
PointLight �� Object �̃T�u�N���X�ł��BPointLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_GetIntensity
���C�g�̖��邳���擾���܂��B
%group
LNPointLight
%prm
pointlight, outReturn
[in]  pointlight : PointLight �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
PointLight �� WorldObject �̃T�u�N���X�ł��BPointLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
PointLight �� Object �̃T�u�N���X�ł��BPointLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_SetRange
���C�g�̉e���͈͂�ݒ肵�܂��B(default: 10.0)
%group
LNPointLight
%prm
pointlight, range
[in] pointlight : PointLight �̃n���h��
[in] range      : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
PointLight �� WorldObject �̃T�u�N���X�ł��BPointLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
PointLight �� Object �̃T�u�N���X�ł��BPointLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_GetRange
���C�g�̉e���͈͂��擾���܂��B
%group
LNPointLight
%prm
pointlight, outReturn
[in]  pointlight : PointLight �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
PointLight �� WorldObject �̃T�u�N���X�ł��BPointLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
PointLight �� Object �̃T�u�N���X�ł��BPointLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_SetAttenuation
���C�g�̌�����ݒ肵�܂��B(default: 1.0)
%group
LNPointLight
%prm
pointlight, attenuation
[in] pointlight  : PointLight �̃n���h��
[in] attenuation : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
PointLight �� WorldObject �̃T�u�N���X�ł��BPointLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
PointLight �� Object �̃T�u�N���X�ł��BPointLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNPointLight_GetAttenuation
���C�g�̌������擾���܂��B
%group
LNPointLight
%prm
pointlight, outReturn
[in]  pointlight : PointLight �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
PointLight �� WorldObject �̃T�u�N���X�ł��BPointLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
PointLight �� Object �̃T�u�N���X�ł��BPointLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_Create
����̐ݒ�ŃX�|�b�g���C�g���쐬���܂��B
%group
LNSpotLight
%prm
outSpotLight
[out] outSpotLight(0) : �쐬���ꂽ SpotLight �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
SpotLight �� WorldObject �̃T�u�N���X�ł��BSpotLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
SpotLight �� Object �̃T�u�N���X�ł��BSpotLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_CreateWithColorAndRange
�F�Ɣ͈͂��w�肵�ăX�|�b�g���C�g���쐬���܂��B
%group
LNSpotLight
%prm
color, range, angle, outSpotLight
[in]  color        : (Color �^�̒l) 
[in]  range        : 
[in]  angle        : 
[out] outSpotLight(0) : �쐬���ꂽ SpotLight �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
SpotLight �� WorldObject �̃T�u�N���X�ł��BSpotLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
SpotLight �� Object �̃T�u�N���X�ł��BSpotLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_SetEnabled
���C�g�̗L����Ԃ�ݒ肵�܂��Bfalse �̏ꍇ�A���C�g�̓V�[���ɉe�����܂���B(default: true)
%group
LNSpotLight
%prm
spotlight, enabled
[in] spotlight : SpotLight �̃n���h��
[in] enabled   : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
SpotLight �� WorldObject �̃T�u�N���X�ł��BSpotLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
SpotLight �� Object �̃T�u�N���X�ł��BSpotLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_IsEnabled
���C�g�̗L����Ԃ��擾���܂��B
%group
LNSpotLight
%prm
spotlight, outReturn
[in]  spotlight : SpotLight �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
SpotLight �� WorldObject �̃T�u�N���X�ł��BSpotLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
SpotLight �� Object �̃T�u�N���X�ł��BSpotLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_SetColor
���C�g�J���[��ݒ肵�܂��B(default: White)
%group
LNSpotLight
%prm
spotlight, color
[in] spotlight : SpotLight �̃n���h��
[in] color     : (Color �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
SpotLight �� WorldObject �̃T�u�N���X�ł��BSpotLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
SpotLight �� Object �̃T�u�N���X�ł��BSpotLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_GetColor
���C�g�J���[���擾���܂��B
%group
LNSpotLight
%prm
spotlight, outReturn
[in]  spotlight : SpotLight �̃n���h��
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
SpotLight �� WorldObject �̃T�u�N���X�ł��BSpotLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
SpotLight �� Object �̃T�u�N���X�ł��BSpotLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_SetIntensity
���C�g�̖��邳��ݒ肵�܂��B(default: 1.0)
%group
LNSpotLight
%prm
spotlight, intensity
[in] spotlight : SpotLight �̃n���h��
[in] intensity : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
SpotLight �� WorldObject �̃T�u�N���X�ł��BSpotLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
SpotLight �� Object �̃T�u�N���X�ł��BSpotLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_GetIntensity
���C�g�̖��邳���擾���܂��B
%group
LNSpotLight
%prm
spotlight, outReturn
[in]  spotlight : SpotLight �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
SpotLight �� WorldObject �̃T�u�N���X�ł��BSpotLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
SpotLight �� Object �̃T�u�N���X�ł��BSpotLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_SetRange
���C�g�̉e���͈͂�ݒ肵�܂��B(default: 10.0)
%group
LNSpotLight
%prm
spotlight, range
[in] spotlight : SpotLight �̃n���h��
[in] range     : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
SpotLight �� WorldObject �̃T�u�N���X�ł��BSpotLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
SpotLight �� Object �̃T�u�N���X�ł��BSpotLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_GetRange
���C�g�̉e���͈͂��擾���܂��B
%group
LNSpotLight
%prm
spotlight, outReturn
[in]  spotlight : SpotLight �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
SpotLight �� WorldObject �̃T�u�N���X�ł��BSpotLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
SpotLight �� Object �̃T�u�N���X�ł��BSpotLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_SetAttenuation
���C�g�̌�����ݒ肵�܂��B(default: 1.0)
%group
LNSpotLight
%prm
spotlight, attenuation
[in] spotlight   : SpotLight �̃n���h��
[in] attenuation : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
SpotLight �� WorldObject �̃T�u�N���X�ł��BSpotLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
SpotLight �� Object �̃T�u�N���X�ł��BSpotLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_GetAttenuation
���C�g�̌������擾���܂��B
%group
LNSpotLight
%prm
spotlight, outReturn
[in]  spotlight : SpotLight �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
SpotLight �� WorldObject �̃T�u�N���X�ł��BSpotLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
SpotLight �� Object �̃T�u�N���X�ł��BSpotLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_SetAngle
�X�|�b�g���C�g�̃R�[���p�x��ݒ肵�܂��B(���W�A���P�ʁAdefault: PI / 3)
%group
LNSpotLight
%prm
spotlight, angle
[in] spotlight : SpotLight �̃n���h��
[in] angle     : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
SpotLight �� WorldObject �̃T�u�N���X�ł��BSpotLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
SpotLight �� Object �̃T�u�N���X�ł��BSpotLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_GetAngle
�X�|�b�g���C�g�̃R�[���p�x���擾���܂��B(���W�A���P��)
%group
LNSpotLight
%prm
spotlight, outReturn
[in]  spotlight : SpotLight �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
SpotLight �� WorldObject �̃T�u�N���X�ł��BSpotLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
SpotLight �� Object �̃T�u�N���X�ł��BSpotLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_SetPenumbra
�X�|�b�g���C�g�̃R�[���p�x�ɑ΂��錸������ݒ肵�܂��B(0..1, default: 0)
%group
LNSpotLight
%prm
spotlight, penumbra
[in] spotlight : SpotLight �̃n���h��
[in] penumbra  : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
SpotLight �� WorldObject �̃T�u�N���X�ł��BSpotLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
SpotLight �� Object �̃T�u�N���X�ł��BSpotLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSpotLight_GetPenumbra
�X�|�b�g���C�g�̃R�[���p�x�ɑ΂��錸������ݒ肵�܂��B
%group
LNSpotLight
%prm
spotlight, outReturn
[in]  spotlight : SpotLight �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
SpotLight �� WorldObject �̃T�u�N���X�ł��BSpotLight �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
SpotLight �� Object �̃T�u�N���X�ł��BSpotLight �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outTestDelegate(0) : �쐬���ꂽ TestDelegate �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
TestDelegate �� Object �̃T�u�N���X�ł��BTestDelegate �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outSprite(0) : �쐬���ꂽ Sprite �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sprite �� VisualObject �̃T�u�N���X�ł��BSprite �n���h���� VisualObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNVisualObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Sprite �� WorldObject �̃T�u�N���X�ł��BSprite �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Sprite �� Object �̃T�u�N���X�ł��BSprite �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outSprite(0) : �쐬���ꂽ Sprite �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sprite �� VisualObject �̃T�u�N���X�ł��BSprite �n���h���� VisualObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNVisualObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Sprite �� WorldObject �̃T�u�N���X�ł��BSprite �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Sprite �� Object �̃T�u�N���X�ł��BSprite �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outSprite(0) : �쐬���ꂽ Sprite �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sprite �� VisualObject �̃T�u�N���X�ł��BSprite �n���h���� VisualObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNVisualObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Sprite �� WorldObject �̃T�u�N���X�ł��BSprite �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Sprite �� Object �̃T�u�N���X�ł��BSprite �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSprite_SetTexture
�X�v���C�g���\������e�N�X�`����ݒ肵�܂��B
%group
LNSprite
%prm
sprite, value
[in] sprite : Sprite �̃n���h��
[in] value  : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sprite �� VisualObject �̃T�u�N���X�ł��BSprite �n���h���� VisualObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNVisualObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Sprite �� WorldObject �̃T�u�N���X�ł��BSprite �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Sprite �� Object �̃T�u�N���X�ł��BSprite �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSprite_SetSize
�X�v���C�g�̑傫����ݒ肵�܂��B
%group
LNSprite
%prm
sprite, value
[in] sprite : Sprite �̃n���h��
[in] value  : (Size �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sprite �� VisualObject �̃T�u�N���X�ł��BSprite �n���h���� VisualObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNVisualObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Sprite �� WorldObject �̃T�u�N���X�ł��BSprite �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Sprite �� Object �̃T�u�N���X�ł��BSprite �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNSprite_SetSizeWH
�X�v���C�g�̑傫����ݒ肵�܂��B
%group
LNSprite
%prm
sprite, width, height
[in] sprite : Sprite �̃n���h��
[in] width  : 
[in] height : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sprite �� VisualObject �̃T�u�N���X�ł��BSprite �n���h���� VisualObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNVisualObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Sprite �� WorldObject �̃T�u�N���X�ł��BSprite �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Sprite �� Object �̃T�u�N���X�ł��BSprite �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] sprite : Sprite �̃n���h��
[in] x      : 
[in] y      : 
[in] width  : 
[in] height : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sprite �� VisualObject �̃T�u�N���X�ł��BSprite �n���h���� VisualObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNVisualObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Sprite �� WorldObject �̃T�u�N���X�ł��BSprite �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Sprite �� Object �̃T�u�N���X�ł��BSprite �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] sprite   : Sprite �̃n���h��
[in] callback : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Sprite �� VisualObject �̃T�u�N���X�ł��BSprite �n���h���� VisualObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNVisualObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Sprite �� WorldObject �̃T�u�N���X�ł��BSprite �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Sprite �� Object �̃T�u�N���X�ł��BSprite �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCameraOrbitControlComponent_Create
CameraOrbitControlComponent ���쐬���܂��B
%group
LNCameraOrbitControlComponent
%prm
outCameraOrbitControlComponent
[out] outCameraOrbitControlComponent(0) : �쐬���ꂽ CameraOrbitControlComponent �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
CameraOrbitControlComponent �� Component �̃T�u�N���X�ł��BCameraOrbitControlComponent �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
CameraOrbitControlComponent �� Object �̃T�u�N���X�ł��BCameraOrbitControlComponent �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRaycaster_FromScreen
���C���̃J�������g�p���āA�w�肵���X�N���[�����W���琳�ʂɌ��������C���`���� Raycaster ���擾���܂��B
%group
LNRaycaster
%prm
point, outReturn
[in]  point     : (Point �^�̒l) 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Raycaster �� Object �̃T�u�N���X�ł��BRaycaster �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRaycaster_IntersectPlane
�w�肵�������̕��ʂƂ̌���������s���܂��B
%group
LNRaycaster
%prm
raycaster, normalX, normalY, normalZ, outReturn
[in]  raycaster : Raycaster �̃n���h��
[in]  normalX   : 
[in]  normalY   : 
[in]  normalZ   : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Raycaster �� Object �̃T�u�N���X�ł��BRaycaster �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNRaycastResult_GetPoint
���[���h���W��̌����_
%group
LNRaycastResult
%prm
raycastresult, outReturn
[in]  raycastresult : RaycastResult �̃n���h��
[out] outReturn(0)  : (Vector3 �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
RaycastResult �� Object �̃T�u�N���X�ł��BRaycastResult �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldRenderView_SetGuideGridEnabled
���� WorldRenderView ���`�悷�� 3D �V�[����ɁA�O���b�h��\�����邩�ǂ�����ݒ肵�܂��B
%group
LNWorldRenderView
%prm
worldrenderview, value
[in] worldrenderview : WorldRenderView �̃n���h��
[in] value           : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldRenderView �� RenderView �̃T�u�N���X�ł��BWorldRenderView �n���h���� RenderView �n���h���Ƃ��Ĉ������Ƃ��ł��A LNRenderView_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
WorldRenderView �� Object �̃T�u�N���X�ł��BWorldRenderView �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldRenderView_GetGuideGridEnabled
���� WorldRenderView ���`�悷�� 3D �V�[����ɁA�O���b�h��\�����邩�ǂ������擾���܂��B
%group
LNWorldRenderView
%prm
worldrenderview, outReturn
[in]  worldrenderview : WorldRenderView �̃n���h��
[out] outReturn(0)    : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
WorldRenderView �� RenderView �̃T�u�N���X�ł��BWorldRenderView �n���h���� RenderView �n���h���Ƃ��Ĉ������Ƃ��ł��A LNRenderView_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
WorldRenderView �� Object �̃T�u�N���X�ł��BWorldRenderView �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outPlaneMesh(0) : �쐬���ꂽ PlaneMesh �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
PlaneMesh �� ShapeObject �̃T�u�N���X�ł��BPlaneMesh �n���h���� ShapeObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNShapeObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
PlaneMesh �� VisualObject �̃T�u�N���X�ł��BPlaneMesh �n���h���� VisualObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNVisualObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
PlaneMesh �� WorldObject �̃T�u�N���X�ł��BPlaneMesh �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
PlaneMesh �� Object �̃T�u�N���X�ł��BPlaneMesh �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNBoxMesh_Create
�e���̃T�C�Y�� 1 �ł��� BoxMesh ���쐬���܂��B
%group
LNBoxMesh
%prm
outBoxMesh
[out] outBoxMesh(0) : �쐬���ꂽ BoxMesh �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
BoxMesh �� ShapeObject �̃T�u�N���X�ł��BBoxMesh �n���h���� ShapeObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNShapeObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
BoxMesh �� VisualObject �̃T�u�N���X�ł��BBoxMesh �n���h���� VisualObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNVisualObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
BoxMesh �� WorldObject �̃T�u�N���X�ł��BBoxMesh �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
BoxMesh �� Object �̃T�u�N���X�ł��BBoxMesh �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outBoxMesh(0) : �쐬���ꂽ BoxMesh �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
BoxMesh �� ShapeObject �̃T�u�N���X�ł��BBoxMesh �n���h���� ShapeObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNShapeObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
BoxMesh �� VisualObject �̃T�u�N���X�ł��BBoxMesh �n���h���� VisualObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNVisualObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
BoxMesh �� WorldObject �̃T�u�N���X�ł��BBoxMesh �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
BoxMesh �� Object �̃T�u�N���X�ł��BBoxMesh �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outMeshComponent(0) : �쐬���ꂽ MeshComponent �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
MeshComponent �� VisualComponent �̃T�u�N���X�ł��BMeshComponent �n���h���� VisualComponent �n���h���Ƃ��Ĉ������Ƃ��ł��A LNVisualComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
MeshComponent �� Component �̃T�u�N���X�ł��BMeshComponent �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
MeshComponent �� Object �̃T�u�N���X�ł��BMeshComponent �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] meshcomponent : MeshComponent �̃n���h��
[in] model         : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
MeshComponent �� VisualComponent �̃T�u�N���X�ł��BMeshComponent �n���h���� VisualComponent �n���h���Ƃ��Ĉ������Ƃ��ł��A LNVisualComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
MeshComponent �� Component �̃T�u�N���X�ł��BMeshComponent �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
MeshComponent �� Object �̃T�u�N���X�ł��BMeshComponent �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMeshComponent_MakeCollisionBody
�w�肵�����O�� MeshContainer ����A�Փ˔���p�� Body ���쐬���܂��B
%group
LNMeshComponent
%prm
meshcomponent, meshContainerName
[in] meshcomponent     : MeshComponent �̃n���h��
[in] meshContainerName : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
MeshComponent �� VisualComponent �̃T�u�N���X�ł��BMeshComponent �n���h���� VisualComponent �n���h���Ƃ��Ĉ������Ƃ��ł��A LNVisualComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
MeshComponent �� Component �̃T�u�N���X�ł��BMeshComponent �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
MeshComponent �� Object �̃T�u�N���X�ł��BMeshComponent �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNCollision_GetWorldObject
�������g�ƏՓ˂��Ă��鑼�� WorldObject
%group
LNCollision
%prm
collision, outReturn
[in]  collision : Collision �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Collision �� Object �̃T�u�N���X�ł��BCollision �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outTriggerBodyComponent(0) : �쐬���ꂽ TriggerBodyComponent �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
TriggerBodyComponent �� Component �̃T�u�N���X�ł��BTriggerBodyComponent �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
TriggerBodyComponent �� Object �̃T�u�N���X�ł��BTriggerBodyComponent �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] triggerbodycomponent : TriggerBodyComponent �̃n���h��
[in] shape                : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
TriggerBodyComponent �� Component �̃T�u�N���X�ł��BTriggerBodyComponent �n���h���� Component �n���h���Ƃ��Ĉ������Ƃ��ł��A LNComponent_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
TriggerBodyComponent �� Object �̃T�u�N���X�ł��BTriggerBodyComponent �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outParticleEmitter(0) : �쐬���ꂽ ParticleEmitter �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
ParticleEmitter �� VisualObject �̃T�u�N���X�ł��BParticleEmitter �n���h���� VisualObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNVisualObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
ParticleEmitter �� WorldObject �̃T�u�N���X�ł��BParticleEmitter �n���h���� WorldObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNWorldObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
ParticleEmitter �� Object �̃T�u�N���X�ł��BParticleEmitter �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetClearMode
�V�[���w�i�̃N���A���@��ݒ肵�܂��B
%group
LNScene
%prm
value
[in] value : 
                LN_SCENE_CLEAR_MODE_NONE
                �N���A���܂���B
                LN_SCENE_CLEAR_MODE_COLOR_AND_DEPTH
                �w�i�F�Ɛ[�x�o�b�t�@���N���A���܂��B
                LN_SCENE_CLEAR_MODE_SKY
                (������)
                LN_SCENE_CLEAR_MODE_SKY0
                (������)
                LN_SCENE_CLEAR_MODE_SKY_DOME
                �X�J�C�h�[���ŃN���A���܂��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetSkyColor
ClearMode �� SkyDome �ł���Ƃ��Ɏg�p����A��̊�{�F��ݒ肵�܂��B�A���t�@�l�́A�ݒ肵���F�̓K�p���ł��B
%group
LNScene
%prm
value
[in] value : (Color �^�̒l) 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetSkyHorizonColor
ClearMode �� SkyDome �ł���Ƃ��Ɏg�p����A�n���̊�{�F��ݒ肵�܂��B�A���t�@�l�́A�ݒ肵���F�̓K�p���ł��B
%group
LNScene
%prm
value
[in] value : (Color �^�̒l) 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetSkyCloudColor
ClearMode �� SkyDome �ł���Ƃ��Ɏg�p����A�_�̊�{�F��ݒ肵�܂��B�A���t�@�l�́A�ݒ肵���F�̓K�p���ł��B
%group
LNScene
%prm
value
[in] value : (Color �^�̒l) 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetSkyOverlayColor
ClearMode �� SkyDome �ł���Ƃ��Ɏg�p����A��S�̂ɉe������F��ݒ肵�܂��B�A���t�@�l�́A�ݒ肵���F�̓K�p���ł��B
%group
LNScene
%prm
value
[in] value : (Color �^�̒l) 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_GotoLevel
�w�肵�����x���֑J�ڂ��܂��B�����̑S�Ẵ��x���͔�A�N�e�B�u���܂��͍폜����܂��B
%group
LNScene
%prm
level, withEffect
[in] level               : 
[in] withEffect(LN_TRUE) : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_CallLevel
���݂̃��x�����A�N�e�B�u�����A�w�肵�����x���֑J�ڂ��܂��B
%group
LNScene
%prm
level, withEffect
[in] level               : 
[in] withEffect(LN_TRUE) : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_ReturnLevel
���݂̃��x�����I�����A�ЂƂO�̃��x���֑J�ڂ��܂��B
%group
LNScene
%prm
withEffect
[in] withEffect(LN_TRUE) : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_ActiveLevel
���݂̃A�N�e�B�u�ȃ��x�����擾���܂��B
%group
LNScene
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsTransitionEffectRunning
���x���̑J�ڃG�t�F�N�g�����s���ł��邩���m�F���܂��B
%group
LNScene
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTransitionEffectMode
���x���J�ڎ��̃G�t�F�N�g�̎�ނ�ݒ肵�܂��B
%group
LNScene
%prm
value
[in] value : 
                LN_LEVEL_TRANSITION_EFFECT_MODE_NONE
                �G�t�F�N�g����
                LN_LEVEL_TRANSITION_EFFECT_MODE_FADE_IN_OUT
                �t�F�[�h�C���E�t�F�[�h�A�E�g
                LN_LEVEL_TRANSITION_EFFECT_MODE_CROSS_FADE
                �N���X�t�F�[�h

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_TransitionEffectMode
���x���J�ڎ��̃G�t�F�N�g�̎�ނ��擾���܂��B
%group
LNScene
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B
                     LN_LEVEL_TRANSITION_EFFECT_MODE_NONE
                     �G�t�F�N�g����
                     LN_LEVEL_TRANSITION_EFFECT_MODE_FADE_IN_OUT
                     �t�F�[�h�C���E�t�F�[�h�A�E�g
                     LN_LEVEL_TRANSITION_EFFECT_MODE_CROSS_FADE
                     �N���X�t�F�[�h

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTransitionDuration
���x���̑J�ڂɂ����鎞�Ԃ�ݒ肵�܂��B(Unit: �b)
%group
LNScene
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_TransitionDuration
���x���̑J�ڂɂ����鎞�Ԃ��擾���܂��B(Unit: �b)
%group
LNScene
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTransitionEffectColor
���x���̑J�ڃ��[�h�� FadeInOut �ł���ꍇ�Ɏg�p����F��ݒ肵�܂��B
%group
LNScene
%prm
value
[in] value : (Color �^�̒l) 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_TransitionEffectColor
���x���̑J�ڃ��[�h�� FadeInOut �ł���ꍇ�Ɏg�p����F���擾���܂��B
%group
LNScene
%prm
outReturn
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTransitionEffectMaskTexture
���x���̑J�ڃG�t�F�N�g�Ŏg�p����}�X�N�e�N�X�`����ݒ肵�܂��B
%group
LNScene
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_TransitionEffectMaskTexture
���x���̑J�ڃG�t�F�N�g�Ŏg�p����}�X�N�e�N�X�`�����擾���܂��B
%group
LNScene
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTransitionEffectVague
���x���̑J�ڃG�t�F�N�g�̋��E�̂����܂�����ݒ肵�܂��B
%group
LNScene
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_TransitionEffectVague
���x���̑J�ڃG�t�F�N�g�̋��E�̂����܂������擾���܂��B
%group
LNScene
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_StartFadeOut
�t�F�[�h�A�E�g�G�t�F�N�g���J�n���܂��B
%group
LNScene
%prm


stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_StartFadeIn
�t�F�[�h�C���G�t�F�N�g���J�n���܂��B
%group
LNScene
%prm


stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetFogStartDistance
�t�H�O���J�n����J��������̋�����ݒ肵�܂��B
%group
LNScene
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetFogColor
�t�H�O�̃��C���J���[��ݒ肵�܂��B
%group
LNScene
%prm
value
[in] value : (Color �^�̒l) 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetFogDensity
�t�H�O�̔Z����ݒ肵�܂��B
%group
LNScene
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetFogHeightDensity
�����t�H�O�̔Z����ݒ肵�܂��B
%group
LNScene
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetFogLowerHeight
�t�H�O�̍����̉�����ݒ肵�܂��B
%group
LNScene
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetFogUpperHeight
�t�H�O�̍����̏����ݒ肵�܂��B
%group
LNScene
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetHDREnabled
HDR �����_�����O�̗L����ݒ肵�܂��B (default: false)
%group
LNScene
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsHDREnabled
HDR �����_�����O�̗L�����擾���܂��B
%group
LNScene
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetScreenBlendColor
��ʑS�̂ւ̃u�����h�J���[��ݒ肵�܂��B(default: Black)
%group
LNScene
%prm
value
[in] value : (Color �^�̒l) 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_ScreenBlendColor
��ʑS�̂ւ̃u�����h�J���[���擾���܂��B
%group
LNScene
%prm
outReturn
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

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
[in] value : (ColorTone �^�̒l) 

stat : 0=����, ���l=���s

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
[out] outReturn(0) : (ColorTone �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetAntialiasEnabled
�A���`�G�C���A�X�̗L����ݒ肵�܂��B(default: false)
%group
LNScene
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsAntialiasEnabled
�A���`�G�C���A�X�̗L�����擾���܂��B
%group
LNScene
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetSSREnabled
SSR (Screen Space Reflection) �̗L����ݒ肵�܂��B(default: false)
%group
LNScene
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsSSREnabled
SSR �̗L�����擾���܂��B
%group
LNScene
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetSSAOEnabled
SSAO (Screen Space Ambient Occlusion) �̗L����ݒ肵�܂��B(default: false)
%group
LNScene
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsSSAOEnabled
SSAO �̗L�����擾���܂��B
%group
LNScene
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetBloomEnabled
�u���[���G�t�F�N�g�̗L����ݒ肵�܂��B(default: false)
%group
LNScene
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsBloomEnabled
�u���[���G�t�F�N�g�̗L�����擾���܂��B
%group
LNScene
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetDOFEnabled
��ʊE�[�x�̗L����ݒ肵�܂��B(default: false)
%group
LNScene
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsDOFEnabled
��ʊE�[�x�̗L�����擾���܂��B
%group
LNScene
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetTonemapEnabled
�g�[���}�b�s���O�̗L����ݒ肵�܂��B(default: false)
%group
LNScene
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsTonemapEnabled
�g�[���}�b�s���O�̗L�����擾���܂��B
%group
LNScene
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetVignetteEnabled
�r�l�b�g�G�t�F�N�g�̗L����ݒ肵�܂��B(default: false)
%group
LNScene
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsVignetteEnabled
�r�l�b�g�G�t�F�N�g�̗L�����擾���܂��B
%group
LNScene
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_SetGammaEnabled
�K���}�␳�̗L����ݒ肵�܂��B(default: false)
%group
LNScene
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNScene_IsGammaEnabled
�K���}�␳�̗L�����擾���܂��B
%group
LNScene
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

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

stat : 0=����, ���l=���s

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

stat : 0=����, ���l=���s

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

stat : 0=����, ���l=���s

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

stat : 0=����, ���l=���s

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

stat : 0=����, ���l=���s

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

stat : 0=����, ���l=���s

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

stat : 0=����, ���l=���s

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

stat : 0=����, ���l=���s

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
[out] outLevel(0) : �쐬���ꂽ Level �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Level �� AssetObject �̃T�u�N���X�ł��BLevel �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Level �� Object �̃T�u�N���X�ł��BLevel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLevel_AddObject
�w�肵�� WorldObject ���A���� Level �q�I�u�W�F�N�g�Ƃ��Ēǉ����܂��B
%group
LNLevel
%prm
level, obj
[in] level : Level �̃n���h��
[in] obj   : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Level �� AssetObject �̃T�u�N���X�ł��BLevel �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Level �� Object �̃T�u�N���X�ł��BLevel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLevel_RemoveObject
�w�肵�� WorldObject ���A���� Level �q�I�u�W�F�N�g�����O���܂��B
%group
LNLevel
%prm
level, obj
[in] level : Level �̃n���h��
[in] obj   : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Level �� AssetObject �̃T�u�N���X�ł��BLevel �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Level �� Object �̃T�u�N���X�ł��BLevel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLevel_AddSubLevel
�w�肵�� Level ���A���� Level �� Sub-Level �Ƃ��Ēǉ����܂��B
%group
LNLevel
%prm
level, sublevel
[in] level    : Level �̃n���h��
[in] sublevel : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Level �� AssetObject �̃T�u�N���X�ł��BLevel �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Level �� Object �̃T�u�N���X�ł��BLevel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLevel_RemoveSubLevel
�w�肵�� Level ���A���� Level �� Sub-Level ���珜�O���܂��B
%group
LNLevel
%prm
level, sublevel
[in] level    : Level �̃n���h��
[in] sublevel : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Level �� AssetObject �̃T�u�N���X�ł��BLevel �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Level �� Object �̃T�u�N���X�ł��BLevel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNLevel_RemoveAllSubLevels
���ׂĂ� Sub-Level �����O���܂��B
%group
LNLevel
%prm
level
[in] level : Level �̃n���h��

stat : 0=����, ���l=���s

%inst


���l
--------------------
Level �� AssetObject �̃T�u�N���X�ł��BLevel �n���h���� AssetObject �n���h���Ƃ��Ĉ������Ƃ��ł��A LNAssetObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
Level �� Object �̃T�u�N���X�ł��BLevel �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Red
�F�̔Z�� (0~9) ���w�肵�āA Red �J���[�p���b�g�̐F���擾���܂��B
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Pink
�F�̔Z�� (0~9) ���w�肵�āA Pink �J���[�p���b�g�̐F���擾���܂��B
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Purple
�F�̔Z�� (0~9) ���w�肵�āA Purple �J���[�p���b�g�̐F���擾���܂��B
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_DeepPurple
�F�̔Z�� (0~9) ���w�肵�āA DeepPurple �J���[�p���b�g�̐F���擾���܂��B
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Indigo
�F�̔Z�� (0~9) ���w�肵�āA Indigo �J���[�p���b�g�̐F���擾���܂��B
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Blue
�F�̔Z�� (0~9) ���w�肵�āA Blue �J���[�p���b�g�̐F���擾���܂��B
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_LightBlue
�F�̔Z�� (0~9) ���w�肵�āA LightBlue �J���[�p���b�g�̐F���擾���܂��B
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Cyan
�F�̔Z�� (0~9) ���w�肵�āA Cyan �J���[�p���b�g�̐F���擾���܂��B
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Teal
�F�̔Z�� (0~9) ���w�肵�āA Teal �J���[�p���b�g�̐F���擾���܂��B
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Green
�F�̔Z�� (0~9) ���w�肵�āA Green �J���[�p���b�g�̐F���擾���܂��B
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_LightGreen
�F�̔Z�� (0~9) ���w�肵�āA LightGreen �J���[�p���b�g�̐F���擾���܂��B
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Lime
�F�̔Z�� (0~9) ���w�肵�āA Lime �J���[�p���b�g�̐F���擾���܂��B
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Yellow
�F�̔Z�� (0~9) ���w�肵�āA Yellow �J���[�p���b�g�̐F���擾���܂��B
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Amber
�F�̔Z�� (0~9) ���w�肵�āA Amber �J���[�p���b�g�̐F���擾���܂��B
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Orange
�F�̔Z�� (0~9) ���w�肵�āA Orange �J���[�p���b�g�̐F���擾���܂��B
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_DeepOrange
�F�̔Z�� (0~9) ���w�肵�āA DeepOrange �J���[�p���b�g�̐F���擾���܂��B
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Brown
�F�̔Z�� (0~9) ���w�肵�āA Brown �J���[�p���b�g�̐F���擾���܂��B
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Grey
�F�̔Z�� (0~9) ���w�肵�āA Grey �J���[�p���b�g�̐F���擾���܂��B
%group
LNUIColors
%prm
shades, outReturn
[in]  shades(5) : 
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIColors_Get
�J���[�C���f�b�N�X�ƐF�̔Z�� (0~9) ���w�肵�āA�F���擾���܂��B
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
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIEventArgs_Sender
�C�x���g�̔������ƂȂ����v�f���擾���܂��B
%group
LNUIEventArgs
%prm
uieventargs, outReturn
[in]  uieventargs : UIEventArgs �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIEventArgs �� Object �̃T�u�N���X�ł��BUIEventArgs �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outUIGeneralEventHandler(0) : �쐬���ꂽ UIGeneralEventHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIGeneralEventHandler �� Object �̃T�u�N���X�ł��BUIGeneralEventHandler �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outUIEventHandler(0) : �쐬���ꂽ UIEventHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIEventHandler �� Object �̃T�u�N���X�ł��BUIEventHandler �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetSize
�v�f�̃T�C�Y��ݒ肵�܂��B�T�C�Y�ɂ́Aborder �� padding �̕��ƍ����͊܂܂�܂���B(��Fwidth 10, border 10 �Ƃ���ƁA�v�f�̍ŏI�T�C�Y�� 20 �ƂȂ�܂�)
%group
LNUIElement
%prm
uielement, size
[in] uielement : UIElement �̃n���h��
[in] size      : (Size �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetSizeWH
�v�f�̃T�C�Y��ݒ肵�܂��B�T�C�Y�ɂ́Aborder �� padding �̕��ƍ����͊܂܂�܂���B(��Fwidth 10, border 10 �Ƃ���ƁA�v�f�̍ŏI�T�C�Y�� 20 �ƂȂ�܂�)
%group
LNUIElement
%prm
uielement, width, height
[in] uielement : UIElement �̃n���h��
[in] width     : 
[in] height    : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] uielement : UIElement �̃n���h��
[in] value     : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] uielement : UIElement �̃n���h��
[in] value     : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetMargin
�v�f�� margin �l (�O���̗]��) ��ݒ肵�܂��B
%group
LNUIElement
%prm
uielement, margin
[in] uielement : UIElement �̃n���h��
[in] margin    : (Thickness �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetMargin
�v�f�� margin �l (�O���̗]��) ���擾���܂��B
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : (Thickness �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetPadding
�v�f�� padding �l (�����̗]��) ��ݒ肵�܂��B���̗]���͘_���c���[�̎q�v�f�̃��C�A�E�g�ɉe�����܂��B
%group
LNUIElement
%prm
uielement, padding
[in] uielement : UIElement �̃n���h��
[in] padding   : (Thickness �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetPadding
�v�f�� padding �l (�����̗]��) ���擾���܂��B���̗]���͘_���c���[�̎q�v�f�̃��C�A�E�g�ɉe�����܂��B
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : (Thickness �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetHAlignment
�v�f�̉������̔z�u���@��ݒ肵�܂��B
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement �̃n���h��
[in] value     : 
                    LN_UIHALIGNMENT_LEFT
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�̍����ɑ����Ĕz�u���܂��B
                    LN_UIHALIGNMENT_CENTER
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�̒����ɑ����Ĕz�u���܂��B
                    LN_UIHALIGNMENT_RIGHT
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�̉E���ɑ����Ĕz�u���܂��B
                    LN_UIHALIGNMENT_STRETCH
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�S�̂Ɉ����L�΂��܂��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetHAlignment
�v�f�̉������̔z�u���@���擾���܂��B
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B
                     LN_UIHALIGNMENT_LEFT
                     �q�v�f���A�e�̃��C�A�E�g �X���b�g�̍����ɑ����Ĕz�u���܂��B
                     LN_UIHALIGNMENT_CENTER
                     �q�v�f���A�e�̃��C�A�E�g �X���b�g�̒����ɑ����Ĕz�u���܂��B
                     LN_UIHALIGNMENT_RIGHT
                     �q�v�f���A�e�̃��C�A�E�g �X���b�g�̉E���ɑ����Ĕz�u���܂��B
                     LN_UIHALIGNMENT_STRETCH
                     �q�v�f���A�e�̃��C�A�E�g �X���b�g�S�̂Ɉ����L�΂��܂��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetVAlignment
�v�f�̏c�����̔z�u���@��ݒ肵�܂��B
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement �̃n���h��
[in] value     : 
                    LN_UIVALIGNMENT_TOP
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�̏�[�ɑ����Ĕz�u���܂��B
                    LN_UIVALIGNMENT_CENTER
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�̒����ɑ����Ĕz�u���܂��B
                    LN_UIVALIGNMENT_BOTTOM
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�̉��[�ɑ����Ĕz�u���܂��B
                    LN_UIVALIGNMENT_STRETCH
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�S�̂Ɉ����L�΂��܂��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetVAlignment
�v�f�̏c�����̔z�u���@���擾���܂��B
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B
                     LN_UIVALIGNMENT_TOP
                     �q�v�f���A�e�̃��C�A�E�g �X���b�g�̏�[�ɑ����Ĕz�u���܂��B
                     LN_UIVALIGNMENT_CENTER
                     �q�v�f���A�e�̃��C�A�E�g �X���b�g�̒����ɑ����Ĕz�u���܂��B
                     LN_UIVALIGNMENT_BOTTOM
                     �q�v�f���A�e�̃��C�A�E�g �X���b�g�̉��[�ɑ����Ĕz�u���܂��B
                     LN_UIVALIGNMENT_STRETCH
                     �q�v�f���A�e�̃��C�A�E�g �X���b�g�S�̂Ɉ����L�΂��܂��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetAlignments
�v�f�̔z�u���@��ݒ肵�܂��B
%group
LNUIElement
%prm
uielement, halign, valign
[in] uielement : UIElement �̃n���h��
[in] halign    : 
                    LN_UIHALIGNMENT_LEFT
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�̍����ɑ����Ĕz�u���܂��B
                    LN_UIHALIGNMENT_CENTER
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�̒����ɑ����Ĕz�u���܂��B
                    LN_UIHALIGNMENT_RIGHT
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�̉E���ɑ����Ĕz�u���܂��B
                    LN_UIHALIGNMENT_STRETCH
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�S�̂Ɉ����L�΂��܂��B
[in] valign    : 
                    LN_UIVALIGNMENT_TOP
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�̏�[�ɑ����Ĕz�u���܂��B
                    LN_UIVALIGNMENT_CENTER
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�̒����ɑ����Ĕz�u���܂��B
                    LN_UIVALIGNMENT_BOTTOM
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�̉��[�ɑ����Ĕz�u���܂��B
                    LN_UIVALIGNMENT_STRETCH
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�S�̂Ɉ����L�΂��܂��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetPosition
���̃I�u�W�F�N�g�̈ʒu��ݒ肵�܂��B
%group
LNUIElement
%prm
uielement, pos
[in] uielement : UIElement �̃n���h��
[in] pos       : (Vector3 �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetPositionXYZ
���̃I�u�W�F�N�g�̈ʒu��ݒ肵�܂��B
%group
LNUIElement
%prm
uielement, x, y, z
[in] uielement   : UIElement �̃n���h��
[in] x           : 
[in] y           : 
[in] z(0.000000) : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetPosition
���̃I�u�W�F�N�g�̈ʒu���ʒu���擾���܂��B
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : (Vector3 �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetRotation
���̃I�u�W�F�N�g�̉�]��ݒ肵�܂��B
%group
LNUIElement
%prm
uielement, rot
[in] uielement : UIElement �̃n���h��
[in] rot       : (Quaternion �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetEulerAngles
���̃I�u�W�F�N�g�̉�]���I�C���[�p����ݒ肵�܂��B(radian)
%group
LNUIElement
%prm
uielement, x, y, z
[in] uielement : UIElement �̃n���h��
[in] x         : 
[in] y         : 
[in] z         : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetRotation
���̃I�u�W�F�N�g�̉�]���擾���܂��B
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : (Quaternion �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetScale
���̃I�u�W�F�N�g�̊g�嗦��ݒ肵�܂��B
%group
LNUIElement
%prm
uielement, scale
[in] uielement : UIElement �̃n���h��
[in] scale     : (Vector3 �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetScaleS
���̃I�u�W�F�N�g�̊g�嗦��ݒ肵�܂��B
%group
LNUIElement
%prm
uielement, xyz
[in] uielement : UIElement �̃n���h��
[in] xyz       : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetScaleXY
���̃I�u�W�F�N�g�̊g�嗦��ݒ肵�܂��B
%group
LNUIElement
%prm
uielement, x, y
[in] uielement : UIElement �̃n���h��
[in] x         : 
[in] y         : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetScale
���̃I�u�W�F�N�g�̊g�嗦���擾���܂��B
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : (Vector3 �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetCenterPoint
���̃I�u�W�F�N�g�̃��[�J���̒��S�ʒu��ݒ肵�܂��B
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement �̃n���h��
[in] value     : (Vector3 �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetCenterPointXYZ
���̃I�u�W�F�N�g�̃��[�J���̒��S�ʒu��ݒ肵�܂��B
%group
LNUIElement
%prm
uielement, x, y, z
[in] uielement   : UIElement �̃n���h��
[in] x           : 
[in] y           : 
[in] z(0.000000) : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetCenterPoint
���̃I�u�W�F�N�g�̃��[�J���̒��S�ʒu���擾���܂��B
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : (Vector3 �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetEnabled
�v�f�̗L����Ԃ�ݒ肵�܂��B
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement �̃n���h��
[in] value     : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_IsEnabled
�v�f�̗L����Ԃ��擾���܂��B
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetData
�C�ӂ̃��[�U�[�f�[�^��ݒ肵�܂��B
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement �̃n���h��
[in] value     : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetData
�C�ӂ̃��[�U�[�f�[�^���擾���܂��B
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetBackgroundColor
�w�i�̐F��ݒ肵�܂��B
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement �̃n���h��
[in] value     : (Color �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetBackgroundColor
�w�i�̐F���擾���܂��B
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetBorderThickness
�g���̑�����ݒ肵�܂��B
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement �̃n���h��
[in] value     : (Thickness �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetBorderThickness
�g���̑������擾���܂��B
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : (Thickness �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetBorderColor
�g���̐F��ݒ肵�܂��B
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement �̃n���h��
[in] value     : (Color �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetBorderColor
�g���̐F���擾���܂��B
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : (Color �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetCornerRadius
�l�p�`�̊p�̔��a��ݒ肵�܂��B
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement �̃n���h��
[in] value     : (CornerRadius �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetCornerRadius
�l�p�`�̊p�̔��a���擾���܂��B
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : (CornerRadius �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetVisibility
����Ԃ�ݒ肵�܂��B(default: UIVisibility::Visible)
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement �̃n���h��
[in] value     : 
                    LN_UIVISIBILITY_VISIBLE
                    �v�f��\�����܂��B
                    LN_UIVISIBILITY_HIDDEN
                    �v�f��\�����܂��񂪁A���̗v�f�̗̈�����C�A�E�g���ɗ\�񂵂܂��B
                    LN_UIVISIBILITY_COLLAPSED
                    �v�f��\�����܂���B�܂��A���̗v�f�̗̈�����C�A�E�g���ɗ\�񂵂܂���B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetVisibility
����Ԃ��擾���܂��B
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B
                     LN_UIVISIBILITY_VISIBLE
                     �v�f��\�����܂��B
                     LN_UIVISIBILITY_HIDDEN
                     �v�f��\�����܂��񂪁A���̗v�f�̗̈�����C�A�E�g���ɗ\�񂵂܂��B
                     LN_UIVISIBILITY_COLLAPSED
                     �v�f��\�����܂���B�܂��A���̗v�f�̗̈�����C�A�E�g���ɗ\�񂵂܂���B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetOpacity
�s�����x��ݒ肵�܂��B(default: 1.0)
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement �̃n���h��
[in] value     : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetOpacity
�s�����x���擾���܂��B
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_AddChild
Add element to container. �_���I�Ȏq�v�f�Ƃ��Ēǉ�����B
%group
LNUIElement
%prm
uielement, child
[in] uielement : UIElement �̃n���h��
[in] child     : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_SetFocusable
���̓t�H�[�J�X�𓾂邱�Ƃ��ł��邩�ǂ�����ݒ肵�܂��B(default: false)
%group
LNUIElement
%prm
uielement, value
[in] uielement : UIElement �̃n���h��
[in] value     : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_GetFocusable
���̓t�H�[�J�X�𓾂邱�Ƃ��ł��邩�ǂ������擾���܂��B
%group
LNUIElement
%prm
uielement, outReturn
[in]  uielement : UIElement �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIElement_AddInto
���� UIElement ���w�肵�� UIElement �֎q�v�f�Ƃ��Ēǉ����܂��B�ȗ������ꍇ�̓f�t�H���g�̃��[�g UIElement �֒ǉ����܂��B
%group
LNUIElement
%prm
uielement, parent
[in] uielement              : UIElement �̃n���h��
[in] parent(LN_NULL_HANDLE) : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIElement �� UILayoutElement �̃T�u�N���X�ł��BUIElement �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIElement �� Object �̃T�u�N���X�ł��BUIElement �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIText_Create
UIText ���쐬���܂��B
%group
LNUIText
%prm
outUIText
[out] outUIText(0) : �쐬���ꂽ UIText �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIText �� UIElement �̃T�u�N���X�ł��BUIText �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIText �� UILayoutElement �̃T�u�N���X�ł��BUIText �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIText �� Object �̃T�u�N���X�ł��BUIText �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIText_CreateWithText
�\����������w�肵�āAUITextBlock ���쐬���܂��B
%group
LNUIText
%prm
text, outUIText
[in]  text      : 
[out] outUIText(0) : �쐬���ꂽ UIText �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIText �� UIElement �̃T�u�N���X�ł��BUIText �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIText �� UILayoutElement �̃T�u�N���X�ł��BUIText �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIText �� Object �̃T�u�N���X�ł��BUIText �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIText_SetText
�\���������ݒ肵�܂��B
%group
LNUIText
%prm
uitext, value
[in] uitext : UIText �̃n���h��
[in] value  : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIText �� UIElement �̃T�u�N���X�ł��BUIText �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIText �� UILayoutElement �̃T�u�N���X�ł��BUIText �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIText �� Object �̃T�u�N���X�ł��BUIText �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIText_GetText
�\����������擾���܂��B
%group
LNUIText
%prm
uitext, outReturn
[in]  uitext    : UIText �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIText �� UIElement �̃T�u�N���X�ł��BUIText �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIText �� UILayoutElement �̃T�u�N���X�ł��BUIText �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIText �� Object �̃T�u�N���X�ł��BUIText �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUISprite_Create
UISprite ���쐬���܂��B
%group
LNUISprite
%prm
outUISprite
[out] outUISprite(0) : �쐬���ꂽ UISprite �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UISprite �� UIElement �̃T�u�N���X�ł��BUISprite �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UISprite �� UILayoutElement �̃T�u�N���X�ł��BUISprite �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UISprite �� Object �̃T�u�N���X�ł��BUISprite �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUISprite_CreateWithTexture
�e�N�X�`�����w�肵�� UISprite ���쐬���܂��B
%group
LNUISprite
%prm
texture, outUISprite
[in]  texture     : 
[out] outUISprite(0) : �쐬���ꂽ UISprite �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UISprite �� UIElement �̃T�u�N���X�ł��BUISprite �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UISprite �� UILayoutElement �̃T�u�N���X�ł��BUISprite �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UISprite �� Object �̃T�u�N���X�ł��BUISprite �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUISprite_SetTexture
�X�v���C�g���\������e�N�X�`����ݒ肵�܂��B
%group
LNUISprite
%prm
uisprite, texture
[in] uisprite : UISprite �̃n���h��
[in] texture  : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UISprite �� UIElement �̃T�u�N���X�ł��BUISprite �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UISprite �� UILayoutElement �̃T�u�N���X�ł��BUISprite �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UISprite �� Object �̃T�u�N���X�ł��BUISprite �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUISprite_SetSourceRect
�e�N�X�`���̂ǂ̕�����\�����邩�������]����`��ݒ肵�܂��B(�s�N�Z���P��) �f�t�H���g�� Rect::Empty �ŁA�e�N�X�`���S�̂�]�����邱�Ƃ������܂��B
%group
LNUISprite
%prm
uisprite, rect
[in] uisprite : UISprite �̃n���h��
[in] rect     : (Rect �^�̒l) 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UISprite �� UIElement �̃T�u�N���X�ł��BUISprite �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UISprite �� UILayoutElement �̃T�u�N���X�ł��BUISprite �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UISprite �� Object �̃T�u�N���X�ł��BUISprite �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] uisprite : UISprite �̃n���h��
[in] x        : 
[in] y        : 
[in] width    : 
[in] height   : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UISprite �� UIElement �̃T�u�N���X�ł��BUISprite �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UISprite �� UILayoutElement �̃T�u�N���X�ł��BUISprite �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UISprite �� Object �̃T�u�N���X�ł��BUISprite �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUISprite_GetSourceRect
�e�N�X�`���̂ǂ̕�����\�����邩�������]����`���擾���܂��B(�s�N�Z���P��)
%group
LNUISprite
%prm
uisprite, outReturn
[in]  uisprite  : UISprite �̃n���h��
[out] outReturn(0) : (Rect �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UISprite �� UIElement �̃T�u�N���X�ł��BUISprite �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UISprite �� UILayoutElement �̃T�u�N���X�ł��BUISprite �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UISprite �� Object �̃T�u�N���X�ł��BUISprite �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] uisprite : UISprite �̃n���h��
[in] shader   : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UISprite �� UIElement �̃T�u�N���X�ł��BUISprite �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UISprite �� UILayoutElement �̃T�u�N���X�ł��BUISprite �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UISprite �� Object �̃T�u�N���X�ł��BUISprite �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIIcon_LoadFontIcon
�A�C�R�������w�肵�� UIIcon ���쐬���܂��B
%group
LNUIIcon
%prm
iconName, outReturn
[in]  iconName  : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIIcon �� UIElement �̃T�u�N���X�ł��BUIIcon �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIIcon �� UILayoutElement �̃T�u�N���X�ł��BUIIcon �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIIcon �� Object �̃T�u�N���X�ł��BUIIcon �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIIcon_LoadFontIconWithNameSize
�A�C�R�����E�T�C�Y ���w�肵�� UIIcon ���쐬���܂��B
%group
LNUIIcon
%prm
iconName, size, outReturn
[in]  iconName  : 
[in]  size      : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIIcon �� UIElement �̃T�u�N���X�ł��BUIIcon �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIIcon �� UILayoutElement �̃T�u�N���X�ł��BUIIcon �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIIcon �� Object �̃T�u�N���X�ł��BUIIcon �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIIcon_LoadFontIconWithNameSizeColor
�A�C�R�����E�T�C�Y�E�F (TextColor) ���w�肵�� UIIcon ���쐬���܂��B
%group
LNUIIcon
%prm
iconName, size, color, outReturn
[in]  iconName  : 
[in]  size      : 
[in]  color     : (Color �^�̒l) 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIIcon �� UIElement �̃T�u�N���X�ł��BUIIcon �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIIcon �� UILayoutElement �̃T�u�N���X�ł��BUIIcon �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIIcon �� Object �̃T�u�N���X�ł��BUIIcon �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outUIMessageTextArea(0) : �쐬���ꂽ UIMessageTextArea �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIMessageTextArea �� UIElement �̃T�u�N���X�ł��BUIMessageTextArea �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIMessageTextArea �� UILayoutElement �̃T�u�N���X�ł��BUIMessageTextArea �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIMessageTextArea �� Object �̃T�u�N���X�ł��BUIMessageTextArea �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] uimessagetextarea : UIMessageTextArea �̃n���h��
[in] value             : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIMessageTextArea �� UIElement �̃T�u�N���X�ł��BUIMessageTextArea �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIMessageTextArea �� UILayoutElement �̃T�u�N���X�ł��BUIMessageTextArea �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIMessageTextArea �� Object �̃T�u�N���X�ł��BUIMessageTextArea �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] uimessagetextarea : UIMessageTextArea �̃n���h��
[in] value             : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIMessageTextArea �� UIElement �̃T�u�N���X�ł��BUIMessageTextArea �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIMessageTextArea �� UILayoutElement �̃T�u�N���X�ł��BUIMessageTextArea �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIMessageTextArea �� Object �̃T�u�N���X�ł��BUIMessageTextArea �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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

stat : 0=����, ���l=���s

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

stat : 0=����, ���l=���s

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
[out] outUIBoxLayout(0) : �쐬���ꂽ UIBoxLayout �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIBoxLayout �� UILayoutPanel �̃T�u�N���X�ł��BUIBoxLayout �n���h���� UILayoutPanel �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutPanel_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIBoxLayout �� UIElement �̃T�u�N���X�ł��BUIBoxLayout �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIBoxLayout �� UILayoutElement �̃T�u�N���X�ł��BUIBoxLayout �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIBoxLayout �� Object �̃T�u�N���X�ł��BUIBoxLayout �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIBoxLayout_SetOrientation
���C�A�E�g������ݒ肵�܂��B(default: Vertical)
%group
LNUIBoxLayout
%prm
uiboxlayout, orientation
[in] uiboxlayout : UIBoxLayout �̃n���h��
[in] orientation : 
                      LN_UILAYOUT_ORIENTATION_HORIZONTAL
                      ���������ɔz�u���܂��B
                      LN_UILAYOUT_ORIENTATION_VERTICAL
                      ���������ɔz�u���܂��B
                      LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL
                      ���������i�E���獶�j�ɔz�u���܂��B
                      LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL
                      ���������i�������j�ɔz�u���܂��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIBoxLayout �� UILayoutPanel �̃T�u�N���X�ł��BUIBoxLayout �n���h���� UILayoutPanel �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutPanel_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIBoxLayout �� UIElement �̃T�u�N���X�ł��BUIBoxLayout �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIBoxLayout �� UILayoutElement �̃T�u�N���X�ł��BUIBoxLayout �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIBoxLayout �� Object �̃T�u�N���X�ł��BUIBoxLayout �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIBoxLayout_GetOrientation
���C�A�E�g�������擾���܂��B
%group
LNUIBoxLayout
%prm
uiboxlayout, outReturn
[in]  uiboxlayout : UIBoxLayout �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B
                       LN_UILAYOUT_ORIENTATION_HORIZONTAL
                       ���������ɔz�u���܂��B
                       LN_UILAYOUT_ORIENTATION_VERTICAL
                       ���������ɔz�u���܂��B
                       LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL
                       ���������i�E���獶�j�ɔz�u���܂��B
                       LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL
                       ���������i�������j�ɔz�u���܂��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIBoxLayout �� UILayoutPanel �̃T�u�N���X�ł��BUIBoxLayout �n���h���� UILayoutPanel �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutPanel_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIBoxLayout �� UIElement �̃T�u�N���X�ł��BUIBoxLayout �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIBoxLayout �� UILayoutElement �̃T�u�N���X�ł��BUIBoxLayout �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIBoxLayout �� Object �̃T�u�N���X�ł��BUIBoxLayout �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outUIStackLayout(0) : �쐬���ꂽ UIStackLayout �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIStackLayout �� UILayoutPanel �̃T�u�N���X�ł��BUIStackLayout �n���h���� UILayoutPanel �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutPanel_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIStackLayout �� UIElement �̃T�u�N���X�ł��BUIStackLayout �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIStackLayout �� UILayoutElement �̃T�u�N���X�ł��BUIStackLayout �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIStackLayout �� Object �̃T�u�N���X�ł��BUIStackLayout �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] uistacklayout : UIStackLayout �̃n���h��
[in] orientation   : 
                        LN_UILAYOUT_ORIENTATION_HORIZONTAL
                        ���������ɔz�u���܂��B
                        LN_UILAYOUT_ORIENTATION_VERTICAL
                        ���������ɔz�u���܂��B
                        LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL
                        ���������i�E���獶�j�ɔz�u���܂��B
                        LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL
                        ���������i�������j�ɔz�u���܂��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIStackLayout �� UILayoutPanel �̃T�u�N���X�ł��BUIStackLayout �n���h���� UILayoutPanel �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutPanel_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIStackLayout �� UIElement �̃T�u�N���X�ł��BUIStackLayout �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIStackLayout �� UILayoutElement �̃T�u�N���X�ł��BUIStackLayout �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIStackLayout �� Object �̃T�u�N���X�ł��BUIStackLayout �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  uistacklayout : UIStackLayout �̃n���h��
[out] outReturn(0)  : ���ʂ��i�[����ϐ��B
                         LN_UILAYOUT_ORIENTATION_HORIZONTAL
                         ���������ɔz�u���܂��B
                         LN_UILAYOUT_ORIENTATION_VERTICAL
                         ���������ɔz�u���܂��B
                         LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL
                         ���������i�E���獶�j�ɔz�u���܂��B
                         LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL
                         ���������i�������j�ɔz�u���܂��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIStackLayout �� UILayoutPanel �̃T�u�N���X�ł��BUIStackLayout �n���h���� UILayoutPanel �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutPanel_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIStackLayout �� UIElement �̃T�u�N���X�ł��BUIStackLayout �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIStackLayout �� UILayoutElement �̃T�u�N���X�ł��BUIStackLayout �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIStackLayout �� Object �̃T�u�N���X�ł��BUIStackLayout �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outUIGridLayout(0) : �쐬���ꂽ UIGridLayout �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIGridLayout �� UILayoutPanel �̃T�u�N���X�ł��BUIGridLayout �n���h���� UILayoutPanel �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutPanel_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIGridLayout �� UIElement �̃T�u�N���X�ł��BUIGridLayout �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIGridLayout �� UILayoutElement �̃T�u�N���X�ł��BUIGridLayout �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIGridLayout �� Object �̃T�u�N���X�ł��BUIGridLayout �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] uigridlayout : UIGridLayout �̃n���h��
[in] value        : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIGridLayout �� UILayoutPanel �̃T�u�N���X�ł��BUIGridLayout �n���h���� UILayoutPanel �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutPanel_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIGridLayout �� UIElement �̃T�u�N���X�ł��BUIGridLayout �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIGridLayout �� UILayoutElement �̃T�u�N���X�ł��BUIGridLayout �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIGridLayout �� Object �̃T�u�N���X�ł��BUIGridLayout �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIGridLayout �� UILayoutPanel �̃T�u�N���X�ł��BUIGridLayout �n���h���� UILayoutPanel �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutPanel_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIGridLayout �� UIElement �̃T�u�N���X�ł��BUIGridLayout �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIGridLayout �� UILayoutElement �̃T�u�N���X�ł��BUIGridLayout �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIGridLayout �� Object �̃T�u�N���X�ł��BUIGridLayout �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIGridLayout �� UILayoutPanel �̃T�u�N���X�ł��BUIGridLayout �n���h���� UILayoutPanel �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutPanel_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIGridLayout �� UIElement �̃T�u�N���X�ł��BUIGridLayout �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIGridLayout �� UILayoutElement �̃T�u�N���X�ł��BUIGridLayout �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIGridLayout �� Object �̃T�u�N���X�ł��BUIGridLayout �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIGridLayout �� UILayoutPanel �̃T�u�N���X�ł��BUIGridLayout �n���h���� UILayoutPanel �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutPanel_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIGridLayout �� UIElement �̃T�u�N���X�ł��BUIGridLayout �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIGridLayout �� UILayoutElement �̃T�u�N���X�ł��BUIGridLayout �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIGridLayout �� Object �̃T�u�N���X�ł��BUIGridLayout �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outUIControl(0) : �쐬���ꂽ UIControl �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIControl �� UIElement �̃T�u�N���X�ł��BUIControl �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIControl �� UILayoutElement �̃T�u�N���X�ł��BUIControl �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIControl �� Object �̃T�u�N���X�ł��BUIControl �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] uicontrol : UIControl �̃n���h��
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

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIControl �� UIElement �̃T�u�N���X�ł��BUIControl �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIControl �� UILayoutElement �̃T�u�N���X�ł��BUIControl �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIControl �� Object �̃T�u�N���X�ł��BUIControl �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] uibuttonbase : UIButtonBase �̃n���h��
[in] text         : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIButtonBase �� UIControl �̃T�u�N���X�ł��BUIButtonBase �n���h���� UIControl �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIControl_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIButtonBase �� UIElement �̃T�u�N���X�ł��BUIButtonBase �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIButtonBase �� UILayoutElement �̃T�u�N���X�ł��BUIButtonBase �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIButtonBase �� Object �̃T�u�N���X�ł��BUIButtonBase �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outUIButton(0) : �쐬���ꂽ UIButton �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIButton �� UIButtonBase �̃T�u�N���X�ł��BUIButton �n���h���� UIButtonBase �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIButtonBase_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIButton �� UIControl �̃T�u�N���X�ł��BUIButton �n���h���� UIControl �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIControl_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIButton �� UIElement �̃T�u�N���X�ł��BUIButton �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIButton �� UILayoutElement �̃T�u�N���X�ł��BUIButton �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIButton �� Object �̃T�u�N���X�ł��BUIButton �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIButton_CreateWithText
�\����������w�肵�� UIButton ���쐬���܂��B
%group
LNUIButton
%prm
text, outUIButton
[in]  text        : 
[out] outUIButton(0) : �쐬���ꂽ UIButton �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIButton �� UIButtonBase �̃T�u�N���X�ł��BUIButton �n���h���� UIButtonBase �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIButtonBase_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIButton �� UIControl �̃T�u�N���X�ł��BUIButton �n���h���� UIControl �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIControl_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIButton �� UIElement �̃T�u�N���X�ł��BUIButton �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIButton �� UILayoutElement �̃T�u�N���X�ł��BUIButton �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIButton �� Object �̃T�u�N���X�ł��BUIButton �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIButton_ConnectOnClicked
Clicked �C�x���g�̒ʒm���󂯎��R�[���o�b�N��o�^���܂��B
%group
LNUIButton
%prm
uibutton, handler, outReturn
[in]  uibutton  : UIButton �̃n���h��
[in]  handler   : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIButton �� UIButtonBase �̃T�u�N���X�ł��BUIButton �n���h���� UIButtonBase �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIButtonBase_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIButton �� UIControl �̃T�u�N���X�ł��BUIButton �n���h���� UIControl �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIControl_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIButton �� UIElement �̃T�u�N���X�ł��BUIButton �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIButton �� UILayoutElement �̃T�u�N���X�ł��BUIButton �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIButton �� Object �̃T�u�N���X�ł��BUIButton �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outUIWindow(0) : �쐬���ꂽ UIWindow �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIWindow �� UIControl �̃T�u�N���X�ł��BUIWindow �n���h���� UIControl �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIControl_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIWindow �� UIElement �̃T�u�N���X�ł��BUIWindow �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIWindow �� UILayoutElement �̃T�u�N���X�ł��BUIWindow �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIWindow �� Object �̃T�u�N���X�ł��BUIWindow �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIListItem_ConnectOnSubmit
Submit �C�x���g�̒ʒm���󂯎��R�[���o�b�N��o�^���܂��B
%group
LNUIListItem
%prm
uilistitem, handler, outReturn
[in]  uilistitem : UIListItem �̃n���h��
[in]  handler    : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIListItem �� UIControl �̃T�u�N���X�ł��BUIListItem �n���h���� UIControl �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIControl_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListItem �� UIElement �̃T�u�N���X�ł��BUIListItem �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListItem �� UILayoutElement �̃T�u�N���X�ł��BUIListItem �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListItem �� Object �̃T�u�N���X�ł��BUIListItem �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] uilistitemscontrol : UIListItemsControl �̃n���h��
[in] layout             : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIListItemsControl �� UIControl �̃T�u�N���X�ł��BUIListItemsControl �n���h���� UIControl �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIControl_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListItemsControl �� UIElement �̃T�u�N���X�ł��BUIListItemsControl �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListItemsControl �� UILayoutElement �̃T�u�N���X�ł��BUIListItemsControl �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListItemsControl �� Object �̃T�u�N���X�ł��BUIListItemsControl �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  uilistitemscontrol : UIListItemsControl �̃n���h��
[out] outReturn(0)       : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIListItemsControl �� UIControl �̃T�u�N���X�ł��BUIListItemsControl �n���h���� UIControl �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIControl_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListItemsControl �� UIElement �̃T�u�N���X�ł��BUIListItemsControl �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListItemsControl �� UILayoutElement �̃T�u�N���X�ł��BUIListItemsControl �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListItemsControl �� Object �̃T�u�N���X�ł��BUIListItemsControl �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] uilistitemscontrol : UIListItemsControl �̃n���h��
[in] value              : 
                             LN_UILIST_SUBMIT_MODE_SINGLE
                             �Q�[��UI�p�BHover �őI����ԁA�V���O���N���b�N�� Submit. Hover ��Ԃ͎g�p����Ȃ��B
                             LN_UILIST_SUBMIT_MODE_DOUBLE
                             �G�f�B�^UI�p�B�V���O���N���b�N�őI����ԁA�_�u���N���b�N�� Submit.

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIListItemsControl �� UIControl �̃T�u�N���X�ł��BUIListItemsControl �n���h���� UIControl �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIControl_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListItemsControl �� UIElement �̃T�u�N���X�ł��BUIListItemsControl �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListItemsControl �� UILayoutElement �̃T�u�N���X�ł��BUIListItemsControl �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListItemsControl �� Object �̃T�u�N���X�ł��BUIListItemsControl �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  uilistitemscontrol : UIListItemsControl �̃n���h��
[out] outReturn(0)       : ���ʂ��i�[����ϐ��B
                              LN_UILIST_SUBMIT_MODE_SINGLE
                              �Q�[��UI�p�BHover �őI����ԁA�V���O���N���b�N�� Submit. Hover ��Ԃ͎g�p����Ȃ��B
                              LN_UILIST_SUBMIT_MODE_DOUBLE
                              �G�f�B�^UI�p�B�V���O���N���b�N�őI����ԁA�_�u���N���b�N�� Submit.

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIListItemsControl �� UIControl �̃T�u�N���X�ł��BUIListItemsControl �n���h���� UIControl �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIControl_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListItemsControl �� UIElement �̃T�u�N���X�ł��BUIListItemsControl �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListItemsControl �� UILayoutElement �̃T�u�N���X�ł��BUIListItemsControl �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListItemsControl �� Object �̃T�u�N���X�ł��BUIListItemsControl �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIListItemsControl_ConnectOnSubmit
Submit �C�x���g�̒ʒm���󂯎��R�[���o�b�N��o�^���܂��B
%group
LNUIListItemsControl
%prm
uilistitemscontrol, handler, outReturn
[in]  uilistitemscontrol : UIListItemsControl �̃n���h��
[in]  handler            : 
[out] outReturn(0)       : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIListItemsControl �� UIControl �̃T�u�N���X�ł��BUIListItemsControl �n���h���� UIControl �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIControl_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListItemsControl �� UIElement �̃T�u�N���X�ł��BUIListItemsControl �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListItemsControl �� UILayoutElement �̃T�u�N���X�ł��BUIListItemsControl �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListItemsControl �� Object �̃T�u�N���X�ł��BUIListItemsControl �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outUIListBoxItem(0) : �쐬���ꂽ UIListBoxItem �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIListBoxItem �� UIListItem �̃T�u�N���X�ł��BUIListBoxItem �n���h���� UIListItem �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIListItem_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListBoxItem �� UIControl �̃T�u�N���X�ł��BUIListBoxItem �n���h���� UIControl �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIControl_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListBoxItem �� UIElement �̃T�u�N���X�ł��BUIListBoxItem �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListBoxItem �� UILayoutElement �̃T�u�N���X�ł��BUIListBoxItem �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListBoxItem �� Object �̃T�u�N���X�ł��BUIListBoxItem �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outUIListBox(0) : �쐬���ꂽ UIListBox �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIListBox �� UIListItemsControl �̃T�u�N���X�ł��BUIListBox �n���h���� UIListItemsControl �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIListItemsControl_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListBox �� UIControl �̃T�u�N���X�ł��BUIListBox �n���h���� UIControl �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIControl_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListBox �� UIElement �̃T�u�N���X�ł��BUIListBox �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListBox �� UILayoutElement �̃T�u�N���X�ł��BUIListBox �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListBox �� Object �̃T�u�N���X�ł��BUIListBox �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUIListBox_AddItem
UIListBoxItem ��ǉ����A���̃C���X�^���X��Ԃ��܂��B
%group
LNUIListBox
%prm
uilistbox, content, outReturn
[in]  uilistbox : UIListBox �̃n���h��
[in]  content   : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
UIListBox �� UIListItemsControl �̃T�u�N���X�ł��BUIListBox �n���h���� UIListItemsControl �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIListItemsControl_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListBox �� UIControl �̃T�u�N���X�ł��BUIListBox �n���h���� UIControl �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIControl_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListBox �� UIElement �̃T�u�N���X�ł��BUIListBox �n���h���� UIElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUIElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListBox �� UILayoutElement �̃T�u�N���X�ł��BUIListBox �n���h���� UILayoutElement �n���h���Ƃ��Ĉ������Ƃ��ł��A LNUILayoutElement_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
UIListBox �� Object �̃T�u�N���X�ł��BUIListBox �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
                         [�񐄏� GLFW�ł�;]
                         LN_KEYS_SEMICOLON
                         ;	[�񐄏� GLFW�ł�=]
                         LN_KEYS_COMMA
                         ,
                         LN_KEYS_PERIOD
                         .
                         LN_KEYS_SLASH
                         /
                         LN_KEYS_MINUS
                         -
                         LN_KEYS_BACKSLASH
                         \(�o�b�N�X���b�V��) [�񐄏� GLFW�ł�NOWORD2]
                         LN_KEYS_YEN
                         \(BaskSpace�L�[�̍���\�L�[) [�񐄏� GLFW�ł�I]
                         LN_KEYS_CARET
                         ^ [�񐄏� GLFW�ł�GLFW_KEY_APOSTROPHE(')]
                         LN_KEYS_LBRACKET
                         [
                         LN_KEYS_RBRACKET
                         ]
                         LN_KEYS_ANDROID_PLUS
                         Android '+'
                         LN_KEYS_TERMINATOR
                         
[out] outKeyGesture(0) : �쐬���ꂽ KeyGesture �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
KeyGesture �� InputGesture �̃T�u�N���X�ł��BKeyGesture �n���h���� InputGesture �n���h���Ƃ��Ĉ������Ƃ��ł��A LNInputGesture_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B
KeyGesture �� Object �̃T�u�N���X�ł��BKeyGesture �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInput_IsPressed
�{�^�������݉�����Ă��邩�𔻒肵�܂��B
%group
LNInput
%prm
buttonName, outReturn
[in]  buttonName : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInput_IsTriggered
�{�^�����V���������ꂽ�u�Ԃ𔻒肵�܂��B
%group
LNInput
%prm
buttonName, outReturn
[in]  buttonName : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInput_IsTriggeredOff
�{�^���������ꂽ�u�Ԃ𔻒肵�܂��B
%group
LNInput
%prm
buttonName, outReturn
[in]  buttonName : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInput_IsRepeated
�{�^�����V���������ꂽ�u�Ԃƃ��s�[�g��Ԃ𔻒肵�܂��B
%group
LNInput
%prm
buttonName, outReturn
[in]  buttonName : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInput_GetAxisValue
�w�肵�����̃A�i���O�l���擾���܂��B
%group
LNInput
%prm
buttonName, outReturn
[in]  buttonName : 
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInput_AddBinding
�{�^���̃A�N�V�����}�b�s���O��ǉ����܂��B
%group
LNInput
%prm
buttonName, gesture
[in] buttonName : 
[in] gesture    : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInput_RemoveBinding
�A�N�V�����}�b�s���O�����O���܂��B
%group
LNInput
%prm
gesture
[in] gesture : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInput_ClearBindings
�w�肵���{�^���̃A�N�V�����}�b�s���O�����ׂč폜���܂��B
%group
LNInput
%prm
buttonName
[in] buttonName : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInput_ClearAllBindings
�S�ẴA�N�V�����}�b�s���O���폜���܂��B
%group
LNInput
%prm


stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMouse_Pressed
�{�^�������݉�����Ă��邩�𔻒肵�܂��B
%group
LNMouse
%prm
button, outReturn
[in]  button    : 
                     LN_MOUSE_BUTTONS_NONE
                     �����܂��͉�����Ă��Ȃ����Ƃ�����
                     LN_MOUSE_BUTTONS_LEFT
                     ���{�^��
                     LN_MOUSE_BUTTONS_RIGHT
                     �E�{�^��
                     LN_MOUSE_BUTTONS_MIDDLE
                     �����{�^��
                     LN_MOUSE_BUTTONS_X1
                     �g���{�^��1
                     LN_MOUSE_BUTTONS_X2
                     �g���{�^��2
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMouse_Triggered
�{�^�����V���������ꂽ�u�Ԃ𔻒肵�܂��B
%group
LNMouse
%prm
button, outReturn
[in]  button    : 
                     LN_MOUSE_BUTTONS_NONE
                     �����܂��͉�����Ă��Ȃ����Ƃ�����
                     LN_MOUSE_BUTTONS_LEFT
                     ���{�^��
                     LN_MOUSE_BUTTONS_RIGHT
                     �E�{�^��
                     LN_MOUSE_BUTTONS_MIDDLE
                     �����{�^��
                     LN_MOUSE_BUTTONS_X1
                     �g���{�^��1
                     LN_MOUSE_BUTTONS_X2
                     �g���{�^��2
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMouse_TriggeredOff
�{�^���������ꂽ�u�Ԃ𔻒肵�܂��B
%group
LNMouse
%prm
button, outReturn
[in]  button    : 
                     LN_MOUSE_BUTTONS_NONE
                     �����܂��͉�����Ă��Ȃ����Ƃ�����
                     LN_MOUSE_BUTTONS_LEFT
                     ���{�^��
                     LN_MOUSE_BUTTONS_RIGHT
                     �E�{�^��
                     LN_MOUSE_BUTTONS_MIDDLE
                     �����{�^��
                     LN_MOUSE_BUTTONS_X1
                     �g���{�^��1
                     LN_MOUSE_BUTTONS_X2
                     �g���{�^��2
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMouse_Repeated
�{�^�����V���������ꂽ�u�Ԃƃ��s�[�g��Ԃ𔻒肵�܂��B
%group
LNMouse
%prm
button, outReturn
[in]  button    : 
                     LN_MOUSE_BUTTONS_NONE
                     �����܂��͉�����Ă��Ȃ����Ƃ�����
                     LN_MOUSE_BUTTONS_LEFT
                     ���{�^��
                     LN_MOUSE_BUTTONS_RIGHT
                     �E�{�^��
                     LN_MOUSE_BUTTONS_MIDDLE
                     �����{�^��
                     LN_MOUSE_BUTTONS_X1
                     �g���{�^��1
                     LN_MOUSE_BUTTONS_X2
                     �g���{�^��2
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMouse_GetPosition
�}�E�X�|�C���^�̈ʒu���擾���܂��B
%group
LNMouse
%prm
outReturn
[out] outReturn(0) : (Point �^�̒l) ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommand_Code
�R�}���h�̎��s�R�[�h���擾���܂��B
%group
LNInterpreterCommand
%prm
interpretercommand, outReturn
[in]  interpretercommand : InterpreterCommand �̃n���h��
[out] outReturn(0)       : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
InterpreterCommand �� Object �̃T�u�N���X�ł��BInterpreterCommand �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommand_ParamsCount
�R�}���h�̃p�����[�^�����擾���܂��B
%group
LNInterpreterCommand
%prm
interpretercommand, outReturn
[in]  interpretercommand : InterpreterCommand �̃n���h��
[out] outReturn(0)       : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
InterpreterCommand �� Object �̃T�u�N���X�ł��BInterpreterCommand �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommand_Param
�R�}���h�̃p�����[�^���擾���܂��B
%group
LNInterpreterCommand
%prm
interpretercommand, index, outReturn
[in]  interpretercommand : InterpreterCommand �̃n���h��
[in]  index              : 
[out] outReturn(0)       : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
InterpreterCommand �� Object �̃T�u�N���X�ł��BInterpreterCommand �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommandList_Create
InterpreterCommandList ���쐬���܂��B
%group
LNInterpreterCommandList
%prm
outInterpreterCommandList
[out] outInterpreterCommandList(0) : �쐬���ꂽ InterpreterCommandList �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
InterpreterCommandList �� Object �̃T�u�N���X�ł��BInterpreterCommandList �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommandList_AddCommand
�R�}���h��ǉ����܂��B
%group
LNInterpreterCommandList
%prm
interpretercommandlist, code
[in] interpretercommandlist : InterpreterCommandList �̃n���h��
[in] code                   : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
InterpreterCommandList �� Object �̃T�u�N���X�ł��BInterpreterCommandList �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommandList_AddCommand1
�R�}���h�� 1 �̃p�����[�^��ǉ����܂��B
%group
LNInterpreterCommandList
%prm
interpretercommandlist, code, param0
[in] interpretercommandlist : InterpreterCommandList �̃n���h��
[in] code                   : 
[in] param0                 : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
InterpreterCommandList �� Object �̃T�u�N���X�ł��BInterpreterCommandList �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommandList_AddCommand2
�R�}���h�� 2 �̃p�����[�^��ǉ����܂��B
%group
LNInterpreterCommandList
%prm
interpretercommandlist, code, param0, param1
[in] interpretercommandlist : InterpreterCommandList �̃n���h��
[in] code                   : 
[in] param0                 : 
[in] param1                 : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
InterpreterCommandList �� Object �̃T�u�N���X�ł��BInterpreterCommandList �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommandList_AddCommand3
�R�}���h�� 3 �̃p�����[�^��ǉ����܂��B
%group
LNInterpreterCommandList
%prm
interpretercommandlist, code, param0, param1, param2
[in] interpretercommandlist : InterpreterCommandList �̃n���h��
[in] code                   : 
[in] param0                 : 
[in] param1                 : 
[in] param2                 : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
InterpreterCommandList �� Object �̃T�u�N���X�ł��BInterpreterCommandList �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreterCommandList_AddCommand4
�R�}���h�� 4 �̃p�����[�^��ǉ����܂��B
%group
LNInterpreterCommandList
%prm
interpretercommandlist, code, param0, param1, param2, param3
[in] interpretercommandlist : InterpreterCommandList �̃n���h��
[in] code                   : 
[in] param0                 : 
[in] param1                 : 
[in] param2                 : 
[in] param3                 : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
InterpreterCommandList �� Object �̃T�u�N���X�ł��BInterpreterCommandList �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[out] outInterpreterCommandDelegate(0) : �쐬���ꂽ InterpreterCommandDelegate �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
InterpreterCommandDelegate �� Object �̃T�u�N���X�ł��BInterpreterCommandDelegate �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreter_Create
Interpreter ���쐬���܂��B
%group
LNInterpreter
%prm
outInterpreter
[out] outInterpreter(0) : �쐬���ꂽ Interpreter �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Interpreter �� Object �̃T�u�N���X�ł��BInterpreter �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreter_Clear
�R�}���h���X�g���N���A���܂��B���s���̃R�}���h���X�g�͋����I�ɔj������܂��B
%group
LNInterpreter
%prm
interpreter
[in] interpreter : Interpreter �̃n���h��

stat : 0=����, ���l=���s

%inst


���l
--------------------
Interpreter �� Object �̃T�u�N���X�ł��BInterpreter �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreter_Run
�R�}���h���X�g��ݒ肵�A���s���J�n���܂��B
%group
LNInterpreter
%prm
interpreter, commandList
[in] interpreter : Interpreter �̃n���h��
[in] commandList : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Interpreter �� Object �̃T�u�N���X�ł��BInterpreter �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNInterpreter_IsRunning
�R�}���h���X�g�̎��s���ł��邩���m�F���܂��B
%group
LNInterpreter
%prm
interpreter, outReturn
[in]  interpreter : Interpreter �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Interpreter �� Object �̃T�u�N���X�ł��BInterpreter �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] interpreter : Interpreter �̃n���h��

stat : 0=����, ���l=���s

%inst


���l
--------------------
Interpreter �� Object �̃T�u�N���X�ł��BInterpreter �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] interpreter : Interpreter �̃n���h��

stat : 0=����, ���l=���s

%inst


���l
--------------------
Interpreter �� Object �̃T�u�N���X�ł��BInterpreter �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] interpreter : Interpreter �̃n���h��
[in] name        : 
[in] handler     : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Interpreter �� Object �̃T�u�N���X�ł��BInterpreter �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] interpreter : Interpreter �̃n���h��
[in] mode        : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Interpreter �� Object �̃T�u�N���X�ł��BInterpreter �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  interpreter : Interpreter �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Interpreter �� Object �̃T�u�N���X�ł��BInterpreter �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in] interpreter : Interpreter �̃n���h��
[in] count       : 

stat : 0=����, ���l=���s

%inst


���l
--------------------
Interpreter �� Object �̃T�u�N���X�ł��BInterpreter �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

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
[in]  interpreter : Interpreter �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Interpreter �� Object �̃T�u�N���X�ł��BInterpreter �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetMainWindowTitle
���C���E�B���h�E�̃^�C�g���������ݒ肵�܂��B
%group
LNEngineSettings
%prm
title
[in] title : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetMainWindowSize
���C���E�B���h�E�̃N���C�A���g�̈�̕��ƍ�����ݒ肵�܂��B(default: 640x480)
%group
LNEngineSettings
%prm
width, height
[in] width  : 
[in] height : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetMainWorldViewSize
���C���E�B���h�E�ɑ΂��č쐬����� WorldView �̃T�C�Y��ݒ肵�܂��B(default: �N���C�A���g�̈�̃T�C�Y�Ɠ���)
%group
LNEngineSettings
%prm
width, height
[in] width  : 
[in] height : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetMainWindowResizable
���C���E�B���h�E�̃T�C�Y�����[�U�[���ύX�ł��邩�ǂ������w�肵�܂��B(default: false)
%group
LNEngineSettings
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_AddAssetDirectory
�A�Z�b�g���ۑ�����Ă���f�B���N�g����o�^���܂��B
%group
LNEngineSettings
%prm
path
[in] path : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_AddAssetArchive
�A�Z�b�g�t�@�C����o�^���܂��B
%group
LNEngineSettings
%prm
fileFullPath, password
[in] fileFullPath : 
[in] password     : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetFrameRate
�t���[�����[�g��ݒ肵�܂��B(default: 60)
%group
LNEngineSettings
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetUITheme
�f�t�H���g�� UI �e�[�}����ݒ肵�܂��B
%group
LNEngineSettings
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetFontFile
�f�t�H���g�̃t�H���g�t�@�C����ݒ肵�܂��B
%group
LNEngineSettings
%prm
filePath
[in] filePath : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetDebugToolEnabled
(default: Debug �r���h�̏ꍇtrue�A����ȊO�� false)
%group
LNEngineSettings
%prm
enabled
[in] enabled : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetDebugMode
�f�o�b�O���[�h�̗L����ݒ肵�܂��B(default: Debug �r���h�̏ꍇtrue�A����ȊO�� false)
%group
LNEngineSettings
%prm
enabled
[in] enabled : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetEngineLogFilePath
�f�o�b�O�p�̃��O�t�@�C���̏o�͐�t�@�C���p�X��ݒ肵�܂��B(default: Empty(���s�t�@�C���̃f�B���N�g���֏o��))
%group
LNEngineSettings
%prm
filePath
[in] filePath : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetPriorityGPUName
�D��I�Ɏg�p���� GPU �̖��O���w�肵�܂��B
%group
LNEngineSettings
%prm
filePath
[in] filePath : 

stat : 0=����, ���l=���s

%inst
���݂� DirectX12 ���g�p����ꍇ�̂ݗL���ŁA�f�t�H���g�� GPU �ł͓��삪�s����ȏꍇ�� "Microsoft Basic Render Driver" �����������Ƃ��ł��܂��B

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

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetUserMainWindow
���[�U�[�w��̃��C���E�B���h�E�̃E�B���h�E�n���h����ݒ肵�܂��B
%group
LNEngineSettings
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_Initialize
�G���W���̏������������s���܂��B
%group
LNEngine
%prm


stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_Terminate
�G���W���̏I���������s���܂��B
%group
LNEngine
%prm


stat : 0=����, ���l=���s

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
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_GetTime
�A�v���P�[�V�����J�n����̌o�ߎ��Ԃ��擾���܂��B���̒l�̓^�C���X�P�[���̉e�����󂯂܂��B
%group
LNEngine
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_GetWorld
�f�t�H���g�ō쐬����郁�C���� World �ł��B
%group
LNEngine
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_GetMainCamera
�f�t�H���g�ō쐬����郁�C���� Camera �ł��B
%group
LNEngine
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_GetMainLight
�f�t�H���g�ō쐬����郁�C���� Light �ł��B
%group
LNEngine
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_GetRenderView
�f�t�H���g�ō쐬����郁�C���� RenderView �ł��B
%group
LNEngine
%prm
outReturn
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outApplication(0) : �쐬���ꂽ Application �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Application �� Object �̃T�u�N���X�ł��BApplication �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNApplication_OnInit
�G���W���̏���������������������ɌĂяo����܂��B
%group
LNApplication
%prm
application
[in] application : Application �̃n���h��

stat : 0=����, ���l=���s

%inst


���l
--------------------
Application �� Object �̃T�u�N���X�ł��BApplication �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNApplication_OnUpdate
���t���[���Ăяo����܂��B
%group
LNApplication
%prm
application
[in] application : Application �̃n���h��

stat : 0=����, ���l=���s

%inst


���l
--------------------
Application �� Object �̃T�u�N���X�ł��BApplication �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNApplication_World
�f�t�H���g�ō쐬����郁�C���� World ���擾���܂��B
%group
LNApplication
%prm
application, outReturn
[in]  application : Application �̃n���h��
[out] outReturn(0) : ���ʂ��i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


���l
--------------------
Application �� Object �̃T�u�N���X�ł��BApplication �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNApplication_Run
Application �̎��s���J�n���܂��B
%group
LNApplication
%prm
application
[in] application : Application �̃n���h��

stat : 0=����, ���l=���s

%inst


���l
--------------------
Application �� Object �̃T�u�N���X�ł��BApplication �n���h���� Object �n���h���Ƃ��Ĉ������Ƃ��ł��A LNObject_ ����n�܂閽�ߓ��Ŏg�p�ł��܂��B

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDebug_SetGuideGridEnabled
3D �V�[����ɁA�O���b�h��\�����邩�ǂ�����ݒ肵�܂��B
%group
LNDebug
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDebug_SetPhysicsDebugDrawEnabled
3D �V�[����ɁA�R���W�����y�ѕ������Z�̏���\�����邩�ǂ�����ݒ肵�܂��B
%group
LNDebug
%prm
value
[in] value : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDebug_Print
�E�B���h�E��Ƀf�o�b�O�������\�����܂��B
%group
LNDebug
%prm
str
[in] str : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDebug_PrintWithTime
�\�����Ԃ��w�肵�āA�E�B���h�E��Ƀf�o�b�O�������\�����܂��B
%group
LNDebug
%prm
time, str
[in] time : 
[in] str  : 

stat : 0=����, ���l=���s

%inst


%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNDebug_PrintWithTimeAndColor
�\�����Ԃƕ����F���w�肵�āA�E�B���h�E��Ƀf�o�b�O�������\�����܂��B
%group
LNDebug
%prm
time, color, str
[in] time  : 
[in] color : (Color �^�̒l) 
[in] str   : 

stat : 0=����, ���l=���s

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
[out] outObjectSerializeHandler(0) : �쐬���ꂽ ObjectSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outEventConnectionSerializeHandler(0) : �쐬���ꂽ EventConnectionSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outVariantSerializeHandler(0) : �쐬���ꂽ VariantSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outZVTestClass1SerializeHandler(0) : �쐬���ꂽ ZVTestClass1SerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outZVTestEventArgs1SerializeHandler(0) : �쐬���ꂽ ZVTestEventArgs1SerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outSerializer2SerializeHandler(0) : �쐬���ꂽ Serializer2SerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outAssetObjectSerializeHandler(0) : �쐬���ꂽ AssetObjectSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outAssetImportSettingsSerializeHandler(0) : �쐬���ꂽ AssetImportSettingsSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outAssetModelSerializeHandler(0) : �쐬���ꂽ AssetModelSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outSoundSerializeHandler(0) : �쐬���ꂽ SoundSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outTextureSerializeHandler(0) : �쐬���ꂽ TextureSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outTexture2DSerializeHandler(0) : �쐬���ꂽ Texture2DSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outShaderSerializeHandler(0) : �쐬���ꂽ ShaderSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outRenderViewSerializeHandler(0) : �쐬���ꂽ RenderViewSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outMaterialSerializeHandler(0) : �쐬���ꂽ MaterialSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outMeshNodeSerializeHandler(0) : �쐬���ꂽ MeshNodeSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outAnimationControllerSerializeHandler(0) : �쐬���ꂽ AnimationControllerSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outMeshModelSerializeHandler(0) : �쐬���ꂽ MeshModelSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outMeshImportSettingsSerializeHandler(0) : �쐬���ꂽ MeshImportSettingsSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outSkinnedMeshModelSerializeHandler(0) : �쐬���ꂽ SkinnedMeshModelSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outCollisionShapeSerializeHandler(0) : �쐬���ꂽ CollisionShapeSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outBoxCollisionShapeSerializeHandler(0) : �쐬���ꂽ BoxCollisionShapeSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outAnimationCurveSerializeHandler(0) : �쐬���ꂽ AnimationCurveSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outKeyFrameAnimationCurveSerializeHandler(0) : �쐬���ꂽ KeyFrameAnimationCurveSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outAnimationClipSerializeHandler(0) : �쐬���ꂽ AnimationClipSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outAnimationStateSerializeHandler(0) : �쐬���ꂽ AnimationStateSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outEffectResourceSerializeHandler(0) : �쐬���ꂽ EffectResourceSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outParticleEmitterModelSerializeHandler(0) : �쐬���ꂽ ParticleEmitterModelSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outParticleModelSerializeHandler(0) : �쐬���ꂽ ParticleModelSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outComponentSerializeHandler(0) : �쐬���ꂽ ComponentSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outVisualComponentSerializeHandler(0) : �쐬���ꂽ VisualComponentSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outSpriteComponentSerializeHandler(0) : �쐬���ꂽ SpriteComponentSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outCharacterControllerSerializeHandler(0) : �쐬���ꂽ CharacterControllerSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outWorldSerializeHandler(0) : �쐬���ꂽ WorldSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outComponentListSerializeHandler(0) : �쐬���ꂽ ComponentListSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outWorldObjectSerializeHandler(0) : �쐬���ꂽ WorldObjectSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outWorldObjectPreUpdateHandler(0) : �쐬���ꂽ WorldObjectPreUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outWorldObjectUpdateHandler(0) : �쐬���ꂽ WorldObjectUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outVisualObjectSerializeHandler(0) : �쐬���ꂽ VisualObjectSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outVisualObjectPreUpdateHandler(0) : �쐬���ꂽ VisualObjectPreUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outVisualObjectUpdateHandler(0) : �쐬���ꂽ VisualObjectUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outCameraSerializeHandler(0) : �쐬���ꂽ CameraSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outCameraPreUpdateHandler(0) : �쐬���ꂽ CameraPreUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outCameraUpdateHandler(0) : �쐬���ꂽ CameraUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outEnvironmentLightSerializeHandler(0) : �쐬���ꂽ EnvironmentLightSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outEnvironmentLightPreUpdateHandler(0) : �쐬���ꂽ EnvironmentLightPreUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outEnvironmentLightUpdateHandler(0) : �쐬���ꂽ EnvironmentLightUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outDirectionalLightSerializeHandler(0) : �쐬���ꂽ DirectionalLightSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outDirectionalLightPreUpdateHandler(0) : �쐬���ꂽ DirectionalLightPreUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outDirectionalLightUpdateHandler(0) : �쐬���ꂽ DirectionalLightUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outPointLightSerializeHandler(0) : �쐬���ꂽ PointLightSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outPointLightPreUpdateHandler(0) : �쐬���ꂽ PointLightPreUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outPointLightUpdateHandler(0) : �쐬���ꂽ PointLightUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outSpotLightSerializeHandler(0) : �쐬���ꂽ SpotLightSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outSpotLightPreUpdateHandler(0) : �쐬���ꂽ SpotLightPreUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outSpotLightUpdateHandler(0) : �쐬���ꂽ SpotLightUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outSpriteSerializeHandler(0) : �쐬���ꂽ SpriteSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outSpritePreUpdateHandler(0) : �쐬���ꂽ SpritePreUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outSpriteUpdateHandler(0) : �쐬���ꂽ SpriteUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outCameraOrbitControlComponentSerializeHandler(0) : �쐬���ꂽ CameraOrbitControlComponentSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outRaycasterSerializeHandler(0) : �쐬���ꂽ RaycasterSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outRaycastResultSerializeHandler(0) : �쐬���ꂽ RaycastResultSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outWorldRenderViewSerializeHandler(0) : �쐬���ꂽ WorldRenderViewSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outShapeObjectSerializeHandler(0) : �쐬���ꂽ ShapeObjectSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outShapeObjectPreUpdateHandler(0) : �쐬���ꂽ ShapeObjectPreUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outShapeObjectUpdateHandler(0) : �쐬���ꂽ ShapeObjectUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outPlaneMeshSerializeHandler(0) : �쐬���ꂽ PlaneMeshSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outPlaneMeshPreUpdateHandler(0) : �쐬���ꂽ PlaneMeshPreUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outPlaneMeshUpdateHandler(0) : �쐬���ꂽ PlaneMeshUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outBoxMeshSerializeHandler(0) : �쐬���ꂽ BoxMeshSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outBoxMeshPreUpdateHandler(0) : �쐬���ꂽ BoxMeshPreUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outBoxMeshUpdateHandler(0) : �쐬���ꂽ BoxMeshUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outMeshComponentSerializeHandler(0) : �쐬���ꂽ MeshComponentSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outCollisionSerializeHandler(0) : �쐬���ꂽ CollisionSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outTriggerBodyComponentSerializeHandler(0) : �쐬���ꂽ TriggerBodyComponentSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outParticleEmitterSerializeHandler(0) : �쐬���ꂽ ParticleEmitterSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outParticleEmitterPreUpdateHandler(0) : �쐬���ꂽ ParticleEmitterPreUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outParticleEmitterUpdateHandler(0) : �쐬���ꂽ ParticleEmitterUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outLevelSerializeHandler(0) : �쐬���ꂽ LevelSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outLevelStartHandler(0) : �쐬���ꂽ LevelStartHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outLevelStopHandler(0) : �쐬���ꂽ LevelStopHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outLevelPauseHandler(0) : �쐬���ꂽ LevelPauseHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outLevelResumeHandler(0) : �쐬���ꂽ LevelResumeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outLevelUpdateHandler(0) : �쐬���ꂽ LevelUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUIEventArgsSerializeHandler(0) : �쐬���ꂽ UIEventArgsSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUILayoutElementSerializeHandler(0) : �쐬���ꂽ UILayoutElementSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUIElementSerializeHandler(0) : �쐬���ꂽ UIElementSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUITextSerializeHandler(0) : �쐬���ꂽ UITextSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUISpriteSerializeHandler(0) : �쐬���ꂽ UISpriteSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUIIconSerializeHandler(0) : �쐬���ꂽ UIIconSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUIMessageTextAreaSerializeHandler(0) : �쐬���ꂽ UIMessageTextAreaSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUILayoutPanelSerializeHandler(0) : �쐬���ꂽ UILayoutPanelSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUIBoxLayoutSerializeHandler(0) : �쐬���ꂽ UIBoxLayoutSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUIStackLayoutSerializeHandler(0) : �쐬���ꂽ UIStackLayoutSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUIGridLayoutSerializeHandler(0) : �쐬���ꂽ UIGridLayoutSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUIControlSerializeHandler(0) : �쐬���ꂽ UIControlSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUIButtonBaseSerializeHandler(0) : �쐬���ꂽ UIButtonBaseSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUIButtonSerializeHandler(0) : �쐬���ꂽ UIButtonSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUIWindowSerializeHandler(0) : �쐬���ꂽ UIWindowSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUIListItemSerializeHandler(0) : �쐬���ꂽ UIListItemSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUIListItemsControlSerializeHandler(0) : �쐬���ꂽ UIListItemsControlSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUIListBoxItemSerializeHandler(0) : �쐬���ꂽ UIListBoxItemSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outUIListBoxSerializeHandler(0) : �쐬���ꂽ UIListBoxSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outInputGestureSerializeHandler(0) : �쐬���ꂽ InputGestureSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outKeyGestureSerializeHandler(0) : �쐬���ꂽ KeyGestureSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outInterpreterCommandSerializeHandler(0) : �쐬���ꂽ InterpreterCommandSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outInterpreterCommandListSerializeHandler(0) : �쐬���ꂽ InterpreterCommandListSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outInterpreterSerializeHandler(0) : �쐬���ꂽ InterpreterSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outInterpreterUpdateWaitHandler(0) : �쐬���ꂽ InterpreterUpdateWaitHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outApplicationSerializeHandler(0) : �쐬���ꂽ ApplicationSerializeHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outApplicationInitHandler(0) : �쐬���ꂽ ApplicationInitHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

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
[out] outApplicationUpdateHandler(0) : �쐬���ꂽ ApplicationUpdateHandler �̃n���h�����i�[����ϐ��B

stat : 0=����, ���l=���s

%inst


%href


