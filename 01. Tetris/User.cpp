///
/// User.cpp
/// ȭ���� ǥ���ϴ� �۾��� OnPaint��, �޽��� ó���� WndProc���� �����մϴ�.
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

	if (iCount == iTestCount) {  // FPS ���
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
			pMem = reinterpret_cast<BYTE *>(malloc(800 * 600 * 4));  // 800 * 600 �ȼ��� ARGB(Alpha�� ������� ����.)
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
			MessageBox(p_hWnd, TEXT("�Լ��� ���޵� �Ű� ������ �߸��Ǿ����ϴ�."), TEXT("HRESULT = E_INVALIDARG"), MB_OK | MB_ICONERROR);
			break;
		case E_OUTOFMEMORY:
			MessageBox(p_hWnd, TEXT("�޸��� �������� �ʿ��� �κ��� ����� �������� �ʾҽ��ϴ�."), TEXT("HRESULT = E_OUTOFMEMORY"), MB_OK | MB_ICONERROR);
			break;
		case E_ACCESSDENIED:
			MessageBox(p_hWnd, TEXT("���α׷����� ����� �� ���� ������ ħ���߽��ϴ�."), TEXT("HRESULT = E_ACCESSDENIED"), MB_OK | MB_ICONERROR);
			break;
		case E_FAIL:
			MessageBox(p_hWnd, TEXT("��� ������ ���α׷��� ����Ǵ� �� �����Ͽ����ϴ�."), TEXT("HRESULT = E_FAIL"), MB_OK | MB_ICONERROR);
			break;
		}
		PostMessage(p_hWnd, WM_DESTROY, NULL, NULL);
	} catch(...) {
		MessageBox(p_hWnd, TEXT("���ν������� �� �� ���� ���ܰ� �߰ߵǾ����ϴ�.\n���α׷��� �����մϴ�."), TEXT("�� �� ���� ���� �߻�."), MB_OK | MB_ICONERROR);
		PostQuitMessage(0);
	}

	return DefWindowProc(p_hWnd, p_iMessage, p_wParam, p_lParam);
}