#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>

// Stores database data about translation
// TODO: figure out a way to access selected language by it's name
struct Translation {
    std::string language;
    std::string meaning;
};

// Stores database data about a word
struct Word {
    std::string kanji;
    std::string reading;
    std::vector<Translation> meanings;
    int jlptLevel = {0};
};

// Connects to an sqlite database and works with it
class Dictionary {

    public:
        // Constructs a class and connects to a database
        Dictionary(const std::string& dbPath);
        ~Dictionary();
        
        // Searchs for a word in a db by its kanji
        Word getEntryByKanji(const std::string& kanji, const std::string& language);
        // Searchs for a word in a db by its hiragana reading (probably useless)
        Word getEntryByReading(const std::string& reading, const std::string& language);

    private:
        sqlite3* db;

        // Searches in db by a given sqlite statement
        Word findSqliteEntries(sqlite3_stmt* stmt);

};
