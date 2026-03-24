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

Board::~Board()
{
	return;
}


Candy* Board::getCell(int x, int y) const
{
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

	Candy* self = m_tauler[x][y];
	int contador = 0;
	int const var = 2;

	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
	{
		return false;
	}


	for (int i = 0; i < 5; i++) //Horizontal
	{
		int chequea = x - var + i;

		if (self->getType() == m_tauler[chequea][y]->getType() && chequea >= 0 && chequea <= m_width)
		{
			contador++;
		}
		else
		{
			contador = 0;
		}
	}

	if (contador == 3)
	{
		return true;
	}
	else
	{
		contador = 0;
	}


	for (int j = 0; j < 5; j++) //Vertical
	{
		int chequea = y - var + j;

		if (self->getType() == m_tauler[x][chequea]->getType() && chequea >= 0 && chequea <= m_height)
		{
			contador++;
		}
		else
		{
			contador = 0;
		}
	}

	if (contador == 3)
	{
		return true;
	}
	else
	{
		contador = 0;
	}


	for (int k = 0; k < 5; k++) //Diagonal 1 "\"
	{
		int chequeaX = x - var + k;
		int chequeaY = y + var - k;

		if (self->getType() == m_tauler[chequeaX][chequeaY]->getType() && chequeaX >= 0 && chequeaY >= 0 && chequeaX <= m_width && chequeaY <= m_height)
		{
			contador++;
		}
		else
		{
			contador = 0;
		}
	}

	if (contador == 3)
	{
		return true;
	}
	else
	{
		contador = 0;
	}

	for (int l = 0; l < 5; l++) //Diagonal 2 "/"
	{
		int chequeaX = x + var - l;
		int chequeaY = y + var - l;

		if (self->getType() == m_tauler[chequeaX][chequeaY]->getType() && chequeaX >= 0 && chequeaY >= 0 && chequeaX <= m_width && chequeaY <= m_height)
		{
			contador++;
		}
		else
		{
			contador = 0;
		}
	}

	if (contador == 3)
	{
		return true;
	}
	else
	{
		contador = 0;
	}
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
	std::vector<Candy*> explodedCandies;
	do
	{
		// Es verifica quins han d'explotar i es torna les coordenades en el tauler
		std::vector<coords> explosions = returnExplosions();
		if (explosions.size() == 0)
		{
			exploded = false;
			return explodedCandies;
		}

		//Eliminar excplotats
		for (unsigned int i = 0; i < explosions.size(); i++)
		{
			coords coord = explosions[i];
			explodedCandies.push_back(m_tauler[coord.x][coord.y]);
			m_tauler[coord.x][coord.y] = nullptr;
		}

		dropCandies();



	} while (exploded);

	return explodedCandies;
}


std::vector<coords> Board::returnExplosions()
{
	std::vector<coords> result;
	for (int x = 0; x < m_width; x++)
	{
		for (int y = 0; y < m_height; y++)
		{
			if (shouldExplode(x, y))
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

void Board::dropCandies(void)
{
	// Baixar cada col
	for (int x = 0; x < m_width; ++x)
	{
		int write = m_height - 1;
		for (int read = m_height - 1; read >= 0; --read)
		{
			if (m_tauler[x][read] != nullptr)
			{
				if (write != read)
				{
					m_tauler[x][write] = m_tauler[x][read];
					m_tauler[x][read] = nullptr;
				}
				--write;
			}
		}
		for (int y = write; y >= 0; --y)
		{
			m_tauler[x][y] = nullptr;
		}
	}
}

bool Board::insideBoard(int x, int y) const
{
	if (x < m_width && x >= 0 && y < m_height && y >= 0)
	{
		return true;
	}
	return false;
}

