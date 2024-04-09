#ifndef POKER_LOGIC_H_
#define POKER_LOGIC_H_

#include "vector"
#include "algorithm"
#include "string"
#include "unordered_map"

using namespace std;

//Poker Logic, each function takes a vector of Strings in format "SV" (Suit/Value)
//Takes a vector<string> of community cards + one player hand and returns true if a flush is found.
bool isFlush(vector<string> hand);

//Straight - iterate through the hand if (hand[i+1] == hand[i]-1)
//Takes a vector<string> of community cards + one player hand and returns true if a straight is found.
int isStraight(vector<string> hand);

//Of a Kind - iterate through hand if (hand[i] == hand[i+1]).  Check for 3 and 4 of a Kind.
//Takes a vector<string> of community cards + one player hand and returns true if a match is found
int ofAKind(vector<string> hand);

// 10:Royal Flush, 9:Straight Flush, 8:Four Of A Kind, 7:Full House, 6:Flush, 5:Straight, 4:Three of a Kind, 
// 3:Two Pair, 2:Pair, 1:High Card
int rankHand(vector<string> hand);

#endif // POKER_LOGIC_H_
