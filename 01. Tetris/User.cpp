///
/// User.cpp
/// 화면을 표시하는 작업은 OnPaint에, 메시지 처리는 WndProc에서 진행합니다.
///


#include "System.h"
#include <stdio.h>

#pragma comment(lib, "winmm.lib")

HDC hDC = NULL;
HBITMAP hBack = NULL;
HBITMAP hOld = NULL;
HDC hCom = NULL;

BYTE *pMem = NULL;

BOOL bStart = FALSE;

#include "TetrisTitle.h"
#include "Tetris.h"

HRESULT __stdcall OnPaint(_In_ HWND p_hWnd) {
	HRESULT hResult = S_OK;

	static const UINT iTestCount = 500;

	static DWORD timePrev = timeGetTime();
	static UINT iCount = 0;
	static CHAR timeStr[20] = { 0, };

	if (bStart) {
		Tetris::UpdateBlock(FALSE);
		Tetris::Draw(hCom);
	} else {
		Title::Draw(hCom);
	}

	if (iCount == iTestCount) {  // FPS 계산
		sprintf_s<20>(timeStr, "%f", 1000.f / static_cast<float>((timeGetTime() - timePrev)) * static_cast<float>(iTestCount));
		iCount = 0;
		timePrev = timeGetTime();
	}

	iCount++;

	TextOutA(hCom, 0, 0, timeStr, strnlen_s(timeStr, 20));
	BitBlt(hDC, 0, 0, 800, 600, hCom, 0, 0, SRCCOPY);

	return S_OK;
}

LRESULT __stdcall WndProc(_In_ HWND p_hWnd, _In_ UINT p_iMessage, _In_ WPARAM p_wParam, _In_ LPARAM p_lParam) {
	try {
		switch (p_iMessage) {
		case WM_CREATE:
			hDC = GetDC(p_hWnd);
			hCom = CreateCompatibleDC(hDC);
			hBack = CreateCompatibleBitmap(hDC, 800, 600);
			hOld = reinterpret_cast<HBITMAP>(SelectObject(hCom, hBack));
			pMem = reinterpret_cast<BYTE *>(malloc(800 * 600 * 4));  // 800 * 600 픽셀에 ARGB(Alpha는 사용하지 않음.)
			Tetris::CreateBlock();
			break;
		case WM_MOUSEMOVE:
			if (!bStart) {
				Title::CheckButton(p_lParam);
			}
			break;
		case WM_LBUTTONDOWN:
			if (!bStart) {
				switch (Title::CheckButtonClick()) {
				case 1:
					bStart = TRUE;
					break;
				case 2:
					DestroyWindow(p_hWnd);
					break;	
				}
			}
			break;
		case WM_KEYDOWN:
			if (p_wParam == VK_ESCAPE && bStart) {
				bStart = FALSE;
			} else if (p_wParam == VK_DOWN) {
				Tetris::UpdateBlock(TRUE);
			} else if (p_wParam == VK_LEFT) {
				Tetris::MoveBlocksLeft();
			} else if (p_wParam == VK_RIGHT) {
				Tetris::MoveBlocksRight();
			} else if (p_wParam == VK_UP) {
				Tetris::RotateBlocks();
			}
			break;
		case WM_DESTROY:
			SelectObject(hCom, hOld);
			DeleteObject(hBack);
			DeleteDC(hCom);
			ReleaseDC(p_hWnd, hDC);
			PostQuitMessage(0);
			break;
		}
	} catch(_In_ HRESULT p_hResult) {
		switch(p_hResult) {
		case E_INVALIDARG:
			MessageBox(p_hWnd, TEXT("함수에 전달된 매개 변수가 잘못되었습니다."), TEXT("HRESULT = E_INVALIDARG"), MB_OK | MB_ICONERROR);
			break;
		case E_OUTOFMEMORY:
			MessageBox(p_hWnd, TEXT("메모리의 부족으로 필요한 부분이 제대로 생성되지 않았습니다."), TEXT("HRESULT = E_OUTOFMEMORY"), MB_OK | MB_ICONERROR);
			break;
		case E_ACCESSDENIED:
			MessageBox(p_hWnd, TEXT("프로그램에서 사용할 수 없는 영역을 침입했습니다."), TEXT("HRESULT = E_ACCESSDENIED"), MB_OK | MB_ICONERROR);
			break;
		case E_FAIL:
			MessageBox(p_hWnd, TEXT("어떠한 이유로 프로그램이 실행되는 데 실패하였습니다."), TEXT("HRESULT = E_FAIL"), MB_OK | MB_ICONERROR);
			break;
		}
		PostMessage(p_hWnd, WM_DESTROY, NULL, NULL);
	} catch(...) {
		MessageBox(p_hWnd, TEXT("프로시저에서 알 수 없는 예외가 발견되었습니다.\n프로그램을 종료합니다."), TEXT("알 수 없는 예외 발생."), MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	return DefWindowProc(p_hWnd, p_iMessage, p_wParam, p_lParam);
}