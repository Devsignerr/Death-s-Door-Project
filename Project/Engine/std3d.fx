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
    
    float4 pDOFDepthTex : SV_Target4;
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
        float4 vEmissive = g_tex_3.Sample(g_sam_0, _in.vUV.xy);    
        
        if (vEmissive.w > 0.f)
        {
            output.vDiffLight = vEmissive;         
        }
    }   
    
    output.vDiffLight.x = output.vDiffLight.x * g_vEmis.x;
    output.vDiffLight.y = output.vDiffLight.y * g_vEmis.y;
    output.vDiffLight.z = output.vDiffLight.z * g_vEmis.z;
    
     // 페이퍼번
    if (btex_4)
    {
        float2 FullUV = _in.vUV;
        float Burnf = 0;
        float test = 0.f;
        Burnf = g_vec4_0.w;
        
        float3 PaperBurn = g_tex_4.Sample(g_sam_0, FullUV).xyz;
	    
        //밝을수록 먼저 사라지도록 수정 
        if(g_int_1==1)
            test = (PaperBurn.x * 4.f + PaperBurn.y * 0.25f + PaperBurn.z * 0.25f) / 3.f;
        else if( g_int_1 ==0)
            test = (PaperBurn.x  + PaperBurn.y  + PaperBurn.z ) / 3.f;
        
        if (test < Burnf)
            clip(-1);
        else if (test < Burnf + 0.05f && test > Burnf - 0.05f)
            output.vDiff = float4(g_vec4_0.x, g_vec4_0.y, g_vec4_0.z, 1.f);
    }
    
    
     {
        //float4 vWorldPos = mul( , g_matViewInv); // 메인카메라 view 역행렬을 곱해서 월드좌표를 알아낸다.
        float4 vProj = mul(float4(_in.vViewPos, 1.f), g_matProj); // 투영시킨 좌표 구하기
        float fDepth = vProj.z / vProj.w; // w 로 나눠서 실제 투영좌표 z 값을 구한다.(올바르게 비교하기 위해서)
        
        
        //이부분을 플레이어의 위치에서 물체의 위치까지로 변경할수 있을까
        //수정사항 
        
        float Distance = (float) fDOFDistance;
        
        if (fDOFDistance+0.0015f < fDepth)
        {
            float Diff = (fDepth - 0.0015f - fDOFDistance) * 2000.f;
        //output.vDiff = float4(1.0f, 0.0f, 1.0f - Diff, 1.0f);
            output.pDOFDepthTex.r = Diff;
        }      
    }
    
    if (g_int_2 == 1)
    {
        float4 vWorldPos = mul(float4(_in.vViewPos, 1.f), g_matViewInv);
        
        if (vWorldPos.y < g_float_2)
        {
            float Diff = (g_float_2 - vWorldPos.y) / g_float_3;
            
            float4 black = float4(0.f, 0.f, 0.f, 1.f);
            
            vObjectColor = lerp(vObjectColor, black, Diff);
            
            output.vDiff = vObjectColor;
        }
    }


 
    output.vDiff.x = output.vDiff.x * g_vDiff.x;
    output.vDiff.y = output.vDiff.y * g_vDiff.y;
    output.vDiff.z = output.vDiff.z * g_vDiff.z;
   
    
    
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
