<<<<<<< HEAD
#include <string>
#include "SDL/SDL.h"
#include "draw.hpp"
#include "fixedpt.hpp"
=======
#include "SDL/SDL.h"
#include <string>
#include "draw.hpp"
#include "fixedpt.h"
>>>>>>> d9b9ea93a9d99ce91855115e053e9e2c8f7c2f11

#define min3(a, b, c) std::min(std::min((a),(b)),(c))
#define max3(a, b, c) std::max(std::max((a),(b)),(c))

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel) { 

    int bpp = surface->format->BytesPerPixel; 
    // Here p is the address to the pixel we want to set 
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp; 

    switch(bpp) { 
    case 1: 
        *p = pixel; 
        break; 
    case 2: 
        *(Uint16 *)p = pixel; 
        break; 
    case 3: 
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) { 
            p[0] = (pixel >> 16) & 0xff; 
            p[1] = (pixel >> 8) & 0xff; 
            p[2] = pixel & 0xff; 
        } else { 
            p[0] = pixel & 0xff; 
            p[1] = (pixel >> 8) & 0xff; 
            p[2] = (pixel >> 16) & 0xff; 
        } 
        break; 
    case 4: 
        *(Uint32 *)p = pixel; 
        break; 
    } 
} 

void line(SDL_Surface* screen, Vertex v1, Vertex v2) {
    
    Uint32 x1 = v1.pos.x;
    Uint32 y1 = v1.pos.y;
    Uint32 x2 = v2.pos.x;
    Uint32 y2 = v2.pos.y;

    Uint32 color1 = v1.color;
    Uint32 color2 = v2.color;

    Uint8 r1, g1, b1;
    Uint8 r2, g2, b2;

    SDL_GetRGB(color1, screen->format, &r1, &g1, &b1);
    SDL_GetRGB(color2, screen->format, &r2, &g2, &b2);

    int dx = abs( x1 - x2 );
    int dy = abs( y1 - y2 );

    int max_delta = std::max(dx, dy);

    float delta_r = (r2 - r1) / (float)max_delta;
    float delta_g = (g2 - g1) / (float)max_delta;
    float delta_b = (b2 - b1) / (float)max_delta;

    float r = r1;
    float g = g1;
    float b = b1;

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int e2;

    while(1) {

        putpixel(screen, x1, y1, SDL_MapRGB(screen->format, r, g, b));
        
        r += delta_r;
        g += delta_g;
        b += delta_b;

        if(x1 == x2 && y1 == y2) { break; }
        
        e2 = 2 * err;

        if( e2 > -dy ) {
            err -= dy;
            x1 += sx;
        }

        if(x1 == x2 && y1 == y2) {
            putpixel(screen, x1, y1, color2);
            break;
        }

        if(e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

SDL_Surface *load_image( std::string filename ) {

    //Temporary storage for the image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = SDL_LoadBMP( filename.c_str() );

    //If nothing went wrong in loading the image
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old image
        SDL_FreeSurface( loadedImage );
    }

    //Return the optimized image
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination ) {

    //Make a temporary rectangle to hold the offsets
    SDL_Rect offset;

    //Give the offsets to the rectangle
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

void triangle(SDL_Surface* screen, const Vertex& v1, const Vertex& v2, const Vertex& v3) {

    unsigned int* colorBuffer = (unsigned int*)screen->pixels;
    unsigned int* buffer;
    int stride = screen->pitch;

    // 28.4 fixed-point coordinates
    const int Y1 = rint(16.0f * v1.pos.y);
    const int Y2 = rint(16.0f * v2.pos.y);
    const int Y3 = rint(16.0f * v3.pos.y);

    const int X1 = rint(16.0f * v1.pos.x);
    const int X2 = rint(16.0f * v2.pos.x);
    const int X3 = rint(16.0f * v3.pos.x);

    // Deltas
    const int DX12 = X1 - X2;
    const int DX23 = X2 - X3;
    const int DX31 = X3 - X1;

    const int DY12 = Y1 - Y2;
    const int DY23 = Y2 - Y3;
    const int DY31 = Y3 - Y1;

    // Fixed-point deltas
    const int FDX12 = DX12 << 4;
    const int FDX23 = DX23 << 4;
    const int FDX31 = DX31 << 4;

    const int FDY12 = DY12 << 4;
    const int FDY23 = DY23 << 4;
    const int FDY31 = DY31 << 4;

    // Bounding rectangle
    int minx = (min3(X1, X2, X3) + 0xF) >> 4;
    int maxx = (max3(X1, X2, X3) + 0xF) >> 4;
    int miny = (min3(Y1, Y2, Y3) + 0xF) >> 4;
    int maxy = (max3(Y1, Y2, Y3) + 0xF) >> 4;

    // Block size, standard 8x8 (must be power of two)
    const int q = 8;

    // Start in corner of 8x8 block
    minx &= ~(q - 1);
    miny &= ~(q - 1);

    (char*&)colorBuffer += miny * stride;

    // Half-edge constants
    int C1 = DY12 * X1 - DX12 * Y1;
    int C2 = DY23 * X2 - DX23 * Y2;
    int C3 = DY31 * X3 - DX31 * Y3;

    // Correct for fill convention
    if(DY12 < 0 || (DY12 == 0 && DX12 > 0)) C1++;
    if(DY23 < 0 || (DY23 == 0 && DX23 > 0)) C2++;
    if(DY31 < 0 || (DY31 == 0 && DX31 > 0)) C3++;

    // Color Calculation Constants
    const int Y2_Y3 = DY23;
    const int X1_X3 = X1 - X3;
    const int X3_X2 = X3 - X2;
    const int Y1_Y3 = Y1 - Y3;
    const int Y3_Y1 = DY31;
    const int denom = fp_mul<4>(Y2_Y3, X1_X3) + fp_mul<4>(X3_X2, Y1_Y3);

    // Loop through blocks
    for(int y = miny; y < maxy; y += q)
    {
        for(int x = minx; x < maxx; x += q)
        {
            // Corners of block
            int x0 = x << 4;
            int x1 = (x + q - 1) << 4;
            int y0 = y << 4;
            int y1 = (y + q - 1) << 4;

            // Evaluate half-space functions
            bool a00 = C1 + DX12 * y0 - DY12 * x0 > 0;
            bool a10 = C1 + DX12 * y0 - DY12 * x1 > 0;
            bool a01 = C1 + DX12 * y1 - DY12 * x0 > 0;
            bool a11 = C1 + DX12 * y1 - DY12 * x1 > 0;
            int a = (a00 << 0) | (a10 << 1) | (a01 << 2) | (a11 << 3);
    
            bool b00 = C2 + DX23 * y0 - DY23 * x0 > 0;
            bool b10 = C2 + DX23 * y0 - DY23 * x1 > 0;
            bool b01 = C2 + DX23 * y1 - DY23 * x0 > 0;
            bool b11 = C2 + DX23 * y1 - DY23 * x1 > 0;
            int b = (b00 << 0) | (b10 << 1) | (b01 << 2) | (b11 << 3);
    
            bool c00 = C3 + DX31 * y0 - DY31 * x0 > 0;
            bool c10 = C3 + DX31 * y0 - DY31 * x1 > 0;
            bool c01 = C3 + DX31 * y1 - DY31 * x0 > 0;
            bool c11 = C3 + DX31 * y1 - DY31 * x1 > 0;
            int c = (c00 << 0) | (c10 << 1) | (c01 << 2) | (c11 << 3);

            // Skip block when outside an edge
            if(a == 0x0 || b == 0x0 || c == 0x0) continue;

            // Accept whole block when totally covered
            buffer = colorBuffer;
            if(a == 0xF && b == 0xF && c == 0xF)
            {
                for(int iy = y; iy < y + q; iy++)
                {
                    for(int ix = x; ix < x + q; ix++)
                    {
                        const int lambda1 = fp_div<4>( fp_mul<4>(Y2_Y3, int2fp<4>(ix)-X3) + fp_mul<4>(X3_X2, int2fp<4>(iy)-Y3), denom);
                        const int lambda2 = fp_div<4>( fp_mul<4>(Y3_Y1, int2fp<4>(ix)-X3) + fp_mul<4>(X1_X3, int2fp<4>(iy)-Y3), denom);  
                        const int lambda3 = int2fp<4>(1) - lambda1 - lambda2;

                        int r1 = 0, g1 = 0, b1 = 0;
                        int r2 = 0, g2 = 0, b2 = 0;
                        int r3 = 0, g3 = 0, b3 = 0;

                        r1 = (v1.color >> 16) & 0xFF;
                        g1 = (v1.color >> 8) & 0xFF;
                        b1 = v1.color & 0xFF;
                        r2 = (v2.color >> 16) & 0xFF;
                        g2 = (v2.color >> 8) & 0xFF;
                        b2 = v2.color & 0xFF;
                        r3 = (v3.color >> 16) & 0xFF;
                        g3 = (v3.color >> 8) & 0xFF;
                        b3 = v3.color & 0xFF;

                        int red   = fp2int<4>( fp_mul<4>(lambda1, int2fp<4>(r1)) + fp_mul<4>(lambda2, int2fp<4>(r2)) + fp_mul<4>(lambda3, int2fp<4>(r3)) );
                        int green = fp2int<4>( fp_mul<4>(lambda1, int2fp<4>(g1)) + fp_mul<4>(lambda2, int2fp<4>(g2)) + fp_mul<4>(lambda3, int2fp<4>(g3)) );
                        int blue  = fp2int<4>( fp_mul<4>(lambda1, int2fp<4>(b1)) + fp_mul<4>(lambda2, int2fp<4>(b2)) + fp_mul<4>(lambda3, int2fp<4>(b3)) );
                        
                        buffer[ix] = red << 16 | green << 8 | blue;
                    }
                    (char*&)buffer += stride;
                }
            }
            else // Partially covered block
            {
                int CY1 = C1 + DX12 * y0 - DY12 * x0;
                int CY2 = C2 + DX23 * y0 - DY23 * x0;
                int CY3 = C3 + DX31 * y0 - DY31 * x0;

                for(int iy = y; iy < y + q; iy++)
                {
                    int CX1 = CY1;
                    int CX2 = CY2;
                    int CX3 = CY3;

                    for(int ix = x; ix < x + q; ix++)
                    {
                        if(CX1 > 0 && CX2 > 0 && CX3 > 0)
                        {
                        
                            const int lambda1 = fp_div<4>( fp_mul<4>(Y2_Y3, (ix<<4)-X3) + fp_mul<4>(X3_X2, (iy<<4)-Y3), denom);
                            const int lambda2 = fp_div<4>( fp_mul<4>(Y3_Y1, (ix<<4)-X3) + fp_mul<4>(X1_X3, (iy<<4)-Y3), denom);  
                            const int lambda3 = int2fp<4>(1) - lambda1 - lambda2;

                            int r1 = 0, g1 = 0, b1 = 0;
                            int r2 = 0, g2 = 0, b2 = 0;
                            int r3 = 0, g3 = 0, b3 = 0;

                            r1 = (v1.color >> 16) & 0xFF;
                            g1 = (v1.color >>  8) & 0xFF;
                            b1 = v1.color & 0xFF;
                            r2 = (v2.color >> 16) & 0xFF;
                            g2 = (v2.color >>  8) & 0xFF;
                            b2 = v2.color & 0xFF;
                            r3 = (v3.color >> 16) & 0xFF;
                            g3 = (v3.color >>  8) & 0xFF;
                            b3 = v3.color & 0xFF;

                            int red   = fp2int<4>( fp_mul<4>(lambda1, int2fp<4>(r1)) + fp_mul<4>(lambda2, int2fp<4>(r2)) + fp_mul<4>(lambda3, int2fp<4>(r3)) );
                            int green = fp2int<4>( fp_mul<4>(lambda1, int2fp<4>(g1)) + fp_mul<4>(lambda2, int2fp<4>(g2)) + fp_mul<4>(lambda3, int2fp<4>(g3)) );
                            int blue  = fp2int<4>( fp_mul<4>(lambda1, int2fp<4>(b1)) + fp_mul<4>(lambda2, int2fp<4>(b2)) + fp_mul<4>(lambda3, int2fp<4>(b3)) );
                            
                            buffer[ix] = red << 16 | green << 8 | blue;
                        }

                        CX1 -= FDY12;
                        CX2 -= FDY23;
                        CX3 -= FDY31;
                    }

                    CY1 += FDX12;
                    CY2 += FDX23;
                    CY3 += FDX31;

                    (char*&)buffer += stride;
                }
            }
        } 
        (char*&)colorBuffer += q * stride;   
    }
}

void render_screen( SDL_Surface* screen ) {

    if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) { 
        fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError()); 
            exit(2); 
        } 
    }

    static float theta = 0; 

    Vertex a(SCREEN_WIDTH/2 + 10*cos(theta), SCREEN_HEIGHT/2 + 10*sin(theta), 0xFF0000);
    Vertex b(SCREEN_WIDTH/2 + 10*cos(theta), 0, 0x00FF00);
    Vertex c(0, SCREEN_HEIGHT/2 + 10*sin(theta),0x0000FF);
    SDL_FillRect(screen, NULL, 0x00000000);
    triangle(screen, a, b, c);
    theta += 0.01;

    if ( SDL_MUSTLOCK(screen) ) { 
       SDL_UnlockSurface(screen); 
    } 
}