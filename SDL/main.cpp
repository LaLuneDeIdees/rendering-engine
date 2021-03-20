#include <iostream>
#include <cmath>
#include <SDL/SDL.h>
using namespace std;

SDL_Surface *screen;
SDL_Event event;
float perspektiv = 1;
float distanceMax;

float width, height;

void writePerZ(float z);
void setPixel(SDL_Surface *screen, int x, int y, uint8_t R, uint8_t G, uint8_t B);
void Init();
void Events();

float sizeRectA = 1000;
int playSurfaceWidth = 10, playSurfaceHeight = 10;
bool playSurface[10][10] =
{
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,0,1,1,0,1},
    {1,0,0,1,0,0,1,1,0,1},
    {1,0,0,1,0,0,0,0,0,1},
    {1,0,1,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,1,0,1},
    {1,0,1,0,0,1,1,1,0,1},
    {1,0,0,0,0,0,0,1,0,1},
    {1,1,1,1,1,1,1,1,1,1},
};
struct Player
{
    float x=0,y=0;
    float Vx = 1,Vy = 1;
    float degOfLook = 60.0/180*M_PI;
};
Player thisPlayer;
float sinV, cosV, sinP, cosP;

int main()
{
    Init();
    bool done = true;
//main loop
    while(done)
    {
        SDL_FillRect(screen,nullptr,SDL_MapRGB(screen->format,0,0,0)); //background(0,0,0)
        SDL_Rect r;
        r.x = 0;
        r.w = width;
        r.y = 0;
        r.h = height/2;
        SDL_FillRect(screen,&r,SDL_MapRGB(screen->format,150,150,255)); //background(0,0,0)
        r.y = height/2;
        SDL_FillRect(screen,&r,SDL_MapRGB(screen->format,50,0,0)); //background(0,0,0)

        float Vxthis = thisPlayer.Vx;
        float Vythis = thisPlayer.Vy;

        float dethDeg = thisPlayer.degOfLook/2;
        float xtmp,ytmp;
        xtmp = Vxthis*cos(dethDeg) + Vythis*sin(dethDeg);
        ytmp = Vythis*cos(dethDeg) - Vxthis*sin(dethDeg);
        Vxthis = xtmp;
        Vythis = ytmp;
        for(int i =0; i<screen->w; i++)
        {
        float minZ = distanceMax-1;
        int zX = Vxthis/abs(Vxthis);
        int zY = Vythis/abs(Vythis);
        float k = Vythis/ Vxthis;
//            setPixel(screen, thisPlayer.x + Vxthis*20, thisPlayer.y+Vythis*20,0,255,0);
            float starttmp =int(thisPlayer.x/sizeRectA+((zX>0)? 1:0))*sizeRectA;
            for(float tmp = starttmp; abs(tmp-starttmp) < distanceMax; tmp += sizeRectA*zX){
                float tmpYthis = (tmp-thisPlayer.x)*k;
                float zthis = sqrt(pow(tmpYthis,2)+pow(tmp-thisPlayer.x,2));
                if(zthis > distanceMax)break;
                if(tmp < 0 || tmp>sizeRectA*playSurfaceWidth || thisPlayer.y+tmpYthis <0 || thisPlayer.y+tmpYthis > sizeRectA*playSurfaceHeight)continue;
                if(playSurface[int((thisPlayer.y+tmpYthis)/sizeRectA)][int(tmp/sizeRectA+((zX>0)? 0:-1))]){
//                setPixel(screen, tmp, thisPlayer.y+tmpYthis, 0,0,255);
minZ = zthis;
                break;
                }
            }


            starttmp =int(thisPlayer.y/sizeRectA+((zY>0)? 1:0))*sizeRectA;
            for(float tmp = starttmp; abs(tmp-starttmp) < distanceMax; tmp += sizeRectA*zY){
                float tmpXthis = (tmp-thisPlayer.y)/k;
                float zthis = sqrt(pow(tmpXthis,2)+pow(tmp-thisPlayer.y,2));
                if(zthis > distanceMax)break;
                if(tmp < 0 || tmp>sizeRectA*playSurfaceHeight || thisPlayer.x+tmpXthis <0 || thisPlayer.x+tmpXthis > sizeRectA*playSurfaceWidth)continue;
                if(playSurface[int(tmp/sizeRectA+((zY>0)? 0:-1))][int((thisPlayer.x+tmpXthis)/sizeRectA)]){
//                setPixel(screen, thisPlayer.x+tmpXthis,tmp,  0,0,255);
                if(zthis < minZ)minZ = zthis;
                break;
                }
            }
            writePerZ(minZ);
            xtmp = Vxthis*cosP - Vythis*sinP;
            ytmp = Vythis*cosP + Vxthis*sinP;
            Vxthis = xtmp;
            Vythis = ytmp;
        }

        //Events();
        SDL_PumpEvents();
        uint8_t *keyb = SDL_GetKeyState(nullptr);
        if(keyb[SDLK_UP])
            perspektiv+=10;
        if(keyb[SDLK_DOWN])
            perspektiv-=10;
        if(keyb[SDLK_ESCAPE])
            done=false;
float kV = 5;

float lastX  = thisPlayer.x, lastY = thisPlayer.y;
        if(keyb[SDLK_w])
        {
            thisPlayer.x+=kV*thisPlayer.Vx/sqrt(pow(thisPlayer.Vx,2)+pow(thisPlayer.Vy,2));
            thisPlayer.y+=kV*thisPlayer.Vy/sqrt(pow(thisPlayer.Vx,2)+pow(thisPlayer.Vy,2));
        }
        if(keyb[SDLK_s])
        {
            thisPlayer.x-=kV*thisPlayer.Vx/sqrt(pow(thisPlayer.Vx,2)+pow(thisPlayer.Vy,2));
            thisPlayer.y-=kV*thisPlayer.Vy/sqrt(pow(thisPlayer.Vx,2)+pow(thisPlayer.Vy,2));
        }
        if(keyb[SDLK_a])
        {
            thisPlayer.y-=kV*thisPlayer.Vx/sqrt(pow(thisPlayer.Vx,2)+pow(thisPlayer.Vy,2));
            thisPlayer.x+=kV*thisPlayer.Vy/sqrt(pow(thisPlayer.Vx,2)+pow(thisPlayer.Vy,2));
        }
        if(keyb[SDLK_d])
        {
            thisPlayer.y+=kV*thisPlayer.Vx/sqrt(pow(thisPlayer.Vx,2)+pow(thisPlayer.Vy,2));
            thisPlayer.x-=kV*thisPlayer.Vy/sqrt(pow(thisPlayer.Vx,2)+pow(thisPlayer.Vy,2));
        }
        if(playSurface[int(thisPlayer.y/sizeRectA)][int(thisPlayer.x/sizeRectA)]){
            thisPlayer.x = lastX;
            thisPlayer.y = lastY;
        }
        xtmp = thisPlayer.Vx;
        ytmp = thisPlayer.Vy;
        if(keyb[SDLK_RIGHT])
        {
            xtmp = thisPlayer.Vx*cosV - thisPlayer.Vy*sinV;
            ytmp = thisPlayer.Vy*cosV + thisPlayer.Vx*sinV;
        }
        if(keyb[SDLK_LEFT])
        {
            xtmp = thisPlayer.Vx*cosV + thisPlayer.Vy*sinV;
            ytmp = thisPlayer.Vy*cosV - thisPlayer.Vx*sinV;
        }
        thisPlayer.Vx = xtmp;
        thisPlayer.Vy = ytmp;
//        setPixel(screen,thisPlayer.x,thisPlayer.y,255,0,0);
        SDL_Flip(screen);
        SDL_Delay(1000/30);
    }
//end main loop
    SDL_Quit();
    return 0;

}

void setPixel(SDL_Surface *screen, int x, int y, uint8_t R, uint8_t G, uint8_t B)
{
    if(x<0 || y<0 || x>= width || y>= height)return;
    uint32_t color = SDL_MapRGB(screen->format,R,G,B);
    uint32_t *bufp = (uint32_t*)screen->pixels + y*screen->pitch/4 + x;
    *bufp = color;
    return;
}

void Init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING)<0 || (screen=SDL_SetVideoMode(0,0,32,SDL_SWSURFACE|SDL_FULLSCREEN)) == nullptr)
    {
        exit(1);
    }
    SDL_ShowCursor(SDL_DISABLE);
    width = screen->w;
    height = screen->h;
    distanceMax = 10000;
    sinV = sin(3.0/180*M_PI);
    cosV = cos(3.0/180*M_PI);
    sinP = sin(60.0/width/180*M_PI);
    cosP = cos(60.0/width/180*M_PI);

    thisPlayer.x = sizeRectA*playSurfaceWidth/2;
    thisPlayer.y = sizeRectA*playSurfaceHeight/2;
    cout << "\nOK" << endl;
}

int countColumForDrawZLines = 0;
float hOfWall = 0.6;
bool setHWall = false;
void writePerZ(float z)
{
    if(z>0){
    if(z<distanceMax)
    {
        if(!setHWall)
        {
            hOfWall *= height;
            setHWall = true;
        };
        int newLenOfWall = int(perspektiv*hOfWall/z);
//        int newLenOfWall = int(hOfWall-z*perspektiv);
        if(newLenOfWall>height)
            newLenOfWall = height;
        int colorThis = 255 - 255.0/(distanceMax)*z;
//        int colorThis = 50.0*255.0/z;
        if(colorThis>255)colorThis = 255;
        for(int i = (height-newLenOfWall)/2; i<height - (height-newLenOfWall)/2; i++)
        {
            setPixel(screen,countColumForDrawZLines,i,colorThis,colorThis,colorThis);
        }
    }
    }
    countColumForDrawZLines++;
    if(countColumForDrawZLines>=width)
        countColumForDrawZLines=0;
}

void Events()
{
    while(SDL_PollEvent(&event))
    {
        if ( event.type == SDL_QUIT )
        {
        }
        if ( event.type == SDL_KEYDOWN )
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
            }
        }
    }
}
