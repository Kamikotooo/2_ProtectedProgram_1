#include <windows.h>
#include <conio.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <string.h>

wchar_t* getname(char* arg) {
	int size = 0;
	int i = 0;
	for (size, i; arg[i] != '\0'; size++, i++) if (arg[i] == '\\') size = -1;
	char* Aname = (char*)malloc(size + 1);
	Aname[size] = '\0';
	for (int j = i - size; arg[j] != '\0'; j++) Aname[j - (i - size)] = arg[j];
	LPWSTR Wname = (PWCHAR)calloc(size + 1, sizeof(WCHAR));
	DWORD iResult = MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED,
		Aname,
		INFINITE,
		Wname,
		size);
	return Wname;
}

int main(int argc, char* argv[])
{
	wchar_t* Wname = getname(argv[0]);

	int countproc = 0;
	HANDLE hSnap;
	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == NULL)
	{
		return 0;
	}
	PROCESSENTRY32 proc;
	proc.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnap, &proc))
	{
		do {
			wprintf_s(L"%s\n", proc.szExeFile);
			if (!wcscmp(Wname, proc.szExeFile)) countproc++;
			if (countproc > 1) {
				MessageBox(NULL, L"second process", L"", MB_ICONWARNING);
				exit(-1);
			}
		} while (Process32Next(hSnap, &proc));
	}
	CloseHandle(hSnap);
	printf_s("\n----%d----\n", countproc);

	_getch();
	return 0;
}