

//------------------------------------------------------------------------------
// ピクセルシェーダ
LN_SURFACE_OUTPUT BasicSS(LN_SURFACE_INPUT i)
{
	//float d = uAmplitude * displace( vTexCoord0.xy );
	//oColor = vec4( d, d, d, 1.0 );
	
	//return float4(d, d, d, 1.0);
	//return float4(1, 1, 0, 1);
	
	LN_SURFACE_OUTPUT o;
	o.Albedo = float4(1, 1, 0, 1);
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


