#include "qt_multimedia_subtitle_provider.h"
#include <QList>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/codec_desc.h>
#include <libavutil/dict.h>
}

QList<ISubtitleProvider::Track> QtMultimediaSubtitleProvider::getSubtitleTracks(const QString& videoPath){
    QList<ISubtitleProvider::Track> tracks;

    AVFormatContext* ctx = nullptr;
    if (avformat_open_input(&ctx, videoPath.toUtf8().constData(), nullptr, nullptr) < 0)
        return tracks;

    if (avformat_find_stream_info(ctx, nullptr) < 0) {
        avformat_close_input(&ctx);
        return tracks;
    }

    int subIndex = 0;

    for (unsigned int i = 0; i < ctx->nb_streams; i++) {
        AVStream* stream = ctx->streams[i];

        if (stream->codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE)
            continue;

        ISubtitleProvider::Track t;
        t.index = subIndex++;

        const AVCodecDescriptor* desc = avcodec_descriptor_get(stream->codecpar->codec_id);
        t.format = desc ? QString(desc->name) : "unknown";

        AVDictionaryEntry* lang = av_dict_get(stream->metadata, "language", nullptr, 0);
        AVDictionaryEntry* title = av_dict_get(stream->metadata, "title", nullptr, 0);

        t.language = lang ? QString(lang->value) : "";
        t.title = title ? QString(title->value) : "";
        
        tracks.append(t);
    }

    avformat_close_input(&ctx);
    return tracks;
}

QString QtMultimediaSubtitleProvider::getSubtitleContent(const QString& videoPath, const Track& track){
    AVFormatContext *ctx = nullptr;
    if (avformat_open_input(&ctx, videoPath.toUtf8().constData(), nullptr, nullptr) < 0)
        return "";
    
    avformat_find_stream_info(ctx, nullptr);

    int subIndex = 0;
    int streamIndex = -1;
    for (unsigned int i = 0; i < ctx->nb_streams; i++){
        if (ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE) {
            if (subIndex == track.index) {
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

    QString result;
    AVPacket* pkt = av_packet_alloc();

    while (av_read_frame(ctx, pkt) >= 0){
        if (pkt->stream_index == streamIndex) {
            result += QString::fromUtf8((const char*)pkt->data, pkt->size);
        }
        av_packet_unref(pkt);
    }
    av_packet_free(&pkt);
    avformat_close_input(&ctx);
    return result;

}

