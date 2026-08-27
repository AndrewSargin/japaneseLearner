#include "subtitle_parser.h"
#include <fstream>
#include <libavutil/avutil.h>
#include <sstream>
#include <string>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/codec_desc.h>
#include <libavutil/dict.h>
}

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

std::string SubtitleParser::getSubtitleContent(const std::string& videoPath, const int idx){
    AVFormatContext *ctx = nullptr;
    if (avformat_open_input(&ctx, videoPath.c_str(), nullptr, nullptr) < 0)
        return "";
    
    avformat_find_stream_info(ctx, nullptr);

    int subIndex = 0;
    int streamIndex = -1;
    for (unsigned int i = 0; i < ctx->nb_streams; i++){
        if (ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE) {
            if (subIndex == idx) {
                streamIndex = i;
                break;
            }
            subIndex++;
        }
    }

    if (streamIndex < 0) {
        avformat_close_input(&ctx);
        return "";
    }

    std::string result;
    AVPacket* pkt = av_packet_alloc();

    while (av_read_frame(ctx, pkt) >= 0){
        if (pkt->stream_index == streamIndex) {
            result += std::string((const char*)pkt->data, pkt->size);
        }
        av_packet_unref(pkt);
    }
    av_packet_free(&pkt);
    avformat_close_input(&ctx);
    return result;

}

std::vector<Subtitle> SubtitleParser::getEmbededSubtitles(const std::string& videoPath, int idx, const std::string& format){

    std::vector<Subtitle> result;

    AVFormatContext* ctx = nullptr;
    if (avformat_open_input(&ctx, videoPath.c_str(), nullptr, nullptr) < 0)
        return result;
    if (avformat_find_stream_info(ctx, nullptr) < 0) {
        avformat_close_input(&ctx);
        return result;
    }

    int subIndex = 0, streamIndex = -1;
    AVRational timeBase{1, 1};

    for (unsigned int i = 0; i < ctx->nb_streams; i++) {
        if (ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE) {
            if (subIndex == idx) {
                streamIndex = i;
                timeBase = ctx->streams[i]->time_base;
                break;
            }
            subIndex++;
        }
    }

    if (streamIndex < 0) {
        avformat_close_input(&ctx);
        return result;
    }

    AVPacket* pkt = av_packet_alloc();

    while (av_read_frame(ctx, pkt) >= 0) {

        if (pkt->stream_index == streamIndex && pkt->size > 0) {

            std::string raw((const char*)pkt->data, pkt->size);

            if (format == "mov_text") {
                if (raw.size() > 2) raw = raw.substr(2);
            }
            else if (format == "ass" || format == "ssa") {
                int commas = 0, pos = 0;
                for (; pos < (int)raw.size() && commas < 8; pos++)
                    if (raw[pos] == ',') commas++;
                raw = pos <= (int)raw.size() ? raw.substr(pos) : "";

                std::string cleaned;
                bool inTag = false;
                for (char c : raw) {
                    if (c == '{') inTag = true;
                    else if (c == '}') inTag = false;
                    else if (!inTag) cleaned += c;
                }
                raw = cleaned;
            }

            if (!raw.empty()) {
                Subtitle sub;
                sub.start_time = pkt->pts * av_q2d(timeBase);
                double duration = pkt->duration > 0 ? pkt->duration * av_q2d(timeBase) : 2.0;
                sub.end_time = sub.start_time + duration;
                sub.text = raw;
                result.push_back(sub);
            }
        }
        av_packet_unref(pkt);
    }

    av_packet_free(&pkt);
    avformat_close_input(&ctx);
    return result;
}
