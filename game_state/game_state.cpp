#include <game_state.h>


Game_state game_state_init(p_Piece curr_piece){
    Game_state new_game_state = {0};
    new_game_state.curr_piece = curr_piece;
    new_game_state.cleared_lines = 0;
    new_game_state.highlight_end_time = 0;
    new_game_state.line_count = 0;
    new_game_state.next_drop_time = 0;
    new_game_state.time = 0;
    new_game_state.level = 0;
    new_game_state.phase = GAME_PHASE_START;
    return new_game_state;;
}

bool check_collision(char* well, Piece p){
    for(int i = 0; i < p.side; i++){
        for(int n = 0; n < p.side; n++){
            if(get_square_value(p, i, n)){
                if(*(well + (p.y*WIDTH/SQUARE_SIZE) + (p.x/SQUARE_SIZE) + (i*WIDTH) + n)){
                    return true;
                }if((p.x/SQUARE_SIZE) + 1 + n > WIDTH )
                    return true;
                if((p.x/SQUARE_SIZE) + n < 0)
                    return true;
                if((p.y/SQUARE_SIZE) + 1 + i > HEIGHT)
                    return true;
                if((p.y/SQUARE_SIZE) + i < 0)
                    return true;
            }
        }
    }return false;
}

bool drop_piece(Game_state game_state){
    game_state.curr_piece->y = game_state.curr_piece->y + SQUARE_SIZE;
    if(check_collision(game_state.well, *game_state.curr_piece)){
        game_state.curr_piece->y = game_state.curr_piece->y - SQUARE_SIZE;
        return false;
    }return true;
}

void merge_piece(Game_state game_state){
    char c;
    for(int i = 0; i < game_state.curr_piece->side; i++){
        for(int n = 0; n < game_state.curr_piece->side; n++){
            if(c = get_square_value(*game_state.curr_piece, i, n)){
                *(game_state.well + (game_state.curr_piece->y*WIDTH/SQUARE_SIZE) + (game_state.curr_piece->x/SQUARE_SIZE) + (i*WIDTH) + n) = game_state.curr_piece->c;
            }
        }
    }
}


bool check_collision_after_rotation(char* well, p_Piece p){
    Piece tmp_piece = *p;
    if(tmp_piece.x < 0){
        if(tmp_piece.vector == 1 || tmp_piece.vector == 3){
            tmp_piece.x = 0;
            tmp_piece.vector = (tmp_piece.vector + 1)%4;
            if(check_collision(well, tmp_piece))
                return true;
            p->x = 0;
        }
    }else if(tmp_piece.x/SQUARE_SIZE + tmp_piece.side > WIDTH){
        if(tmp_piece.vector == 3 || tmp_piece.vector == 1){
            tmp_piece.x = tmp_piece.x - (((tmp_piece.x/SQUARE_SIZE) + tmp_piece.side - WIDTH)*SQUARE_SIZE);
            tmp_piece.vector = (tmp_piece.vector + 1)%4;
            if(check_collision(well, tmp_piece))
                return true;
            p->x = tmp_piece.x - (((tmp_piece.x/SQUARE_SIZE) + tmp_piece.side - WIDTH)*SQUARE_SIZE);
        }
    }else{
        tmp_piece.vector = (tmp_piece.vector + 1)%4;
        return(check_collision(well, tmp_piece));
    }
    return false;
}