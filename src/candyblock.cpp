#include "candyblock.h"

#include <random>
#include "graphics.h"
#include "candy.h"
#include "board.h"


CandyBlock::CandyBlock()
{
    droppedTimes = 0;
}

void CandyBlock::setBlock()
{
    std::random_device rd; //Genera semilla aleatoria
    std::mt19937 gen(rd()); //Cargar semilla

    std::uniform_int_distribution<int> dist(0, (int)(CandyType::COUNT)-1); //Parametros de que a que Candy, 0-5

    for (int i = 0; i < 3; i++)
    {
        m_candyBlock[i].candy = new Candy((CandyType)(dist(gen)));
    }

}
//Habra que pasar width/2 aproximado y height a x y y 
void CandyBlock::posicionarStartBlock(int startX, int startY)
{
    int Y = startY;

    for (int i = 0; i < 3; i++)
    {
        m_candyBlock[i].coord.x = startX;
        m_candyBlock[i].coord.y = Y -= 1;
    }

    // (int)width / 2, height + 2 (i = 2)
    // (int)width / 2, height + 1 (i = 1)
    // (int)width / 2, height     (i = 0)

}


void CandyBlock::dropBlock()
{
    for (int i = 0; i < 3; i++)
    {
        m_candyBlock[i].coord.y += 1;
        droppedTimes++;
    }
}

void CandyBlock::moveRight()
{
    for (int i = 0; i < 3; i++)
    {
        m_candyBlock[i].coord.x += 1;
    }
}

void CandyBlock::moveLeft()
{
    for (int i = 0; i < 3; i++)
    {
        m_candyBlock[i].coord.x -= 1;
    }
}

/*void CandyBlock::rotateBlocks()
{
    int carameloAbajo, carameloMedio, carameloArriba;

    if (m_candyBlock[2].coord.y > m_candyBlock[1].coord.y)
    {
        if (m_candyBlock[2].coord.y > m_candyBlock[0].coord.y)
        {
            carameloArriba = m_candyBlock[2].coord.y;
            carameloMedio = m_candyBlock[1].coord.y;
            carameloAbajo = m_candyBlock[0].coord.y;

            m_candyBlock[0].coord.y = carameloArriba;
            m_candyBlock[2].coord.y = carameloMedio;
            m_candyBlock[1].coord.y = carameloAbajo;


        }
        else
        {
            carameloArriba = m_candyBlock[0].coord.y;
            carameloMedio = m_candyBlock[2].coord.y;
            carameloAbajo = m_candyBlock[1].coord.y;

            m_candyBlock[1].coord.y = carameloArriba;
            m_candyBlock[0].coord.y = carameloMedio;
            m_candyBlock[2].coord.y = carameloAbajo;

        }
    }
    else
    {
        carameloArriba = m_candyBlock[1].coord.y;
        carameloMedio = m_candyBlock[0].coord.y;
        carameloAbajo = m_candyBlock[2].coord.y;

        m_candyBlock[2].coord.y = carameloArriba;
        m_candyBlock[1].coord.y = carameloMedio;
        m_candyBlock[0].coord.y = carameloAbajo;

    }


    /*
    A       B
    B ----> C
    C       A
    

    //2
    //1
    //0

}
*/

void CandyBlock::rotateBlocks()
{

    Candy* candy0 = m_candyBlock[0].candy;
    Candy* candy1 = m_candyBlock[1].candy;
    Candy* candy2 = m_candyBlock[2].candy;

    Candy* temp0;
    Candy* temp1;
    Candy* temp2;

    temp0 = candy0;
    temp1 = candy1;
    temp2 = candy2;

    candy0 = temp1;
    candy1 = temp2;
    candy2 = temp0;

    m_candyBlock[0].candy = candy0;
    m_candyBlock[1].candy = candy1;
    m_candyBlock[2].candy = candy2;

    /*
    A       B
    B ----> C
    C       A
    */

    //2 C2    A0    B1
    //1 B1 -> C2 -> A0
    //0 A0    B1    C2

}

bool CandyBlock::leftFree(Board &m_board)
{
    for (int i = 0; i < 3; i++)
    {
        coords coord = m_candyBlock[i].coord;
        int lx = coord.x - 1;
        if (lx < 0)
        {
            return false;
        }
        // Only check board occupancy for rows that are actually inside the board
        if (coord.y >= 0 && coord.y < m_board.getHeight())
        {
            if (m_board.getCell(lx, coord.y) != nullptr)
            {
                return false;
            }
        }
    }
    return true;
}

bool CandyBlock::rightFree(Board &m_board)
{
    for (int i = 0; i < 3; i++)
    {
        coords coord = m_candyBlock[i].coord;
        int rx = coord.x + 1;
        if (rx >= m_board.getWidth())
        {
            return false;
        }

        if (coord.y >= 0 && coord.y < m_board.getHeight())
        {
            if (m_board.getCell(rx, coord.y) != nullptr)
            {
                return false;
            }
        }
    }
    return true;
}

bool CandyBlock::bottomFree(Board& m_board)
{
    for (int i = 0; i < 3; i++)
    {
        coords coord = m_candyBlock[i].coord;
        int ny = coord.y + 1;

        // If the next row would be outside the board (i.e., below last row), not free
        if (ny >= m_board.getHeight())
        {
            return false;
        }

        // Only check occupancy if the cell below is actually inside the board
        if (ny >= 0 && ny < m_board.getHeight())
        {
            if (m_board.getCell(coord.x, ny) != nullptr)
            {
                return false;
            }
        }
    }

    return true;
}

bool CandyBlock::gameLost(Board &m_board)
{
    for (int i = 0; i < 3; ++i)
    {
        if (m_candyBlock[i].coord.y >= m_board.getHeight())
        {
            return true;
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const CandyBlock& block)
{
    for (int i = 0; i < 3; ++i)
    {
        int candyType = static_cast<int>(block.m_candyBlock[i].candy->getType());
        os << candyType << ' ' << block.m_candyBlock[i].coord.x << ' ' << block.m_candyBlock[i].coord.y << ' ';
    }
    return os;
}

std::istream& operator>>(std::istream& is, CandyBlock& blockCandy)
{
    for (int i = 0; i < 3; ++i)
    {
        int candyType;
        int x, y;

        if (!(is >> candyType))
        {
            return is;
        }
        if (!(is >> x))
        {
            return is;
        }
        if (!(is >> y))
        {
            return is;
        }

        if (blockCandy.m_candyBlock[i].candy != nullptr)
        {
            delete blockCandy.m_candyBlock[i].candy;
        }
        blockCandy.m_candyBlock[i].candy = new Candy(static_cast<CandyType>(candyType));
        blockCandy.m_candyBlock[i].coord.x = x;
        blockCandy.m_candyBlock[i].coord.y = y;
    }
    return is;
}