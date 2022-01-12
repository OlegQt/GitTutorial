#include <windows.h>

#include <d2d1.h>
#include <d2d1helper.h>

class Engine
{
public:
	Engine(HINSTANCE);
	~Engine();
	
	// Register the window class and call methods for instantiating drawing resources
	HRESULT Initialize();

	// Process and dispatch messages
	void RunMessageLoop();

	// The windows procedure.
	static LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

private:
	HINSTANCE hInst;
	HWND hWnd;

	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* pRenderTarget;
	ID2D1SolidColorBrush* pBrush;

	// Initialize device-independent resources.
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateTarget();
	HRESULT Render();
	void DiscardDeviceResources();
};