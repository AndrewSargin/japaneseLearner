#include <sqlite3.h>
#include <string>
#include <vector>

struct Translation {
    std::string language;
    std::string meaning;
};

struct Word {
    std::string kanji;
    std::string reading;
    std::vector<Translation> meanings;
    int jlptLevel = {0};
};

class Dictionary {

    public:
        Dictionary(const std::string& dbPath);
        ~Dictionary();

        Word getEntryByKanji(const std::string& kanji);
        Word getEntryByReading(const std::string& reading);

    private:
        sqlite3* db;
        int sqlSelectCallback(void*, int, char**, char**);

};
