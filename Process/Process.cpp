///////////////////////////////////////////////////////////////////////
// Process.cpp - class used to start process                         //
// ver 1.0                                                           //
// Author : Ammar Salman, SU, assalman@syr.edu                        //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////

#include "../Logger/Logger.h"
#include "Process.h"
#include "../Utilities/StringUtilities/StringUtilities.h"

using namespace Utilities;

CBP Process::cbp_ = []() {
	Log::Debug::write("\n  --- child process exited ---");
	Log::Demo::write("\n  --- child process exited ---");
};

//----< Process constructor >------------------------------------------
/*
* - This eventually will have more functionality
*/
Process::Process()
{
	GetStartupInfo(&startInfo_);
	startInfo_.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;
	startInfo_.dwX = 200;
	startInfo_.dwY = 250;
	startInfo_.dwYSize = 300;
}

//----< define application new process runs >--------------------------

void Process::application(const std::string& appName)
{
	appName_ = sToW(appName);
}
//----< define commandline passed to new application >-----------------

void Process::commandLine(const std::string& cmdLine)
{
	commandLine_ = sToW(cmdLine);
}
//----< define title bar for console window >--------------------------
/*
* - applications like Notepad overwrite this string
*/
void Process::title(const std::string& title)
{
	title_ = sToW(title);
	startInfo_.lpTitle = const_cast<LPWSTR>(title_.c_str());
}
//----< start new child process >--------------------------------------

bool Process::create(const std::string& appName)
{
	std::wstring app;
	if (appName.size() == 0)
		app = appName_;
	else
	{
		app = sToW(appName);
	}
	LPCTSTR applic = app.c_str();
	LPTSTR cmdLine = const_cast<LPTSTR>(commandLine_.c_str());
	LPSECURITY_ATTRIBUTES prosec = nullptr;
	LPSECURITY_ATTRIBUTES thrdsec = nullptr;
	BOOL inheritHandles = false;
	DWORD createFlags = CREATE_NO_WINDOW;  // to allow JOB assignment
	LPVOID environment = nullptr;
	LPCTSTR currentPath = nullptr;
	LPSTARTUPINFO pStartInfo = &startInfo_;
	LPPROCESS_INFORMATION pPrInfo = &procInfo_;

	BOOL OK =
		CreateProcess(
			applic, cmdLine, prosec, thrdsec, inheritHandles,
			createFlags, environment, currentPath, pStartInfo, pPrInfo
		);

	return OK;
}

///////////////////////////////////////////////////////////////////////
// child process exit callback processing

//----< provide new callable object for callback to invoke >-----------

void Process::setCallBackProcessing(CBP cbp)
{
	cbp_ = cbp;
}
//----< function called when child process exists >--------------------

void Process::callback()
{
	cbp_();
}
//----< Windows API declared function type for callbacks >-------------

void CALLBACK WaitOrTimerCallback(_In_ PVOID lpParameter, _In_ BOOLEAN TimerOrWaitFired)
{
	Process p;
	p.callback();
	return;
}
//----< register callback with Windows API >---------------------------

void Process::registerCallback()
{
	HANDLE hNewHandle;
	HANDLE hProcHandle = procInfo_.hProcess;
	RegisterWaitForSingleObject(
		&hNewHandle, hProcHandle, WaitOrTimerCallback,
		NULL, INFINITE, WT_EXECUTEONLYONCE
	);
}

HANDLE Process::getProcessHandle()
{
	return procInfo_.hProcess;
}


#ifdef TEST_PROCESS

#include <iostream>
#include <conio.h>

int main()
{
	std::cout << "\n  Demonstrating code pop-up in notepad";
	std::cout << "\n ======================================";

	Process p;
	p.title("test application");
	std::string appPath = "c:/windows/system32/notepad.exe";  // path to application to start
	p.application(appPath);

	std::string cmdLine = "/A ../Process/Process.h";  // asking notepad to display Process.h
	p.commandLine(cmdLine);

	std::cout << "\n  Will start notepad 5 times and each time wait for termination.";
	std::cout << "\n  You need to kill each window (upper right button) to continue.";
	std::cout << "\n  Press key to start";
	_getche();

	for (int i = 0; i < 5; ++i)
	{
		std::cout << "\n  starting process: \"" << appPath << "\"";
		std::cout << "\n  with this cmdlne: \"" << cmdLine << "\"";
		p.create();
		CBP callback = []() { std::cout << "\n  --- child process exited with this message ---"; };
		p.setCallBackProcessing(callback);
		p.registerCallback();

		WaitForSingleObject(p.getProcessHandle(), INFINITE);  // wait for created process to terminate
	}

	std::cout << "\n  after OnExit";
	std::cout.flush();
	char ch;
	std::cin.read(&ch, 1);
	return 0;
}

#endif
