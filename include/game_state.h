#include <block.h>

#define WIDTH 10
#define HEIGHT 22
#define SCORE 0


enum PHASE {GAME_PHASE_START, GAME_PHASE_PLAY, GAME_PHASE_LINE, GAME_PHASE_GAMEOVER};

typedef struct Game_state{
    char* well;
    p_Piece curr_piece;
    float time;
    float highlight_end_time;
    float next_drop_time;
    int start_level;
    int level;
    PHASE phase;
    int line_count;
    int cleared_lines;
    int score;
    char lines[HEIGHT];
} Game_state, *p_Game_state;

Game_state game_state_init(p_Piece curr_piece);

bool check_collision(char* well, Piece p);

bool check_collision_after_rotation(char* well, p_Piece p);

bool drop_piece(Game_state game_state);

void merge_piece(Game_state game_state);