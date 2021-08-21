#ifndef _BLOOM
#define _BLOOM

static float2 PixelKernel[13] =
{
    { -6, 0 },
    { -5, 0 },
    { -4, 0 },
    { -3, 0 },
    { -2, 0 },
    { -1, 0 },
    { 0, 0 },
    { 1, 0 },
    { 2, 0 },
    { 3, 0 },
    { 4, 0 },
    { 5, 0 },
    { 6, 0 },
};

//미리 계산해 둔 가우스 필터의 마스크 값
static float BlurWeights[13] =
{
    0.002216f, 0.008764f, 0.026995f, 0.064759f, 0.120985f, 0.176033f, 0.199471f, 0.176033f, 0.120985f, 0.064759f,
    0.026995f, 0.008764f, 0.002216f,

};


#include "value.fx"

//g_tex_0 : DifferedLightTargetTex
//g_tex_1 : ShadowDepthTarget;
//g_tex_2 : DiffuseTarget;

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

struct PS_OUT
{
    float4 vBloomLightTarget : SV_Target0;
   // float4 vBloomShadowTarget : SV_Target1;
};



VTX_OUT VS_BLOOM(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
        
    output.vPosition.xy = _in.vLocalPos * 2.f;
    output.vPosition.zw = 1.f;
    
    //output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    //output.vUV = _in.vUV;
    
    return output;
}


PS_OUT PS_BLOOM(VTX_OUT _in) 
{
    PS_OUT output = (PS_OUT) 0.f;

    float Directions = 8.0;
    float4 BloomColor =0.f;
    float2 vFullUV = _in.vPosition.xy / g_vResolution * 4.f;
    
    if (g_int_0==0)
    {
        for (int index = 0; index < 13; ++index)
        {
            BloomColor += g_tex_0.Sample(g_sam_0, float2(vFullUV.x + (PixelKernel[index].x / g_vResolution.x * 4.f), vFullUV.y)) * BlurWeights[index];
            BloomColor += g_tex_0.Sample(g_sam_0, float2(vFullUV.x,vFullUV.y + (PixelKernel[index].x / g_vResolution.y * 4.f))) * BlurWeights[index];
        }
        
        //컬러 비율 
        float red = BloomColor.x;
        float green = BloomColor.y;
        float blue = BloomColor.z;
        
        float sum = red + green + blue;
        
        red /= sum;
        green /= sum;
        blue /= sum;
        
        red += 1.f;
        green += 1.f;
        blue += 1.f;      
        
        //흐린 색 강화 
        BloomColor = sqrt(BloomColor);
        //BloomColor = sqrt(BloomColor);
                
        BloomColor.x *= pow(red, 8);
        BloomColor.y *= pow(green, 8);
        BloomColor.z *= pow(blue, 8);
       
        BloomColor /= 30.7f;
        
        
        if (BloomColor.w>0.f)        
            output.vBloomLightTarget = BloomColor;
        else
        {
            output.vBloomLightTarget = float4(0.f, 0.f, 0.f, 0.f);

        }
    }
    
   // else if (g_int_0 ==1)
   // {
   //    float4 ShadowColor = 0.f;
   //     
   //     for (int index = 0; index < 13; ++index)
   //     {
   //         ShadowColor += g_tex_1.Sample(g_sam_0, float2(vFullUV.x + (PixelKernel[index].x / g_vResolution.x * 4.f), vFullUV.y)) * BlurWeights[index];
   //         ShadowColor += g_tex_1.Sample(g_sam_0, float2(vFullUV.x, vFullUV.y + (PixelKernel[index].x / g_vResolution.y * 4.f))) * BlurWeights[index];
   //     }
   //     
   //    // ShadowColor /= 3.4f;
   //     ShadowColor = log(ShadowColor);
   //     ShadowColor /= 1.4f;
   //     
   //    // ShadowColor += g_tex_1.Sample(g_sam_0, vFullUV);
   //     
   //     output.vBloomShadowTarget.xyz = ShadowColor.r ;
   // }
    
    return output;
}





#endif