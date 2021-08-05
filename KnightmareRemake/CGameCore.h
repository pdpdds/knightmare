#pragma once
#include <windows.h>

class CGameCore
{	
public:	
	CGameCore(void);
	virtual ~CGameCore(void);

	BOOL Initialize(HINSTANCE hInstance, HWND hWnd, int xres, int yres);

	static CGameCore* GetInstance()
	{
		if (NULL == m_pGameCore)
		{
			m_pGameCore = new CGameCore();
		}
		return m_pGameCore;
	}

	static LRESULT CALLBACK WndProc (HWND opening, UINT iMsg, WPARAM wParam, LPARAM lParam);

	BOOL InitStage();
	void UpdateGameStatus();
	void ProcessInput (HWND hwnd);
	BOOL ProcessGame();
	void Render();
	BOOL ProcessDirective(INT iX, INT iY);

	BOOL GetPause()
	{
		return m_bPaused;
	}
	
protected:

private:
	static CGameCore* m_pGameCore;
	static BOOL m_bPaused;
	static HWND m_hWnd;

	static BOOL m_bLevelReset;
	static INT iStageNum;	
};
