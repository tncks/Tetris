///
/// System.h
/// 시스템을 시동합니다.
///
#pragma once

#include <Windows.h>
#include <tchar.h>

int __stdcall _tWinMain(_In_ HINSTANCE p_hInstance, _In_ HINSTANCE p_hPrevInstance, _In_ TCHAR *p_pszLine, _In_ int p_nShow);
LRESULT __stdcall WndProc(_In_ HWND p_hWnd, _In_ UINT p_iMessage, _In_ WPARAM p_wParam, _In_ LPARAM p_lParam);

HRESULT __stdcall OnPaint(_In_ HWND p_hWnd);