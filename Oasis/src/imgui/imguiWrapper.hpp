#pragma once

#include <vector>
#include <functional>

struct ImGuiContext;
namespace Oasis
{
    class Event;
    class ImGuiWrapper
    {
    public:
        static void Init();
        static void Shutdown();

        static void AddWindowFunction(std::function<void()> func);

        static void OnEvent(const Event& event);
        static void Update(float deltaTime);

        static ImGuiContext * GetContext();
    private:
        static std::vector<std::function<void()>> s_windowFunctions;
        static ImGuiContext * s_context;
    };
}