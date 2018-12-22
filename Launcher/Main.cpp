#include <iostream>
#include <sstream>
#include <string>

#include <json.hpp>

#include "Crypto.hpp"
#include "HttpClient.hpp"
#include "RegQuery.hpp"
#include "WmiQuery.hpp"

using namespace std;
using nlohmann::json;

/*string getDeviceID() {
    string uuid{};
    
    uuid += wmiQueryString("SELECT * FROM Win32_ComputerSystemProduct", "UUID");
    uuid += regQueryString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", "MachineGuid");

    auto uuidHash = hashString("SHA256", uuid);

    return stringEncode(CRYPT_STRING_HEXRAW, uuidHash);
}*/

int main(int argc, char* argv[]) {
    try {
        auto encrypted = encryptString("password1", "Hello, world!");

        cout << stringEncode(CRYPT_STRING_HEXRAW, encrypted) << endl;

        auto decrypted = decryptData("password1", encrypted);

        cout << stringEncode(CRYPT_STRING_HEXRAW, decrypted) << endl;
        cout << (char*)decrypted.data() << endl;


        /*// Get username and password.
        string username = "***REMOVED***";
        string password = "***REMOVED***";
        auto passwordHash = hashString("SHA512", password);

        password = stringEncode(CRYPT_STRING_HEXRAW, passwordHash);

        string header{ "Content-Type: application/json" };
        string body{ json{
            { "id", username },
            { "password", password },
            { "client_id", "7853644408" },
            { "scope", "us.launcher.all" },
            { "device_id", getDeviceID() }
        }.dump() };

        cout << body << endl;

        auto response = httpPost("https://accounts.nexon.net/account/login/launcher", header, body);

        cout << response << endl;

        auto jsonResponse = json::parse(response);
        auto token = jsonResponse["access_token"].get<string>();
        auto b64Token = stringEncode(CRYPT_STRING_BASE64, (const uint8_t*)token.data(), token.length());

        cout << token << endl;
        cout << b64Token << endl;

        header = (ostringstream{} <<
            "Cookie: nxtk=" << token << ";domain=.nexon.net;path=/;\r\n" <<
            "Authorization: bearer " << b64Token).str();

        cout << header << endl;

        response = httpGet("https://api.nexon.io/users/me/passport", header, "");

        cout << response << endl;*/
    }
    catch (const exception& e) {
        cerr << "Error (" << GetLastError() << "): " << e.what() << endl;
        return 1;
    }

    return 0;
}
