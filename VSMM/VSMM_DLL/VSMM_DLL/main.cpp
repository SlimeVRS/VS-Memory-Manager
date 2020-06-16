#include <iostream>
#include "main.h"
#include "UU_ID.h"
#include "GarbageCollector.h"

void Principal::dll()
{
	std::cout << "fuck u" << UU_ID::GenerarUUID <<std::endl;
	std::cout << "test obtener instance" << GarbageCollector::ObtenerGCInstance << std::endl;
	&GarbageCollector::GenerarJSON;
}

