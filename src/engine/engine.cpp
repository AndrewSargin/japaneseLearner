#include <iostream>
#include "engine.h"

// Constructs a class and initializes a dictionary and tokenizer
Engine::Engine(const std::string& db_path, const std::string& dict_path, const std::string& dictionary) :
    dictionary(db_path),
    tokenizer(dict_path, dictionary) {
    subsManager = SubtitleManager();
}

Engine::~Engine(){

}

void Engine::loadOriginalSubtitles(const SubtitleManager::SubtitleEntry& track){
   
    originalSubtitles.clear();
    if (!track.isEmbeded)
        originalSubtitles = SubtitleParser::parse(track.filePath);
    else originalSubtitles = SubtitleParser::getEmbededSubtitles(track.filePath, track.index, track.format);

}

void Engine::loadTranslationSubtitles(const SubtitleManager::SubtitleEntry& track){

    translationSubtitles.clear();
    if (!track.isEmbeded)
        translationSubtitles = SubtitleParser::parse(track.filePath);
    else translationSubtitles = SubtitleParser::getEmbededSubtitles(track.filePath, track.index, track.format);}

// For now the search is done by looking through the vector of subtitles
// It's a binaty search now, but for a played video it's better to store an index
// Also this method might not find subs that go parallel if such exist
// In the future I plan to make a subtitle cache generation to store data in .db
std::vector<std::string> Engine::getPhraseAtTime(double time){

    std::vector<std::string> phrases;

    int right = originalSubtitles.size() - 1;
    int left = {0};
    int index;
    bool found = false;

    // Cycle for original subs
    while(!found){
        
        if(left > right) {
            //std::cout << "no subs at this mark" << std::endl;
            phrases.push_back("");
            found = true;
        }
        else {
            index = left + (right - left) / 2;

            if(time < originalSubtitles[index].start_time){
                right = index - 1;
            }
            else if(time > originalSubtitles[index].end_time){
                left = index + 1;
            }
            else if(time >= originalSubtitles[index].start_time && time <= originalSubtitles[index].end_time){
                found = true;
                phrases.push_back(originalSubtitles[index].text);
            }
            else {
                found = true;
                std::cout << "in if clause: no subs at this mark" << std::endl;
                phrases.push_back("");
            }

        }

    }

    right = translationSubtitles.size() - 1;
    left = 0;
    index = 0;
    found = false;

    // Cycle for translation subs
    while(!found){
        
        if(left > right) {
            //std::cout << "no subs at this mark" << std::endl;
            phrases.push_back("");
            found = true;
        }
        else {
            index = left + (right - left) / 2;

            if(time < translationSubtitles[index].start_time){
                right = index - 1;
            }
            else if(time > translationSubtitles[index].end_time){
                left = index + 1;
            }
            else if(time >= translationSubtitles[index].start_time && time <= translationSubtitles[index].end_time){
                found = true;
                phrases.push_back(translationSubtitles[index].text);
            }
            else {
                found = true;
                std::cout << "in if clause: no subs at this mark" << std::endl;
                phrases.push_back("");
            }

        }

    }

    return phrases;
}

void Engine::printCurrentWords(const std::string& phrase){

    std::vector<Token> tokens = tokenizer.getTokens(phrase);

    for (Token token : tokens){

        std::cout << token.POS << " " << token.reading << " " << token.dictForm;

        Word word = dictionary.getEntryByKanji(token.dictForm, fallbackLanguage);
        if(word.kanji.length() != 0){

            // Have to make a way to get needed translation
            // And also to check if the translation exists
            std::cout << " " << word.meanings.at(0).meaning;
            
            if(word.jlptLevel != 0) 
                std::cout << " N" << word.jlptLevel; 
            
        }

        std::cout << std::endl;
    }

}

std::vector<Word> Engine::getWordsFromPhrase(const std::string& phrase){
    std::vector<Token> tokens = tokenizer.getTokens(phrase);

    std::vector<Word> words;

    for (Token token : tokens) {

        Word word = dictionary.getEntryByKanji(token.dictForm, fallbackLanguage);

        if (word.kanji.empty()) {
            word =  dictionary.getEntryByReading(token.reading, fallbackLanguage);
        }

        words.push_back(word);
    }

    return words;
}

void Engine::setLanguage(const std::string& language){
    prefferedLanguage = language;

}

void Engine::prepareVideo(const std::string& videoPath){
    subsManager.clear();

    currentVideo = videoPath;
    subsManager.getSubtitleTracksFromVideo(videoPath);
}

void Engine::addExternalSubtitleToList(const std::string& path) {
    std::string format = SubtitleParser::detectFormat(path);
    subsManager.addExternalTrack(path, format);
}

void Engine::loadBothSubtitles(int idx1, int idx2){

    std::vector<SubtitleManager::SubtitleEntry> subtitleList = subsManager.getTracks();

    for (int i = 0; i < subtitleList.size(); i++) {
        if (idx1 == subtitleList[i].index) {
            loadOriginalSubtitles(subtitleList[i]);
        }
        if (idx2 == subtitleList[i].index) {
            loadTranslationSubtitles(subtitleList[i]);
        }
    }
}
