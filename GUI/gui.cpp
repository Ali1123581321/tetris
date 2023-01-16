#include <gui.h>


#define GAMEOVER_ROW 2
char well[WIDTH*HEIGHT] = {0};

int random(int min, int max){
    int range = max - min;
    return(min + (rand() % range));
}

Piece spawn_piece(){
    int rs = random(0, 7);
    int rc = random(1, 7);
    return(create_piece(rs, SCREEN_WIDTH/2, 0, rc));
}


int find_line(p_Game_state game_state){
    int line_count = 0;
    int count = 0;
    for(int i = 0; i < HEIGHT; i++){
        for(int n = 0; n < WIDTH; n++){
            if(*((game_state->well) + (i*WIDTH) + n) == 0)
                break;
            count++;            
        }if(count == WIDTH){
            *(game_state->lines + i) = 1;
            line_count++;
        }count = 0;
    }return line_count;
}
void copy_line(p_Game_state game_state, int i, int n){
    for(int m = 0; m < WIDTH; m++){
        *(game_state->well + (n*WIDTH) + m) = *(game_state->well + (i*WIDTH) + m);
    }
}
void clear_lines(p_Game_state game_state){
    int n = HEIGHT - 1;
    for(int i = HEIGHT - 1; i >= 0; i--){
        if(!(*(game_state->lines + i))){
            copy_line(game_state, i, n);
            n--;
        }else{
            *(game_state->lines + i) = 0;
        }
    }
}

bool check_row_empty(Game_state game_state){
    for(int i = 0; i < WIDTH; i++){
        if(*(game_state.well + ((GAMEOVER_ROW - 1)*WIDTH) + i)){
            return true;
        }
    }return false;
}

void draw_tmp_piece(SDL_Renderer *renderer, Piece p){
    SDL_Rect r;
    for(int i = 0; i < p.side; i++){
        for(int n = 0; n < p.side; n++){
            if(get_square_value(p, i, n)){
                r.x = p.x + (n*SQUARE_SIZE); r.y = p.y + (i*SQUARE_SIZE); r.h = SQUARE_SIZE; r.w = SQUARE_SIZE;
                SDL_SetRenderDrawColor(renderer, BASE_COLORS[p.c].r, BASE_COLORS[p.c].g, BASE_COLORS[p.c].b, BASE_COLORS[p.c].a);
                SDL_RenderDrawRect(renderer, &r);
            }
        }
    }
}

void render_game(SDL_Renderer* renderer, Game_state gs){
    for(int i = 0; i < HEIGHT; i++){
        for(int n = 0; n < WIDTH; n++){
            draw_square(renderer, n*SQUARE_SIZE, i*SQUARE_SIZE, *(gs.well + (i*WIDTH) + n));
        }
    }if(gs.phase == GAME_PHASE_LINE){
        SDL_Rect r;
        for(int i = 0; i < HEIGHT; i++){
            if(*(gs.lines + i)){
                r.x = 0; r.y = i*SQUARE_SIZE; r.w = WIDTH*SQUARE_SIZE; r.h = SQUARE_SIZE;
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderFillRect(renderer, &r);
            }
        }
    }if(gs.phase == GAME_PHASE_PLAY){
        draw_piece(renderer, *gs.curr_piece);
        Piece tmp_piece = *gs.curr_piece;
        while(!check_collision(gs.well, tmp_piece)){
            tmp_piece.y += SQUARE_SIZE;
        }tmp_piece.y -= SQUARE_SIZE;
        draw_tmp_piece(renderer, tmp_piece);
    }for(int i = 0; i < GAMEOVER_ROW; i++){
        for(int n = 0; n < WIDTH; n++){
            draw_square(renderer, n*SQUARE_SIZE, i*SQUARE_SIZE, 7);
        }
    }
}

float get_time_to_next_drop(int level){
    if(level > 29)
        level = 29;
    return(FRAMES_PER_DROP[level] * TARGET_SECOND_PER_FRAME);
}


bool soft_drop(p_Game_state gs){
    gs->curr_piece->y = gs->curr_piece->y + SQUARE_SIZE;
    if(check_collision(gs->well, *(gs->curr_piece))){
        gs->curr_piece->y = gs->curr_piece->y - SQUARE_SIZE;
        merge_piece(*gs);
        *(gs->curr_piece) = spawn_piece();
        return false;
    }gs->next_drop_time = gs->time + get_time_to_next_drop(gs->level);
    return true;
}


void update_game_play(p_Game_state game_state, p_Input_State input){
    Piece tmp_piece = *(game_state->curr_piece);
    if(input->dleft > 0){
        tmp_piece.x = tmp_piece.x - SQUARE_SIZE;
        if(!check_collision(game_state->well, tmp_piece))
            game_state->curr_piece->x = tmp_piece.x;
    }else if(input->dright > 0){
        tmp_piece.x = tmp_piece.x + SQUARE_SIZE;
        if(!check_collision(game_state->well, tmp_piece))
            game_state->curr_piece->x = tmp_piece.x;
    }else if(input->down > 0)
        soft_drop(game_state);
    else if(input->dup > 0){
        if(!check_collision_after_rotation(game_state->well, game_state->curr_piece))
            game_state->curr_piece->vector = (game_state->curr_piece->vector + 1)%4;
    }else if(input->da > 0){
        while(soft_drop(game_state));
    }
    while(game_state->time >= game_state->next_drop_time){
        if(!soft_drop(game_state))
            break;
    }
    if((game_state->cleared_lines = find_line(game_state)) > 0){
        printf("cleared lines are %d \n", game_state->cleared_lines);
        game_state->phase = GAME_PHASE_LINE;
        game_state->highlight_end_time = game_state->time + 0.25f;
    }
    if(check_row_empty(*game_state)){
        game_state->phase = GAME_PHASE_GAMEOVER;
        printf("GAME OVER \n");
    }
}

int compute_score(Game_state game_state){
    switch (game_state.cleared_lines){
    case 1:
        return (40*(game_state.level + 1));
    case 2:
        return (100*(game_state.level + 1));
    case 3:
        return (300*(game_state.level + 1));
    case 4:
        return (1200*(game_state.level + 1));
    default:
        return 0;
    }
}

int min(int a, int b){
    return(a < b ? a : b);
}

int max(int a, int b){
    return(a > b ? a : b);
}

int get_line_for_next_level(Game_state game_state){
    int first_advance = min((game_state.start_level*10) + 10, 
                            max(100, ((game_state.start_level * 10) - 50)));
    if(game_state.level == game_state.start_level)
        return first_advance;
    else{
        return(first_advance + (game_state.level - game_state.start_level)*10);
    }
}

void update_game_line(p_Game_state game_state){
    if(game_state->time >= game_state->highlight_end_time){
        clear_lines(game_state);
        game_state->phase = GAME_PHASE_PLAY;
        game_state->score += compute_score(*game_state);
        game_state->line_count += game_state->cleared_lines;
        if(game_state->line_count >= get_line_for_next_level(*game_state)){
            game_state->level++;
            printf("level: %d \n", game_state->level);
        }
        printf("score: %d, lines: %d \n", game_state->score, game_state->line_count);
    }
}

void update_game_start(p_Game_state gs, p_Input_State input){
    if(input->dup > 0 && gs->start_level < 29){
        gs->start_level += 1;
        printf("%d \n", gs->start_level);
    }if(input->ddown > 0 && gs->start_level > 0){
        gs->start_level--;
        printf("%d \n", gs->start_level);
    }if(input->da > 0){
        for(int i = 0; i < HEIGHT; i++){
            gs->lines[i] = 0;
        }
        for(int i = 0; i < HEIGHT*WIDTH; i++)
            gs->well[i] = 0;
        gs->level = gs->start_level;
        *gs->curr_piece = spawn_piece();
        gs->phase = GAME_PHASE_PLAY;
        printf("score: 0 \n");
    }
}

void update_game_gameover(p_Game_state game_state, p_Input_State input){
    if(input->da > 0){
        game_state->level = 0; 
        game_state->start_level = 0;
        game_state->line_count = 0;
        game_state->score = 0;
        game_state->phase = GAME_PHASE_START;
    }
}

void update_game(p_Game_state gs, p_Input_State input){
    switch (gs->phase)
    {
    case GAME_PHASE_PLAY:
        update_game_play(gs, input);
        break;
    case GAME_PHASE_LINE:
        update_game_line(gs);
        break;   
    case GAME_PHASE_GAMEOVER:
        update_game_gameover(gs, input);
        break;
    case GAME_PHASE_START:
        update_game_start(gs, input);
        break;
    }
}

void init(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT + 100, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(window == NULL){
        printf("error is %s", SDL_GetError);
        return;
    }
    Piece curr_piece = spawn_piece();
    Game_state game_state = game_state_init(&curr_piece);
    game_state.well = well;
    for(int i = 0; i < HEIGHT*WIDTH; i++)
        *(game_state.well + i) = 0;
    Input_State input = {0, 0 ,0, 0, 0, 0, 0, 0, 0, 0};
    bool quit = false;
    while(!quit){
        game_state.time = SDL_GetTicks() / 1000.f;
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                quit = true;
            }
        }
        Input_State tmp_input = input;
        int key_count;
        const unsigned char* key_states = SDL_GetKeyboardState(&key_count);
        input.left = key_states[SDL_SCANCODE_LEFT];
        input.right = key_states[SDL_SCANCODE_RIGHT];
        input.up = key_states[SDL_SCANCODE_UP];
        input.down = key_states[SDL_SCANCODE_DOWN];
        input.a = key_states[SDL_SCANCODE_SPACE];
        input.dleft = input.left - tmp_input.left;
        input.dright = input.right - tmp_input.right;
        input.dup = input.up - tmp_input.up;
        input.ddown = input.down - tmp_input.down;
        input.da = input.a - tmp_input.a;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        update_game(&game_state, &input);
        render_game(renderer, game_state);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
}