#ifndef CANDYBLOCK_H
#define CANDYBLOCK_H

#include "board.h"
#include <iostream>



struct CandyCoords
{
    Candy* candy;
    coords coord;

};

class CandyBlock
{
public:
    CandyBlock();
    CandyCoords getCandyBlock(int i) const { return m_candyBlock[i]; }

    void setBlock();
    void posicionarStartBlock(int startX, int startY);

    void rotateBlocks();

    void dropBlock();
    void moveRight();
    void moveLeft();


    bool leftFree(Board &board);
    bool rightFree(Board &board);
    bool bottomFree(Board &board);

    bool gameLost(Board &board);

    friend std::ostream& operator<<(std::ostream& os, const CandyBlock& block);
    friend std::istream& operator>>(std::istream& is, CandyBlock& block);

private:
    CandyCoords m_candyBlock[3];
    int droppedTimes;

};
#endif
