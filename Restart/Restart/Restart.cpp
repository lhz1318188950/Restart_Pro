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
		//��WCHAR*����ת��Ϊconst char*����
		sprintf_s(pname, "%ws", pe.szExeFile);
		//�Ƚ������ַ���������ҵ���Ҫ�ҵĽ���
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
		DWORD pid = GetProcessIdFromName("explorer.exe");       //��ȡ����ID
		HANDLE token = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);    //��ȡ����ȫ��Ȩ��
		while (clock() - start < delay_2);
		TerminateProcess(token, 0);                                    //�˳�����
		WinExec("explorer.exe", SW_SHOWMINIMIZED);
		// HINSTANCE hNewExe = ShellExecuteA(NULL, "open", "C:\\Program Files (x86)\\Tencent\\QQBrowser\\QQBrowser.exe", NULL, NULL, SW_SHOW);     //��������
		while (clock() - start < delay_1);           //��ʱ�ȴ�
	}
	system("pause");
	return 1;
}