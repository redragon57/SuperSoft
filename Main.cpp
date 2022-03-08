#include "imgui-master/imgui.h"
#include "imgui-master/backends/imgui_impl_sdl.h"
#include "imgui-master/backends/imgui_impl_sdlrenderer.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>
#include <string>

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

std::vector<std::string> active, groupsoft = 
{"Outil à la création","Réseau","Sécurité","Gestion de fichier","Internet","Biologie","Physique",
"Math","Economie","Divers","Outil favoris","Paramétre"};
std::vector<std::vector<std::string>> software = {
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

void Home(ImGuiIO& io){
    static float f = 0.0f;
    ImGui::Text("Home > ");
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}

int main(int, char**){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0){
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("SuperSoft", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, window_flags);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return false;
    }
    //SDL_RendererInfo info;
    //SDL_GetRendererInfo(renderer, &info);
    //SDL_Log("Current SDL_Renderer: %s", info.name);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        if (show_demo_window){
            Home(io);
        }
        // Rendering
        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }
    // Cleanup
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
