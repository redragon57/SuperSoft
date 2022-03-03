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

#define PI 3.14159265358979323f

// mettre en place des ancres haut, bas ...
float getAngle(int x, int y){
    return PI-atan2(y,x);
}

//mettre plus de paramétrage comme la couleur ou la police les ellipses...
// Objets //
// Base structure
class Widget{
    public:
        SDL_Rect b = {0,0,0,0}; std::string id;
        virtual void Render(SDL_Renderer *rend)=0;
};

// Forme

class Arc : public Widget{
    public:
        int radius, thick = 2, begin = 0, ending = 359;
        Arc(SDL_Rect r, int rad, int thk = 2, int beg = 0, int end = 359){
            b=r;radius=rad;thick=thk;begin=beg;ending=end;
        }
        void Render(SDL_Renderer *rend) override{
            SDL_SetRenderDrawColor(rend,50,150,250,0);
            begin%=360,ending%=360; b.w = radius*2; b.h = radius*2;
            if (ending<begin){
                int saveend = ending; ending = 359; Render(rend);
                begin = 0; ending = saveend; Render(rend);
            }
            else{
                int dx, dy, res, w = 0, h = 0, rad = (radius-thick)*(radius-thick), rad2 = radius*radius;
                float point_angle, beg = begin * PI / 180, end = ending * PI / 180;
                for (; w < b.w; w++)
                    for (h = 0; h < b.h; h++){
                        dx = (float)radius - w; dy = (float)radius - h; res = dx*dx + dy*dy;
                        point_angle = getAngle(w-radius,h-radius);
                        if (res <= rad2 && res>rad && point_angle<=end && beg <= point_angle)
                            SDL_RenderDrawPoint(rend, b.x + dx, b.y + dy);
                    }
            }
        }
};
class Circle : public Widget{
    public:
        int radius, thick = 2;
        Circle(SDL_Rect r, int rad, int thk = 2){
            b=r;radius=rad;thick=thk;
        }
        void Render(SDL_Renderer *rend) override {
            SDL_SetRenderDrawColor(rend,50,150,250,0);
            b.w = radius*2; b.h = radius*2;
            int dx, dy, res, w = 0, h = 0, rad = (radius-thick)*(radius-thick), rad2 = radius*radius;
            for (; w < radius * 2; w++)
                for (h = 0; h < radius * 2; h++){
                    dx = radius - w; dy = radius - h; res = dx*dx + dy*dy;
                    if (res <= rad2 && res>rad) SDL_RenderDrawPoint(rend, b.x + dx, b.y + dy);
                }
        }
};
class Round : public Widget{
    public:
        int radius = 2;
        Round(SDL_Rect r, int rad = 2){
            b=r;radius=rad;
        }
        void Render(SDL_Renderer *rend) override {
            SDL_SetRenderDrawColor(rend,50,150,250,0);
            b.w = radius*2; b.h = radius*2;
            int dx, dy, w = 0, h = 0, rad = radius*radius;
            for (; w < b.w; w++)
                for (h = 0; h < b.h; h++){
                    dx = radius - w;dy = radius - h;
                    if ((dx*dx + dy*dy) <= rad)
                        SDL_RenderDrawPoint(rend, b.x + dx, b.y + dy);
                }
        }
};
class Line : public Widget{
    public:
        int x1,x2,y1,y2,thick=2;
        Line(int xdeb, int ydeb, int xfin, int yfin, int thk=2){
            x1 = xdeb; x2 = xfin; y1 = ydeb; y2 = yfin; thick = thk;
            if (x1<x2){b.x = x1;b.w = x2-x1;}
            else {b.x = x2;b.w = x1-x2;}
            if (y1<y2){b.y = y1;b.h = y2-y1;}
            else {b.y = y2;b.h = y1-y2;}
        }
        void Render(SDL_Renderer * rend) override {
            SDL_SetRenderDrawColor(rend,50,150,250,0);
            int i = 0;
            if (b.x<b.y) for (; i < thick; i++) SDL_RenderDrawLine(rend,x1+i, y1, x2+i, y2);
            for (; i < thick; i++) SDL_RenderDrawLine(rend,x1, y1+i, x2, y2+i);
        }
};
class Box : public Widget{
    public:
        void Render(SDL_Renderer *rend) override {
            SDL_RenderDrawRect(rend,&b);
        }
};

// Standard object
// pour tout les objets standard, mettre des events automatiquement
// panel, switch button, hover text

class Label : public Widget{
    public:
        std::string txt; int size;
        Label(SDL_Rect r, std::string text, int s){
            b=r;txt=text;size=s;
        }
        void Render(SDL_Renderer *rend) override {
            TTF_Font* police = TTF_OpenFont("Ubuntu/Ubuntu-Regular.ttf", size);
            if (!police) {
                printf("TTF_OpenFont: %s\n", TTF_GetError());
            }
            SDL_Color Blue = {50, 150, 250};
            SDL_Surface* surfaceMessage = TTF_RenderText_Blended(police, txt.c_str(), Blue);
            SDL_Texture* Message = SDL_CreateTextureFromSurface(rend, surfaceMessage);
            int texW = 0,texH = 0;
            SDL_QueryTexture(Message, NULL, NULL, &texW, &texH);
            b = { b.x, b.y, texW, texH };
            SDL_RenderCopy(rend, Message, NULL, &b);
            TTF_CloseFont(police);
        }
};
class TextBox : public Widget{
    public:
        Label txt;
        void Render(SDL_Renderer *rend) override {
            SDL_Color savecol; SDL_GetRenderDrawColor(rend,&savecol.r,&savecol.g,&savecol.b,&savecol.a);
            SDL_SetRenderDrawColor(rend,230,230,230,0);
            SDL_RenderDrawRect(rend,&b);
            SDL_SetRenderDrawColor(rend,savecol.r,savecol.g,savecol.b,savecol.a);
            txt.Render(rend);
        }
};
class RichTextBox : public Widget{
    public:
        void Render(SDL_Renderer *rend) override {
            SDL_Color savecol; SDL_GetRenderDrawColor(rend,&savecol.r,&savecol.g,&savecol.b,&savecol.a);
            SDL_SetRenderDrawColor(rend,230,230,230,0);
            SDL_RenderDrawRect(rend,&b);
            SDL_SetRenderDrawColor(rend,savecol.r,savecol.g,savecol.b,savecol.a);
            //for (Widget w : li) w.Render(rend);
        }
};
class Image : public Widget{
    public:
        // surcharger le constructeur avec une option de proportion et de hauteur/largeur
        std::string path;
        Image(SDL_Rect r,std::string chem){
            b=r;path = chem;
        }
        void Render(SDL_Renderer* rend) override {
            SDL_Texture* Message = SDL_CreateTextureFromSurface(rend, IMG_Load(path.c_str()));
            int texW = 0,texH = 0; SDL_QueryTexture(Message, NULL, NULL, &texW, &texH);
            b = { b.x, b.y, texW/2, texH/2 }; SDL_RenderCopy(rend, Message, NULL, &b);
        }
};

// Advanced object
// 3D, graphique, node, graph
class DNA : public Widget {
    public:
        //factoriser le calcule
        int radius, avance; bool verticale = true;
        DNA(SDL_Rect r, int rad, int avan, bool vert = true){
            b=r; radius = rad; avance = avan; verticale = vert;
        }
        void Sinusoid(SDL_Renderer *rend, int sinus, int h){
            float w = sinus-radius;
            for (; w < sinus; w++) {
                if (verticale) SDL_RenderDrawPoint(rend, b.x+w, b.y+h);
                else SDL_RenderDrawPoint(rend, b.y+h,b.x+w);
            }
        }
        void Render(SDL_Renderer *rend) override{
            SDL_SetRenderDrawColor(rend,50,150,250,0);
            int sinus, sinus2; bool first; float w,h=0;
            uint8_t r,g,b2,a; SDL_GetRenderDrawColor(rend,&r,&g,&b2,&a);
            if (!verticale){
                int save = b.x; b.x = b.y; b.y =save;
            }
            for (; h < b.h; h++){
                sinus = (sin((h+avance)/20)+1)*b.w+radius;
                sinus2 = (sin((h+avance)/20+PI)+1)*b.w+radius;
                first = sinus>(sin((h+avance-2)/20)+1)*b.w+radius;
                if (first) radius += 15; Sinusoid(rend,sinus,h);
                if (!first) radius += 15;Sinusoid(rend,sinus2,h);
                radius-=15;
                if ((int)(h/(radius/2))%2==0){
                    if (sinus2 < sinus){
                        int savesin = sinus; sinus = sinus2; sinus2 = savesin;
                    }
                    if (sinus2-sinus>30){
                        SDL_SetRenderDrawColor(rend, r-50,g-50,b2-50,a);
                        for (w = sinus-radius/2; w < sinus2-radius/2; w++) {
                            if (verticale) SDL_RenderDrawPoint(rend, b.x+w, b.y+h);
                            else SDL_RenderDrawPoint(rend, b.y+h,b.x+w);
                        }
                        SDL_SetRenderDrawColor(rend, r,g,b2,a);
                    }
                }
            }
        }
};
