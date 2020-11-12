#include "extractor.hpp"
#include "requests.hpp"

#include <iostream>
#include <vector>
#include <cstring>

namespace videx::extractors::yt {
  std::vector<video> extract_videos(const std::string& url) {
    std::string src = requests::make_request(url);

    std::size_t index = 0;

    std::vector<video> videos;

    while (index != std::string::npos) {
      bool compact = false;
      bool channel = false;

      std::size_t temp_index = src.find("compactVideoRenderer\":{\"videoId", index);
      if (temp_index == std::string::npos) {
        temp_index = src.find("videoRenderer\":{\"videoId", index);

        if (temp_index == std::string::npos) {
          temp_index = src.find("VideoRenderer\":{\"videoId", index);
          channel = true;
        }

        if (temp_index == std::string::npos)
          break;

        index = temp_index;
      }
      else
        compact = true;

      video video;

      index = src.find("\"videoId\"", index) + strlen("\"videoId\":\"");
      video.url = "/watch?v=" + src.substr(index, 11);

      index = src.find("\"url\":\"", index) + strlen("\"url\":\"");
      video.thumbnail = src.substr(index, src.find("\"", index) - index);

      if (compact)
        index = src.find("simpleText", index) + strlen("simpleText\":\"");
      else
        index = src.find("\"title\":{\"runs\":[{\"text\":\"", index) + strlen("\"title\":{\"runs\":[{\"text\":\"");

      video.title = src.substr(index, src.find("\"", index) - index);

      if (!channel) {
        index = src.find("longBylineText", index) + strlen("longBylineText\":{\"runs\":[{\"text\":\"");
        video.channel = src.substr(index, src.find("\"", index) - index);

        index = src.find("webCommandMetadata", index) + strlen("webCommandMetadata\":{\"url\":\"/");
        video.channel_url = src.substr(index, src.find("\"", index) - index);

        index = src.find("lengthText", index);
        index = src.find("simpleText", index) + strlen("simpleText\":\"");
      }

      video.length = src.substr(index, src.find("\"", index) - index);

      videos.push_back(video);
    }

    return videos;
  }

  std::string extract_playback(const std::string& url) {
    std::string src = requests::make_request(url);

    std::size_t index = src.find("googlevideo.com\\/videoplayback");
    if (index != std::string::npos) {
      index -= 30;
      index = src.find("https:", index);

      std::string watch_url = src.substr(index, src.find("\"", index) - index - 1);

      while ((index = watch_url.find("\\/")) != std::string::npos)
        watch_url.replace(index, strlen("\\/"), "/");

      while ((index = watch_url.find("\\\\u0026")) != std::string::npos)
        watch_url.replace(index, strlen("\\\\u0026"), "&");

      return watch_url;
    }

    return "";
  }
}
