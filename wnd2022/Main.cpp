#include <windows.h>
#include <tchar.h>


// Глобальные переменные:
HINSTANCE hInst;                    // текущий экземпляр
WCHAR szTitle[]=_T("wnd");                  // Текст строки заголовка
WCHAR szWindowClass[]=_T("222");            // имя класса главного окна
HWND hWnd;

// Объявление функций
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);

//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine, int iCmdShow)
{
    //wcsncpy(szWindowClass, _T("win32app"),3);

    
    // Регистрация нового оконного класса
    if (!MyRegisterClass(hInstance))
    {
        MessageBox(NULL, _T("Не могу зарегить класс"),_T("Ошибка"), MB_OK | MB_ICONSTOP);
        return -1;
    }
    if (!InitInstance(hInstance, iCmdShow))
    {
        return -1;
    }
    
    MSG msg;
    // Цикл обработки сообщений
    while (GetMessage(&msg, 0, 0, 0))
    {
        TranslateMessage(&msg); // Сообщения от клавиатуры
        DispatchMessage(&msg); // Запуск оконной процедуры
    }

    return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    HDC hDC;
    PAINTSTRUCT ps;

    switch (msg)
    {

    case WM_PAINT:
    {
        hDC = BeginPaint(hWnd, &ps);

        // Что-то рисуется

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDI_APPLICATION);
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;

    return RegisterClassExW(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_BORDER| WS_CAPTION | WS_SYSMENU| WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT, 200, 200, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}