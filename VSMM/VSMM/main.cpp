#include <iostream>
#include "main.h"
#include "UU_ID.h"
#include "GarbageCollector.h"
#include "VSPointer.h"

void Principal::dll()
{
	while (true) {

		VSPtr<char> vsptr2 = VSPtr<char>::New();
		*vsptr2 = 'x';
		std::cout << "El valor es: " << &vsptr2 << "\n";

		VSPtr<bool> vsptr3 = VSPtr<bool>::NewRemote();
		*vsptr3 = false;
		std::cout << "El valor es: " << &vsptr3 << "\n";

		VSPtr<int> vsptr = VSPtr<int>::New();
		*vsptr = 845;
		std::cout << "El valor es: " << &vsptr << "\n";

		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
}