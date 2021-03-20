#ifndef MYLIB_H
#define MYLIB_H

#include <iostream>
#include <cmath>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
using namespace std;

namespace thisForLib{
SDL_Surface *screen;
bool run = true;
int width, height, bpp;
bool fullScreen = false;
int translate[]={0,0};
uint32_t stroke = 0xFFFFFF00;
}

uint8_t *bufk;
void setup();
void draw();
void exit(); 

void size(int w = 100, int h = 100, bool ful = false)
{
          SDL_Quit();
          if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
                    exit(-1);
          }
          thisForLib::width = w;
          thisForLib::height = h;
          thisForLib::fullScreen = ful;
          if ((thisForLib::screen =  SDL_SetVideoMode(thisForLib::width, thisForLib::height, thisForLib::bpp, ((thisForLib::fullScreen) ? (SDL_SWSURFACE | SDL_FULLSCREEN) : (SDL_SWSURFACE)))) == nullptr)
          {
                    exit(-1);
          }
          thisForLib::width = thisForLib::screen->w;
          thisForLib::height = thisForLib::screen->h;
    return;
}
void App()
{
          try
          {
                    size();
                    setup();
                    SDL_Flip(thisForLib::screen);
                    while (thisForLib::run)
                    {
                              SDL_PumpEvents();
                              bufk = SDL_GetKeyState(nullptr);
                              draw();
                              SDL_Flip(thisForLib::screen);
                              SDL_Delay(1000 / 30);
                    }
                    SDL_Quit();
          }
          catch (exception ex)
          {
                    exit();
          }
    return;
}

void setPixel(int x, int y, uint32_t color,double rot = 0,SDL_Surface* sur = thisForLib::screen){
    if(x<0 || y<0 || x>= sur->w || y>= sur->h)return;
    double sinA,cosA;
    sinA = sin(rot/360*3.14);
    cosA = cos(rot/360*3.14);
    int tmpx = (int)(cosA*x-sinA*y);
    int tmpy = (int)(cosA*y+sinA*x);
    x = tmpx+thisForLib::translate[0];
    y = tmpy+thisForLib::translate[1];
    if(x<0 || y<0 || x>= sur->w || y>= sur->h)return;
    uint32_t *bufp = (uint32_t*)sur->pixels + sur->pitch*y/4 + x;
    *bufp = color;
    return;
}

void exit()
{
          SDL_Quit();
          exit(0);
}

void line(int x1, int y1, int x2, int y2){
    float k, dirx;
    int startX, dirY, startY;
    int dirX = abs(x1-x2)+1;
    if(x1>x2){
        startY = y2;
        startX = x2;
        dirY = y1-y2;
    }else{
        startY = y1;
        startX = x1;
        dirY = y2-y1;
    }
    k = dirY/(float)dirX;
    dirx = 1/(abs(k)+1);
    for(float x = 0; x<dirX; x+= dirx){
        setPixel(x+startX,x*k+startY, thisForLib::stroke);
    }
}

void background(int r, int g, int b){
    uint32_t color = SDL_MapRGB(thisForLib::screen->format, r, g, b);
    SDL_FillRect(thisForLib::screen, nullptr, color);
}

SDL_Surface* loadImage(const char* File){
 SDL_Surface* Surf_Temp = NULL;
    SDL_Surface* Surf_Return = NULL;
 
    if((Surf_Temp = IMG_Load(File)) == NULL) {
        return NULL;
    }
 
    Surf_Return = SDL_DisplayFormatAlpha(Surf_Temp);
    SDL_FreeSurface(Surf_Temp);
 
    return Surf_Return;
}

uint32_t getColor(SDL_Surface* img, int x, int y){
		if(x<0 || y<0 || x>= img->w || y>= img->h)return 0;
          SDL_Color rgb;
		uint32_t *bufp = (uint32_t*)img->pixels + img->pitch*y/4 + x;
        SDL_GetRGB(*bufp, img->format, &rgb.r, &rgb.g, &rgb.b);
        uint32_t color = SDL_MapRGB(thisForLib::screen->format, rgb.r,rgb.g,rgb.b);
        return color;
}
SDL_Surface *Scale(SDL_Surface *Surface, Uint16 Width, Uint16 Height)
{
    if(!Surface || !Width || !Height)
        return 0;
     
    SDL_Surface *_ret = SDL_CreateRGBSurface(Surface->flags, Width, Height, Surface->format->BitsPerPixel,
        Surface->format->Rmask, Surface->format->Gmask, Surface->format->Bmask, Surface->format->Amask);
 
    double    _stretch_factor_x = (static_cast<double>(Width)  / static_cast<double>(Surface->w)),
        _stretch_factor_y = (static_cast<double>(Height) / static_cast<double>(Surface->h));
 
    for(Sint32 y = 0; y < Surface->h; y++)
        for(Sint32 x = 0; x < Surface->w; x++)
            for(Sint32 o_y = 0; o_y < _stretch_factor_y; ++o_y)
                for(Sint32 o_x = 0; o_x < _stretch_factor_x; ++o_x)
                    setPixel(static_cast<Sint32>(_stretch_factor_x * x) + o_x, 
                        static_cast<Sint32>(_stretch_factor_y * y) + o_y, getColor(Surface, x, y), 0,_ret);
 
    return _ret;
}

void drawImage(SDL_Surface* img, int _x,int _y, double rot = 0){
          for(int y=0;y<img->h;y++){
          for(int x =0;x<img->w;x++){
          		setPixel(x+_x,y+_y,getColor(img,x,y),rot);
          }
          }
}

#endif // MYLIB_H
