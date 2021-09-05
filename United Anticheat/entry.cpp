#include <iostream>
#include <Windows.h>

void CreateProcessAndWait(const char* path, const char* args, const char* dllPath = "0")
{
	// TODO: Add path checking to see if it exists/is a dll/is a exe.
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	if (!CreateProcessW(reinterpret_cast<LPCWSTR>(path),
		reinterpret_cast<LPWSTR>(args),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi)
		)
		std::cout << "Could not launch " + std::string(path); // TODO: Split at the last /
	else
	{
		if(dllPath != "0") // Inject DLL
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

void main()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
    std::cout << "Hello World!\n";
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	if (!CreateProcessW(L"ac",
		reinterpret_cast<LPWSTR>("args"),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi)
		)
		std::cout << "Cannot launch Fortnite.";
	else
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}