#include "imgui-master/imgui.h"
#include "imgui-master/backends/imgui_impl_sdl.h"
#include "imgui-master/backends/imgui_impl_sdlrenderer.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

bool fullscreen = false;
std::vector<const char *> show_window, groupsoft = 
{"Outil à la création","Réseau","Sécurité","Gestion de fichier","Internet","Biologie","Physique",
"Math","Economie","Divers","Outil favoris","Paramétre","Proposition"};
std::vector<std::vector<const char *>> software = {
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
    "Minage de cryptomonnaie","Musique","Vidéo","Alerteur d'événement","Spammer"},{}};


// faire toggle fullscreen
// faire fond hexagone

void togglefullscreen(ImGuiIO& io){
    if(fullscreen) {
        io.ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;
        ImGui::UpdatePlatformWindows();
    }
    else io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    fullscreen = !fullscreen;
    printf("lol\n");
}

template <typename T>
bool contains(std::vector<T> vec, const T & elem){
    if (find(vec.begin(), vec.end(), elem) != vec.end()) return true;
    return false;
}

void Home(ImGuiIO& io){
    //ImDrawList* draw_list = ImGui::GetWindowDrawList();
    //const ImU32 col = ImColor(ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    //draw_list->AddCircleFilled(ImVec2(12.0f, 12.0f), 12.0f, col, 6);
    int j, i = 0;
    ImGui::DockSpaceOverViewport();
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Software")) {
            for (; i < groupsoft.size()-2; i++)
                if (ImGui::BeginMenu(groupsoft[i])) {
                    for (j = 0; j < software[i].size(); j++)
                        if (ImGui::MenuItem(software[i][j]))
                            show_window.push_back(software[i][j]);
                    ImGui::EndMenu();
                }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("About")) {
            ImGui::ShowAboutWindow(); ImGui::EndMenu();
        }
        ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
        ImGui::EndMainMenuBar();
    }
}

void FunctionExecutor(const char* s){
    bool show = contains(show_window,s);
    if(show){
        int i = 0;
        if(!ImGui::Begin(s, &show)) ImGui::End();
        else{
            ImGui::Text("%s",s);
            switch(i){
                case 0: break;
                default: break;
            }
            ImGui::End();
        }
    }
    if (!show) remove(show_window.begin(),show_window.end(),s);
}

int main(int, char**){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0){
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("SuperSoft", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, window_flags);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log("Error creating SDL_Renderer!");
        return EXIT_FAILURE;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontFromFileTTF("Ubuntu/Ubuntu-Light.ttf", 16.0f);
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingWithShift = true;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);
    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);

    std::vector<const char *> all;
    for(auto && v : software) all.insert(all.end(), v.begin(), v.end());

    bool done = false;
    ImGuiStyle* style = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Text] = ImVec4(0.2f, 0.6f, 1.0f, 1.00f);
    // Main loop
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) done = true;
            if (event.type == SDL_WINDOWEVENT && 
            event.window.event == SDL_WINDOWEVENT_CLOSE && 
            event.window.windowID == SDL_GetWindowID(window)) done = true;
            if (SDL_KEYDOWN==event.type)
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_F11: togglefullscreen(io); break;
                    default:break;
                }
        }
        ImGui_ImplSDLRenderer_NewFrame(); ImGui_ImplSDL2_NewFrame(); ImGui::NewFrame();
        Home(io); for (const char* s : all) FunctionExecutor(s);
        // Rendering
        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
        //SDL_Delay(1000/72);
    }
    // Cleanup
    ImGui_ImplSDLRenderer_Shutdown(); ImGui_ImplSDL2_Shutdown(); ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window); SDL_Quit();
    return 0;
}
