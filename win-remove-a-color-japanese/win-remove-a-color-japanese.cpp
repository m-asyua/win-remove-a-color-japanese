


#include <windows.h>
#include <windowsx.h>
#include <tchar.h>


#define BUTTON_ID_EXE      0100
#define BUTTON_ID_RESET    0101
#define BUTTON_ID_REFRESH  0102
#define BUTTON_ID_SAVE     0103
#define BUTTON_ID_LOAD     0104
#define BUTTON_ID_LOAD_EXE 0105
#define BUTTON_ID_CHECKBOX 0106

#define EDIT_R 02101
#define EDIT_G 02102
#define EDIT_B 02103

HWND check;
HWND combo;
HWND label;
HWND label2;
HWND label3;
HWND label4;
HWND label5;

HWND r_str, g_str, b_str;

HWND hn_array[256];
int hn_counter = 0;
int selectedIndex = -1;


BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lp) {

	wchar_t  strWindowText[512];
	//wchar_t  strClassName[256];

	GetWindowText(hwnd, strWindowText, 256);
	//GetClassName( hwnd, strClassName, 256);

	if (strWindowText[0] == 0)  return TRUE;
	if (IsIconic(hwnd))  return TRUE;

	if (!IsWindowVisible(hwnd))  return TRUE;
	if (!IsWindowEnabled(hwnd))  return TRUE;

	hn_array[hn_counter++] = hwnd;
	SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)strWindowText);
	return TRUE;
}


void my_exe() {

	TCHAR r_text[10], g_text[10], b_text[10];

	TCHAR selectedText[256];
	TCHAR selectedText2[256];

	// get color
	GetWindowText(r_str, r_text, (GetWindowTextLength(r_str) + 1));
	GetWindowText(g_str, g_text, (GetWindowTextLength(g_str) + 1));
	GetWindowText(b_str, b_text, (GetWindowTextLength(b_str) + 1));

	int rv = _wtoi(r_text);
	int gv = _wtoi(g_text);
	int bv = _wtoi(b_text);

	selectedIndex = (int)(SendMessage(combo, CB_GETCURSEL, 0, 0));

	HWND t_hwnd = hn_array[selectedIndex];
	LONG lStyle = GetWindowLong(t_hwnd, GWL_EXSTYLE);
	lStyle |= WS_EX_LAYERED;
	SetWindowLong(t_hwnd, GWL_EXSTYLE, lStyle);
	bool t = SetLayeredWindowAttributes(t_hwnd, RGB(rv, gv, bv), 0, LWA_COLORKEY);

	SendMessage(combo, CB_GETLBTEXT, selectedIndex, (LPARAM)selectedText);
	wsprintf(selectedText2, TEXT("R:%d,G:%d,B:%d, %s"), rv, gv, bv, selectedText);
	SetWindowText(label, selectedText2);

}

void my_load_exe() {
	my_exe();
}

void save_check(int flag) {
	HANDLE hFile;
	DWORD WriteSize;
	LPCTSTR filename = TEXT("my_flag.ini");
	TCHAR buf[256];

	hFile = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, TEXT("file error (save flag)"), NULL, MB_OK);
		return;
	}

	wsprintf(buf, TEXT("%d\n"), flag);

	bool t = WriteFile(hFile, buf, lstrlen(buf) * sizeof(TCHAR), &WriteSize, NULL);
	CloseHandle(hFile);

	//	TCHAR str4[256] = _T("内容を保存しました");
	//	SetWindowText(label4, str4);


}

void my_save() {

	TCHAR r_text[10], g_text[10], b_text[10];

	TCHAR selectedText[256];
	TCHAR selectedText2[256];

	// get color
	GetWindowText(r_str, r_text, (GetWindowTextLength(r_str) + 1));
	GetWindowText(g_str, g_text, (GetWindowTextLength(g_str) + 1));
	GetWindowText(b_str, b_text, (GetWindowTextLength(b_str) + 1));

	int rv = _wtoi(r_text);
	int gv = _wtoi(g_text);
	int bv = _wtoi(b_text);

	SendMessage(combo, CB_GETLBTEXT, selectedIndex, (LPARAM)selectedText);
	wsprintf(selectedText2, TEXT("R:%d,G:%d,B:%d, %s"), rv, gv, bv, selectedText);

	// save
	HANDLE hFile;
	DWORD WriteSize;
	LPCTSTR filename = TEXT("my_setting.ini");
	TCHAR buf[256];

	hFile = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, TEXT("file error (save setting)"), NULL, MB_OK);
		return;
	}

	wsprintf(buf, TEXT("%d\n%d\n%d\n%s\n"), rv, gv, bv, selectedText);

	WriteFile(hFile, buf, lstrlen(buf) * sizeof(TCHAR), &WriteSize, NULL);
	CloseHandle(hFile);

	SetWindowText(label4, _T("内容を保存しました"));


}





bool my_read_file(TCHAR* buff, LPCTSTR filename) {

	TCHAR buf_tmp[256];
	HANDLE hFile;

	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(NULL, TEXT("file error"), NULL, MB_OK);
		return false;
	}

	ReadFile(hFile, buf_tmp, sizeof(buf_tmp) / sizeof(TCHAR), NULL, NULL);
	CloseHandle(hFile);

	memcpy(buff, buf_tmp, sizeof(buf_tmp));

	return true;
}

bool load_check() {

	TCHAR* context;
	TCHAR* token;
	TCHAR flag_text[128];

	TCHAR buf[256];

	LPCTSTR target = TEXT("my_flag.ini");

	// file exist?
	if (GetFileAttributes(target) == 0xFFFFFFFF) {
		save_check(0);
	}

	if (my_read_file(buf, target) == -1) { return -1; }

	token = wcstok_s(buf, TEXT("\n"), &context);

	wsprintf(flag_text, TEXT("%s"), token);

	// flag on or off
	int flag = _wtoi(flag_text);
	if (flag == 1) {
		SendMessage(check, BM_SETCHECK, BST_CHECKED, 0);
		return true;
	}
	else {
		SendMessage(check, BM_SETCHECK, BST_UNCHECKED, 0);
		return false;
	}

}



bool my_load() {

	TCHAR Delim = '\n';
	TCHAR* context;
	TCHAR* token;
	int t_counter = 0;

	TCHAR r_text[10], g_text[10], b_text[10];
	TCHAR tool_text[256];

	TCHAR buf2[256];

	TCHAR buf[256];

	LPCTSTR target = TEXT("my_setting.ini");
	if (my_read_file(buf, target) == -1) { return -1; }

	token = wcstok_s(buf, TEXT("\n"), &context);

	while (token) {
		if (t_counter == 0) { wsprintf(r_text, TEXT("%s"), token); }
		if (t_counter == 1) { wsprintf(g_text, TEXT("%s"), token); }
		if (t_counter == 2) { wsprintf(b_text, TEXT("%s"), token); }
		if (t_counter == 3) { wsprintf(tool_text, TEXT("%s"), token); }

		t_counter++;

		token = wcstok_s(NULL, TEXT("\n"), &context);
	}


	SetWindowText(r_str, r_text);
	SetWindowText(g_str, g_text);
	SetWindowText(b_str, b_text);


	wsprintf(buf2, TEXT("R:%s,G:%s,B:%s, %s"), r_text, g_text, b_text, tool_text);

	// display content of file
	SetWindowText(label2, buf2);

	int result = -1;

	// find text from combobox and display the text
	TCHAR tmp_text[512];

	for (int i = 0; i < SendMessage(combo, CB_GETCOUNT, 0, 0); i++) {
		SendMessage(combo, CB_GETLBTEXT, i, (LPARAM)tmp_text);
		if (wcsstr(tmp_text, tool_text) != NULL) {
			// select i
			SendMessage(combo, (UINT)CB_SETCURSEL, (WPARAM)i, 0);
			result = 1;
			break;
		}
	}


	if (result == 1) {
		SetWindowText(label3, TEXT("同じ名前のウィンドウが見つかりました（実行可能）"));
		SetWindowText(label5, _T("内容を呼び出しました"));
		return true;
	}
	else {
		SetWindowText(label3, TEXT("同じ名前のウィンドウが見つかりませんでした（実行不可）"));
		return false;
	}

}


void my_reset() {
	HWND t_hwnd = hn_array[selectedIndex];
	LONG lStyle = GetWindowLong(t_hwnd, GWL_EXSTYLE);
	lStyle &= ~WS_EX_LAYERED;
	SetWindowLong(t_hwnd, GWL_EXSTYLE, lStyle);
}

void my_refresh() {
	// refresh combo
	ComboBox_ResetContent(combo);

	hn_counter = 0;
	EnumWindows(EnumWindowsProc, NULL); // callback
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {

	HDC hdc;
	PAINTSTRUCT ps;

	TCHAR str0[256] = _T("1.透明にしたいウィンドウを選んでください");
	TCHAR str1[256] = _T("  ※ブラウザ等ではハードウェアアクセラレーションは使用しない設定にしておいてください");
	TCHAR str2[256] = _T("2.透明にしたい色を指定してください(0-255)");
	TCHAR str3[256] = _T("3.実行");
	TCHAR str_r[5] = _T("R");
	TCHAR str_g[5] = _T("G");
	TCHAR str_b[5] = _T("B");


	switch (msg) {

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		TextOut(hdc, 10, 5, str0, lstrlen((LPTSTR)str0));
		TextOut(hdc, 10, 25, str1, lstrlen((LPTSTR)str1));
		TextOut(hdc, 10, 90, str2, lstrlen((LPTSTR)str2));
		TextOut(hdc, 10, 160, str3, lstrlen((LPTSTR)str3));

		TextOut(hdc, 35, 120, str_r, lstrlen((LPTSTR)str_r));
		TextOut(hdc, 135, 120, str_g, lstrlen((LPTSTR)str_g));
		TextOut(hdc, 235, 120, str_b, lstrlen((LPTSTR)str_b));

		EndPaint(hwnd, &ps);

	case WM_COMMAND:

		if (HIWORD(wp) == CBN_SELCHANGE) {
			selectedIndex = (int)(SendMessage(combo, CB_GETCURSEL, 0, 0));
		}

		switch (LOWORD(wp)) {
		case BUTTON_ID_EXE:
			my_exe();
			break;
		case BUTTON_ID_RESET:
			my_reset();
			break;
		case BUTTON_ID_REFRESH:
			my_refresh();
			break;
		case BUTTON_ID_SAVE:
			my_save();
			break;
		case BUTTON_ID_LOAD:
			my_load();
			break;
		case BUTTON_ID_LOAD_EXE:
			my_load_exe();
			break;
		case BUTTON_ID_CHECKBOX:

			if (BST_CHECKED == SendMessage(check, BM_GETCHECK, 0, 0)) {
				// save_check_on
				save_check(1);
			}
			else {
				// save_check_off
				save_check(0);
			}
			break;
		}

		return 0;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {

	HWND hwnd;
	MSG  msg;
	WNDCLASS winc;

	winc.style = CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc = WndProc;
	winc.hInstance = hInstance;
	winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winc.hCursor = LoadCursor(NULL, IDC_ARROW);
	winc.cbClsExtra = winc.cbWndExtra = 0;
	winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName = NULL;
	winc.lpszClassName = TEXT("window_transparent_test");

	if (!RegisterClass(&winc)) return -1;

	hwnd = CreateWindow(
		TEXT("window_transparent_test"), TEXT("window_setting"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		NULL,
		NULL,
		800,
		600,
		NULL, NULL, hInstance, NULL
	);

	combo = CreateWindow(
		TEXT("COMBOBOX"), NULL,
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		20, 45, 750, 380,
		hwnd, NULL, hInstance, NULL
	);

	r_str = CreateWindow(TEXT("EDIT"), TEXT("0"), WS_CHILD | WS_VISIBLE | ES_RIGHT | WS_BORDER, 50, 120, 60, 30, hwnd, (HMENU)EDIT_R, hInstance, NULL);
	g_str = CreateWindow(TEXT("EDIT"), TEXT("0"), WS_CHILD | WS_VISIBLE | ES_RIGHT | WS_BORDER, 150, 120, 60, 30, hwnd, (HMENU)EDIT_G, hInstance, NULL);
	b_str = CreateWindow(TEXT("EDIT"), TEXT("0"), WS_CHILD | WS_VISIBLE | ES_RIGHT | WS_BORDER, 250, 120, 60, 30, hwnd, (HMENU)EDIT_B, hInstance, NULL);

	CreateWindow(TEXT("BUTTON"), TEXT("ウィンドウ情報を再読み込み"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 550, 120, 220, 40, hwnd, (HMENU)BUTTON_ID_REFRESH, hInstance, NULL);

	CreateWindow(TEXT("BUTTON"), TEXT("実行"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 20, 190, 100, 50, hwnd, (HMENU)BUTTON_ID_EXE, hInstance, NULL);
	CreateWindow(TEXT("BUTTON"), TEXT("リセット"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 200, 190, 100, 50, hwnd, (HMENU)BUTTON_ID_RESET, hInstance, NULL);

	label = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE, 30, 250, 700, 35, hwnd, NULL, hInstance, NULL);

	CreateWindow(TEXT("BUTTON"), TEXT("上記の内容を保存(SAVE)"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 20, 290, 250, 35, hwnd, (HMENU)BUTTON_ID_SAVE, hInstance, NULL);

	label4 = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE, 320, 290, 300, 35, hwnd, NULL, hInstance, NULL);

	CreateWindow(TEXT("BUTTON"), TEXT("以前に保存した内容を呼び出し(LOAD)"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 10, 340, 290, 35, hwnd, (HMENU)BUTTON_ID_LOAD, hInstance, NULL);

	label5 = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE, 320, 340, 300, 35, hwnd, NULL, hInstance, NULL);

	label2 = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE, 30, 380, 700, 35, hwnd, NULL, hInstance, NULL);
	label3 = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE, 30, 420, 430, 35, hwnd, NULL, hInstance, NULL);

	CreateWindow(TEXT("BUTTON"), TEXT("呼び出し内容を実行"), WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 470, 420, 160, 35, hwnd, (HMENU)BUTTON_ID_LOAD_EXE, hInstance, NULL);

	check = CreateWindow(
		TEXT("BUTTON"), TEXT("このプログラムを起動するたびに，保存内容を自動で実行するようにする->"),
		WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_LEFTTEXT | BS_AUTOCHECKBOX,
		20, 480, 580, 30,
		hwnd, (HMENU)BUTTON_ID_CHECKBOX, hInstance, NULL
	);

	hn_counter = 0;
	EnumWindows(EnumWindowsProc, NULL); // callback

	if (load_check() != false) {
		if (my_load() != false) {
			my_exe();
		}
	}

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;

}

