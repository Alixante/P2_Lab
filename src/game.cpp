#include "game.h"
#include <random>
#include "graphics.h"
#include "candy.h"
#include "board.h"

Game::Game()
{
    // Implement your code here
}

Game::~Game()
{
    // Implement your code here
}

void Game::update(const Controller& controller)
{
    static Board tablero;
    static int cursorX = 0;
    static int cursorY = 0;
    static int result = 0;

    // Implement your code here
    if (controller.isDownPressed())
    {
        if (cursorY < tablero.getHeight() - 1)
        {
            ++cursorY;
        }
    }
    if (controller.isUpPressed())
    {
        if (cursorY > 0) //No tienes sentido, odio mi vida
    }
   
}

void Game::render(GraphicManager& graphics)
{
    // Implement your code here
    std::random_device rd; //Genera semilla aleatoria
    std::mt19937 gen(rd()); //Cargar semilla

    std::uniform_int_distribution<int> dist(0, (int)(CandyType::COUNT) - 1); //Parametros de que a que Candy, 0-5

    CandyType caramelos[3]; 


    for (int i = 0; i < 3; i++)
    {
        caramelos[i] = (CandyType)(dist(gen)); //Ejecuta rand()
    }

    const int board_size = 10;
    const int board_padding = 3;
    graphics.drawRectangle(
        CANDY_IMAGE_HEIGHT * board_padding, CANDY_IMAGE_HEIGHT * board_padding,
        CANDY_IMAGE_WIDTH * board_size,
        CANDY_IMAGE_HEIGHT * board_size,
        5, 150, 150, 150);

    for (int j = 0; j < 3; j++)
    {
        graphics.drawImage(Candy(caramelos[j]).getResourceName(), CANDY_IMAGE_WIDTH * 3, CANDY_IMAGE_HEIGHT * 3);
    }

    // Title [draw images]
    graphics.drawImage("img/logo_small.png", 10, 10);
    // Score and footer [draw text]
    graphics.drawText("Movement: [Up] [Down] [Left] [Right]  --  "
        "Buttons: [Q] [W] [E]  --  Exit [ESC]",
        25, 700, 20, 100, 100, 100);
    graphics.drawText("Score: ", 450, 10, 70, 125, 200, 125);
}

void Game::run()
{
    const int screen_width = 750;
    const int screen_height = 750;
    const int bg_red = 255;
    const int bg_green = 255;
    const int bg_blue = 255;
    runGraphicGame(*this, screen_width, screen_height, bg_red, bg_green, bg_blue);
}

bool Game::dump(const std::string& output_path) const
{
    // Implement your code here
    return false;
}

bool Game::load(const std::string& input_path)
{
    // Implement your code here
    return false;
}

bool Game::operator==(const Game& other) const
{
    // Implement your code here
    return false;
}