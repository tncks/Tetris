#pragma once

#include "Graphics.h"

class Tetris {
public:
	static VOID Draw(HDC hDC);
	static VOID UpdateBlock(BOOL bForce);
	static VOID MoveBlocksLeft(VOID);
	static VOID MoveBlocksRight(VOID);
	static VOID RotateBlocks(VOID);
	static VOID CreateBlock(VOID);
};

