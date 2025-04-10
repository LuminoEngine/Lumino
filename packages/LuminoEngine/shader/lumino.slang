module lumino;

public Sampler2D ln_MaterialTexture;

public cbuffer LNRenderElementBuffer
{
    /* [0]   */ public float4x4 ln_World;
    /* [64]  */ public float4x4 ln_WorldI;
    /* [128] */ public float4x4 ln_WorldViewProjection;
    /* [192] */ public float4x4 ln_WorldView;
    /* [256] */ public float4x4 ln_WorldViewIT;
    /* [320] */ public float4 ln_BoneTextureReciprocalSize;
    /* [336] */ public float4 ln_MorphWeights;
    /* [352] */// int ln_objectId;
};  /* [368(alignd:16)] */// int dummy[3];

public namespace lumino {
    /** 
     * Standard vertex shader input structure.
     */
    public struct VSInput {
        public float3 position: POSITION;
        public float3 normal: NORMAL0;
        public float2 uv: TEXCOORD0;
        public float4 color: COLOR0;
    };
}
