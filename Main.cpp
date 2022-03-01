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

int nb_Frame = 0, FPS = 0;
bool isFullScreen = true;
string path = "Home >";
vector<Widget*> all;

// mettre une initialisation et des menu séparer pour améliorer les performances

void toggleFullScreen(SDL_Window* window){
    isFullScreen = !isFullScreen;
    if (!isFullScreen){
        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        auto Width = DM.w;
        auto Height = DM.h;
        SDL_SetWindowSize(window,Width,Height);
    }
    else SDL_SetWindowSize(window,800,800);
    SDL_SetWindowFullscreen(window, !isFullScreen);
}

void ListFuturist(vector<string> liste, int x, int y, SDL_Rect b){
    int lisize = liste.size(), sizettf = 14, sizecircle = 2, separation = 18; Line l1, l2;
    for (int i = 0; i < lisize; i++){
        Round rnd; rnd.b = {b.x+separation/2,b.y+i*separation+separation/2,0,0}; rnd.radius = sizecircle;
        Label lbl; lbl.txt = liste[i]; lbl.b = {b.x+separation,b.y+i*separation-(separation+sizettf+sizecircle)/2,0,0};
        lbl.size = sizettf; //all.push_back(&rnd); all.push_back(&lbl);
        if (lbl.b.x <= x && x <= lbl.b.x+lbl.b.w && lbl.b.y <= y && y <= lbl.b.y+lbl.b.h){
            int deg = nb_Frame*4; 
            Arc a; a.b = {lbl.b.x,lbl.b.y+i*separation,0,0}; a.radius = 6; a.thick = 2; a.begin = deg; a.ending = 80+deg; //all.push_back(&a); 
            deg += 120; Arc a2 = a; a2.begin = deg; a2.ending = deg+80; //all.push_back(&a2);
            deg += 120; Arc a3 = a; a3.begin = deg; a3.ending = deg+80; //all.push_back(&a3);
        }
    }
    int end = b.y+(lisize-0.2)*separation;
    l1.x1 = b.x; l1.y1 = b.y; l1.x2 = b.x; l1.y2 = end;
    l2.x1 = b.x; l2.y1 = end; l2.x2 = 5+separation/1.5+b.x; l2.y2 = end+separation/2;
    all.push_back(&l1); all.push_back(&l2);
}

uint8_t main_loop(SDL_Renderer* rend, SDL_Window* win){
    uint8_t close = 0;
    //event
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: close = 1; break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_F11:toggleFullScreen(win); break;
                    default:break;
                }
        }
    }
    //affichage
    SDL_RenderClear(rend);

    all.clear();

    // side bar
    int x, y, w, h; SDL_GetMouseState(&x, &y); SDL_GetWindowSize(win, &w, &h);
    Label lblinfo; lblinfo.txt = to_string(x)+"/"+to_string(w)+" : "+to_string(y)+"/"+to_string(h)
        +" : " + to_string(FPS) + " FPS";
    lblinfo.b = {0,h-14,0,0}; lblinfo.size = 14;
    all.push_back(&lblinfo);

    Label lblpath; lblpath.txt = path; lblpath.size = 12;
    all.push_back(&lblpath);

    ListFuturist({"Visual Scripting","Modelisation 3D","Image","Son","Moteur de jeu"},x,y,{5,15,0,0});
    
    // Home

    int deg = nb_Frame*2;
    Arc a; a.b = {200,200,0,0}; a.radius = 60; a.thick = 15; a.begin = deg; a.ending = deg+80; all.push_back(&a);
    deg += 120; Arc a2 = a; a2.begin = deg; a2.ending = deg+80; all.push_back(&a2);
    deg += 120; Arc a3 = a; a3.begin = deg; a3.ending = deg+80; all.push_back(&a3);

    Image img; img.b = {200,10,0,0}; img.path = "World_map_-_low_resolution.png";
    all.push_back(&img);

    DNA dna; dna.b = {0,200,50,200}; dna.radius = 15; dna.avance = 1.5*nb_Frame;
    all.push_back(&dna);

    for (Widget* w : all) w->Render(rend);
    nb_Frame++; if (nb_Frame>=1000) nb_Frame %= 314;
    /*
    Text(rend,to_string(x)+"/"+to_string(w)+" : "+to_string(y)+"/"+to_string(h),0,h-14,14);
    */
    
    return close;
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
    
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    uint8_t close = 0;
    while (close%2==0){
        auto start = chrono::steady_clock::now();
        close = main_loop(rend,win);
        SDL_SetRenderDrawColor(rend,20,20,20,0);
        SDL_RenderPresent(rend);
        SDL_Delay(1000 / 70);//70 FPS
        chrono::duration<double> duree = chrono::steady_clock::now()-start;
        FPS = 1/(duree.count());
    }
    SDL_DestroyRenderer(rend);SDL_DestroyWindow(win);TTF_Quit();SDL_Quit();return 0;
}
