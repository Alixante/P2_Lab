#include <filesystem>
#include "board.h"
#include "candy.h"
#include "controller.h"
#include "game.h"
#include "util.h"

void printFallo(std::string texto)
{
    cout << "No se ha pasado el test " <<texto << std::endl;
}



bool testPropio()
{
    bool pasadoTests = true;
    //Para probar setCell y getCell
    Candy c(CandyType::TYPE_ORANGE);
    Board tauler1(10, 10);
    int valorX = 0;
    int valorY = 0;
    tauler1.setCell(&c, valorX, valorY);
    if (tauler1.getCell(valorX, valorY) != &c)
    {
        cout << "No se ha pasado el test getCell(" << valorX << "," << valorY << ")" << std::endl;
        pasadoTests = false;
    }

    valorX = 12;
    valorY = 12;

    if (tauler1.getCell(valorX, valorY) != nullptr)
    {
        cout << "No se ha pasado el test getCell(" << valorX << "," << valorY << ") que deberia dar nullptr" << std::endl;
        pasadoTests = false;
    }

    Candy c2(CandyType::TYPE_RED);
    tauler1.setCell(&c2, 20, 20);
    if (tauler1.getCell(20, 20) != nullptr)
    {
        cout << "No se ha pasado el test setCell fuera de rango" << std::endl;
        pasadoTests = false;
    }

    //para hacer getWidth y getHeight
    if (tauler1.getWidth() != 10 || tauler1.getHeight() != 10)
    {
        cout << "No se ha pasado el test getWidth" << std::endl;
        pasadoTests = false;
    }


    

    //para shouldExplode
    Board tauler2(10, 10);
    Candy vermell(CandyType::TYPE_RED);
    tauler2.setCell(&vermell, 0, 0);
    tauler2.setCell(&vermell, 1, 0);
    tauler2.setCell(&vermell, 2, 0);
    if (!tauler2.shouldExplode(1, 0))
    {
        pasadoTests = false;
    }

    //negativos creo
    Board tauler2b(10, 10);
    Candy azul(CandyType::TYPE_BLUE);
    tauler2b.setCell(&azul, 0, 0);
    tauler2b.setCell(&azul, 1, 0);
    if (tauler2b.shouldExplode(0, 0))
    {
        cout << "No se ha pasado el test shouldExplode negativoo" << std::endl;
        pasadoTests = false;
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
        pasadoTests = false;
    }

    //negativo
    Board tauler3b(10, 10);
    Candy amarillo(CandyType::TYPE_YELLOW);
    tauler3b.setCell(&amarillo, 3, 3);
    tauler3b.setCell(&amarillo, 5, 5);
    auto exploded2 = tauler3b.explodeAndDrop();
    if (exploded2.size() != 0)
    {
        cout << "No se ha pasado el test explodeAndDrop ngativo" << std::endl;
        pasadoTests = false;
    }

    //para dump y load
    Board tauler4(5, 5);
    Candy green(CandyType::TYPE_GREEN);
    tauler4.setCell(&green, 0, 0);
    if (!tauler4.dump(getDataDirPath() + "test.txt"))
    {
        pasadoTests = false;
    }

    Board tauler5(0, 0);
    if (!tauler5.load(getDataDirPath() + "test.txt"))
    {
        pasadoTests = false;
    }

    if (tauler5.getCell(0, 0)->getType() != CandyType::TYPE_GREEN)
    {
        pasadoTests = false;
    }

    std::filesystem::remove(getDataDirPath() + "test.txt");

    //negativo
    Board tauler6(5, 5);
    if (tauler6.load(getDataDirPath() + "noexiste.txt"))
    {
        cout << "No se ha pasado el test load sina rxivo" << std::endl;
        pasadoTests = false;
    }

    return pasadoTests;
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
