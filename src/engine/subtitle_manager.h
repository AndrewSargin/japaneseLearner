#pragma once

#include <string>
#include <vector>


class SubtitleManager {

    public:
        struct SubtitleEntry {
            int index;
            std::string language;
            std::string format;
            std::string title;
            std::string filePath;
            bool isEmbeded;
        };

        SubtitleManager();
        ~SubtitleManager();
        void clear();
        void addExternalTrack(const std::string& path, const std::string& format);
        void getSubtitleTracksFromVideo(const std::string& videoPath);
        const std::vector<SubtitleEntry>& getTracks() const { return subtitleList; };

    private:
        std::vector<SubtitleEntry> subtitleList;
};
