#include "config.h"
#include "editor.h"
#include <iostream>
#include <fstream>
#include <string>

void LoadConfig()
{
    std::ifstream configFile("cfg.ini");
    if (configFile.is_open())
    {
        std::string line;
        while (std::getline(configFile, line))
        {
            if (line.find("boxWidth") != std::string::npos)
                boxWidth = std::stof(line.substr(line.find("=") + 1));
            else if (line.find("boxHeight") != std::string::npos)
                boxHeight = std::stof(line.substr(line.find("=") + 1));
            else if (line.find("gap") != std::string::npos)
                gap = std::stof(line.substr(line.find("=") + 1));
            else if (line.find("offsetX") != std::string::npos)
                offsetX = std::stof(line.substr(line.find("=") + 1));
            else if (line.find("offsetY") != std::string::npos)
                offsetY = std::stof(line.substr(line.find("=") + 1));
            else if (line.find("lineThickness") != std::string::npos)
                lineThickness = std::stof(line.substr(line.find("=") + 1));
            else if (line.find("cornerLength") != std::string::npos)
                cornerLength = std::stof(line.substr(line.find("=") + 1));
            else if (line.find("healthPercent") != std::string::npos)
                healthPercent = std::stof(line.substr(line.find("=") + 1));
            else if (line.find("healthBarHeight") != std::string::npos)
                healthBarHeight = std::stof(line.substr(line.find("=") + 1));
            else if (line.find("healthBarOffset") != std::string::npos)
                healthBarOffset = std::stof(line.substr(line.find("=") + 1));
            else if (line.find("nicknameOffsetY") != std::string::npos)
                nicknameOffsetY = std::stof(line.substr(line.find("=") + 1));
            else if (line.find("distanceOffsetY") != std::string::npos)
                distanceOffsetY = std::stof(line.substr(line.find("=") + 1));
            else if (line.find("pingOffsetY") != std::string::npos)
                pingOffsetY = std::stof(line.substr(line.find("=") + 1));
        }
        configFile.close();
        std::cout << "Config loaded successfully.\n";
    }
    else
    {
        std::cout << "Unable to open config file.\n";
    }
}

void SaveConfig()
{
    std::ofstream configFile("cfg.ini");
    if (configFile.is_open())
    {
        configFile << "boxWidth=" << boxWidth << "\n";
        configFile << "boxHeight=" << boxHeight << "\n";
        configFile << "gap=" << gap << "\n";
        configFile << "offsetX=" << offsetX << "\n";
        configFile << "offsetY=" << offsetY << "\n";
        configFile << "lineThickness=" << lineThickness << "\n";
        configFile << "cornerLength=" << cornerLength << "\n";
        configFile << "healthPercent=" << healthPercent << "\n";
        configFile << "healthBarHeight=" << healthBarHeight << "\n";
        configFile << "healthBarOffset=" << healthBarOffset << "\n";
        configFile << "nicknameOffsetY=" << nicknameOffsetY << "\n";
        configFile << "distanceOffsetY=" << distanceOffsetY << "\n";
        configFile << "pingOffsetY=" << pingOffsetY << "\n";
        configFile.close();
        std::cout << "Config saved successfully.\n";
    }
    else
    {
        std::cout << "Unable to open config file for writing.\n";
    }
}
