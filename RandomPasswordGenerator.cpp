
#include "framework.h"
#include "resource.h"

#include <CommCtrl.h>
#include <string>
#include <ctime>

#pragma comment(lib, "ComCtl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define IDM_PASSWORD_TYPE       120
#define IDM_PASSWORD_LENGTH     530
#define IDM_PASSWORD_BOX        940

#define IDM_BUTTON_GENERATE     320
#define IDM_BUTTON_CLEAR        850

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

const int PASSWORD_TYPE_ALL = 0;
const int PASSWORD_TYPE_ALPHANUMERIC = 1;
const int PASSWORD_TYPE_NUMERIC = 2;
const int PASSWORD_TYPE_AHPHABET = 3;

const std::wstring Charecters_all = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*";
const std::wstring Charecters_ALPHANUMERIC = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
const std::wstring Charecters_ALPHA = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::wstring Charecters_NUMERIC = L"1234567890";

HWND passType, passLen, passBox;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

std::wstring generatePasswword(int length = 8, int type = 0)
{
    std::wstring password;
    std::wstring charss;
    switch (type)
    {
    case PASSWORD_TYPE_NUMERIC:
        charss = Charecters_NUMERIC;
        break;
    case PASSWORD_TYPE_ALPHANUMERIC:
        charss = Charecters_ALPHANUMERIC;
        break;
    case PASSWORD_TYPE_ALL:
        charss = Charecters_all;
        break;
    case PASSWORD_TYPE_AHPHABET:
    default:
        charss = Charecters_ALPHA;
        break;
    }

    for (int i = 0; i < length; i++)
    {
        int index = rand() % charss.length();
        password += charss[index];
    }

    return password;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RANDOMPASSWORDGENERATOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}



BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, CW_USEDEFAULT, 600, 300, nullptr, nullptr, hInstance, nullptr);
   
   CreateWindowEx(WS_EX_TRANSPARENT, WC_STATIC, L"Password Type: ", WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
       30, 30, 140, 30, hWnd, nullptr, hInstance, nullptr);

   passType = CreateWindowW(WC_COMBOBOX, L"", CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
       200, 30, 350 , 200, hWnd, (HMENU) IDM_PASSWORD_TYPE, hInstance, nullptr);
   
   TCHAR Password_Types[4][25] = {
       L"ALPHANUMERIC +  Symbol",
       L"ALPHANUMERIC",
       L"NUMERIC", 
       L"ALPHABET"
   };

   TCHAR t[25];
   for (int k = 0; k < 4; k++)
   {
       wcscpy_s(t, sizeof(t) / sizeof(TCHAR), (TCHAR *)Password_Types[k]);

       SendMessage(passType, CB_ADDSTRING, 0, (LPARAM)t);
   }
   SendMessage(passType, CB_SETCURSEL, 0, NULL);

   CreateWindowEx(WS_EX_TRANSPARENT, WC_STATIC, L"Password Length: ", WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
       30, 73, 140, 30,/*271, 43, 83, 13*/ hWnd, nullptr, hInstance, nullptr);
   
   INITCOMMONCONTROLSEX controls;
   controls.dwICC = ICC_UPDOWN_CLASS;
   InitCommonControlsEx(&controls);
   
   HWND passLENGTHBuddy = CreateWindowEx(WS_EX_RIGHT, WC_EDIT, L"", ES_READONLY| WS_CHILD | WS_OVERLAPPED | WS_BORDER | WS_VISIBLE,
       200, 70, 350 - 20, 25, hWnd, (HMENU)IDM_PASSWORD_LENGTH, hInstance, nullptr);
   passLen = CreateWindowEx(WS_EX_RIGHTSCROLLBAR | WS_EX_LEFT | WS_EX_LTRREADING, UPDOWN_CLASS, NULL, 
       WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | UDS_ARROWKEYS | UDS_SETBUDDYINT,
       200 + 350 - 20 , 70, 50, 25, hWnd, (HMENU)IDM_PASSWORD_LENGTH, hInstance, nullptr);


   SendMessage(passLen, UDM_SETRANGE32, 8, 100);
   SendMessage(passLen, UDM_SETBUDDY, (WPARAM) passLENGTHBuddy, 0);
   SendMessage(passLen, UDM_SETPOS, 0, 8);

   CreateWindowEx(WS_EX_TRANSPARENT, WC_STATIC, L"Generated Password: ", WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
       30, 110, 140, 30, hWnd, nullptr, hInstance, nullptr);

   passBox = CreateWindowW( WC_EDIT, L"", WS_CHILD | WS_OVERLAPPED | WS_BORDER | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE,
       30, 140 , 525, 60, hWnd, (HMENU)IDM_PASSWORD_LENGTH, hInstance, nullptr);

   CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, WC_BUTTON, L"Generate Password", BS_PUSHBUTTON | BS_TEXT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
       50, 210, 150, 40, hWnd, (HMENU) IDM_BUTTON_GENERATE, hInstance, nullptr);

   CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR, WC_BUTTON, L"Clear", BS_PUSHBUTTON | BS_TEXT | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
       350 , 210, 150, 40, hWnd, (HMENU) IDM_BUTTON_CLEAR, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CTLCOLORSTATIC:
        return (LRESULT)(HBRUSH)(COLOR_WINDOW + 1);
    case WM_COMMAND:
        if (LOWORD(wParam) == IDM_BUTTON_CLEAR)
        {
            SendMessage(passBox, WM_SETTEXT, 0, (LPARAM)L"");
        }
        else if (LOWORD(wParam) == IDM_BUTTON_GENERATE)
        {
            int len = LOWORD(SendMessage(passLen, UDM_GETPOS, 0, 0));
            int type = SendMessage(passType, CB_GETCURSEL, 0, 0);
            SendMessage(passBox, WM_SETTEXT, 0, (LPARAM)generatePasswword(len, type).c_str());
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    srand(time(nullptr));

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_RANDOMPASSWORDGENERATOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
