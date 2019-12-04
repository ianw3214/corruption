#pragma once

#include <string>

namespace Oasis
{
    constexpr int kMaxSavedLines = 30;
    class Console
    {
    public:
        static void Init();
        static void Shutdown();

        static void Print(const std::string& str);
    private:
        static int s_start;
        static int s_size;
        static std::string s_lines[kMaxSavedLines];
    };
}