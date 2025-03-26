#ifndef _BULLET__H_
#define _BULLET__H_
 struct Bullet{
    int x,y;
    int speed;
    bool active;
    int direction;
    Bullet(int stX,int stY,int dir){
    x=stX;
    y=stY;
    speed = 20;
    active = true;
    direction = dir;
    }
    void move(){
    x+=speed * direction;
    if(x<0||x>1920) {
        active=false;
    }
    }
 };
#endif // _BULLET__H_
