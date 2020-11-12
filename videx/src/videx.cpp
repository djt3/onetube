#include <iostream>
#include "extractor.hpp"
#include "videx.hpp"
#include "video.hpp"

namespace videx {
  std::vector<video> extract_videos(const std::string& url) {
    if (url.find("youtube.com") != std::string::npos) {
      return videx::extractors::yt::extract_videos(url);
    }


    return std::vector<video>();
  }

  std::string extract_playback(const std::string& url) {
    if (url.find("youtube.com") != std::string::npos) {
      return videx::extractors::yt::extract_playback(url);
    }


    return "";
  }
}

/*int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "please enter a url" << std::endl;
    return 1;
  }

  videx::extract_videos(argv[1]);

  return 0;
}*/
