#include "board.h"
#include <memory>
#include <iostream>
#include <random>
#include <fstream>


char candyTypeToChar(CandyType c)
{
	char result;
	switch (c)
	{
	case(CandyType::TYPE_RED):
		result = 'R';
		break;
	case(CandyType::TYPE_BLUE):
		result = 'B';
		break;
	case(CandyType::TYPE_GREEN):
		result = 'G';
		break;
	case(CandyType::TYPE_ORANGE):
		result = 'O';
		break;
	case(CandyType::TYPE_PURPLE):
		result = 'P';
		break;
	case(CandyType::TYPE_YELLOW):
		result = 'Y';
		break;
	default:
		result = 'X';
		break;
	}
	return result;
}

bool checkFile(std::ifstream& fitxer, std::string nomFitxer)
{
	if (!fitxer.is_open())
	{
		std::cout << "No s'ha pogut obrir el fitxer: " << nomFitxer << std::endl;
		return 0;
	}
	//He bscat com fer-ho i sembla ser que es el millor, primer mira el primer caracter del arxiu i si es el EOF, ho diu
	else if (fitxer.peek() == EOF)
	{
		std::cout << "El fitxer està buit: " << nomFitxer << std::endl;
		return 0;
	}

	else if (!fitxer.good())
	{
		std::cout << "Fitxer not good" << std::endl;
		return 0;
	}
	return 1;
}
bool checkFile(std::ofstream& fitxer, std::string nomFitxer)
{
	if (!fitxer.is_open())
	{
		std::cout << "No s'ha pogut obrir el fitxer: " << nomFitxer << std::endl;
		return 0;
	}
	else if (fitxer.eof())
	{
		std::cout << "El fitxer està buit: " << nomFitxer << std::endl;
		return 0;
	}
	else if (!fitxer.good())
	{
		std::cout << "Fitxer not good" << std::endl;
		return 0;
	}
	return 1;

}

CandyType charToCandYType(char c)
{
	CandyType result;
	switch (c)
	{
	case('R'):
		result = CandyType::TYPE_RED;
		break;
	case('B'):
		result = CandyType::TYPE_BLUE;
		break;
	case('G'):
		result = CandyType::TYPE_GREEN;
		break;
	case('O'):
		result = CandyType::TYPE_ORANGE;
		break;
	case('P'):
		result = CandyType::TYPE_PURPLE;
		break;
	case('Y'):
		result = CandyType::TYPE_YELLOW;
		break;
	default:
		result = CandyType::TYPE_YELLOW;
		break;
	}
	return result;
}

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
	std::ofstream fitxer(output_path);
	if (!checkFile(fitxer, "dump"))
	{
		return false;
	}
	fitxer << m_width << ' ' << m_height << std::endl;
	for (int x = 0; x < m_width; x++)
	{
		for (int y = 0; y < m_height; y++)
		{
			Candy* c = m_tauler[x][y];
			if (c != nullptr)
			{
				char typeChar = candyTypeToChar(c->getType());
				fitxer << typeChar;
			}
			else
			{
				fitxer << 'N';
			}
		}
		fitxer << std::endl;
	}


	return fitxer.good();
}

bool Board::load(const std::string& input_path)
{
	std::ifstream arxiu(input_path);
	if (!checkFile(arxiu, "load"))
	{
		return false;
	}

	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			m_tauler[i][j] = nullptr;

		}
	}

	arxiu >> m_width >> m_height;
	std::cout << m_width << " " << m_height << std::endl;

	for (int x = 0; x < m_width; x++)
	{
		for (int y = 0; y < m_height; y++)
		{
			char candyChar;
			arxiu >> candyChar;
			CandyType result;
			if (candyChar != 'N')
			{
				CandyType candyType = charToCandYType(candyChar);
				Candy c(candyType);
				m_tauler[x][y] = &c;
				
			}
			std::cout << candyChar;
			
		}
		std::cout << std::endl;
	}
	return true;
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


