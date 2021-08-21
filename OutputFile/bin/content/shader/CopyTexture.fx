#ifndef _COPYTEX
#define _COPYTEX

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

//�̸� ����� �� ���콺 ������ ����ũ ��
static float BlurWeights[13] =
{
    0.002216f, 0.008764f, 0.026995f, 0.064759f, 0.120985f, 0.176033f, 0.199471f, 0.176033f, 0.120985f, 0.064759f,
    0.026995f, 0.008764f, 0.002216f,

};


RWTexture2D<float4> g_DestTex : register(u0);
Texture2D g_SrcTex : register(t13);

// Compute Shader ���� Thread ID ���ϱ�
// SV_GroupID           : ȣ��� �����尡 ���� �׷��� 3���� �ε��� ��ǥ
// SV_GroupThreadID     : �׷� ������ ȣ��� �������� 3���� �ε��� ��ǥ
// SV_GroupIndex        : �׷� ������ ȣ��� �������� 1���� �ε��� ��ǥ(3�������� ������ �׷� �����带 1������ ��ġ�� ����)
// SV_DispatchTreadID   : ȣ��� �����带 ��� �׷��� �������� �������� 3���� �ε��� ��ǥ

// g_vec2_0 : �ٿ������ �ؽ�ó�� �ػ� (������ 2�� ����� �پ��)
// �ϴ� ������ 400 x 225
#define DOWNSAMPLE_RESOLUTION g_vec2_0

[numthreads(32, 32, 1)]
void CS_CopyTex(uint3 _iThreadIdx : SV_DispatchThreadID)
{
    if (_iThreadIdx.x >= g_vec2_0.x || _iThreadIdx.y >= g_vec2_0.y)
        return;
    
    float4 Color = (float4) 0.f;
 
    float2 _fUv = _iThreadIdx.xy / (DOWNSAMPLE_RESOLUTION.xy);
    int index = 0;
    
    if(g_int_0==0)
    {
        for (; index < 13; ++index)
        {
            Color += g_SrcTex.SampleLevel(g_sam_0, float2(_fUv.x + (PixelKernel[index].x / DOWNSAMPLE_RESOLUTION.x), _fUv.y), 0) * BlurWeights[index];
            Color += g_SrcTex.SampleLevel(g_sam_0, float2(_fUv.x, _fUv.y + (PixelKernel[index].x / DOWNSAMPLE_RESOLUTION.y)), 0) * BlurWeights[index];
        }
        
        g_DestTex[_iThreadIdx.xy] = Color;
    }
    
    else if (g_int_0==1)
    {
       for (; index < 13; ++index)
       {
           Color.rgb += g_SrcTex.SampleLevel(g_sam_0, float2(_fUv.x + (PixelKernel[index].x / DOWNSAMPLE_RESOLUTION.x), _fUv.y), 0).rgb * BlurWeights[index];
           Color.rgb += g_SrcTex.SampleLevel(g_sam_0, float2(_fUv.x, _fUv.y + (PixelKernel[index].x / DOWNSAMPLE_RESOLUTION.y)), 0).rgb * BlurWeights[index];
       }     
                
          //�÷� ���� 
        float red = Color.r;
        float green = Color.g;
        float blue = Color.b;
        
        float sum = red + green + blue;
        
        red /= sum;
        green /= sum;
        blue /= sum;
        
        red += 1.f;
        green += 1.f;
        blue += 1.f;
        
        //�帰 �� ��ȭ 
        Color = sqrt(Color);
        //BloomColor = sqrt(BloomColor);
                
        Color.x *= pow(red, 8);
        Color.y *= pow(green, 8);
        Color.z *= pow(blue, 8);
        
        Color /= 30.7f;
        
        
        if (Color.x > 0.f)        
            g_DestTex[_iThreadIdx.xy] = Color;
        else
        {
            g_DestTex[_iThreadIdx.xy] = float4(0.f, 0.f, 0.f, 1.f);
        }      
    }
}

#endif