#ifndef _MOVE__H
#define _MOVE__H
#include"graphics.h"
#define INITIAL_SPEED 3

struct Move{
   int x,y;
   int dx=0,dy=0;
   int speed =INITIAL_SPEED;
   void move(){
   x+=dx;
   y+=dy;
   }
   void quaphai(){
   dx=speed;}
   void quatrai(){
   dx=-speed;}
   void stop(){
   dx=0;dy=0;}


   };
   void render(const Move &mouse,const Graphics &graphics){
   SDL_Rect destRect ={mouse.x,mouse.y,256,256};
   SDL_RenderCopy(graphics.renderer,graphics.character,nullptr,&destRect);
   }
   bool gameOver(const Move& mouse) {
    return mouse.x < 0 || mouse.x >= SCREEN_WIDTH ||mouse.y < 0 || mouse.y >= SCREEN_HEIGHT;
}

#endif // _MOVE__H
