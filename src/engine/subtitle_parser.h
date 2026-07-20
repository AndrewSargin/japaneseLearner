#pragma once

#include <vector>
#include <string>

struct Subtitle {
    double start_time;
    double end_time;
    std::string text;
};

class SubtitleParser {

    public:
        static std::vector<Subtitle> parse(const std::string& filepath);
        static std::string detectFormat(const std::string& filepath);

    private:
        static std::vector<double> parseTimecode(const std::string& timecode);
        //std::string detectFormat(const std::string& filepath);
        static double convertToSeconds(const std::string& line);
        static bool isNumber(const std::string& line);
};
