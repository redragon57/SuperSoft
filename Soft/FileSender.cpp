#include "../include.cpp"

char * txt = "lol";

char * openfilename() {
    char filename[1024];
    FILE *f = popen("zenity --file-selection", "r");
    fgets(filename, 1024, f);
    return filename;
}

void FileSend(){
    if (ImGui::Button("Open File Dialog")){
        txt = openfilename();
    }
    ImGui::Text(txt);
}