#include "Client.h"

int main()
{
	const int PORT = 12345;

	Client client("localhost", PORT);
	client.start();

	return 0;
}