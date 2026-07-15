#include <iostream>
#include <ostream>
#include <string>

#include "engine/subtitle_parser.h"
#include "engine/dictionary.h"

int main () {

    std::string testPath = "/home/andrew/projects/japanese-learner/data/test.srt"; 
    std::string db_path = "/home/andrew/projects/japanese-learner/data/dictionary.db";
    SubtitleParser parser;
    auto subs = parser.parse(testPath);

    for (auto& sub:subs) {
        std::cout << "Start: " << sub.start_time << std::endl;
        std::cout << "End: " << sub.end_time << std::endl;
        std::cout << "Text: " << sub.text << std::endl;
        std::cout << "---" << std::endl;
    }

    std::cout << parser.detectFormat(testPath);

    Dictionary dict = Dictionary(db_path);
    Word word = dict.getEntryByKanji("dsff");

    std::cout << word.kanji << ", " << word.reading << ", ";
    for (int i = 0; i < word.meanings.size(); i++) {
        std::cout << word.meanings[i].meaning << ", ";
    }

    std::cout << std::endl;
    return 0;
}
