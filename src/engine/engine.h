#pragma once

#include <vector>
#include <string>
#include "dictionary.h"
#include "subtitle_parser.h"

class Engine {
    
    public:
        // Constructs a class and initializes a dictionary
        Engine(const std::string& db_path);
        ~Engine();
        // Loads subtitles by their filepath
        void loadSubtitles(const std::string& filepath);
        // Searches for a text in subtitles by a given time
        std::string getPhraseAtTime(double time);

    private:
        // Stores subtitles
        std::vector<Subtitle> subtitles;
        // Stores dictionary object
        Dictionary dictionary;
};
