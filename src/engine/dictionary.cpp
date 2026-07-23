#include "dictionary.h"
#include <iostream>
#include <sqlite3.h>
#include <string>

// Connetcting to database by .db file path
Dictionary::Dictionary(const std::string& db_path){
    
    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Could not connect to database: " << sqlite3_errmsg(db) << std::endl;
    }
    else {
        std::cout << "Connection to database is estabileshed." << std::endl;
    }
}

Dictionary::~Dictionary(){

};

// Searching for word in japanese by its kanji
Word Dictionary::getEntryByKanji(const std::string& kanji){
    std::string sql = "SELECT w.kanji, w.reading, w.jlpt_level, t.language, t.meaning\n"
                      "FROM words w\n"
                      "JOIN translations t ON w.id = t.word_id\n"
                      "WHERE w.kanji = ?";
    
    sqlite3_stmt* stmt;

    Word word;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    int rc = sqlite3_bind_text(stmt, 1, kanji.c_str(), -1, SQLITE_STATIC);

    if( rc != SQLITE_OK) {
        std::cout << "Error: " << sqlite3_errmsg(db) << std::endl;
    }
    else {
        word = findSqliteEntries(stmt);
    }

    //std::cout << "Search is finished" << std::endl;
    return word;
};


// Searching for a word by its reading (probably useless)
Word Dictionary::getEntryByReading(const std::string& reading ){
    std::string sql = "SELECT w.kanji, w.reading, w.jlpt_level, t.language, t.meaning\n"
                      "FROM words w\n"
                      "JOIN translations t ON w.id = t.word_id\n"
                      "WHERE w.reading = ?";

    sqlite3_stmt* stmt;

    Word word;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if(sqlite3_bind_text(stmt, 1, reading.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        std::cout << "Error: " << sqlite3_errmsg(db) << std::endl;
    }
    else {
        word = findSqliteEntries(stmt);
    }
    
    //std::cout << "Search is finished" << std::endl;
    return word;
    
}

// Search in database by a given statement
Word Dictionary::findSqliteEntries(sqlite3_stmt* stmt){
    
    int colNum = sqlite3_column_count(stmt);
    bool done = false;
    bool first_iter = true;
    Word word;

    //std::cout << "Starting search" << std::endl;

    while(!done){
    
        switch(sqlite3_step(stmt)){
            case SQLITE_ROW: {
                if (first_iter) {
                    word.kanji = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
                    word.reading = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
                    word.jlptLevel = sqlite3_column_int(stmt, 2);
                    first_iter = false;
                }
                Translation translation;
                translation.language = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
                translation.meaning = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
                word.meanings.push_back(translation);
                break;
            }

            case SQLITE_DONE:
                sqlite3_finalize(stmt);
                done = true;
                break;
        }
    }

    return word;
}
