#ifndef _STD2D
#define _STD2D

#include "value.fx"
#include "func.fx"
static const float PI = 3.14159265f;

struct VTX_INPUT
{
    float3 vPos : POSITION; // Sementic
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;    
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;    
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
    float2 vProjPos : POSITION;
    float3 vViewPos : POSITION1;
};

// ==========================
// Std2D Shader
// g_tex_0 : Output Texture
// ==========================

VTX_OUT VS_Std2D(VTX_INPUT _in)
{ 
    VTX_OUT output = (VTX_OUT) 0.f;                    
    
    float4 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);
        
    output.vPosition = vProjPos;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    output.vProjPos = output.vPosition.xy;
    output.vViewPos = vViewPos.xyz;
        
    return output;
}

float4 PS_Std2D(VTX_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    if (g_Anim2DInfo[0].iAnimUse[0])
    {        
        float2 AnimUV = float2(g_Anim2DInfo[0].vLeftTop.x + g_Anim2DInfo[0].vStep.x * _in.vUV.x, g_Anim2DInfo[0].vLeftTop.y + g_Anim2DInfo[0].vStep.y * _in.vUV.y);
        vColor = g_animtex.Sample(g_sam_0, AnimUV);        
    }
    else
    {
        vColor = g_tex_0.Sample(g_sam_0, _in.vUV);        
    }    
     
    
    tLightColor LightColor = (tLightColor) 0.f;
        
    for (uint i = 0; i < iLight2DCount; ++i)
    {
        CalLight2D(LightColor, _in.vViewPos, i);        
    }
    
    vColor.rgb = (vColor.rgb * LightColor.vDiff.rgb) + (vColor.rgb * LightColor.vAmb.rgb);
        
    return vColor;
}


// =========================
// Collider2D Shader
// g_int_0 : Collision check
// =========================
float4 VS_Collider2D(float3 _vPos : POSITION) : SV_Position
{
    float4 vPosition = (float4) 0.f;    
    vPosition = mul(float4(_vPos, 1.f), g_matWVP);
    
    return vPosition;
}

float4 PS_Collider2D(float4 _vScreenPos : SV_Position) : SV_Target
{
    if (g_int_0)
    {
        return float4(1.f, 0.f, 0.f, 1.f);
    }
    else
    {
        return float4(0.f, 1.f, 0.f, 1.f);
    }
}


struct PS_PARTICLE_OUT
{
    float4 vDiff : SV_Target0;
    float4 vNormal : SV_Target1;
    float4 vPosition : SV_Target2;
    float4 vDiffLight : SV_Target3; //디퍼드 단계에서 미리 빛을 라이트 타겟에 넣어놓아 그림자에 걸릴일이 없어진다 . 
                                    //또한 , 라이트 타겟을 가져와 블룸처리를 할수도 있다 
    
    float4 pDOFDepthTex : SV_Target4;
};

// =========================
// Particle Rendering Shader
// mesh : point mesh
// g_tex_0 : particle texture
// =========================
struct VTX_PARTICLE_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;   
    uint iInstID : SV_InstanceID;
};

struct VTX_PARTICLE_OUT
{
    float4 vViewPos : POSITION;
    float2 vUV : TEXCOORD;
    float fInstID : FOG;
};

struct GS_PARTICLE_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float fInstID : FOG;
};


StructuredBuffer<tParticle> g_particelbuffer : register(t100);

VTX_PARTICLE_OUT VS_Particle(VTX_PARTICLE_IN _in)
{
    VTX_PARTICLE_OUT output = (VTX_PARTICLE_OUT) 0.f;

    float3 vWorldPos = _in.vLocalPos;
    
    vWorldPos += g_particelbuffer[_in.iInstID].vWorldPos;
    
    output.vViewPos = mul(float4(vWorldPos, 1.f), g_matView);
    output.vUV = _in.vUV;
    output.fInstID = _in.iInstID;
    
    return output;
}

[maxvertexcount(6)]
void GS_Particle(point VTX_PARTICLE_OUT _in[1], inout TriangleStream<GS_PARTICLE_OUT> _stream)
{
    GS_PARTICLE_OUT arrVTX[4] = { (GS_PARTICLE_OUT) 0.f, (GS_PARTICLE_OUT) 0.f, (GS_PARTICLE_OUT) 0.f, (GS_PARTICLE_OUT)0.f };
        
    // 인스턴스 아이디로, 파티클 버퍼의 활성유무 체크
    uint instID = (uint) _in[0].fInstID;    
    if (0 == g_particelbuffer[instID].iAlive)
    {
        // 비활성 파티클인 경우 폐기
        return;
    }
    
    // 0 -- 1
    // | \  |
    // 3 -- 2
   
    
    float fRatio = (g_particelbuffer[instID].m_fCurTime / g_particelbuffer[instID].m_fMaxLife) ;
    
    float randvalue = g_particelbuffer[instID].RandValue;
    
    float fScale = (g_vec4_1 - g_vec4_0) * fRatio + g_vec4_0;   
    
    fRatio += randvalue*10.f;
    
    
    float4 vSpin = 1.f;
    float4 vSpin2 = 1.f;
    
    if (g_particelbuffer[instID].iParticleType == 0 || g_particelbuffer[instID].iParticleType == 1)
    {  
        if (g_particelbuffer[instID].iLeftSpin == 0)
        {
            vSpin = float4(cos(fRatio), sin(fRatio), 0.f, 0.f);
            vSpin2 = float4(sin(fRatio), cos(fRatio), 0.f, 0.f);
        }
        else if (g_particelbuffer[instID].iLeftSpin == 1)
        {
            vSpin2 = float4(cos(fRatio), sin(fRatio), 0.f, 0.f);
            vSpin = float4(sin(fRatio), cos(fRatio), 0.f, 0.f);
        }
    }
    
   
    arrVTX[0].vPosition = _in[0].vViewPos + float4(-fScale / 2.f, fScale / 2, 0.f, 0.f) * vSpin;
    arrVTX[0].vUV = float2(0.f, 0.f);
    arrVTX[0].fInstID = (float) instID;

    arrVTX[1].vPosition = _in[0].vViewPos + float4(fScale / 2.f, fScale / 2, 0.f, 0.f) * vSpin2;
    arrVTX[1].vUV = float2(1.f, 0.f);
    arrVTX[1].fInstID = (float) instID;

    arrVTX[2].vPosition = _in[0].vViewPos + float4(fScale / 2.f, -fScale / 2, 0.f, 0.f) * vSpin;
    arrVTX[2].vUV = float2(1.f, 1.f);
    arrVTX[2].fInstID = (float) instID;

    arrVTX[3].vPosition = _in[0].vViewPos + float4(-fScale / 2.f, -fScale / 2, 0.f, 0.f) * vSpin2;
    arrVTX[3].vUV = float2(0.f, 1.f);
    arrVTX[3].fInstID = (float) instID;
    

    for (int i = 0; i < 4; ++i)
    {
        arrVTX[i].vPosition = mul(arrVTX[i].vPosition, g_matProj);
    }
    
    _stream.Append(arrVTX[0]);
    _stream.Append(arrVTX[2]);
    _stream.Append(arrVTX[3]);
    _stream.RestartStrip();
    
    _stream.Append(arrVTX[2]);
    _stream.Append(arrVTX[0]);
    _stream.Append(arrVTX[1]);
    _stream.RestartStrip();
}


float4 PS_Particle(GS_PARTICLE_OUT _in) :SV_Target
{
    float4 vColor = (float4) 0.f;
    float4 vColor2 = (float4) 0.f;
    uint iInst = (uint) _in.fInstID;
    
    float fRatio = g_particelbuffer[iInst].m_fCurTime / g_particelbuffer[iInst].m_fMaxLife;
    
    vColor = (g_vec4_3 - g_vec4_2) * fRatio + g_vec4_2;
    vColor2 = g_tex_0.Sample(g_sam_0, _in.vUV);
    vColor2.xyzw *= vColor;
    
    if(g_int_1)
    {
        float2 FullUV = _in.vUV;
        float test = 0.f;
        
        float3 PaperBurn = g_tex_1.Sample(g_sam_1, FullUV).xyz;
	    
        //밝을수록 먼저 사라지도록 수정 
            test = (PaperBurn.x + PaperBurn.y + PaperBurn.z) / 4.f;
        
        if (test <= fRatio)
        {
            //clip(-1);
            return float4(0.f, 0.f, 0.f, 0.f);
        }
    }
    
  
    return vColor2;
    
   
}


PS_PARTICLE_OUT PS_Deffered_Particle(GS_PARTICLE_OUT _in) 
{
    PS_PARTICLE_OUT output = (PS_PARTICLE_OUT) 0.f;

    
    float4 vColor = (float4) 0.f;
    
    uint iInst = (uint) _in.fInstID;
    
    float fRatio = g_particelbuffer[iInst].m_fCurTime / g_particelbuffer[iInst].m_fMaxLife;
    vColor = (g_vec4_3 - g_vec4_2) * fRatio + g_vec4_2;
        
    vColor= g_tex_0.Sample(g_sam_0, _in.vUV) * vColor;
    
    if (vColor.a > 0.1f)
    {
        output.vDiff = vColor;
        output.vDiffLight.x = vColor.x * g_vEmis.x;
        output.vDiffLight.x = vColor.y * g_vEmis.y;
        output.vDiffLight.x = vColor.z * g_vEmis.z;
    }
    else
        clip(-1);
    
    return output;
    
}

 
#endif

