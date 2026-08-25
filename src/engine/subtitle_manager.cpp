#include "subtitle_manager.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/codec_desc.h>
#include <libavutil/dict.h>
}

SubtitleManager::SubtitleManager(){

}

SubtitleManager::~SubtitleManager(){

}

void SubtitleManager::clear(){
    subtitleList.clear();
}

void SubtitleManager::addExternalTrack(const std::string& path, const std::string& format){
    SubtitleEntry track;

    track.index = subtitleList.size();
    track.format = format;
    int lastSlash = path.find_last_of('/');
    int lastDot = path.find_last_of('.');
    track.title = path.substr(lastSlash + 1, lastDot - lastSlash - 1);
    track.filePath = path;
    track.isEmbeded = false;
    track.language = "";

    subtitleList.push_back(track);
}

void SubtitleManager::getSubtitleTracksFromVideo(const std::string& videoPath){

    AVFormatContext* ctx = nullptr;
    if (avformat_open_input(&ctx, videoPath.c_str(), nullptr, nullptr) < 0)
        return;

    if(avformat_find_stream_info(ctx, nullptr) < 0) {
        avformat_close_input(&ctx);
        return;
    }

    int subIndex = 0;
    
    for (unsigned int i = 0; i < ctx->nb_streams; i++) {
        AVStream* stream = ctx->streams[i];

        if (stream->codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE)
            continue;

        SubtitleEntry track;

        track.index = subtitleList.size();

        const AVCodecDescriptor* desc = avcodec_descriptor_get(stream->codecpar->codec_id);
        track.format = desc ? desc->name : "unknown";

        AVDictionaryEntry* lang = av_dict_get(stream->metadata, "language", nullptr, 0);
        AVDictionaryEntry* title = av_dict_get(stream->metadata, "title", nullptr, 0);

        track.language = lang ? lang->value : "";
        track.title = title ? title->value : "";
        track.isEmbeded = true;
        track.filePath = videoPath;

        subtitleList.push_back(track);
    }

    avformat_close_input(&ctx);
}
