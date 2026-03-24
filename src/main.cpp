/// Project entry point
#include <iostream>
#include "game.h"
#include "../test/test.h"


int main(int argc, const char* argv[])
{
    
    if (!test())
    {
        std::cout << "Tests NO superados. Los has definido ya?" << std::endl;
    }
    else
    {
        std::cout << "Test superados." << std::endl;
    }
    int resultat_test = test_propio();
    if (resultat_test != -1)
    {
        cout << "Error en test " << resultat_test << std::endl;
    }
    else
    {
        cout << "Se han pasado todos los test nuestros custom" << std::endl;
    }

    // The following code runs the graphic part
    Game game;
    // Run until ESC is pressed
    game.run();
    

    //Loading and dumping tests

    return 0;
}
