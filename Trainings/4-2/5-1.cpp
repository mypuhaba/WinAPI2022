#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <atlimage.h>
#define SIZE 25 // ���� ���� ����
#define WIDTH 30 //

LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Practice";
HINSTANCE g_hinst;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR ipszCmdPram, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX wndclass; // ����ü ����
	g_hinst = hInstance;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW; // ������ ��� ��Ÿ��
	wndclass.lpfnWndProc = (WNDPROC)WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // ������ ����
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW); // Ŀ�� ����
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ���
	wndclass.lpszMenuName = NULL; // �޴� �̸�
	wndclass.lpszClassName = lpszClass; // Ŭ���� �̸�
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION); //���� ������
	RegisterClassExW(&wndclass);

	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW,
		0, 0, 900, 800, NULL, (HMENU)NULL, hInstance, NULL); //������ ����� �Լ�

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) { // �޽��� ť�κ��� �޽����� ���´�
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memdc;
	static HBITMAP hBitmap;
	PAINTSTRUCT ps;
	static CImage img, imgSprite;
	static int xPos = 0, yPos = 0;
	static RECT rect;
	static DWORD op = SRCCOPY;

	switch (iMsg)
	{
	case WM_CREATE:
		img.Load(L"image.bmp"); //--- background
		//imgSprite.Load(L��bitmap2.bmp��); //--- sprite image
		GetClientRect(hwnd, &rect);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		memdc = CreateCompatibleDC(hdc);
		(HBITMAP)SelectObject(memdc, hBitmap);
		img.Draw(memdc, 0, 0, rect.right, rect.bottom, 0, 0, img.GetWidth(), img.GetHeight()); //--- �޸� DC�� ��� �׸���
			//imgSprite.Draw(memdc, xPos, yPos, 100, 100, 0, 0, 100, 100); //--- �޸� DC�� ��������Ʈ �׸���
		BitBlt(hdc, 0, 0, rect.right, rect.bottom, memdc, 0, 0, op); //--- �޸� DC�� �׸��� ȭ�� DC�� �����ϱ�
		DeleteObject(hBitmap);
		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;

	case WM_TIMER:
		xPos += 5;
		yPos += 5;
		InvalidateRect(hwnd, NULL, false);
		break;

	case WM_MOUSEMOVE:
		break;

	case WM_LBUTTONUP:
		
		break;

	case WM_LBUTTONDOWN:
	
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 'r':
			static int dir = 1;
			if (dir == 1)
			{
				op = NOTSRCCOPY;
				InvalidateRgn(hwnd, NULL, true);
			}
			else dir = 1;
			
			break;
		}
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
