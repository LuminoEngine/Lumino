
;============================================================
; Lumino �w���v�t�@�C��
;============================================================

%dll
Lumino

%ver
0.9.0

%date
2020/12/21

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
LNVector3_GetX

%group
LNVector3
%prm
vector3, outReturn
[in]  vector3   : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

%href



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
[in] vector3 : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] vector3 : instance
[in] x       : 
[in] y       : 
[in] z       : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in]  vector3   : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in]  vector3   : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] vector3 : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in]  vec       : 
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] vector4 : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] vector4 : instance
[in] x       : 
[in] y       : 
[in] z       : 
[in] w       : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] quaternion : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] quaternion : instance
[in] x          : 
[in] y          : 
[in] z          : 
[in] w          : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] quaternion : instance
[in] axis       : 
[in] r          : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
axis ���P�ʃx�N�g���łȂ���ΐ��K�����Ă���v�Z���s���܂��B
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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] matrix : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] color : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] color        : instance
[in] r_           : 
[in] g_           : 
[in] b_           : 
[in] a_(1.000000) : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] colortone : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] colortone : instance
[in] r_        : 
[in] g_        : 
[in] b_        : 
[in] s_        : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] point : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] point : instance
[in] x_    : 
[in] y_    : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] size : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] size : instance
[in] w    : 
[in] h    : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] rect : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] rect   : instance
[in] x      : 
[in] y      : 
[in] width  : 
[in] height : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in]  rect      : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] rect : instance
[in] size : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in]  rect      : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] thickness : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] thickness : instance
[in] left_     : 
[in] top_      : 
[in] right_    : 
[in] bottom_   : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] cornerradius : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] cornerradius : instance
[in] topLeft      : 
[in] topRight     : 
[in] bottomRight  : 
[in] bottomLeft   : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] object : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] object : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in]  object    : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outPromiseFailureDelegate(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outVariant(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outZVTestDelegate1(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outZVTestDelegate2(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outZVTestDelegate3(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outZVTestEventHandler1(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outZVTestEventHandler2(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outZVTestClass1(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outZVTestEventArgs1(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outZVTestEventArgs1(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0)     : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outAssetModel(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in]  filePath                    : 
[in]  encoding((LNEncodingType)0) : 
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
[out] outReturn(0)                : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] sound : instance
[in] value : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  sound     : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] sound : instance
[in] value : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  sound     : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] sound   : instance
[in] enabled : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  sound     : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] sound  : instance
[in] begin  : 
[in] length : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] sound : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] sound : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] sound : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] sound : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] sound        : instance
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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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


stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] position         : 
[in] distance         : 
[in] volume(1.000000) : 
[in] pitch(1.000000)  : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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


stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outTexture2DDelegate(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.
                     LN_GRAPHICS_API_DEFAULT
                     �����I��
                     LN_GRAPHICS_API_OPEN_GL
                     OpenGL
                     LN_GRAPHICS_API_VULKAN
                     Vulkan

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] width        : 
[in] height       : 
[in] outTexture2D(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] width        : 
[in] height       : 
[in] format       : 
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
[in] outTexture2D(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�T�|�[�g���Ă���t�H�[�}�b�g�͎��̒ʂ�ł��BPNG(.png), JPG(.jpg), TGA(.tga), BMP(.bmp), GIF(.gif)
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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] shader        : instance
[in] parameterName : 
[in] value         : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] shader        : instance
[in] parameterName : 
[in] value         : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] shader        : instance
[in] parameterName : 
[in] value         : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outMaterial(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.
                     LN_SHADING_MODEL_DEFAULT
                     Default
                     LN_SHADING_MODEL_UNLIT
                     Unlit

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] meshnode : instance
[in] value    : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  meshnode  : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  animationcontroller : instance
[in]  animationClip       : 
[out] outReturn(0)        : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outMeshImportSettings(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outBoxCollisionShape(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outBoxCollisionShape(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  animationcurve : instance
[in]  time           : 
[out] outReturn(0)   : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outKeyFrameAnimationCurve(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] keyframeanimationcurve             : instance
[in] time                               : 
[in] value                              : 
[in] rightTangentMode((LNTangentMode)0) : 
                                             LN_TANGENT_MODE_LINEAR
                                             ���`���
                                             LN_TANGENT_MODE_TANGENT
                                             �ڐ� (���x) ���g�p������� (�G���~�[�g�X�v���C��)
                                             LN_TANGENT_MODE_AUTO
                                             �L�[�t���[���̒l��ʉ߂���Ȃ߂炩�ȕ�� (Catmull-Rom)
                                             LN_TANGENT_MODE_CONSTANT
                                             ��ԂȂ�
[in] tangent(0.000000)                  : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
rightTangentMode �́A�V�����ǉ�����L�[�t���[���̉E���̕�ԕ��@�ł��B�V�����ǉ�����L�[�t���[���̍����̕ۊǕ��@�́A���̂ЂƂO�̃L�[�t���[���̉E���̕ۊǕ��@���ݒ肳��܂��B
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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] animationclip : instance
[in] value         : 
                        LN_ANIMATION_WRAP_MODE_ONCE
                        �J��Ԃ����s�킸�A1�x�����Đ����܂��B
                        LN_ANIMATION_WRAP_MODE_LOOP
                        �Ō�܂ōĐ����ꂽ��A�擪�ɖ߂��ă��[�v���܂��B
                        LN_ANIMATION_WRAP_MODE_ALTERNATE
                        �Ō�܂ōĐ����ꂽ��A�t�����ɖ߂��ă��[�v���܂��B

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  animationclip : instance
[out] outReturn(0)  : instance.
                         LN_ANIMATION_WRAP_MODE_ONCE
                         �J��Ԃ����s�킸�A1�x�����Đ����܂��B
                         LN_ANIMATION_WRAP_MODE_LOOP
                         �Ō�܂ōĐ����ꂽ��A�擪�ɖ߂��ă��[�v���܂��B
                         LN_ANIMATION_WRAP_MODE_ALTERNATE
                         �Ō�܂ōĐ����ꂽ��A�t�����ɖ߂��ă��[�v���܂��B

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] animationclip : instance
[in] value         : 
                        LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT
                        ���[�g�m�[�h�̂݁A���s�ړ���L�������܂��B
                        LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT_Y
                        AllowTranslationOnlyRootY
                        LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION
                        ���ׂẴm�[�h�̕��s�ړ���L�������܂��B
                        LN_HIERARCHICAL_ANIMATION_MODE_DISABLE_TRANSLATION
                        ���s�ړ��𖳌������܂��B

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  animationclip : instance
[out] outReturn(0)  : instance.
                         LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT
                         ���[�g�m�[�h�̂݁A���s�ړ���L�������܂��B
                         LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION_ONLY_ROOT_Y
                         AllowTranslationOnlyRootY
                         LN_HIERARCHICAL_ANIMATION_MODE_ALLOW_TRANSLATION
                         ���ׂẴm�[�h�̕��s�ړ���L�������܂��B
                         LN_HIERARCHICAL_ANIMATION_MODE_DISABLE_TRANSLATION
                         ���s�ړ��𖳌������܂��B

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outParticleEmitterModel(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] particleemittermodel : instance
[in] count                : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] particleemittermodel : instance
[in] rate                 : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] particleemittermodel : instance
[in] time                 : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] particleemittermodel : instance
[in] value                : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
                               ��Ɏ��_����������
                               LN_PARTICLE_GEOMETRY_DIRECTION_TOP
                               �i�s���������ɁA�\�� (Y+) �����_���猩����悤�ɂ���
                               LN_PARTICLE_GEOMETRY_DIRECTION_VERTICAL_BILLBOARD
                               Y �������ɒ������A�J�����̕��������܂��B
                               LN_PARTICLE_GEOMETRY_DIRECTION_HORIZONTAL_BILLBOARD
                               XZ ����́u���v���ʂƕ��s�ɂȂ�܂��B

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outParticleModel(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0)  : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] visualcomponent : instance
[in] value           : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  visualcomponent : instance
[out] outReturn(0)    : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] spritecomponent : instance
[in] texture         : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outCollisionEventHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] outCharacterController(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0)        : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0)        : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] charactercontroller : instance
[in] value               : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
false ���w�肵���ꍇ�A�L�����N�^�[�̑z��O�̎�����h�~���邽�߁A���x�� 0 �Ƀ��Z�b�g����܂��B
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
[in] charactercontroller : instance
[in] value               : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
�L���ł���ꍇ�A�֘A�t�����Ă���J������ʂ��āA�`���ƂȂ�r���[�� MouseGrab ���擾���܂��B�܂�A�}�E�X�J�[�\���͔�\���ƂȂ� UI ���|�C���g���ăN���b�N���铙�̑���͂ł��Ȃ��Ȃ�܂��B
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
[in] charactercontroller : instance
[in] value               : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  charactercontroller : instance
[out] outReturn(0)        : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] charactercontroller : instance
[in] value               : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  charactercontroller : instance
[out] outReturn(0)        : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] charactercontroller : instance
[in] handler             : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] charactercontroller : instance
[in] handler             : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] charactercontroller : instance
[in] handler             : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] world : instance
[in] obj   : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0)  : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0)  : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] outWorldObject(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] worldobject : instance
[in] pos         : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] worldobject : instance
[in] x           : 
[in] y           : 
[in] z(0.000000) : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  worldobject : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetRotationQuaternion
���̃I�u�W�F�N�g�̉�]��ݒ肵�܂��B
%group
LNWorldObject
%prm
worldobject, rot
[in] worldobject : instance
[in] rot         : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNWorldObject_SetRotation
���̃I�u�W�F�N�g�̉�]���I�C���[�p����ݒ肵�܂�(radian) �B��]������ Z(Roll) > X(Pich) > Y(Yaw) �ł��B
%group
LNWorldObject
%prm
worldobject, x, y, z
[in] worldobject : instance
[in] x           : 
[in] y           : 
[in] z           : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  worldobject : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] worldobject : instance
[in] scale       : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] worldobject : instance
[in] xyz         : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] worldobject : instance
[in] x           : 
[in] y           : 
[in] z(1.000000) : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  worldobject : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] worldobject : instance
[in] value       : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] worldobject : instance
[in] x           : 
[in] y           : 
[in] z(0.000000) : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  worldobject : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] worldobject : instance
[in] target      : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] worldobject : instance
[in] x           : 
[in] y           : 
[in] z           : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] worldobject : instance
[in] component   : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] worldobject : instance
[in] component   : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] worldobject : instance
[in] tag         : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] worldobject : instance
[in] tag         : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  worldobject : instance
[in]  tag         : 
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] worldobject : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
���ۂ̍폜�́A���݂̃t���[���̃A�b�v�f�[�g������ɍs���܂��B�폜���ꂽ WorldObject �́A�e�� World, Level, WorldObject ��������O����܂��B
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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] visualobject : instance
[in] value        : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  visualobject : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] visualobject : instance
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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] visualobject : instance
[in] value        : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  visualobject : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] environmentlight : instance
[in] enabled          : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  environmentlight : instance
[out] outReturn(0)     : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] environmentlight : instance
[in] value            : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  environmentlight : instance
[out] outReturn(0)     : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] environmentlight : instance
[in] value            : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  environmentlight : instance
[out] outReturn(0)     : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  environmentlight : instance
[out] outReturn(0)     : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] environmentlight : instance
[in] value            : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  environmentlight : instance
[out] outReturn(0)     : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] environmentlight : instance
[in] value            : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] environmentlight : instance
[in] intensity        : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  environmentlight : instance
[out] outReturn(0)     : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] environmentlight : instance
[in] value            : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  environmentlight : instance
[out] outReturn(0)     : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] environmentlight : instance
[in] value            : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  environmentlight : instance
[out] outReturn(0)     : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] outDirectionalLight(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] color               : 
[in] outDirectionalLight(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] directionallight : instance
[in] enabled          : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  directionallight : instance
[out] outReturn(0)     : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] directionallight : instance
[in] color            : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  directionallight : instance
[out] outReturn(0)     : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] directionallight : instance
[in] intensity        : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  directionallight : instance
[out] outReturn(0)     : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] directionallight : instance
[in] value            : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  directionallight : instance
[out] outReturn(0)     : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] directionallight : instance
[in] value            : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  directionallight : instance
[out] outReturn(0)     : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] outPointLight(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] color         : 
[in] range         : 
[in] outPointLight(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] pointlight : instance
[in] enabled    : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  pointlight : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] pointlight : instance
[in] color      : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  pointlight : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] pointlight : instance
[in] intensity  : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  pointlight : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] pointlight : instance
[in] range      : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  pointlight : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] pointlight  : instance
[in] attenuation : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  pointlight : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] outSpotLight(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] color        : 
[in] range        : 
[in] angle        : 
[in] outSpotLight(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] spotlight : instance
[in] enabled   : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  spotlight : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] spotlight : instance
[in] color     : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  spotlight : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] spotlight : instance
[in] intensity : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  spotlight : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] spotlight : instance
[in] range     : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  spotlight : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] spotlight   : instance
[in] attenuation : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  spotlight : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] spotlight : instance
[in] angle     : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  spotlight : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] spotlight : instance
[in] penumbra  : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  spotlight : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outTestDelegate(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outSprite(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outSprite(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outSprite(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] sprite : instance
[in] value  : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] sprite : instance
[in] value  : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] sprite : instance
[in] width  : 
[in] height : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] outCameraOrbitControlComponent(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  point     : 
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  raycaster : instance
[in]  normalX   : 
[in]  normalY   : 
[in]  normalZ   : 
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  raycastresult : instance
[out] outReturn(0)  : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] worldrenderview : instance
[in] value           : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  worldrenderview : instance
[out] outReturn(0)    : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] outBoxMesh(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outBoxMesh(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outPlaneMesh(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNStaticMesh_GetModel
�w�肵�����O�� MeshContainer ����A�Փ˔���p�� Body ���쐬���܂��B
%group
LNStaticMesh
%prm
staticmesh, outReturn
[in]  staticmesh : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNStaticMesh_MakeCollisionBody
�w�肵�����O�� MeshContainer ����A�Փ˔���p�� Body ���쐬���܂��B
%group
LNStaticMesh
%prm
staticmesh, meshContainerName
[in] staticmesh        : instance
[in] meshContainerName : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outStaticMeshComponent(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNStaticMeshComponent_MakeCollisionBody
�w�肵�����O�� MeshContainer ����A�Փ˔���p�� Body ���쐬���܂��B
%group
LNStaticMeshComponent
%prm
staticmeshcomponent, meshContainerName
[in] staticmeshcomponent : instance
[in] meshContainerName   : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outSkinnedMeshComponent(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  collision : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outTriggerBodyComponent(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outParticleEmitter(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] value : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] value : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] value : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] value : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.
                     LN_LEVEL_TRANSITION_EFFECT_MODE_NONE
                     �G�t�F�N�g����
                     LN_LEVEL_TRANSITION_EFFECT_MODE_FADE_IN_OUT
                     �t�F�[�h�C���E�t�F�[�h�A�E�g
                     LN_LEVEL_TRANSITION_EFFECT_MODE_CROSS_FADE
                     �N���X�t�F�[�h

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] value : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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


stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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


stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] value : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] value : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outLevel(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] level : instance
[in] obj   : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] level : instance
[in] obj   : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] level    : instance
[in] sublevel : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] level    : instance
[in] sublevel : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] level : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in]  uieventargs : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIGeneralEventHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIEventHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] size      : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] width     : 
[in] height    : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] margin    : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uielement : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] padding   : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uielement : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] value     : 
                    LN_UIHALIGNMENT_LEFT
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�̍����ɑ����Ĕz�u���܂��B
                    LN_UIHALIGNMENT_CENTER
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�̒����ɑ����Ĕz�u���܂��B
                    LN_UIHALIGNMENT_RIGHT
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�̉E���ɑ����Ĕz�u���܂��B
                    LN_UIHALIGNMENT_STRETCH
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�S�̂Ɉ����L�΂��܂��B

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uielement : instance
[out] outReturn(0) : instance.
                     LN_UIHALIGNMENT_LEFT
                     �q�v�f���A�e�̃��C�A�E�g �X���b�g�̍����ɑ����Ĕz�u���܂��B
                     LN_UIHALIGNMENT_CENTER
                     �q�v�f���A�e�̃��C�A�E�g �X���b�g�̒����ɑ����Ĕz�u���܂��B
                     LN_UIHALIGNMENT_RIGHT
                     �q�v�f���A�e�̃��C�A�E�g �X���b�g�̉E���ɑ����Ĕz�u���܂��B
                     LN_UIHALIGNMENT_STRETCH
                     �q�v�f���A�e�̃��C�A�E�g �X���b�g�S�̂Ɉ����L�΂��܂��B

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] value     : 
                    LN_UIVALIGNMENT_TOP
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�̏�[�ɑ����Ĕz�u���܂��B
                    LN_UIVALIGNMENT_CENTER
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�̒����ɑ����Ĕz�u���܂��B
                    LN_UIVALIGNMENT_BOTTOM
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�̉��[�ɑ����Ĕz�u���܂��B
                    LN_UIVALIGNMENT_STRETCH
                    �q�v�f���A�e�̃��C�A�E�g �X���b�g�S�̂Ɉ����L�΂��܂��B

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uielement : instance
[out] outReturn(0) : instance.
                     LN_UIVALIGNMENT_TOP
                     �q�v�f���A�e�̃��C�A�E�g �X���b�g�̏�[�ɑ����Ĕz�u���܂��B
                     LN_UIVALIGNMENT_CENTER
                     �q�v�f���A�e�̃��C�A�E�g �X���b�g�̒����ɑ����Ĕz�u���܂��B
                     LN_UIVALIGNMENT_BOTTOM
                     �q�v�f���A�e�̃��C�A�E�g �X���b�g�̉��[�ɑ����Ĕz�u���܂��B
                     LN_UIVALIGNMENT_STRETCH
                     �q�v�f���A�e�̃��C�A�E�g �X���b�g�S�̂Ɉ����L�΂��܂��B

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] pos       : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement   : instance
[in] x           : 
[in] y           : 
[in] z(0.000000) : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uielement : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] rot       : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] x         : 
[in] y         : 
[in] z         : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uielement : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] scale     : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] xyz       : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] x         : 
[in] y         : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uielement : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] value     : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement   : instance
[in] x           : 
[in] y           : 
[in] z(0.000000) : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uielement : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] value     : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uielement : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] value     : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uielement : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] value     : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uielement : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] value     : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uielement : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] value     : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uielement : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] value     : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uielement : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] value     : 
                    LN_UIVISIBILITY_VISIBLE
                    �v�f��\�����܂��B
                    LN_UIVISIBILITY_HIDDEN
                    �v�f��\�����܂��񂪁A���̗v�f�̗̈�����C�A�E�g���ɗ\�񂵂܂��B
                    LN_UIVISIBILITY_COLLAPSED
                    �v�f��\�����܂���B�܂��A���̗v�f�̗̈�����C�A�E�g���ɗ\�񂵂܂���B

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uielement : instance
[out] outReturn(0) : instance.
                     LN_UIVISIBILITY_VISIBLE
                     �v�f��\�����܂��B
                     LN_UIVISIBILITY_HIDDEN
                     �v�f��\�����܂��񂪁A���̗v�f�̗̈�����C�A�E�g���ɗ\�񂵂܂��B
                     LN_UIVISIBILITY_COLLAPSED
                     �v�f��\�����܂���B�܂��A���̗v�f�̗̈�����C�A�E�g���ɗ\�񂵂܂���B

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] value     : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uielement : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] child     : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uielement : instance
[in] value     : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uielement : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUITextBlock_Create
UITextBlock ���쐬���܂��B
%group
LNUITextBlock
%prm
outUITextBlock
[in] outUITextBlock(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUITextBlock_CreateWithText
�\����������w�肵�āAUITextBlock ���쐬���܂��B
%group
LNUITextBlock
%prm
text, outUITextBlock
[in] text           : 
[in] outUITextBlock(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUITextBlock_SetText
�\���������ݒ肵�܂��B
%group
LNUITextBlock
%prm
uitextblock, value
[in] uitextblock : instance
[in] value       : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNUITextBlock_GetText
�\����������擾���܂��B
%group
LNUITextBlock
%prm
uitextblock, outReturn
[in]  uitextblock : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] outUISprite(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] texture     : 
[in] outUISprite(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uisprite : instance
[in] texture  : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uisprite : instance
[in] rect     : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uisprite  : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  color     : 
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIMessageTextArea(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIBoxLayout(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] uiboxlayout : instance
[in] orientation : 
                      LN_UILAYOUT_ORIENTATION_HORIZONTAL
                      ���������ɔz�u���܂��B
                      LN_UILAYOUT_ORIENTATION_VERTICAL
                      ���������ɔz�u���܂��B
                      LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL
                      ���������i�E���獶�j�ɔz�u���܂��B
                      LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL
                      ���������i�������j�ɔz�u���܂��B

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uiboxlayout : instance
[out] outReturn(0) : instance.
                       LN_UILAYOUT_ORIENTATION_HORIZONTAL
                       ���������ɔz�u���܂��B
                       LN_UILAYOUT_ORIENTATION_VERTICAL
                       ���������ɔz�u���܂��B
                       LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL
                       ���������i�E���獶�j�ɔz�u���܂��B
                       LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL
                       ���������i�������j�ɔz�u���܂��B

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIStackLayout(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
                        ���������ɔz�u���܂��B
                        LN_UILAYOUT_ORIENTATION_VERTICAL
                        ���������ɔz�u���܂��B
                        LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL
                        ���������i�E���獶�j�ɔz�u���܂��B
                        LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL
                        ���������i�������j�ɔz�u���܂��B

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0)  : instance.
                         LN_UILAYOUT_ORIENTATION_HORIZONTAL
                         ���������ɔz�u���܂��B
                         LN_UILAYOUT_ORIENTATION_VERTICAL
                         ���������ɔz�u���܂��B
                         LN_UILAYOUT_ORIENTATION_REVERSE_HORIZONTAL
                         ���������i�E���獶�j�ɔz�u���܂��B
                         LN_UILAYOUT_ORIENTATION_REVERSE_VERTICAL
                         ���������i�������j�ɔz�u���܂��B

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIGridLayout(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIControl(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIButton(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] text        : 
[in] outUIButton(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uibutton  : instance
[in]  handler   : 
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIWindow(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uilistitem : instance
[in]  handler    : 
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
                             �Q�[��UI�p�BHover �őI����ԁA�V���O���N���b�N�� Submit. Hover ��Ԃ͎g�p����Ȃ��B
                             LN_UILIST_SUBMIT_MODE_DOUBLE
                             �G�f�B�^UI�p�B�V���O���N���b�N�őI����ԁA�_�u���N���b�N�� Submit.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0)       : instance.
                              LN_UILIST_SUBMIT_MODE_SINGLE
                              �Q�[��UI�p�BHover �őI����ԁA�V���O���N���b�N�� Submit. Hover ��Ԃ͎g�p����Ȃ��B
                              LN_UILIST_SUBMIT_MODE_DOUBLE
                              �G�f�B�^UI�p�B�V���O���N���b�N�őI����ԁA�_�u���N���b�N�� Submit.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIListBoxItem(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIListBox(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  uilistbox : instance
[in]  content   : 
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
                        
[in] outKeyGesture(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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


stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNMouse_Position
�}�E�X�|�C���^�̈ʒu���擾���܂��B
%group
LNMouse
%prm
outReturn
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in]  interpretercommand : instance
[out] outReturn(0)       : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  interpretercommand : instance
[out] outReturn(0)       : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  interpretercommand : instance
[in]  index              : 
[out] outReturn(0)       : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] outInterpreterCommandList(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] interpretercommandlist : instance
[in] code                   : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] interpretercommandlist : instance
[in] code                   : 
[in] param0                 : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] interpretercommandlist : instance
[in] code                   : 
[in] param0                 : 
[in] param1                 : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] interpretercommandlist : instance
[in] code                   : 
[in] param0                 : 
[in] param1                 : 
[in] param2                 : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] interpretercommandlist : instance
[in] code                   : 
[in] param0                 : 
[in] param1                 : 
[in] param2                 : 
[in] param3                 : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outInterpreterCommandDelegate(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] outInterpreter(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] interpreter : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] interpreter : instance
[in] commandList : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  interpreter : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetMainWindowResizable
���C���E�B���h�E�����[�U�[���T�C�Y�ύX�ł��邩�ǂ������w�肵�܂��B(default: false)
%group
LNEngineSettings
%prm
value
[in] value : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngineSettings_SetEngineLogEnabled
�f�o�b�O�p�̃��O�t�@�C���̏o�͗L����ݒ肵�܂��B(default: Debug �r���h�̏ꍇtrue�A����ȊO�� false)
%group
LNEngineSettings
%prm
enabled
[in] enabled : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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


stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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


stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_Run
�w�肵�� Application �̎��s���J�n���܂��B
%group
LNEngine
%prm
app
[in] app : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst
���̋@�\���Ăяo�����ꍇ�AEngine::initialize(), Engine::finalize(), Engine::update() ���Ăяo�����Ƃ͂ł��Ȃ��Ȃ�܂��B����� Application::onInit(), Application::onUpdate() �Ȃǂ��g�p���Ă��������B
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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

%href



;------------------------------------------------------------
;
;------------------------------------------------------------
%index
LNEngine_GetCamera
�f�t�H���g�ō쐬����郁�C���� Camera �ł��B
%group
LNEngine
%prm
outReturn
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outApplication(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] application : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in] application : instance

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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
[in]  application : instance
[out] outReturn(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] color : 
[in] str   : 

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outObjectSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outEventConnectionSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outVariantSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outZVTestClass1SerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outZVTestEventArgs1SerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outSerializer2SerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outAssetObjectSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outAssetImportSettingsSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outAssetModelSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outSoundSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outTextureSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outTexture2DSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outShaderSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outRenderViewSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outMaterialSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outMeshNodeSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outAnimationControllerSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outMeshModelSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outMeshImportSettingsSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outSkinnedMeshModelSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outCollisionShapeSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outBoxCollisionShapeSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outAnimationCurveSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outKeyFrameAnimationCurveSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outAnimationClipSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outAnimationStateSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outEffectResourceSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outParticleEmitterModelSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outParticleModelSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outComponentSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outVisualComponentSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outSpriteComponentSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outCharacterControllerSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outWorldSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outComponentListSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outWorldObjectSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outWorldObjectPreUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outWorldObjectUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outVisualObjectSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outVisualObjectPreUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outVisualObjectUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outCameraSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outCameraPreUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outCameraUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outEnvironmentLightSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outEnvironmentLightPreUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outEnvironmentLightUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outDirectionalLightSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outDirectionalLightPreUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outDirectionalLightUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outPointLightSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outPointLightPreUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outPointLightUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outSpotLightSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outSpotLightPreUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outSpotLightUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outSpriteSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outSpritePreUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outSpriteUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outCameraOrbitControlComponentSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outRaycasterSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outRaycastResultSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outWorldRenderViewSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outBoxMeshSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outBoxMeshPreUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outBoxMeshUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outPlaneMeshSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outPlaneMeshPreUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outPlaneMeshUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outStaticMeshSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outStaticMeshPreUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outStaticMeshUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outStaticMeshComponentSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outSkinnedMeshComponentSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outCollisionSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outTriggerBodyComponentSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outParticleEmitterSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outParticleEmitterPreUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outParticleEmitterUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outLevelSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outLevelStartHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outLevelStopHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outLevelPauseHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outLevelResumeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outLevelUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIEventArgsSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUILayoutElementSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIElementSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUITextBlockSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUISpriteSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIIconSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIMessageTextAreaSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUILayoutPanelSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIBoxLayoutSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIStackLayoutSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIGridLayoutSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIControlSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIButtonBaseSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIButtonSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIWindowSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIListItemSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIListItemsControlSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIListBoxItemSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outUIListBoxSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outInputGestureSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outKeyGestureSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outInterpreterCommandSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outInterpreterCommandListSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outInterpreterSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outInterpreterUpdateWaitHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outApplicationSerializeHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outApplicationInitHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

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
[in] outApplicationUpdateHandler(0) : instance.

stat : �G���[�R�[�h (�G���[�R�[�h�ɂ��Ă� LNError_GetLastErrorCode ���Q�Ƃ��Ă�������)

%inst

%href


