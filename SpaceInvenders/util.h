#pragma once
#include <iostream>

#define SETCOLOR(c, r, g, b) {c[0]=r; c[1]=g; c[2]=b; }
std::string getPlanetName(const std::string& level_name);