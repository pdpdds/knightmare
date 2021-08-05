#include "GameMain.h"
#include "CGameCore.h"

HINSTANCE g_hInst;
HWND g_hWnd;

#define SPRITE_DIAMETER 30
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	g_hInst = hInstance;
	static char Name[] = "Knightmare";
	MSG         msg;
	WNDCLASS  wndclass;
	wndclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = CGameCore::WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = (HINSTANCE)GetWindowLong(NULL, GWL_HINSTANCE);
	wndclass.hIcon = LoadIcon(NULL, "YUKINO");
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = Name;

	RegisterClass(&wndclass);

	g_hWnd = CreateWindow(Name,
		"Knightmare", WS_POPUP | WS_VISIBLE,
		200,
		60, SCREEN_WIDTH,
		SCREEN_HEIGHT,

		NULL,
		NULL,
		(HINSTANCE)GetWindowLong(NULL, GWL_HINSTANCE),
		NULL);

	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);

	CGameCore::GetInstance()->Initialize(hInstance, g_hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);
	SetTimer(g_hWnd, 1, 980, NULL);

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))

		{
			if (!GetMessage(&msg, NULL, 0, 0))
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else if (CGameCore::GetInstance()->GetPause())
		{
			WaitMessage();
		}
		else
		{
			CGameCore::GetInstance()->ProcessGame();
		}

	}

	return 0;
}










