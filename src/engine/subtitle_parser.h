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
        // Doesn't have implementation now
        static std::string detectFormat(const std::string& filepath);

    private:
        // Takes a string containing start and end time and returns a vector of numeric times
        static std::vector<double> parseTimecode(const std::string& timecode);
        //std::string detectFormat(const std::string& filepath);
        // Converts string with time from subtitle to a numeric value
        static double convertToSeconds(const std::string& line);
        // Checks if a string is a numeric value
        static bool isNumber(const std::string& line);
};
