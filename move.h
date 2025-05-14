#ifndef _MOVE__H
#define _MOVE__H
#include"graphics.h"
#define INITIAL_SPEED 3

enum characterstate{IDLE,RUNNING_LEFT,RUNNING_RIGHT};
struct Move{
   int x,y;
   int dx=0,dy=0;
   int playerHP=5;
   int playerMN=0;
   int speed =INITIAL_SPEED;
   int frame=0;
   int framecount=2;
   int framedelay=100;
   Uint32 lastFrameTime=0;
   bool facingRight=true;
   characterstate state=IDLE;

   void move(){
   x+=dx;
   y+=dy;
   updateAnimation();
   }
   void updateAnimation(){
   Uint32 currentTime=SDL_GetTicks();
   if(currentTime>lastFrameTime+framedelay){
        if(dx!=0){
            state=RUNNING_RIGHT;
            frame=(frame+1)%framecount;
        }
        else{
         state=IDLE;
         frame=0;
        }
        lastFrameTime=currentTime;
    }
   }
   void len(){
   dy=-speed;
   }
   void xuong(){
   dy=speed;}
   void quaphai(){
   dx=speed;
   state=RUNNING_RIGHT;
   facingRight=true;}
   void quatrai(){
   dx=-speed;
   state=RUNNING_LEFT;
   facingRight=false;}
   void stop(){
   dx=0;dy=0;
   state=IDLE;
   }
   bool invincible = false;
   Uint32 invincibleStart = 0;

   void takeDamage(int damage) {
    Uint32 currentTime = SDL_GetTicks();
    if (!invincible) {
        playerHP -= damage;
        playerMN++;
        if (playerHP < 0) playerHP = 0;

        invincible = true;
        invincibleStart = currentTime;
    }

    if (currentTime - invincibleStart > 2000) {
        invincible = false;
    }
}
 void takedamage(int damage){
   playerHP-=damage;
 }

   };
   void render(const Move &mouse,const Graphics &graphics){
   SDL_Rect destRect ={mouse.x,mouse.y,256,256};
   SDL_Texture* currentFrame=graphics.characterFrames[mouse.frame];
   SDL_RendererFlip flip = mouse.facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
   SDL_RenderCopyEx(graphics.renderer, currentFrame, nullptr, &destRect, 0, nullptr, flip);

   }

   bool gameOver(const Move& mouse) {
    return mouse.x < 0 || mouse.x >= SCREEN_WIDTH ||mouse.y < 0 || mouse.y >= SCREEN_HEIGHT||mouse.playerHP<1;
}


#endif // _MOVE__H
