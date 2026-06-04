/**
 * Game logic and rendering implementation. 
 */
#ifndef GAME_H
#define GAME_H

#include "graphics.h"
#include "controller.h"
#include "board.h"
#include "candyblock.h"

//En vez de crear una clase nueva voy a usar un struct que junte todo
/*
struct coords
{
    int x;
    int y;
};

struct CandyCoords
{
    Candy* candy;
    coords coord;

};
*/



/**
 * Main game class: keep track of the game state it. 
 * When run_graphic_game() is called, the game loop will call
 * update() and render() every frame, in that order.
 */

class Game
{
public:
    Game();

    ~Game();

    /// Run the game loop
    void run();

    /**
     * Update the game state. Called every frame when run().
     * This part is not expected to do any rendering.
     * 
     * @param controller the Controller to use for input handling.
     */
    void update(const Controller& controller);

    /**
     * Draw the next frame. Called once per frame, after update().
     * 
     * @param graphics the GraphicManager to use for rendering.
     */
    void render(GraphicManager& graphics);

    /**
     * Serialize and write the current game state (board and falling block) to a file.
     * @param output_path path where to save the game state.
     * @return true if the dump was successful, false otherwise.
     */
    bool dump(const std::string& output_path) const;

    /**
     * Load a serialized game state (board and falling block) from a file created with dump().
     * @param input_path path from which to load the game state.
     * @return true if the load was successful, false otherwise.
     */
    bool load(const std::string& input_path);

    /// @return true if this game is equal to the other game (same board state and falling block)
    bool operator==(const Game& other) const;

    void printPositions();
    void printIndexes(GraphicManager& graphics);


private:

    Board m_board;
    int m_nFrame;
    bool m_blockExists;
    CandyBlock candyBlock;
    
    int m_score;
    bool m_gameOver;
    


    //Perteneciente a candyBlock struct
    /*
    CandyCoords m_candyBlock[3];
    int droppedTimes;


    void setBlock();
    void posicionarStartBlock(int startX, int startY);

    void rotateBlocks();

    void dropBlock();
    void moveRight();
    void moveLeft();


    bool leftFree();
    bool rightFree();
    bool bottomFree();
    bool gameLost();
    */

    



};



#endif


