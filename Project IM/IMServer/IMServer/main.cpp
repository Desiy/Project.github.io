#include "Server.h"

int main()
{
	Server s(44255);
	for (int i = 0;i < 100;++i)
	{
		s.listenForNewConnection();
	}
	system("pause");
	return 0;
}