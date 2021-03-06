#include <Core/Game.h>

int main(int argc, char* argv[])
{
    if(GOL_Init("Test", (int)strtof(argv[1], NULL), (int)strtof(argv[2], NULL), (int)strtof(argv[3], NULL), (int)strtof(argv[4], NULL)) != 0)
    {
        getchar();
        return EXIT_FAILURE;
    }
    
    if(GOL_LoadState(argv[5]) != 0)
    {
        getchar();
        return EXIT_FAILURE;
    }

    GOL_Color background_color = {.r = 255, .g = 255, .b = 255, .a = 255};
    GOL_Color grid_outline_color = {.r = 153, .g = 204, .b = 255, .a = 255};

    while(GOL_IsRunning())
    {
        GOL_HandleInput();
        GOL_Render(background_color, grid_outline_color);
    }

    GOL_SaveState();
    GOL_Quit();

    return EXIT_SUCCESS;
}