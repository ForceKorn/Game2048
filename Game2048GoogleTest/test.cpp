#include "pch.h"
#include "Board.h"
#include <tuple>
#include <span>

inline constexpr int GAME_WIN_VALUE = 2048;

TEST(Game2048, BoardValidMoveDown)
{
	int before[16] = {
		2, 8, 2, 4,
		0, 4, 2, 0,
		0, 0, 2, 0,
		2, 2, 2, 0
	};
	Board b(GAME_WIN_VALUE, 4, 4);
	b.setBoard(before);
	b.move('s');

	std::tuple<int, bool> after[16] = {
		 std::make_tuple(0, false), std::make_tuple(0, false), std::make_tuple(0, false), std::make_tuple(0, false),
		 std::make_tuple(0, false), std::make_tuple(8,  true), std::make_tuple(0, false), std::make_tuple(0, false),
		 std::make_tuple(0, false), std::make_tuple(4,  true), std::make_tuple(4,  true), std::make_tuple(0, false),
		 std::make_tuple(4,  true), std::make_tuple(2,  true), std::make_tuple(4,  true), std::make_tuple(4,  true),
	};
	EXPECT_TRUE(b.fuzzyEqual(after));
}

TEST(Game2048, BoardValidMoveUp)
{
	int before[16] = {
		2, 2, 2, 0,
		0, 0, 2, 0,
		0, 4, 2, 0,
		2, 8, 2, 4,
	};
	Board b(GAME_WIN_VALUE, 4, 4);
	b.setBoard(before);
	b.move('w');

	std::tuple<int, bool> after[16] = {
		std::make_tuple(4,  true), std::make_tuple(2,  true), std::make_tuple(4,  true), std::make_tuple(4,  true),
		std::make_tuple(0, false), std::make_tuple(4,  true), std::make_tuple(4,  true), std::make_tuple(0, false),
		std::make_tuple(0, false), std::make_tuple(8,  true), std::make_tuple(0, false), std::make_tuple(0, false),
		std::make_tuple(0, false), std::make_tuple(0, false), std::make_tuple(0, false), std::make_tuple(0, false),
	};
	EXPECT_TRUE(b.fuzzyEqual(after));
}

TEST(Game2048, BoardValidMoveLeft)
{
	int before[16] = {
		0, 0, 0, 4,
		2, 2, 2, 2,
		2, 0, 4, 8,
		2, 0, 0, 2,
	};
	Board b(GAME_WIN_VALUE, 4, 4);
	b.setBoard(before);
	b.move('a');

	std::tuple<int, bool> after[16] = {
		std::make_tuple(4, true),std::make_tuple(0, false), std::make_tuple(0, false), std::make_tuple(0, false),
		std::make_tuple(4, true),std::make_tuple(4,  true), std::make_tuple(0, false), std::make_tuple(0, false),
		std::make_tuple(2, true),std::make_tuple(4,  true), std::make_tuple(8,  true), std::make_tuple(0, false),
		std::make_tuple(4, true),std::make_tuple(0, false), std::make_tuple(0, false), std::make_tuple(0, false),
	};
	EXPECT_TRUE(b.fuzzyEqual(after));
}

TEST(Game2048, BoardValidMoveRight)
{
	int before[16] = {
		4, 0, 0, 0,
		2, 2, 2, 2,
		8, 4, 0, 2,
		2, 0, 0, 2,
	};
	Board b(GAME_WIN_VALUE, 4, 4);
	b.setBoard(before);
	b.move('d');

	std::tuple<int, bool> after[16] = {
		std::make_tuple(0, false), std::make_tuple(0, false), std::make_tuple(0, false), std::make_tuple(4, true),
		std::make_tuple(0, false), std::make_tuple(0, false), std::make_tuple(4,  true), std::make_tuple(4, true),
		std::make_tuple(0, false), std::make_tuple(8,  true), std::make_tuple(4,  true), std::make_tuple(2, true),
		std::make_tuple(0, false), std::make_tuple(0, false), std::make_tuple(0, false), std::make_tuple(4, true),
	};
	EXPECT_TRUE(b.fuzzyEqual(after));
}

TEST(Game2048, BoardIsNotFull)
{
	Board b(GAME_WIN_VALUE, 4, 4);
	int nonFail[16] = {
		32, 16, 128, 16,
		16, 32, 16, 32,
		32, 16, 32, 64,
		16, 32, 64, 0
	};
	b.setBoard(nonFail);
	b.move('s');
	EXPECT_TRUE(b.canMove());
}

TEST(Game2048, GameLostByNoMoves)
{
	Board b(GAME_WIN_VALUE, 4, 4);
	int failed[16] = {
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 10, 11, 12,
		13, 15, 14, 16
	};
	b.setBoard(failed);
	EXPECT_TRUE(b.isFull());
	EXPECT_FALSE(b.canMove());
}

TEST(Game2048, GameHasMovesInFullBoard)
{
	Board b(GAME_WIN_VALUE, 4, 4);
	int hasMoves[16] = {
		2,		8,		2,		2,
		2,		16,		4,		16,
		8,		2,		8,		2,
		32,		128,	256,	64
	};
	b.setBoard(hasMoves);
	EXPECT_TRUE(b.canMove());

	b.move('a');
	EXPECT_TRUE(b.canMove());
}

TEST(Game2048, GameVictory)
{
	Board b(GAME_WIN_VALUE, 4, 4);
	int hasMoves[16] = {
		2,		8,		2,		2,
		2,		16,		4,		16,
		8,		2,		8,		2,
		32,		128, 1024,   1024
	};
	b.setBoard(hasMoves);
	EXPECT_TRUE(b.canMove());
	EXPECT_TRUE(b.isFull());
	EXPECT_FALSE(b.reachedVictoryValue());

	b.move('d');
	EXPECT_TRUE(b.canMove());
	EXPECT_TRUE(b.reachedVictoryValue());
}