#include <iostream>
#include "main.h"
#include "UU_ID.h"
#include "GarbageCollector.h"
#include "VSPointer.h"

void Principal::dll()
{
	std::cout << "fuck u" << UU_ID::GenerarUUID <<std::endl;
	std::cout << "test obtener instance" << GarbageCollector::ObtenerGCInstance << std::endl;

	while (true) {

		VSPtr<char> vsptr = VSPtr<char>::New();
		*vsptr = 'x';
		std::cout << "El valor es: " << "\n";
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
}