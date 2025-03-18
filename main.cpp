#include <iostream>
#include <SDL.h>
#include<SDL_image.h>
#include"move.h"
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
    graphics.character= graphics.loadTexture("img\\Sprite2.png");
    SDL_Texture * background = graphics.loadTexture("img\\background.jpg");
    SDL_Texture * gameover = graphics.loadTexture("img\\gameover.jpg");
    SDL_Texture*HP=graphics.loadTexture("img\\hp1.png");
    SDL_Texture*MN=graphics.loadTexture("img\\mn6.png");

    Move mouse;
    mouse.x = 0;
    mouse.y = 730;

    bool quit = false;
    SDL_Event event;
    while (!quit && !gameOver(mouse)) {
    graphics.prepareScene(background);
    graphics.renderTexture(HP,0,0);
    graphics.renderTexture(MN,-7,84);


        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = true;
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);


        if (currentKeyStates[SDL_SCANCODE_LEFT]) {mouse.quatrai();}

        else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {mouse.quaphai();}

         else if(currentKeyStates[SDL_KEYUP]) {mouse.stop(); mouse.y=730;}



        mouse.move();

        render(mouse, graphics);

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
