#include "stdio.h"
#include "stdlib.h"
#include "windows.h"
#include "shellapi.h"
#include "tlhelp32.h"
#include "comdef.h"
#include "ctime"

DWORD GetProcessIdFromName(const char*processName)
{
	PROCESSENTRY32 pe;
	DWORD id = 0;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe))
		return 0;
	char pname[300];
	do
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32Next(hSnapshot, &pe) == FALSE)
			break;
		//把WCHAR*类型转换为const char*类型
		sprintf_s(pname, "%ws", pe.szExeFile);
		//比较两个字符串，如果找到了要找的进程
		if (strcmp(pname, processName) == 0)
		{
			id = pe.th32ProcessID;
			break;
		}

	} while (1);

	CloseHandle(hSnapshot);

	return id;
}

int main(void)
{
	clock_t delay_1, delay_2;
	delay_1 = 300 * CLOCKS_PER_SEC;
	delay_2 = 1 * CLOCKS_PER_SEC;
	while (1)
	{
		clock_t start = clock();
		DWORD pid = GetProcessIdFromName("explorer.exe");       //获取进程ID
		HANDLE token = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);    //获取进程全部权限
		while (clock() - start < delay_2);
		TerminateProcess(token, 0);                                    //退出程序
		WinExec("explorer.exe", SW_SHOWMINIMIZED);
		// HINSTANCE hNewExe = ShellExecuteA(NULL, "open", "C:\\Program Files (x86)\\Tencent\\QQBrowser\\QQBrowser.exe", NULL, NULL, SW_SHOW);     //启动程序
		while (clock() - start < delay_1);           //延时等待
	}
	system("pause");
	return 1;
}