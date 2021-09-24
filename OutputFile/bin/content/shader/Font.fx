#ifndef _FONT
#define _FONT

#include "value.fx"
#include "std3d.fx"

VTX_OUT VS_Font(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    // ���� �����̽��� ��ġ �� ���� ���͸� ViewSpace ���� ��ȯ �� �ȼ� ���̴��� ����
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV).xyz;
    output.vVeiwNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV)).xyz;
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV)).xyz;
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV)).xyz;
   
    return output;
}

float4 PS_Font(VTX_OUT _in) : SV_Target
{
    // Albedo Texture Sampling
    float4 vObjectColor = float4(1.f, 0.f, 1.f, 1.f);
    if (btex_0)
        vObjectColor = g_tex_0.Sample(g_sam_0, _in.vUV);
  
    
    vObjectColor = Fade_In_Out(vObjectColor, g_int_0, g_float_0);
    
      
    if (1 == g_int_1)
    {
        vObjectColor.a *= 3.0f;
    }
       
    
    return vObjectColor;
}


#endif