#ifndef _STRUCT
#define _STRUCT

struct tParticle
{
    float3 vWorldPos;
    float3 vWorldDir;

    float m_fCurTime;
    float m_fMaxLife;
    float m_fSpeed;
    
    int iAlive;
    int iLeftSpin;
    int iParticleType;
    
    float RandValue;
    int   padding[3];
};

struct tParticleShared
{
    int iAliveCount;
    int iPadding[3];
};

struct tLightColor
{
    float4 vDiff; // Diffuse (???ݻ籤 ????)	
    float4 vSpec; // Specular ?ݻ籤 ????
    float4 vAmb; // Ambient (ȯ?汤)
};

struct tLightInfo
{
    tLightColor color; // ???? ????

    float4 vDir; // ?????? ????
    float4 vWorldPos; // ?????? ???? ??ġ
    
    uint  eType; // ???? Ÿ??		
    float fRange; // ?????? ????
    float fAngle; // ???? ????
	
    int iPadding;
};

struct tAnim2D
{
    float2  vLeftTop;
    float2  vStep;
    int     iAnimUse[4];
};


struct tSkinningInfo
{
    float3 vPos;
    float3 vTangent;
    float3 vBinormal;
    float3 vNormal;
};

#endif
