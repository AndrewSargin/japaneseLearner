#include "subtitle_parser.h"
#include <fstream>
#include <iostream>

bool SubtitleParser::isNumber (const std::string& line){
    for (int i = 0; i < line.size(); i++){
        if (!std::isdigit(line[i]))
            return false;
    
    }
    return true;   

}

// Loads subtitles and processes them to store in a class
std::vector<Subtitle> SubtitleParser::parse (const std::string& filepath){

    std::string line;
    bool onText = false;
    std::vector<Subtitle> subs;
    Subtitle current_sub;
    std::ifstream in (filepath);
    if (in.is_open()) {
        while (std::getline(in, line)) {
          
            // bruh, forgot about carriage return in dos style files. FIXED
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

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

        if(!current_sub.text.empty()) {
            subs.push_back(current_sub);
        }



    }

    else std::cout << "File is not open" << std::endl;

    in.close();

    return subs;

}

// Converts start and end time from string to doubles
std::vector<double> SubtitleParser::parseTimecode(const std::string& line){

    std::vector<double> times;
    std::string startTimeStr = line.substr(0,12);
    std::string endTimeStr = line.substr(17);

    times.push_back(convertToSeconds(startTimeStr));
    times.push_back(convertToSeconds(endTimeStr));

    return times;
    
}

// Converts time from hh:mm:ss,ms (string) to seconds (double)
double SubtitleParser::convertToSeconds(const std::string& line){
    int firstColon = line.find_first_of(":");
    int hours = std::stoi(line.substr(0, firstColon));
    int minutes = std::stoi(line.substr(firstColon + 1, firstColon + 3));
    int seconds = std::stoi(line.substr(firstColon + 4, firstColon + 6));
    int mseconds = std::stoi(line.substr(firstColon + 7));
    return hours * 3600 + minutes * 60 + seconds + mseconds * 0.001;

}

// Doesn't have proper implementation now
std::string SubtitleParser::detectFormat(const std::string& filepath){
    
    return filepath.substr(filepath.find_last_of(".") + 1);

}

