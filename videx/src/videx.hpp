#ifndef _VIDEX_H_
#define _VIDEX_H_

#include <vector>

#include "video.hpp"

namespace videx {
  extern std::vector<video> extract_videos(const std::string& url);
  extern std::string extract_playback(const std::string& url);
}

#endif
