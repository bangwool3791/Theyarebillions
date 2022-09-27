#include "pch.h"
#include "CEngine.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CLevelMgr.h"
#include "CDevice.h"

#include "test.h"

CEngine::CEngine()
	: m_hMainWnd(nullptr)
{

}

CEngine::~CEngine()
{
	::release();
}

int CEngine::init(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
	m_hMainWnd = _hWnd;
	m_vResolution.x = (float)_iWidth;
	m_vResolution.y = (float)_iHeight;

	// ��Ʈ�� �ػ󵵸� �����ϱ� ���� ���� ������ ũ�� ���
	RECT rt = { 0, 0, (LONG)_iWidth , (LONG)_iHeight };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

	// ������ ũ�� ���� �� Show ����
	SetWindowPos(m_hMainWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hMainWnd, true);

	// Manager �ʱ�ȭ
	if (FAILED(CDevice::GetInst()->init(m_hMainWnd, m_vResolution)))
	{
		return E_FAIL;
	}

	CPathMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CLevelMgr::GetInst()->init();


	::init();


	return S_OK;
}

void CEngine::progress()
{
	tick();

	render();
}

void CEngine::tick()
{
	// Manager ������Ʈ
	CTimeMgr::GetInst()->tick();
	CKeyMgr::GetInst()->tick();

	//CLevelMgr::GetInst()->tick();

	::tick();
}

void CEngine::render()
{
	CDevice::GetInst()->TargetClear();

	CTimeMgr::GetInst()->render();

	//CLevelMgr::GetInst()->render();

	::render();

	CDevice::GetInst()->Present();
}