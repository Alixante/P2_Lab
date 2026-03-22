#include "board.h"
#include <memory>
#include <iostream>
#include <random>

Board::Board(int width, int height) //Hecho
{
	if (width <= 0)
	{
		width = DEFAULT_BOARD_WIDTH;
	}

	if (height <= 0)
	{
		height = DEFAULT_BOARD_HEIGHT;
	}

	m_width = width;
	m_height = height;

	m_tauler.resize(width, std::vector<Candy*>(height));
}
std::to_int
Board::~Board()
{

}


Candy* Board::getCell(int x, int y) const
{
	if (!insideBoard(x, y))
	{
		return nullptr;
	}

	Candy* cela = m_tauler[x][y];
	if (cela != nullptr)
	{
		return cela;
	}
	return nullptr;
}

void Board::setCell(Candy* candy, int x, int y)
{
	if (!insideBoard(x, y))
	{
		return;
	}

	m_tauler[x][y] = candy;

}


int Board::getWidth() const //Hecho
{
	return m_width;
}


int Board::getHeight() const //Hecho
{
	return m_height;
}

bool Board::shouldExplode(int x, int y) const
{

	Candy self = m_tauler[x][y];
	int contador1 = 0, contador2 = 0;
	int var = 2;



	for (int i = 0; i < 5; i++) //Horizontal
	{
		if (self.getType() == m_tauler[x - var + i][y].getType() && x - var >= 0 && x + var <= m_width)
		{
			contador1++;			
		}
		else
		{
			contador1 = 0;
		}
	}
	
	if (contador1 == 3)
	{
		return true;
	}
	else
	{
		contador1 = 0;
	}
	
	for (int j = 0; i < 5; i++) //Horizontal
	{
		if (self.getType() == m_tauler[x][y - var + j].getType() && y - var >= 0 && y + var <= m_width)
		{
			contador1++;			
		}
		else
		{
			contador1 = 0;
		}
	}
	
	if (contador1 == 3)
	{
		return true;
	}
	else
	{
		contador1 = 0;
	}

	

	//si posicion x tiene candy rojo, si x+1 tiene tmb candy rojo contador++


	return false;
}

std::vector<Candy*> Board::explodeAndDrop()
{
	/*bool noExplosion = true;
	std::vector<Candy> explodedCandies;
	while (!noExplosion)
	{
		for (int x = 0; x < m_width; x++)
		{
			for (int y = 0; y < m_heigh; y++)
			{
				if (shouldExplode(x, y))
				{
					explodedCandies.push_back(m_tauler[x][y]);
					m_tauler[x][y] = nullptr;
					noExplode = false;

				}
			}
		}
	}
	*/
	bool exploded = true;
	std::vector<Candy> explodedCandies;
	do
	{
		// Es verifica quins han d'explotar i es torna les coordenades en el tauler
		std::vector<coords> explosions = returnExplosions();
		if (explosions.size() == 0)
		{
			exploded = false;
			return explodedCandies;
		}
	
		//Eliminar els que hagin explotat
		for (int i = 0; i < explosions.size(); i++)
		{
			coords coord = explosions[i];
			m_tauler[coord.x][coord.y] = nullptr;
		}



	} while (exploded)

	return explodedCandies;
}

struct coords
{
	int x;
	int y;
};
std::vector<coords> Board::returnExplosions()
{
	std::vector<coords> result;
	for (int x = 0; x < m_width; x++)
	{
		for (int y = 0; y < m_heigh; y++)
		{
			if (shouldExplode(x, y)
			{
				coords position;
				position.x = x;
				position.y = y;
				result.push_back(position);
			}
		}
	}
	return result;
}


void Board::dropCandies()
{
	for (int x = 0; x < m_width; x++)
	{
		for (int y = m_height; y < m_height; y++)
		{
			
		}
	}
}

bool Board::dump(const std::string& output_path) const
{
	// Implement your code here
	return false;
}

bool Board::load(const std::string& input_path)
{
	// Implement your code here
	return false;
}

bool Board::insideBoard(int x, int y)
{
	if (x < m_width && x >= 0 && y < m_height && y >= 0)
	{
		return true;
	}
	return false;
}


