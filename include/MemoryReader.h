#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <filesystem>
#include <memory>
#include "DataReader.h"
#include "Unit.h"
#include "TcharMacros.h"

using namespace std;

#define HOTA_PROC_NAME TEXT("h3hota HD.exe")

// Can change after update. FFFF FFFF if hero opened, 0
#define MONSTER_ID_ADDRESS 0x00199A6C

// If town is opened =1, if heroes are trading >40
//#define IS_TOWN_OPENED 0x00199AD4

// Text displayed at the bottom of the map
#define BOTTOM_BAR_TEXT 0x00697428

// Describes if game is paused. 
// 1 - when paused (ex. game not started)
// 0 - otherwise (only in adventure map)
#define IS_GAME_PAUSED 0x00699668

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
	* Reads chunk of string from given memory address.
	* 
	* @param stringStartAddress Memory address from which we read.
	*/
	string readString(int stringStartAddress);

	/**
	 * @brief Reads string partialy count from memory address.
	 * 
	 * @param stringStartAddress Memory address from which we read.
	 * @param bytesToRead How many characters to read.
	 */
	string readString(int stringStartAddress,size_t bytesToRead);

	/**
	* Read given unit data from a file.
	* 
	* @param unitID Which unit data we want to read.
	* @return Success of the read.
	*/
	bool readUnit(int unitID);

	std::filesystem::path getExePath();

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
	* Allowing to attach properly once again.
	*/
	void clearMemoryReader();

	/**
	* Polls game status reading it from memory.
	* 
	* @return Game pause status.
	*/
	bool pollGamePause();

	/**
	* Describes if game is paused.
	* 
	* @return True if game is paused or not started yet, false otherwise.
	*/
	bool isGamePaused();
	bool isGameStillRunning();

private:
	// Game PID
	DWORD processID;
	HANDLE processHandle;
	std::unique_ptr<DataReader> reader;
	bool gamePaused;

	// Hook processing mouse events
	static HHOOK mouseHook;

	// Pointer to this object used in hook callbacks
	static MemoryReader* hookThisPtr;

	// Game window
	HWND window;

	// Game directory
	std::filesystem::path exeDir;
};

template<typename T>
inline T MemoryReader::readMemory(LPCVOID address)
{
	T data;
	ReadProcessMemory(processHandle, address, &data, sizeof(T),NULL);
	return data;
}

template<typename T>
inline T MemoryReader::readMemory(int address)
{
	return readMemory<T>((LPCVOID)address);
}
