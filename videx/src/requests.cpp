#include "requests.hpp"

#include <curlpp/Easy.hpp>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <sstream>

namespace videx::requests {
  std::string make_request(const std::string& url) {
    curlpp::Cleanup cleanup;

    std::stringstream result;
    curlpp::Easy request;

    request.setOpt(cURLpp::Options::WriteStream(&result));
    request.setOpt<curlpp::options::Url>(url);
    request.setOpt<curlpp::options::UserAgent>("Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:59.0) Gecko/20100101 Firefox/59.0");

    request.perform();

    return result.str();
  }
}
