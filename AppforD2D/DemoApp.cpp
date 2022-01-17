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

	this->pLogig = new CLogic;
	this->btnA = {10,10,20,20};
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
	if (this->pLogig)
	{
		this->pLogig->~CLogic();
		this->pLogig = NULL;
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
		wcex.lpfnWndProc = (WNDPROC)this->WndProc;
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
	//FLOAT dpiX, dpiY;
	// The factory returns the current system DPI. 
	// This is also the value it will use to create its own windows.
	//m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);
	this->hWnd = CreateWindow(L"D2DDemoApp",L"app D2D",WS_OVERLAPPED|WS_SYSMENU,0,0,500,500,NULL,NULL,this->hInst,this);
	if (!this->hWnd) return S_FALSE;
	CreateWindow(L"Button", L"but", WS_CHILD | WS_VISIBLE|BS_OWNERDRAW, 
		this->btnA.l, this->btnA.t, this->btnA.w, this->btnA.h,
		this->hWnd, (HMENU)ID_BUTTON_A, this->hInst, NULL);
	
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
	Engine * pThis;
	if (message == WM_NCCREATE)
	{
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<Engine*>(lpcs->lpCreateParams);
		// Put the value in a safe place for future use
		SetWindowLongPtr(hWnd, GWLP_USERDATA,	reinterpret_cast<LONG_PTR>(pThis));
	}
	else
	{
		// Recover the "this" pointer from where our WM_NCCREATE handler stashed it.
		pThis = reinterpret_cast<Engine*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}
	if (pThis) pThis->Procedure(hWnd, message, wParam, lParam);
	
	return (DefWindowProc(hWnd, message, wParam, lParam));
}
LRESULT Engine::Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_DESTROY)
	{
		PostQuitMessage(NULL);
		return 0;
	}
	if (message == WM_LBUTTONDOWN)
	{
		// Достаем координаты щелчка
		float Xpos, Ypos;
		Xpos = static_cast<float>LOWORD(lParam);
		Ypos = static_cast<float>HIWORD(lParam);
		this->pLogig->AddElement(Xpos, Ypos, 5.0f);
	}
	if (message == WM_COMMAND)
	{
		if (wParam == ID_BUTTON_A)
		{
			this->btnA.pushed = !this->btnA.pushed;
		}
	}

	return S_OK;
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
	HRESULT hr = m_pDirect2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(this->hWnd, size),
		&this->pRenderTarget
	);
	if(SUCCEEDED(hr)) this->pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	if (SUCCEEDED(hr)&& !pBrush) hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &this->pBrush);
	return hr;

}
HRESULT Engine::Render()
{
	HRESULT hr = this->CreateTarget();
	if (FAILED(hr)) return hr;
	this->pRenderTarget->BeginDraw();
	this->pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	this->pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Gray));
	// Draw here
	int inc = 0;
	while (inc<pLogig->GetArraySize())
	{
		CBall* pBall = pLogig->GetElement(inc);
		this->pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(pBall->xPos, pBall->yPos),pBall->Diameter, pBall->Diameter), this->pBrush, 1.0f);
		inc++;
	}

	
	//Ниже рендер интерфейса	
	D2D1_RECT_F btnRect = { this->btnA.l,this->btnA.t,this->btnA.l + this->btnA.w,this->btnA.t + this->btnA.h };
	if (this->btnA.pushed) {this->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::AliceBlue));}
	else { this->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Gray));}
	this->pRenderTarget->FillRectangle(btnRect, this->pBrush);
	this->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	this->pRenderTarget->DrawRectangle(btnRect, this->pBrush, 2.0f, NULL);

	// up to this end
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
