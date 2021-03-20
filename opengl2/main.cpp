#include <SDL/SDL.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

int width,height;

SDL_Surface* screen;
uint8_t* bufk;

uint32_t white=0;
uint32_t black=0;

void setPixel(int x, int y, uint32_t color){
    x+=600;y+=400;
    if(x<0 || y<0 || x>= screen->w || y>= screen->h)return;
    uint32_t *bufp = (uint32_t*)screen->pixels + screen->pitch*y/4 + x;
    if(*bufp > color)return;
    *bufp = color;
    return;
}
void swap(int* i1, int*i2){
    int t = *i2;
    *i2=*i1;
    *i1=t;
}
void line(int x1, int y1, int x2,int y2){
    bool swaping =false;
    if(abs(x2-x1) < abs(y1-y2)){
        x1+=y1;y1=x1-y1;x1-=y1;
        x2+=y2;y2=x2-y2;x2-=y2;
        swaping=true;
    }
    if(x2<x1){
        x1+=x2;x2=x1-x2;x1-=x2;
        y1+=y2;y2=y1-y2;y1-=y2;
    }
    int dx = x2-x1;
    int dy = y2-y1;
    int direct = (dy>0)? 1:-1;

    int k = abs(dy);
    int error =0;

    int y = y1;
    for(int x =x1;x<=x2;x++){
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

void triangle(int x1, int y1, int x2,int y2, int x3,int y3){
    if(y1>y2){
        swap(&x1,&x2);
        swap(&y1,&y2);
    }
    if(y2>y3){
        swap(&x2,&x3);
        swap(&y2,&y3);
        if(y1>y2){
            swap(&x1,&x2);
            swap(&y1,&y2);
        }
    }
    float k1 = (x2-x1)/(float)(y2-y1+1);
    float k2 = (x3-x2)/(float)(y3-y2+1);
    float k3 = (x3-x1)/(float)(y3-y1+1);
    float xx1,xx2;
    xx1=x1;xx2=x1;
    for(int y = y1;y<=y3;y++){
        if(xx1>xx2)for(int p=xx2;p<=xx1;p++)setPixel(p,y,white);
        else for(int p=xx1;p<=xx2;p++)setPixel(p,y,white);
        if(y<=y2){
            xx1+=k1;
        }else{
            xx1+=k2;
        }
        xx2+=k3;
    }
}

vector<float> rotateZ(vector<float> points){
    float cosa,sina;
    cosa = cos(0.02);
    sina= sin(0.02);
    float x,y,x1,y1;
    for(int i=0;i<points.size();){
        x = points[i++];
        y = points[i++];i++;
        x1 = x*cosa - y*sina;
        y1 = x*sina + y*cosa;
        points[i-3]=x1;
        points[i-2]=y1;
    }
    return points;
}
vector<float> rotateY(vector<float> points){
    float cosa,sina;
    cosa = cos(0.02);
    sina= sin(0.02);
    float x,y,x1,y1;
    for(int i=0;i<points.size();){
        x = points[i++];i++;
        y = points[i++];
        x1 = x*cosa - y*sina;
        y1 = x*sina + y*cosa;
        points[i-3]=x1;
        points[i-1]=y1;
    }
    return points;
}
vector<float> rotateX(vector<float> points){
    float cosa,sina;
    cosa = cos(0.02);
    sina= sin(0.02);
    float x,y,x1,y1;
    for(int i=0;i<points.size();){
        i++;
        x = points[i++];
        y = points[i++];
        x1 = x*cosa - y*sina;
        y1 = x*sina + y*cosa;
        points[i-2]=x1;
        points[i-1]=y1;
    }
    return points;
}
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
    white=SDL_MapRGB(screen->format, 0, 0, 0);

    ifstream file("model1");
    vector<float> points;
    vector<int> faces;
        string tmp;
    while(!file.eof()){
        file>>tmp;
        if(tmp == "v"){
            file>>tmp;
            points.push_back(stof(tmp)*100.0);
            file>>tmp;
            points.push_back(stof(tmp)*100.0);
            file>>tmp;
            points.push_back(stof(tmp)*100.0);
        }else if(tmp == "f"){
            file>>tmp;
            faces.push_back(stoi(tmp)-1);
            file>>tmp;
            faces.push_back(stoi(tmp)-1);
            file>>tmp;
            faces.push_back(stoi(tmp)-1);
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

        for(int i =0;i<faces.size();){
            float x1,x2,x3,y1,y2,y3,z1,z2,z3;

            x1 = points[faces[i]*3];
            y1 = points[faces[i]*3+2];
            z1 = points[faces[i++]*3+1];
            x2 = points[faces[i]*3];
            y2 = points[faces[i]*3+2];
            z2 = points[faces[i++]*3+1];
            x3 = points[faces[i]*3];
            y3 = points[faces[i]*3+2];
            z3 = points[faces[i++]*3+1];
            z1+=z2+z3;
            z1=z1/3+200;
            z1 = z1*255/400;
            if(z1<0)continue;if(z1>255)z1=255;
            white = SDL_MapRGB(screen->format,z1,z1,z1);

            triangle(x1,y1,x2,y2,x3,y3);

        }
        points = rotateZ(points);
        points = rotateX(points);
        points = rotateX(points);
        points = rotateY(points);

//        triangle(50,50,0,0,100,0);

        SDL_Flip(screen);
        frames++;
        if(frames/(float)(SDL_GetTicks()-timer)*1000>40)delay+=0.1;
        SDL_Delay(delay);
    }
    SDL_Quit();
    return 0;
}
