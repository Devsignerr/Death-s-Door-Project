#ifndef _FIRE
#define _FIRE

#include "value.fx"
#include "std3d.fx"


VTX_OUT VS_Fire(VTX_IN_INST _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
        
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    // 로컬 스페이스의 위치 및 법선 벡터를 ViewSpace 까지 변환 후 픽셀 쉐이더로 전달
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV).xyz;
    output.vVeiwNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV)).xyz;
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV)).xyz;
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV)).xyz;
   
    return output;
}


PS_OUT PS_Fire(VTX_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float3 vViewNormal = _in.vVeiwNormal;
    
    float4 BrighterColor = g_vDiff;
    float4 MiddleColor = g_vAmb;
    float4 DarkerColor = g_vSpec;
  
    float2 UV = _in.vUV + float2(0, g_AccTime/2.f);   
    
    float NoiseTex = g_tex_0.Sample(g_sam_0, UV).x;
    float gradientValue = g_tex_1.Sample(g_sam_0, _in.vUV).x;

    float step1 = step(NoiseTex, gradientValue);
    float step2 = step(NoiseTex, gradientValue - 0.3);
    float step3 = step(NoiseTex, gradientValue - 0.5);
    
    //if (step1 == 0.0f) 
    //    clip(-1);
  
    float4 c = float4(lerp(BrighterColor, DarkerColor, step1 - step2));
    
    output.vDiff.xyz = lerp(c.rgb, MiddleColor.rgb, step2 - step3);
    
    if (_in.vUV.x < 0.3f)
    {
        output.vDiff.xyz = g_vEmis;
    }
 
          
    //output.vDiff = vObjectColor;
    output.vNormal.xyz = vViewNormal;
    output.vPosition.xyz = _in.vViewPos;
    
    
     {
        //float4 vWorldPos = mul( , g_matViewInv); // 메인카메라 view 역행렬을 곱해서 월드좌표를 알아낸다.
        float4 vProj = mul(float4(_in.vViewPos, 1.f), g_matProj); // 투영시킨 좌표 구하기
        float fDepth = vProj.z / vProj.w; // w 로 나눠서 실제 투영좌표 z 값을 구한다.(올바르게 비교하기 위해서)
        
        
        
        float Distance = (float) fDOFDistance;
        
        if (fDOFDistance + 0.0015f < fDepth)
        {
            float Diff = (fDepth - 0.0015f - fDOFDistance) * 2000.f;
        //output.vDiff = float4(1.0f, 0.0f, 1.0f - Diff, 1.0f);
            output.pDOFDepthTex.r = Diff;
        }
    }
    

  
    return output;
}



PS_OUT PS_Fire2(VTX_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float3 vViewNormal = _in.vVeiwNormal;
    
    float4 BrighterColor = g_vDiff;
    float4 MiddleColor = g_vAmb;
    float4 DarkerColor = g_vSpec;
  
    float2 UV = _in.vUV - float2(0, g_AccTime);
    
    float NoiseTex = g_tex_0.Sample(g_sam_0, UV).x;
    float gradientValue = g_tex_1.Sample(g_sam_0, _in.vUV).x;

    float step1 = step(NoiseTex, gradientValue);
    float step2 = step(NoiseTex, gradientValue - 0.4);
    float step3 = step(NoiseTex, gradientValue - 0.5);
    
    //if (step1 == 0.0f) 
    //    clip(-1);
  
    float4 c = float4(lerp(BrighterColor, DarkerColor, step1 - step2));
    
    output.vDiff.xyz = lerp(c.rgb, MiddleColor.rgb, step2 - step3);
    
    if (_in.vUV.x < 0.3f)
    {
        output.vDiff.xyz = g_vEmis;
    }
 
          
    //output.vDiff = vObjectColor;
    output.vNormal.xyz = vViewNormal;
    output.vPosition.xyz = _in.vViewPos;
    
    
     {
        //float4 vWorldPos = mul( , g_matViewInv); // 메인카메라 view 역행렬을 곱해서 월드좌표를 알아낸다.
        float4 vProj = mul(float4(_in.vViewPos, 1.f), g_matProj); // 투영시킨 좌표 구하기
        float fDepth = vProj.z / vProj.w; // w 로 나눠서 실제 투영좌표 z 값을 구한다.(올바르게 비교하기 위해서)
        
        
        
        float Distance = (float) fDOFDistance;
        
        if (fDOFDistance + 0.0015f < fDepth)
        {
            float Diff = (fDepth - 0.0015f - fDOFDistance) * 2000.f;
        //output.vDiff = float4(1.0f, 0.0f, 1.0f - Diff, 1.0f);
            output.pDOFDepthTex.r = Diff;
        }
    }
    
  
    return output;
}


#endif