
#include <Lumino.fxh>



































float3 frustumRayTL;
float3 frustumRayTR;
float3 frustumRayBL;
//float2 windowSize = float2(640, 480);//ln_ViewportPixelSize.xy;




float3 v3CameraPos;		// The camera's current position
float fCameraHeight;	// The camera's current height

float3 v3LightPos;		// The direction vector to the light source
float fCameraHeight2;	// fCameraHeight^2

float3 v3InvWavelength;	// 1 / pow(wavelength, 4) for the red, green, and blue channels
float fScale;			// 1 / (fOuterRadius - fInnerRadius)

float fOuterRadius;		// The outer (atmosphere) radius
float fOuterRadius2;	// fOuterRadius^2
float fInnerRadius;		// The inner (planetary) radius
float fInnerRadius2;	// fInnerRadius^2

float fKrESun;			// Kr * ESun
float fKmESun;			// Km * ESun
float fKr4PI;			// Kr * 4 * PI
float fKm4PI;			// Km * 4 * PI

float fScaleDepth;		// The scale depth (i.e. the altitude at which the atmosphere's average density is found)
float fScaleOverScaleDepth;	// fScale / fScaleDepth
float g;
float exposure;


float4x4 _localWorld;
const float _nearClip = 0.3;
const float3 _LightColor0 = float3(1, 1, 1);
const float3 _WorldSpaceLightPos0 = float3(0, 1, 0);//float3(0, 0.75,  0.75);	// directional light direction
const float _Exposure = 1.3;
const float3 _GroundColor = float3(0.369, 0.349, 0.341);//float3(1, 1, 1);
const float _SunSize = 0.04;
const float3 _SkyTint = float3(0, 0, 1);//float3(0.5, 0.5, 0.5);//
const float _AtmosphereThickness = 1;
const float _SunSizeConvergence = 5.0;


// RGB wavelengths
// .35 (.62=158), .43 (.68=174), .525 (.75=190)
static const float3 kDefaultScatteringWavelength = float3(.65, .57, .475);
static const float3 kVariableRangeForScatteringWavelength = float3(.15, .15, .15);

#define OUTER_RADIUS 1.025
static const float kOuterRadius = OUTER_RADIUS;
static const float kOuterRadius2 = OUTER_RADIUS*OUTER_RADIUS;
static const float kInnerRadius = 1.0;
static const float kInnerRadius2 = 1.0;


static const float kCameraHeight = 0.0001;

#define kRAYLEIGH (lerp(0.0, 0.0025, pow(_AtmosphereThickness,2.5)))		// Rayleigh constant
#define kMIE 0.0010      		// Mie constant
#define kSUN_BRIGHTNESS 20.0 	// Sun brightness

#define kMAX_SCATTER 50.0 // Maximum scattering value, to prevent math overflows on Adrenos

static const float kSunScale = 400.0 * kSUN_BRIGHTNESS;
static const float kKmESun = kMIE * kSUN_BRIGHTNESS;
static const float kKm4PI = kMIE * 4.0 * 3.14159265;
static const float kScale = 1.0 / (OUTER_RADIUS - 1.0);
static const float kScaleDepth = 0.25;
static const float kScaleOverScaleDepth = (1.0 / (OUTER_RADIUS - 1.0)) / 0.25;
static const float kSamples = 2.0; // THIS IS UNROLLED MANUALLY, DON'T TOUCH

#define MIE_G (-0.990)
#define MIE_G2 0.9801

#define SKY_GROUND_THRESHOLD 0.02

struct VSOutput
{
	float4	svPos		: SV_POSITION;

	float3	vertex			: TEXCOORD0;
	float3	groundColor		: TEXCOORD1;
	float3	skyColor		: TEXCOORD2;
	float3	sunColor		: TEXCOORD3;
};

struct PSInput
{
	float3	vertex			: TEXCOORD0;
	float3	groundColor		: TEXCOORD1;
	float3	skyColor		: TEXCOORD2;
	float3	sunColor		: TEXCOORD3;
};

//------------------------------------------------------------------------------
#define COLOR_2_GAMMA(color) color
#define COLOR_2_LINEAR(color) color*color
#define LINEAR_2_OUTPUT(color) sqrt(color)

float getRayleighPhase(float eyeCos2)
{
	return 0.75 + 0.75*eyeCos2;
}
float getRayleighPhase(float3 light, float3 ray)
{
	float eyeCos	= dot(light, ray);
	return getRayleighPhase(eyeCos * eyeCos);
}

float scale(float inCos)
{
	float x = 1.0 - inCos;
	return 0.25 * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

VSOutput VS_Main(LN_VSInput v)
{
	VSOutput o;
	o.svPos = float4(v.Pos, 1.0);

	float3 kSkyTintInGammaSpace = COLOR_2_GAMMA(_SkyTint); // convert tint from Linear back to Gamma
	float3 kScatteringWavelength = lerp (
		kDefaultScatteringWavelength-kVariableRangeForScatteringWavelength,
		kDefaultScatteringWavelength+kVariableRangeForScatteringWavelength,
		float3(1,1,1) - kSkyTintInGammaSpace); // using Tint in sRGB gamma allows for more visually linear interpolation and to keep (.5) at (128, gray in sRGB) point
	float3 kInvWavelength = 1.0 / pow(kScatteringWavelength, 4);

	float kKrESun = kRAYLEIGH * kSUN_BRIGHTNESS;
	float kKr4PI = kRAYLEIGH * 4.0 * 3.14159265;

	float3 cameraPos = float3(0,kInnerRadius + kCameraHeight,0); 	// The camera's current position

	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	float3 viewportPos = float3(v.Pos.x, v.Pos.y, 1.0);
	float3 eyeRayRaw = mul((float3x3)_localWorld, viewportPos);
	float3 eyeRay = normalize(eyeRayRaw);

	float far = 0.0;
	float3 cIn, cOut;


	if(eyeRay.y >= 0.0)
	{
		// Sky
		// Calculate the length of the "atmosphere"
		far = sqrt(kOuterRadius2 + kInnerRadius2 * eyeRay.y * eyeRay.y - kInnerRadius2) - kInnerRadius * eyeRay.y;

		float3 pos = cameraPos + far * eyeRay;

		// Calculate the ray's starting position, then calculate its scattering offset
		float height = kInnerRadius + kCameraHeight;
		float depth = exp(kScaleOverScaleDepth * (-kCameraHeight));
		float startAngle = dot(eyeRay, cameraPos) / height;
		float startOffset = depth*scale(startAngle);


		// Initialize the scattering loop variables
		float sampleLength = far / kSamples;
		float scaledLength = sampleLength * kScale;
		float3 sampleRay = eyeRay * sampleLength;
		float3 samplePoint = cameraPos + sampleRay * 0.5;

		// Now loop through the sample rays
		float3 frontColor = float3(0.0, 0.0, 0.0);
		// Weird workaround: WP8 and desktop FL_9_1 do not like the for loop here
		// (but an almost identical loop is perfectly fine in the ground calculations below)
		// Just unrolling this manually seems to make everything fine again.
//				for(int i=0; i<int(kSamples); i++)
		{
			float height = length(samplePoint);
			float depth = exp(kScaleOverScaleDepth * (kInnerRadius - height));
			float lightAngle = dot(_WorldSpaceLightPos0.xyz, samplePoint) / height;
			float cameraAngle = dot(eyeRay, samplePoint) / height;
			float scatter = (startOffset + depth*(scale(lightAngle) - scale(cameraAngle)));
			float3 attenuate = exp(-clamp(scatter, 0.0, kMAX_SCATTER) * (kInvWavelength * kKr4PI + kKm4PI));

			frontColor += attenuate * (depth * scaledLength);
			samplePoint += sampleRay;
		}
		{
			float height = length(samplePoint);
			float depth = exp(kScaleOverScaleDepth * (kInnerRadius - height));
			float lightAngle = dot(_WorldSpaceLightPos0.xyz, samplePoint) / height;
			float cameraAngle = dot(eyeRay, samplePoint) / height;
			float scatter = (startOffset + depth*(scale(lightAngle) - scale(cameraAngle)));
			float3 attenuate = exp(-clamp(scatter, 0.0, kMAX_SCATTER) * (kInvWavelength * kKr4PI + kKm4PI));

			frontColor += attenuate * (depth * scaledLength);
			samplePoint += sampleRay;
		}



		// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
		cIn = frontColor * (kInvWavelength * kKrESun);
		cOut = frontColor * kKmESun;
	}
	else
	{
		// Ground
		far = (-kCameraHeight) / (min(-0.001, eyeRay.y));

		float3 pos = cameraPos + far * eyeRay;

		// Calculate the ray's starting position, then calculate its scattering offset
		float depth = exp((-kCameraHeight) * (1.0/kScaleDepth));
		float cameraAngle = dot(-eyeRay, pos);
		float lightAngle = dot(_WorldSpaceLightPos0.xyz, pos);
		float cameraScale = scale(cameraAngle);
		float lightScale = scale(lightAngle);
		float cameraOffset = depth*cameraScale;
		float temp = (lightScale + cameraScale);

		// Initialize the scattering loop variables
		float sampleLength = far / kSamples;
		float scaledLength = sampleLength * kScale;
		float3 sampleRay = eyeRay * sampleLength;
		float3 samplePoint = cameraPos + sampleRay * 0.5;

		// Now loop through the sample rays
		float3 frontColor = float3(0.0, 0.0, 0.0);
		float3 attenuate;
//				for(int i=0; i<int(kSamples); i++) // Loop removed because we kept hitting SM2.0 temp variable limits. Doesn't affect the image too much.
		{
			float height = length(samplePoint);
			float depth = exp(kScaleOverScaleDepth * (kInnerRadius - height));
			float scatter = depth*temp - cameraOffset;
			attenuate = exp(-clamp(scatter, 0.0, kMAX_SCATTER) * (kInvWavelength * kKr4PI + kKm4PI));
			frontColor += attenuate * (depth * scaledLength);
			samplePoint += sampleRay;
		}

		cIn = frontColor * (kInvWavelength * kKrESun + kKmESun);
		cOut = clamp(attenuate, 0.0, 1.0);
	}

	o.vertex 			= -viewportPos;
	o.groundColor	= _Exposure * (cIn + COLOR_2_LINEAR(_GroundColor) * cOut);
	o.skyColor	= _Exposure * (cIn * getRayleighPhase(_WorldSpaceLightPos0.xyz, -eyeRay));
	o.sunColor	= _Exposure * (cOut * _LightColor0.xyz);


	o.skyColor = cOut;

	//OUT.groundColor	= sqrt(OUT.groundColor);
	//OUT.skyColor	= sqrt(OUT.skyColor);
	//OUT.sunColor= sqrt(OUT.sunColor);
	return o;
}
//------------------------------------------------------------------------------

float getMiePhase(float eyeCos, float eyeCos2)
{
	float temp = 1.0 + MIE_G2 - 2.0 * MIE_G * eyeCos;
	temp = pow(temp, pow(_SunSize,0.65) * 10);
	temp = max(temp,1.0e-4); // prevent division by zero, esp. in half precision
	temp = 1.5 * ((1.0 - MIE_G2) / (2.0 + MIE_G2)) * (1.0 + eyeCos2) / temp;

	//	temp = pow(temp, .454545);
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
	float3 col = float3(0.0, 0.0, 0.0);

	//col = mul((float3x3)ln_World, float3(0, 0, 1));
	//return float4(col,1.0);



	float3 ray = normalize(mul((float3x3)_localWorld, input.vertex));
	

	float y = ray.y / SKY_GROUND_THRESHOLD;

	// test
	if(ray.y >= 0.0) {
		// sky
		//return float4(0.5, 0.5, 1, 1);
	}
	else {
		//return float4(1, 1, 1, 1);
	}


	col = lerp(input.skyColor, input.groundColor, saturate(y));

#if 1
	if(y < 0.0)
	{
		col += input.sunColor * calcSunAttenuation(_WorldSpaceLightPos0.xyz, -ray);

		//float eyeCos = dot(_WorldSpaceLightPos0.xyz, ray);
		//float eyeCos2 = eyeCos * eyeCos;
		//float mie = getMiePhase(eyeCos, eyeCos2);
		//col += mie * input.sunColor;
	}
#endif

	//col = LINEAR_2_OUTPUT(col);

	return float4(col,1.0);
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
