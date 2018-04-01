
#include "Tetris.h"

BOOL bBlocks[10 * 20] = { FALSE, };
BOOL bMoves[10 * 20] = { FALSE, };
BOOL bTemps[10 * 20] = { FALSE, };

DWORD timeUpdate = 1000;

INT nScore = 0;

VOID Tetris::Draw(HDC hDC) {
	static WCHAR sz[20] = { 0, };

	MyClear(TRUE);

	// 테두리
#pragma omp parallel for
	for (int x = 0; x < 12; x++) {
		MyRectangle(17 + (x * 17), 17, 15, 15, 0);
		MyRectangle(17 + (x * 17), 17 + (21 * 17), 15, 15, 0);
	}
#pragma omp parallel for
	for (int y = 1; y < 21; y++) {
		MyRectangle(17, 17 + (y * 17), 15, 15, 0);
		MyRectangle(17 + (11 * 17), 17 + (y * 17), 15, 15, 0);
	}

	// 쌓여있는 블럭
#pragma omp parallel for
	for (int x = 0; x < 10; x++) {
#pragma omp parallel for
		for (int y = 0; y < 20; y++) {
			if (bBlocks[y * 10 + x]) {
				MyRectangle(34 + (x * 17), 34 + (y * 17), 15, 15, MyRGB(RGB(255, 0, 0)));
			}
		}
	}

	// 움직일 수 있는 블럭
#pragma omp parallel for
	for (int x = 0; x < 10; x++) {
#pragma omp parallel for
		for (int y = 0; y < 20; y++) {
			if (bMoves[y * 10 + x]) {
				MyRectangle(34 + (x * 17), 34 + (y * 17), 15, 15, MyRGB(RGB(0, 0, 255)));
			}
		}
	}

	MyFinish();

	wsprintfW(sz, L"점수: %d점", nScore);

	TextOutW(hDC, 224, 17, sz, lstrlenW(sz));

	return;
}

VOID Tetris::UpdateBlock(BOOL bForce) {
	static DWORD timePrev = timeGetTime();
	
	BOOL bSuccess = TRUE;

	if (timeGetTime() - timePrev >= timeUpdate || bForce) {  // 갱신 속도
		memset(bTemps, 0, sizeof(BOOL) * 20 * 10);  // 임시 공간은 일단 FALSE로 초기화한다.
		timePrev = timeGetTime();  // 시간을 초기화한다.

		for (int x = 0; x < 10; x++) {  // 마지막 행에 한 개라도 있을 경우
			if (bMoves[19 * 10 + x]) {
				bSuccess = FALSE;
				break;
			}

		}

		// 일단 한 칸 내린다.
		if (bSuccess) {
			memcpy(bTemps + 10, bMoves, sizeof(BOOL) * 10 * 19);
		}

		for (int x = 0; x < 10; x++) {
			if (!bSuccess) {
				break;
			}
			for (int y = 0; y < 20; y++) {
				// 겹치는 블럭이 존재할 경우
				if (bBlocks[y * 10 + x] && bTemps[y * 10 + x]) {
					bSuccess = FALSE;
					break;
				}
			}
		}

		if (bSuccess) {  // 내리기에 성공했을 경우
			memcpy(bMoves, bTemps, sizeof(BOOL) * 10 * 20);  // 같게 만든다.(복사)
		} else {  // 실패했을 경우
			INT nLine = 0;
			for (int x = 0; x < 10 * 20; x++) {
				if (bMoves[x]) {
					bBlocks[x] = TRUE;
				}
			}
			memset(bMoves, 0, sizeof(BOOL) * 10 * 20);  // 움직이는 블럭을 삭제한다.

			for (int y = 0; y < 20; y++) {
				for (int x = 0; x < 10; x++) {
					if (!bBlocks[y * 10 + x]) {  // 블럭이 한 개라도 없으면
						break;  // 빠져나온다.
					} else if (x == 9) {  // 마지막 까지 꽉 차있을경우
						bSuccess = TRUE;
						nLine = y;
					}
				}
			}

			if (bSuccess) {
				nScore += 1;

				// 한 줄을 통째로 삭제한다.(FALSE으로 만든다.)
				memset(bBlocks + nLine * 10 + 0, 0, sizeof(BOOL) * 10);

				// 미리 한 칸 내려 움직일 수 있는 블럭으로 만든다.
				memcpy(bMoves + 10, bBlocks, sizeof(BOOL) * 10 * nLine);
				// 움직이는데 방해가 되지 않도록 기존 블럭들은 삭제한다.
				memset(bBlocks, 0, sizeof(BOOL) * 10 * nLine);

				// 블럭을 굳힌다.
				UpdateBlock(TRUE);
			} else {
				CreateBlock();
			}
		}
	}

	return;
}

VOID Tetris::MoveBlocksLeft(VOID) {
	BOOL bSuccess = TRUE;

	memset(bTemps, 0, sizeof(BOOL) * 20 * 10);

	for (int y = 0; y < 20; y++) {
		// 가장 왼쪽에 움직일 수 있는 블럭이 있을 경우
		if (bMoves[y * 10 + 0]) {
			bSuccess = FALSE;
			break;
		}
	}

	for (int x = 1; x < 10 * 20; x++) {
		bTemps[x - 1] = bMoves[x];  // 왼 쪽으로 한 칸 이동합니다.
		if (bBlocks[x - 1] && bTemps[x - 1]) {
			bSuccess = FALSE;
			break;
		}
	}

	if (bSuccess) {
		memcpy(bMoves, bTemps, sizeof(BOOL) * 10 * 20);
	}

	return;
}

VOID Tetris::MoveBlocksRight(VOID) {
	BOOL bSuccess = TRUE;

	memset(bTemps, 0, sizeof(BOOL) * 20 * 10);

	for (int y = 0; y < 20; y++) {
		// 가장 오른쪽에 움직일 수 있는 블럭이 있을 경우
		if (bMoves[y * 10 + 9]) {
			bSuccess = FALSE;
			break;
		}
	}

	for (int x = 0; x < 10 * 20 - 1; x++) {
		bTemps[x + 1] = bMoves[x];  // 왼 쪽으로 한 칸 이동합니다.
		if (bBlocks[x + 1] && bTemps[x + 1]) {
			bSuccess = FALSE;
			break;
		}
	}

	if (bSuccess) {
		memcpy(bMoves, bTemps, sizeof(BOOL) * 10 * 20);
	}

	return;
}

VOID Tetris::CreateBlock(VOID) {
	INT nRandom = rand() % 6;  // 블럭의 갯수만큼

	switch (nRandom) {
	case 0:  // 사각형 모양
		bMoves[0 * 10 + 4] = TRUE;
		bMoves[0 * 10 + 5] = TRUE;
		bMoves[1 * 10 + 4] = TRUE;
		bMoves[1 * 10 + 5] = TRUE;
		break;
	case 1:  // ㄴ 모양
		bMoves[0 * 10 + 3] = TRUE;
		bMoves[1 * 10 + 3] = TRUE;
		bMoves[1 * 10 + 4] = TRUE;
		bMoves[1 * 10 + 5] = TRUE;
		break;
	case 2:  // ㄱㄴ 모양
		bMoves[0 * 10 + 3] = TRUE;
		bMoves[0 * 10 + 4] = TRUE;
		bMoves[1 * 10 + 4] = TRUE;
		bMoves[1 * 10 + 5] = TRUE;
		break;
	case 3:  // ㄱㄴ 역모양
		bMoves[0 * 10 + 5] = TRUE;
		bMoves[0 * 10 + 4] = TRUE;
		bMoves[1 * 10 + 4] = TRUE;
		bMoves[1 * 10 + 3] = TRUE;
		break;
	case 4:  // ㄴ 역모양
		bMoves[0 * 10 + 3] = TRUE;
		bMoves[0 * 10 + 4] = TRUE;
		bMoves[0 * 10 + 5] = TRUE;
		bMoves[1 * 10 + 3] = TRUE;
		break;
	case 5:  // ㅡ 모양
		bMoves[0 * 10 + 3] = TRUE;
		bMoves[0 * 10 + 4] = TRUE;
		bMoves[0 * 10 + 5] = TRUE;
		bMoves[0 * 10 + 6] = TRUE;
		break;
	}

	return;
}


VOID Tetris::RotateBlocks(VOID) {
	int X = 10, Y = 20, CX = 0, CY = 0;

	memset(bTemps, 0, sizeof(BOOL) * 10 * 20);

	// 우선 둘러싸는 사각형을 구한다.
	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 20; y++) {
			if (bMoves[y * 10 + x]) {
				X = X < x ? X : x;
				Y = Y < y ? Y : y;
				CX = CX > x ? CX : x;
				CY = CY > y ? CY : y;
			}
		}
	}

	// 정사각형으로 만든다.
	if (CX - X > CY - Y) {
		CY = Y + (CX - X);
	} else if (CY - Y > CX - X) {
		CX = X + (CY - Y);
	}

	// 벽에 닿을 경우 실패
	if (CX >= 10 || CY >= 20) {
		return;
	}

	// 회전을 시작한다.
	for (int x = X; x <= CX; x++) {
		for (int y = Y; y <= CY; y++) {
			bTemps[(Y + (x - X)) * 10 + CX - (y - Y)] = bMoves[y * 10 + x];
		}
	}

	// 충돌하는 블럭이 있는지 검사한다.
	for (int x = X; x <= CX; x++) {
		for (int y = Y; y <= CY; y++) {
			if (bTemps[y * 10 + x] && bBlocks[y * 10 + x]) {
				return;
			}
		}
	}

	memcpy(bMoves, bTemps, sizeof(BOOL) * 10 * 20);
	
	return;
}