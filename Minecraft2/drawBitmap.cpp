#include <Windows.h>
bool drawBitmap(HDC hdc, HBITMAP image, int x, int y) {
	if (!image) return false;
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, image);
	BITMAP bmp;
	GetObject(image, sizeof(BITMAP), &bmp);
	BitBlt(hdc, x, y, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hOldBmp);
	DeleteDC(hdcMem);
	return true;
}