#pragma once

class CEngine
	: public CSingleton<CEngine>
{

private:
	HWND		m_hMainWnd;
	Vec2		m_vResolution;	// 윈도우 해상도


public:
	int init(HWND _hWnd, UINT _iWidth, UINT _iHeight);
	void progress();

private:
	void tick();
	void finaltick();
	void render();

public:
	CEngine();
	virtual ~CEngine();
public:
	HWND GetMainHwnd() { return m_hMainWnd; }
};

