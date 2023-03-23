#include "ChatGPT.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include <sstream>
#include <regex>

std::string data;

size_t GetData(char* buffer, size_t sizeofitem, size_t numitems, void* ignore)
{
	size_t bytes = sizeofitem * numitems;
	data.append(buffer);

	return bytes;
}

void ChatGPT::Deserialize()
{
	nlohmann::json complete = nlohmann::json::parse(data);
	_response = complete["choices"][0]["message"]["content"];

	std::regex rgx("Response [0-9]+");
	_response = std::regex_replace(_response, rgx, "");

	while (_response.front() == '\n')
		_response.erase(_response.begin());

	data.clear();
}

std::string ChatGPT::Ask(const std::string& prompt)
{
	// Initialize HTTP client library
	curl_global_init(CURL_GLOBAL_ALL);

	// Initialize CURL handle
	CURL* curl = curl_easy_init();

	// Set CURL options
	if (!curl)
	{
		std::cout << "CURL: Init Failed" << std::endl;
		return "";
	}

	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
	headers = curl_slist_append(headers, "Content-Type: application/json");

	std::stringstream fullRequest;
	int i = 0;
	for (auto& h : _history)
	{
		fullRequest << "Prompt " << i << "\\n" << h.first << "\\n\\nResponse " << i << "\\n" << h.second << "\\n\\n";
		++i;
	}
	fullRequest << "Prompt " << i << "\\n" << prompt;
	std::string body = "{ \"model\": \"gpt-3.5-turbo\", \"messages\": [{\"role\": \"user\", \"content\": \"" + fullRequest.str() + "\"}]}";

	curl_easy_setopt(curl, CURLOPT_URL, API_URL.c_str());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GetData);

	// Perform the request
	CURLcode res = curl_easy_perform(curl);

	// Check for errors
	if (res != CURLE_OK) {
		std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		data = "";
	}

	Deserialize();

	// Clean up
	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);

	std::string resp_edit;
	for (char c : _response)
	{
		if (c == '\n') resp_edit.push_back(' ');
		else resp_edit.push_back(c);
	}

	_history.push_back({ prompt, resp_edit });

	return _response;
}
