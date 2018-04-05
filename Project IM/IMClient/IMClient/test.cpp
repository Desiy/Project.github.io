#include "Client.h"

int main()
{
	Client myClient1("127.0.0.1", 44255);
	if (!myClient1.Connect())
	{
		cout << "Failed to connect to a server." << endl;
		system("pause");
		return 1;
	}
	string user1input;
	while (true)
	{
		getline(cin, user1input);
		if (!myClient1.sendString(user1input))
			break;
		Sleep(10);
	}
	
	return 0;
}