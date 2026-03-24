#include <filesystem>
#include "board.h"
#include "candy.h"
#include "controller.h"
#include "game.h"
#include "util.h"
int test_propio()
{
    //Para probar setCell y getCell
    Candy c(CandyType::TYPE_ORANGE);
    Board tauler1(10, 10);
    tauler1.setCell(&c, 0, 0);
    if (tauler1.getCell(0, 0) != &c)
    {

        return 1;
    }

    // para hacer getWidth y getHeight
    if (tauler1.getWidth() != 10 || tauler1.getHeight() != 10)
    {
        return 2;
    }

    // para shouldExplode
    Board tauler2(10, 10);
    Candy vermell(CandyType::TYPE_RED);
    tauler2.setCell(&vermell, 0, 0);
    tauler2.setCell(&vermell, 1, 0);
    tauler2.setCell(&vermell, 2, 0);
    if (!tauler2.shouldExplode(1, 0))
    {
        return 3;
    }
    //para explodeAndDrop
    Board tauler3(10, 10);
    Candy blau(CandyType::TYPE_BLUE);
    tauler3.setCell(&blau, 5, 5);
    tauler3.setCell(&blau, 5, 6);
    tauler3.setCell(&blau, 5, 7);
    auto exploded = tauler3.explodeAndDrop();
    if (exploded.size() != 3)
    {
        return 4;
    }

    // para dump y load
    Board tauler4(5, 5);
    Candy green(CandyType::TYPE_GREEN);
    tauler4.setCell(&green, 0, 0);
    if (!tauler4.dump(getDataDirPath() + "test.txt"))
    {
        return 5;
    }

    Board tauler5(0, 0);
    if (!tauler5.load(getDataDirPath() + "test.txt"))
    {
        return 6;
    }

    if (tauler5.getCell(0, 0)->getType() != CandyType::TYPE_GREEN)
    {
        return 7;
    }

    std::filesystem::remove(getDataDirPath() + "test.txt");


    return -1;
}

bool test()
{
    // Test board 2D container
    Candy c(CandyType::TYPE_ORANGE);
    Board b(10, 10);
    b.setCell(&c, 0, 0);
    if (b.getCell(0, 0) != &c)
    {
        return false;
    }

    // Dump and load board
    {
        Board b2(10, 10);
        if (!b.dump(getDataDirPath() + "dump_board.txt"))
        {
            return false;
        }
        if (!b2.load(getDataDirPath() + "dump_board.txt"))
        {
            return false;
        }
        if (b2.getCell(0, 0)->getType() != c.getType())
        {
            return false;
        }
        std::filesystem::remove(getDataDirPath() + "dump_board.txt");
    }

    // Dump and load game
    {
        Game g;
        Controller cont;
        g.update(cont);
        if (!g.dump(getDataDirPath() + "dump_game.txt"))
        {
            return false;
        }
        Game g2;
        if (!g2.load(getDataDirPath() + "dump_game.txt"))
        {
            return false;
        }
        if (g != g2)
        {
            return false;
        }
        std::filesystem::remove(getDataDirPath() + "dump_game.txt");
    }

    return true;
}
