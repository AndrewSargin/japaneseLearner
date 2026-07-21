#include "tokenizer.h"
#include <mecab.h>

Tokenizer::Tokenizer(const std::string& dict_path, const std::string& dictionary){

    tagger = MeCab::createTagger(dict_path.c_str());
    node = nullptr;

};

Tokenizer::~Tokenizer(){
    delete tagger;
    delete node;

};

std::vector<Token> Tokenizer::getTokens(const std::string& text){
    
//For now it is always unidic

    node = tagger->parseToNode(text.c_str());
    std::vector<Token> tokenized;

    if(1) {
        tokenized = getByUnidic();
    }
    else tokenized = getByIpadic();

    return tokenized;
};

std::vector<Token> Tokenizer::getByUnidic(){

    std::vector<Token> tokens;

    for(; node; node = node->next){

        if (node->stat == MECAB_BOS_NODE || node->stat == MECAB_EOS_NODE)
            continue;
        
        Token token;

        token.sentenceForm = std::string(node->surface, node->length);
        std::string feature = node->feature;

        std::vector<std::string> columns = splitByComma(feature);

        token.POS = columns[0];
        token.dictForm = columns[7];
        token.reading = columns[9];

        tokens.push_back(token);
    }

    return tokens;


};

std::vector<Token> Tokenizer::getByIpadic(){

};

std::vector<std::string> Tokenizer::splitByComma(const std::string& feature){

        std::vector<std::string> substrings;
        bool inQuotes = false;
        std::string currentToken;

        for (char ch : feature){

            if (ch == '"')
            {
                inQuotes = !inQuotes;
                currentToken += ch;    
            }
            else if (ch == ',' && !inQuotes){
                substrings.push_back(currentToken);
                currentToken = "";
            }
            else {
                currentToken += ch;
            }
        }

        if (!currentToken.empty() || (!substrings.empty() && feature.back() == ',')) {
            substrings.push_back(currentToken);
        }

        return substrings;

};

std::string Tokenizer::kataToHira(const std::string& katakana){



}
