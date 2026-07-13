#include <vector>
#include <string>

struct Subtitle {
    double start_time;
    double end_time;
    std::string text;
};

class SubtitleParser {

    public:
        std::vector<Subtitle> parse(const std::string& filepath);

    private:
        std::vector<double> parseTimecode(const std::string& timecode);
        std::string detectFormat(const std::string& filepath);
};

double convertToSeconds(std::string line);
