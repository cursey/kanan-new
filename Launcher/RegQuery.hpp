#pragma once

#include <string>
#include <string_view>

#include <Windows.h>

std::string regQueryString(HKEY key, std::string_view subKey, std::string_view valueName);

