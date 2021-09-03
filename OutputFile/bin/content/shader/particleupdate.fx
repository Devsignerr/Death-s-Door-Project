#ifndef _PARTICLEUPDATE
#define _PARTICLEUPDATE

#include "value.fx"
#include "func.fx"

// =========================
// Particle Update Shader
// g_int_0 : max particle count
// g_float_0 : min life time
// g_float_1 : max life time
// g_float_2 : min speed
// g_float_3 : max speed
// g_vec4_0 : world position
// g_vec4_1 : create range

// g_particlebuffer : particle data array

// =========================
RWStructuredBuffer<tParticle> g_particlebuffer : register(u0);
RWStructuredBuffer<tParticleShared> g_shared : register(u1);


[numthreads(1024, 1, 1)]
void CS_ParticleUpdate(int3 _ThreadIdx : SV_DispatchThreadID)
{
    if (g_int_0 <= _ThreadIdx.x)
        return;
    
    // particle update
    if (0 == g_particlebuffer[_ThreadIdx.x].iAlive)
    {
        // 2. 스레드 동기화
        // - atomic function
        int iOrigin = g_shared[0].iAliveCount;
        int iExchange = 0;
        
        while (0 < iOrigin)
        {
            int iInput = iOrigin - 1;
            InterlockedCompareExchange(g_shared[0].iAliveCount, iOrigin, iInput, iExchange);
        
            // 성공
            if (iOrigin == iExchange)
            {
                // 파티클 활성화       
                g_particlebuffer[_ThreadIdx.x].iAlive = 1;
                break;
            }
            
            iOrigin = iInput;
        }    
        
        if (0 == g_particlebuffer[_ThreadIdx.x].iAlive)
            return;        
        
        
        // 활성화 된 파티클을 랜덤한 좌표, 방향으로 초기화한다.
        float fKey = _ThreadIdx.x / (float) g_int_0; // 각 스레드가 겹치지 않는 키값을 얻어내기 위해서       
        float3 vRand = float3(Rand(fKey), Rand(fKey * 2.f), Rand(fKey * 3.f)); // 3번의 랜덤을 구함(키값 중복을 피하기 위해서 배수함)
        
        // vRand 의 각 성분값이 0 ~ 1 이므로, -0.5 ~ 0.5 범위로 확장 후 범위를 곱한다.
        float3 vPositionRange = (vRand - 0.5f) * g_vec4_1.xyz;
              
        g_particlebuffer[_ThreadIdx.x].iParticleType = g_int_1;
        
        g_particlebuffer[_ThreadIdx.x].vWorldPos = vPositionRange + g_vec4_0.xyz;
        
        
	    //위로 올라가는 타입 UP
        if(g_int_1==0)
            g_particlebuffer[_ThreadIdx.x].vWorldDir = float3(0, 1, 0);
        
        //땅으로 넓게 퍼지는 타입 SPREAD_XZ
        else if (g_int_1 == 1)
        {
            vPositionRange.y = 0.f;
            g_particlebuffer[_ThreadIdx.x].vWorldDir = normalize(vPositionRange);
        }       
        
        //사방으로 넓게 퍼지는 타입 SPREAD_XYZ
        else if (g_int_1 == 2)                 
            g_particlebuffer[_ThreadIdx.x].vWorldDir = normalize(vPositionRange);
        
        //가운데로 모이는 타입 GATHER
        else if (g_int_1 == 3)
        {
            g_particlebuffer[_ThreadIdx.x].vWorldDir = -normalize(vPositionRange);
        }
             
        //제자리에서 머무는 타입 STAY
        else if (g_int_1 == 4)                 
            g_particlebuffer[_ThreadIdx.x].vWorldDir = normalize(vPositionRange)/10.f;
        
  
    	// 얇게 길어지며 퍼지는 타입 SPREAD_LONG
        else if (g_int_1 == 5)
        {
            g_particlebuffer[_ThreadIdx.x].vWorldDir = normalize(vPositionRange);
        }
             
        
        g_particlebuffer[_ThreadIdx.x].m_fSpeed = g_float_2 + (g_float_3 - g_float_2) * vRand.x;
        
        g_particlebuffer[_ThreadIdx.x].m_fCurTime = 0.f;
        g_particlebuffer[_ThreadIdx.x].m_fMaxLife = g_float_0 + (g_float_1 - g_float_0) * vRand.y;
        
        float Random = Rand(fKey) * 2.f;
        int Spin = 0;
        modf(Random, Spin);
        g_particlebuffer[_ThreadIdx.x].iLeftSpin = Spin;
        
    }
    else
    {
        float slowSpeed = 0.f;
        
        if (g_int_2 != 0)
        {         
            g_particlebuffer[_ThreadIdx.x].m_fSpeed -= g_DT * g_int_2;
            if(g_particlebuffer[_ThreadIdx.x].m_fSpeed < 0.f)
                g_particlebuffer[_ThreadIdx.x].m_fSpeed = 0.f;

        }     
        
       // 파티클 업데이트        
        g_particlebuffer[_ThreadIdx.x].m_fCurTime += g_DT;
        
        if (g_particlebuffer[_ThreadIdx.x].m_fCurTime >= g_particlebuffer[_ThreadIdx.x].m_fMaxLife)
        {
            g_particlebuffer[_ThreadIdx.x].iAlive = 0;
            return;
        }
        
        if (g_particlebuffer[_ThreadIdx.x].iParticleType==5)
        {
            //0~1 사이 값
            float Ratio = g_particlebuffer[_ThreadIdx.x].m_fCurTime / g_particlebuffer[_ThreadIdx.x].m_fMaxLife;
            float AdditionalSpeed = sin(Ratio * 3.1412);
            AdditionalSpeed = pow(AdditionalSpeed, 3);
            g_particlebuffer[_ThreadIdx.x].vWorldPos += g_particlebuffer[_ThreadIdx.x].vWorldDir * g_particlebuffer[_ThreadIdx.x].m_fSpeed * g_DT * AdditionalSpeed;
        }
        else
        {
            g_particlebuffer[_ThreadIdx.x].vWorldPos += g_particlebuffer[_ThreadIdx.x].vWorldDir * g_particlebuffer[_ThreadIdx.x].m_fSpeed * g_DT;
        }
        
    }       
}










#endif