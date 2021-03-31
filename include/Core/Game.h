#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

extern SDL_Window* m_window;
extern SDL_Renderer* m_renderer;
extern SDL_Event event;
extern uint16_t m_width;
extern uint16_t m_height;
extern uint16_t m_rows;
extern uint16_t m_columns;
extern bool m_isRunning;

typedef SDL_Color GOL_Color;

typedef struct Point
{
    int x;
    int y;
}Point;

struct Cell;
typedef struct Cell
{
    int state;

    int pos_x;
    int pos_y;

    int width;
    int height;

    struct Cell* neighbors[8];

    GOL_Color dead_color;
    GOL_Color alive_color;
}Cell;

extern Cell* m_cells;
extern Cell* m_next_gen_cells;

/**
 * @brief this function should be called to initialize
 *        SDL2 and the game's window
 * 
 * @param name the title of the window
 * @param width the width of the window
 * @param height the height of the window
 * @param rows the number of rows in the game's grid
 * @param columns the number of columns in the game's grid
 * 
 * @return 0 if initialization is succesful -1 otherwise
 */
int GOL_Init(char* name, uint16_t width, uint16_t height, uint16_t rows, uint16_t columns);

/**
 * @brief this function is responsible for handeling
 *        inputs by the user and should be called
 *        every frame before updating and rendering
 *        the game
 */ 
void GOL_HandleInput();

/**
 * @brief this function loads a .txt file
 *        containing data about each cell's
 *        state to quickly initialize the 
 *        world and should be called after 
 *        initializing the program
 * 
 * @param filepath the filepath of the .txt file
 * 
 * @return 0 if file is the same size of the world -1 otherwise
 */
int GOL_LoadState(const char* filepath);

/**
 * @brief this function saves the world's state after
 *        a predetirmened amount of steps(frames)
 */
void GOL_SaveState();

/**
 * @brief this function is responsible for all rendering
 *        occuring in the game and should be called every
 *        frame
 * 
 * @param background_color the color of the background
 * @param grid_outline_color the color of the grid's outline
 */ 
void GOL_Render(GOL_Color background_color, GOL_Color grid_outline_color);

/**
 * @brief returns if window is running or not
 */
bool GOL_IsRunning();

/**
 * @brief returns the width of the window
 */
uint16_t GOL_GetWidth();

/**
 * @brief returns the height of the window
 */
uint16_t GOL_GetHeight();

/**
 * @brief returns the number of rows in the grid
 */
uint16_t GOL_GetGridRows();

/**
 * @brief returns the number of columns in the grid
 */
uint16_t GOL_GetGridColumns();

/**
 * @brief returns the SDL window pointer
 */
SDL_Window* GOL_GetWindow();

/**
 * @brief returns the SDL renderer pointer
 */
SDL_Renderer* GOL_GetRenderer();

/**
 * @brief call this function when wanting to quit the game 
 */
void GOL_Quit();

#endif