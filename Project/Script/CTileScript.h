#pragma once
#include <Engine/CScript.h>

class Terrain;
class CGameObjectEx;

class CTileScript :
    public CScript
{
private:
    CGameObject*        m_pCameraObject;
    Vec2                m_vRenderResolution;
    tTile               m_tTileInfo;
private:
    bool Picking(Vec3 vPos);
public:
    virtual void begin();
    virtual void tick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);
public:
    CLONE(CTileScript);
public:
    CTileScript();
    virtual ~CTileScript();
};

