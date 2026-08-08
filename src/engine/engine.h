#pragma once

#include <vector>
#include <string>
#include "dictionary.h"
#include "subtitle_parser.h"
#include "tokenizer.h"

class Engine {
    
    public:
        // Constructs a class and initializes a dictionary
        Engine(const std::string& db_path, const std::string& dict_path, const std::string& dictionary);
        ~Engine();
        // Loads subtitles by their filepath
        void loadSubtitles(const std::string& filepath);
        // Searches for a text in subtitles by a given time
        std::string getPhraseAtTime(double time);
        // Outputs processed word from text
        void printCurrentWords(const std::string& phrase);
        // Reinitializes tokenizer with active chosen dictionary
        void changeDictionary(std::string dictName);

        std::vector<Word> getWordsFromPhrase(const std::string& phrase);

    private:
        // Stores subtitles
        std::vector<Subtitle> subtitles;
        // Stores dictionary object
        Dictionary dictionary;
        Tokenizer tokenizer;
};
