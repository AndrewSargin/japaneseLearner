#include <string>
#include "engine/engine.h"


int main () {

    // Paths used for testing
    std::string testPath = "/home/andrew/projects/japanese-learner/data/test.srt"; 
    std::string db_path = "/home/andrew/projects/japanese-learner/data/dictionary.db";
    std::string unidic_path = "-d /usr/lib/mecab/dic/unidic";

    // Construct engine
    Engine engine(db_path, unidic_path, "unidic");
    engine.loadSubtitles(testPath);
    std::string phrase = engine.getPhraseAtTime(613.500);
    engine.printCurrentWords(phrase);
    




    return 0;
}
