#pragma once
#include <Engine/CScript.h>

class CGameObjectEx;

class CInterfaceScript :
    public CScript
{
private:
    UINT                m_id;
    CGameObject* m_pCameraObject;
    CGameObject* m_pMouseObject;
public:
    virtual void begin();
    virtual void tick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);
public:
    CLONE(CInterfaceScript);
public:
    CInterfaceScript();
    virtual ~CInterfaceScript();
};
