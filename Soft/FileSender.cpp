#include "../include.cpp"
#include "ImGuiFileDialog.cpp"

void FileSend(){
    // open Dialog Simple
    if (ImGui::Button("Open File Dialog"))
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", ".");
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")){
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
        }
        ImGuiFileDialog::Instance()->Close();
    }
}