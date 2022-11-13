#pragma once
#include "CScript.h"

class CMissileScript2 :
    public CScript
{
private:
    float       m_fSpeed;

public:
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _pOther) {};
    virtual void Overlap(CCollider2D* _pOther) {};
    virtual void EndOverlap(CCollider2D* _pOther) {};
    CLONE(CMissileScript2);
public:
    CMissileScript2();
    ~CMissileScript2();
};
