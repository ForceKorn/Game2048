#include "Game.h"

#include <iostream>
#include <string>

Game::Game(std::ostream& display, std::istream& keyboard)
	: m_board(), m_displayDevice(display), m_inputDevice(keyboard) {}

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

		const bool noMovesLeft = !m_board.canMove();
		const bool isVictory = m_board.containsValue(2048);
		if (isVictory)
		{
			handleWin();
			return;
		}

		if (noMovesLeft)
		{
			handleLose();
			return;
		}
	}
}

void Game::display()
{
	system("cls"); // clear console

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
	m_displayDevice << "Congratulations! You have reached 2048!\n";
}

void Game::handleLose() 
{
	m_displayDevice << "Game over. You lose.\n";
}