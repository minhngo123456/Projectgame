#ifndef _GRAPHICS__H
#define _GRAPHICS__H
#include<SDL_image.h>
#include<SDL.h>
#include <SDL_mixer.h>
#include<SDL_ttf.h>
#include"button.h"
struct Graphics {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Mix_Music* backgroundMusic;
    Mix_Chunk* menuHoverSound;
    Mix_Chunk* menuSelectSound;
    Mix_Music* menuOpenSound;
    Mix_Chunk* shootSound;
    Mix_Chunk* hitSound;
    Mix_Chunk* enemyDieSound;
    SDL_Texture* characterFrames[5];
    enum GameState {
    MENU,
    PLAYING,
    EXIT
    };
    GameState gameState=MENU;
    void logErrorAndExit(const char* msg, const char* error)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                       "%s: %s", msg, error);
        SDL_Quit();
    }

    void init() {
        if (TTF_Init() == -1) {
            logErrorAndExit("SDL_ttf could not initialize! SDL_ttf Error: ",
                             TTF_GetError());
        }

        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
            logErrorAndExit("SDL_Init", SDL_GetError());

        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                   SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
                   SDL_WINDOW_SHOWN);
        if (window == nullptr)
                   logErrorAndExit("CreateWindow", SDL_GetError());

        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
            logErrorAndExit( "SDL_image error:", IMG_GetError());

        renderer = SDL_CreateRenderer(window, -1,
                     SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        //renderer =
                  SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));

        if (renderer == nullptr)
             logErrorAndExit("CreateRenderer", SDL_GetError());

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
        logErrorAndExit( "SDL_mixer could not initialize! SDL_mixer Error: %s\n",
                    Mix_GetError() );
}
          loadSounds();
    }

    void prepareScene(SDL_Texture * background)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy( renderer, background, NULL, NULL);
    }

    void presentScene()
    {
        SDL_RenderPresent(renderer);
    }

    SDL_Texture *loadTexture(const char *filename)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                       "Loading %s", filename);
        SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
        if (texture == NULL)
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                  SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());

        return texture;
    }

    void renderTexture(SDL_Texture *texture, int x, int y)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }

    void quit()
    {
        IMG_Quit();
        TTF_Quit();
        Mix_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    void loadframes(){
        characterFrames[0]=loadTexture("img/manmanman.png");
        characterFrames[1]=loadTexture("img/manphai1.png");
        characterFrames[2]=loadTexture("img/manphai2.png");
        characterFrames[3]=loadTexture("img/mantrai1.png");
        characterFrames[4]=loadTexture("img/mantrai2.png");

    }
    void renderHealthBar(SDL_Renderer* renderer, int hp, int maxHP, int x, int y) {
    int barWidth = 250;
    int barHeight =35;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect bgRect = {x, y, barWidth, barHeight};
    SDL_RenderFillRect(renderer, &bgRect);

    int hpWidth = (hp * barWidth) / maxHP;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect hpRect = {x, y, hpWidth, barHeight};
    SDL_RenderFillRect(renderer, &hpRect);


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &bgRect);
}
    void renderManaBar(SDL_Renderer* renderer, int mn, int maxMN, int x, int y) {
    int barWidth = 250;
    int barHeight =35;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect bgRect = {x, y, barWidth, barHeight};
    SDL_RenderFillRect(renderer, &bgRect);

    int mnWidth = (mn * barWidth) / maxMN;
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect mnRect = {x, y, mnWidth, barHeight};
    SDL_RenderFillRect(renderer, &mnRect);


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &bgRect);
}
TTF_Font* loadFont(const char* path, int size)
    {
        TTF_Font* gFont = TTF_OpenFont( path, size );
        if (gFont == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load font %s", TTF_GetError());
        }
        return gFont;
    }
SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Color textColor)
    {
        SDL_Surface* textSurface = TTF_RenderText_Solid( font, text, textColor );
        if( textSurface == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Render text surface %s", TTF_GetError());
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if( texture == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Create texture from text %s", SDL_GetError());
        }

        SDL_FreeSurface( textSurface );
        return texture;
    }
Mix_Music *loadMusic(const char* path)
    {
        Mix_Music *gMusic = Mix_LoadMUS(path);
        if (gMusic == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                "Could not load music! SDL_mixer Error: %s", Mix_GetError());
        }
        return gMusic;
    }
    void play(Mix_Music *gMusic)
    {
        if (gMusic == nullptr) return;

        if (Mix_PlayingMusic() == 0) {
            Mix_PlayMusic( gMusic, -1 );
        }
        else if( Mix_PausedMusic() == 1 ) {
            Mix_ResumeMusic();
        }
    }
    Mix_Chunk* loadSound(const char* path) {
    Mix_Chunk* sound = Mix_LoadWAV(path);
    if (sound == nullptr) {
        logErrorAndExit("Could not load sound effect! SDL_mixer Error: ", Mix_GetError());
    }
    return sound;
}

void loadSounds() {
     backgroundMusic = loadMusic("bmusic.mp3");
     menuHoverSound = loadSound("hsound.mp3");
     menuSelectSound = loadSound("sltedsound.mp3");
     menuOpenSound = loadMusic("osound.mp3");
     shootSound = loadSound("bulletsound.mp3");
     hitSound = loadSound("hitsound.mp3");
     enemyDieSound = loadSound("enemyDieSound.mp3");

}

void playSound(Mix_Chunk* sound) {
    if (sound != nullptr) {
        Mix_PlayChannel(-1, sound, 0);
    }
}


};

#endif
