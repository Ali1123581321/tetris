#include <block.h>

void draw_square(SDL_Renderer* renderer, int x, int y, int c){
    SDL_Rect r;
    int edge = SQUARE_SIZE/8;
    r.h = SQUARE_SIZE; r.w = SQUARE_SIZE; r.x = x;  r.y = y;
    Color base_color = BASE_COLORS[c];
    Color dark_color = DARK_COLORS[c];
    Color light_color = LIGHT_COLORS[c];
    SDL_SetRenderDrawColor(renderer, dark_color.r, dark_color.g, dark_color.b, dark_color.a);
    SDL_RenderDrawRect(renderer, &r);
    SDL_RenderFillRect(renderer, &r);
    SDL_SetRenderDrawColor(renderer, light_color.r, light_color.g, light_color.b, light_color.a);
    r.x = r.x + edge; r.h = r.h - edge; r.w = r.w - edge;
    SDL_RenderFillRect(renderer, &r);
    SDL_SetRenderDrawColor(renderer, base_color.r, base_color.g, base_color.b, base_color.a);
    r.y = r.y + edge; r.h = r.h - edge; r.w = r.w - edge;
    SDL_RenderFillRect(renderer, &r);
}
//It transforms from the rotated piece into the original piece. This helps because we can iterate over
//the shape without changing it, and still be able to draw it with rotation
char get_square_value(Piece p, int row, int col){
    const char* tmp_shape = pieces[p.piece_number].shape;
    switch (p.vector)
    {
    case 0:
        return(tmp_shape[(row*p.side + col)]);
    case 1:
        return(tmp_shape[((p.side*(p.side - col - 1)) + row)]);
    case 2:
        return(tmp_shape[((p.side - row - 1)*p.side) + (p.side - col - 1)]);
    case 3:
        return(tmp_shape[((col*p.side) + p.side - row - 1)]);
    default:
        break;
    }return 0;
}
void draw_piece(SDL_Renderer *renderer, Piece self){
    for(int i = 0; i < self.side; i++){
        for(int n = 0; n < self.side; n++){
            if(get_square_value(self, i, n)){
                draw_square(renderer, (self.x + (n*SQUARE_SIZE)), (self.y + (i*SQUARE_SIZE)), self.c);
            }
        }
    }
}

Piece create_piece(int number, int x, int y, int color){
    Piece new_piece = {number, 0, pieces[number].side, x, y, color, draw_piece};
    return new_piece;
} 