#pragma once
#include <Engine\CScript.h>
class CSoldierScript :
    public CScript
{
private:
    UNIT_STATE       m_eState;
    bool             m_bActiveJps = false;
    bool             m_bAttack = false;
    int              m_iHp = 0;
    int              m_iAttack = 0;
    float            m_fAccTime;
    float            m_fSpeed;
    Vec3             m_vDest;
    CGameObject*     m_pTargetObject;
    Ptr<CPrefab>     m_Prefab;
    vector<Vec3>     m_vecJps;
    Vec3             m_vSource;
private:
    void SaveToFile(FILE* _File);
    void LoadFromFile(FILE* _File);
public:
    virtual void begin();
    virtual void tick();
    virtual void BeginOverlap(CCollider2D* _pOther);
    virtual void Overlap(CCollider2D* _pOther);
    virtual void EndOverlap(CCollider2D* _pOther);
    CLONE(CSoldierScript);
private:
    bool Func(CGameObject* _lhs, CGameObject* _rhs);
    void ChaseEnemy();
    void ProcessEnemy();
public :
    void Move(Int32 x, Int32 z);
    void Attck(Int32 x, Int32 z) {}
    void JpsAlgorithm(Int32 x, Int32 z);
    void SetDestPos(Vec3 _vPos);
    UINT GetHp() { return m_iHp; }
    void SetHp(UINT _iHp) { m_iHp = _iHp; }
public:
    CSoldierScript();
    virtual ~CSoldierScript();
};
