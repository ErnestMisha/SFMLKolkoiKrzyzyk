#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include <iostream>

class MyGame
{
public:
	MyGame(char choice, sf::TcpSocket& socket);
	void play();
	
	void drawBoard(sf::RenderWindow &gameWindow);
	void drawSlot(sf::RenderWindow &gameWindow, int symbol, int x_pos, int y_pos);
	void drawWinScene(sf::RenderWindow &gameWindow, int side);
	bool isWinner(int side);
	bool handleClick(int side, int x_axis, int y_axis);
	bool checkBoard();

private:
	sf::TcpSocket* spoint;
	char status;
	int move[2];
	size_t nothing;
	sf::RenderWindow gameWindow;
	std::vector<std::vector<int>> gameBoard;
};