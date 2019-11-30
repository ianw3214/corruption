#include "timer.hpp"

#include <imgui.h>

std::vector<Profiler::Profile> Profiler::s_profiles;

void Profiler::Init()
{
    // TODO: REPLACE THIS WITH PROPER INITIALIZATION CODE
    static bool init = false;
    if (init) return;
    init = true;

    Oasis::ImGuiWrapper::AddWindowFunction(&Profiler::ImGuiFunc);
}

void Profiler::AddProfile(const char* name, float time)
{
    s_profiles.emplace_back(name, time);
}

void Profiler::Update()
{
    s_profiles.clear();
}

void Profiler::ImGuiFunc()
{
    ImGui::SetCurrentContext(Oasis::ImGuiWrapper::GetContext());
    static bool show = true;
    ImGui::Begin("PROFILER INFO", &show, ImGuiWindowFlags_MenuBar);
    for (const Profile& profile : s_profiles)
    {
        ImGui::Text("%s: %f ms", profile.m_name, profile.m_time);
    }
    ImGui::End();
}