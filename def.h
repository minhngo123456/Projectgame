#ifndef _DEF__H
#define _DEFS__H
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1024;

const char* WINDOW_TITLE = "Dark Castle Expedition";
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 60;
const int TOTAL_BUTTONS = 2;
const char* MON_SPRITE_FILE = "img\\mons2.png";
const int MON2_CLIPS[][4] = {
    {  0, 0, 180, 152},
    { 182.5, 0, 180, 152},
    {182.5*2, 0, 180, 152},
    {182.5*3, 0, 180, 152},
    {182.55*4, 0, 180, 152},
    {182.5*5, 0, 180, 152}};
const int MON2_FRAMES = sizeof(MON2_CLIPS)/sizeof(int)/6;
const char* MONATTACK_SPRITE_FILE = "img\\mons2attack.png";
const int MON2ATTACK_CLIPS[][4] = {
    {  0, 0, 180, 177},
    { 181, 0, 180, 177},
    {181*2, 0, 180, 177},
    {181*3, 0, 180, 177},
    };
const int MON2ATTACK_FRAMES = sizeof(MON2ATTACK_CLIPS)/sizeof(int)/4;

const char* MONATTACKED_SPRITE_FILE = "img\\mons2attacked.png";
const int MON2ATTACKED_CLIPS[][4] = {
    {  0, 0, 165, 174},
    { 174, 0, 160, 174},
    {174*2, 0, 165, 174},
    {174*3, 0, 165, 174},
    };
const int MON2ATTACKED_FRAMES = sizeof(MON2ATTACKED_CLIPS)/sizeof(int)/4;



#endif // _DEF__H
