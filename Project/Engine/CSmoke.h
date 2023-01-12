#pragma once
#include "CParticleSystem.h"

class CStructuredBuffer;

/*
* 랜더 컴포넌트(재질, 메쉬)
* 재질->랜더용 셰이더
* 컴퓨트 셰이더
*/
class CSmoke :
	public CParticleSystem
{
public:
	virtual void finaltick()override;
	virtual void render()override;

public:
	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;
	CLONE(CSmoke);

public:
	CSmoke();
	CSmoke(COMPONENT_TYPE _type);
	CSmoke(const CSmoke& _Rhs);
	virtual ~CSmoke();
};