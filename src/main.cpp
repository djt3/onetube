#include <iostream>
#include <fstream>
#include <vector>
#include "fcgio.h"
#include "../videx/src/videx.hpp"

void add_video(const videx::video& video) {
  std::cout
    << "      <li>\r\n" 
    << "        <a href=\"" << video.url << "\">\r\n"
    << "          <img class=\"video_thumb\" src=\"" << video.thumbnail << "\"/>\r\n"
    << "          <div class=\"video_title\">\r\n"
    << "              <h2>" << video.title << "</h2>\r\n"
    << "          </div>\r\n"
    << "        </a>"
    << "        <a href=\"" << video.channel_url << "\">\r\n"
    << "          <div class=\"video_channel\">\r\n"
    << "              <h2>" << video.channel << "</h2>\r\n"
    << "          </div>\r\n"
    << "        </a>\r\n"
    << "      </li>\r\n";
}

int main(void) {
  std::streambuf* cin_streambuf  = std::cin.rdbuf();
  std::streambuf* cout_streambuf = std::cout.rdbuf();
  std::streambuf* cerr_streambuf = std::cerr.rdbuf();

  FCGX_Request request;

  FCGX_Init();
  FCGX_InitRequest(&request, 0, 0);

  while (FCGX_Accept_r(&request) == 0) {
    fcgi_streambuf cin_fcgi_streambuf(request.in);
    fcgi_streambuf cout_fcgi_streambuf(request.out);
    fcgi_streambuf cerr_fcgi_streambuf(request.err);

    std::cin.rdbuf(&cin_fcgi_streambuf);
    std::cout.rdbuf(&cout_fcgi_streambuf);
    std::cerr.rdbuf(&cerr_fcgi_streambuf);

    std::cout << "Content-type: text/html\r\n\r\n";

    std::string url(FCGX_GetParam("REQUEST_URI", request.envp));
     
    std::size_t watch_pos = url.find("watch?v=");
    if (watch_pos != std::string::npos) {
      std::string page_url = "https://www.youtube.com/" + url.substr(watch_pos);
      std::string video_url = videx::extract_playback(page_url);

      std::ifstream file("watch.html");

      std::string line;
      while (std::getline(file, line)) {
        std::cout << line << "\r\n";

        if (line.find("<ul ") != std::string::npos) {
          std::vector<videx::video> videos = videx::extract_videos(page_url);

          for (const auto& video : videos)
           add_video(video);
        }

        else if (line.find("<div class=\"video\">") != std::string::npos) {
          std::cout << "<video><source src=\"" << video_url << "\"></video>\r\n";
        }
      }
    }

    else {
      std::ifstream file("index.html");

      std::string line;
      while (std::getline(file, line)) {
        std::cout << line << "\r\n";

        if (line.find("<ul ") != std::string::npos) {
          std::size_t search_idx = url.find("results?search_query");
          std::string yt_url = "https://www.youtube.com";

          if (search_idx != std::string::npos)
            yt_url += "/" + url.substr(search_idx, url.length() - search_idx);

          else {
            std::size_t channel_idx = url.find("/c");

            if (channel_idx == std::string::npos)
              search_idx = url.find("/c");
            if (channel_idx == std::string::npos)
              search_idx = url.find("/user");

            if (channel_idx != std::string::npos) {
              yt_url += url.substr(channel_idx, url.length() - channel_idx) + "/videos";
            }
            else
              yt_url += "/";
          }


          std::vector<videx::video> videos = videx::extract_videos(yt_url);

          for (const auto& video : videos)
           add_video(video);
        } 
      }
    }
  }

  std::cin.rdbuf(cin_streambuf);
  std::cout.rdbuf(cout_streambuf);
  std::cerr.rdbuf(cerr_streambuf);

  return 0;
}
