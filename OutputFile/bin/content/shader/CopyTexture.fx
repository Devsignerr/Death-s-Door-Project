#ifndef _COPYTEX
#define _COPYTEX

#include "value.fx"

RWTexture2D<float4> g_DestTex : register(u0);
Texture2D g_SrcTex : register(t13);
texture2D g_StaticSrcTex : register(t14);

// Compute Shader ���� Thread ID ���ϱ�
// SV_GroupID           : ȣ��� �����尡 ���� �׷��� 3���� �ε��� ��ǥ
// SV_GroupThreadID     : �׷� ������ ȣ��� �������� 3���� �ε��� ��ǥ
// SV_GroupIndex        : �׷� ������ ȣ��� �������� 1���� �ε��� ��ǥ(3�������� ������ �׷� �����带 1������ ��ġ�� ����)
// SV_DispatchTreadID   : ȣ��� �����带 ��� �׷��� �������� �������� 3���� �ε��� ��ǥ

// g_vec2_0 : ���縦 �޴� ���� �ؽ����� �ػ�
#define ORIGIN_RESOLUTION g_vec2_0
[numthreads(32, 32, 1)]
void CS_CopyTex(uint3 _iThreadIdx : SV_DispatchThreadID)
{
   // if (_iThreadIdx.x >= g_vec2_0.x || _iThreadIdx.y >= g_vec2_0.y)
   //     return;
   // 
   // if(_iThreadIdx.y == 127)
   //     g_DestTex[_iThreadIdx.xy] = float4(1.f, 0.f, 0.f, 1.f);

    g_DestTex[_iThreadIdx.xy] = float4(0.0f, 0.0f, 0.0f, 0.0f);

    if (_iThreadIdx.x >= (uint) ORIGIN_RESOLUTION.x || _iThreadIdx.y >= (uint)ORIGIN_RESOLUTION.y)
        return;

    if (0.0f != g_StaticSrcTex.SampleLevel(g_sam_0, _iThreadIdx.xy / ORIGIN_RESOLUTION.xy, 0).r)
    {
        g_DestTex[_iThreadIdx.xy] = g_StaticSrcTex.SampleLevel(g_sam_0, _iThreadIdx.xy / ORIGIN_RESOLUTION.xy, 0);
    }
    if (0.0f != g_SrcTex.SampleLevel(g_sam_0, _iThreadIdx.xy / ORIGIN_RESOLUTION.xy, 0).r)
    {
        g_DestTex[_iThreadIdx.xy] = g_SrcTex.SampleLevel(g_sam_0, _iThreadIdx.xy / ORIGIN_RESOLUTION.xy, 0);
    }


}

#endif