#include "UU_ID.h"

std::string const UU_ID::CHARS = "qwertyuiopasdfghjklzxcvbnm1234567890";
std::string UU_ID::GenerarUUID() {
	std::random_device rd;
	std::mt19937 mr(rd());
	std::uniform_int_distribution<int> dist(0, CHARS.size() - 1);
	std::string currentUUID = std::string(24, ' ');
	for (char &i : currentUUID) {
		int indice = dist(mr);
		i = CHARS[indice];
	}
	return currentUUID;
}