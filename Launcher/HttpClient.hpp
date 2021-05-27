#pragma once

#include <string>
#include <string_view>

#include <Windows.h>
#include <winhttp.h>

class HttpClient {
public:
    HttpClient();
    virtual ~HttpClient();

    void request(std::string_view method, std::string_view url, std::string_view header, std::string_view body);
    void get(std::string_view url, std::string_view header, std::string_view body);
    void post(std::string_view url, std::string_view header, std::string_view body);

    std::string response();
    std::string header(std::string_view name, DWORD index = 0);

private:
    friend class HttpError;

    HINTERNET m_session{};
    HINTERNET m_connection{};
    HINTERNET m_request{};
    std::string m_last_url{};
};
