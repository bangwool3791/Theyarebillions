#pragma once
#include <Engine\CScript.h>

class CCamera;

class CAssistScript :
	public CScript
{
private:
	CCamera* m_pMainCam;

	Vec4    m_vColor;
	float	m_fThickness;
	float	m_fGridInterval;

public:
	virtual void begin() override;
	virtual void tick() override;
	virtual void BeginOverlap(CCollider2D* _pOther) {};
	virtual void Overlap(CCollider2D* _pOther) {};
	virtual void EndOverlap(CCollider2D* _pOther) {};

public:
	void SetMainCam(CCamera* _pCam) { m_pMainCam = _pCam; }
	void SetGridColor(Vec4 _vColor) { m_vColor = _vColor; }
	void SetThickness(float _fThickness) { m_fThickness = _fThickness; }
	void SetGridInterval(float _fGridInterval) { m_fGridInterval = _fGridInterval; }

	CLONE(CAssistScript);
public:
	CAssistScript();
	virtual ~CAssistScript();
};
