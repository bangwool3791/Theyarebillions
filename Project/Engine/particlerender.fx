#ifndef _PARTICLERENDER
#define _PARTICLERENDER

#include "register.fx"


// ======================
// Particle Render Shader
// RS_TYPE : CULL_NONE
// BS_TYPE : ALPHABLEND
// DS_TYPE : NO_WRITE
// DOMAIN : Transparent
StructuredBuffer<tParticle> ParticleBuffer : register(t17);
#define ParticleIndex g_int_0
// ======================


struct VS_IN
{
    float3 vPos : POSITION;
    uint iInstance  : SV_InstanceID;
};

struct VS_OUT
{
    float3 vLocalPos : POSITION;
    uint iInstance : SV_InstanceID;
};



VS_OUT VS_ParticleRender(VS_IN _in)
{
    VS_OUT output = (VS_OUT)0.f;

    output.vLocalPos = _in.vPos;
    output.iInstance = _in.iInstance;

    return output;
}

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};


[maxvertexcount(6)]
void GS_ParticleRender(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _OutStream)
{
    GS_OUT output[4] = { (GS_OUT)0.f, (GS_OUT)0.f, (GS_OUT)0.f, (GS_OUT)0.f };

    if (0 == ParticleBuffer[_in[0].iInstance].iActive)
        return;


    float3 vWorldPos = _in[0].vLocalPos + ParticleBuffer[_in[0].iInstance].vRelativePos.xyz;
    float3 vViewPos = mul(float4(vWorldPos, 1.f), g_matView);



    float3 NewPos[4] =
    {
        vViewPos - float3(-0.5f, 0.5f, 0.f) * float3(100.f, 100.f, 1.f),
        vViewPos - float3(0.5f, 0.5f, 0.f) * float3(100.f, 100.f, 1.f),
        vViewPos - float3(0.5f, -0.5f, 0.f) * float3(100.f, 100.f, 1.f),
        vViewPos - float3(-0.5f, -0.5f, 0.f) * float3(100.f, 100.f, 1.f)
    };

    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition = mul(float4(NewPos[i], 1.f), g_matProj);
    }


    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);


    // 0 -- 1
    // | \  |
    // 3 -- 2
    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
    _OutStream.Append(output[2]);
    _OutStream.RestartStrip();

    _OutStream.Append(output[0]);
    _OutStream.Append(output[2]);
    _OutStream.Append(output[3]);
    _OutStream.RestartStrip();
}




float4 PS_ParticleRender(GS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;

    vColor = g_tex_0.Sample(g_sam_0, _in.vUV);

    return vColor;

}









#endif