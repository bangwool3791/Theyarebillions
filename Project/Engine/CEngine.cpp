#include "pch.h"
#include "CEngine.h"

#include "smallobjallocator.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CFontMgr.h"
#include "CDevice.h"
#include "CEventMgr.h"
#include "CCollisionMgr.h"
#include "CSound.h"

#include "CAnimator2D.h"

extern std::unordered_map<std::string, ChunkGroup*>			g_map_pool;
wchar_t CEngine::g_szHp[255];
wchar_t CEngine::g_szFullName[255];
vector<pair<wstring, Vec2>> CEngine::g_IconText;
vector<tTextInfo> CEngine::g_vecUiText;
CEngine::CEngine()
	: m_hMainWnd(nullptr)
{

}

CEngine::~CEngine()
{
	for (auto iter{ g_map_pool.begin() }; iter != g_map_pool.end(); ++iter)
	{
		delete iter->second;
	}

	for (auto iter{ CAnimator2D::GetRef().begin() }; iter != CAnimator2D::GetRef().end(); ++iter)
	{
		delete iter->second;
	}
}

int CEngine::init(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
	m_hMainWnd = _hWnd;
	m_vResolution.x = (float)_iWidth;
	m_vResolution.y = (float)_iHeight;

	// 비트맵 해상도를 설정하기 위한 실제 윈도우 크기 계산
	RECT rt = { 0, 0, (LONG)_iWidth , (LONG)_iHeight };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

	// 윈도우 크기 변경 및 Show 설정
	SetWindowPos(m_hMainWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hMainWnd, true);

	// Manager 초기화
	if (FAILED(CDevice::GetInst()->init(m_hMainWnd, m_vResolution)))
	{
		return E_FAIL;
	}

	CPathMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CResMgr::GetInst()->init();
	CRenderMgr::GetInst()->init();
	CLevelMgr::GetInst()->init();
	CFontMgr::GetInst()->init();
	return S_OK;
}

void CEngine::progress()
{
	tick();
	finaltick();
	render();
}

void CEngine::tick()
{
	// Manager 업데이트
	CResMgr::GetInst()->tick();
	CSound::g_pFMOD->update();
	CTimeMgr::GetInst()->tick();
	CKeyMgr::GetInst()->tick();
	CRenderMgr::GetInst()->tick();
	CLevelMgr::GetInst()->progress();
	CCollisionMgr::GetInst()->tick();
}

void CEngine::finaltick()
{

}

void CEngine::render()
{
	CDevice::GetInst()->TargetClear();

	CRenderMgr::GetInst()->render();

	CTimeMgr::GetInst()->render();

	CFontMgr::GetInst()->DrawFont(g_szFullName, 685.f, 790.f, 17.f, FONT_RGBA(20, 255, 20, 200));
	CFontMgr::GetInst()->DrawFont(g_szHp, 685.f, 850.f, 12.f, FONT_RGBA(20, 255, 20, 200));

	for (size_t i{}; i < CEngine::g_IconText.size(); ++i)
	{
		if('+' == g_IconText[i].first[0])
			CFontMgr::GetInst()->DrawFont(g_IconText[i].first.c_str(), g_IconText[i].second.x, g_IconText[i].second.y, 17.f, FONT_RGBA(20, 255, 20, 200));
		else
			CFontMgr::GetInst()->DrawFont(g_IconText[i].first.c_str(), g_IconText[i].second.x, g_IconText[i].second.y, 17.f, FONT_RGBA(255, 20, 20, 200));
	}

	for (size_t i{}; i < CEngine::g_vecUiText.size(); ++i)
		CFontMgr::GetInst()->DrawFont(g_vecUiText[i].sz, g_vecUiText[i].vPos.x, g_vecUiText[i].vPos.y, g_vecUiText[i].fSize, 
			FONT_RGBA(g_vecUiText[i].vColor.x, g_vecUiText[i].vColor.y,
				g_vecUiText[i].vColor.z, g_vecUiText[i].vColor.w));
	/*
	* 1 Tick
	* Present
	* 백버퍼 프론트 버퍼 전환이 일어난다.
	* [클라이언트] 그리드를 그린다.
	* 2 Tick
	* Target 클리어가 일어난다.
	*/

	CEventMgr::GetInst()->tick();
}
