#ifndef _POSTEFFECT
#define _POSTEFFECT

#include "value.fx"

// ==============================
// PostEffect Shader
// g_tex_0 : Target Copy Texture
// ==============================

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
    
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}


float4 PS_PostEffect(VTX_OUT _in) : SV_Target
{
    //return float4(1.f, 0.f, 0.f, 1.f);
    
    float4 vColor = (float4) 0.f;
       
    // 호출된 픽셀의 화면 전체 기준 UV
    float2 vFullUV = _in.vPosition.xy / g_vResolution;    
    
    vFullUV.x = vFullUV.x + cos(vFullUV.y * 6.28 + g_AccTime * 2.f) * 0.1f;
    
    vColor = g_tex_0.Sample(g_sam_0, vFullUV);  
       
    return vColor;
}






#endif