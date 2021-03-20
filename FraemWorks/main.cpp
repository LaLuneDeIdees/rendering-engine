#include "mylib.cpp"
void key();
uint32_t color;
int last[100];
SDL_Surface* img=nullptr;
void setup()
{
          size(0, 0, true);
          color = SDL_MapRGB(thisForLib::screen->format, 255,255,255);
          thisForLib::stroke = color;
          img = loadImage("img.png");
          img = Scale(img, 100,100);
          drawImage(img,100,100, 45);
          thisForLib::translate[0]=200;
          thisForLib::translate[1]=200;
}
int r=0;
void draw()
{
          background(0,0,128);
          drawImage(img,-50,-50, r);
          r+=10;
          key();
}

void key()
{
          if (bufk[SDLK_ESCAPE])
          {
                    exit();
          }
}

int main()
{
          App();
          return 0;
}
