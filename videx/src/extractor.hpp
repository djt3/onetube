#ifndef _EXTRACTOR_H_
#define _EXTRACTOR_H_

#include <string>
#include <vector>

#include "video.hpp"

namespace videx::extractors::yt {
  extern std::vector<video> extract_videos(const std::string& url);
  extern std::string extract_playback(const std::string& url);
}

#endif
