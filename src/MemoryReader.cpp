#include "MemoryReader.h"

// init static
MemoryReader* MemoryReader::hookThisPtr = NULL;
HHOOK MemoryReader::mouseHook = NULL;

MemoryReader::MemoryReader(): reader(nullptr)
{
	processID=NULL;
	processHandle=NULL;
	window = NULL;
	hookThisPtr = this;
	gamePaused = true;
}

MemoryReader::~MemoryReader()
{
	if (processHandle)
		CloseHandle(processHandle);
	if (mouseHook)
		UnhookWindowsHookEx(mouseHook);
}

bool MemoryReader::attachToProcess()
{
	// Prevent opening new handlers if one exists
	if (processHandle != NULL)
		return true;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,NULL);
	if (Process32First(snapshotHandle, &entry))
	{
		while (Process32Next(snapshotHandle, &entry))
		{
			if (TEXTCMP(entry.szExeFile,HOTA_PROC_NAME)==0)
			{
				processID = entry.th32ProcessID;
				// Get path to the exe directory
				MODULEENTRY32 processModule;
				processModule.dwSize=sizeof(MODULEENTRY32);
				HANDLE processSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,processID);
				Module32First(processSnapshot,&processModule);
				exeDir=processModule.szExePath;
				if(exeDir.has_filename())
					exeDir=exeDir.remove_filename();
				CloseHandle(processHandle);
				std::filesystem::path hotaDataFile=exeDir;
				// exeDir have "\\" at the end
				reader=std::make_unique<DataReader>(exeDir.concat("Data\\HotA_lng.lod"),
					hotaDataFile.concat("HotA.dat"));
				break;
			}
		}
	}
	CloseHandle(snapshotHandle);
	if (this->processID == NULL)
		return false;
	// Give minimal access rights
	processHandle=OpenProcess(PROCESS_VM_READ|PROCESS_QUERY_LIMITED_INFORMATION, false, processID);
	return processHandle != NULL&&processHandle!=INVALID_HANDLE_VALUE;
}

string MemoryReader::readString(int stringStartAddress)
{
	const size_t BUFF_SIZE=128;
	char buff[BUFF_SIZE];
	ReadProcessMemory(processHandle, (LPCVOID)stringStartAddress, buff, BUFF_SIZE, NULL);
	return string(buff,BUFF_SIZE);
}

string MemoryReader::readString(int stringStartAddress, size_t bytesToRead)
{
	char* buff=new char[bytesToRead];
	ReadProcessMemory(processHandle, (LPCVOID)stringStartAddress, buff, bytesToRead, NULL);
	return string(buff,bytesToRead);
}

bool MemoryReader::readUnit(int unitID)
{
	selectedUnit = reader->readUnit(unitID);
	return !selectedUnit.isNull();
}

std::filesystem::path MemoryReader::getExePath()
{
	return exeDir;
}

DWORD MemoryReader::getProcessID()
{
	return processID;
}

Unit MemoryReader::getUnit()
{
	return selectedUnit;
}

bool MemoryReader::setMouseHook()
{
	return (mouseHook=SetWindowsHookEx(WH_MOUSE_LL, staticProcessMouseHook, NULL,NULL));
}

LRESULT CALLBACK MemoryReader::staticProcessMouseHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		if (wParam == WM_RBUTTONUP)
		{
			if (hookThisPtr != NULL)
			{
				// Process clicks only inside our game window
				if(hookThisPtr->window==GetForegroundWindow())
				{
					// Prevent from random monsters being read if not on adventure map (ex. in town)
					if (!hookThisPtr->gamePaused)
					{
						// Only monster string contain '-' char
						if (hookThisPtr->readString(BOTTOM_BAR_TEXT,16).find("-") != string::npos)
						{
							int monster = hookThisPtr->readMemory<int>(MONSTER_ID_ADDRESS);
							// Prevent reading file twice for the same unit
							if (monster != hookThisPtr->selectedUnit.getUnitID())
								hookThisPtr->readUnit(monster);
						}
					}
				}
			}
		}
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

bool MemoryReader::findWindow()
{
	return findWindow(this->processID);
}

bool MemoryReader::findWindow(DWORD processID)
{
	EnumWindows(findWindowCall, processID);
	return window != NULL;
}

BOOL CALLBACK MemoryReader::findWindowCall(HWND hwnd, LPARAM lParam)
{
	if (hookThisPtr->processID==NULL)
		return FALSE;
	DWORD pid = 0;
	GetWindowThreadProcessId(hwnd, &pid);
	if (pid == lParam)
	{
		hookThisPtr->window = hwnd;
		return FALSE;
	}
	return TRUE;
}

HWND MemoryReader::getWindow()
{
	return window;
}

void MemoryReader::clearMemoryReader()
{
	if (processHandle)
	{
		CloseHandle(processHandle);
		processHandle = NULL;
	}
	if (mouseHook)
	{
		UnhookWindowsHookEx(mouseHook);
		window = NULL;
	}
	processID = NULL;
	// Reset last selected unit with default constructor
	selectedUnit = Unit();
}

bool MemoryReader::pollGamePause()
{
	gamePaused = readMemory<int>(IS_GAME_PAUSED);
	return gamePaused;
}

bool MemoryReader::isGamePaused()
{
	return gamePaused;
}

bool MemoryReader::isGameStillRunning()
{
	if (processHandle == NULL)
		return false;
	DWORD status;
	GetExitCodeProcess(processHandle, &status);
	return status==STILL_ACTIVE;
}