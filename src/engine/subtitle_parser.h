#pragma once

#include <vector>
#include <string>

// Stores subtitle data
struct Subtitle {
    double start_time;
    double end_time;
    std::string text;
};

// Class for processing subtitle information
class SubtitleParser {

    public:
        // Opens a subtitile file and parses it
        static std::vector<Subtitle> parse(const std::string& filepath);
        // Parses subtitles from string
        static std::vector<Subtitle> parseFromString(const std::string& content, const std::string& format);
        // Detects if subtitles are in .srt or .ass format
        static std::string detectFormat(const std::string& filepath);
      
    private:
        // Takes a string containing start and end time and returns a vector of numeric times
        static std::vector<double> parseTimecode(const std::string& timecode);
        // Converts string with time from subtitle to a numeric value
        static double convertToSeconds(const std::string& line);
        // Checks if a string is a numeric value
        static bool isNumber(const std::string& line);
        // Reads all subtitles in one string
        static std::string readFile(const std::string& filepath);
        // Parses .srt subtitles
        static std::vector<Subtitle> parseSRT(const std::string& content);
        // Parses .ass subtitles
        static std::vector<Subtitle> parseASS(const std::string& content);
};
