#include <iostream>
#include <SDL.h>
#include<SDL_image.h>
#include"move.h"
#include <vector>
#include "bullet.h"
#include"Enemy.h"
#include<bits/stdc++.h>
#include"graphics.h"
#include<SDL_ttf.h>
std::vector<Bullet> bullets;
std::vector<Enemy> enemies;
std::vector<Bullet>bullets2;
Uint32 lastSpawnTime=0;
Uint32 displayTime=0;
bool hasDisplayedText=false;
using namespace std;

bool checkCollision(const Move &mouse, const Enemy &enemy) {
    SDL_Rect playerRect = {mouse.x, mouse.y, 256, 256};
    SDL_Rect enemyRect = {enemy.x, enemy.y, 500, 500};

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


int main(int argc, char *argv[])
{
    enum GameState {
    MENU,
    PLAYING,
    EXIT
    };

    Graphics graphics;
    graphics.init();
    graphics.loadframes();
    SDL_Texture * menu = graphics.loadTexture("img\\menu.png");
    SDL_Texture * background = graphics.loadTexture("img\\background.jpg");
    SDL_Texture * gameover = graphics.loadTexture("img\\gameover.jpg");
    SDL_Texture*EM=graphics.loadTexture("img\\enemy1.png");
    TTF_Font* font = graphics.loadFont("arial.ttf", 24);

    SDL_Color color = {255, 255, 0, 0};
    SDL_Texture* helloText = graphics.renderText("Ready for until, press X!", font, color);
    Button startButton(SCREEN_WIDTH/2 - BUTTON_WIDTH/2, 400);
    startButton.normalTexture = graphics.renderText("Start Game", font, {255, 255, 255, 255});
    startButton.hoverTexture = graphics.renderText("Start Game", font, {255, 215, 0, 255});

    Button quitButton(SCREEN_WIDTH/2 - BUTTON_WIDTH/2, 500);
    quitButton.normalTexture = graphics.renderText("Quit", font, {255, 255, 255, 255});
    quitButton.hoverTexture = graphics.renderText("Quit", font, {255, 215, 0, 255});

    Button restartButton(SCREEN_WIDTH/2 - BUTTON_WIDTH/2, 400);
    restartButton.normalTexture = graphics.renderText("Restart", font, {255, 255, 255, 255});
    restartButton.hoverTexture = graphics.renderText("Restart", font, {255, 215, 0, 255});
    int currentWave = 1;
    Uint32 waveStartTime = SDL_GetTicks();

    Move mouse;
    mouse.x = 0;
    mouse.y = 730;
    GameState gameState=MENU;
    bool quit = false;
    SDL_Event event;
    while (!quit && !gameOver(mouse)) {
            if(gameState==MENU){
            while(SDL_PollEvent(&event)){
                if(event.type==SDL_QUIT){
                    quit=true;
                }
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                startButton.checkHover(mouseX, mouseY);
                quitButton.checkHover(mouseX, mouseY);
                if(startButton.handleEvent(&event)){
                    gameState=PLAYING;
                }
                if(quitButton.handleEvent(&event)){
                    quit = true;
                }
            }
            graphics.prepareScene(menu);
            startButton.render(graphics.renderer);
            quitButton.render(graphics.renderer);
            graphics.presentScene();
            }

            else if(gameState==PLAYING){





    graphics.prepareScene(background);


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
    // Hiển thị số lượt hiện tại



    Uint32 currentTime=SDL_GetTicks();

    if(currentTime>lastSpawnTime+5000){
            int enemiesToSpawn = 3 + currentWave;

                for (int i = 0; i < enemiesToSpawn; i++) {

                    enemies.push_back(Enemy(SCREEN_WIDTH, 530,
                                         4 + currentWave,
                                         2 + (currentWave / 3)));
                }
                lastSpawnTime = currentTime;

    }
    if (currentTime - waveStartTime > 30000) { // 30 giây/wave
                currentWave++;
                waveStartTime = currentTime;

                // Spawn boss mỗi 5 wave
                if (currentWave % 5 == 0) {
                    enemies.push_back(Enemy(SCREEN_WIDTH, 530, 20, 1));
                }

                // Hiển thị thông báo wave
                SDL_Color waveColor = {255, 215, 0, 255};
                SDL_Texture* waveText = graphics.renderText(
                    ("WAVE " + std::to_string(currentWave)).c_str(),
                    font,
                    waveColor
                );
                graphics.renderTexture(waveText, SCREEN_WIDTH/2 - 100, 100);
                SDL_DestroyTexture(waveText);
            }


    for(auto &enemy:enemies){
        enemy.move();
        if(checkCollision(mouse,enemy)){
            mouse.takeDamage(1);
            if(mouse.playerMN>=5){
                mouse.playerMN=5;
            }

            if(mouse.playerHP==0){
            quit=true;
         }
            }

        graphics.renderTexture(EM,enemy.x,enemy.y);
        SDL_Rect healthBarBg = {enemy.x+250, enemy.y-10 , ((4+currentWave)*50)/4, 5};
        SDL_SetRenderDrawColor(graphics.renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(graphics.renderer, &healthBarBg);

        SDL_Rect healthBar = {enemy.x+250, enemy.y-10 , (enemy.blood * 50) / 4, 5};
        SDL_SetRenderDrawColor(graphics.renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(graphics.renderer, &healthBar);
    }
  for (auto &bullet : bullets2) {
    SDL_Rect bulletRect = {bullet.x, bullet.y, 60, 5};
    for (auto &enemy : enemies) {
        SDL_Rect enemyRect = {enemy.x, enemy.y, 500, 500};
        if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
            enemy.takeDamage(5);
            bullet.active = false;
        }
    }
}
    for (auto &bullet : bullets) {
            SDL_Rect bulletRect = {bullet.x, bullet.y, 60, 5};
            bool bulletHit = false;
            for (auto &enemy : enemies) {
                SDL_Rect enemyRect = {enemy.x, enemy.y, 500, 500};
                if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
                    enemy.takeDamage(1);
                    if (bullet.level == 1 && !bullet.hasGivenMana) {
                    mouse.playerMN = std::min(mouse.playerMN + 1, 5);
                    bullet.hasGivenMana = true; // Đánh dấu đã tăng mana
            }

            bulletHit = true; // Đánh dấu đã trúng
            bullet.active = false; // Hủy đạn ngay
             // Thoát vòng lặp enemy

                }
            }
            if (bulletHit) break;
        }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
    [](const Enemy &e) { return !e.active; }), enemies.end());

        while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) quit = true;
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int bulletX = mouse.x + (mouse.facingRight ? 90 : -10);
                int bulletY = mouse.y + 90;
                int bulletDir = (mouse.facingRight ? 1 : -1);

    bullets.push_back(Bullet(bulletX, bulletY, bulletDir,1));
}

        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        bool moving=false;
        if (currentKeyStates[SDL_SCANCODE_X]) {
    if (mouse.playerMN >= 5) {
        if (bullets2.size() < 5) {
            int bulletX = mouse.x + (mouse.facingRight ? 90 : -10);
            int bulletY = mouse.y + 90;
            int bulletDir = (mouse.facingRight ? 1 : -1);


            bullets2.push_back(Bullet(bulletX, bulletY, bulletDir, 2));

            mouse.playerMN = 0;
        }
    }
}
        if (currentKeyStates[SDL_SCANCODE_LEFT]||currentKeyStates[SDL_SCANCODE_A]) {mouse.quatrai();moving=true;}

        else if (currentKeyStates[SDL_SCANCODE_RIGHT]||currentKeyStates[SDL_SCANCODE_D]) {mouse.quaphai();moving=false;}

         else if(currentKeyStates[SDL_KEYUP]) {mouse.stop(); }




        mouse.move();
        if (mouse.playerMN == 5 && !hasDisplayedText) {
            displayTime = SDL_GetTicks();
            hasDisplayedText = true;
        }

        if (hasDisplayedText && SDL_GetTicks() - displayTime < 1000) {
            graphics.renderTexture(helloText,0 ,100);
        }

        if (hasDisplayedText && SDL_GetTicks() - displayTime >= 1000) {
            hasDisplayedText = false;
        }

        render(mouse,graphics);
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
    }}
bool inGameOver = true;
Uint32 gameOverTime = SDL_GetTicks();

while (inGameOver && !quit) {
    Uint32 currentTime = SDL_GetTicks();
    float fade = std::min(1.0f, (currentTime - gameOverTime) / 1000.0f);

    // Xử lý sự kiện
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit = true;
            inGameOver = false;
        }

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Cập nhật trạng thái hover
        restartButton.checkHover(mouseX, mouseY);
        quitButton.checkHover(mouseX, mouseY);

        // Xử lý click chuột
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            if (restartButton.isHovered) {
                // Reset toàn bộ game
                currentWave = 1;
                waveStartTime = SDL_GetTicks();
                mouse = Move(); // Reset player
                enemies.clear();
                bullets.clear();
                bullets2.clear();
                gameState = PLAYING;
                inGameOver = false;
                cout << "Game restarted!" << endl;
            }
            else if (quitButton.isHovered) {
                quit = true;
                inGameOver = false;
            }
        }
    }

    // Render
    graphics.prepareScene(gameover);

    // Hiệu ứng fade
    SDL_SetTextureAlphaMod(gameover, (Uint8)(fade * 255));

    // Render nút
    restartButton.render(graphics.renderer);
    quitButton.render(graphics.renderer);

    // Hiển thị thông tin wave
    SDL_Color textColor = {255, 255, 255, (Uint8)(fade * 255)};
    SDL_Texture* waveText = graphics.renderText(
        ("Reached Wave: " + std::to_string(currentWave)).c_str(),
        font,
        textColor
    );
    graphics.renderTexture(waveText, SCREEN_WIDTH/2 - 150, 300);
    SDL_DestroyTexture(waveText);

    graphics.presentScene();
    SDL_Delay(10);
}
    SDL_DestroyTexture( background );
    background = NULL;
    SDL_DestroyTexture(gameover);
    gameover=NULL;
    graphics.quit();
    return 0;

}

