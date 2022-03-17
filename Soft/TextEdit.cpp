#include "../include.cpp"

struct Funcs{
    static int MyResizeCallback(ImGuiInputTextCallbackData* data){
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
            ImVector<char>* my_str = (ImVector<char>*)data->UserData;
            IM_ASSERT(my_str->begin() == data->Buf);
            my_str->resize(data->BufSize); // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
            data->Buf = my_str->begin();
        }
        return 0;
    }

    static bool MyInputTextMultiline(const char* label, ImVector<char>* my_str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        return ImGui::InputTextMultiline(label, my_str->begin(), (size_t)my_str->size(), size, flags | ImGuiInputTextFlags_CallbackResize, Funcs::MyResizeCallback, (void*)my_str);
    }
};

void TxtEdit(){
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
            if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    static ImVector<char> my_str;
    if (my_str.empty()) my_str.push_back(0);
    Funcs::MyInputTextMultiline("##MyStr", &my_str, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16));
    ImGui::Text("Data: %p\nSize: %d\nCapacity: %d", (void*)my_str.begin(), my_str.size(), my_str.capacity());
}