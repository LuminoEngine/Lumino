
#pragma once
#include <d3d9.h>
#include "Common.h"

class DXDevice
{
public:

	static const int WINDOW_WIDTH = 640;
	static const int WINDOW_HEIGHT = 480;

	HWND						mWindowHandle;
	IDirect3D9*		            mDirect3D;
	IDirect3DDevice9*	        mDxDevice;

public:

	bool initlaize()
	{
		mWindowHandle = NULL;
		mDirect3D = NULL;
		mDxDevice = NULL;

		HRESULT hr;
		mDirect3D = ::Direct3DCreate9( D3D_SDK_VERSION );

		D3DPRESENT_PARAMETERS d3dpp = {
			WINDOW_WIDTH,WINDOW_HEIGHT,D3DFMT_UNKNOWN,0,D3DMULTISAMPLE_NONE,0,
			D3DSWAPEFFECT_DISCARD,NULL,TRUE,TRUE,D3DFMT_D24S8,0,0}; 

	   if( FAILED( mDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mWindowHandle, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &mDxDevice ) ) )
	   if( FAILED( mDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mWindowHandle, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &mDxDevice ) ) )
	   if( FAILED( mDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, mWindowHandle, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &mDxDevice ) ) )
	   if( FAILED( mDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, mWindowHandle, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &mDxDevice ) ) )
	   {
		  return false;
	   }


		return true;
	}

	void finalize()
	{
		LN_SAFE_RELEASE( mDxDevice );
		LN_SAFE_RELEASE( mDirect3D );
	}

	IDirect3DDevice9* getDxDevice() { return mDxDevice; }
};