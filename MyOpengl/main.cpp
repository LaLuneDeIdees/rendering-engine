#include <iostream>
#include <SDL/SDL.h>
#include <fstream>
using namespace std;
SDL_Surface *screen;
int width,height;
int bpp = 32;
bool runable = true;
SDL_Event event;
uint32_t stroke;

void setup();
void draw();
void setPixel(int *x,int *y, uint32_t *color);
void line(int *x, int *y, int *x1, int *y1);
void keysState(uint8_t* keybuf);
void triangle(int *x1,int *y1,int *x2,int *y2,int *x3,int *y3);

int main()
{
    if(SDL_Init(SDL_INIT_EVERYTHING)<0 || (screen = SDL_SetVideoMode(0,0,bpp,SDL_SWSURFACE))==nullptr)
    {
        exit(1);
    }
    width = screen->w;
    height = screen->h;
    setup();
    while(runable)
    {
        draw();
        SDL_Flip(screen);
        SDL_PumpEvents();
        uint8_t *bufk = SDL_GetKeyState(nullptr);
        keysState(bufk);
//        SDL_Delay(1000/30);
    }
    SDL_Quit();
    cout << "Hello world!" << endl;
    return 0;
}

uint32_t colort;
int j=4;
int vert[3000000];
int cur=3;
int ofcolor = 128;
void setup()
{
    stroke = SDL_MapRGB(screen->format,255,255,0);
    ifstream file("model");
    string str;
    while(!file.eof()){
        file >> str;
        if(str=="v"){
        file >> str;
        int tmp3 = stof(str)*100;
       file >> str;
        int tmp2 = stof(str)*-100+500;
        file >> str;
        int tmp1 = stof(str)*100+500;
        vert[cur++]=tmp1;
        vert[cur++]=tmp2;
        vert[cur++]=tmp3;
        }else if(str=="f"){
        file >> str;
        int tmp1 = stoi(str);
        file >> str;
        int tmp2 = stoi(str);
        file >> str;
        int tmp3 = stoi(str);

//        if(vert[tmp1*2]!=0 && vert[tmp1*2]!=0 && vert[tmp1*2+1]!=0 && vert[tmp2*2+1]!=0 && vert[tmp2*2]!=0 && vert[tmp3*2+1]!=0 && vert[tmp3*2]!=0){
stroke = SDL_MapRGB(screen->format, ofcolor, ofcolor,ofcolor);
triangle(&vert[tmp1*3+1],&vert[tmp1*3+2],&vert[tmp2*3+1],&vert[tmp2*3+2],&vert[tmp3*3+1],&vert[tmp3*3+2]);
ofcolor = 255;
stroke = SDL_MapRGB(screen->format, ofcolor, ofcolor,ofcolor);
//       line(&vert[tmp1*3],&vert[tmp1*3+1],&vert[tmp2*3],&vert[tmp2*3+1]);
//       line(&vert[tmp2*3],&vert[tmp2*3+1],&vert[tmp3*3],&vert[tmp3*3+1]);
//       line(&vert[tmp1*3],&vert[tmp1*3+1],&vert[tmp3*3],&vert[tmp3*3+1]);
ofcolor = 128;
//        }
        }
    }
}
void draw()
{
}

void keysState(uint8_t* keybuf)
{
    if(keybuf[SDLK_ESCAPE])
    {
        exit(0);
    }
}

void setPixel(int x, int y, uint32_t color)
{
    if(x<0 || x>=width || y<0 || y>=height)
    {
        return;
    }
    else
    {
        uint32_t *budp = (uint32_t*)screen->pixels + y*screen->pitch/4 + x;
        *budp = color;
    }
    return;
}

void line(int *x, int *y, int *x1, int *y1)
{
    int y0, x0, dirx;
    dirx = abs(*x-*x1);
    float k, deltax;
    if(*x1>*x)
    {
        y0 = *y;
        x0 =*x;
        k= (*y1-*y)/(float)(dirx);
    }
    else
    {
        y0 = *y1;
        x0 = *x1;
        k= (*y-*y1)/(float)(dirx);
    }
    deltax = 1/(abs(k)+2.0);
    for(float tmp = 0; tmp<dirx; tmp+=deltax)
    {
        float tmp1 = tmp*k;
        setPixel(x0+tmp,y0+tmp1, stroke);
    }
}

void background(int r, int g, int b){
    SDL_FillRect(screen,nullptr,SDL_MapRGB(screen->format,r,g,b));
}

void triangle(int *x1,int *y1,int *x2,int *y2,int *x3,int *y3){

    int y0, x0, dirx;
    dirx = abs(*x1-*x2);
    float k, deltax;
    if(*x2>*x1)
    {
        y0 = *y1;
        x0 =*x1;
        k= (*y2-*y1)/(float)(dirx);
    }
    else
    {
        y0 = *y2;
        x0 = *x2;
        k= (*y1-*y2)/(float)(dirx);
    }
    deltax = 1/(abs(k)+2.0);
    for(float tmp = 0; tmp<dirx; tmp+=deltax)
    {
        int tmp1 = tmp*k+y0;
        int tmp2 = tmp+x0;
//        setPixel(x0+tmp,y0+tmp1, stroke);
        line(x3,y3, &tmp2, &tmp1);
    }
}
