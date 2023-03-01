#pragma once
#include <windows.h>
#include <TlHelp32.h>
#include"DataReader.h"
#include"Unit.h"

using namespace std;

#define HOTA_PROC_NAME L"h3hota HD.exe"

// Can change after update. FFFF FFFF if hero opened, 0
#define MONSTER_ID_ADDRESS 0x00199AD0

// If town is opened =1, if heroes are trading >40
#define IS_TOWN_OPENED 0x00199AD4

// Text displayed at the bottom of the map
#define BOTTOM_BAR_TEXT 0x00697428

/**
* Process memory I/O.
*/
class MemoryReader
{
public:
	// Last clicked unit
	Unit selectedUnit;

	MemoryReader();
	~MemoryReader();

	/**
	* @return Success of an attach.
	*/
	bool attachToProcess();

	/**
	* Reads memory from given address.
	*/
	template <typename T> T readMemory(LPCVOID address);

	/**
	* Reads memory from given address.
	*/
	template <typename T> T readMemory(int address);

	/**
	* Counts the size of a string from given memory address.
	*
	* @param stringStartAddress Memory address from which we count our string size.
	* @return String size.
	*/
	size_t readStringSize(int stringStartAddress);

	/**
	* Reads entire string from given memory address.
	* 
	* @param stringStartAddress Memory address from which we read.
	*/
	string readString(int stringStartAddress);

	/**
	* Read given unit data from a file.
	* 
	* @param unitID Which unit data we want to read.
	* @return Success of the read.
	*/
	bool readUnit(H3Unit::byte unitID);

	DWORD getProcessID();

	/**
	* @return Mouse clicked unit.
	*/
	Unit getUnit();

	/**
	* Setup mouse hook for the overlay.
	* 
	* @return Success of the setup.
	*/
	bool setMouseHook();

	/**
	* Processes mouse actions in the hook.
	*/
	static LRESULT CALLBACK staticProcessMouseHook(int nCode, WPARAM wParam, LPARAM lParam);

	/**
	* Find window HWND of processID.
	*/
	bool findWindow();

	/**
	* Find window HWND of given PID.
	* 
	* @param Process ID of window HWND we want to find.
	*/
	bool findWindow(DWORD processID);

	/**
	* Callback for WindowEnumerate to find.
	*/
	static BOOL CALLBACK findWindowCall(HWND hwnd, LPARAM lParam);

	HWND getWindow();

	/**
	* Clears MemoryReader object variables, hooks and handlers.
	*/
	void clearMemoryReader();
	bool resetDataReader();
	bool isGameStillRunning();

private:
	// Game PID
	DWORD processID;
	HANDLE processHandler;
	DataReader reader;
	// Hook processing mouse events
	static HHOOK mouseHook;

	// Pointer to this object used in hook callbacks
	static MemoryReader* hookThisPtr;

	// Game window
	HWND window;
};

template<typename T>
inline T MemoryReader::readMemory(LPCVOID address)
{
	T data;
	ReadProcessMemory(processHandler, address, &data, sizeof(T),NULL);
	return data;
}

template<typename T>
inline T MemoryReader::readMemory(int address)
{
	return readMemory<T>((LPCVOID)address);
}
