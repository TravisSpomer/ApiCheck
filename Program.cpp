#include "ApiCheck.h"
#include <iostream>

int main()
{
	RoInitialize(RO_INIT_MULTITHREADED);

	if (ApiCheck::IsRevealBrushAvailable())
		std::cout << "Hooray! RevealBrush is available on your system.\n";
	else
		std::cout << "Your version of Windows doesn't support RevealBrush. :(\n";

	return 0;
}
