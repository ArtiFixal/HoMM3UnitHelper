#include "MemoryReader.h"

// init static
MemoryReader* MemoryReader::hookThisPtr = NULL;
HHOOK MemoryReader::mouseHook = NULL;

MemoryReader::MemoryReader()
{
	processID=NULL;
	processHandler=NULL;
	window = NULL;
	hookThisPtr = this;
	gamePaused = true;
}

MemoryReader::~MemoryReader()
{
	if (processHandler)
		CloseHandle(processHandler);
	if (mouseHook)
		UnhookWindowsHookEx(mouseHook);
}

bool MemoryReader::attachToProcess()
{
	// Prevent opening new handlers if one exists
	if (processHandler != NULL)
		return true;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshotHandler = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,NULL);
	if (Process32First(snapshotHandler, &entry))
	{
		while (Process32Next(snapshotHandler, &entry))
		{
			if (wcscmp(entry.szExeFile,HOTA_PROC_NAME)==0)
			{
				processID = entry.th32ProcessID;
				break;
			}
		}
	}
	CloseHandle(snapshotHandler);
	if (this->processID == NULL)
		return false;
	// Give minimal access rights
	processHandler=OpenProcess(PROCESS_VM_READ|PROCESS_QUERY_LIMITED_INFORMATION, false, processID);
	return processHandler != NULL&&processHandler!=INVALID_HANDLE_VALUE;
}

/*uint MemoryReader::getModuleAddress(const wchar_t* moduleName)
{
	uint moduleAddress = NULL;
	HANDLE snapshotHandler = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE| TH32CS_SNAPMODULE32,processID);
	MODULEENTRY32 entry;
	entry.dwSize = sizeof(MODULEENTRY32);
	if (Module32First(snapshotHandler, &entry))
	{
		do
		{
			if (wcscmp(entry.szModule,moduleName)==0)
			{
				moduleAddress = (uint)entry.modBaseAddr;
				break;
			}
		} while (Module32Next(snapshotHandler, &entry));
	}
	CloseHandle(snapshotHandler);
	return moduleAddress;
}*/

size_t MemoryReader::readStringSize(int stringStartAddress)
{
	// Prevent from reading entire memory in search for \0
	const size_t MAX_READ = 129;
	for (size_t i = 0; i < MAX_READ; i++)
	{
		// End of string
		if (readMemory<H3Unit::byte>(stringStartAddress + i) == 0)
			return i+1;
	}
	return 0;
}

string MemoryReader::readString(int stringStartAddress)
{
	size_t l = readStringSize(stringStartAddress);
	if (l == 0)
		return "";
	string s="";
	// Preallocate string to avoid its resizing and moving it multiple times in loop.
	s.get_allocator().allocate(l+1);
	char c;
	for (size_t i = 0; i < l; i++)
	{
		ReadProcessMemory(processHandler, (LPCVOID)(stringStartAddress+i), &c, 1, NULL);
		s += c;
	}
	return s;
}

bool MemoryReader::readUnit(H3Unit::byte unitID)
{
	selectedUnit = reader.readUnit(unitID);
	return !selectedUnit.isNull();
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
						if (hookThisPtr->readString(BOTTOM_BAR_TEXT).find("-") != string::npos)
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
	if (processHandler)
	{
		CloseHandle(processHandler);
		processHandler = NULL;
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

bool MemoryReader::resetDataReader()
{
	return reader.reloadFile();
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
	if (processHandler == NULL)
		return false;
	DWORD status;
	GetExitCodeProcess(processHandler, &status);
	return status==STILL_ACTIVE;
}