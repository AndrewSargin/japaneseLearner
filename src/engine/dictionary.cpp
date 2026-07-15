#include "dictionary.h"
#include <iostream>
#include <sqlite3.h>
#include <string>

int callback(void *NotUsed, int numOfResults, char** values, char** colName){

    for (int i = 0; i < numOfResults; i++){

        std::cout << colName[i] << ": " << values[i] << std::endl;

    }

    std::cout << std::endl;

    return 0;
};

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

Word Dictionary::getEntryByKanji(const std::string& kanji){
    std::string sql = "SELECT w.kanji, w.reading, t.language, t.meaning\n"
                      "FROM words w\n"
                      "JOIN translations t ON w.id = t.word_id\n"
                      "WHERE w.kanji = '食べる'";
    
    sqlite3_stmt* stmt;

    Word word;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);


    if( rc != SQLITE_OK) {
        std::cout << "Error: " << sqlite3_errmsg(db) << std::endl;
    }
    else {

        int colNum = sqlite3_column_count(stmt);
        bool done = false;
        bool first_iter = true;

        std::cout << "Starting search" << std::endl;

        while(!done){

            switch(sqlite3_step(stmt)){
                case SQLITE_ROW: {
                    if(first_iter) {
                        word.kanji = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));    
                        word.reading = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
                        first_iter = false;
                    }
                    Translation trans;
                    trans.language = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
                    trans.meaning = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
                    word.meanings.push_back(trans);
                    break;
                }

                case SQLITE_DONE:
                    sqlite3_finalize(stmt);
                    done = true;
                    break;

            }

        }

    }

    //sqlite3_exec(db, sql.c_str(), sqlSelectCallback, 0, &zErrMsg);

    std::cout << "Search is finished" << std::endl;
    return word;
};

int Dictionary::sqlSelectCallback(void* NotUsed, int numOfResults, char** value, char** colName){

    for( int i = 0; i < numOfResults; i++) {
    
        std::cout << colName[i] << ": " << value[i] << std::endl;

    }

    std::cout << std::endl;

    return 0;

};
