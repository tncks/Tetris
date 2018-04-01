
#include "System.h"

int __stdcall _tWinMain(_In_ HINSTANCE p_hInstance, _In_ HINSTANCE p_hPrevInstance, _In_ TCHAR *p_pszLine, _In_ int p_nShow)
{
	HRESULT hResult = S_OK;

	WNDCLASSEX *pWcex = NULL;
	ATOM *pAtom = NULL;

	HWND hWnd = NULL;
	MSG *pMsg = NULL;

	TCHAR *pszApplicationName = TEXT("Windows Application");

	// Unused Parameter
	UNREFERENCED_PARAMETER(p_hPrevInstance);

	try {
		/* Setting Window */ {
			pWcex = reinterpret_cast<WNDCLASSEX *>(malloc(sizeof(WNDCLASSEX)));
			if(!pWcex) {
				hResult = E_OUTOFMEMORY;
				throw hResult;
			}

			// Set Wndclass 0
			memset(pWcex, 0, sizeof(WNDCLASSEX));

			pWcex->cbSize = sizeof(WNDCLASSEX);
			pWcex->hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
			pWcex->hCursor = LoadCursor(nullptr, MAKEINTRESOURCE(IDC_ARROW));
			pWcex->hIcon = LoadIcon(nullptr, MAKEINTRESOURCE(IDC_ICON));
			pWcex->hIconSm = pWcex->hIcon;
			pWcex->hInstance = p_hInstance;
			pWcex->lpfnWndProc = WndProc;
			pWcex->lpszClassName = pszApplicationName;

			pAtom = reinterpret_cast<ATOM *>(malloc(sizeof(ATOM)));
			if(!pAtom) {
				hResult = E_OUTOFMEMORY;
				throw hResult;
			}

			*pAtom = RegisterClassEx(pWcex);
			if(!pAtom) {
				hResult = E_INVALIDARG;
				throw hResult;
			}

			// Create Window
			hWnd = CreateWindowEx(NULL, pszApplicationName, pszApplicationName, WS_POPUP, ((GetSystemMetrics(SM_CXSCREEN) - 800) / 2), ((GetSystemMetrics(SM_CYSCREEN) - 600) / 2), 800, 600, nullptr, nullptr, p_hInstance, nullptr);
			if(!hWnd) {
				throw GetLastError();
			}
			if(ShowWindow(hWnd, p_nShow)) {
				hResult = E_FAIL;
				throw hResult;
			}

			free(pWcex);
			pWcex = NULL;
		}

		/* System Start */ {
			pMsg = reinterpret_cast<MSG *>(malloc(sizeof(MSG)));
			if(!pMsg) {
				hResult = E_OUTOFMEMORY;
				throw hResult;
			}
			// Set Message Struct 0
			memset(pMsg, 0, sizeof(MSG));

			while(true) {
				if(PeekMessage(pMsg, nullptr, NULL, NULL, PM_REMOVE) && pMsg->message != WM_PAINT) {
					if(pMsg->message != WM_QUIT) {
						TranslateMessage(pMsg);
						DispatchMessage(pMsg);
					} else {
						// Exit Loop
						break;
					}
				} else {
					hResult = OnPaint(hWnd);
					if(FAILED(hResult))
						throw hResult;
				}
			}

			free(pMsg);
			pMsg = NULL;
		}

		free(pAtom);
		pAtom = NULL;

		if(IsWindow(hWnd))
			DestroyWindow(hWnd);
		hWnd = NULL;
	} catch(_In_ HRESULT p_hResult) {
		UNREFERENCED_PARAMETER(p_hResult);

		switch(hResult) {
		case E_INVALIDARG:
			MessageBox(hWnd, TEXT("�Լ��� ���޵� �Ű� ������ �߸��Ǿ����ϴ�."), TEXT("HRESULT = E_INVALIDARG"), MB_OK | MB_ICONERROR);
			break;
		case E_OUTOFMEMORY:
			MessageBox(hWnd, TEXT("�޸��� �������� �ʿ��� �κ��� ����� �������� �ʾҽ��ϴ�."), TEXT("HRESULT = E_OUTOFMEMORY"), MB_OK | MB_ICONERROR);
			break;
		case E_ACCESSDENIED:
			MessageBox(hWnd, TEXT("���α׷����� ����� �� ���� ������ ħ���߽��ϴ�."), TEXT("HRESULT = E_ACCESSDENIED"), MB_OK | MB_ICONERROR);
			break;
		case E_FAIL:
			MessageBox(hWnd, TEXT("��� ������ ���α׷��� ����Ǵ� �� �����Ͽ����ϴ�."), TEXT("HRESULT = E_FAIL"), MB_OK | MB_ICONERROR);
			break;
		}
		
		goto EXCEPTION_HANDLE;
	} catch(_In_ DWORD p_dwError) {
		switch(p_dwError) {
		case ERROR_SUCCESS:
			MessageBox(hWnd, TEXT("�� �� ���� ������ �߻��Ͽ����ϴ�.\n�۾� ������ �����Ͽ����� �Լ����� ���ܸ� �������ϴ�."), TEXT("Error: Success!"), MB_OK | MB_ICONERROR);
			break;
		case ERROR_CANNOT_FIND_WND_CLASS:
			MessageBox(hWnd, TEXT("������ Ŭ������ ã�� �� �����ϴ�.\nŬ���� �̸��� �߸� �Է��߰ų� ��� ������ ����� �̷������ �ʾ��� �� �ֽ��ϴ�."), TEXT("Error: Can't find Window Class!"), MB_OK | MB_ICONERROR);
			break;
		}
		
		goto EXCEPTION_HANDLE;
	} catch(...) {
		MessageBox(hWnd, TEXT("���α׷����� �� �� ���� ���ܰ� �߰ߵǾ����ϴ�.\n���α׷��� �����մϴ�."), TEXT("�� �� ���� ���� �߻�."), MB_OK | MB_ICONERROR);
		
		goto EXCEPTION_HANDLE;
	}

	if(false) {
EXCEPTION_HANDLE:		
		if(pWcex) {
			free(pWcex);
			pWcex = NULL;
		}
		if(pAtom) {
			free(pAtom);
			pAtom = NULL;
		}
		if(pMsg) {
			free(pMsg);
			pMsg = NULL;
		}
		if(IsWindow(hWnd)) {
			DestroyWindow(hWnd);
			hWnd = NULL;
		}

		return -1;
	}

	return 0;
}