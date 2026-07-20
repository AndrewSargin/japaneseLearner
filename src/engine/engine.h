#pragma once

#include <vector>
#include <string>
#include "dictionary.h"
#include "subtitle_parser.h"

class Engine {
    
    public:
        Engine(const std::string& db_path);
        ~Engine();
        void loadSubtitles(const std::string& filepath);
        std::string getPhraseAtTime(double time);

    private:
        std::vector<Subtitle> subtitles;
        Dictionary dictionary;
};
