#include <game_state.h>

#define SCREEN_WIDTH 10*SQUARE_SIZE
#define SCREEN_HEIGHT 22*SQUARE_SIZE

static const float TARGET_SECOND_PER_FRAME = 1.f/60.f;
static const int FRAMES_PER_DROP[] = {
    48,
    43,
    38,
    33,
    28,
    23,
    18,
    13,
    8,
    6,
    5,
    5,
    5,
    4,
    4,
    4,
    3,
    3,
    3,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    1
};


typedef struct Input_State {
    char left;
    char right;
    char up;
    char down;
    char a;

    char dleft;
    char dright;
    char dup;
    char ddown;
    char da;
} Input_State, *p_Input_State;

void init();