#include <iostream>
#include <stdexcept>
#include <functional>

#include <comdef.h>
#include <WbemIdl.h>

#include <String.hpp>

#include "ScopeExit.hpp"
#include "WmiQuery.hpp"

using std::runtime_error;
using std::string;
using std::string_view;
using kanan::narrow;
using kanan::widen;

string wmiQueryString(string_view query, string_view propertyName) {
    // Initialize COM.
    if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
        throw runtime_error{ "Failed to initialize COM" };
    }

    SCOPE_EXIT([] { CoUninitialize(); });

    // Initialize COM security levels.
    /*if (FAILED(CoInitializeSecurity(
        nullptr,
        -1,
        nullptr,
        nullptr,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        nullptr,
        EOAC_NONE,
        nullptr
    ))) {
        throw runtime_error{ "Failed to initialize COM security" };
    }*/

    // Create WMI locator.
    IWbemLocator* locator{};

    if (FAILED(CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&locator))) {
        throw runtime_error{ "Failed to create IWbemLocator instance" };
    }

    SCOPE_EXIT([&] { locator->Release(); });

    // Connect to WMI services.
    IWbemServices* services{};

    if (FAILED(locator->ConnectServer(
        bstr_t{ L"ROOT\\CIMV2" },
        nullptr,
        nullptr,
        nullptr,
        0,
        nullptr,
        nullptr,
        &services
    ))) {
        throw runtime_error{ "Failed to connect to IWbemServices" };
    }

    SCOPE_EXIT([&] { services->Release(); });

    // Set security levels.
    if (FAILED(CoSetProxyBlanket(
        services,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        nullptr,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        nullptr,
        EOAC_NONE
    ))) {
        throw runtime_error{ "Failed to set IWbemServices security levels" };
    }

    // Make the query.
    IEnumWbemClassObject* enumerator{};

    if (FAILED(services->ExecQuery(
        bstr_t{ L"WQL" },
        bstr_t{ query.data() },
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        nullptr,
        &enumerator
    ))) {
        throw runtime_error{ "Failed to query IWbemServices" };
    }

    SCOPE_EXIT([&] { enumerator->Release(); });

    // Extract the result.
    IWbemClassObject* object{};
    ULONG numObjects = 0;

    if (FAILED(enumerator->Next(WBEM_INFINITE, 1, &object, &numObjects))) {
        throw runtime_error{ "Failed to extract result from WMI enumerator" };
    }

    if (numObjects == 0) {
        throw runtime_error{ "Failed to extract WMI result" };
    }

    SCOPE_EXIT([&] { object->Release(); });

    VARIANT prop{};

    if (FAILED(object->Get(widen(propertyName).c_str(), 0, &prop, nullptr, nullptr))) {
        throw runtime_error{ "Failed to extract property from query result" };
    }

    SCOPE_EXIT([&] { VariantClear(&prop); });

    return narrow(prop.bstrVal);
}
