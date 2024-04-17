// Written by JHoulihan
#ifndef POKER_GRAPHICS_H_
#define POKER_GRAPHICS_H_

#include "Poker_Classes.h"
#include "iostream"
#include "vector"
#include "string"

using namespace std;

// creates card sprite given card string (ex. "AS" = Ace Spades)
string createCardSprite(vector<string> handInfo);

// clears screen
void clearScreen(int numOfLines);

// creates a 7 character long string format 000,000 given an int
string createStringFromInt(int input);

// creates a gamplay screen.  Flags: 1 - In Game, 2 - End Hand
void createScreen(player *player, vector<cpuOpponent*> cpuInPlay, vector<string> communityCards, int screenFlag);

// creates a menu screen.  Flags: 1 - Main, 2 - Settings
void createMenuScreen(settingsHolder *settings, int menuFlag);

string getPlayerInput();

#endif //POKER_GRAPHICS_H_
