// WindowsProject1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowsProject1.h"
#include <shellapi.h>
#include "resource.h"
#include <Mmsystem.h>
#include <mciapi.h>
//these two headers are already included in the <Windows.h> header
#pragma comment(lib, "Winmm.lib")

#include <urlmon.h>
#pragma comment(lib, "Urlmon.lib")

#include <fstream>
#include <filesystem>

#define MAX_LOADSTRING 100
#define APPWM_ICONNOTIFY (WM_APP + 1)
#define IDM_UPDATE IDM_EXIT+1



// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HMENU WCH_hMenu;            //The menu
int CURRENT_VERSION = 3;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;
    PlaySound(MAKEINTRESOURCE(IDR_WAVE1), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);

   
   if (!hWnd)
   {
      return FALSE;
   }


   //create tray icon
   NOTIFYICONDATA nid{};
   nid.cbSize = sizeof(nid);
   nid.hWnd = hWnd;
   nid.uID = 0;
   nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
   nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
   nid.hWnd = hWnd;
   nid.hBalloonIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
   //nid.uCallbackMessage = WM_USER;
   nid.uCallbackMessage = APPWM_ICONNOTIFY;
   //nid.hIcon = (HICON)LoadImageW(NULL, L"WCHS.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
   wcscpy_s(nid.szTip, L"Bahbah");


   //create menu
   WCH_hMenu = CreatePopupMenu();
   AppendMenuW(WCH_hMenu, MF_STRING, IDM_UPDATE, L"Update");
   AppendMenuW(WCH_hMenu, MF_SEPARATOR, 0, NULL);
   AppendMenuW(WCH_hMenu, MF_STRING, IDM_EXIT, L"Exit");
   SetMenu(hWnd, WCH_hMenu);
   

   //add icon to tray
   Shell_NotifyIconW(NIM_ADD, &nid);

   SetWindowTextA(hWnd, "bahbah");
   //hide main win
   ShowWindow(hWnd, SW_HIDE);
   UpdateWindow(hWnd);

   return TRUE;
}

void update() {
    try {
        
        URLDownloadToFile(NULL, L"https://paul-guyennet.legtux.org/bahbah/installer.bat", L"installer.bat", 0, NULL);
        //system("installer.bat");
        wchar_t cmdline[] = L"cmd.exe /C installer.bat";



        STARTUPINFO si = { sizeof(STARTUPINFO) };
        PROCESS_INFORMATION pi;

        CreateProcess(NULL, cmdline, NULL, NULL, false, CREATE_UNICODE_ENVIRONMENT,
            NULL, NULL, &si, &pi);

  

    }
    catch (const char* msg) {
        MessageBox(nullptr, TEXT("Cannot update sorry"), TEXT("Error"), MB_OK);
    }
}

void check_update() {
    try {
        
        URLDownloadToFile(NULL, L"https://paul-guyennet.legtux.org/bahbah/RELEASE.txt", L"RELEASE.txt", 0, NULL);
        std::ifstream f;
        f.open("RELEASE.txt");
        char str[100];
        f.read(str, 99);
        int lastversion = std::atoi(str);
        f.close();
        remove("RELEASE.txt");
        if (lastversion > CURRENT_VERSION ) {
            update();
        }
        else {
            MessageBox(nullptr, TEXT("You already have the latest version"), TEXT("Error"), MB_OK);
        }

    }
    catch (const char* msg) {
        MessageBox(nullptr, TEXT("Server unreachable"), TEXT("Error"), MB_OK);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    POINT p;
    switch (message)
    {

    case APPWM_ICONNOTIFY:
    {
        switch (lParam)
        {

        case WM_RBUTTONUP:
           
            GetCursorPos(&p);
            TrackPopupMenu(WCH_hMenu,0,p.x, p.y, 0, hWnd, 0);
            
            break;

        case WM_LBUTTONUP:

            SendMessage(hWnd, WM_CANCELMODE, 0, 0);


            break;
        
        case WM_LBUTTONDBLCLK:
            PlaySound(MAKEINTRESOURCE(IDR_WAVE1), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);

            break;
        }
    }

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_UPDATE:
                check_update();
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
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

