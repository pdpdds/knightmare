#pragma once
#include "GameMain.h"

#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }
#define KEYDOWN(State,KeyID)	(State[KeyID] & 0x80) 

class DirectDrawMethod
{
public:
	DirectDrawMethod(void);
	virtual ~DirectDrawMethod(void);
	
	BOOL Initialize(HWND hWnd, int xres, int yres);

	HRESULT	Copy_Bitmap (LPDIRECTDRAWSURFACE7 pDDS, 
		                 HBITMAP hBitmap, 
						 INT iX=0, 
						 INT iY=0, 
						 INT iDeltaX = 0, 
						 INT iDeltaY = 0);

	int DirectDrawMethod::Blit_Surface(	LPDIRECTDRAWSURFACE7 dest,    // surface to draw the surface on
				 int x, int y,                 // position to draw at
				 int width, int height,					  
				 LPDIRECTDRAWSURFACE7 source, // source surface to draw 
				 int src_x, int src_y,
				 int src_width, int src_height,
				 int transparent);
	void FillSurface (LPDIRECTDRAWSURFACE7 lpSurface, 
		              DWORD dwColor);

	LPDIRECTDRAWSURFACE7 Create_Surface (INT iWidth, 
		                                INT iHeight, 
										INT iMemFlags = DDSCAPS_SYSTEMMEMORY, 
										DWORD dwColorKey = 0);

	LPDIRECTDRAWSURFACE7	Load_Bitmap (LPCSTR szBitmap_name, 
									 COLORREF dwColorKey, 
									 INT iDx = 0, 
									 INT iDy = 0);

	HRESULT Set_ColorKey(LPDIRECTDRAWSURFACE7 pdds, COLORREF rgb);

	HRESULT  UninitDirectDraw( void );

	HRESULT  InitDirectDraw (HWND	hMain, 
		                     INT iXRes, 
							 INT iYRes, 
							 INT iColordepth );


	LPDIRECTDRAWSURFACE7 DirectDrawMethodCreate_Surface(int width, int height, int mem_flags, DWORD color_key);

	static DirectDrawMethod* GetInstance()
	{
		if (NULL == m_pDirectDrawManager)
		{
			m_pDirectDrawManager = new DirectDrawMethod();
		}
		return m_pDirectDrawManager;
	}

	LPDIRECTDRAWSURFACE7 GetBackBuffer()
	{
		return g_pDisplay->GetBackBuffer();
	}
	BOOL Render ();

protected:

private:

	DWORD	Pitch;
	DDSURFACEDESC2        m_ddsd;  
		
	HWND m_hWnd;
	static DirectDrawMethod* m_pDirectDrawManager;

	CDisplay*            g_pDisplay;

	HBRUSH hbrush1,hbrush2;
	HPEN hpen1,hpen2;

	HFONT hfont;
};
