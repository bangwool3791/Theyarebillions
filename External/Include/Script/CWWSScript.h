#pragma once
#include <Engine/CScript.h>

class CWWSScript :
    public CScript
{
private:
    BUILD_STATE                 m_eBuildState;
    Vec3                        m_vMousePos;
    CGameObject*                m_pLevelMouseObject;
    CGameObject*                m_pTileObject;
    bool                        m_bCheck[40000]{};
    UINT                        m_iIndex = 0;

    queue<UINT> m_result{};
    queue<UINT> m_queue{};
    vector<tTile> m_vectoken{};
    float m_fDt = 0.f;

    void SetTile(UINT _iTile);
    void SetTile(UINT _iTile, UINT _iValue);
    bool IsBlocked(UINT _iTile);
    void SetTileInfo(queue<UINT>& que, queue<UINT>& result, UINT _value);
public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);
public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
public:
    CLONE(CWWSScript);
public:
    CWWSScript();
    virtual ~CWWSScript();
};
