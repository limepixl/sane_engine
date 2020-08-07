#pragma once
#include <unordered_map>
#include <string>

struct Shader
{
	uint32_t ID;
	std::unordered_map<std::string, uint32_t> locations;
};