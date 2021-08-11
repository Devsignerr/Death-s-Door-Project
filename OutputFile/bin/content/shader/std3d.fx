#ifndef _STD3D
#define _STD3D

#include "value.fx"
#include "func.fx"


struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
    
    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;   
    float2 vUV : TEXCOORD;
    
    float3 vViewPos : POSITION;
    
    float3 vVeiwNormal : NORMAL;   
    float3 vViewTangent : TANGENT;
    float3 vViewBinormal : BINORMAL;
};

// =========================
// Std3D Shader
// g_tex_0 : Output Texture
// g_texcube_0 : Enviroment Map
// =========================
VTX_OUT VS_Std3D(VTX_IN _in)
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

float4 PS_Std3D(VTX_OUT _in) : SV_Target
{
    // Albedo Texture Sampling
    float4 vObjectColor = float4(1.f, 0.f, 1.f, 1.f);
    if (btex_0)
    {
        vObjectColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
        
    // Normal Mapping
    float3 vViewNormal = _in.vVeiwNormal;
    if(btex_1)
    {
        float3x3 matTBN =
        {
              _in.vViewTangent
            , _in.vViewBinormal
            , _in.vVeiwNormal
        };
        
        float3 vTangentNormal = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;
        vTangentNormal = (vTangentNormal - 0.5f) * 2.f;
        vViewNormal = normalize(mul(vTangentNormal, matTBN));
    }    
    
    tLightColor lightcolor = (tLightColor) 0.f;
    
    // 광원에 의한 최종 라이팅 누적
    for (int i = 0; i < iLight3DCount; ++i)
    {
        tLightColor color = CalLight(i, vViewNormal, _in.vViewPos);
        lightcolor.vDiff += color.vDiff;
        lightcolor.vSpec += color.vSpec;
        lightcolor.vAmb += color.vAmb;        
    }    
    
    // 환경맵핑
    if(btexcube_0)
    {
        float3 vEye = normalize(_in.vViewPos);
        float3 vWorldEye = normalize(mul(float4(vEye, 0.f), g_matViewInv).xyz);
        float3 vWorldNormal = normalize(mul(float4(_in.vVeiwNormal, 0.f), g_matViewInv).xyz);
        float3 vWorldEvyRelfect = reflect(vWorldEye, vWorldNormal);
        float4 vReflectColor = g_texcube_0.Sample(g_sam_0, vWorldEvyRelfect);
        
        vObjectColor = lerp(vObjectColor, vReflectColor, 0.9f);
    }
    
    
    float4 vColor = lightcolor.vDiff * vObjectColor
                    + lightcolor.vSpec        
                    + lightcolor.vAmb * vObjectColor;    
    return vColor;
}


// =========================
// Collider3D Shader
// g_int_0 : Collision check
// =========================
float4 VS_Collider3D(float3 _vPos : POSITION) : SV_Position
{
    float4 vPosition = (float4) 0.f;
    vPosition = mul(float4(_vPos, 1.f), g_matWVP);

    return vPosition;
}

float4 PS_Collider3D(float4 _vScreenPos : SV_Position) : SV_Target
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



// =========================
// Std3D Deferred Shader
// g_tex_0 : Output Texture
// g_texcube_0 : Enviroment Map
// =========================
VTX_OUT VS_Std3D_Deferred(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    if (iAnim3D)
    {
        Skinning(_in.vPos, _in.vTangent
        , _in.vBinormal, _in.vNormal
        , _in.vWeights, _in.vIndices, 0);
    }
        
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    // 로컬 스페이스의 위치 및 법선 벡터를 ViewSpace 까지 변환 후 픽셀 쉐이더로 전달
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV).xyz;
    output.vVeiwNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV)).xyz;
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV)).xyz;
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV)).xyz;
   
    return output;
}


struct VTX_IN_INST
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
    
    float4 vWeights : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
    
    // Instancing 당 데이터
    row_major matrix matWorld : WORLD;
    row_major matrix matWV : WV;
    row_major matrix matWVP : WVP;
    int iRowIdx : ROWINDEX; // Animaion 행렬 행

    uint iInstanceID : SV_InstanceID;
};

VTX_OUT VS_Std3D_Deferred_Inst(VTX_IN_INST _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    if (iAnim3D)
    {
        Skinning(_in.vPos, _in.vTangent
        , _in.vBinormal, _in.vNormal
        , _in.vWeights, _in.vIndices, _in.iRowIdx);
    }
    
    output.vPosition = mul(float4(_in.vPos, 1.f), _in.matWVP);
    output.vUV = _in.vUV;
    
    // 로컬 스페이스의 위치 및 법선 벡터를 ViewSpace 까지 변환 후 픽셀 쉐이더로 전달
    output.vViewPos = mul(float4(_in.vPos, 1.f), _in.matWV).xyz;
    
    output.vVeiwNormal = normalize(mul(float4(_in.vNormal, 0.f), _in.matWV)).xyz;
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), _in.matWV)).xyz;
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), _in.matWV)).xyz;
   
    return output;
}


struct PS_OUT
{
    float4 vDiff : SV_Target0;
    float4 vNormal : SV_Target1;
    float4 vPosition : SV_Target2;
    float4 vDiffLight : SV_Target3; //디퍼드 단계에서 미리 빛을 라이트 타겟에 넣어놓아 그림자에 걸릴일이 없어진다 . 
                                    //또한 , 라이트 타겟을 가져와 블룸처리를 할수도 있다 
};

PS_OUT PS_Std3D_Deferred(VTX_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    // Albedo Texture Sampling
    float4 vObjectColor = float4(0.13f, 0.13f, 0.13f, 1.f);
    if (btex_0)
    {
        vObjectColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }   
        
    // Normal Mapping
    float3 vViewNormal = _in.vVeiwNormal;
    
    if (btex_1)
    {
        float3x3 matTBN =
        {
            _in.vViewTangent
            , _in.vViewBinormal
            , _in.vVeiwNormal
        };
        
        float3 vTangentNormal = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;
        vTangentNormal = (vTangentNormal - 0.5f) * 2.f;
        vViewNormal = normalize(mul(vTangentNormal, matTBN));
    }
       
    
    // 환경맵핑
    if (btexcube_0)
    {
        float3 vEye = normalize(_in.vViewPos);
        float3 vWorldEye = normalize(mul(float4(vEye, 0.f), g_matViewInv).xyz);
        float3 vWorldNormal = normalize(mul(float4(_in.vVeiwNormal, 0.f), g_matViewInv).xyz);
        float3 vWorldEvyRelfect = reflect(vWorldEye, vWorldNormal);
        float4 vReflectColor = g_texcube_0.Sample(g_sam_0, vWorldEvyRelfect);
        
        vObjectColor = lerp(vObjectColor, vReflectColor, 0.9f);
    }
    
    output.vDiff = vObjectColor;
    output.vNormal.xyz = vViewNormal;
    output.vPosition.xyz = _in.vViewPos;
    
     // 반사광 맵이 있다면
    if (btex_2)
    {
        float4 vMtrlCoeff = g_tex_2.Sample(g_sam_0, _in.vUV);
        output.vNormal.a = encode(vMtrlCoeff);
    }
    
    // 발광 맵이 있다면
    if (btex_3)
    {
        float4 vEmissive = g_tex_3.Sample(g_sam_0, _in.vUV);
        
        if (vEmissive.x > 0.f || vEmissive.y > 0.f || vEmissive.z > 0.f)
        {            
            output.vDiffLight.x = g_vEmis.x * 2;
            output.vDiffLight.y = g_vEmis.y * 2;
            output.vDiffLight.z = g_vEmis.z * 2;
        }
        else
        {
            output.vDiffLight.x = vEmissive * 2;
            output.vDiffLight.y = vEmissive * 2;
            output.vDiffLight.z = vEmissive * 2;
        }
               
        //output.vDiffLight.xyz = vEmissive.xyz ;  //현재는 상수값이 곱해져 있지만 계수값을 넘겨 처리할수 있도록 한다 
    }
    
   //if (g_vEmis.x > 0.1f || g_vEmis.y > 0.1f || g_vEmis.z > 0.1f )
   //{
   //    output.vDiffLight.x=  g_vEmis.x*2;
   //    output.vDiffLight.y = g_vEmis.y*2;
   //    output.vDiffLight.z = g_vEmis.z*2;
   //}
    
    if (g_vDiff.x != 1.f || g_vDiff.y != 1.f || g_vDiff.z != 1.f)
   {
        output.vDiff.x = (output.vDiff.x * (1.f - g_vDiff.w)) + g_vDiff.x * g_vDiff.w;
        output.vDiff.y = (output.vDiff.y * (1.f - g_vDiff.w)) + g_vDiff.y * g_vDiff.w;
        output.vDiff.z = (output.vDiff.z * (1.f - g_vDiff.w)) + g_vDiff.z * g_vDiff.w;
   
   }
    
    
    return output;
}















// =========================
// SkyBox Shader
// mesh : sphere, cube
// g_int_0 : type
// g_tex_0 : skybox texture
// g_texcube_0 : skybox array texture
// ========================
struct VTX_SKY_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;   
};

struct VTX_SKY_OUT
{
    float4 vPosition : SV_Position;
    float3 vUVDir : NORMAL;
    float2 vUV : TEXCOORD;
};

VTX_SKY_OUT VS_SkyBox(VTX_SKY_IN _in)
{
    VTX_SKY_OUT output = (VTX_SKY_OUT) 0.f;
    
    float4 vViewPos = mul(float4(_in.vPos, 0.f), g_matView);        
    float4 vProjPos = mul(float4(vViewPos.xyz, 1.f), g_matProj);        
    vProjPos.z = vProjPos.w;
    
    output.vPosition = vProjPos;    
    output.vUV = _in.vUV;
    
    output.vUVDir = normalize(_in.vPos);
    
    return output;
}

float4 PS_SkyBox(VTX_SKY_OUT _in) : SV_Target
{   
    if (1 == g_int_0)
    {
        return g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    else
    {
        return g_texcube_0.Sample(g_sam_0, _in.vUVDir);
    }
}

#endif
