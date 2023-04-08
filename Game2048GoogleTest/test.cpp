#include "pch.h"
#include "Board.h"

TEST(Game2048, BoardTestIsNotFull)
{
	Board b;
	int nonFail[16] = {
		32, 16, 128, 16,
		16, 32, 16, 32,
		32, 16, 32, 64,
		16, 32, 64, 0
	};
	b.setBoard(nonFail);
	b.move('s');
	EXPECT_EQ(b.canMove(), true);

	//EXPECT_EQ(1, 1);
	//EXPECT_TRUE(true);
}

TEST(Game2048, BoardTestIsFull)
{
	Board b;
	int failed[16] = {
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 10, 11, 12,
		13, 15, 14, 16
	};
	b.setBoard(failed);
	EXPECT_EQ(b.canMove(), false);
}

TEST(Game2048, BoardHasMoves)
{
	Board b;
	int hasMoves[16] = {
		2,		8,		2,		2,
		2,		16,		4,		16,
		8,		2,		8,		2,
		32,		128,	256,	64
	};
	b.setBoard(hasMoves);
	EXPECT_EQ(b.canMove(), true);

	b.move('a');
	EXPECT_EQ(b.canMove(), true);
}