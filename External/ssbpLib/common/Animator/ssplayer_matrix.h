#ifndef __SSPLAYER_MATRIX__
#define __SSPLAYER_MATRIX__


#include <memory>

namespace ss
{

void	IdentityMatrix( float* matrix );
void    ScaleMatrix( float* _matrix , const float x , const float y , const float z);
void    TranslationMatrix( float* _matrix , const float x , const float y , const float z );
void	MultiplyMatrix(const float *m0, const float *m1, float *matrix);
void    Matrix4RotationX( float* _matrix ,const float radians );
void    Matrix4RotationY( float* _matrix ,const float radians );
void    Matrix4RotationZ( float* _matrix ,const float radians );
void	MatrixCopy(float* src, float* dst);

inline	void	TranslationMatrixM(  float* _matrix , const float x , const float y , const float z )
{
	float	_m[16];
	IdentityMatrix( _m );
	TranslationMatrix( _m , x , y , z );

	MultiplyMatrix( _m , _matrix , _matrix );
}

inline	void	ScaleMatrixM(  float* _matrix , const float x , const float y , const float z )
{

	float	_m[16];
	IdentityMatrix( _m );
	ScaleMatrix( _m , x , y , z );
	MultiplyMatrix( _m , _matrix , _matrix );
}

inline	void	RotationXYZMatrixM(  float* _matrix , const float x , const float y , const float z )
{

	if ( x != 0.0f )
	{
		float	_m[16];
		IdentityMatrix( _m );
		Matrix4RotationX( _m , x );

		MultiplyMatrix( _m , _matrix , _matrix );
	}

	if ( y != 0.0f )
	{
		float	_m[16];
		IdentityMatrix( _m );
		Matrix4RotationY( _m , y );

		MultiplyMatrix( _m , _matrix , _matrix );
	}

	if ( z != 0.0f )
	{
		float	_m[16];
		IdentityMatrix( _m );
		Matrix4RotationZ( _m , z );

		MultiplyMatrix( _m , _matrix , _matrix );
	}
}
inline	void	MatrixCopy(float* src, float* dst)
{
	int i;
	for (i = 0; i < 16; i++)
	{
		dst[i] = src[i];
	}
}


};

#endif

