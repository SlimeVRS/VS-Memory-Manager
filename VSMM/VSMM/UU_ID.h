#ifndef UU_ID_H
#define UU_ID_H
#include <random>

class UU_ID {
private:
	static const std::string CHARS;
public:
	static std::string GenerarUUID();
};

#endif
