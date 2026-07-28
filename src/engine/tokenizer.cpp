#include "tokenizer.h"
#include <cstdint>
#include <mecab.h>
#include <string>

Tokenizer::Tokenizer(const std::string& dict_path, const std::string& dictionary){

    tagger = MeCab::createTagger(dict_path.c_str());
    meCabDictionary = dictionary;
};

Tokenizer::~Tokenizer(){
    delete tagger;

};

// Calls suitable method depending on chosen dictionary
std::vector<Token> Tokenizer::getTokens(const std::string& text){
    
//For now it is always unidic

    const MeCab::Node *node = tagger->parseToNode(text.c_str());
    std::vector<Token> tokenized;

    if(1) {
        tokenized = getByUnidic(node);
    }
    else tokenized = getByIpadic(node);

    return tokenized;
};


//Should I remove spaces from text?
// Tokenizes sentences with Unidic
std::vector<Token> Tokenizer::getByUnidic(const MeCab::Node *node){

    std::vector<Token> tokens;

    for(; node; node = node->next){

        if (node->stat == MECAB_BOS_NODE || node->stat == MECAB_EOS_NODE)
            continue;
        
        Token token;

        token.sentenceForm = std::string(node->surface, node->length);
        std::string feature = node->feature;

        std::vector<std::string> columns = splitByComma(feature);

        if (columns.size() < 8 || columns[0] == "補助記号") continue;
        token.POS = columns[0];
        token.dictForm = columns[7];
        token.reading = kataToHira(columns[9]);

        tokens.push_back(token);
    }

    return tokens;


};

// Tokenizes sentences with Ipadic
std::vector<Token> Tokenizer::getByIpadic(const MeCab::Node *node){

};

// Splits output of node->feature by commas
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

// Converts katakana to hiragana
std::string Tokenizer::kataToHira(const std::string& katakana){

    std::string hiragana;

    size_t i = 0;

    while (i < katakana.size()) {

        unsigned char ch = katakana[i];
        
        if (i + 2 < katakana.size() && ch == 0xE3) {
            unsigned char ch2 = katakana[i + 1];
            unsigned char ch3 = katakana[i + 2];

            if ((ch2 == 0x82 && ch3 >= 0xA1) || (ch2 == 0x83 && ch3 <= 0xB6)) {
                
                uint32_t codepoint = ((ch & 0x0F) << 12) |
                                     ((ch2 & 0x3F) << 6) |
                                     (ch3 & 0x3F);
                codepoint -= 0x60;

                hiragana += (char)(0xE0 | (codepoint >> 12));
                hiragana += (char)(0x80 | ((codepoint >> 6) & 0x3F));
                hiragana += (char)(0x80 | (codepoint & 0x3F));
                i += 3;
                continue;
            }

        }

        hiragana += katakana[i];
        i++;
    
    }

    return hiragana;
        
}
