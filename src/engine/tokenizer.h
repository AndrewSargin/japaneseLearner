#pragma once

#include <string>
#include <vector>
#include "mecab.h"

struct Token {
    std::string POS;
    std::string dictForm;
    std::string sentenceForm;
    std::string reading;
};


class Tokenizer {

    public:

        Tokenizer(const std::string& dict_path, const std::string& dictionary);
        ~Tokenizer();
        std::vector<Token> getTokens(const std::string& text);

    private:
        std::vector<Token> getByUnidic();
        std::vector<Token> getByIpadic();
        std::string kataToHira(const std::string& katakana);
        std::vector<std::string> splitByComma(const std::string& features);

        MeCab::Tagger *tagger;
        const MeCab::Node *node;


};
