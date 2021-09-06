#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <tchar.h>
#include <stdio.h>

#pragma warning (disable : 6276 ) // cast warning idk

namespace fs = std::filesystem;

std::wstring widen(const std::string& s)
{
	std::vector<wchar_t> buf(MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.size() + 1, 0, 0));

	MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.size() + 1, &buf[0], buf.size());

	return std::wstring(&buf[0]);
}

void CreateProcessAndWait(std::string path, std::string args = " ", const char* dllPath = "0", bool fullDirectory = false) // yes i spent 6 hours on this function and i only call it twice. do i regret it, not at all.
{
	// TODO: Add path checking to see if it exists/is a dll/is a exe.
	std::wstring wargs = widen(args);
	if (wargs.size() != 0)
	{
		if (wargs[0] != L' ')
		{
			wargs.insert(0, L" ");
		}
	}
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	std::wstring wpath;
	fs::path fspath = fs::current_path() / path;
	if (!fullDirectory)
	{
		path = fspath.string();
		wpath = widen(path);
		std::cout << fspath.string();
		//fspath = path;
	}

	wchar_t* wcp = new wchar_t[wargs.size() + 1];
	if (wcp == 0)
	{
		// stuff
	}
	const wchar_t* temp = wargs.c_str();
	wcscpy_s(wcp, wargs.size() + 1, temp);

	if (!CreateProcessW(const_cast<LPCWSTR>(wpath.c_str()),
		wcp, // to print it out just cast it to a const char*
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi)
		)
	{
		std::cout << "Could not launch " + fspath.filename().string() + " Error: " << GetLastError() << std::endl;
	}
	else
	{
		if (dllPath != "0"); // Inject DLL
		WaitForSingleObject(pi.hProcess, INFINITE);
		delete[]wcp; // we may be able to do this earlier but i havent tested.
		wcp = 0;
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

int main()
{
	CreateProcessAndWait("LPWSTR Testing.exe");
	CreateProcess
	std::cin.get();
}