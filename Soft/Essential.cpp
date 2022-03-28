#include "../include.cpp"
#include "ImGuiFileDialog/ImGuiFileDialog.cpp"
#include "ImGuiFileDialog/ImGuiFileDialog.h"

void MultiLineText(const char * s){
    std::istringstream f(s); std::string line;    
    while (std::getline(f, line)) ImGui::Text("%s",line.c_str());
}

std::string openFileDialog(){
    std::string filePath = "";
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".wav,.flac,.mp3,.*", ".");
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()){
            std::string filePathName = ImGuiFileDialog::Instance()->GetCurrentFileName();
            filePath = ImGuiFileDialog::Instance()->GetFilePathName();
        }
        ImGuiFileDialog::Instance()->Close();
    }
    return filePath;
}
