#include "Game.h"

#include <iostream>
#include <string>

inline constexpr int GAME_WIN_VALUE = 2048;
inline constexpr int GAME_BOARD_HEIGHT = 5;
inline constexpr int GAME_BOARD_WIDTH  = 4;

Game::Game(std::ostream& display, std::istream& keyboard) : 
	m_board(GAME_WIN_VALUE, GAME_BOARD_HEIGHT, GAME_BOARD_WIDTH),
	m_previousMoveBoards(GAME_WIN_VALUE, GAME_BOARD_HEIGHT, GAME_BOARD_WIDTH),
	m_displayDevice(display), 
	m_inputDevice(keyboard) {}

void Game::run() 
{
	reset();
	display();
	
	while (m_board.canMove())
	{
		const char direction = getUserMoveDirection();
		if (direction == 'r')
		{
			m_board = m_previousMoveBoards;
			m_previousMoveBoards = m_board;
			display();
			continue;
		}

		m_previousMoveBoards = m_board;
		m_board.move(direction);
		display();

		const bool isVictory = m_board.reachedVictoryValue();
		if (isVictory)
		{
			handleWin();
			return;
		}

		const bool noMovesLeft = !m_board.canMove();
		if (noMovesLeft)
		{
			handleLose();
			return;
		}
	}
}

void Game::display()
{
	system("cls");

	m_displayDevice << "Score: " << m_board.getScore() << '\n';
	m_board.display(m_displayDevice);
}

char Game::getUserMoveDirection()
{
	m_displayDevice << "Enter a move (w/a/s/d) or (r) to restore last move: ";
	
	std::string input;
	std::getline(m_inputDevice, input);
	return input.empty() ? char{ 0 } : input.front();
}

void Game::reset() 
{
	m_board.reset();
}

void Game::handleWin() 
{
	m_displayDevice << "Congratulations! You have reached " << GAME_WIN_VALUE << "!\n";
}

void Game::handleLose() 
{
	m_displayDevice << "Game over. You lose.\n";
}