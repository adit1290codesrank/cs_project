#pragma once
#include <bitset>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "sha256.h"

//SHA256 Encryption
std::string sha256_encrypt(std::string input);

//Functions to read data from environment file
std::map<std::string, std::string> readEnv(std::string path);
std::pair<std::string, std::string> splitKeyValue(std::string text);

std::map<std::string, std::string> readEnv(std::string path)
{
	std::map<std::string, std::string> env;
	std::ifstream read_file(path);
	std::string text;
	while (std::getline(read_file, text))
	{
		std::cout << text;
		std::pair<std::string, std::string> key_value = splitKeyValue(text);
		env.insert(key_value);
	}
	return env;
}

std::pair<std::string, std::string> splitKeyValue(std::string text)
{
	std::string key = "", value = "";
	int pos = text.find("=");
	key = text.substr(0, pos);
	value = text.substr(pos + 1);
	return { key, value };
}

std::string sha256_encrypt(std::string input)
{
	sha256 sha;
	sha.update(input);
	std::array<uint8_t, 32> digest = sha.digest();
	return sha256::toString(digest);
}