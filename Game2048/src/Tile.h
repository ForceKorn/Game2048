#ifndef TILE_H
#define TILE_H

class Tile 
{
public:
	Tile();
	explicit Tile(int val);

	int getValue() const;
	void setValue(int val);
	bool isEmpty() const;

private:
	int value;
};

#endif // TILE_H