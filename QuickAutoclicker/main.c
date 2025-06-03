#define START_F8 1001
#define STOP_F7 1002

#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL isBeginning = FALSE;

DWORD beginClick(void *data)
{
	DWORD milliseconds = (DWORD)data;

	if (milliseconds == 0)
	{
		while (isBeginning)
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, 0, 0);
		}

		return 0;
	}

	while (isBeginning)
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, 0, 0);
        mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, 0, 0);

		Sleep(milliseconds);
	}

	return 0;
}

DWORD beginRandomClick(void *data)
{
	DWORD milliseconds = (DWORD)data;

	if (milliseconds == 0)
	{
		while (isBeginning)
		{
			if (rand() % 80 < 65)
			{
				mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, 0, 0);
			}
		}

		return 0;
	}

	while (isBeginning)
	{
		if (rand() % 80 < 65)
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, 0, 0);
		}

		Sleep(milliseconds);
	}

	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	WNDCLASS wc;
	memset(&wc, 0, sizeof(WNDCLASS));

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"QuickAutoclicker Window Class";

	RegisterClass(&wc);

	HWND window = CreateWindow(
		L"QuickAutoclicker Window Class", 
		L"QuickAutoclicker v2.0", 
		WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, 306, 239,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (window == NULL) return -1;

	HWND textBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"Edit", L"1000",
        WS_CHILD | WS_VISIBLE, 75, 30, 140, 20, window, NULL, NULL, NULL);

	HWND label1 = CreateWindow(L"STATIC", L"ST_U", WS_CHILD | WS_VISIBLE, 100, 5, 90, 20, window, NULL, NULL, NULL);
    SetWindowText(label1, L"Milliseconds:");
    HWND label2 = CreateWindow(L"STATIC", L"ST_U", WS_CHILD | WS_VISIBLE, 120, 55, 50, 20, window, NULL, NULL, NULL);
    SetWindowText(label2, L"Control:");

    HWND label3 = CreateWindow(L"STATIC", L"ST_U", WS_CHILD | WS_VISIBLE, 115, 75, 60, 20, window, NULL, NULL, NULL);
    SetWindowText(label3, L"F8 - start");
    HWND label4 = CreateWindow(L"STATIC", L"ST_U", WS_CHILD | WS_VISIBLE, 115, 95, 60, 20, window, NULL, NULL, NULL);
    SetWindowText(label4, L"F7 - stop");

    HWND checkBox = CreateWindowEx(NULL, L"BUTTON", L"Random click", BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE, 5, 170, 
		110, 20, window, (HMENU)1, NULL, NULL);

	ShowWindow(window, nCmdShow);
	UpdateWindow(window);

	RegisterHotKey(NULL, START_F8, NULL, VK_F8);
	RegisterHotKey(NULL, STOP_F7, NULL, VK_F7);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_HOTKEY)
		{
			switch (msg.wParam)
			{
			case START_F8:
				{
					if (isBeginning == FALSE)
					{
						isBeginning = TRUE;

						EnableWindow(textBox, FALSE);
						EnableWindow(checkBox, FALSE);

						TCHAR buff[1024];
						GetWindowText(textBox, buff, 1024);
						DWORD milliseconds = wcstod(buff, NULL);

						LRESULT checked = SendMessage(checkBox, BM_GETCHECK, 0, 0);

						if (checked == BST_CHECKED)
							CreateThread(NULL, 0, beginRandomClick, (void*)milliseconds, 0, NULL);
						else
							CreateThread(NULL, 0, beginClick, (void*)milliseconds, 0, NULL);
					}
				}
				break;

			case STOP_F7:
				{
					if (isBeginning)
					{
						isBeginning = FALSE;

						EnableWindow(textBox, TRUE);
						EnableWindow(checkBox, TRUE);
					}
				}
				break;
			}
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
        }
        return 0;
    }

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
