#include "windows.h"
#include "malloc.h"
#include "CommCtrl.h"
#include "time.h"

#define StartButtonID 1001
#define FinishButtonID 1002
#define MY_MSG WM_USER + 100 

HINSTANCE hInstance;
HANDLE hStartEvent, hStopEvent;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK EnumWindowsProc(HWND, LPARAM);
BOOL IsAltTabWindow(HWND);
DWORD WINAPI ThreadA(void*);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInst, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASSEX wc;
	HWND hWnd;

	LPCTSTR ClassName = L"SimpleMailWinClass";
	LPCTSTR MenuName = L"NoneMenu";

	int e = 0;
	DWORD style = 0, cstyle = 0;
	HBRUSH hb;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	hInstance = hInst;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wc.lpszClassName = ClassName;
	wc.lpszMenuName = MenuName;
	wc.hIconSm = LoadIcon((HINSTANCE)NULL, IDI_ASTERISK);
	wc.hIcon = LoadIcon((HINSTANCE)NULL, IDI_ASTERISK);
	hb = CreateSolidBrush(RGB(212, 221, 223));
	wc.hbrBackground = hb;
	style = (WS_EX_TOOLWINDOW | WS_VISIBLE | WS_CAPTION | WS_SYSMENU);

	if (RegisterClassEx(&wc) == 0)
	{
		MessageBox(NULL, L"Can't Register Windows Class", L"Mouse Mover v.01", MB_ICONERROR); return 0;
	}

	hWnd = CreateWindowEx(WS_EX_TOOLWINDOW, ClassName, L"Mouse Mover v.01", style,
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 140, NULL, NULL, GetModuleHandle(NULL), NULL);
	if (!hWnd)
	{
		e = GetLastError();
		MessageBox(NULL, L"Can't Create Window", L"Mouse Mover v.01", MB_ICONERROR);
		return 0;
	}
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HFONT hFont;
	static HWND hStartBtn, hFinishBtn = 0, hComboBox = 0;

	LPCTSTR BtnClassName = L"BUTTON";
	DWORD ThreadID = 0, idx = 0;
	HDESK hDesktop = 0;
	BOOL bEnum = FALSE;
	static WCHAR selected[256];

	static HANDLE hThread; static DWORD thID = 0;
	int c = 0, e = 0;

	switch (message)
	{
	case WM_CREATE:
		hFont = CreateFont(0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Sagoe UI"));

		hStartBtn = CreateWindowEx(0, BtnClassName, L"Begin", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			25, 60, 80, 25, hWnd, (HMENU)StartButtonID, hInstance, NULL);
		hFinishBtn = CreateWindowEx(0, BtnClassName, L"Finish", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			280, 60, 80, 25, hWnd, (HMENU)FinishButtonID, hInstance, NULL);
		hComboBox = CreateWindow(WC_COMBOBOX, L"", CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL,
			20, 20, 360, 10000, hWnd, NULL, hInstance, NULL);
		if (hStartBtn == NULL || hFinishBtn == NULL || hComboBox == NULL)
		{
			MessageBox(NULL, L"Can't Create Buttons", L"Mouse Mover v.01", MB_ICONERROR); PostQuitMessage(0); return 0;
		}

		SendMessage(hStartBtn, WM_SETFONT, (WPARAM)hFont, 0);
		SendMessage(hFinishBtn, WM_SETFONT, (WPARAM)hFont, 0);
		SendMessage(hComboBox, WM_SETFONT, (WPARAM)hFont, 0);
		EnableWindow(hFinishBtn, FALSE);

		ThreadID = GetCurrentThreadId();
		hDesktop = GetThreadDesktop(ThreadID);
		bEnum = EnumWindows(EnumWindowsProc, (LPARAM)hComboBox);
		break;
	case WM_COMMAND:
		if ((LOWORD(wParam) == FinishButtonID) && (HIWORD(wParam) == BN_CLICKED))
		{
			//stop thread
			hStopEvent = CreateEvent(0, FALSE, FALSE, 0);
			if (!hStartEvent)MessageBox(NULL, L"Can't Create Stop Event", L"Mouse Mover v.01", MB_ICONERROR);

			if (PostThreadMessage(thID, MY_MSG, (WPARAM)NULL, 0) == 0)
			{
				e = GetLastError();
				MessageBox(NULL, L"Can't Post Message to Thread", L"Mouse Mover v.01", MB_ICONERROR);
			}
			else
			{
				WaitForSingleObject(hStopEvent, INFINITE);
				CloseHandle(hStopEvent);
				EnableWindow(hStartBtn, TRUE);
				EnableWindow(hFinishBtn, FALSE);
			}
		}
		if ((LOWORD(wParam) == StartButtonID) && (HIWORD(wParam) == BN_CLICKED))
		{
			idx = SendMessage(hComboBox, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
			memset(selected, 0, sizeof(WCHAR) * 256);
			SendMessage(hComboBox, CB_GETLBTEXT, idx, (LPARAM)selected);
			//start thread (selected)
			hStartEvent = CreateEvent(0, FALSE, FALSE, 0);
			if (!hStartEvent)MessageBox(NULL, L"Can't Create Start Event", L"Mouse Mover v.01", MB_ICONERROR);
			else
			{
				hThread = CreateThread(NULL, 0, &ThreadA, (LPVOID)selected, 0, &thID);
				if (hThread)EnableWindow(hStartBtn, FALSE);
				else MessageBox(NULL, L"Can't Create Thread", L"Mouse Mover v.01", MB_ICONERROR);
				WaitForSingleObject(hStartEvent, INFINITE);
				CloseHandle(hStartEvent);
				EnableWindow(hStartBtn, FALSE);
				EnableWindow(hFinishBtn, TRUE);
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	int wlen = 0;
	WCHAR wname[256] = { 0 };
	HWND hComboBox = (HWND)lParam;
	LRESULT lres = 0;

	wlen = GetWindowTextLength(hWnd);
	if (!wlen) return TRUE;
	if (IsAltTabWindow(hWnd))
	{
		GetWindowText(hWnd, wname, 255);
		lres = SendMessage(hComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)wname);
		if (lres == CB_ERR || lres == CB_ERRSPACE)
			MessageBox(NULL, L"CB_ADDSTRING Error", L"Mouse Mover v.01", MB_ICONERROR);
		SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)lres, (LPARAM)0);
	}
	return TRUE;
}

BOOL IsAltTabWindow(HWND hwnd)
{
	TITLEBARINFO ti;
	HWND hwndTry, hwndWalk = NULL;

	if (!IsWindowVisible(hwnd))
		return FALSE;

	hwndTry = GetAncestor(hwnd, GA_ROOTOWNER);
	while (hwndTry != hwndWalk)
	{
		hwndWalk = hwndTry;
		hwndTry = GetLastActivePopup(hwndWalk);
		if (IsWindowVisible(hwndTry))
			break;
	}
	if (hwndWalk != hwnd)
		return FALSE;

	ti.cbSize = sizeof(ti);
	GetTitleBarInfo(hwnd, &ti);
	if (ti.rgstate[0] & STATE_SYSTEM_INVISIBLE)
		return FALSE;

	if (GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW)
		return FALSE;

	return TRUE;
}

DWORD WINAPI ThreadA(void* param)
{
	MSG msg;
	HWND hPrey;
	RECT rect = { 0 };
	int e = 0, h = 0, v = 0, w = 0, l = 0;
	WCHAR* prey = (WCHAR*)param;
	hPrey = FindWindow(NULL, prey);
	srand(time(NULL));

	PeekMessage(&msg, NULL, WM_USER, WM_USER + 100, PM_NOREMOVE);
	if (!SetEvent(hStartEvent)) { MessageBox(NULL, L"Can't Handle Start Event", L"Mouse Mover v.01", MB_ICONERROR); return 0; }
	while (1)
	{
		if (GetWindowRect(hPrey, &rect))
		{
			SetForegroundWindow(hPrey);
			SetActiveWindow(hPrey);
			SetFocus(hPrey);
			Sleep(2000);
			v = rand() % rect.right + rect.left;
			l = rand() % rect.bottom + rect.top;

			SetCursorPos(v, l);
			if (PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
				switch (msg.message)
				{
				case MY_MSG:
					if (!SetEvent(hStopEvent)) { MessageBox(NULL, L"Can't Handle Stop Event", L"Mouse Mover v.01", MB_ICONERROR); return 0; }
					ExitThread(0);
					break;
				}
		}
		else { MessageBox(NULL, L"Window was Closed?", L"Mouse Mover v.01", MB_ICONWARNING); return 0; }
	}
	return 0;
}