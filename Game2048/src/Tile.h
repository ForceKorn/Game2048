#ifndef TILE_H
#define TILE_H

#include <iosfwd>

class Tile 
{
public:
	Tile(int number = 0);
	void display(std::ostream& os);

public:
	int m_number = 0;
};

#endif // TILE_H