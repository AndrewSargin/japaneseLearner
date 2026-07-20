#include <iostream>
#include "engine.h"

Engine::Engine(const std::string& db_path) : dictionary(db_path) {

}

Engine::~Engine(){

}

void Engine::loadSubtitles(const std::string& filepath){
   
    subtitles.clear();
    subtitles = SubtitleParser::parse(filepath);

}

// For now the search is done by looking through the vector of subtitles
// It`s a binaty search now, but for a played video it`s better to store an index
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

