
#include <Lumino.fxh>

float4x4 _localWorld;
float4x4 _scaleMatrix;

#if 1
const float EARTH_RADIUS = 6370997.0;//10.0
const float PI = 3.14159;
const float Kr = 0.0025 - 0.00015;		// Rayleigh scattering constant
const float Km = 0.0010 + 0.0015;		// Mie scattering constant
const float ESun = 1300.0;		// Sun brightness constant
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

const float3 v3LightPos = float3(0, 1, 0);		// The direction vector to the light source
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

const int nSamples = 2;
const float fSamples = 2.0;

struct VSOutput
{
	float4	svPos		: SV_POSITION;
	float3	v3Direction	: TEXCOORD0;
	float3  FrontColor	: TEXCOORD1;
	float3  FrontSecondaryColor	: TEXCOORD2;
	float3  TestColor	: TEXCOORD3;
};

struct PSInput
{
	float3	v3Direction	: TEXCOORD0;
	float3  FrontColor	: TEXCOORD1;
	float3  FrontSecondaryColor	: TEXCOORD2;
	float3  TestColor	: TEXCOORD3;
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
	float3 fWavelength4 = pow(fWavelength, 4.0);
	float3 v3InvWavelength = 1.0 / fWavelength4;	// 1 / pow(wavelength, 4) for the red, green, and blue channels


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

	float3 viewportPos = float3(v.Pos.x, v.Pos.y, 1.0);
	float3 localRay = mul(viewportPos, (float3x3)_scaleMatrix);
	float3 eyeRay = normalize(mul(localRay, (float3x3)_localWorld));	// 回転乗算のみなので、原点からの方向である

	float3 cameraPos = v3CameraPos;
	cameraPos.y += fInnerRadius;
	float3 skyPos = IntersectionPos(cameraPos, eyeRay, fOuterRadius);
	if (skyPos.x == 0) {
		// TODO: Camera is out of celestial sphere.
		//clip(-1);
		skyPos = float3(0, 0, 0);
	}

	float3 v3Ray = skyPos - cameraPos;
	o.TestColor = v3Ray;// / fOuterRadius;
	float fFar = length(v3Ray);
	v3Ray /= fFar;

	float fCameraHeight = length(cameraPos);

	// Calculate the ray's starting position, then calculate its scattering offset
	float3 v3Start = v3CameraPos;
	float fHeight = length(v3Start);
	float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
	float fStartAngle = dot(v3Ray, v3Start) / fHeight;
	float fStartOffset = fDepth*scale(fStartAngle);

	// Initialize the scattering loop variables
	//gl_FrontColor = vec4(0.0, 0.0, 0.0, 0.0);
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

		//o.TestColor = float3(0, 0, 0);
		//o.TestColor = v3FrontColor * 100000;
		//o.TestColor.r = fKr4PI * 100;;
		//o.TestColor.g = fKm4PI * 100;;
	}

	// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
	o.FrontSecondaryColor.rgb = v3FrontColor * fKmESun;
	o.FrontColor.rgb = v3FrontColor * (v3InvWavelength * fKrESun);
	o.v3Direction = -v3Ray;//v3CameraPos - v3Pos;

	return o;
}
//------------------------------------------------------------------------------

#define MIE_G (-0.990)
#define MIE_G2 0.9801

float4 PS_Main(PSInput input) : SV_TARGET
{
	return float4(input.TestColor, 1);

	float fCos = dot(v3LightPos, input.v3Direction) / length(input.v3Direction);
	float fMiePhase = 1.5 * ((1.0 - MIE_G2) / (2.0 + MIE_G2)) * (1.0 + fCos*fCos) / pow(1.0 + MIE_G2 - 2.0*MIE_G*fCos, 1.5);
	float4 color;
    color.rgb = input.FrontColor + fMiePhase * input.FrontSecondaryColor;
	color.a = color.b;
	return color;
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
