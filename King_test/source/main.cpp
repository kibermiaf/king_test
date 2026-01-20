#include "Core/MessageService.h"
#include "UI/CLI.h"
#include <iostream>

int main() 
{
	try 
	{
		// 1. Create the Service (The Backend)
		// We create it on the stack because it needs to live as long as the app runs.
		Core::InMemoryMessageService messageService;

		// 2. Create the UI (The Frontend)
		// We inject the service into the UI.
		// Or we can create a MockMessageService and pass it here for testing instead.

		UI::CLI cli(messageService);

		// 3. Run the application
		cli.run();
	}
	catch (const std::exception& e) 
	{
		// Global error catcher just in case
		std::cerr << "CRITICAL ERROR: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}