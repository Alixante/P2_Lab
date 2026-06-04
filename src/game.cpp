#include "game.h"
#include <random>
#include <fstream>
#include "graphics.h"
#include "candy.h"
#include "board.h"
#include "candyblock.h"

Game::Game()
{
    // Implement your code here
    m_blockExists = false;
    m_nFrame = 0;
    m_gameOver = false;
}

Game::~Game()
{
    // Implement your code here
}

void Game::update(const Controller& controller)
{
    static int cursorX = 0;
    static int cursorY = 0;
    static int result = 0;

    static string filePath = " data/save.txt";


    static bool endDrop = false;

    if (!m_gameOver)
    {
        if (endDrop)
        {
            std::cout << "Paquet aterritzat\n";
            vector<Candy*> exploded;
            do
            {
                exploded = m_board.explodeAndDrop();
                for (int i = 0; i < int(exploded.size()); i++)
                {
                    CandyType t = exploded[i]->getType();
                    cout << "Exploding: " << endl;
                }
                m_score += int(exploded.size());
            } while (!exploded.empty());
            endDrop = false;
        }


        if (Keyboard_GetKeyTrg(KEYBOARD_W))
        {
            if (dump(filePath))
            {
                cout << "SAVED TO PATH" << endl;
            }
            else 
            {
                cout << "FAILED TO SAVE" << endl;
            }
            
        }


        m_nFrame++;
        printPositions();
        if (!m_blockExists)
        {
            candyBlock.setBlock();
            candyBlock.posicionarStartBlock((int)(m_board.getWidth() / 2), 0);
            m_blockExists = true;
            cout << "Created block" << endl;
        }
        if (Keyboard_GetKeyTrg(KEYBOARD_Q) || controller.isUpPressed())
        {
            candyBlock.rotateBlocks();
            cout << "Rotating" << endl;
        }

        if (controller.isLeftPressed())
        {
            if (candyBlock.leftFree(m_board))
            {
                candyBlock.moveLeft();
                cout << "Moving left" << endl;
            }
        }

        else if (controller.isRightPressed())
        {
            if (candyBlock.rightFree(m_board))
            {
                candyBlock.moveRight();
                cout << "Moving right" << endl;
            }
        }

        if (!candyBlock.bottomFree(m_board))
        {
            endDrop = true;
            cout << "Bottom not free" << endl;
        }

        else
        {
            if (controller.isDownPressed())
            {
                candyBlock.dropBlock();
                cout << "Dropping cause pressed" << endl;
            }
            else if (m_nFrame > 60)
            {
                candyBlock.dropBlock();
                m_nFrame = 0;
                printPositions();
            }
        }

        if (endDrop)
        {
            // Before placing, verify placement is inside the board and cells are free.
            bool lost = false;
            for (int i = 0; i < 3; ++i)
            {
                coords c = candyBlock.getCandyBlock(i).coord;
                //verificamos que esten todas denbtroo
                if (c.x < 0 || c.x >= m_board.getWidth() || c.y < 0 || c.y >= m_board.getHeight())
                {
                    lost = true;
                    break;
                }
                if (m_board.getCell(c.x, c.y) != nullptr)
                {
                    lost = true;
                    break;
                }
            }

            if (lost)
            {
                m_gameOver = true;
                cout << "Game over" << endl;
            }
            else
            {
                for (int i = 0; i < 3; i++)
                {
                    m_board.setCell(candyBlock.getCandyBlock(i).candy,
                        candyBlock.getCandyBlock(i).coord.x,
                        candyBlock.getCandyBlock(i).coord.y);
                }
                m_blockExists = false;
            }
        }



    }
    /*
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
        if (cursorY > 0)
        {
        }//No tienes sentido, odio mi vida Hola wenas
    }
    */
   
}

void Game::render(GraphicManager& graphics)
{
    const int board_size = 10;
    const int board_padding = 3;
   
    static bool debugPositions = false;


    //Render indexes

    for (int i = 0; i < DEFAULT_BOARD_WIDTH; i++)
    {
        for (int j = 0; j < DEFAULT_BOARD_HEIGHT; j++)
        {
            if (m_board.getCell(i, j) != nullptr)
            {
                graphics.drawImage(m_board.getCell(i, j)->getResourceName(),
                    CANDY_IMAGE_WIDTH * (3 + i) + board_padding, CANDY_IMAGE_HEIGHT * (3 + j) + board_padding);
            }

            if (debugPositions)
            {
                std::string s = std::to_string(j);
                graphics.drawText(s,
                    CANDY_IMAGE_WIDTH * (3 + i) + board_padding, CANDY_IMAGE_HEIGHT * (3 + j) + board_padding);
            }
        }
    }



    graphics.drawRectangle(
        CANDY_IMAGE_HEIGHT * board_padding, CANDY_IMAGE_HEIGHT * board_padding,
        CANDY_IMAGE_WIDTH * board_size,
        CANDY_IMAGE_HEIGHT * board_size,
        5, 150, 150, 150);
        
    // Va de uno en uno, si hay
    // candy, verifica el color y lo muestra
    
    for (int i = 0; i < DEFAULT_BOARD_WIDTH; i++)
    {
        for (int j = 0; j < DEFAULT_BOARD_HEIGHT; j++)
        {
            if (m_board.getCell(i,j) != nullptr)
            {
                graphics.drawImage(m_board.getCell(i,j)->getResourceName(),
                    CANDY_IMAGE_WIDTH * (3 + i) + board_padding, CANDY_IMAGE_HEIGHT * (3 + j) + board_padding);
            }
        }
    }

    //Renderizar el block que cae
    for (int j = 0; j < 3; j++)
    {
        coords coord = candyBlock.getCandyBlock(j).coord;
        graphics.drawImage(candyBlock.getCandyBlock(j).candy->getResourceName(),
            CANDY_IMAGE_WIDTH * (3 + coord.x) + board_padding, CANDY_IMAGE_HEIGHT * (3 + coord.y) + board_padding);
    }

    // Title [draw images]
    graphics.drawImage("img/logo_small.png", 10, 10);
    // Score and footer [draw text]
    graphics.drawText("Movement: [Up] [Down] [Left] [Right]  --  "
        "Buttons: [Q] [W] [E]  --  Exit [ESC]",
        25, 700, 20, 100, 100, 100);
    graphics.drawText("Score: ", 450, 10, 70, 125, 200, 125);
    std::string scoreString = std::to_string(m_score);
    graphics.drawText(scoreString, 650, 10, 70, 125, 200, 125);

    /*

    const int board_size = 10;
    const int board_padding = 3;
    graphics.drawRectangle(
        CANDY_IMAGE_HEIGHT * board_padding, CANDY_IMAGE_HEIGHT * board_padding,
        CANDY_IMAGE_WIDTH * board_size,
        CANDY_IMAGE_HEIGHT * board_size,
        5, 150, 150, 150);
    // Board: place a candy piece
    graphics.drawImage(Candy(CandyType::TYPE_PURPLE).getResourceName(),
        CANDY_IMAGE_WIDTH * 3,
        CANDY_IMAGE_HEIGHT * 3);
    // Title [draw images]
    graphics.drawImage("img/logo_small.png", 10, 10);
    // Score and footer [draw text]
    graphics.drawText("Movement: [Up] [Down] [Left] [Right]  --  "
        "Buttons: [Q] [W] [E]  --  Exit [ESC]",
        25, 700, 20, 100, 100, 100);
    graphics.drawText("Score: ", 450, 10, 70, 125, 200, 125);
    */
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
    std::ofstream fitxer(output_path);

    if (!fitxer.is_open())
    {
        return false;
    }

    fitxer << m_gameOver << ' ' << m_score << ' ' << m_blockExists << std::endl;
    fitxer << m_board;
    fitxer << candyBlock << std::endl;

    fitxer.close();
    return fitxer.good();
}

bool Game::load(const std::string& input_path)
{
    std::ifstream fitxer(input_path);

    if (!fitxer.is_open())
    {
        return false;
    }
    fitxer >> m_gameOver;
    fitxer >> m_score;
    fitxer >> m_blockExists;
    if (!fitxer)
    {
        return false;
    }

    fitxer >> m_board;
    if (!fitxer)
    {
        return false;
    }

    fitxer >> candyBlock;
    if (!fitxer)
    {
        return false;
    }

    m_nFrame = 0;

    return true;
}

bool Game::operator==(const Game& other) const
{
    
    if (m_blockExists != other.m_blockExists || m_gameOver != other.m_gameOver ||
        m_score != other.m_score)
    {
        return false;
    }
    
    if (m_board.getWidth() != other.m_board.getWidth() || m_board.getHeight() != other.m_board.getHeight())
    {
        return false;
    }


    for (int j = 0; j < m_board.getHeight(); j++)
    {
        for (int i = 0; i < m_board.getWidth(); i++)
        {
            Candy* candy = m_board.getCell(i, j);
            Candy* candyOther = other.m_board.getCell(i, j);
            
            if ((candy == nullptr) != (candyOther == nullptr))
            {
                return false;
            }
            
            if (candy != nullptr && candy->getType() != candyOther->getType())
            {
                return false;
            }
        }
    }
    
    if (m_blockExists == true)
    {
        for (int i = 0; i < 3; i++)
        {
            CandyCoords cordenada = candyBlock.getCandyBlock(i);
            CandyCoords cordenadaOtra = other.candyBlock.getCandyBlock(i);
            
            if (cordenada.coord.x != cordenadaOtra.coord.x || cordenada.coord.y != cordenadaOtra.coord.y)
            {
                return false;
            }
            
            // Compare candy types
            if (cordenada.candy->getType() != cordenadaOtra.candy->getType())
            {
                return false;
            }
        }
    }
    
    return true;
}


void Game::printPositions()
{
    for (int j = 0; j < 3; j++)
    {
        CandyCoords canCoord = candyBlock.getCandyBlock(j);
        std::cout << "Candy " << j << " -> (" << canCoord.coord.x << ", " << canCoord.coord.y << ")" << std::endl;
    }
   
}


/*
void Game::setBlock()
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
void Game::posicionarStartBlock(int startX, int startY)
{
    int Y = startY;

    for (int i = 0; i < 3; i++)
    {
        m_candyBlock[i].coord.x = startX;
        m_candyBlock[i].coord.y = Y += 1;
    }
    
    // (int)width / 2, height + 2 (i = 2)
    // (int)width / 2, height + 1 (i = 1)
    // (int)width / 2, height     (i = 0)
    
}


void Game::dropBlock()
{
    for (int i = 0; i < 3; i++)
    {
        m_candyBlock[i].coord.y -= 1;
        droppedTimes++;
    }
}

void Game::moveRight()
{
    for (int i = 0; i < 3; i++)
    {
        m_candyBlock[i].coord.x += 1;
    }
}

void Game::moveLeft()
{
    for (int i = 0; i < 3; i++)
    {
        m_candyBlock[i].coord.x -= 1;
    }
}

void Game::rotateBlocks()
{
    int carameloAbajo, carameloMedio, carameloArriba;
    
    if (m_candyBlock[2].coord.y > m_candyBlock[1].coord.y)
    {
        if (m_candyBlock[2].coord.y > m_candyBlock[0].coord.y)
        {
            carameloArriba = m_candyBlock[2].coord.y;
            carameloMedio = m_candyBlock[1].coord.y;
            carameloAbajo = m_candyBlock[0].coord.y;

            m_candyBlock[1].coord.y = carameloArriba;
            m_candyBlock[0].coord.y = carameloMedio;
            m_candyBlock[2].coord.y = carameloAbajo;

            
        }
        else
        {
            carameloArriba = m_candyBlock[0].coord.y;
            carameloMedio = m_candyBlock[2].coord.y;
            carameloAbajo = m_candyBlock[1].coord.y;

            m_candyBlock[2].coord.y = carameloArriba;
            m_candyBlock[1].coord.y = carameloMedio;
            m_candyBlock[0].coord.y = carameloAbajo;
            
        }
    }
    else
    {
        carameloArriba = m_candyBlock[1].coord.y;
        carameloMedio = m_candyBlock[0].coord.y;
        carameloAbajo = m_candyBlock[2].coord.y;

        m_candyBlock[0].coord.y = carameloArriba;
        m_candyBlock[2].coord.y = carameloMedio;
        m_candyBlock[1].coord.y = carameloAbajo;

    }
    
    
    /*
    A       B
    B ----> C
    C       A
    

}

bool Game::leftFree()
{
    for (int i = 0; i < 3; i++)
    {
        coords coord = m_candyBlock[i].coord;
        if (! ((coord.y > 0) && (coord.x > 0)) )
        {
            return false;
        }

        if (m_board.getCell(coord.x - 1, coord.y) != nullptr)
        {
            return false;
        }
    }
    return true;
}

bool Game::rightFree()
{
    for (int i = 0; i < 3; i++)
    {
        coords coord = m_candyBlock[i].coord;
        if (!( (coord.y > 0) && (coord.x - 1 < m_board.getWidth()) ))
        {
            return false;
        }

        if (m_board.getCell(coord.x + 1, coord.y) != nullptr)
        {
            return false;
        }
    }
    return true;
}

bool Game::bottomFree()
{
    coords coord = m_candyBlock[0].coord;
    if (!(coord.y < 0))
    {
        return false;
    }

    if (m_board.getCell(coord.x, coord.y - 1) != nullptr)
    {
        return false;
    }

    return true;
}

bool Game::gameLost()
{
    if (candyBlock.getCandyBlock(3).coord.y > m_board.getHeight())
    {
        return true;
    }
    return false;
}


*/