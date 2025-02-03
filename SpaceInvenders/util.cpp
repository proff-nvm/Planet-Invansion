#include "util.h"

std::string getPlanetName(const std::string& level_name) {
    if (level_name == "level1") return "Mars";
    if (level_name == "level2") return "Venus";
    if (level_name == "level3") return "Jupiter";
    return "Unknown";
}
