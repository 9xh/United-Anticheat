#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <tchar.h>
#include <stdio.h>

#pragma warning (disable : 6276 ) // cast warning idk
#pragma warning (disable : 4267 ) // loss of data warning (i should probably fix this but im to lazy)

namespace fs = std::filesystem;

std::wstring widen(const std::string& s)
{
	std::vector<wchar_t> buf(MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.size() + 1, 0, 0));

	MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.size() + 1, &buf[0], buf.size());

	return std::wstring(&buf[0]);
}

struct ProcessParams
{
	std::string exeName;
	std::string exeArguments = " ";
	const char* dllPath = "";
	bool useCommandLine = false; // this may be useless i have to test.
	DWORD creationFlags = CREATE_NEW_CONSOLE;
	bool exeIsFullPath = false;
	bool dllIsFullPath = false;
};

void NewProcess(const ProcessParams& params) // this took me too long.
{
	// TODO: Add path checking to see if it exists/is a dll/is a exe.
	
	// Setting wpath and path to the full path

	std::string path = params.exeName;
	std::string args = params.exeArguments;
	const char* dllPath = params.dllPath;
	bool useCommandLine = params.useCommandLine;
	DWORD creationFlags = params.creationFlags;
	bool fullDirectory = params.exeIsFullPath;

	if (path.find(".exe") == std::string::npos) path = path + ".exe"; // path.append(".exe");

	std::wstring wpath;
	fs::path fspath = fs::current_path() / path;
	if (!fullDirectory)
	{
		path = fspath.string();
		wpath = widen(path);
		// std::cout << fspath.string() << std::endl;
	}

	LPCWSTR finalPath = NULL;
	if (!useCommandLine)
	{
		finalPath = const_cast<LPCWSTR>(wpath.c_str());
		// creationFlags = CREATE_NO_WINDOW; // TODO: Test this lol
	}
	else args = path + " " + args; // args.insert(0, path);

	
	// Make sure arguments isn't nothing.
	
	std::wstring wargs = widen(args);
	if (wargs.size() != 0)
	{
		if (wargs[0] != L' ') wargs.insert(0, L" ");
	}

	// memory stuff

	wchar_t* wcp = new wchar_t[wargs.size() + 1];
	const wchar_t* temp = wargs.c_str();
	wcscpy_s(wcp, wargs.size() + 1, temp);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcessW(finalPath,
		wcp,
		NULL,
		NULL,
		FALSE,
		creationFlags,
		NULL,
		NULL,
		&si,
		&pi)
		)
	{
		std::cout << "Couldn't create process " + fspath.filename().string() + " and the error code is " << GetLastError() << std::endl;
	}
	else
	{
		// if (fs::exists(dllPath)) // Inject DLL
		WaitForSingleObject(pi.hProcess, INFINITE);
		// std::cout << GetExitCodeProcess(pi.hProcess, idk what to put here) << std::endl;
		delete[]wcp; // we may be able to do this earlier but i havent tested.
		wcp = 0;
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

int main(int argc, char* argv[])
{
	ProcessParams params;
	params.exeName = "LPWSTR Testing.exe";
	params.exeArguments = "Testing";
	NewProcess(params);
	std::cin.get();
}