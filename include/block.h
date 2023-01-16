#include<SDL.h>
#include <stdio.h>
#include <colors.h>

#define SQUARE_SIZE 20

const char s1[] = {
    0,0,0,
    1,1,1,
    0,1,0
    };

const char s2[] = {
    0,0,0,0,
    1,1,1,1,
    0,0,0,0,
    0,0,0,0
};

const char s3[] = {
    1,1,
    1,1
};

const char s4[] = {
    0,0,0,
    1,1,1,
    1,0,0
};

const char s5[] = {
    0,0,0,
    1,1,0,
    0,1,1
};

const char s6[] = {
    0,0,0,
    1,1,1,
    0,0,1
};
const char s7[] = {
    0,0,0,
    0,1,1,
    1,1,0
};

typedef struct Shape{
    const char* shape;
    int side;
}Shape;

const Shape shape1{
    s1, 3
};
const Shape shape2{
    s2, 4
};
const Shape shape3{
    s3, 2
};
const Shape shape4{
    s4, 3
};
const Shape shape5{
    s5, 3
};
const Shape shape6{
    s6, 3
};
const Shape shape7{
    s7, 3
};

static const Shape pieces[] = {shape1, shape2, shape3, shape4, shape5, shape6, shape7};


typedef struct Piece{
    int piece_number;
    int vector;
    int side;
    int x;
    int y;
    int c;
    void (*draw_piece)(SDL_Renderer *renderer, Piece self);
} Piece, *p_Piece;

Piece create_piece(int number, int x, int y, int c);

void draw_square(SDL_Renderer* renderer, int x, int y, int c);

void draw_piece(SDL_Renderer *renderer, Piece self);

char get_square_value(Piece p, int row, int col);