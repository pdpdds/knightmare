#include "CDirectDrawMethod.h"
#include <windows.h>

DirectDrawMethod* DirectDrawMethod::m_pDirectDrawManager = NULL;

DirectDrawMethod::DirectDrawMethod(void)
{
	hfont = NULL;	
}

DirectDrawMethod::~DirectDrawMethod(void)
{
		
}

HRESULT  DirectDrawMethod::UninitDirectDraw( void )
{
	DeleteObject(hbrush1); 	
	DeleteObject(hbrush2); 
	DeleteObject(hpen1); 
	DeleteObject(hpen2); 

//////////////////////////////////////////////////////////////////////////////////
//Release Resource
//////////////////////////////////////////////////////////////////////////////////
	/*if( lpDD2 != NULL )
	{
		if( lpPrimarySurface != NULL )		
		{
			if( lpPrimarySurface->Release() == DD_OK )
			{
				lpPrimarySurface = NULL;
			}
		}



		if ( lpDD2->Release() == DD_OK )
			lpDD2 = NULL;
	}*/

	return TRUE;
}

HRESULT  DirectDrawMethod::InitDirectDraw( HWND	MainhWnd, int xres, int yres, int colordepth )
{

	g_pDisplay = new CDisplay();
	if( FAILED( g_pDisplay->CreateWindowedDisplay(MainhWnd ,xres ,yres  ) ) )
	{
		MessageBox( MainhWnd, TEXT("Failed initializing DirectDraw."),
			TEXT("DirectDraw Sample"), MB_ICONERROR | MB_OK );
		return 0;
	}

	return DD_OK;
}


void DirectDrawMethod::FillSurface(LPDIRECTDRAWSURFACE7 lpSurface, DWORD dwColor)
{
	DDBLTFX ddbltfx;

	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = dwColor;

	lpSurface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
}

int DirectDrawMethod::Blit_Surface(	LPDIRECTDRAWSURFACE7 dest,    // surface to draw the surface on
				 int x, int y,                 // position to draw at
				 int width, int height,					  
				 LPDIRECTDRAWSURFACE7 source, // source surface to draw 
				 int src_x, int src_y,
				 int src_width, int src_height,
				 int transparent)          // transparency flag
{
	// draw a bob at the x,y defined in the BOB
	// on the destination surface defined in dest

	RECT dest_rect;   // the destination rectangle
	RECT source_rect;

	// fill in the destination rect
	dest_rect.left   = x;
	dest_rect.top    = y;
	dest_rect.right  = x+width;
	dest_rect.bottom = y+height;

	source_rect.left = src_x;
	source_rect.top = src_y;
	source_rect.right = src_x + src_width;
	source_rect.bottom = src_y + src_height;

	// test transparency flag

	if (transparent)
	{
		// enable color key blit
		// blt to destination surface
		if (FAILED(dest->Blt(&dest_rect, source,
			&source_rect,(DDBLT_WAIT | DDBLT_KEYSRC),
			NULL)))
			return(0);

	} // end if
	else
	{
		// perform blit without color key
		// blt to destination surface
		if (FAILED(dest->Blt(&dest_rect, source,
			&source_rect,(DDBLT_WAIT),
			NULL)))
			return(0);

	} // end if

	// return success
	return(1);

} // end Blit_Surface2

LPDIRECTDRAWSURFACE7 DirectDrawMethod::Load_Bitmap(LPCSTR szBitmap_name, COLORREF color_key , int dx, int dy)
{
	HBITMAP                 hbm;
	BITMAP                  bm;    
	LPDIRECTDRAWSURFACE7 pdds;


	//
	//  Try to load the bitmap as a resource, if that fails, try it as a file
	//
	hbm = (HBITMAP) LoadImage(GetModuleHandle(NULL), szBitmap_name, IMAGE_BITMAP, dx,
		dy, LR_CREATEDIBSECTION);
	if (hbm == NULL)
		hbm = (HBITMAP) LoadImage(NULL, szBitmap_name, IMAGE_BITMAP, dx, dy,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hbm == NULL)
		return NULL;
	//
	// Get size of the bitmap
	//
	GetObject(hbm, sizeof(bm), &bm);
	//
	// Create a DirectDrawSurface for this bitmap
	//

	DDRAW_INIT_STRUCT(m_ddsd);
	m_ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	m_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	m_ddsd.dwWidth = bm.bmWidth;
	m_ddsd.dwHeight = bm.bmHeight;
	if (g_pDisplay->GetDirectDraw()->CreateSurface(&m_ddsd, &pdds, NULL) != DD_OK)
		return NULL;


	// copy bitmap on the surface
	Copy_Bitmap(pdds, hbm);
	DeleteObject(hbm);

	return pdds;
}

HRESULT	DirectDrawMethod::Copy_Bitmap (LPDIRECTDRAWSURFACE7 pDDS, 
		                 HBITMAP hBitmap, 
						 INT iX, 
						 INT iY, 
						 INT iDeltaX,
						 INT iDeltaY)
{
	HDC                     hdcImage;
	HDC                     hdc;
	BITMAP                  bm;    
	HRESULT                 hr;

	if (hBitmap == NULL || pDDS == NULL)
		return E_FAIL;
	//
	// Make sure this surface is restored.
	//
	pDDS->Restore();
	//
	// Select bitmap into a memoryDC so we can use it.
	//
	hdcImage = CreateCompatibleDC(NULL);
	if (!hdcImage)
		OutputDebugString("createcompatible dc failed\n");
	SelectObject(hdcImage, hBitmap);
	//
	// Get size of the bitmap
	//
	GetObject(hBitmap, sizeof(bm), &bm);
	iDeltaX = (iDeltaX == 0 ? bm.bmWidth : iDeltaX);     // Use the passed size, unless zero
	iDeltaY = (iDeltaY == 0 ? bm.bmHeight : iDeltaY);
	//
	// Get size of surface.
	//
	m_ddsd.dwSize = sizeof(m_ddsd);
	m_ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
	pDDS->GetSurfaceDesc(&m_ddsd);

	if ((hr = pDDS->GetDC(&hdc)) == DD_OK)
	{
		StretchBlt(hdc, 0, 0, m_ddsd.dwWidth, m_ddsd.dwHeight, hdcImage, iX, iY,
			iDeltaX, iDeltaY, SRCCOPY);
		pDDS->ReleaseDC(hdc);
	}
	DeleteDC(hdcImage);
	return hr;
}


LPDIRECTDRAWSURFACE7 DirectDrawMethod::DirectDrawMethodCreate_Surface(int width, int height, int mem_flags, DWORD color_key)
{
	// this function creates an offscreen plain surface



	LPDIRECTDRAWSURFACE7 tmp_lpdds;  // temporary surface

	// set to access caps, width, and height
	DDRAW_INIT_STRUCT(m_ddsd);
	m_ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

	// set dimensions of the new bitmap surface
	m_ddsd.dwWidth  =  width;
	m_ddsd.dwHeight =  height;

	// set surface to offscreen plain
	m_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | mem_flags;

	// create the offscreen surface
	if (FAILED(g_pDisplay->GetDirectDraw()->CreateSurface(&m_ddsd,&tmp_lpdds,NULL)))
		return(NULL);

	// set color key to color 0
	DDCOLORKEY color_key1; // used to set color key
	color_key1.dwColorSpaceLowValue  = color_key;
	color_key1.dwColorSpaceHighValue = color_key;

	// now set the color key for source blitting
	tmp_lpdds->SetColorKey(DDCKEY_SRCBLT, &color_key1);

	// return create surface
	return(tmp_lpdds);
} // end Create_Surface

HRESULT DirectDrawMethod::Set_ColorKey(LPDIRECTDRAWSURFACE7 pdds, COLORREF rgb)
{
	DDCOLORKEY              ddck;

	ddck.dwColorSpaceLowValue = rgb;
	ddck.dwColorSpaceHighValue = rgb;
	return pdds->SetColorKey(DDCKEY_SRCBLT, &ddck);

}

BOOL DirectDrawMethod::Initialize(HWND hWnd, int xres, int yres)
{	
	HRESULT hr;
	if ( InitDirectDraw( hWnd, xres, yres, 8) != DD_OK )
		exit(0);


	SetCursor(NULL);

	
	return TRUE;
}

BOOL DirectDrawMethod::Render ()
{	
	/*int k;
	g_pDisplay->Clear( 0 );

	for(int g=0;g<35;g++)
		g_pDisplay->Blt( g*iSize, 0,g_Surface4, NULL );

	for(int g=0;g<35;g++)
		g_pDisplay->Blt( g*iSize, 400,g_Surface4, NULL );

	for(int g=0;g<20;g++)
		g_pDisplay->Blt( 0,g*iSize,g_Surface4, NULL );

	for(int g=0;g<20;g++)
		g_pDisplay->Blt( 380,g*iSize,g_Surface4, NULL );

	for(int g=1;g<19;g++)		
		for(k=1;k<20;k++)
			g_pDisplay->Blt( g*iSize,k*iSize,g_Surface1, NULL );

	for(int k=1;k<19;k++)
	{
		for(int g=1;g<19;g++)
		{ 
			if ( *(ppPoint1 + (g * iSize) + k) == 0 &&
				*(ppPoint2 + (g * iSize) + k) == 0)
			{
				g_pDisplay->Blt( k*iSize,g*iSize,g_Surface1, NULL );
			}
			else if(*(ppPoint1 + (g * iSize) + k) == 1)
			{
				g_pDisplay->Blt( k*iSize,g*iSize,g_Surface4, NULL );
			}
			else if(*(ppPoint1 + (g * iSize) + k) == 3)
			{
				g_pDisplay->Blt( k*iSize,g*iSize,g_Surface3, NULL );
			}
			else if(*(ppPoint2 + (g * iSize) + k) == 2)
			{
				g_pDisplay->Blt( k*iSize,g*iSize,g_Surface2, NULL );
			}
		}
	}
								
	g_pDisplay->Blt( iHeroX*iSize,iHeroY*iSize,g_Surface5, NULL );	

	g_pDisplay->Blt( 10,440,g_TextSurface, NULL );
	g_pDisplay->Blt( 10,455,g_TextSurface2, NULL );*/

	HRESULT hr;
	if( FAILED( hr = g_pDisplay->Present() ) )
		return FALSE;

	Sleep(75);
	return TRUE;
}