#include "subtitle_parser.h"
#include <fstream>
#include <sstream>
#include <string>
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

    std::string format = detectFormat(filepath);
    std::string content = readFile(filepath);
    return parseFromString(content, format); 

}

std::string SubtitleParser::readFile(const std::string& filepath){
    std::ifstream file(filepath);
    return std::string(std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>());
}

std::vector<Subtitle> SubtitleParser::parseFromString(const std::string& content, const std::string& format){

    if (format == "subrip" || format == "srt")
        return parseSRT(content);
    if (format == "ass")
        return parseASS(content);
    return {}; 
}

std::vector<Subtitle> SubtitleParser::parseSRT(const std::string& content){

    std::istringstream stream(content);
    std::string line;
    bool onText = false;
    std::vector<Subtitle> subs;
    Subtitle current_sub;
    while (std::getline(stream, line)) {
      
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

    return subs;

}

std::vector<Subtitle> SubtitleParser::parseASS(const std::string& content){
    std::istringstream stream(content);
    std::string line;
    std::vector<Subtitle> subs;
    Subtitle current_sub;

    while (std::getline(stream, line)) {
        if (line.find("[Events]") != std::string::npos)
            break;
    }

    std::getline(stream, line);
    int firstColon = line.find_first_of(":");
    line = line.substr(firstColon + 2);

    int start = 0, end = 0, text = 0;

    for (int i = 0; line.size() > 0; i++) {
 
        size_t comma = line.find_first_of(",");
        std::string field;
        if (comma == std::string::npos) {
            field = line;
            line = "";
        }
        else {
            field = line.substr(0, comma);
            line = line.substr(comma + 1);
        }

        field.erase(0, field.find_first_not_of(" "));

        if (field == "Start") {
            start = i;
        }
        else if (field == "End") {
            end = i;
        }
        else if (field == "Text") {
            text = i;
        }

    }

    std::vector<std::string> lineEntries;

    while (std::getline(stream, line)) {
        int firstColon = line.find_first_of(":");
        if (line.substr(0, firstColon) != "Dialogue") 
            continue;

        line = line.substr(firstColon + 2);

        for (int i = 0; line.size() > 0; i++) {

            size_t comma = line.find_first_of(",");
           
            if (i == text) {
                current_sub.text = line;

                std::string cleaned;
                bool inTag = false;
                for (char c : current_sub.text) {
                    if (c == '{') inTag = true;
                    else if (c == '}') inTag = false;
                    else if (!inTag) cleaned += c;
                }

                current_sub.text = cleaned;
                break;
            }

            if (comma == std::string::npos) break;

            if (i == start) {
                current_sub.start_time = convertToSeconds(line.substr(0, comma));
            }
            else if (i == end) {
                current_sub.end_time = convertToSeconds(line.substr(0, comma));
            }

            line = line.substr(comma + 1);

        }

        if (current_sub.text != "")
            subs.push_back(current_sub);
        current_sub = {};
    }

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
    int h_end = line.find(':');
    int m_end = line.find(':', h_end + 1);
    int s_end = line.find_first_of(".,", m_end + 1);

    int hours = std::stoi(line.substr(0, h_end));
    int minutes = std::stoi(line.substr(h_end + 1, m_end - h_end - 1));
    int seconds = std::stoi(line.substr(m_end + 1, s_end - m_end - 1));
    double fraction = std::stod("0." + line.substr(s_end + 1));
    return hours * 3600 + minutes * 60 + seconds + fraction;

}

std::string SubtitleParser::detectFormat(const std::string& filepath){
    if (filepath.ends_with(".srt")) return "srt";
    if (filepath.ends_with(".ass") || filepath.ends_with(".ssa")) return "ass";
    
    std::ifstream file(filepath);
    std::string firstline;
    std::getline(file, firstline);

    if(firstline.find("[Script info]") != std::string::npos) return "ass";
    return "srt";

}

