#ifndef _POSTEFFECT
#define _POSTEFFECT

#include "value.fx"
#include "func.fx"



// ==============================
// PostEffect Shader
// g_tex_0 : Target Copy Texture
// g_int_0 : Fade Type; 0 - Out, 1 - In
// g_float_0 : Fade Time
// ==============================


#define FADETYPE     g_int_0
#define FADETIME      g_float_0

struct VTX_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VTX_OUT VS_PostEffect(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
   // output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
   // output.vUV = _in.vUV;
    
    output.vPosition.xy = _in.vLocalPos * 2.f;
    output.vPosition.zw = 1.f;
    
    return output;
}


float4 PS_PostEffect(VTX_OUT _in) : SV_Target
{
    //return float4(1.f, 0.f, 0.f, 1.f);
    
    float4 vOriginalColor = (float4) 0.f;
    float4 vBloomColor = (float4) 0.f;
    float4 vDownSampleColor = (float4) 0.f;
    float fDOFDepth = 0.f;
    
    
    float vDOFDepth = 0.f;
    
    // 호출된 픽셀의 화면 전체 기준 UV
    float2 vFullUV = _in.vPosition.xy / g_vResolution;
        
    vOriginalColor = g_tex_0.Sample(g_sam_0, vFullUV);
    vDownSampleColor = g_tex_1.Sample(g_sam_0, vFullUV);
    fDOFDepth = g_tex_2.Sample(g_sam_0, vFullUV);
    vBloomColor = g_tex_3.Sample(g_sam_0, vFullUV);
    
    fDOFDepth = clamp(fDOFDepth, 0.1f, 0.8f);
    
    vOriginalColor = lerp(vOriginalColor, vDownSampleColor * 0.81f, fDOFDepth);
    
    vOriginalColor += vBloomColor;
    
    return Fade_In_Out(vOriginalColor, FADETYPE, FADETIME);
}






#endif