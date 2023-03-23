#pragma once
#include <string>
#include <iostream>
#include <vector>

class ChatGPT
{
	std::string API_URL = "https://api.openai.com/v1/chat/completions";
	std::string _response;

	std::vector<std::pair<std::string, std::string>> _history;

	void Deserialize();

public:
	std::string apiKey = "";
	std::string Ask(const std::string& prompt);
};

