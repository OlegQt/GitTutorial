#include <windows.h>
#include "DemoApp.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ INT nCmdShow)
{
	MyClass* ptrClass = new MyClass(hInstance);
	if (FAILED(ptrClass->Initialize())) return 0;
	

	
	return 0;
}