#ifndef DRAW_H
#define DRAW_H

#include "maTricks/vector.hpp"
#include "maTricks/matrix.hpp"

//The attributes of the screen
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 32

struct Vertex {
    Vec2f pos;
    Uint32 color;

    Vertex(float x, float y, Uint32 col) {
        pos.x = x;
        pos.y = y;
        color = col;
    }
};

// Create and SDL_Surface from a Bitmap.
SDL_Surface* load_image( std::string filename );

// Blit one surface onto another.
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );

// Use Bresenham's to draw a line from p1 to p2. Lerp the color.
void line(SDL_Surface* screen, Vertex p1, Vertex p2);

// Change the color of a single pixel.
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

// Draw a triangle!
void triangle(SDL_Surface* screen, const Vertex& v1, const Vertex& v2, const Vertex& v3);

// Draw everything!
void render_screen( SDL_Surface* screen );
#endif