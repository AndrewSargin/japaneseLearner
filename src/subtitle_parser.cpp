#include "subtitle_parser.h"
#include <cctype>
#include <string>
#include <fstream>
#include <iostream>

bool isNumber (std::string line){
    for (int i = 0; i < line.size(); i++){
        if (!std::isdigit(line[i]))
            return false;
    
    }
    return true;   

}

std::vector<Subtitle> SubtitleParser::parse (const std::string& filepath){

    std::string line;
    bool onText = false;
    std::vector<Subtitle> subs;
    Subtitle current_sub;
    std::ifstream in (filepath);
    if (in.is_open()) {
        while (std::getline(in, line)) {
            
            if (line.empty()) {
                
                subs.push_back(current_sub);
                current_sub = {};
                onText = false;

            }
            else if (isNumber(line)) {

            }

            else if (line.find("-->") != std::string::npos) {
                std::vector<double> times = parseTimecode(line);
                current_sub.start_time = times[0];
                current_sub.end_time = times[1];
                onText = true;
            }
            else if (onText) {
                
                if(!current_sub.text.empty())
                    current_sub.text += "\n";
                current_sub.text += line;
            }

        }   

        if(!current_sub.text.empty())
            subs.push_back(current_sub);

    }

    else std::cout << "File is not open" << std::endl;

    in.close();

    return subs;

}

std::vector<double> SubtitleParser::parseTimecode(const std::string& line){

    std::vector<double> times;
    std::string startTimeStr = line.substr(0,12);
    std::string endTimeStr = line.substr(17);

    times.push_back(convertToSeconds(startTimeStr));
    times.push_back(convertToSeconds(endTimeStr));

    return times;
    
}

double convertToSeconds(std::string line){
    int hours = std::stoi(line.substr(0,2));
    int minutes = std::stoi(line.substr(3,5));
    int seconds = std::stoi(line.substr(6,8));
    int mseconds = std::stoi(line.substr(9));
    return hours * 3600 + minutes * 60 + seconds + mseconds * 0.001;

}

std::string SubtitleParser::detectFormat(const std::string& filepath){
    
    return filepath;

}

