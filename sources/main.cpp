#include "../headers/MyGame.h" 

int main()
{
	sf::TcpSocket socket;
	char choice;
	do
	{
		std::cout << "Kim chcesz byc?\n";
		std::cout << "s - serwer(kolko)\n";
		std::cout << "k - klient(krzyzyk)\n";
		std::cin >> choice;
		choice = tolower(choice);
	} while (choice!='s' && choice!='k');
	if (choice == 's')
	{
		sf::IpAddress address;
		sf::TcpListener listener;
		std::cout << "Trwa oczekiwanie na klienta\n";
		std::cout << "Adres serwera: " << address.getLocalAddress() << std::endl;
		listener.listen(50000);
		listener.accept(socket);
		system("clear");
		std::cout << "Nawiazano polaczenie!!!\n";
		std::cout << "Grasz jako kolko\n";
	}
	else
	{
		std::string ip;
		while (true)
		{
			std::cout << "Podaj adres serwera: ";
			std::cin >> ip;
			if(socket.connect(ip, 50000) == sf::Socket::Done)
				break;
		}
		system("clear");
		std::cout << "Nawiazano polaczenie!!!\n";
		std::cout << "Grasz jako krzyzyk\n";
	}
	MyGame game(choice,socket);
	game.play();
	system("clear");
	std::cout << "Rozgrywka zostala zakonczona\n";
	return 0;
}