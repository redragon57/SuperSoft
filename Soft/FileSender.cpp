#include "../include.cpp"

std::string txt = "lol";

void FileSend(){
    if (ImGui::Button("Open File Dialog")){
        txt = openFileDialog();
    }
    ImGui::Text("%s",txt.c_str());
}