#include "pch.h"

#include "CTexture.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"

CAnimator2D::CAnimator2D()
    :CComponent(COMPONENT_TYPE::ANIMATOR2D)
    , m_bRepeat(false)
    , m_mapAnim{}
{
}

CAnimator2D::~CAnimator2D()
{
    Safe_Del_Map(m_mapAnim);
}

void CAnimator2D::CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS)
{
    assert(_AtlasTex.Get());

    CAnimation2D* pAnim = FindAnimation(_strKey);
    assert(!pAnim);

    CAnimation2D* pAnimation2D = new CAnimation2D;
    pAnimation2D->Create(_strKey, _AtlasTex, _vLeftTop, _vSlice, _fStep, _iMaxFrm, _FPS);
    pAnimation2D->m_pOwner = this;
    m_mapAnim[_strKey] = pAnimation2D;
}

void CAnimator2D::finaltick()
{
    if (!IsValid(m_pCurAnim))
        return;

    if (m_pCurAnim->m_bFinish && m_bRepeat)
    {
        m_pCurAnim->Reset();
    }

    m_pCurAnim->finaltick();
}


CAnimation2D* CAnimator2D::FindAnimation(const wstring& _strKey)
{
    CAnimation2D* iter = m_mapAnim[_strKey];

    if (iter == nullptr)
        return nullptr;

    return iter;
}

void CAnimator2D::Play(const wstring& _strKey, bool _bRepeat)
{
    CAnimation2D* pAnimation = FindAnimation(_strKey);

    assert(pAnimation);

    if (IsValid(pAnimation))
    {
        m_pCurAnim = pAnimation;
    }
    m_bRepeat = _bRepeat;
    pAnimation->Reset();
}

void CAnimator2D::UpdateData()
{
    if (!IsValid(m_pCurAnim))
        return;

    m_pCurAnim->UpdateData();
}

void CAnimator2D::Clear()
{
    if (!IsValid(m_pCurAnim))
        return;

    m_pCurAnim->Clear();
}
