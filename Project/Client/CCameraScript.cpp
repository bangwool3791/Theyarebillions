#include "pch.h"
#include "CCameraScript.h"


CCameraScript::CCameraScript()
	:CScript(-1)
	, m_fSpeed(100.f)
{
}

CCameraScript::~CCameraScript()
{
}

void CCameraScript::begin()
{
}

void CCameraScript::tick()
{
	Move();
}

void CCameraScript::Move()
{
	Vec3 vPos = Transform()->GetRelativePos();

	float fSpeed = m_fSpeed;

	if (KEY_PRESSED(KEY::LSHIFT))
	{
		fSpeed *= 5.f;
	}

	if (Camera()->GetProjType() == PROJ_TYPE::PERSPECTIVE)
	{
		Vec3 vFront = Transform()->GetRelativeDir(DIR::FRONT);
		Vec3 vRight = Transform()->GetRelativeDir(DIR::RIGHT);

		if (KEY_PRESSED(KEY::W))
			vPos.z += DT * fSpeed;
		if (KEY_PRESSED(KEY::S))
			vPos.z += DT * -fSpeed;
		if (KEY_PRESSED(KEY::A))
			vPos += DT * -vRight * fSpeed;
		if (KEY_PRESSED(KEY::D))
			vPos += DT * vRight * fSpeed;

		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();


		if (KEY_PRESSED(KEY::RBTN))
		{
			Vec3 vRot = Transform()->GetRelativeRotation();

			vRot.y += vMouseDir.x * DT * XM_PI;
			vRot.x -= vMouseDir.y * DT * XM_PI;
			Transform()->SetRelativeRotation(vRot);
		}
		Transform()->SetRelativePos(vPos);

		if (KEY_PRESSED(KEY::SPACE))
		{
			Camera()->SetProjType(PROJ_TYPE::ORTHOGRAHPICS);
		}
	}

	else
	{
		Vec3 vRot = Transform()->GetRelativeRotation();
		if (vRot.x != 0.f)
			vRot.x = 0.f;
		if (vRot.y != 0.f)
			vRot.y = 0.f;

		if (KEY_PRESSED(KEY::W))
			vPos.y += DT * fSpeed;
		if (KEY_PRESSED(KEY::S))
			vPos.y -= DT * fSpeed;
		if (KEY_PRESSED(KEY::A))
			vPos.x -= DT * fSpeed;
		if (KEY_PRESSED(KEY::D))
			vPos.x += DT * fSpeed;

		if (vPos != Transform()->GetRelativePos())
			Transform()->SetRelativePos(vPos);

		if (KEY_PRESSED(KEY::NUM_1))
		{
			float fScale = Camera()->GetOrthographicScale();
			fScale += DT;
			Camera()->SetOrthographicScale(fScale);
		}
		else if (KEY_PRESSED(KEY::NUM_2))
		{
			float fScale = Camera()->GetOrthographicScale();
			fScale -= DT;
			Camera()->SetOrthographicScale(fScale);
		}

		if (KEY_PRESSED(KEY::SPACE))
		{
			Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		}
	}
}
