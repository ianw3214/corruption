#include "console.hpp"
using namespace Oasis;

#include "imgui/imguiWrapper.hpp"
#include <imgui.h>

int Console::s_start = 0;
int Console::s_size = 0;
std::string Console::s_lines[kMaxSavedLines];

void Console::Init()
{
    ImGuiWrapper::AddWindowFunction([=](){
        static bool show = true;
        ImGui::Begin("CONSOLE", &show);
        for (int i = 0; i < s_size; ++i)
        {
            int index = s_start + i;
            if (index >= kMaxSavedLines)
            {
                index -= kMaxSavedLines;
            }
            ImGui::Text(s_lines[index].c_str());
        }
        ImGui::SetScrollHere(0.999f);
        ImGui::End();
    });
}

void Console::Shutdown()
{

}

void Console::Print(const std::string& str)
{
    // There's no way for the size to be greater than max AND start != 0, so just insert to end
    if (s_size < kMaxSavedLines)
    {
        s_lines[s_size] = str;
        s_size++;
    }
    else
    {
        s_lines[s_start] = str;
        s_start++;
        if (s_start >= kMaxSavedLines)
        {
            s_start = 0;
        }
    }
    
}
