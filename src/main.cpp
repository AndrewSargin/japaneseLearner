#include <iostream>
#include <fstream>
#include <ostream>
#include <string>

#include "subtitle_parser.h"

int main () {

    std::string line;

    std::ifstream in("../data/test.srt");
    
    SubtitleParser parser;
    auto subs = parser.parse("../data/test.srt");

    for (auto& sub:subs) {
        std::cout << "Start: " << sub.start_time << std::endl;
        std::cout << "End: " << sub.end_time << std::endl;
        std::cout << "Text: " << sub.text << std::endl;
        std::cout << "---" << std::endl;
    }


    return 0;
}
