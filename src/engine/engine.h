#pragma once

#include <vector>
#include <string>
#include "dictionary.h"
#include "subtitle_parser.h"
#include "tokenizer.h"
#include "subtitle_manager.h"

class Engine {
    
    public:
        // Constructs a class and initializes a dictionary
        Engine(const std::string& db_path, const std::string& dict_path, const std::string& dictionary);
        ~Engine();
        // Loads subtitles
        void loadOriginalSubtitles(const SubtitleManager::SubtitleEntry& track);
        void loadTranslationSubtitles(const SubtitleManager::SubtitleEntry& track);
        // Loads subtitles by their indices
        void loadBothSubtitles(int idx1, int idx2);
        // Searches for a text in subtitles by a given time
        std::vector<std::string> getPhraseAtTime(double time);
        // Outputs processed word from text
        void printCurrentWords(const std::string& phrase);
        // Reinitializes tokenizer with active chosen dictionary
        void changeDictionary(std::string dictName);
        // Set language for translation
        void setLanguage(const std::string& language);

        void prepareVideo(const std::string& videoPath);

        void addExternalSubtitleToList(const std::string& path);

        std::vector<Word> getWordsFromPhrase(const std::string& phrase);

        const SubtitleManager& getSubtitleManager() const { return subsManager; };

    private:
        // Stores subtitles
        std::vector<Subtitle> originalSubtitles;
        std::vector<Subtitle> translationSubtitles;
        // Stores dictionary object
        Dictionary dictionary;
        Tokenizer tokenizer;
        SubtitleManager subsManager;
        

        std::string currentVideo;

        std::string prefferedLanguage = "eng";
        std::string fallbackLanguage = "eng";
};
