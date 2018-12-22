#pragma once

#include <string>
#include <string_view>

std::string wmiQueryString(std::string_view query, std::string_view propertyName);
