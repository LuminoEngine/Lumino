
#include <Lumino.fxh>

float4x4 _localWorld;
float4x4 _scaleMatrix;
const float _SunSize = 0.04;
const float _SunSizeConvergence = 5.0;

#if 1
const float EARTH_RADIUS = 6370997.0;//10.0
const float PI = 3.14159;
const float Kr = 0.0025 - 0.00015;		// Rayleigh scattering constant
const float Km = 0.0010 - 0.0005;		// Mie scattering constant
const float ESun = 1300.0;		// Sun brightness constant
const float exposure = 0.08;
#else
const float EARTH_RADIUS = 6370997.0;//10.0
const float PI = 3.14159;
const float Kr = 0.0025;		// Rayleigh scattering constant
const float Km = 0.0010;		// Mie scattering constant
const float ESun = 20.0f;		// Sun brightness constant
#endif

const float3 fWavelength = float3(0.650, 0.570, 0.475);

float3 v3CameraPos;		// The camera's current position
//const float fCameraHeight = 0.1;	// The camera's current height

const float3 v3LightPos = normalize(float3(0, 1, 1));		// The direction vector to the light source
const float fOuterRadius = EARTH_RADIUS * 1.025;						// The outer (atmosphere) radius
const float fOuterRadius2 = fOuterRadius * fOuterRadius;	// fOuterRadius^2
const float fInnerRadius = EARTH_RADIUS;							// The inner (planetary) radius
const float fInnerRadius2 = fInnerRadius * fInnerRadius;	// fInnerRadius^2
const float fKrESun = Kr * ESun;
const float fKmESun = Km * ESun;
const float fKr4PI = Kr * 4.0 * PI;
const float fKm4PI = Km * 4.0 * PI;
const float fScale = 1.0 / (fOuterRadius - fInnerRadius);			// 1 / (fOuterRadius - fInnerRadius)
const float fScaleDepth = 0.25;		// The scale depth (i.e. the altitude at which the atmosphere's average density is found)
const float fScaleOverScaleDepth = ((1.0 / (fOuterRadius - fInnerRadius)) / fScaleDepth);

const int nSamples = 5;//2
const float fSamples = 5.0;//2.0

const float kHDSundiskIntensityFactor = 15.0;

struct VSOutput
{
	float4	svPos		: SV_POSITION;
	float3	v3Direction	: TEXCOORD0;
	float3  FrontColor	: TEXCOORD1;
	float3  FrontSecondaryColor	: TEXCOORD2;
	float3  TestColor	: TEXCOORD3;
	float3  eyeRay	: TEXCOORD4;
};

struct PSInput
{
	float3	v3Direction	: TEXCOORD0;
	float3  FrontColor	: TEXCOORD1;
	float3  FrontSecondaryColor	: TEXCOORD2;
	float3  TestColor	: TEXCOORD3;
	float3  eyeRay	: TEXCOORD4;
};


//------------------------------------------------------------------------------
float3 IntersectionPos(float3 rayPos, float3 rayDir, float sphereRadius)
{
	const float A = dot(rayDir, rayDir);
	const float B = 2.0*dot(rayPos, rayDir);
	const float C = dot(rayPos, rayPos) - sphereRadius*sphereRadius;

	return B*B - 4.0*A*C < 0.000001 ? float3(0, 0, 0) : (rayPos + rayDir*(0.5*(-B + sqrt(B*B - 4.0*A*C)) / A));
}

float scale(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

VSOutput VS_Main(LN_VSInput v)
{
#if 0	// original code
	// Get the ray from the camera to the vertex, and its length (which is the far point of the ray passing through the atmosphere)
	vec3 v3Pos = gl_Vertex.xyz;			// これは、fOuterRadius 分の半径を持つ球メッシュの頂点
	vec3 v3Ray = v3Pos - v3CameraPos;	// v3CameraPos は普通のワールド上の位置だが、fOuterRadius の内部であることが前提。
	float fFar = length(v3Ray);
	v3Ray /= fFar;
#endif

	VSOutput o;
	o.svPos = float4(v.Pos, 1.0);
	o.TestColor = float3(0, 0, 0);
	o.v3Direction = float3(0, 0, 0);
	o.FrontColor = float3(0, 0, 0);
	o.FrontSecondaryColor = float3(0, 0, 0);
	o.TestColor = float3(0, 0, 0);

	float3 viewportPos = float3(v.Pos.x, v.Pos.y, 1.0);
#if 1
	float3 localRayTL = mul(float3(-1, -1, 1), (float3x3)_scaleMatrix);
	float3 localRayTR = mul(float3(1, -1, 1), (float3x3)_scaleMatrix);
	float3 localRayBL = mul(float3(-1, 1, 1), (float3x3)_scaleMatrix);
	float3 localRayBR = mul(float3(1, 1, 1), (float3x3)_scaleMatrix);
	float3 deltaX = (localRayTR - localRayTL) * ((v.Pos.x + 1.0)/ 2.0);
	float3 deltaY = (localRayBL - localRayTL) * ((v.Pos.y + 1.0)/ 2.0);
	float3 localRay = normalize(localRayTL + deltaX + deltaY);
	float3 eyeRay = normalize(mul(localRay, (float3x3)_localWorld));
#else
	float3 localRay = mul(viewportPos, (float3x3)_scaleMatrix);
	float3 eyeRay = normalize(mul(localRay, (float3x3)_localWorld));	// 回転乗算のみなので、原点からの方向である
#endif
	o.eyeRay = eyeRay;


	
	float3 fWavelength4 = pow(fWavelength, 4.0);
	float3 v3InvWavelength = 1.0 / fWavelength4;	// 1 / pow(wavelength, 4) for the red, green, and blue channels
	
	float3 cameraPos = v3CameraPos;
	cameraPos.y += fInnerRadius;
	float3 skyPos = IntersectionPos(cameraPos, eyeRay, fOuterRadius);
	if (skyPos.x == 0) {
		// TODO: Camera is out of celestial sphere.
		//clip(-1);
		//skyPos = float3(0, 0, 0);
	}

	float3 v3Ray = skyPos - cameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;

	float fCameraHeight = length(cameraPos);

	// Calculate the ray's starting position, then calculate its scattering offset
	float3 v3Start = cameraPos;
	float fHeight = length(v3Start);
	float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
	float fStartAngle = dot(v3Ray, v3Start) / fHeight;
	float fStartOffset = fDepth*scale(fStartAngle);

	// Initialize the scattering loop variables
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	float3 v3SampleRay = v3Ray * fSampleLength;
	float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

	// Now loop through the sample rays
	float3 v3FrontColor = float3(0.0, 0.0, 0.0);
	for(int i=0; i<nSamples; i++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fLightAngle = dot(v3LightPos, v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		float fScatter = (fStartOffset + fDepth*(scale(fLightAngle) - scale(fCameraAngle)));
		float3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}

	// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
	/* secondaryColor */ float3 FrontSecondaryColor.rgb = v3FrontColor * fKmESun;
	/* primaryColor */ float3 FrontColor.rgb = v3FrontColor * (v3InvWavelength * fKrESun);
	float3 v3Direction = -v3Ray;






	o.v3Direction = v3Direction;
	o.FrontColor = FrontColor;
	o.FrontSecondaryColor = FrontSecondaryColor;
	return o;
}
//------------------------------------------------------------------------------

#define MIE_G (-0.999)//(-0.990)
#define MIE_G2 0.9801

float getMiePhase(float eyeCos, float eyeCos2)
{
	float temp = 1.0 + MIE_G2 - 2.0 * MIE_G * eyeCos;
	temp = pow(temp, pow(_SunSize,0.65) * 10);
	temp = max(temp,1.0e-4); // prevent division by zero, esp. in half precision
	temp = 1.5 * ((1.0 - MIE_G2) / (2.0 + MIE_G2)) * (1.0 + eyeCos2) / temp;
 #if defined(UNITY_COLORSPACE_GAMMA) && SKYBOX_COLOR_IN_TARGET_COLOR_SPACE
	temp = pow(temp, .454545);
#endif
	return temp;
}

half calcSunAttenuation(half3 lightPos, half3 ray)
{
#if 0 // SKYBOX_SUNDISK == SKYBOX_SUNDISK_SIMPLE
	half3 delta = lightPos - ray;
	half dist = length(delta);
	half spot = 1.0 - smoothstep(0.0, _SunSize, dist);
	return spot * spot;
#else // SKYBOX_SUNDISK_HQ
	half focusedEyeCos = pow(saturate(dot(lightPos, ray)), _SunSizeConvergence);
	return getMiePhase(-focusedEyeCos, focusedEyeCos * focusedEyeCos);
#endif
}

float4 PS_Main(PSInput input) : SV_TARGET
{	
	float3 FrontSecondaryColor = input.FrontSecondaryColor;
	float3 FrontColor = input.FrontColor;
	float3 v3Direction = input.v3Direction;
	float3 v3Ray = v3Direction;


	//-----------------------------------------------

#if 1
	float fCos = dot(v3LightPos, v3Direction) / length(v3Direction);
	float fRayPhase = 0.75 * (1.0 + fCos * fCos);

	float fMiePhase = 1.5 * ((1.0 - MIE_G2) / (2.0 + MIE_G2)) * (1.0 + fCos*fCos) / pow(abs(1.0 + MIE_G2 - 2.0*MIE_G*fCos), 1.5);
	// fMiePhase: contains sun

	float3 raycolor = (FrontColor*fRayPhase).xyz;
	float3 miecolor = (FrontSecondaryColor*fMiePhase).xyz;
	

	float3 c = float3(1.0, 1.0, 1.0) - exp(-exposure * (raycolor + miecolor));


#if 0
	{
		float3 cOut = FrontSecondaryColor;
		float3 lightColor = float3(1, 1, 1);
		float lightColorIntensity = clamp(length(lightColor), 0.25, 1);
		float3 sunColor = kHDSundiskIntensityFactor * saturate(cOut) * lightColor / lightColorIntensity;
		c += sunColor * calcSunAttenuation(v3LightPos, v3Ray);
	}
#endif
	

	float4 color;
	color.rgb = c;
	color.a = 1;//color.b;
	return color;
#else
	float fCos = dot(v3LightPos, v3Direction) / length(v3Direction);
	float fMiePhase = 1.5 * ((1.0 - MIE_G2) / (2.0 + MIE_G2)) * (1.0 + fCos*fCos) / pow(1.0 + MIE_G2 - 2.0*MIE_G*fCos, 1.5);
	float4 color;
    color.rgb = FrontColor + fMiePhase * FrontSecondaryColor;
	color.a = color.b;
	return color;
#endif
}
//------------------------------------------------------------------------------


technique Forward_Geometry_UnLighting
{
    pass Pass1
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}
