#ifndef __SSPLAYER_MACRO__
#define __SSPLAYER_MACRO__


namespace ss
{

#define __PI__	(3.14159265358979323846f)
#define RadianToDegree(Radian) ((double)Radian * (180.0f / __PI__))
#define DegreeToRadian(Degree) ((double)Degree * (__PI__ / 180.0f))

#define foreach(T, c, i) for(T::iterator i = c.begin(); i!=c.end(); ++i)

};
#endif
