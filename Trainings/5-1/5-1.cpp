#include <windows.h>
#include <atlimage.h>

LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"5-1";
HINSTANCE g_hinst;

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
void TextPrint(HDC hdc, int x, int y, TCHAR text[]);

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
	PAINTSTRUCT ps;
	static CImage img;
	static HDC hdc, memdc;
	static RECT rect; // ������ �簢��
	static int xPos, yPos, nW, nH, sw, sh, zw, zh, mx, my;
	// xPos/yPos: DC�� �׸� ��ġ
	// nW/nH: �׸� �簢�� ũ��, sw/sh: �ҽ� �̹��� ũ��
	// zw/zh: ũ�� ����, mx/my: ���콺 ��ġ
	static HBITMAP hBitmap;
	static DWORD dwRop; // �ҽ� ���
	static int div; // 2: 2x2����, 3: 3x3����
	static RECT selection;
	static bool select[3][3];
	static int i, j;
	static bool click;

	switch (iMsg)
	{
	case WM_CREATE:
		dwRop = SRCCOPY;
		img.Load(TEXT("image1.jpg"));
		GetClientRect(hwnd, &rect);
		sw = img.GetWidth();
		sh = img.GetHeight();
		xPos = 0;
		nW = sw; nH = sh;
		zw = 0; zh = 0;
		div = 1;
		click = false;
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		memdc = CreateCompatibleDC(hdc);
		(HBITMAP)SelectObject(memdc, hBitmap);
		HBRUSH hBrush;
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		(HBRUSH)SelectObject(hdc, hBrush);
		
		if (div == 2)
		{
			img.Draw(memdc, 0, 0, nW, nH, 0, 0, sw, sh);
			if (click)
			{
				selection = { j * nW + xPos, i * nH, j * nW + nW + xPos, i * nH + nH };
				if (select[0][0])
				{
					BitBlt(hdc, nW, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 2
					BitBlt(hdc, xPos, 0, nW, nH, memdc, 0, 0, SRCCOPY); // 3
					BitBlt(hdc, 0, nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 4
					BitBlt(hdc, nW, nH, nW, nH, memdc, 0, 0, SRCCOPY); // 1
				}
				else if (select[0][1])
				{
					BitBlt(hdc, 0, 0, nW, nH, memdc, 0, 0, SRCCOPY); // 1
					BitBlt(hdc, xPos + nW, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 3
					BitBlt(hdc, 0, nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 4
					BitBlt(hdc, nW, nH, nW, nH, memdc, 0, 0, SRCCOPY); // 2
				}
				else if (select[1][0])
				{
					BitBlt(hdc, 0, 0, nW, nH, memdc, 0, 0, SRCCOPY); // 1
					BitBlt(hdc, nW, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 2
					BitBlt(hdc, nW, nH, nW, nH, memdc, 0, 0, SRCCOPY); // 4
					BitBlt(hdc, xPos, nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 3
				}
				else if (select[1][1])
				{
					BitBlt(hdc, 0, 0, nW, nH, memdc, 0, 0, SRCCOPY); // 1
					BitBlt(hdc, nW, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 2
					BitBlt(hdc, 0, nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 3
					BitBlt(hdc, nW + xPos, nH, nW, nH, memdc, 0, 0, SRCCOPY); // 4
				}
				FrameRect(hdc, &selection, hBrush);
				DeleteObject(hBitmap);
			}
			else
			{
				BitBlt(hdc, 0, 0, nW, nH, memdc, 0, 0, SRCCOPY); // 1
				BitBlt(hdc, nW, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 2
				BitBlt(hdc, 0, nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 3
				BitBlt(hdc, nW, nH, nW, nH, memdc, 0, 0, SRCCOPY); // 4
			}
		}
		else if (div == 3)
		{
				img.Draw(memdc, 0, 0, nW, nH, 0, 0, sw, sh);
				if (click)
				{
					selection = { j * nW + xPos, i * nH, j * nW + nW + xPos, i * nH + nH };
					if (select[0][0])
					{
						BitBlt(hdc, nW, 0, nW, nH, memdc, 0, 0, SRCCOPY); // 2
						BitBlt(hdc, nW * 2, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY);  // 3
						BitBlt(hdc, 0, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 4
						BitBlt(hdc, 0 + nW, 0 + nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 5
						BitBlt(hdc, 0 + nW * 2, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 6
						BitBlt(hdc, 0, 0 + 2 * nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 7
						BitBlt(hdc, 0 + nW, 0 + nH * 2, nW, nH, memdc, 0, 0, SRCCOPY); // 8
						BitBlt(hdc, 0 + nW * 2, 0 + nH * 2, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 9
						BitBlt(hdc, xPos, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 1
					}
					else if (select[0][1])
					{
						BitBlt(hdc, 0, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 1
						BitBlt(hdc, nW * 2, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY);  // 3
						BitBlt(hdc, 0, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 4
						BitBlt(hdc, 0 + nW, 0 + nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 5
						BitBlt(hdc, 0 + nW * 2, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 6
						BitBlt(hdc, 0, 0 + 2 * nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 7
						BitBlt(hdc, 0 + nW, 0 + nH * 2, nW, nH, memdc, 0, 0, SRCCOPY); // 8
						BitBlt(hdc, 0 + nW * 2, 0 + nH * 2, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 9
						BitBlt(hdc, nW + xPos, 0, nW, nH, memdc, 0, 0, SRCCOPY); // 2
					}
					else if (select[0][2])
					{
						BitBlt(hdc, 0, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 1
						BitBlt(hdc, nW, 0, nW, nH, memdc, 0, 0, SRCCOPY); // 2
						BitBlt(hdc, 0, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 4
						BitBlt(hdc, 0 + nW, 0 + nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 5
						BitBlt(hdc, 0 + nW * 2, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 6
						BitBlt(hdc, 0, 0 + 2 * nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 7
						BitBlt(hdc, 0 + nW, 0 + nH * 2, nW, nH, memdc, 0, 0, SRCCOPY); // 8
						BitBlt(hdc, 0 + nW * 2, 0 + nH * 2, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 9
						BitBlt(hdc, nW * 2 + xPos, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY);  // 3
					}
					else if (select[1][0])
					{
						BitBlt(hdc, 0, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 1
						BitBlt(hdc, nW, 0, nW, nH, memdc, 0, 0, SRCCOPY); // 2
						BitBlt(hdc, nW * 2, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY);  // 3
						BitBlt(hdc, 0 + nW, 0 + nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 5
						BitBlt(hdc, 0 + nW * 2, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 6
						BitBlt(hdc, 0, 0 + 2 * nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 7
						BitBlt(hdc, 0 + nW, 0 + nH * 2, nW, nH, memdc, 0, 0, SRCCOPY); // 8
						BitBlt(hdc, 0 + nW * 2, 0 + nH * 2, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 9
						BitBlt(hdc, xPos, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 4
					}
					else if (select[1][1])
					{
						BitBlt(hdc, 0, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 1
						BitBlt(hdc, nW, 0, nW, nH, memdc, 0, 0, SRCCOPY); // 2
						BitBlt(hdc, nW * 2, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY);  // 3
						BitBlt(hdc, 0, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 4
						BitBlt(hdc, 0 + nW * 2, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 6
						BitBlt(hdc, 0, 0 + 2 * nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 7
						BitBlt(hdc, 0 + nW, 0 + nH * 2, nW, nH, memdc, 0, 0, SRCCOPY); // 8
						BitBlt(hdc, 0 + nW * 2, 0 + nH * 2, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 9
						BitBlt(hdc, 0 + nW + xPos, 0 + nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 5
					}
					else if (select[1][2])
					{
						BitBlt(hdc, 0, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 1
						BitBlt(hdc, nW, 0, nW, nH, memdc, 0, 0, SRCCOPY); // 2
						BitBlt(hdc, nW * 2, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY);  // 3
						BitBlt(hdc, 0, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 4
						BitBlt(hdc, 0 + nW, 0 + nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 5
						BitBlt(hdc, 0, 0 + 2 * nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 7
						BitBlt(hdc, 0 + nW, 0 + nH * 2, nW, nH, memdc, 0, 0, SRCCOPY); // 8
						BitBlt(hdc, 0 + nW * 2, 0 + nH * 2, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 9
						BitBlt(hdc, 0 + nW * 2 + xPos, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 6
					}
					else if (select[2][0])
					{
						BitBlt(hdc, xPos, 0 + 2 * nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 7
						BitBlt(hdc, 0, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 1
						BitBlt(hdc, nW, 0, nW, nH, memdc, 0, 0, SRCCOPY); // 2
						BitBlt(hdc, nW * 2, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY);  // 3
						BitBlt(hdc, 0, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 4
						BitBlt(hdc, 0 + nW, 0 + nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 5
						BitBlt(hdc, 0 + nW * 2, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 6
						BitBlt(hdc, 0 + nW, 0 + nH * 2, nW, nH, memdc, 0, 0, SRCCOPY); // 8
						BitBlt(hdc, 0 + nW * 2, 0 + nH * 2, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 9
					}
					else if (select[2][1])
					{
						BitBlt(hdc, nW + xPos, 0 + nH * 2, nW, nH, memdc, 0, 0, SRCCOPY); // 8
						BitBlt(hdc, 0, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 1
						BitBlt(hdc, nW, 0, nW, nH, memdc, 0, 0, SRCCOPY); // 2
						BitBlt(hdc, nW * 2, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY);  // 3
						BitBlt(hdc, 0, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 4
						BitBlt(hdc, 0 + nW, 0 + nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 5
						BitBlt(hdc, 0 + nW * 2, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 6
						BitBlt(hdc, 0, 0 + 2 * nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 7
						BitBlt(hdc, 0 + nW * 2, 0 + nH * 2, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 9
					}
					else if (select[2][2])
					{
						BitBlt(hdc, 0, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 1
						BitBlt(hdc, nW, 0, nW, nH, memdc, 0, 0, SRCCOPY); // 2
						BitBlt(hdc, nW * 2, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY);  // 3
						BitBlt(hdc, 0, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 4
						BitBlt(hdc, 0 + nW, 0 + nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 5
						BitBlt(hdc, 0 + nW * 2, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 6
						BitBlt(hdc, 0, 0 + 2 * nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 7
						BitBlt(hdc, 0 + nW, 0 + nH * 2, nW, nH, memdc, 0, 0, SRCCOPY); // 8
						BitBlt(hdc, nW * 2 + xPos, 0 + nH * 2, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 9
					}
					FrameRect(hdc, &selection, hBrush);
					DeleteObject(hBitmap);
				}
				//else if (select[][])
				//{
				//	BitBlt(hdc, 0, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 1
				//	BitBlt(hdc, nW, 0, nW, nH, memdc, 0, 0, SRCCOPY); // 2
				//	BitBlt(hdc, nW * 2, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY);  // 3
				//	BitBlt(hdc, 0, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 4
				//	BitBlt(hdc, 0 + nW, 0 + nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 5
				//	BitBlt(hdc, 0 + nW * 2, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 6
				//	BitBlt(hdc, 0, 0 + 2 * nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 7
				//	BitBlt(hdc, 0 + nW, 0 + nH * 2, nW, nH, memdc, 0, 0, SRCCOPY); // 8
				//	BitBlt(hdc, 0 + nW * 2, 0 + nH * 2, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 9
				//}
		
		else
			{
			BitBlt(hdc, xPos, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 1
			BitBlt(hdc, xPos + nW, 0, nW, nH, memdc, 0, 0, SRCCOPY); // 2
			BitBlt(hdc, xPos + nW * 2, 0, nW, nH, memdc, 0, 0, NOTSRCCOPY);  // 3
			BitBlt(hdc, xPos, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 4
			BitBlt(hdc, xPos + nW, 0 + nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 5
			BitBlt(hdc, xPos + nW * 2, 0 + nH, nW, nH, memdc, 0, 0, SRCCOPY); // 6
			BitBlt(hdc, xPos, 0 + 2 * nH, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 7
			BitBlt(hdc, xPos + nW, 0 + nH * 2, nW, nH, memdc, 0, 0, SRCCOPY); // 8
			BitBlt(hdc, xPos + nW * 2, 0 + nH * 2, nW, nH, memdc, 0, 0, NOTSRCCOPY); // 9
			}
		}
		else
		{
			img.Draw(memdc, 0, 0, nW + zw, nH + zh, 0, 0, sw, sh);
			BitBlt(hdc, xPos, 0, nW + zw, nH + zh, memdc, 0, 0, dwRop);
		}
		DeleteObject(hBrush);
		
		DeleteDC(memdc);
		
		EndPaint(hwnd, &ps);
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 'r':
			if (dwRop == SRCCOPY)
				dwRop = NOTSRCCOPY;
			else dwRop = SRCCOPY;
			InvalidateRgn(hwnd, NULL, false);
			break;
		case 'a':
			if (nW == rect.right)
				nW = sw;
			if (nH == rect.bottom)
				nH = sh;
			else
			{
				nW = rect.right;
				nH = rect.bottom;
			}
			InvalidateRgn(hwnd, NULL, true);
			break;
		case '1': // ���� �� �ʱ�ȭ��
			xPos = 0;
			nW = sw; nH = sh;
			zw = 0; zh = 0;
			div = 1;
			click = false;
			InvalidateRgn(hwnd, NULL, false);
			break;
		case '2':
			xPos = 0;
			nW = sw; nH = sh;
			zw = 0; zh = 0;
			click = false;
			div = 2;
			nW = sw / 2 + zw; nH = sh / 2 + zh;
			InvalidateRgn(hwnd, NULL, false);
			break;
		case '3':
			xPos = 0;
			nW = sw; nH = sh;
			zw = 0; zh = 0;
			click = false;
			div = 3;
			nW = sw / 3 + zw; nH = sh / 3 + zh;
			InvalidateRgn(hwnd, NULL, false);
			break;
		case 'q':
			img.Destroy();
			PostQuitMessage(0);
			break;
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			xPos -= 20;
			InvalidateRgn(hwnd, NULL, true);
			break;
		case VK_RIGHT:
			xPos += 20;
			InvalidateRgn(hwnd, NULL, true);
			break;
		case VK_ADD:
			zw += sw / 50; zh += sh / 50;
			InvalidateRgn(hwnd, NULL, true);
			break;
		case VK_SUBTRACT:
			zw -= sw / 50; zh -= sh / 50;
			InvalidateRgn(hwnd, NULL, true);
			break;
		}
		break;


	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		//xPos = 0;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				select[i][j] = false;
		j = mx / nW; i = my / nH;
		if (mx <= sw && my <= sh)
		select[i][j] = true;
		click = true;
		InvalidateRgn(hwnd, NULL, true);
		break;

	case WM_DESTROY:
		img.Destroy();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}