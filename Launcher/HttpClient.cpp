#include <stdexcept>

#include <Windows.h>
#include <winhttp.h>

#include <String.hpp>

#include "ScopeExit.hpp"
#include "HttpClient.hpp"

using std::runtime_error;
using std::string;
using std::string_view;
using std::wstring;
using kanan::widen;

string httpRequest(string_view method, string_view url, string_view header, string_view body) {
    // Parse the url into its components.
    wstring hostname{};
    wstring path{};

    hostname.resize(1024);
    path.resize(1024);

    URL_COMPONENTS components{};

    components.dwStructSize = sizeof(components);
    components.lpszHostName = hostname.data();
    components.dwHostNameLength = hostname.size();
    components.lpszUrlPath = path.data();
    components.dwUrlPathLength = path.size();

    if (WinHttpCrackUrl(widen(url).c_str(), url.length(), 0, &components) == FALSE) {
        throw runtime_error{ "Failed to parse URL" };
    }

    hostname.resize(components.dwHostNameLength);
    path.resize(components.dwUrlPathLength);

    // Start a WinHTTP session.
    auto session = WinHttpOpen(nullptr, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (session == nullptr) {
        throw runtime_error{ "Failed to start WinHTTP session" };
    }

    SCOPE_EXIT([=] { WinHttpCloseHandle(session); });

    // Start a conection.
    auto connection = WinHttpConnect(session, hostname.c_str(), components.nPort, 0);

    if (connection == nullptr) {
        throw runtime_error{ "Failed to start WinHTTP connection" };
    }

    SCOPE_EXIT([=] { WinHttpCloseHandle(connection); });

    // Start a request.
    auto request = WinHttpOpenRequest(
        connection, 
        widen(method).c_str(), 
        path.c_str(), 
        nullptr, 
        WINHTTP_NO_REFERER, 
        WINHTTP_DEFAULT_ACCEPT_TYPES, 
        (components.nScheme == INTERNET_SCHEME_HTTPS) ? WINHTTP_FLAG_SECURE : 0
    );

    if (request == nullptr) {
        throw runtime_error{ "Failed to start WinHTTP request" };
    }

    SCOPE_EXIT([=] { WinHttpCloseHandle(request);  });

    // Send the request.
    if (WinHttpSendRequest(
        request,
        widen(header).c_str(),
        header.length(),
        (LPVOID)body.data(),
        body.length(),
        body.length(),
        0
    ) == FALSE) {
        throw runtime_error{ "Failed to send request" };
    }

    // Read the response.
    string response{};
    auto offset = 0;

    if (WinHttpReceiveResponse(request, 0) == FALSE) {
        throw runtime_error{ "Failed to read response" };
    }

    DWORD bytesAvailable{};

    do {
        // Get the number of bytes available to read and make space in the response
        // for them.
        bytesAvailable = 0;

        if (WinHttpQueryDataAvailable(request, &bytesAvailable) == FALSE) {
            throw runtime_error{ "Failed to query bytes available" };
        }

        response.resize(response.size() + bytesAvailable);

        // Attempt to read the available bytes.
        DWORD bytesRead{};

        if (WinHttpReadData(request, (LPVOID)(response.data() + offset), bytesAvailable, &bytesRead) == FALSE) {
            throw runtime_error{ "Failed to read data" };
        }

        offset += bytesRead;
        response.resize(response.size() - (bytesAvailable - bytesRead));
    } while (bytesAvailable > 0);

    return response;
}

string httpGet(string_view url, string_view header, string_view body) {
    return httpRequest("GET", url, header, body);
}

std::string httpPost(string_view url, string_view header, string_view body) {
    return httpRequest("POST", url, header, body);
}
