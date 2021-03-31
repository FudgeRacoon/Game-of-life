#include <Core/Game.h>

SDL_Window* m_window = NULL;
SDL_Renderer* m_renderer = NULL;
SDL_Event event;
uint16_t m_width = 0;
uint16_t m_height = 0;
uint16_t m_rows = 0;
uint16_t m_columns = 0;
bool m_isRunning = false;

Cell* m_cells = NULL;
Cell* m_next_gen_cells = NULL;

bool INTERNAL_IsFileSameAsWorld(FILE* file)
{
    int file_rows = 0;
    int file_columns = 0;
    char c = 0;
    char* str = (char*)malloc(sizeof(char) * m_rows);

    while(!feof(file))
    {
        c = getc(file);
        if(c == '\n')
            if(file_rows != m_rows)
                return false;
            else
                file_rows = 0;
        else
            file_rows++;
    }
    file_rows--;

    rewind(file);
    while(!feof(file))
    {
        fscanf(file, "%s", str);
        file_columns++;
    }

    rewind(file);

    return file_rows == m_rows && file_columns == m_columns;
}
void INTERNAL_AssignNeighbors(Cell* cell, int x, int y)
{   
    Point directions[] = 
    {
        {.x =  0, .y = -1},  //Up
        {.x =  0,  .y = 1},  //Down
        {.x =  1,  .y = 0},  //Right
        {.x = -1,  .y = 0},  //Left
        {.x =  1, .y = -1},  //Up-Right
        {.x = -1, .y = -1},  //Up-Left
        {.x =  1,  .y = 1},  //Down-Right
        {.x = -1,  .y = 1},  //Down-Left
    };

    for(int i = 0; i < 8; i++)
    {
        int _x = x + directions[i].x;
        int _y = y + directions[i].y;

        bool withinRange = (_x > 0 && _y > 0) && (_x < m_rows && _y < m_columns);

        if(withinRange)
            cell->neighbors[i] = &m_cells[(m_rows* _y) + _x];
    }
}
void INTERNAL_ApplyRules(Cell* cell, Cell* next_gen_cell)
{
    int alive = 0;
    for(int i = 0; i < 8; i++)
        if(cell->neighbors[i] != NULL && cell->neighbors[i]->state == 1)
            alive++;

    if(cell->state == 1 && alive < 2)
        next_gen_cell->state = 0;
    else if(cell->state == 1 && alive > 3)
        next_gen_cell->state = 0;
    else if(cell->state == 0 && alive == 3)
        next_gen_cell->state = 1;
}
void INTERNAL_RenderCells()
{
    for(int y = 0; y < m_columns; y++)
        for(int x = 0; x < m_rows; x++)
        {
            Cell* cell = &m_cells[(m_rows * y) + x];
            SDL_Rect sprite = {.x = cell->pos_x, .y = cell->pos_y, .w = cell->width, .h = cell->height};

            if(cell->state == 1)
                SDL_SetRenderDrawColor(m_renderer, cell->alive_color.r, cell->alive_color.g, cell->alive_color.b, cell->alive_color.a);
            else
                SDL_SetRenderDrawColor(m_renderer, cell->dead_color.r, cell->dead_color.g, cell->dead_color.b, cell->dead_color.a);

            SDL_RenderFillRect(m_renderer, &sprite);
        }
}

int GOL_Init(char* name, uint16_t width, uint16_t height, uint16_t rows, uint16_t columns)
{
    #pragma region Program initialization
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        perror("Failed to initialize SDL2");
        return EXIT_FAILURE;
    }

    m_window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if(m_window == NULL)
    {
        perror("Failed to create window");
        return EXIT_FAILURE;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, 0);
    if(m_renderer == NULL)
    {
        perror("Failed to create renderer");
        return EXIT_FAILURE;
    }

    m_width = width;
    m_height = height;
    m_rows = rows;
    m_columns = columns;
    m_isRunning = true;
    #pragma endregion

    #pragma region Cells initialization
    m_cells = (Cell*)malloc(sizeof(Cell) * (m_columns * m_rows));
    m_next_gen_cells = (Cell*)malloc(sizeof(Cell) * (m_columns * m_rows));

    for(int y = 0; y < m_columns; y++)
        for(int x = 0; x < m_rows; x++)
        {
            Cell* cell = &m_cells[(m_rows * y) + x];
            Cell* next_gen_cell = &m_next_gen_cells[(m_rows * y) + x];

            cell->pos_x = x * (m_width / m_columns);
            cell->pos_y = y * (m_height / m_rows);

            next_gen_cell->pos_x = x * (m_width / m_columns);
            next_gen_cell->pos_y = y * (m_height / m_rows);

            cell->width = m_width / m_columns;
            cell->height = m_height / m_rows;

            next_gen_cell->width = m_width / m_columns;
            next_gen_cell->height = m_height / m_rows;

            cell->neighbors[0] = NULL;
            cell->neighbors[1] = NULL;
            cell->neighbors[2] = NULL;
            cell->neighbors[3] = NULL;
            cell->neighbors[4] = NULL;
            cell->neighbors[5] = NULL;
            cell->neighbors[6] = NULL;
            cell->neighbors[7] = NULL;

            next_gen_cell->neighbors[0] = NULL;
            next_gen_cell->neighbors[1] = NULL;
            next_gen_cell->neighbors[2] = NULL;
            next_gen_cell->neighbors[3] = NULL;
            next_gen_cell->neighbors[4] = NULL;
            next_gen_cell->neighbors[5] = NULL;
            next_gen_cell->neighbors[6] = NULL;
            next_gen_cell->neighbors[7] = NULL;

            cell->alive_color.r = 0; cell->alive_color.g = 0; cell->alive_color.b = 0; cell->alive_color.a = 255;
            cell->dead_color.r = 255; cell->dead_color.g = 255; cell->dead_color.b = 255; cell->dead_color.a = 255; 

            next_gen_cell->alive_color.r = 0; next_gen_cell->alive_color.g = 0; next_gen_cell->alive_color.b = 0; next_gen_cell->alive_color.a = 255;
            next_gen_cell->dead_color.r = 255; next_gen_cell->dead_color.g = 255; next_gen_cell->dead_color.b = 255; next_gen_cell->dead_color.a = 255; 
        }

    for(int y = 0; y < m_columns; y++)
        for(int x = 0; x < m_rows; x++)
        {
            INTERNAL_AssignNeighbors(&m_cells[(m_rows * y) + x], x, y);
            INTERNAL_AssignNeighbors(&m_next_gen_cells[(m_rows * y) + x], x, y);
        }
    #pragma endregion

    return EXIT_SUCCESS;
}

void GOL_HandleInput()
{
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            m_isRunning = false;
        else if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_q)
                m_isRunning = false;
        }
    }
}

int GOL_LoadState(const char* filepath)
{
    FILE* file = fopen(filepath, "r");
    if(!INTERNAL_IsFileSameAsWorld(file))
    {
        printf("[ERROR]: File data does not match world size");
        return EXIT_FAILURE;
    }

    for(int y = 0; y < m_columns; y++)
        for(int x = 0; x <= m_rows; x++)
        {   
            char c = getc(file);
            if(c != '\n')
            {
                if(c - 48 > 1)
                {
                    printf("[ERROR]: File data is incorrect");
                    return EXIT_FAILURE;
                }

                m_cells[(m_rows * y) + x].state = c - 48;
                m_next_gen_cells[(m_rows * y) + x].state = c - 48;
            }
                
        }

    fclose(file);
    return EXIT_SUCCESS;
}

void GOL_SaveState()
{
    int counter = 0;
    char filepath[25] = "save\\saved_state_0.txt";

    for(int i = 0; i < 10; i++)
    {
        FILE* file = fopen(filepath, "r");
        if(file)
        {
            fclose(file);

            for(int i = 0; filepath[i] != '\0'; i++)
                if(filepath[i] - 48 == counter)
                {
                    counter++;
                    filepath[i] = counter + 48;
                    break;
                }      
        }
        else
            break; 
    }
    FILE* file = fopen(filepath, "w");

    for(int y = 0; y < m_columns; y++)
    {
        for(int x = 0; x < m_rows; x++)
            fprintf(file, "%d", m_cells[(m_rows * y) + x].state);
        
        if(y != m_columns - 1)
            fprintf(file, "\n");
    }
}

void GOL_Render(GOL_Color background_color, GOL_Color grid_outline_color)
{
    SDL_SetRenderDrawColor
    (
        m_renderer, 
        background_color.r, 
        background_color.g, 
        background_color.b, 
        background_color.a
    );
    SDL_RenderClear(m_renderer);
    
    #pragma region Apply rules
    for(int y = 0; y < m_columns; y++)
        for(int x = 0; x < m_rows; x++)
            INTERNAL_ApplyRules(&m_cells[(m_rows * y) + x], &m_next_gen_cells[(m_rows * y) + x]);
    
    for(int y = 0; y < m_columns; y++)
        for(int x = 0; x < m_rows; x++)
            m_cells[(m_rows * y) + x] = m_next_gen_cells[(m_rows * y) + x];
    #pragma endregion

    #pragma region Render cells
    INTERNAL_RenderCells();
    #pragma endregion

    SDL_Delay(20);

    #pragma region Render the grid
    int row_offest = m_height / m_rows;
    for(int row = row_offest; row < m_height; row += row_offest)
    {   
        SDL_SetRenderDrawColor
        (
            m_renderer, 
            grid_outline_color.r, 
            grid_outline_color.g, 
            grid_outline_color.b, 
            grid_outline_color.a
        );
        SDL_RenderDrawLine(m_renderer, 0, row, m_width, row);
    }
    int column_offset = m_width / m_columns;
    for(int column = column_offset; column < m_width; column += column_offset)
    {
        SDL_SetRenderDrawColor
        (
            m_renderer, 
            grid_outline_color.r, 
            grid_outline_color.g, 
            grid_outline_color.b, 
            grid_outline_color.a
        );
        SDL_RenderDrawLine(m_renderer, column, 0, column, m_height);
    }
    #pragma endregion

    SDL_RenderPresent(m_renderer);
}

bool GOL_IsRunning()
{
    return m_isRunning;
}

uint16_t GOL_GetWidth()
{
    return m_width;
}

uint16_t GOL_GetHeight()
{
    return m_height;
}

SDL_Window* GOL_GetWindow()
{
    return m_window;
}

SDL_Renderer* GOL_GetRenderer()
{
    return m_renderer;
}

void GOL_Quit()
{
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit;
}