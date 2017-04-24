
float time = 10.0;

float ln_glsl_mod(float a, float b) { return a-b*floor(a/b); }

//------------------------------------------------------------------------------
// ピクセルシェーダ
LN_SURFACE_OUTPUT BasicSS(LN_SURFACE_INPUT input)
{
	//float d = uAmplitude * displace( vTexCoord0.xy );
	//oColor = vec4( d, d, d, 1.0 );
	
	//return float4(d, d, d, 1.0);
	//return float4(1, 1, 0, 1);
	
	//LN_SURFACE_OUTPUT o;
	//o.Albedo = float4(1, 1, 0, 1);
	//return o;
	
	float2 uv = -1.0 + 2.0*input.TexCoord.xy;// / ln_ViewportPixelSize.xy;
	uv.x *= ln_ViewportPixelSize.x / ln_ViewportPixelSize.y;

    // background	 
//	float3 color = float3(0.9 + 0.2*uv.y);
	float3 color = float3(1.0, 1.0, 1.0);

    // bubbles	
	for( int i=0; i<32; i++ )
	{
        // bubble seeds
		float pha =      sin(float(i)*546.13+1.0)*0.5 + 0.5;
		float siz = pow( sin(float(i)*651.74+5.0)*0.5 + 0.5, 4.0 );
		float pox =      sin(float(i)*321.55+4.1) * ln_ViewportPixelSize.x / ln_ViewportPixelSize.y;

        // buble size, position and color
		float rad = 0.1 + 0.5*siz+sin(time/6.+pha*500.+siz)/20.;
		float2  pos = float2(
			pox+sin(time/10.+pha+siz),
			-1.0-rad + (2.0+2.0*rad) *ln_glsl_mod(pha+0.1*(-time/5.)*(0.2+0.8*siz),1.0));
		float dis = length( uv - pos );
		float3  col = lerp(
		float3(0.194*sin(time/6.0), 0.3, 0.0), 
			float3(1.1*sin(time/9.0), 0.4, 0.8),
			0.5+0.5*sin(float(i)*1.2+1.9));
		      //col+= 8.0*smoothstep( rad*0.95, rad, dis );
		
		//float3  col = lerp( float3(0.94,0.3,0.0), float3(0.1,0.4,0.8), 0.5+0.5*cos(float(i)*1.2+1.9));
		//    col+= 8.0*smoothstep( rad*0.95, rad, dis );
		
        // render
		float f = length(uv-pos)/rad;
		f = sqrt(clamp(1.0-f*f,0.0,1.0));
		color -= col.zyx *(1.0-smoothstep( rad*0.95, rad, dis )) * f;
	}

    // vigneting	
	color *= sqrt(1.5-0.5*length(uv));
	
	LN_SURFACE_OUTPUT o;
	//o.Albedo = float4(input.TexCoord.xy, 0.0,1.0);
	o.Albedo = float4(color, 1.0);
	//o.Albedo = float4(ln_ViewportPixelSize.x / 640, ln_ViewportPixelSize.y / 480, 0, 1.0);
	return o;
}

//------------------------------------------------------------------------------
// 描画テクニック
technique MainTec0
{
	pass P0
	{
		VertexShader = LN_SURFACE_VS(LN_SurfaceVS());
		PixelShader	 = LN_SURFACE_PS(BasicSS());
	}
}


