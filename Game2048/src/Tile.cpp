#include "Tile.h"
#include <iostream>

Tile::Tile(int number) : m_number(number) {}

void Tile::display(std::ostream& os)
{
	os << m_number;
}
