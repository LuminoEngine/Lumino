
#include <Lumino.fxh>


float3 _RayleighColorScale;

float3 sunPosition;
float rayleigh;
float turbidity;
float mieCoefficient;
float3 up;
const float3 cameraPos = float3( 0.0, 0.0, 0.0 );

float luminance;
float mieDirectionalG;

struct VSOutput
{
	float4	svPos : SV_POSITION;
	float3 vWorldPosition : TEXCOORD0;
	float3 vSunDirection : TEXCOORD1;
	float vSunfade : TEXCOORD2;
	float3 vBetaR : TEXCOORD3;
	float3 vBetaM : TEXCOORD4;
	float vSunE : TEXCOORD5;
};

struct PSInput
{
	float3 vWorldPosition : TEXCOORD0;
	float3 vSunDirection : TEXCOORD1;
	float vSunfade : TEXCOORD2;
	float3 vBetaR : TEXCOORD3;
	float3 vBetaM : TEXCOORD4;
	float vSunE : TEXCOORD5;
};

// constants for atmospheric scattering
const float e = 2.71828182845904523536028747135266249775724709369995957;
const float pi = 3.141592653589793238462643383279502884197169;

//------------------------------------------------------------------------------


// wavelength of used primaries, according to preetham
const float3 lambda = float3( 680E-9, 550E-9, 450E-9 );
//const float3 lambda = float3( 550E-9, 550E-9, 550E-9 );
// this pre-calcuation replaces older TotalRayleigh(float3 lambda) function:
// (8.0 * pow(pi, 3.0) * pow(pow(n, 2.0) - 1.0, 2.0) * (6.0 + 3.0 * pn)) / (3.0 * N * pow(lambda, float3(4.0)) * (6.0 - 7.0 * pn))
//const float3 totalRayleigh = float3( 5.804542996261093E-6, 1.3562911419845635E-5, 3.0265902468824876E-5 );
//const float3 totalRayleigh = float3( 0.00000005, 0.00000005, 0.00000005);
		//'const float3 totalRayleigh = float3( 0.000005, 0.000005, 0.000005);
		//'const float3 totalRayleigh = float3( 0.00000005, 0.0000005, 0.0000005);
const float3 totalRayleigh = float3( 0.0000001, 0.0000001, 0.0000001);// * (float3(1000.0) * _RayleighColorScale);

// mie stuff
// K coefficient for the primaries
static const float v = 4.0;
static const float3 K = float3( 0.686, 0.678, 0.666 );
//const float3 K = float3( 0.5, 0.5, 0.0 );
		//'const float3 K = float3( 0.5, 0.5, 5.0 );
// MieConst = pi * pow( ( 2.0 * pi ) / lambda, float3( v - 2.0 ) ) * K
static const float3 MieConst = pi * pow( ( 2.0 * pi ) / lambda, float3(v-2.0, v-2.0, v-2.0) ) * K;
		//'const float3 MieConst = float3( 1.8399918514433978E14, 2.7798023919660528E14, 4.0790479543861094E14 );

// earth shadow hack
// cutoffAngle = pi / 1.95;
const float cutoffAngle = 1.6110731556870734;
const float steepness = 1.5;
const float EE = 1000.0;

float sunIntensity( float zenithAngleCos ) {
	zenithAngleCos = clamp( zenithAngleCos, -1.0, 1.0 );
	return EE * max( 0.0, 1.0 - pow( e, -( ( cutoffAngle - acos( zenithAngleCos ) ) / steepness ) ) );
}

float3 totalMie( float T ) {
	float c = ( 0.2 * T ) * 10E-18;
	return 0.434 * c * MieConst;
}

VSOutput VS_Main(LN_VSInput v)
{
	VSOutput o;

	//float4 worldPosition = modelMatrix * float4( position, 1.0 );
	//float4 worldPosition = ln_World * float4( v.Pos, 1.0 );
	float4 worldPosition = mul(float4( v.Pos, 1.0 ), ln_World);
	o.vWorldPosition = v.Pos.xyz;

	//gl_Position = projectionMatrix * modelViewMatrix * float4( position, 1.0 );
	//gl_Position.z = gl_Position.w; // set z to camera.far
	o.svPos = mul(float4(v.Pos, 1.0f), ln_WorldViewProjection);
	o.svPos.z = o.svPos.w; // set z to camera.far

	o.vSunDirection = normalize( sunPosition );

	o.vSunE = sunIntensity( dot( o.vSunDirection, up ) );

	o.vSunfade = 1.0 - clamp( 1.0 - exp( ( sunPosition.y / 450000.0 ) ), 0.0, 1.0 );


// 実質入力の rayleigh がそのまま使われる。
	float rayleighCoefficient = rayleigh - ( 1.0 * ( 1.0 - o.vSunfade ) );

// extinction (absorbtion + out scattering)
// rayleigh coefficients
	o.vBetaR = (totalRayleigh * (float3(500.0, 500.0, 500.0) * _RayleighColorScale)) * rayleighCoefficient;

// mie coefficients
	o.vBetaM = totalMie( turbidity ) * mieCoefficient;

	return o;
}


//------------------------------------------------------------------------------


const float n = 1.0003; // refractive index of air
const float N = 2.545E25; // number of molecules per unit volume for air at 288.15K and 1013mb (sea level -45 celsius)

// optical length at zenith for molecules
const float rayleighZenithLength = 8.4E3;
const float mieZenithLength = 1.25E3;
// 66 arc seconds -> degrees, and the cosine of that
const float sunAngularDiameterCos = 0.999956676946448443553574619906976478926848692873900859324;

// 3.0 / ( 16.0 * pi )
const float THREE_OVER_SIXTEENPI = 0.05968310365946075;
// 1.0 / ( 4.0 * pi )
const float ONE_OVER_FOURPI = 0.07957747154594767;

float rayleighPhase( float cosTheta ) {
	return THREE_OVER_SIXTEENPI * ( 1.0 + pow( cosTheta, 2.0 ) );
}

float hgPhase( float cosTheta, float g ) {
	float g2 = pow( g, 2.0 );
	float inverse = 1.0 / pow( 1.0 - 2.0 * g * cosTheta + g2, 1.5 );
	return ONE_OVER_FOURPI * ( ( 1.0 - g2 ) * inverse );
}

// Filmic ToneMapping http://filmicgames.com/archives/75
const float A = 0.15;
const float B = 0.50;
const float C = 0.10;
const float D = 0.20;
const float E = 0.02;
const float F = 0.30;

const float whiteScale = 1.0748724675633854; // 1.0 / Uncharted2Tonemap(1000.0)

float3 Uncharted2Tonemap( float3 x ) {
	return ( ( x * ( A * x + C * B ) + D * E ) / ( x * ( A * x + B ) + D * F ) ) - E / F;
}


float4 PS_Main(PSInput input) : SV_TARGET
{
// optical length
// cutoff angle at 90 to avoid singularity in next formula.
	float zenithAngle = acos( max( 0.0, dot( up, normalize( input.vWorldPosition - cameraPos ) ) ) );
	float inverse = 1.0 / ( cos( zenithAngle ) + 0.15 * pow( 93.885 - ( ( zenithAngle * 180.0 ) / pi ), -1.253 ) );
	float sR = rayleighZenithLength * inverse;
	float sM = mieZenithLength * inverse;

// combined extinction factor
		//	( vBetaR * sR + vBetaM * sM )
		//	=> 入力 rayleigh が 0 だと、K の色になるのはなぜ？
		//	=> vBetaR が 0 になる。
	float3 Fex = exp( -( input.vBetaR * sR + input.vBetaM * sM ) );

		//'	gl_FragColor = float4( Fex, 1.0 );
		//'	return;

// in scattering
	float cosTheta = dot( normalize( input.vWorldPosition - cameraPos ), input.vSunDirection );

	float rPhase = rayleighPhase( cosTheta * 0.5 + 0.5 );
	float3 betaRTheta = input.vBetaR * rPhase;

	float mPhase = hgPhase( cosTheta, mieDirectionalG );
	float3 betaMTheta = input.vBetaM * mPhase;

	float3 Lin = pow( input.vSunE * ( ( betaRTheta + betaMTheta ) / ( input.vBetaR + input.vBetaM ) ) * ( 1.0 - Fex ), float3( 1.5, 1.5, 1.5 ) );
	Lin *= lerp( float3(1.0, 1.0, 1.0), pow( input.vSunE * ( ( betaRTheta + betaMTheta ) / ( input.vBetaR + input.vBetaM ) ) * Fex, float3(0.5, 0.5, 0.5) ), clamp( pow( 1.0 - dot( up, input.vSunDirection ), 5.0 ), 0.0, 1.0 ) );

// この時点では K の色と同じ
// でも、夕方、Gを増やすと、太陽の輪郭回りはKだが、外回りに逆色が広がる。
		//'	gl_FragColor = float4( Lin, 1.0 );
		//'	return;

// nightsky
	float3 direction = normalize( input.vWorldPosition - cameraPos );
	float theta = acos( direction.y ); // elevation --> y-axis, [-pi/2, pi/2]
	float phi = atan2( direction.z, direction.x ); // azimuth --> x-axis [-pi/2, pi/2]
	float2 uv = float2( phi, theta ) / float2( 2.0 * pi, pi ) + float2( 0.5, 0.0 );
	float3 L0 = float3(0.1, 0.1, 0.1) * Fex;

// composition + solar disc
	float sundisk = smoothstep( sunAngularDiameterCos, sunAngularDiameterCos + 0.00002, cosTheta );
	L0 += ( input.vSunE * 19000.0 * Fex ) * sundisk;

	float3 texColor = ( Lin + L0 ) * 0.04;// + float3( 0.0, 0.0003, 0.00075 );
// この時点で、Gを増やすと逆色、Gを減らすとK色
		//'	gl_FragColor = float4( texColor, 1.0 );
		//'	return;

	float3 curr = Uncharted2Tonemap( ( log2( 2.0 / pow( luminance, 4.0 ) ) ) * texColor );
	float3 color = curr * whiteScale;

	float ac = 1.0 / ( 1.2 + ( 1.2 * input.vSunfade ) );
	float3 retColor = pow( color, float3(ac, ac, ac) );

	//gl_FragColor = float4( retColor, 1.0 );
	return float4( retColor, 1.0 );
}

//------------------------------------------------------------------------------


technique Forward_Geometry_UnLighting
{
    ShadingModel = Unlit;
    pass Pass1
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}
