#include <iostream>
#include <SDL.h>
#include<SDL_image.h>
#include <SDL_mixer.h>
#include"move.h"
#include <vector>
#include "bullet.h"
#include"Enemy.h"
#include<bits/stdc++.h>
#include"graphics.h"
#include<SDL_ttf.h>
#include <ctime>
std::vector<Bullet> bullets;
std::vector<Enemy> enemies;
std::vector<Bullet>bullets2;
Uint32 lastSpawnTime=0;
Uint32 displayTime=0;
int score = 0;
int bestScore = 0;
bool hasDisplayedText=false;
using namespace std;

bool checkCollision(const Move &mouse, const Enemy &enemy) {
    SDL_Rect playerRect = {mouse.x, mouse.y, 128, 100};
    SDL_Rect enemyRect = {enemy.x, enemy.y, 128, 128};

    return SDL_HasIntersection(&playerRect, &enemyRect);
}

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
int loadBestScore() {
    std::ifstream file("bestscore.txt");
    int best = 0;
    if (file >> best) {
        return best;
    }
    return 0;
}

void saveBestScore(int score) {
    std::ofstream file("bestscore.txt");
    if (file) {
        file << score;
    }
}

int main(int argc, char *argv[])
{
    enum GameState {
    MENU,
    PLAYING,
    SETTINGS,
    EXIT
    };
    srand(static_cast<unsigned>(time(nullptr)));
    bool restartGame = false;
    Graphics graphics;
    graphics.init();

    graphics.loadSounds();
    SDL_Texture * menu = graphics.loadTexture("img\\menu.png");
    SDL_Texture * background = graphics.loadTexture("img\\background.png");
    SDL_Texture * gameover = graphics.loadTexture("img\\gameover.jpg");
    TTF_Font* font = graphics.loadFont("arial.ttf", 24);
    TTF_Font* smallFont = graphics.loadFont("arial.ttf", 36);
    TTF_Font* font1 = graphics.loadFont("arial.ttf", 64);

    SDL_Color color = {255, 255, 0, 0};
    SDL_Texture* ultText = graphics.renderText("Ready for ulti, press X!", font, color);

    const int buttonYStart = 380;
    const int buttonSpacing = 80;

    Button startButton(SCREEN_WIDTH/2 - BUTTON_WIDTH/2, 400,300,70);
    startButton.normalTexture = graphics.renderText("Start Game", smallFont, {255, 255, 255, 255});
    startButton.hoverTexture = graphics.renderText("Start Game", smallFont, {255, 215, 0, 255});

    Button settingsButton(SCREEN_WIDTH/2 - BUTTON_WIDTH/2, 500,210,70);
    settingsButton.normalTexture = graphics.renderText("Settings", smallFont, {255, 255, 255, 255});
    settingsButton.hoverTexture = graphics.renderText("Settings", smallFont, {255, 215, 0, 255});

    Button quitButton(SCREEN_WIDTH/2 - BUTTON_WIDTH/2, 600,100,70);
    quitButton.normalTexture = graphics.renderText("Quit", smallFont, {255, 255, 255, 255});
    quitButton.hoverTexture = graphics.renderText("Quit", smallFont, {255, 215, 0, 255});

    Button backButton(10, 10,120,70);
    backButton.normalTexture = graphics.renderText("Back", smallFont, {255, 255, 255, 255});
    backButton.hoverTexture = graphics.renderText("Back", smallFont, {255, 215, 0, 255});

    Button restartButton(SCREEN_WIDTH/2 - BUTTON_WIDTH/2, 500, 250, 70);
    restartButton.normalTexture = graphics.renderText("Restart", smallFont, {255, 255, 255, 255});
    restartButton.hoverTexture = graphics.renderText("Restart", smallFont, {255, 215, 0, 255});

    int volume = MIX_MAX_VOLUME;
    int sliderX = 700;
    int sliderY = 400;
    int sliderWidth = 300;
    int sliderHeight = 20;
    bool draggingSlider = false;

    bestScore = loadBestScore();


    int currentWave = 1;
    Uint32 waveStartTime = SDL_GetTicks();


    Move mouse;
    Sprite man;
    SDL_Texture* manTexture = graphics.loadTexture(MAN_SPRITE_FILE);
    man.init(manTexture, MAN_FRAMES, MAN_CLIPS);

    Sprite manidle;
    SDL_Texture* manidleTexture = graphics.loadTexture(MANIDLE_SPRITE_FILE);
    manidle.init(manidleTexture, MANIDLE_FRAMES, MANIDLE_CLIPS);

    Sprite mon2;
    SDL_Texture* mon2Texture = graphics.loadTexture(MON_SPRITE_FILE);
    mon2.init(mon2Texture, MON2_FRAMES, MON2_CLIPS);

    Sprite mon2attack;
    SDL_Texture* mon2attackTexture = graphics.loadTexture(MONATTACK_SPRITE_FILE);
    mon2attack.init(mon2attackTexture, MON2ATTACK_FRAMES, MON2ATTACK_CLIPS);

    Sprite mon2attacked;
    SDL_Texture* mon2attackedTexture = graphics.loadTexture(MONATTACKED_SPRITE_FILE);
    mon2attacked.init(mon2attackedTexture, MON2ATTACKED_FRAMES, MON2ATTACKED_CLIPS);

    Sprite manflip;
    SDL_Texture* manflipTexture = graphics.loadTexture(MANFLIP_SPRITE_FILE);
    manflip.init(manflipTexture, MANFLIP_FRAMES, MANFLIP_CLIPS);

    Sprite manidleflip;
    SDL_Texture* manidleflipTexture = graphics.loadTexture(MANIDLEFLIP_SPRITE_FILE);
    manidleflip.init(manidleflipTexture, MANIDLEFLIP_FRAMES, MANIDLEFLIP_CLIPS);


    Sprite shot;
    SDL_Texture* shotTexture = graphics.loadTexture(SHOT_SPRITE_FILE);
    shot.init(shotTexture, SHOT_FRAMES, SHOT_CLIPS);


    GameState gameState=MENU;
    bool quit = false;
    SDL_Event event;
    do {
    score=0;
    mouse = Move();
    mouse.x = 0;
    mouse.y = 500;
    bullets.clear();
    bullets2.clear();
    enemies.clear();
    currentWave = 1;
    waveStartTime = SDL_GetTicks();
    lastSpawnTime = 0;
    hasDisplayedText = false;
    restartGame = false;
    while (!quit && !gameOver(mouse)) {

            if(gameState==MENU){
            if (!Mix_PlayingMusic()) {
            Mix_PlayMusic(graphics.menuOpenSound, -1);
            }
            while(SDL_PollEvent(&event)){
                if(event.type==SDL_QUIT){
                    quit=true;
                }
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                  bool wasHovered = startButton.isHovered || quitButton.isHovered || settingsButton.isHovered;
                  startButton.checkHover(mouseX, mouseY);
                  quitButton.checkHover(mouseX, mouseY);
                  settingsButton.checkHover(mouseX, mouseY);
                  bool nowHovered = startButton.isHovered || quitButton.isHovered || settingsButton.isHovered;
                   if (!wasHovered && nowHovered) {
                         Mix_PlayChannel(-1, graphics.menuHoverSound, 0);
                   }
                if(startButton.handleEvent(&event)){
                    Mix_PlayChannel(-1, graphics.menuSelectSound, 0);
                    gameState = PLAYING;
                    Mix_PlayMusic(graphics.backgroundMusic, -1);
                }

                if(settingsButton.handleEvent(&event)){
                Mix_PlayChannel(-1, graphics.menuSelectSound, 0);
                gameState = SETTINGS;
                    }
                if(quitButton.handleEvent(&event)){
                    Mix_PlayChannel(-1, graphics.menuSelectSound, 0);
                    SDL_Delay(300);
                    quit = true;
                }
            }

            graphics.prepareScene(menu);
            startButton.render(graphics.renderer);
            quitButton.render(graphics.renderer);
            settingsButton.render(graphics.renderer);
            graphics.presentScene();
            }
            else if (gameState == SETTINGS) {
    int sliderWidth = 600;
    int sliderHeight = 40;
    int sliderX = (SCREEN_WIDTH - sliderWidth) / 2;
    int sliderY = (SCREEN_HEIGHT - sliderHeight) / 2;

    while (SDL_PollEvent(&event)) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        backButton.checkHover(mouseX, mouseY);

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                if (mouseX >= sliderX && mouseX <= sliderX + sliderWidth &&
                    mouseY >= sliderY && mouseY <= sliderY + sliderHeight) {
                    draggingSlider = true;
                }

                if (backButton.handleEvent(&event)) {
                    Mix_PlayChannel(-1, graphics.menuSelectSound, 0);
                    gameState = MENU;
                }
            }
        }

        if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
            draggingSlider = false;
        }

        if (event.type == SDL_MOUSEMOTION && draggingSlider) {
            int newX = mouseX;
            newX = std::max(sliderX, std::min(sliderX + sliderWidth, newX));
            volume = ((newX - sliderX) * MIX_MAX_VOLUME) / sliderWidth;
            Mix_Volume(-1, volume);
            Mix_VolumeMusic(volume);
        }
    }

    graphics.prepareScene(menu);

    SDL_Color textColor = {255, 255, 0, 255};
    SDL_Texture* settingsText = graphics.renderText("Sound", font1, textColor);

    int textW, textH;
    SDL_QueryTexture(settingsText, NULL, NULL, &textW, &textH);

    int textX = sliderX - textW - 10;
    int textY = sliderY + (sliderHeight - textH) / 2;

    graphics.renderTexture(settingsText, textX, textY);
    SDL_DestroyTexture(settingsText);

    SDL_Rect sliderBg = {sliderX, sliderY, sliderWidth, sliderHeight};
    SDL_SetRenderDrawColor(graphics.renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(graphics.renderer, &sliderBg);

    int filledWidth = (volume * sliderWidth) / MIX_MAX_VOLUME;
    SDL_Rect sliderFill = {sliderX, sliderY, filledWidth, sliderHeight};
    SDL_SetRenderDrawColor(graphics.renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(graphics.renderer, &sliderFill);

    backButton.render(graphics.renderer);
    graphics.presentScene();
}

    else if(gameState==PLAYING){
    graphics.prepareScene(background);
    SDL_Color white = {255, 255, 255, 255};
    SDL_Texture* scoreText = graphics.renderText(("Score: " + std::to_string(score)).c_str(), font, white);
    SDL_Texture* bestScoreText = graphics.renderText(("Best: " + std::to_string(bestScore)).c_str(), font, white);

    graphics.renderTexture(scoreText, 10, 900);
    graphics.renderTexture(bestScoreText, 10, 940);

    SDL_DestroyTexture(scoreText);
    SDL_DestroyTexture(bestScoreText);


     for (auto &bullet : bullets) {
            bullet.move();
           }

     for (auto &bullet : bullets2) {
            bullet.move();
           }
    for (const auto &bullet : bullets) {
    SDL_Rect bulletRectbg1={bullet.x,bullet.y,60,-2};
    SDL_SetRenderDrawColor(graphics.renderer,  0, 255, 0, 255);
    SDL_RenderFillRect(graphics.renderer, &bulletRectbg1);
    SDL_Rect bulletRectbg2={bullet.x,bullet.y,60,4};
    SDL_SetRenderDrawColor(graphics.renderer,  0, 255, 0, 255);
    SDL_RenderFillRect(graphics.renderer, &bulletRectbg2);
    SDL_Rect bulletRect = {bullet.x, bullet.y, 60, 2};
    SDL_SetRenderDrawColor(graphics.renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(graphics.renderer, &bulletRect);
    }
    //UpgradeBullets
    for(const auto &bullet:bullets2){
    SDL_Rect bulletRectbg1={bullet.x,bullet.y,60,-4};
    SDL_SetRenderDrawColor(graphics.renderer,  255, 0, 0, 255);
    SDL_RenderFillRect(graphics.renderer, &bulletRectbg1);
    SDL_Rect bulletRectbg2={bullet.x,bullet.y,60,8};
    SDL_SetRenderDrawColor(graphics.renderer,  255, 0, 0, 255);
    SDL_RenderFillRect(graphics.renderer, &bulletRectbg2);
    SDL_Rect bulletRect = {bullet.x, bullet.y, 60, 5};
    SDL_SetRenderDrawColor(graphics.renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(graphics.renderer, &bulletRect);
    }
    static int enemiesToSpawn = 0;
    static int spawnedEnemies = 0;
    static Uint32 lastEnemySpawnTime = 0;
    static Uint32 lastSpawnTime = 0;
    static std::vector<int> usedY;

    Uint32 currentTime = SDL_GetTicks();


    if (currentTime > lastSpawnTime + 5000 && enemiesToSpawn == 0) {
    enemiesToSpawn = 3 + currentWave;
    spawnedEnemies = 0;
    lastEnemySpawnTime = currentTime;
    lastSpawnTime = currentTime;
    usedY.clear();
}


    if (enemiesToSpawn > 0 && currentTime > lastEnemySpawnTime + 700) {
    int yPos;
    bool valid = false;
    int tries = 0, maxTries = 20;

    while (!valid && tries < maxTries) {
        yPos = 350 + (rand() % (600 - 350 + 1));
        valid = true;

        for (int used : usedY) {
            if (abs(yPos - used) < 60) {
                valid = false;
                break;
            }
        }
        tries++;
    }


    if (valid) {
        usedY.push_back(yPos);
        enemies.push_back(Enemy(SCREEN_WIDTH, yPos, 4 + currentWave, 2 + (currentWave / 3)));
        spawnedEnemies++;
        lastEnemySpawnTime = currentTime;
    } else {
        yPos = 350 + (rand() % (600 - 350 + 1));
        enemies.push_back(Enemy(SCREEN_WIDTH, yPos, 4 + currentWave, 2 + (currentWave / 3)));
        spawnedEnemies++;
        lastEnemySpawnTime = currentTime;
    }

    if (spawnedEnemies >= enemiesToSpawn) {
        enemiesToSpawn = 0;
        usedY.clear();
    }
}

    man.tick();
    manidle.tick();
    manidleflip.tick();
    manflip.tick();
    shot.tick();
    mon2.tick();
    mon2attack.tick();
    mon2attacked.tick();
    Uint32 now = SDL_GetTicks();


    for (auto &bullet : bullets2) {
    SDL_Rect bulletRect = {bullet.x, bullet.y, 60, 5};
    for (auto &enemy : enemies) {
        SDL_Rect enemyRect = {enemy.x, enemy.y, 150, 150};
        if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
            enemy.state1=2;
            enemy.takeDamage(5);
            if(enemy.blood==0){
                score+=10*currentWave;
            }
            Mix_PlayChannel(-1, graphics.hitSound, 0);
            bullet.active = false;

        }
    }
}
    for (auto &bullet : bullets) {
            SDL_Rect bulletRect = {bullet.x, bullet.y, 60, 5};
            bool bulletHit = false;
            for (auto &enemy : enemies) {
                SDL_Rect enemyRect = {enemy.x, enemy.y, 150, 150};
                if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
                    enemy.state1=2;
                    enemy.takeDamage(1);
                    if(enemy.blood==0){
                score+=10*currentWave;
            }
                    Mix_PlayChannel(-1, graphics.hitSound, 0);
                    if (bullet.level == 1 && !bullet.hasGivenMana) {
                    mouse.playerMN = std::min(mouse.playerMN + 1, 5);
                    bullet.hasGivenMana = true;
            }
            bulletHit = true;
            bullet.active = false;

                }
            }
            if (bulletHit) break;
        }
    for(auto &enemy:enemies){
        enemy.move();
        if(enemy.x<=0){
            mouse.playerHP--;
        }
        if(checkCollision(mouse,enemy)){
            enemy.state2=1;
            Mix_PlayChannel(-1, graphics.hitSound, 0);
            mouse.takeDamage(1);
            if(mouse.playerMN>=5){
                mouse.playerMN=5;
            }



            }else enemy.state2 = 0;

        if(enemy.state1==2){
            enemy.timer++;
            graphics.render(enemy.x,enemy.y,mon2attacked);
            if(enemy.timer>35){
                enemy.state1=0;
                enemy.timer=0;
            }
        }else if(enemy.state2==1){
        graphics.render(enemy.x,enemy.y,mon2attack);
        }else if(enemy.state2==0)graphics.render(enemy.x,enemy.y,mon2);


        SDL_Rect healthBarBg = {enemy.x+90, enemy.y-10 , ((4+currentWave)*50)/4, 5};
        SDL_SetRenderDrawColor(graphics.renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(graphics.renderer, &healthBarBg);

        SDL_Rect healthBar = {enemy.x+90, enemy.y-10 , (enemy.blood * 50) / 4, 5};
        SDL_SetRenderDrawColor(graphics.renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(graphics.renderer, &healthBar);
    }


    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
    [](const Enemy &e) { return !e.active; }), enemies.end());



        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_X]) {
    if (mouse.playerMN >= 5) {
        if (bullets2.size() < 5) {
            int bulletX = mouse.x + (mouse.facingRight ? 160 : 30);
            int bulletY = mouse.y + 175;
            int bulletDir = (mouse.facingRight ? 1 : -1);


            bullets2.push_back(Bullet(bulletX, bulletY, bulletDir, 2));
            Mix_PlayChannel(-1, graphics.shootSound, 0);
            mouse.playerMN = 0;
        }
    }
}
while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) quit = true;
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int bulletX = mouse.x + (mouse.facingRight ? 160 : 30);
                int bulletY = mouse.y + 177;
                int bulletDir = (mouse.facingRight ? 1 : -1);
      bullets.push_back(Bullet(bulletX, bulletY, bulletDir,1));
      Mix_PlayChannel(-1, graphics.shootSound, 0);
}

        }
         if (currentKeyStates[SDL_SCANCODE_LEFT]||currentKeyStates[SDL_SCANCODE_A]) {mouse.quatrai();graphics.render(mouse.x,mouse.y,manflip);}

        else if (currentKeyStates[SDL_SCANCODE_RIGHT]||currentKeyStates[SDL_SCANCODE_D]) {mouse.quaphai();graphics.render(mouse.x,mouse.y,man);}
        else if (currentKeyStates[SDL_SCANCODE_UP]||currentKeyStates[SDL_SCANCODE_W]) {mouse.len();graphics.render(mouse.x,mouse.y,man);}
        else if (currentKeyStates[SDL_SCANCODE_DOWN]||currentKeyStates[SDL_SCANCODE_S]) {mouse.xuong();graphics.render(mouse.x,mouse.y,man);}
        else if(mouse.facingRight&&currentKeyStates[SDL_KEYUP]) {mouse.stop();graphics.render(mouse.x,mouse.y,manidle); }
        else if(!mouse.facingRight&&currentKeyStates[SDL_KEYUP]) {mouse.stop();graphics.render(mouse.x,mouse.y,manidleflip); }

        mouse.move();
        if (mouse.playerMN == 5 && !hasDisplayedText) {
            displayTime = SDL_GetTicks();
            hasDisplayedText = true;
        }

        if (hasDisplayedText && SDL_GetTicks() - displayTime < 1000) {
            graphics.renderTexture(ultText,0 ,100);
        }

        if (hasDisplayedText && SDL_GetTicks() - displayTime >= 1000) {
            hasDisplayedText = false;
        }


        graphics.renderHealthBar(graphics.renderer,mouse.playerHP,5,0,10);
        graphics.renderManaBar(graphics.renderer,mouse.playerMN,5,0,50);
        SDL_Color infoColor = {255, 255, 255, 255};
            SDL_Texture* waveInfo = graphics.renderText(
                ("Wave: " + std::to_string(currentWave)).c_str(),
                font,
                infoColor
            );
            graphics.renderTexture(waveInfo, 1800, 10);
            SDL_DestroyTexture(waveInfo);
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
    [](const Bullet &b) { return !b.active; }), bullets.end());

    bullets2.erase(std::remove_if(bullets2.begin(), bullets2.end(),
    [](const Bullet &b) { return !b.active; }), bullets2.end());
        graphics.presentScene();
        SDL_Delay(10);
    }}bool inGameOver = true;
while (inGameOver) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            inGameOver = false;
            quit = true;
        }

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        bool wasHovered2 = restartButton.isHovered || quitButton.isHovered;
        restartButton.checkHover(mouseX, mouseY);
        quitButton.checkHover(mouseX,mouseY);
        bool isHovered2 = restartButton.isHovered || quitButton.isHovered;
        if(!wasHovered2&&isHovered2){
            Mix_PlayChannel(-1, graphics.menuHoverSound, 0);
        }
        if (restartButton.handleEvent(&e)) {
            Mix_PlayChannel(-1, graphics.menuSelectSound, 0);
            restartGame = true;
            inGameOver = false;
        }
        if (quitButton.handleEvent(&e)) {
            Mix_PlayChannel(-1, graphics.menuSelectSound, 0);
            quit = true;
            inGameOver = false;
        }
    }
    if (score > bestScore) {
    bestScore = score;
    saveBestScore(score);
}

    graphics.prepareScene(gameover);
    restartButton.render(graphics.renderer);
    quitButton.render(graphics.renderer);
    graphics.presentScene();
}} while (restartGame && !quit);

    SDL_DestroyTexture(menu);
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(gameover);
    SDL_DestroyTexture(ultText);
    SDL_DestroyTexture(startButton.normalTexture);
    SDL_DestroyTexture(startButton.hoverTexture);
    SDL_DestroyTexture(settingsButton.normalTexture);
    SDL_DestroyTexture(settingsButton.hoverTexture);
    SDL_DestroyTexture(quitButton.normalTexture);
    SDL_DestroyTexture(quitButton.hoverTexture);
    SDL_DestroyTexture(backButton.normalTexture);
    SDL_DestroyTexture(backButton.hoverTexture);
    SDL_DestroyTexture(restartButton.normalTexture);
    SDL_DestroyTexture(restartButton.hoverTexture);
    SDL_DestroyTexture(manTexture);
    SDL_DestroyTexture(manidleTexture);
    SDL_DestroyTexture(manflipTexture);
    SDL_DestroyTexture(manidleflipTexture);
    SDL_DestroyTexture(mon2Texture);
    SDL_DestroyTexture(mon2attackTexture);
    SDL_DestroyTexture(mon2attackedTexture);
    SDL_DestroyTexture(shotTexture);
    TTF_CloseFont(font);
    TTF_CloseFont(smallFont);
    Mix_FreeMusic(graphics.menuOpenSound);
    Mix_FreeMusic(graphics.backgroundMusic);
    Mix_FreeChunk(graphics.menuHoverSound);
    Mix_FreeChunk(graphics.menuSelectSound);
    Mix_FreeChunk(graphics.hitSound);
    graphics.quit();
    return 0;

}

