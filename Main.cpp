// for initializing and shutdown functions
#include <SDL2/SDL.h>
 // for rendering images and graphics on screen
#include <SDL2/SDL_image.h>
 // for using SDL_Delay() functions
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include "Widgets.h"
#pragma comment(lib, "user32.lib")
using namespace std;

uint16_t nb_Frame = 0, FPS = 0;
bool isFullScreen = true,close = false;
string path = "Home >";
vector<Widget*> all;
vector<bool> active = {false,false};
int8_t mouse = 0;

// mettre une initialisation et des menu séparer pour améliorer les performances
// FAIRE LES EVENTS

void toggleFullScreen(SDL_Window* window){
    isFullScreen = !isFullScreen;
    if (!isFullScreen){
        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        auto Width = DM.w, Height = DM.h;
        SDL_SetWindowSize(window,Width,Height);
    }
    else SDL_SetWindowSize(window,800,800);
    SDL_SetWindowFullscreen(window, !isFullScreen);
}

void CircleLoad(SDL_Rect b, int rad, int thick, int deg){
    all.push_back(new Arc(b,rad,thick,deg,80+deg)); deg += 120;
    all.push_back(new Arc(b,rad,thick,deg,80+deg)); deg += 120;
    all.push_back(new Arc(b,rad,thick,deg,80+deg));
}

void ListFuturist(vector<string> liste, int x, int y, SDL_Rect b){
    int i = 0, lisize = liste.size(), sizettf = 14, sizecircle = 2, separation = 18;
    for (; i < lisize; i++){
        all.push_back(new Round({b.x+separation/2,b.y+i*separation+separation/2,0,0},sizecircle));
        all.push_back(new Label({b.x+separation,b.y+i*separation-(sizettf+sizecircle-separation)/2,100,sizettf},liste[i],sizettf));
        SDL_Rect lblb = all[all.size()-1]->b;
        if (lblb.x <= x && x <= lblb.x+lblb.w && lblb.y <= y && y <= lblb.y+lblb.h){
            if (mouse%2==1) active[0]=true;
            if (active[0]) CircleLoad({lblb.x-separation/2,lblb.y+separation/2-1,0,0},6,2,30);
            else CircleLoad({lblb.x-separation/2,lblb.y+separation/2-1,0,0},6,2,nb_Frame<<2);
        }
    }
    i = b.y+(lisize-0.2)*separation;
    all.push_back(new Line(b.x,b.y,b.x,i));
    all.push_back(new Line(b.x,i,5+separation/1.5+b.x,i+separation/2));
}

void base_interface(int x, int y, int w, int h){
    all.push_back(new Label({0,0,0,0},path,12));
    all.push_back(new Label({0,h-14,0,0},to_string(x)+"/"+to_string(w)+" : "+to_string(y)+"/"+to_string(h)
        +" : " + to_string(FPS) + " FPS",14));
    ListFuturist({"Visual Scripting","Modelisation 3D","Image","Son","Moteur de jeu"},x,y,{5,20,0,0});
}

void main_loop(SDL_Renderer* rend, SDL_Window* win){
    //event
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: close = true; break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_F11:toggleFullScreen(win); break;
                    default:break;
                }
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button)
                {
                    case SDL_BUTTON_LEFT: mouse=1;
                    case SDL_BUTTON_RIGHT: mouse+=2;
                    case SDL_BUTTON_MIDDLE: mouse+=4; break;
                    default:break;
                }
        }
    }
    //affichage
    SDL_RenderClear(rend);

    all.clear(); 
    int x, y, w, h; SDL_GetMouseState(&x, &y); SDL_GetWindowSize(win, &w, &h);
    base_interface(x,y,w,h);

    CircleLoad({300,300,0,0},60,15,nb_Frame<<1);
    //all.push_back(new Image({200,10,0,0},"World_map_-_low_resolution.png"));
    //all.push_back(new DNA({30,200,50,200},15,1.5*nb_Frame));
    for (Widget* w : all) w->Render(rend);
    nb_Frame++; if (nb_Frame>=1000) nb_Frame %= 314;
}

int main(int argc, char *argv[]){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        printf("error initializing SDL: %s\n", SDL_GetError());
    TTF_Init();
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); 
    SDL_Window* win = SDL_CreateWindow(
        "Launcher - AlphaSoftware", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, 0);
    SDL_SetWindowResizable(win, SDL_TRUE);
    
    Uint32 render_flags = SDL_RENDERER_ACCELERATED+SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    chrono::_V2::steady_clock::time_point start;
    chrono::duration<double> duree;
    while (!close){
        start = chrono::steady_clock::now();
        mouse = 0;
        main_loop(rend,win);
        SDL_SetRenderDrawColor(rend,20,20,20,0);
        SDL_RenderPresent(rend);
        duree = chrono::steady_clock::now()-start;
        FPS = 1/(duree.count());
    }
    SDL_DestroyRenderer(rend); SDL_DestroyWindow(win); TTF_Quit(); SDL_Quit(); return 0;
}
