#include "../headers/MyGame.h"

MyGame::MyGame(char choice,sf::TcpSocket &socket) :

	status(choice),
	spoint(&socket),
	gameWindow(sf::VideoMode(600, 600, 32), "Kolko i Krzyzyk"),
	gameBoard{
		{ { 0,0,0 },
		{   0,0,0 },
		{   0,0,0 } }
					}

{
	gameWindow.setVerticalSyncEnabled(true);
	spoint->setBlocking(false);
}

void MyGame::play()
{
	int round = 1;
	bool match = false;
	bool win = false;
	int winner;
	sf::Event event;
	while (gameWindow.isOpen())
	{
		while (gameWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				gameWindow.close();
		}
		if (status == 's')
		{
			if (round==1)
			{
				if (!match)
				{
					system("clear");
					std::cout << "Twoj ruch\n";
				}
				if (!win && event.type == sf::Event::MouseButtonPressed)
				{
					handleClick(1, event.mouseButton.x, event.mouseButton.y);
					win = isWinner(1);
					if (win)
						winner = 1;
					round = 2;
				}
				match = true;
			}
			else if (round==2)
			{
				if (spoint->send(move, sizeof(move),nothing) == sf::TcpSocket::Done)
					round = 3;
			}
			else
			{
				if (match)
				{
					system("clear");
					std::cout << "Oczekiwanie na ruch przeciwnika\n";
				}
				if (spoint->receive(move, sizeof(move), nothing) == sf::TcpSocket::Done)
				{
					gameBoard[move[0]][move[1]] = 2;
					win = isWinner(2);
					if (win)
						winner = 2;
					round = 1;
				}
				match = false;
			}
		}
		else
		{
			if (round == 1)
			{
				if (!match)
				{
					system("clear");
					std::cout << "Oczekiwanie na ruch przeciwnika\n";
				}
				if (spoint->receive(move, sizeof(move), nothing) == sf::TcpSocket::Done)
				{
					gameBoard[move[0]][move[1]] = 1;
					win = isWinner(1);
					if (win)
						winner = 2;
					round = 2;
				}
				match = true;
			}
			else if (round == 2)
			{
				if (match)
				{
					system("clear");
					std::cout << "Twoj ruch\n";
				}
				if (!win && event.type == sf::Event::MouseButtonPressed)
				{
					handleClick(2, event.mouseButton.x, event.mouseButton.y);
					win = isWinner(2);
					if (win)
						winner = 1;
					round = 3;
				}
				match = false;
			}
			else
			{
				if (spoint->send(move, sizeof(move),nothing) == sf::TcpSocket::Done)
					round = 1;
			}
		}
		gameWindow.clear();
		drawBoard(gameWindow);


		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				drawSlot(gameWindow, gameBoard[i][j], i, j);
		if (!win)
		{
			win = checkBoard();
			if (win)
				winner = 0;
		}
		if (win) {
					drawWinScene(gameWindow, winner);
		}
		gameWindow.display();
	}
}

void MyGame::drawBoard(sf::RenderWindow &gameWindow) 
{
	gameWindow.clear(sf::Color::Black);
	sf::RectangleShape line1(sf::Vector2f(560, 2));
	line1.setFillColor(sf::Color::White);
	line1.setPosition(20, 200);
	sf::RectangleShape line2 = line1;
	line2.setPosition(20, 400);
	sf::RectangleShape line3(sf::Vector2f(2, 560));
	line3.setFillColor(sf::Color::White);
	line3.setPosition(200, 20);
	sf::RectangleShape line4 = line3;
	line4.setPosition(400, 20);
	gameWindow.draw(line1);
	gameWindow.draw(line2);
	gameWindow.draw(line3);
	gameWindow.draw(line4);
}

void MyGame::drawSlot(sf::RenderWindow &gameWindow, int symbol, int x_pos, int y_pos) 
{
	if (symbol == 1) 
	{
		sf::CircleShape shape(60);
		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineThickness(5);
		shape.setOutlineColor(sf::Color::White);
		shape.setPosition(35 + (205 * x_pos), 35 + (205 * y_pos));

		gameWindow.draw(shape);
	}
	else if (symbol == 2)
	{
		sf::RectangleShape cross1(sf::Vector2f(200, 5));
		cross1.setPosition(25 + (210 * x_pos), 20 + (210 * y_pos));
		cross1.rotate(45);
		cross1.setFillColor(sf::Color::White);

		sf::RectangleShape cross2(sf::Vector2f(200, 5));
		cross2.setPosition(25 + (210 * x_pos), 165 + (205 * y_pos));
		cross2.rotate(-45);
		cross2.setFillColor(sf::Color::White);

		gameWindow.draw(cross1);
		gameWindow.draw(cross2);
	}

}

void MyGame::drawWinScene(sf::RenderWindow &gameWindow, int side)
{
	sf::Text winText;
	sf::Font winFont;
	if (!winFont.loadFromFile("assets/Xeron.ttf"))
	{
		std::cout << "Wystapil problem z zaladowaniem czcionki, upewnij sie ze znajduje sie w katalogu roboczym aplikacji" << std::endl;
		exit;
	} 
	winText.setFont(winFont);
	winText.setCharacterSize(50);
	winText.setFillColor(sf::Color::Red);

	if (side == 1)
	{
		winText.setPosition(70, 260);
		winText.setString("Zwyciestwo!");
	}
	else if (side == 2)
	{
		winText.setPosition(90, 260);
		winText.setString("Przegrana!");
	}
	else if (side == 0)
	{
		winText.setPosition(180, 260);
		winText.setString("Remis!");
	}
	gameWindow.draw(winText);	
}

bool MyGame::isWinner(int side)
{
	int count;
	for (int i = 0; i < 3; ++i)
	{
		count = 0;
		for (int j = 0; j < 3; ++j)
		{
			if (gameBoard[i][j] == side)
				++count;
		}

		if (count == 3)
		{
			return true;
			
		}
	}
	for (int j = 0; j < 3; ++j)
	{
		count = 0;
		for (int i = 0; i < 3; ++i)
		{
			if (gameBoard[i][j] == side)
				++count;
		}

		if (count == 3)
		{
			return true;
		}
	}
	count = 0;
	for (int i = 0, j = 0; i < 3; ++i, ++j)
	{
		if (gameBoard[i][j] == side)
			++count;
	}

	if (count == 3)
	{
		return true;
	}

	count = 0;
	for (int i = 0, j = 2; i < 3; ++i, --j)
	{
		if (gameBoard[i][j] == side)
			++count;
	}

	if (count == 3)
	{
		return true;
	}

	return false;

}

bool MyGame::handleClick(int side, int x_axis, int y_axis)
{
	int i, j;

	if (x_axis < 200) 
	{
		i = 0; 
	}
	else if (x_axis < 400)
	{
		i = 1;
	}
	else
	{
		i = 2;
	}

	if (y_axis < 200)
	{
		j = 0;
	}
	else if (y_axis < 400)
	{
		j = 1;
	}
	else
	{
		j = 2;
	}

	move[0] = i;
	move[1] = j;

	if (gameBoard[i][j] == 0)
	{
		gameBoard[i][j] = side;
		return true;
	}

	return false;
}

bool MyGame::checkBoard()
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (!gameBoard[i][j])
				return false;
	return true;
}