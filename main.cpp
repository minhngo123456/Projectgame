#include <iostream>
#include <SDL.h>
#include<SDL_image.h>
#include"move.h"
#include <vector>
#include "bullet.h"
#include"Enemy.h"
#include<bits/stdc++.h>

std::vector<Bullet> bullets;
std::vector<Enemy> enemies;
Uint32 lastSpawnTime=0;



#include"graphics.h"
using namespace std;

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_PollEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}


int main(int argc, char *argv[])
{
    Graphics graphics;
    graphics.init();
    graphics.loadframes();
    SDL_Texture * background = graphics.loadTexture("img\\background.jpg");
    SDL_Texture * gameover = graphics.loadTexture("img\\gameover.jpg");
    SDL_Texture*HP=graphics.loadTexture("img\\hp1.png");
    SDL_Texture*MN=graphics.loadTexture("img\\mn6.png");
    SDL_Texture*EM=graphics.loadTexture("img\\enemy1.png");

    Move mouse;
    mouse.x = 0;
    mouse.y = 730;

    bool quit = false;
    SDL_Event event;
    while (!quit && !gameOver(mouse)) {
    graphics.prepareScene(background);
    graphics.renderTexture(HP,0,0);
    graphics.renderTexture(MN,-7,84);

        for (auto &bullet : bullets) {
            bullet.move();
           }


    for (const auto &bullet : bullets) {
    SDL_Rect bulletRect = {bullet.x, bullet.y, 60, 5};
    SDL_SetRenderDrawColor(graphics.renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(graphics.renderer, &bulletRect);
    }
    Uint32 currentTime=SDL_GetTicks();
    if(currentTime>lastSpawnTime+20000){
        enemies.push_back(Enemy(SCREEN_WIDTH,530,3));
        lastSpawnTime=currentTime;
    }
    for(auto &enemy:enemies){
        enemy.move();
        graphics.renderTexture(EM,enemy.x,enemy.y);
    }
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
    [](const Enemy &e) { return !e.active; }), enemies.end());

        while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) quit = true;
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int bulletX = mouse.x + (mouse.facingRight ? 90 : -10);
                int bulletY = mouse.y + 90;
                int bulletDir = (mouse.facingRight ? 1 : -1);

    bullets.push_back(Bullet(bulletX, bulletY, bulletDir));
}

        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        bool moving=false;

        if (currentKeyStates[SDL_SCANCODE_LEFT]||currentKeyStates[SDL_SCANCODE_A]) {mouse.quatrai();moving=true;}

        else if (currentKeyStates[SDL_SCANCODE_RIGHT]||currentKeyStates[SDL_SCANCODE_D]) {mouse.quaphai();moving=false;}

         else if(currentKeyStates[SDL_KEYUP]) {mouse.stop(); }




        mouse.move();



        render(mouse, graphics);
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
    [](const Bullet &b) { return !b.active; }), bullets.end());
        graphics.presentScene();
        SDL_Delay(10);
    }

    graphics.prepareScene(gameover);
    graphics.presentScene();



    waitUntilKeyPressed();


    SDL_DestroyTexture(HP);
    HP=NULL;

    SDL_DestroyTexture(MN);
    MN=NULL;
    SDL_DestroyTexture( background );
    background = NULL;
    SDL_DestroyTexture(gameover);
    gameover=NULL;
    graphics.quit();
    return 0;

}
