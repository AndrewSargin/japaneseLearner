#include <iostream>
#include <ostream>
#include <string>

#include "engine/subtitle_parser.h"
#include "engine/dictionary.h"
#include "engine/engine.h"


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
    Word word = dict.getEntryByKanji("食べる");

    std::cout << word.kanji << ", " << word.reading << ", ";
    for (int i = 0; i < word.meanings.size(); i++) {
        std::cout << word.meanings[i].meaning << ", ";
    }
    std::cout << std::endl;

    Word word2 = dict.getEntryByReading("いぬ");
    std::cout << word2.kanji << ", " << word2.reading << ", ";
    for (int i = 0; i < word2.meanings.size(); i++) {
        std::cout << word2.meanings[i].meaning << ", ";
    }
    std::cout << std::endl;

    Engine engine = Engine(db_path);

    engine.loadSubtitles(testPath);
    std::cout << "Phrase at 80.102: " << engine.getPhraseAtTime(80.102) << std::endl;
    std::cout << "Phrase at 5166.322: " << engine.getPhraseAtTime(5166.322) << std::endl;
    std::cout << "Phrase at 5166.300: " << engine.getPhraseAtTime(5166.300) << std::endl;

    return 0;
}
