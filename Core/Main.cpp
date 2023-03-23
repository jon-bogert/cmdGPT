#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "advapi32.lib")

#include <iostream>
#include <string>
#include <fstream>

#include "ChatGPT.h"

bool GetKey(std::string& keyStr);

int main(int argc, char* argv[])
{
	std::cout << " ==== CHATGPT ====            (ChatGPT is Copyright OpenAI)" << std::endl << std::endl;

	std::string input;
	ChatGPT chatGpt;

	if (!GetKey(chatGpt.apiKey))
	{
		std::cout << "You must add your personal OpenAI API key to \"key.txt\"" << std::endl;
		(void)getchar();
		return EXIT_FAILURE;
	}

	while (true)
	{
		std::cout << "[You] > ";
		std::getline(std::cin, input);

		if (input == "quit")
		{
			break;
		}

		std::string resp = chatGpt.Ask(input);
		std::cout << std::endl << "[ChatGPT] > " << resp << std::endl << std::endl;
	}

	return 0;
}

bool GetKey(std::string& keyStr)
{
	std::ifstream keyFile;
	keyFile.open("key.txt");
	if (!keyFile.is_open())
	{
		std::ofstream key;
		key.open("key.txt");
		key.close();
		return false;
	}

	std::getline(keyFile, keyStr);
	if (keyStr == "")
	{
		keyFile.close();
		return false;
	}

	keyFile.close();
	return true;
}