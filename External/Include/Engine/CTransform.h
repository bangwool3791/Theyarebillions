#pragma once

#include "CComponent.h"

class CTransform
	: public CComponent
{
public:
	CTransform();
	virtual ~CTransform();

	CLONE(CTransform);
private:
	Vec3		m_vRelativePos;
	Vec3		m_vRelativeScale;
	Vec3		m_vRelativeRotation;

	Vec3		m_vRelativeDir[(UINT)DIR::END];
	Vec3		m_vWorldDir[(UINT)DIR::END];
	Matrix		m_matWorld;

	//부모 행렬의 영향을 받을지 않받을지 결정하는 bool 변수
	bool		m_blgnParentScale;
public:
	void SetRelativePos(Vec3 _vPos) { m_vRelativePos = _vPos; }
	void SetRelativeScale(Vec3 _vPos) { m_vRelativeScale = _vPos; }
	void SetRelativeRotation(Vec3 _vPos) { m_vRelativeRotation = _vPos; }

	void SetRelativePos(float _x, float _y, float _z) { m_vRelativePos = Vec3{ _x,_y,_z }; }
	void SetRelativeScale(float _x, float _y, float _z) { m_vRelativeScale = Vec3{ _x,_y,_z }; }

	void SetRelativeRotationX(float _fAngle) { m_vRelativeRotation.x = _fAngle; }
	void SetRelativeRotationY(float _fAngle) { m_vRelativeRotation.y = _fAngle; }
	void SetRelativeRotationZ(float _fAngle) { m_vRelativeRotation.z = _fAngle; }

	void SetRelativeRotation(float _x, float _y, float _z) { m_vRelativeRotation = Vec3{ _x,_y,_z }; }

	Vec3& GetRelativePos_() { return	m_vRelativePos; }
	Vec3 GetRelativePos() { return	m_vRelativePos; }
	Vec3 GetRelativeScale() { return m_vRelativeScale; }
	Vec3 GetRelativeRotation() { return m_vRelativeRotation; }
	Vec3& GetRelativeRotation_() { return m_vRelativeRotation; }
	Vec3 GetRelativeDir(DIR _eType) { return m_vRelativeDir[(UINT)_eType]; }
	Vec3& GetRelativeDir_(DIR _eType) { return m_vRelativeDir[(UINT)_eType]; }

	Vec3 GetWorldPos() { return m_matWorld.Translation(); }
	Vec3 GetWorldDir(DIR _eType) { return m_vWorldDir[(UINT)_eType]; }
	Vec3 GetWorldScale();

	void SetIgnoreParentScale(bool _blgn) { m_blgnParentScale = _blgn; }
	bool IsIgnoreParentScale() { return m_blgnParentScale; }
	const Matrix& GetWorldMat() { return m_matWorld; }
public:
	void tick();
	void finaltick();
	void UpdateData();

public:
	virtual void SaveToFile(FILE* _File);
	virtual void LoadFromFile(FILE* _File);
};