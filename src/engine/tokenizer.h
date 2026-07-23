#pragma once

#include <string>
#include <vector>
#include "mecab.h"

// Stores data from tokenization by MeCab
struct Token {
    // Part of speech
    std::string POS;
    std::string dictForm;
    std::string sentenceForm;
    std::string reading;
};

// Tokenizes subtitles with MeCab
class Tokenizer {

    public:
        
        Tokenizer(const std::string& dict_path, const std::string& dictionary);
        ~Tokenizer();
        // Calls tokenization methods depending on MeCab dictionary
        // TODO: translate POS from japanese
        std::vector<Token> getTokens(const std::string& text);

    private:
        // Method that utilizes Unidic to tokenize
        std::vector<Token> getByUnidic(const MeCab::Node *node);
        // Method that utilizes Ipadic to tokenize
        std::vector<Token> getByIpadic(const MeCab::Node *node);
        // Converts katakana to hiragana
        std::string kataToHira(const std::string& katakana);
        // Splits an output from node's feature field
        std::vector<std::string> splitByComma(const std::string& features);

        std::string meCabDictionary;

        MeCab::Tagger *tagger;

};
