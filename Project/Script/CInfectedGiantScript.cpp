#include "pch.h"
#include "CInfectedGiantScript.h"

#include <Engine\CDevice.h>
#include <Engine\CLayer.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CCollisionMgr.h>
#include <Engine\CScript.h>
#include <Engine\CCollider2D.h>
#include <Engine\CTransform.h>
#include <Engine\CMaterial.h>
#include <Engine\CMeshRender.h>
#include <Engine\CJpsMgr.h>

#include <Engine\CInterfaceMgr.h>
#include <Engine\CScript.h>

#include "CSoldierScript.h"
#include "CRangerScript.h"
#include "CSniperScript.h"
#include "CTitanScript.h"

#include "CCommandScript.h"
#include "CTentScript.h"
#include "CHuntScript.h"
#include "CSawScript.h"
#include "CQuarryScript.h"
#include "CSCScript.h"
#include "CWWSScript.h"
#include "CPOSScript.h"
#include "CWWCScript.h"
#include "CWWallScript.h"

CInfectedGiantScript::CInfectedGiantScript()
	:CScript{ INFECTEDGIANTSCRIPT }
	, m_fSpeed{ 100.f }
	, m_eState{ UNIT_STATE::NORMAL }
	, m_fTick{0.f}
	, m_bAttack {true}
	, m_fAccTime{}
	, m_pTargetObject{}
{
	m_fHP = 100;
	m_iAttack = 15;

	SetName(L"CInfectedGiantScript");

	AddScriptParam(SCRIPT_PARAM::FLOAT, "Player MoveSpeed", &m_fSpeed);
}

CInfectedGiantScript::~CInfectedGiantScript()
{
}


void CInfectedGiantScript::begin()
{
	CAnimator2D* Animator = GetOwner()->Animator2D();

	if (nullptr == Animator)
		GetOwner()->AddComponent(new CAnimator2D());

	Animator = GetOwner()->Animator2D();

	Animator->CloneAnimation(L"InfectedGiant", *Animator);

	Vec3 vPos = Transform()->GetRelativePos();
	vPos.x += 1.f;
	vPos.z -= 1.f;
	SetDestPos(vPos);

	m_vSource = Transform()->GetRelativePos();
}

#define OFFSET 22.5f

void CInfectedGiantScript::tick()
{
	if (0 > m_fHP)
	{
		m_eState = UNIT_STATE::DEAD;
		//CEffectWoodPrefab
		CGameObject* pObj = CResMgr::GetInst()->FindRes<CPrefab>(L"CEffectBloodPrefab")->Instantiate();
		Instantiate(pObj, Transform()->GetRelativePos(), 3);
		GetOwner()->Destroy();
	}

	if (m_pTargetObject && m_pTargetObject->IsDead())
	{
		m_pTargetObject = nullptr;
		m_eState = UNIT_STATE::NORMAL;
	}

	m_fTick += DT;

	if (KEY_PRESSED(KEY::LBTN))
	{
		const Ray& ray = GetRay();

		if (GetOwner()->Transform()->Picking(ray, m_vDest))
		{
			CInterfaceMgr::GetInst()->SetTarget(GetOwner());
		}
	}

	if (UNIT_STATE::NORMAL == m_eState)
	{
		SetDestPos(m_vDest);
		ProcessEnemy();
		ChaseEnemy();
	}
	else if (UNIT_STATE::RUN == m_eState)
	{
		Vec3 vDir{};

		auto iter = m_vecJps.begin();
		if (iter == m_vecJps.end())
		{
			m_eState = UNIT_STATE::NORMAL;
			return;
		}

		Vec3 vDest = *iter;
		vDir = *iter - Transform()->GetRelativePos();
		vDir.Normalize();
		m_vDest = DT * vDir * m_fSpeed + Transform()->GetRelativePos();

		Transform()->SetRelativePos(m_vDest);
		SetDestPos(vDest);
		m_vSource = Transform()->GetRelativePos();

		Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();

		if (m_vSource.Distance(m_vSource, *iter) < vScale.Length() * 0.5f)
			m_vecJps.erase(iter);

		if (m_vecJps.empty())
		{
			m_bActiveJps = false;

			if (m_pTargetObject)
				ChaseEnemy();
			else
			{
				m_eState = UNIT_STATE::NORMAL;
				SetDestPos(vDest);
			}
		}

		if (m_bAttack)
			ProcessEnemy();
	}
	else if (UNIT_STATE::ATTACK == m_eState)
	{

		if (!m_pTargetObject)
		{
			if (m_bActiveJps)
				m_eState = UNIT_STATE::RUN;
			else
			{
				m_eState = UNIT_STATE::NORMAL;
				m_bAttack = false;
			}

			m_pTargetObject = nullptr;
		}

		if (m_fTick >= 2.f)
		{
			if (m_pTargetObject)
			{
				SetPlayerHP();
			}

			m_fTick -= 2.f;
		}

		if (m_pTargetObject)
		{
			Vec3 vDest = m_pTargetObject->Transform()->GetRelativePos();
			SetDestPos(vDest);
		}

		ChaseEnemy();
	}
	else if (UNIT_STATE::LOADATTACK == m_eState)
	{
		if (m_fTick >= 2.f)
		{
			if (m_pTargetObject)
			{
				Vec3 vPos = m_pTargetObject->Transform()->GetRelativePos() - m_vSource;
				m_eState = UNIT_STATE::ATTACK;
				SetDestPos(vPos);
			}
			m_fTick -= 2.f;
		}

	}
	else if (UNIT_STATE::DEAD == m_eState)
	{
		Vec3 vPos = Transform()->GetRelativePos();
		vPos.x += 1;
		vPos.z += 1;

		SetDestPos(vPos);

		if (Animator2D()->IsEnd())
		{
			Destroy();
		}
	}


	//Set Animation according to radian

}

void CInfectedGiantScript::SetDestPos(Vec3 _vPos)
{
	m_vTarget = _vPos;
	/*
	* 방향 벡터
	* x축 기저 벡터
	* 내적
	*/

	Vec3 vDir = m_vTarget - GetOwner()->Transform()->GetRelativePos();
	vDir = vDir.Normalize();

	Vec3 vAxis{ 1.f, 0.f, 0.f };

	if (0 <= vAxis.Dot(vDir))
	{
		GetOwner()->Transform()->SetRelativeRotationX(XM_PI * -0.25f);
		GetOwner()->Transform()->SetRelativeRotationY(0.f);
		GetOwner()->Transform()->SetRelativeRotationZ(0.f);
	}
	else if (0 > vAxis.Dot(vDir))
	{
		GetOwner()->Transform()->SetRelativeRotationX(XM_PI * 0.25f);
		GetOwner()->Transform()->SetRelativeRotationY(0.f);
		GetOwner()->Transform()->SetRelativeRotationZ(XM_PI);
	}

	float fAngle = acosf(vAxis.Dot(vDir));

	if (m_vTarget.z < GetOwner()->Transform()->GetRelativePos().z)
		fAngle = 2 * XM_PI - fAngle;

	if (XM_PI * 0.5f <= fAngle && XM_PI > fAngle)
		fAngle = XM_PI - fAngle;
	//3사분면
	if (XM_PI <= fAngle && XM_PI * 1.5f > fAngle)
	{
		fAngle = fAngle - XM_PI;
		fAngle = 2.f * XM_PI - fAngle;
	}

	fAngle = fAngle / XM_PI * 180;

	wstring str{};

	if (UNIT_STATE::NORMAL == m_eState)
		str = L"InfectedGiant_Normal";
	else if (UNIT_STATE::LOADATTACK == m_eState)
		str = L"InfectedGiant_LoadAttack";
	else if (UNIT_STATE::ATTACK == m_eState)
		str = L"InfectedGiant_Attack";
	else if (UNIT_STATE::RUN == m_eState)
		str = L"InfectedGiant_Walk";
	else if (UNIT_STATE::DEAD == m_eState)
	{
		str = L"InfectedGiant_Die_000";
		if (m_strBackUp != str)
			GetOwner()->Animator2D()->Play(str);

		m_strBackUp = str;
		return;
	}

	if (360.f <= fAngle)
		fAngle -= 360.f;

	if (fAngle <= 90 + OFFSET && 90 - OFFSET < fAngle)
	{
		str += L"_234";
	}
	else if (fAngle <= 67.5 + OFFSET && 67.5 - OFFSET < fAngle)
	{
		str += L"_252";
	}
	else if (fAngle <= 45.f + OFFSET && 45.f - OFFSET < fAngle)
	{
		str += L"_270";
	}
	else if (fAngle <= 22.5f + OFFSET && 22.5f - OFFSET < fAngle)
	{
		str += L"_288";
	}
	else if (fAngle <= 0.f + OFFSET && 0.f - OFFSET < fAngle)
	{
		str += L"_306";
	}
	else if (fAngle <= 360.f - 22.5f + OFFSET && 360.f - 22.5f - OFFSET < fAngle)
	{
		str += L"_000";
	}
	else if (fAngle <= 360.f - 45.f + OFFSET && 360.f - 45.f - OFFSET < fAngle)
	{
		str += L"_018";
	}
	else if (fAngle <= 360.f - 67.5f + OFFSET && 360.f - 67.5f - OFFSET < fAngle)
		str += L"_036";
	else if (fAngle <= 360.f - 90.f + OFFSET && 360.f - 90.f - OFFSET < fAngle)
		str += L"_036";

	if (m_strBackUp != str)
		GetOwner()->Animator2D()->Play(str);

	m_strBackUp = str;
}

void CInfectedGiantScript::JpsAlgorithm(Int32 x, Int32 z)
{
	Vec3 vPos = Transform()->GetRelativePos();
	tTile tTile = CJpsMgr::GetInst()->GetTileObj()->TileMap()->GetInfo(vPos);
	Int32 x1 = tTile.iIndex % TILEX;
	Int32 z1 = tTile.iIndex / TILEX;
	m_vecJps.clear();

	//cout << "시작 타일 인덱스 " << x1 << " " << z1 << endl;
	//cout << "종료 타일 인덱스 " << x << " " << z << endl;
	if (x1 != x || z1 != z)
	{
		m_vecJps = CJpsMgr::GetInst()->UpdateMonsterPath(x1, z1, x, z);

		m_bActiveJps = true;

		m_eState = UNIT_STATE::RUN;
	}
}

void CInfectedGiantScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CInfectedGiantScript::Overlap(CCollider2D* _pOther)
{
	//유닛 공통 영역
	const wstring& str = _pOther->GetOwner()->GetName();

	if (L"Solider" == str || L"CRanger" == str || L"CSniper" == str || L"CTitan" == str)
	{
		Vec2 vRelativePos = GetOwner()->Collider2D()->GetFinalPos() - _pOther->GetFinalPos();
		Vec2 vScale = (GetOwner()->Collider2D()->GetScale() + _pOther->GetScale()) * 0.5f;
		Vec3 vDiff{};
		vDiff.x = fabsf(vScale.x - fabsf(vRelativePos.x));
		vDiff.z = fabsf(vScale.y - fabsf(vRelativePos.y));
		Vec3 vDir = { vRelativePos.x , 0.f, vRelativePos.y };
		vDir = vDir.Normalize();

		Vec3 vPos = Transform()->GetRelativePos();

		Transform()->SetRelativePos(vPos + vDiff * vDir * DT * 8);
	}
	else if (str.find(L"CInfected") != std::wstring::npos)
	{
		Vec2 vRelativePos = GetOwner()->Collider2D()->GetFinalPos() - _pOther->GetFinalPos();
		Vec2 vScale = (GetOwner()->Collider2D()->GetScale() + _pOther->GetScale()) * 0.5f;
		Vec3 vDiff{};
		vDiff.x = fabsf(vScale.x - fabsf(vRelativePos.x));
		vDiff.z = fabsf(vScale.y - fabsf(vRelativePos.y));
		Vec3 vDir = { vRelativePos.x , 0.f, vRelativePos.y };
		vDir = vDir.Normalize();

		Vec3 vPos = Transform()->GetRelativePos();

		Transform()->SetRelativePos(vPos + vDiff * vDir * DT * 8);
	}
}

void CInfectedGiantScript::EndOverlap(CCollider2D* _pOther)
{
}

void CInfectedGiantScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
	fwrite(&m_fSpeed, sizeof(float), 1, _File);

	SaveResourceRef(m_Prefab, _File);
}

void CInfectedGiantScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
	fread(&m_fSpeed, sizeof(float), 1, _File);

	LoadResourceRef(m_Prefab, _File);
}

void CInfectedGiantScript::ProcessEnemy()
{
	vector<CGameObject*> vecEnemy = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(1)->GetParentObjects();

	if (!vecEnemy.empty())
	{
		sort(vecEnemy.begin(), vecEnemy.end(), [&](CGameObject* lhs, CGameObject* rhs)
			{
				Vec3 vRhs = rhs->Transform()->GetRelativePos();
				Vec3 vLhs = lhs->Transform()->GetRelativePos();

				if (m_vSource.Distance(m_vSource, vRhs) > m_vSource.Distance(m_vSource, vLhs))
					return true;
				else
					return false;
			});

		Vec3 vEnemyPos = vecEnemy[0]->Transform()->GetRelativePos();
		Vec3 vScale1 = vecEnemy[0]->Transform()->GetRelativeScale() * 0.5f;
		Vec3 vScale2 = Transform()->GetRelativeScale() * 0.5f;

		if (m_vSource.Distance(m_vSource, vEnemyPos) <= vScale1.Distance(vScale1, vScale2) + 50.f)
		{
			m_pTargetObject = vecEnemy[0];
			m_eState = UNIT_STATE::LOADATTACK;
			SetDestPos(vEnemyPos);
		}
	}
}

void CInfectedGiantScript::ChaseEnemy()
{
	vector<CGameObject*> vecEnemy = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(1)->GetParentObjects();
	
	if (!vecEnemy.empty())
	{
		sort(vecEnemy.begin(), vecEnemy.end(), [&](CGameObject* lhs, CGameObject* rhs)
			{
				Vec3 vRhs = rhs->Transform()->GetRelativePos();
				Vec3 vLhs = lhs->Transform()->GetRelativePos();

				if (m_vSource.Distance(m_vSource, vRhs) > m_vSource.Distance(m_vSource, vLhs))
					return true;
				else
					return false;
			});

		Vec3 vEnemyPos = vecEnemy[0]->Transform()->GetRelativePos();
		Vec3 vScale1 = vecEnemy[0]->Transform()->GetRelativeScale() * 0.5f;
		Vec3 vScale2 = Transform()->GetRelativeScale() * 0.5f;

		if (m_vSource.Distance(m_vSource, vEnemyPos) > vScale1.Distance(vScale1, vScale2) + 50.f)
		{
			tTile tTile = CJpsMgr::GetInst()->GetTileObj()->TileMap()->GetInfo(vEnemyPos);
			Int32 x = tTile.iIndex % TILEX;
			Int32 z = tTile.iIndex / TILEX;

			m_eState = UNIT_STATE::RUN;
			SetDestPos(vEnemyPos);

			JpsAlgorithm(x, z);
		}
	}
}

void CInfectedGiantScript::Move(Int32 _x, Int32 _z)
{
	m_pTargetObject = nullptr;
	m_bAttack = false;
	JpsAlgorithm(_x, _z);
}

void CInfectedGiantScript::SetPlayerHP()
{
	if (m_pTargetObject)
	{
		wstring wstrName = m_pTargetObject->GetName();

		float fHp = m_pTargetObject->GetScripts()[0]->GetHp();
		fHp -= m_iAttack;
		m_pTargetObject->GetScripts()[0]->SetHp(fHp);
	}
}

void CInfectedGiantScript::sound()
{
	static float fDT = DT;
	fDT += DT;
	if (fDT > 30.f)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\zombie_man_normal.wav");
		pSound->Play(1, 0.6f, false);
		fDT = 0.f;
	}
}