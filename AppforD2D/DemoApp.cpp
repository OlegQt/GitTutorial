#include <d2d1.h>
#include "DemoApp.h"

// Комментарий для компоновщика
#pragma comment(lib,"d2d1.lib")

Engine::Engine(HINSTANCE h)
{
	hWnd = NULL;
	this->hInst = h;
	this->m_pDirect2dFactory = NULL;
	this->pBrush = NULL;
	this->pRenderTarget = NULL;
}
Engine::~Engine()
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

HRESULT Engine::Initialize()
{
	HRESULT hr=S_OK;

	// Initialize device-indpendent resources, such as the Direct2D factory
	hr=this->CreateDeviceIndependentResources();
	if (!SUCCEEDED(hr))return hr;
	else {

		// Register the window class.
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = Engine::WndProc;
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
void Engine::RunMessageLoop()
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
LRESULT Engine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	static Engine* This;

	if (message == WM_CREATE) {

		// Attach additional data
		This = (Engine*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		::SetWindowLong(hWnd, GWLP_USERDATA, (LONG_PTR)This);
	}
	else {
		// Using additional data
		This = (Engine*) ::GetWindowLong(hWnd, GWLP_USERDATA);
	}

	if (message == WM_DESTROY)
	{
		PostQuitMessage(NULL);
		return 0;
	}

	return (DefWindowProc(hWnd, message, wParam, lParam));
}
HRESULT Engine::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	return hr;
}
HRESULT Engine::CreateTarget()
{
	if (this->pRenderTarget) return S_FALSE; // Âûõîäèì, åñëè ñöåíà óæå ñîçäàíà

	// Ïîëó÷àåì ðàçìåðû îêíà
	RECT rc;
	GetClientRect(this->hWnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left,rc.bottom - rc.top);

	// Create a Direct2D render target
	return m_pDirect2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(this->hWnd, size),
		&this->pRenderTarget
	);

}
HRESULT Engine::Render()
{
	HRESULT hr = this->CreateTarget();
	if (FAILED(hr)) return hr;
	this->pRenderTarget->BeginDraw();
	this->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	this->pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Gray));
	this->pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	if (!pBrush) hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &this->pBrush);
	//this->pBrush->SetColor(D2D1::ColorF::Black);
	if (FAILED(hr)) return hr;
	this->pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(20.0f, 20.0f),20.0f,20.0f), this->pBrush, 1.0f);
	this->pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	this->pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(60.0f, 20.0f), 20.0f, 20.0f), this->pBrush, 1.0f);


	hr = this->pRenderTarget->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
	return S_OK;
}
void Engine::DiscardDeviceResources()
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
