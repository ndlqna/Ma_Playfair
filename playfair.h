#pragma once
#include <string>

void generateMatrix(std::string key);
std::string formatMessage(std::string msg);
std::string encrypt(std::string message);
std::string decrypt(std::string ciphertext);
