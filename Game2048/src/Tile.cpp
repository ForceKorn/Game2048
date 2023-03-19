#include "Tile.h"

int Tile::getValue() const {
	return value;
}

void Tile::setValue(int val) {
	value = val;
}

bool Tile::isEmpty() const {
	return value == 0;
}
