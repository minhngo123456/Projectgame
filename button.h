#ifndef _BUTTON__H_
#define _BUTTON__H_
#include"def.h"
struct Button{
     SDL_Rect rect;
     SDL_Texture* normalTexture;
     SDL_Texture* hoverTexture;
     bool isHovered;
     Button(int x,int y){
     rect={x,y,BUTTON_WIDTH,BUTTON_HEIGHT};
     isHovered=false;
     }
     bool checkHover(int mouseX, int mouseY){
      isHovered=(mouseX>=rect.x&&mouseX<=rect.x+rect.w&&mouseY>=rect.y&&mouseY<=rect.y+rect.h);
      return isHovered;
     }
     bool handleEvent(SDL_Event* e){
     if(e->type==SDL_MOUSEBUTTONDOWN&&e->button.button==SDL_BUTTON_LEFT){
        if(checkHover(e->button.x,e->button.y)){
            return true;
        }
     }
        return false;
     }
     void render(SDL_Renderer* renderer) {
    SDL_Texture* currentTexture = isHovered ? hoverTexture : normalTexture;
    SDL_RenderCopy(renderer, currentTexture, NULL, &rect);}


};
#endif // _BUTTON__H_
