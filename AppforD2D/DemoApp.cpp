#include <d2d1.h>
#include "DemoApp.h"

//Включение библиотеки для компоновщика
#pragma comment(lib,"d2d1.lib")

MyClass::MyClass(HINSTANCE h)
{
	hWnd = NULL;
	this->hInst = h;
	this->m_pDirect2dFactory = NULL;
	this->pBrush = NULL;
	this->pRenderTarget = NULL;
}
MyClass::~MyClass()
{
	if (this->m_pDirect2dFactory)
	{
		this->m_pDirect2dFactory->Release();
		this->m_pDirect2dFactory = NULL;
	}
	if (this->pBrush)
	{
		this->pBrush->Release();
		this->pBrush = NULL;
	}
	if (this->pRenderTarget)
	{
		this->pRenderTarget->Release();
		this->pRenderTarget = NULL;
	}
}

HRESULT MyClass::Initialize()
{
	HRESULT hr=S_OK;

	// Initialize device-indpendent resources, such as the Direct2D factory
	hr=this->CreateDeviceIndependentResources();
	if (!SUCCEEDED(hr))return hr;
	else {

		// Register the window class.
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = MyClass::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = this->hInst;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = L"D2DDemoApp";

		if (!RegisterClassEx(&wcex)) return S_FALSE;
	}
	// Create the window.
	// Because the CreateWindow function takes its size in pixels,
	// obtain the system DPI and use it to scale the window size.
	FLOAT dpiX, dpiY;
	// The factory returns the current system DPI. 
	// This is also the value it will use to create its own windows.
	//m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);
	this->hWnd = CreateWindow(L"D2DDemoApp",L"app D2D",WS_OVERLAPPED|WS_SYSMENU,0,0,100,100,NULL,NULL,this->hInst,this);
	if (!this->hWnd) return S_FALSE;
	if (SUCCEEDED(hr))
	{
		ShowWindow(this->hWnd, SW_SHOWNORMAL);
		UpdateWindow(this->hWnd);
		this->RunMessageLoop();
	}
	return hr;
}
void MyClass::RunMessageLoop()
{
    MSG msg;
	msg.message = WM_NULL; //initialize variable msg
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}
		else
		{
			this->Render();
		}
	}

}
LRESULT MyClass::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	static MyClass* This;

	if (message == WM_CREATE) {

		// Attach additional data
		This = (MyClass*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		::SetWindowLong(hWnd, GWLP_USERDATA, (LONG_PTR)This);
	}
	else {
		// Using additional data
		This = (MyClass*) ::GetWindowLong(hWnd, GWLP_USERDATA);
	}

	if (message == WM_DESTROY)
	{
		PostQuitMessage(NULL);
		return 0;
	}

	return (DefWindowProc(hWnd, message, wParam, lParam));
}
HRESULT MyClass::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	return hr;
}
HRESULT MyClass::CreateTarget()
{
	if (this->pRenderTarget) return S_FALSE; // Выходим, если сцена уже создана

	// Получаем размеры окна
	RECT rc;
	GetClientRect(this->hWnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left,rc.bottom - rc.top);

	// Create a Direct2D render target.
	return m_pDirect2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(this->hWnd, size),
		&this->pRenderTarget
	);
}
HRESULT MyClass::Render()
{
	HRESULT hr = this->CreateTarget();
	if (FAILED(hr)) return hr;
	this->pRenderTarget->BeginDraw();
	this->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	this->pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Gray));
	hr = this->pRenderTarget->EndDraw();
	
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
	return S_OK;
}
void MyClass::DiscardDeviceResources()
{
	if (this->m_pDirect2dFactory)
	{
		this->m_pDirect2dFactory->Release();
		this->m_pDirect2dFactory = NULL;
	}
	if (this->pBrush)
	{
		this->pBrush->Release();
		this->pBrush = NULL;
	}
	if (this->pRenderTarget)
	{
		this->pRenderTarget->Release();
		this->pRenderTarget = NULL;
	}
}
