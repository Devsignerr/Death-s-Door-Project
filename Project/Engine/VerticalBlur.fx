#ifndef _VERTICALBLUR
#define _VERTICALBLUR

#include "value.fx"


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


RWTexture2D<float4> g_DestTex : register(u0);
Texture2D g_SrcTex : register(t13);

// Compute Shader 에서 Thread ID 구하기
// SV_GroupID           : 호출된 스레드가 속한 그룹의 3차원 인덱스 좌표
// SV_GroupThreadID     : 그룹 내에서 호출된 스레드의 3차원 인덱스 좌표
// SV_GroupIndex        : 그룹 내에서 호출된 스레드의 1차원 인덱스 좌표(3차원으로 구성된 그룹 스레드를 1행으로 배치한 기준)
// SV_DispatchTreadID   : 호출된 스레드를 모든 그룹을 기준으로 했을때의 3차원 인덱스 좌표

// g_vec2_0 : 다운샘플할 텍스처의 해상도 (원본의 2의 배수로 줄어듬)
// 일단 지금은 400 x 225
#define DOWNSAMPLE_RESOLUTION g_vec2_0

[numthreads(32, 32, 1)]
void CS_VerticalBlur(uint3 _iThreadIdx : SV_DispatchThreadID)
{
    if (_iThreadIdx.x >= g_vec2_0.x || _iThreadIdx.y >= g_vec2_0.y)
        return;
    
    float4 Color = (float4) 0.f;
 
    float2 _fUv = _iThreadIdx.xy / (DOWNSAMPLE_RESOLUTION.xy);
    int index = 0;
    
    if (g_int_0 == 0)
    {
        for (; index < 13; ++index)
        {       
            Color += g_SrcTex.SampleLevel(g_sam_0, float2(_fUv.x, _fUv.y + (PixelKernel[index].x / DOWNSAMPLE_RESOLUTION.y)), 0) * BlurWeights[index];
        }
        
        g_DestTex[_iThreadIdx.xy] += Color;
    }
    else if (g_int_0 == 1)
    {
        for (; index < 13; ++index)
        {        
            Color.rgb += g_SrcTex.SampleLevel(g_sam_0, float2(_fUv.x, _fUv.y + (PixelKernel[index].x / DOWNSAMPLE_RESOLUTION.y)), 0).rgb * BlurWeights[index];
        }
                
        float red = Color.r;
          //컬러 비율 
        float green = Color.g;
        float blue = Color.b;
        
        float sum = red + green + blue;
        
        red /= sum;
        green /= sum;
        blue /= sum;
        
        red += 1.f;
        green += 1.f;
        blue += 1.f;
        
        //흐린 색 강화 
        Color = sqrt(Color);
        //BloomColor = sqrt(BloomColor);
                
        Color.x *= pow(red, 8);
        Color.y *= pow(green, 8);
        Color.z *= pow(blue, 8);
        
        Color /= 60.7f;
        
        
        if (Color.x > 0.f)        
            g_DestTex[_iThreadIdx.xy] += Color;
        else
        {
            g_DestTex[_iThreadIdx.xy] = float4(0.f, 0.f, 0.f, 1.f);
        }
    }
}

#endif