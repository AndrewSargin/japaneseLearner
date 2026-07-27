#include <iostream>
#include "engine.h"
#include "tokenizer.h"

// Constructs a class and initializes a dictionary and tokenizer
Engine::Engine(const std::string& db_path, const std::string& dict_path, const std::string& dictionary) :
    dictionary(db_path),
    tokenizer(dict_path, dictionary) {

}

Engine::~Engine(){

}

void Engine::loadSubtitles(const std::string& filepath){
   
    subtitles.clear();
    subtitles = SubtitleParser::parse(filepath);

}

// For now the search is done by looking through the vector of subtitles
// It's a binaty search now, but for a played video it's better to store an index
// Also this method might not find subs that go parallel if such exist
// In the future I plan to make a subtitle cache generation to store data in .db
std::string Engine::getPhraseAtTime(double time){

    int right = subtitles.size() - 1;
    int left = {0};
    int index;
    bool found = false;

    while(!found){
        
        if(left > right) {
            std::cout << "no subs at this mark" << std::endl;
            return "";
        }
        else {
            index = left + (right - left) / 2;

            if(time < subtitles[index].start_time){
                right = index - 1;
            }
            else if(time > subtitles[index].end_time){
                left = index + 1;
            }
            else if(time >= subtitles[index].start_time && time <= subtitles[index].end_time){
                found = true;
                return subtitles[index].text;
            }
            else {
                found = true;
                std::cout << "in if clause: no subs at this mark" << std::endl;
                return "";
            }

        }

    }

    std::cout << "returning" << std::endl;
    return subtitles[index].text;
}

void Engine::printCurrentWords(const std::string& phrase){

    std::vector<Token> tokens = tokenizer.getTokens(phrase);

    for (Token token : tokens){

        std::cout << token.POS << " " << token.reading << " " << token.dictForm;

        Word word = dictionary.getEntryByKanji(token.dictForm);
        if(word.kanji.length() != 0){
            std::cout << " " << word.meanings.at(1).meaning;
            
            if(word.jlptLevel != 0) 
                std::cout << " N" << word.jlptLevel; 
            
        }

        std::cout << std::endl;
    }

}

