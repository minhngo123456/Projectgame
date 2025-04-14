#ifndef _ENEMY__H_
#define _ENEMY__H_
#include"move.h"
Move mouse;
struct Enemy{
   int x,y;
   int speed;
   int blood;
   bool active;
   bool quitgame=false;
   Enemy(int stX,int stY,int bld){
   x=stX;
   y=stY;
   speed=2;
   blood=bld;
   active=true;

   }
   void move(){
   x-=speed;
   if(x<0){
    active=false;
    quitgame=true;
   }
   }
   void takeDamage(int damage){
       blood-=damage;
       if(blood<=0){
        active=false;
       }
   }


};
#endif // _ENEMY__H_
