#pragma once

namespace StringRender
{
    std::string wchar_to_utf8(const wchar_t* wstr) {
        if (wstr == nullptr)
        {
            return "";
        }

        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
        if (size_needed == 0)
        {
            return "";
        }

        std::vector<char> buffer(size_needed);
        if (WideCharToMultiByte(CP_UTF8, 0, wstr, -1, buffer.data(), size_needed, nullptr, nullptr) == 0)
        {
            return "";
        }
        return std::string(buffer.data());
    }

    std::wstring utf8_to_wchar(const char* str)
    {
        if (str == nullptr)
        {
            return L"";
        }

        int size_needed = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
        if (size_needed == 0)
        {
            return L"";
        }

        std::vector<wchar_t> buffer(size_needed);
        if (MultiByteToWideChar(CP_UTF8, 0, str, -1, buffer.data(), size_needed) == 0)
        {
            return L"";
        }

        return std::wstring(buffer.data());
    }
}
