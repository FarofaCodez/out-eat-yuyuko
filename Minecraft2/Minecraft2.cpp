#include <iostream>
#include <Windows.h>
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "winmm.lib")
#include <commctrl.h>
#include "resource.h"
#include "drawBitmap.h"

LRESULT wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HBITMAP rumia = NULL;
HBITMAP yuyuko = NULL;
HBITMAP explosion = NULL;
HINSTANCE hInst_g = NULL;
int score = 0;
int yuyuScore = 0;
bool gameEnded = false;
bool exploded = false;
using namespace std;
HWND eatBtn = NULL;
PAINTSTRUCT ps;
HDC hdc;
bool explosionEnabled = false;

int WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	if (lpCmdLine == "--explode") {
		explosionEnabled = true;
	}
	srand(static_cast<unsigned int>(time(nullptr)));
	WNDCLASS wndClass = {};
	wndClass.lpfnWndProc = wndProc;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_BACKGROUND) + 1;
	wndClass.hInstance = hInst;
	wndClass.lpszClassName = "BakaWndClass";

	hInst_g = hInst;

	RegisterClassA(&wndClass);

	HWND hWnd = CreateWindowExA(
		0,
		"BakaWndClass",
		"Out-Eat Yuyuko",
		WS_TILEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
		NULL, NULL, NULL, NULL
	);

	INITCOMMONCONTROLSEX commonInit = {};
	commonInit.dwICC = ICC_WIN95_CLASSES;
	commonInit.dwSize = sizeof(INITCOMMONCONTROLSEX);

	InitCommonControlsEx(&commonInit);

	eatBtn = CreateWindowEx(
		0,
		"BUTTON",
		"Eat",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		50,
		550,
		300,
		50,
		hWnd,
		(HMENU)1,
		NULL,
		NULL
	);

	MSG msg;
	BOOL bRet;
	while ((bRet = GetMessage(&msg, hWnd, 0, 0)) != 0) {
		if (bRet == -1) {
			return 0;
		}
		else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

LRESULT wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case WM_CREATE:
		yuyuko = LoadBitmap(hInst_g, MAKEINTRESOURCE(IDB_IMAGE1));
		rumia = LoadBitmap(hInst_g, MAKEINTRESOURCE(IDB_IMAGE2));
		explosion = LoadBitmap(hInst_g, MAKEINTRESOURCE(IDB_IMAGE3));
		SetTimer(hWnd, 1, 500, (TIMERPROC)NULL);

		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		drawBitmap(hdc, rumia, 50, 200);
		drawBitmap(hdc, yuyuko, 800, 200);
		if (exploded) {
			drawBitmap(hdc, explosion, 50, 200);
		}

		EndPaint(hWnd, &ps);
		break;
	
	case WM_COMMAND:
		if (wParam == 1) {
			score += 2;
			if (explosionEnabled) {
				int willExplode = rand();
				if (willExplode > 32700) { //32700
					exploded = true;
					RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
					PlaySound(MAKEINTRESOURCE(IDR_WAVE1), hInst_g, SND_ASYNC | SND_NODEFAULT | SND_RESOURCE);
					DestroyWindow(eatBtn);
					gameEnded = true;
				}
			}
		}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		if (gameEnded) break;
		yuyuScore += 5;
		char text[300] = "Out-Eat Yuyuko";
		sprintf_s(text, "Out-Eat Yuyuko -- You: %d, Yuyuko: %d", score, yuyuScore);
		SetWindowText(hWnd, text);
		if (score >= 1000) {
			gameEnded = true;
			SetWindowText(hWnd, "You won!");
		}
		if (yuyuScore >= 1000) {
			gameEnded = true;
			SetWindowText(hWnd, "You lost!");
		}
	}

	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}