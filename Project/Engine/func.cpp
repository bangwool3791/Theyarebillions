#include "pch.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CEventMgr.h"
#include "CRenderMgr.h"
#include "CKeyMgr.h"
#include "CDevice.h"

wstring GetRelativePath(const wstring& _strBase, const wstring& _strPath)
{
	wstring strRelativePath;
	if (-1 == _strPath.find(_strBase))
	{
		return strRelativePath;
	}

	strRelativePath = _strPath.substr(_strBase.length(), _strPath.length());
	return strRelativePath;
}

void Instantiate(CGameObject* _pNewObj, int _iLayerIdx)
{
	tEvent evn{};

	evn.eType = EVENT_TYPE::CREATE_OBJECT;
	evn.wParam = (DWORD_PTR)_pNewObj;
	evn.lParam = (DWORD_PTR)_iLayerIdx;

	CEventMgr::GetInst()->AddEvent(evn);
}

void Instantiate(CGameObject* _pNewObj, Vec3 _vWorldPos, int _iLayerIdx)
{
	_pNewObj->Transform()->SetRelativePos(_vWorldPos);

	tEvent evn{};

	evn.eType = EVENT_TYPE::CREATE_OBJECT;
	evn.wParam = (DWORD_PTR)_pNewObj;
	evn.lParam = (DWORD_PTR)_iLayerIdx;

	CEventMgr::GetInst()->AddEvent(evn);
}

void Instantiate(CGameObject* _pNewObj, CGameObject* _pOwner, int _iLayerIdx)
{
	tEvent evn{};

	evn.eType = EVENT_TYPE::ADD_CHILD;
	evn.wParam = (DWORD_PTR)_pNewObj;
	evn.lParam = (DWORD_PTR)_pOwner;

	CEventMgr::GetInst()->AddEvent(evn);
}

void DebugDrawRect(Vec4 _vColor, Vec3 _vPosition, Vec3 _vScale, Vec3 _vRotation, float _fDuration)
{
	CRenderMgr::GetInst()->DebugDraw(DEBUG_SHAPE::RECT, _vColor, _vPosition, _vScale, _vRotation, 0.f, _fDuration);
}

void DebugDrawCircle(Vec4 _vColor, Vec3 _vPosition, float _fRadius, float _fDuration)
{
	CRenderMgr::GetInst()->DebugDraw(DEBUG_SHAPE::CIRCLE, _vColor, _vPosition, Vec3::Zero, Vec3::Zero, _fRadius, _fDuration);
}

const char* ToString(COMPONENT_TYPE _type)
{
	return COMPONENT_TYPE_CHAR[(UINT)_type];
}

const wchar_t* ToWString(COMPONENT_TYPE _type)
{
	return COMPONENT_TYPE_WCHAR[(UINT)_type];
}

const char* ToString(OBJECT_TYPE _type)
{
	return OBJECT_TYPE_CHAR[(UINT)_type];
}

const wchar_t* ToWString(OBJECT_TYPE _type)
{
	return OBJECT_TYPE_WCHAR[(UINT)_type];
}

const char* ToString(RES_TYPE _type)
{
	return RES_TYPE_CHAR[(UINT)_type];
}

const wchar_t* ToWString(RES_TYPE _type)
{
	return RES_TYPE_WCHAR[(UINT)_type];
}

void SaveStringToFile(const string& _str, FILE* _pFile)
{
	size_t iLen = _str.length();
	fwrite(&iLen, sizeof(size_t), 1, _pFile);
	fwrite(_str.c_str(), sizeof(char), iLen, _pFile);
}

void LoadStringFromFile(string& _str, FILE* _pFile)
{
	size_t iLen = _str.length();
	fread(&iLen, sizeof(size_t), 1, _pFile);
	char szbuffer[256] = {};
	fread(szbuffer, sizeof(char), iLen, _pFile);
	_str = szbuffer;
}

void SaveWStringToFile(const wstring& _str, FILE* _pFile)
{
	size_t iLen = _str.length();
	fwrite(&iLen, sizeof(size_t), 1, _pFile);
	fwrite(_str.c_str(), sizeof(wchar_t), iLen, _pFile);
}

void LoadWStringFromFile(wstring& _str, FILE* _pFile)
{
	size_t iLen = _str.length();
	fread(&iLen, sizeof(size_t), 1, _pFile);
	wchar_t szbuffer[256] = {};
	fread(szbuffer, sizeof(wchar_t), iLen, _pFile);
	_str = szbuffer;
}

string WStringToString(const wstring& _str)
{
	return string(_str.begin(), _str.end());
}

wstring StringToWString(const string& _str)
{
	return wstring(_str.begin(), _str.end());
}

const Ray& GetRay()
{
	Vec2 p = CKeyMgr::GetInst()->GetMousePos();
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();

	p.x = (2.0f * p.x) / vResolution.x - 1.0f;
	p.y = 1.0f - (2.0f * p.y) / vResolution.y;

	XMVECTOR det; //Determinant, needed for matrix inverse function call
	Vector3 origin = Vector3(p.x, p.y, 0);
	Vector3 faraway = Vector3(p.x, p.y, 1);

	XMMATRIX invViewProj = XMMatrixInverse(&det, g_transform.matView * g_transform.matProj);
	Vector3 rayorigin = XMVector3Transform(origin, invViewProj);
	Vector3 rayend = XMVector3Transform(faraway, invViewProj);
	Vector3 raydirection = rayend - rayorigin;
	raydirection.Normalize();
	static Ray ray;
	ray.position = rayorigin;
	ray.direction = raydirection;
	return ray;
}

int g_iColony;
int g_iWorker;
int g_iFood;
int g_iGold;
int g_iWood;
int g_iIron;

int g_iGoldInc;
int g_iWoodInc;
int g_iIronInc;