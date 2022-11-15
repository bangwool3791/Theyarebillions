#ifndef _FUNC
#define _FUNC

#include "struct.fx"
#include "register.fx"


void CalcLight2D(inout tLightColor _LightColor, float3 _vWorldPos, int iLightIdx)
{
    // Directional Light
    if (0 == g_Light2DBuffer[iLightIdx].iLightType)
    {
        _LightColor.vDiff += g_Light2DBuffer[iLightIdx].color.vDiff;
    }

    // Point Light
    else if (1 == g_Light2DBuffer[iLightIdx].iLightType)
    {
        float fDistance = distance(_vWorldPos.xy, g_Light2DBuffer[iLightIdx].vWorldPos.xy);

        if (fDistance < g_Light2DBuffer[iLightIdx].fRadius)
        {
            float fRatio = 1.f - fDistance / g_Light2DBuffer[iLightIdx].fRadius;

            _LightColor.vDiff += g_Light2DBuffer[iLightIdx].color.vDiff * fRatio;
        }
    }

    // Spot Light
    else if (2 == g_Light2DBuffer[iLightIdx].iLightType)
    {
        float3 _vDir = _vWorldPos - g_Light2DBuffer[iLightIdx].vWorldPos;

        float fDistance = distance(_vWorldPos.xy, g_Light2DBuffer[iLightIdx].vWorldPos.xy);

        float fRatio = clamp(exp(2 * (dot(g_Light2DBuffer[iLightIdx].vWorldDir, _vDir / fDistance) - cos(g_Light2DBuffer[iLightIdx].fAngle * 0.5))), 0, 1);
        _LightColor.vDiff += g_Light2DBuffer[iLightIdx].color.vDiff * fRatio;

    }
}

static float GaussianFilter[5][5] =
{
    0.003f  , 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f , 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f , 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f , 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f  , 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

float4 GaussianSample(float2 _vUV)
{
    float4 vOutColor = (float4) 0.f;

    if (1.f < _vUX.x)
    {

    }
    else if (_vUV.x < 0.f)
}
#endif


