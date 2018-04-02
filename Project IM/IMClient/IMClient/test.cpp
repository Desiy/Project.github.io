#include "Client.h"
#include <stdlib.h>
int main()
{
	Client c;
	char *endpoint[] = { { "local host" }, {"127.0.0.1"} };
	char **ptr = endpoint;
	c.startup();
	system("pause");
	return 0;
}