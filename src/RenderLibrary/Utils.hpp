#pragma once

namespace ShaderPlayground
{
    inline static const char* GetApplicationPathLaunch()
    {
        static std::string pathString;
        std::filesystem::path currentPath = std::filesystem::current_path();
        pathString = currentPath.string();
        return pathString.c_str();
    }

    inline static std::string CombinedPathForResources(const char* fileName, const char* getPath = GetApplicationPathLaunch())
    {
        static std::string appPath = getPath;
        std::filesystem::path filePath = appPath;
        filePath.append(fileName);
        return filePath.string();
    }
}
