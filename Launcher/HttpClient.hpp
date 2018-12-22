#pragma once

#include <string>
#include <string_view>

std::string httpRequest(
    std::string_view method,
    std::string_view url,
    std::string_view header,
    std::string_view body
);
std::string httpGet(
    std::string_view url, 
    std::string_view header, 
    std::string_view body
);
std::string httpPost(
    std::string_view url,
    std::string_view header,
    std::string_view body
);
