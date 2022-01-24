#include <stdexcept>

#include <Windows.h>
#include <winhttp.h>

#include <String.hpp>

#include "HttpClient.hpp"
#include "ScopeExit.hpp"

class HttpError : public std::exception {
public:
    HttpError(HttpClient* http, DWORD error_code, const std::string& message) {
        LPTSTR buffer{};

        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
            GetModuleHandle(L"winhttp"), error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&buffer, 0,
            nullptr);

        m_message = "[" + http->m_last_url + "]\n\n" + message + ": " + kanan::narrow(buffer);

        LocalFree(buffer);
    }

    const char* what() const { return m_message.c_str(); }
    auto error_code() const { return m_error_code; }

private:
    DWORD m_error_code{};
    std::string m_message{};
};

HttpClient::HttpClient() {
    m_session =
        WinHttpOpen(nullptr, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (m_session == nullptr) {
        throw HttpError{this, GetLastError(), "Failed to start WinHTTP session"};
    }

    DWORD protocols = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_1 | WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2;

    if (WinHttpSetOption(m_session, WINHTTP_OPTION_SECURE_PROTOCOLS, &protocols, sizeof(protocols)) == FALSE) {
        throw HttpError{this, GetLastError(), "Failed to set the WinHTTP session secure protocols"};
    }
}

HttpClient::~HttpClient() {
    if (m_request != nullptr) {
        WinHttpCloseHandle(m_request);
    }

    if (m_connection != nullptr) {
        WinHttpCloseHandle(m_connection);
    }

    if (m_session != nullptr) {
        WinHttpCloseHandle(m_session);
    }
}

void HttpClient::request(
    std::string_view method, std::string_view url, std::string_view header, std::string_view body) {
    m_last_url = url;

    // Reset our client so we can make a new request.
    if (m_request != nullptr) {
        WinHttpCloseHandle(m_request);
        m_request = nullptr;
    }

    if (m_connection != nullptr) {
        WinHttpCloseHandle(m_connection);
        m_connection = nullptr;
    }

    // Parse the url into its components.
    std::wstring hostname{};
    std::wstring path{};

    hostname.resize(1024);
    path.resize(1024);

    URL_COMPONENTS components{};

    components.dwStructSize = sizeof(components);
    components.lpszHostName = hostname.data();
    components.dwHostNameLength = hostname.size();
    components.lpszUrlPath = path.data();
    components.dwUrlPathLength = path.size();

    if (WinHttpCrackUrl(kanan::widen(url).c_str(), url.length(), 0, &components) == FALSE) {
        throw HttpError{this, GetLastError(), "Failed to parse URL"};
    }

    hostname.resize(components.dwHostNameLength);
    path.resize(components.dwUrlPathLength);

    // Start a conection.
    m_connection = WinHttpConnect(m_session, hostname.c_str(), components.nPort, 0);

    if (m_connection == nullptr) {
        throw HttpError{this, GetLastError(), "Failed to start WinHTTP connection"};
    }

    // Start a request.
    m_request =
        WinHttpOpenRequest(m_connection, kanan::widen(method).c_str(), path.c_str(), nullptr, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES, (components.nScheme == INTERNET_SCHEME_HTTPS) ? WINHTTP_FLAG_SECURE : 0);

    if (m_request == nullptr) {
        throw HttpError{this, GetLastError(), "Failed to start WinHTTP request"};
    }

    // Send the request.
    if (WinHttpSendRequest(m_request, kanan::widen(header).c_str(), header.length(), (LPVOID)body.data(), body.length(),
            body.length(), 0) == FALSE) {
        throw HttpError{this, GetLastError(), "Failed to send request"};
    }

    if (WinHttpReceiveResponse(m_request, 0) == FALSE) {
        throw HttpError{this, GetLastError(), "Failed to read response"};
    }
}

void HttpClient::get(std::string_view url, std::string_view header, std::string_view body) {
    request("GET", url, header, body);
}

void HttpClient::post(std::string_view url, std::string_view header, std::string_view body) {
    request("POST", url, header, body);
}

std::string HttpClient::response() {
    std::string response{};
    auto offset = 0;

    DWORD bytes_avail{};

    do {
        // Get the number of bytes available to read and make space in the response
        // for them.
        bytes_avail = 0;

        if (WinHttpQueryDataAvailable(m_request, &bytes_avail) == FALSE) {
            throw HttpError{this, GetLastError(), "Failed to query bytes available"};
        }

        response.resize(response.size() + bytes_avail);

        // Attempt to read the available bytes.
        DWORD bytes_read{};

        if (WinHttpReadData(m_request, (LPVOID)(response.data() + offset), bytes_avail, &bytes_read) == FALSE) {
            throw HttpError{this, GetLastError(), "Failed to read data"};
        }

        offset += bytes_read;
        response.resize(response.size() - (bytes_avail - bytes_read));
    } while (bytes_avail > 0);

    return response;
}

std::string HttpClient::header(std::string_view name, DWORD index) {
    std::wstring header{};
    DWORD header_length{};

    WinHttpQueryHeaders(
        m_request, WINHTTP_QUERY_CUSTOM, kanan::widen(name).c_str(), WINHTTP_NO_OUTPUT_BUFFER, &header_length, &index);

    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
        throw HttpError{this, GetLastError(), "Failed to query header size"};
    }

    header.resize(header_length / sizeof(wchar_t));

    if (WinHttpQueryHeaders(m_request, WINHTTP_QUERY_CUSTOM, kanan::widen(name).c_str(), header.data(), &header_length,
            &index) == FALSE) {
        throw HttpError{this, GetLastError(), "Failed to query header"};
    }

    return kanan::narrow(header);
}
