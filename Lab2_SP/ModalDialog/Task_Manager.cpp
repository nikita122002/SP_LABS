#include "Task_Manager.h"

Task_Manager* Task_Manager::ptr = nullptr;

Task_Manager::Task_Manager(void)
{
	ptr = this;
}

void Task_Manager::OnInitDialog(HWND h)
{
	hList = GetDlgItem(h, IDC_LIST1);
	hBox = GetDlgItem(h, IDC_EDIT2);
	Update();
}

void Task_Manager::Update()
{
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (h == nullptr)
		return;
	SendMessage(hList, LB_RESETCONTENT, 0, 0);
	PROCESSENTRY32 process = { sizeof(PROCESSENTRY32) };
	BOOL flag = Process32First(h, &process);
	if (flag)
	{
		auto index = SendMessage(hList, LB_ADDSTRING, 0, LPARAM(process.szExeFile));
		SendMessage(hList, LB_SETITEMDATA, index, LPARAM(process.th32ProcessID));

		while (Process32Next(h, &process))
		{
			index = SendMessage(hList, LB_ADDSTRING, 0, LPARAM(process.szExeFile));
			SendMessage(hList, LB_SETITEMDATA, index, LPARAM(process.th32ProcessID));
		}
	}
}

void Task_Manager::OnCommand(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))
	{
	case IDC_BUTTON1:
		Update();
		break;
	case IDC_BUTTON2:
	{
		STARTUPINFO startin = { sizeof(startin) };
		TCHAR pathname[MAX_PATH];
		GetWindowText(hBox, pathname, MAX_PATH);
		CreateProcess(nullptr, pathname, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &startin, &pinfo);
		Update();
	}
	break;
	case IDC_BUTTON3:
	{
		auto n = SendMessage(hList, LB_GETCURSEL, 0, 0);
		DWORD NumberID = SendMessage(hList, LB_GETITEMDATA, n, 0);
		HANDLE Process = OpenProcess(PROCESS_ALL_ACCESS, false, NumberID);
		TerminateProcess(Process, 0);
		CloseHandle(Process);
		Update();
		Update();
	}
	break;
	}
}

BOOL CALLBACK Task_Manager::DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp)
{
	switch (mes)
	{
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		break;
	case WM_INITDIALOG:
		ptr->OnInitDialog(hWnd);
		break;
	case WM_COMMAND:
		ptr->OnCommand(wp, lp);
		break;
	}
	return 0;
}