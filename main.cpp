#include <windows.h>
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include "MemoryReader.h"
#include <filesystem>
#include "resource.h"

/* 
* App defined error codes:
* 
* Windows API error codes are 32 - bit values(bit 31 is the most significant bit). 
* Bit 29 is reserved for application - defined error codes; no system error code has this bit set.
*/

// Game window not found - can't init overlay.
#define ERROR_OVERLAY_INIT DWORD(1<<29)

// Can't setup mouse hook
#define ERROR_OVERLAY_MOUSE_HOOK_INIT DWORD(1<<29+1)

// File containing unit data doesn't exist
#define ERROR_UNIT_DATA_FILE_NOT_FOUND DWORD(1<<29+2)

// URL to file containing data about units
#define UNIT_DATA_FILE_URL "https://github.com/ArtiFixal/HoMM3UnitHelper/raw/main/data/unitData.hdat"

using namespace std;

struct overlayStyle {
	float opacity=0.692f;
};

static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void showOverlay(MemoryReader &mr,overlayStyle style)
{
	ImGuiIO& io = ImGui::GetIO();

	// Prevent from merging overlay into menu
	io.ConfigViewportsNoAutoMerge = true;
	ImGuiWindowFlags overlayFlags= ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	ImGuiWindowClass wc;

	// Request platform backend to create parent <> child relationship
	wc.ParentViewportId = 1;
	wc.ViewportFlagsOverrideSet = ImGuiViewportFlags_IsPlatformWindow;
	wc.ViewportFlagsOverrideSet = ImGuiViewportFlags_OwnedByApp;

	ImGui::SetNextWindowClass(&wc);
	if (ImGui::Begin("HoMM3 Unit Helper Overlay", NULL, overlayFlags))
	{
		Unit u = mr.getUnit();
		if(mr.pollGamePause())
			ImGui::Text("Game paused.");
		else if (u.isNull())
			ImGui::Text("Nothing selected");
		else
		{
			ImGui::Text("%s", u.getName().c_str());
			ImGui::Text("Faction: %s", u.getFactionString().c_str());
			ImGui::Text("Unit Level: %s", u.getLevelString().c_str());
			ImGui::Text("Attack: %i", u.getAttack());
			ImGui::Text("Defense: %i", u.getDefense());
			if (u.isRanged())
				ImGui::Text("Ammo: %i", u.getAmmo());
			ImGui::Text("Damage: %i-%i", u.getMinDmg(), u.getMaxDmg());
			ImGui::Text("Health: %i", u.getHP());
			ImGui::Text("Speed: %i", u.getSpeed());
			if (!u.isNeutral())
			{
				ImGui::Text("If on native terrain: %s", u.getFactionTerrain().c_str());
				ImGui::Text("+1 to attack, defense and speed");
			}
		}
	}

	// Each Viewport has its own HWND so we will use it
	HWND win = (HWND)ImGui::GetWindowViewport()->PlatformHandleRaw;
	{
		RECT gamePos = {NULL};
		::GetWindowRect(mr.getWindow(), &gamePos);
		ImGui::SetWindowPos(ImVec2(500, 500));
		SetWindowLongPtr(win, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_LAYERED);
		SetLayeredWindowAttributes(win, NULL, (BYTE)(style.opacity*255), LWA_ALPHA);

		// Check if the game is in a fullscreen mode, since HoMM3 always 
		// fullscreens to the main monitor we can assume that left=top=0 means we are in fullscreen
		if(gamePos.left==0&&gamePos.top==0)
			ImGui::SetWindowPos(ImVec2(0, 0));
		else
			ImGui::SetWindowPos(ImVec2(gamePos.left + 8, gamePos.top + 51));
		::SetWindowPos(win, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
	ImGui::End();
	// Restore default merging behaviour outside overlay
	io.ConfigViewportsNoAutoMerge = false;
}

int main()
{
	// Create application window
	WNDCLASSEX wc = {sizeof(wc),CS_CLASSDC,WndProc,0L,0L,GetModuleHandle(NULL),NULL,NULL,NULL,NULL,TEXT("HoMM3 Unit Helper")};
	wc.hIcon=LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	::RegisterClassEx(&wc);
	HWND appWindow = ::CreateWindow(wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW, 100, 100, 640, 340, NULL, NULL, wc.hInstance, NULL);

	// Initialize Direct3D
	if (!CreateDeviceD3D(appWindow))
	{
		CleanupDeviceD3D();
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
		return 1;
	}
	::ShowWindow(appWindow, SW_SHOWDEFAULT);
	::UpdateWindow(appWindow);

	// Setup Dear ImGui context
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	io.IniFilename = NULL;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	
	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(appWindow);
	ImGui_ImplDX9_Init(g_pd3dDevice);

	// Our variables
	MemoryReader mr = MemoryReader();
	bool attached=false;
	bool dataFileExists = false;
	bool initOverlay = false;
	DWORD lastError = NULL;
	overlayStyle oStyle;
	const ImVec4 errColor = ImVec4(0.89f,0.259f,0.204f,1);
	const ImVec4 okColor = ImVec4(0.196f,0.8f,0.196f,1);
	ImGuiWindowFlags mainWindowFlags = ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_MenuBar|ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoDocking;

	// Main loop
	bool done = false;
	while (!done)
	{
		// Poll and handle messages (inputs, window resize, etc.)
		MSG msg;
		while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				done = true;
		}
		if (done)
			break;

		// Start the Dear ImGui frame
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// Fullscreen ImGui main window in a host window
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos, ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize,ImGuiCond_Always);
		ImGui::Begin("Menu",NULL,mainWindowFlags);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				//ImGui::Checkbox("Auto-attach on start",&autoAttach);
				ImGui::SliderFloat("Opacity",&oStyle.opacity,0.0f,1.0f);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		if (!attached)
			ImGui::TextColored(errColor, "Not attached");
		else
		{
			ImGui::TextColored(okColor, "Succesfully attached");
			if (!mr.isGameStillRunning())
			{
				mr.clearMemoryReader();
				initOverlay = false;
				attached = false;
			}
		}
		if (ImGui::Button("Attach"))
		{
			if (attached = mr.attachToProcess())
			{
				if (!initOverlay)
				{
					if (mr.findWindow())
					{
						if (mr.setMouseHook())
						{
							lastError = NULL;
							initOverlay = true;
						}
						else
							lastError = ERROR_OVERLAY_MOUSE_HOOK_INIT;
					}
					else
						lastError = ERROR_OVERLAY_INIT;
				}
			}
			else
				lastError = GetLastError();
		}
		if (lastError != NULL)
		{
			switch (lastError)
			{
			case ERROR_ACCESS_DENIED:
				ImGui::TextColored(errColor, "Attach failed: No privileges to attach.");
				ImGui::TextColored(errColor, "Restart game and launcher as regular user or run the helper as administrator.");
				break;
			case ERROR_NO_MORE_FILES:
				ImGui::TextColored(errColor, "Attach failed: Game process not found.");
				break;
			case ERROR_OVERLAY_INIT:
				ImGui::TextColored(errColor, "Overlay initialize failed: Game window not found.");
				break;
			case ERROR_OVERLAY_MOUSE_HOOK_INIT:
				ImGui::TextColored(errColor, "Overlay initialize failed: Can't setup mouse hook.");
				break;
			case ERROR_UNIT_DATA_FILE_NOT_FOUND:
				ImGui::TextColored(errColor, "Overlay initialize failed: Data file not found.");
					if (!dataFileExists)
					{
						ImGui::TextColored(errColor, "Tried to download unit data file but failed.");
						ImGui::TextColored(errColor,"Manually download file from:");
						ImGui::TextColored(errColor,"%s", UNIT_DATA_FILE_URL);
						if (ImGui::Button("Open link"))
							system(string("start ").append(UNIT_DATA_FILE_URL).c_str());
						ImGui::TextColored(errColor, "And drop it inside this program .exe directory.");
					}
					else
						ImGui::TextColored(okColor, "Successfully downloaded unit data file.");
				break;
			default:
				ImGui::TextColored(errColor, "Attach failed: Error: %i", lastError);
				break;
			}
		}
		ImGui::End();
		if (initOverlay)
		{
			// Draw overlay only when window is focused
			if(GetForegroundWindow() == mr.getWindow())
				showOverlay(mr,oStyle);
		}

		// Rendering
		ImGui::EndFrame();
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		D3DCOLOR clear_col_dx = D3DCOLOR_RGBA(0,0,0,255);
		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
		if (g_pd3dDevice->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			g_pd3dDevice->EndScene();
		}

		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
		HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

		// Handle loss of D3D9 device
		if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			ResetDevice();
	}

	// Exit from GUI
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	CleanupDeviceD3D();
	::DestroyWindow(appWindow);
	::UnregisterClass(wc.lpszClassName, wc.hInstance);
	return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return false;
	// Create the D3DDevice
	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.hDeviceWindow = hWnd;
	g_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;// Need to use an explicit format with alpha if needing per-pixel alpha composition.
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;// Present with vsync
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
		return false;
	return true;
}

void CleanupDeviceD3D()
{
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
	if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			g_d3dpp.BackBufferWidth = LOWORD(lParam);
			g_d3dpp.BackBufferHeight = HIWORD(lParam);
			ResetDevice();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	case WM_DPICHANGED:
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
		{
			const RECT* suggested_rect = (RECT*)lParam;
			::SetWindowPos(hWnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
		}
		break;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
