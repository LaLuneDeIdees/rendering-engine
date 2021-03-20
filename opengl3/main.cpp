#include <SDL/SDL.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

SDL_Surface* screen;
uint8_t* bufk;

uint32_t white=0;
uint32_t black=0;

struct Vec{float x=0;float y=0;float z=0;};
struct Face{int f1=0;int f2=0;int f3=0;};
void setPixel(int x, int y, uint32_t color);
void line(Vec v1, Vec v2);
void triangle(Vec v1, Vec v2, Vec v3);

int main ()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        exit(-1);
    }
    if ((screen =  SDL_SetVideoMode(0, 0, 32, SDL_SWSURFACE)) == nullptr)
    {
            exit(-1);
    }
    white=SDL_MapRGB(screen->format, 255, 255, 255);
    black=SDL_MapRGB(screen->format, 0, 0, 0);

    ifstream file("model");
    vector<Vec> points;
    vector<Face> faces;
    string tmp;
    while(!file.eof()){
        file>>tmp;
        if(tmp == "v"){
            Vec tmpV;
            file>>tmp;
            tmpV.x = stof(tmp)*100.0;
            file>>tmp;
            tmpV.y = stof(tmp)*100.0;
            file>>tmp;
            tmpV.z = stof(tmp)*100.0;
            points.push_back(tmpV);
        }else if(tmp == "f"){
            Face tmpF;
            file>>tmp;
            tmpF.f1 = stoi(tmp)-1;
            file>>tmp;
            tmpF.f2 = stoi(tmp)-1;
            file>>tmp;
            tmpF.f3 = stoi(tmp)-1;
            faces.push_back(tmpF);
        }
    }

    uint32_t timer = SDL_GetTicks();
    uint16_t frames = 0;
    float delay=0;
    while(true){
        SDL_PumpEvents();
        bufk = SDL_GetKeyState(nullptr);
        if(bufk[SDLK_ESCAPE])break;

        SDL_FillRect(screen, nullptr, black);

        for(int i =0;i<(int)faces.size();i++){
//                line(points[faces[i].f1], points[faces[i].f2]);
//                line(points[faces[i].f1], points[faces[i].f3]);
//                line(points[faces[i].f3], points[faces[i].f2]);
                white = (points[faces[i].f1].z+points[faces[i].f2].z+points[faces[i].f3].z)/10+100;
                if(white<0)continue;
                if(white>255)white=255;
                white = SDL_MapRGB(screen->format,white,white,white);
                triangle(points[faces[i].f1],points[faces[i].f2],points[faces[i].f3]);
        }

        SDL_Flip(screen);
        frames++;
        if(frames/(float)(SDL_GetTicks()-timer)*1000>40)delay+=0.1;
        SDL_Delay(delay);
    }
    SDL_Quit();
    return 0;
}

void setPixel(int x, int y, uint32_t color){
    x+=600;y+=400;
    if(x<0 || y<0 || x>= screen->w || y>= screen->h)return;
    uint32_t *bufp = (uint32_t*)screen->pixels + screen->pitch*y/4 + x;
    if(*bufp > color)return;
    *bufp = color;
    return;
}

void line(Vec v1, Vec v2){
    bool swaping =false;
    if(abs(v2.x-v1.x) < abs(v2.y-v1.y)){
        float tmp=0;
        tmp = v1.x; v1.x = v1.y; v1.y = tmp;
        tmp = v2.x; v2.x = v2.y; v2.y = tmp;
        swaping=true;
    }
    if(v1.x>v2.x){
        Vec tmp = v1;
        v1=v2;v2=tmp;
    }
    int dx = v2.x-v1.x;
    int dy = v2.y-v1.y;
    int direct = (dy>0)? 1:-1;

    int k = abs(dy);
    int error =0;

    int y = v1.y;

    for(int x =v1.x;x<=v2.x;x++){
        if(swaping){
            setPixel(y,x,white);
        }else{
            setPixel(x,y,white);
        }
        error+=k;
        if(error*2>dx){
            y+=direct;
            error-=dx;
        }
    }
}


void triangle(Vec v1, Vec v2, Vec v3){
    if(v1.y>v2.y){
        Vec tmp = v1;v1 = v2;v2=tmp;
    }
    if(v2.y>v3.y){
        Vec tmp = v2;v2 = v3;v3=tmp;
        if(v1.y>v2.y){
            tmp = v1;v1 = v2;v2=tmp;
        }
    }
    float k1 = (v2.x-v1.x)/(v2.y-v1.y+1);
    float k2 = (v3.x-v2.x)/(v3.y-v2.y+1);
    float k3 = (v3.x-v1.x)/(v3.y-v1.y+1);
    float xx1,xx2;
    xx1=v1.x;xx2=xx1;
    for(int y = v1.y;y<=(int)v3.y;y++){
        if(xx1>xx2){for(int p=(int)xx2;p<=(int)xx1;p++)setPixel(p,y,white);}
        else{ for(int p=(int)xx1;p<=(int)xx2;p++)setPixel(p,y,white);}
        if(y<=(int)v2.y){
            xx1+=k1;
        }else{
            xx1+=k2;
        }
        xx2+=k3;
    }
}
