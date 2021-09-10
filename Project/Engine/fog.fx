#ifndef _UI
#define _FOG

#include "value.fx"
#include "func.fx"
#include "std3d.fx"


VTX_OUT VS_Fog(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
   
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV).xyz;
    output.vVeiwNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV)).xyz;
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV)).xyz;
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV)).xyz;
   
    return output;
}

float4 PS_Fog(VTX_OUT _in) : SV_Target
{
    
    //float4 mainColor = g_tex_0.Sample(g_sam_0, _in.vUV).xyz;
    float4 mainColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    float4 wpos = mul(float4(_in.vViewPos, 1.f), g_matViewInv);
    //wpos.y *= -1.0f;
    
    
    float fog = calcFogHeightUniform(wpos.xyz, g_CamWorldPos, g_float_0, g_float_1);
    
    return lerp(float4(0.f, 0.f, 1.f, 1.f), float4(1.f, 0.f, 0.f, 0.1f), fog);
}


#endif