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
#include <algorithm>
#include <bits/stdc++.h>
#include "Widgets.h"
#pragma comment(lib, "user32.lib")
using namespace std;

// faire un système de recherche de logiciel

uint16_t nb_Frame = 0, FPS = 0;
bool isFullScreen = true, close = false;
string path = "Home > ";
vector<Widget*> all;
int8_t mouse = 0;
// A modifier (faire toute les disciplines)
vector<string> active, groupsoft = 
{"Outil à la création","Réseau","Sécurité","Gestion de fichier","Internet","Biologie","Physique",
"Math","Economie","Divers","Outil favoris","Paramétre"};
vector<vector<string>> software = {
    {"Visual Scripting","Commentateur de programme","Machine leaming",
    "Convertisseur de langage","Éditeur de texte","Modélisation 3D","Moteur de jeu",
    "Éditeur d'Image","Animation","Music maker","Créateur de clé bootable","SQL",
    "Éditeur de vidéos","Optimiseur de programme","Optimiseur de graphisme",
    "Créateur dOS","Simulateur dOS"},
    {"IP connect","Traceroute map","VPN","Image réseaux"},
    {"Hacker","Antivirus","Crypteur","Espionneur","Voice analyseur","Face récognition",
    "Morphologie calculator","Sound learning"},
    {"PDF","Manageur de fichier","Compresseur de donnée","Optimiseur de fichier"},
    {"Chercheur d'information","Navigateur internet","Tchat","Prise en main à distance",
    "Partage de lien","Sci-hub","Bureau à distance"},
    {"Éditeur de gène","Être vivant","Génétique"},
    {"Matériaux","Planète","Galaxie","Simulation de physique","Inventor","Maison","Pubchem",
    "Manufacture"},
    {"Convertisseur","Matrice","Visualisateur de fonction","Créateur d'exercices",
    "Résolveur d'exercices"},
    {"Bourse","Comptabilité","Caisse enregistreuse","Gestion de compte en banque et d'économie"},
    {"Crackeur de logiciel","OCR","Traducteur","Soundbar","Suggestion musique","Simulateur de shell",
    "Gestionnaire de Raspberry","Simulateur de processeur quantique","Schéma électronique",
    "Roadmap","Copieur de logiciel","Téléphone","Vidéo downloader","Autoinstallateur",
    "Music downloader","Reconnaissance musical","Latex et formule","Traducteur",
    "Remote entre appareil connecté","Hand Controller","Importeur de système à distance",
    "Minage de cryptomonnaie","Musique","Vidéo","Alerteur d'événement","Spammer"},{},{}};

// mettre une initialisation et des menu séparer pour améliorer les performances

void join(const vector<string>& v, string c, string& s) {
    s.clear();
    for (vector<string>::const_iterator p = v.begin(); p != v.end(); ++p) {
        s += *p; if (p != v.end() - 1) s += c;
    }
}

template <typename T>
bool contains(vector<T> vec, const T & elem){
    if (find(vec.begin(), vec.end(), elem) != vec.end()) return true;
    return false;
}

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

#pragma region interface

void CircleLoad(SDL_Rect b, int rad, int thick, int deg){
    all.push_back(new Arc(b,rad,thick,deg,80+deg)); deg += 120;
    all.push_back(new Arc(b,rad,thick,deg,80+deg)); deg += 120;
    all.push_back(new Arc(b,rad,thick,deg,80+deg));
}

void ListFuturist(vector<string> liste, int x, int y, SDL_Rect b){
    int asize = active.size(), i = 0, lisize = liste.size(), sizettf = 14, sizecircle = 2, separation = 18;
    SDL_Rect lblb; bool isactive, isin;
    join(active, " > ", path);
    path = "Home > " + path;
    if (asize!=0) {
        if (asize<2) liste = software[find(liste.begin(),liste.end(),active[0])-liste.begin()];
        else liste.clear();
        liste.insert(liste.begin(),active.begin(),active.end());
    }
    lisize = liste.size();
    for (; i < lisize; i++){
        lblb = {b.x+separation,b.y+i*separation-(sizettf+sizecircle-separation)/2,100,sizettf};
        isactive = contains(active, liste[i]);
        isin = lblb.x <= x && x <= lblb.x+lblb.w && lblb.y <= y && y <= lblb.y+lblb.h;
        all.push_back(new Round({b.x+separation/2,b.y+i*separation+separation/2,0,0},sizecircle));
        all.push_back(new Label(lblb,liste[i],sizettf));
        if (isactive){
            if (mouse%2==1 && isin) active.erase(active.end());
            CircleLoad({lblb.x-separation/2,lblb.y+separation/2-1,0,0},6,2,90);
            b.x+=separation/2; b.y+=separation/2;
        }
        else if (isin){
            if (mouse%2==1) active.push_back(liste[i]);
            CircleLoad({lblb.x-separation/2,lblb.y+separation/2-1,0,0},6,2,nb_Frame<<2);
        }
    }
    if (asize<2) {
        i = b.y+(lisize-0.2)*separation;
        all.push_back(new Line(b.x,b.y,b.x,i));
        all.push_back(new Line(b.x,i,5+separation/1.5+b.x,i+separation/2));
    }
}

void base_interface(int x, int y, int w, int h){
    all.push_back(new Label({0,0,0,0},path,12));
    all.push_back(new Label({0,h-14,0,0},to_string(x)+"/"+to_string(w)+" : "+to_string(y)+"/"+to_string(h)
        +" : " + to_string(FPS) + " FPS",14));
    ListFuturist(groupsoft,x,y,{5,20,0,0});
}

#pragma endregion

//faire une seul texture au démarrage et la rappeller

void background(int w, int h){
    Hexagone *hexa = new Hexagone({0,0,40,20});
    SDL_Surface *hex = hexa->sur;
    int j, i = 0;
    for (; i < h/24+2; i++)
        for (j=0; j < w/40+2; j++){
            all.push_back(new Surface({j*44-22,-11+i*24,40,20},hex));
            all.push_back(new Surface({j*44,i*24,40,20},hex));
        }
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
                switch (event.button.button){
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
    //background(w,h);
    base_interface(x,y,w,h);

    all.push_back(new Graphique({200,200,400,100},{1,1,2,3,5,8,13,21,34,5}));
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
    
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, 
    SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_TARGETTEXTURE);
    chrono::_V2::steady_clock::time_point start;
    chrono::duration<double> duree;
    while (!close){
        start = chrono::steady_clock::now(); mouse = 0;
        main_loop(rend,win);
        SDL_SetRenderDrawColor(rend,20,20,20,0);
        SDL_RenderPresent(rend);
        SDL_Delay(1000/70);
        duree = chrono::steady_clock::now()-start;
        FPS = 1/(duree.count());
    }
    SDL_DestroyRenderer(rend); SDL_DestroyWindow(win); TTF_Quit(); SDL_Quit(); return 0;
}
