#include "vector"
#include "algorithm"
#include "string"
#include "unordered_map"

using namespace std;

//Poker Logic, each function takes a vector of Strings in format "SV" (Suit/Value)
//Flush - remove card values, check if 5 suits are the same.
//Takes a vector<string> of community cards + one player hand and returns true if a straight is found.
bool isFlush(vector<string> hand){
    vector<char> suits;
    for (const auto& card: hand)
        suits.push_back(card[1]);

    sort(suits.begin(), suits.end());

    int flushCount = 0;
    for (size_t i = 0; i < suits.size() - 1; ++i) {
        if (suits[i] == suits[i + 1]) {
            flushCount++;
            if (flushCount == 4) {
                return true;
            }
        } else {
            flushCount = 0;
        }
    }
    return false;
}

//Straight - iterate through the hand if (hand[i+1] == hand[i]-1). TODO: Save highest number in straight for tiebreaking.
//Takes a vector<string> of community cards + one player hand and returns true if a straight is found.
int isStraight(vector<string> hand){
    int tempHandArr[7];

    // convert to ints and handle face cards
    char cArr[5] = {'T','J','Q','K','A'};
	int sArr[5] = {10, 11, 12, 13, 14};
    
    for (int i = 0; i < hand.size(); i++) {
        tempHandArr[i] = hand[i][0] - '0';
        for (int j = 0; j < 5; j++) {
            if (hand[i][0] == cArr[j]) {
                tempHandArr[i] = sArr[j];
            }
        }
    }
    
    //sort
    sort(begin(tempHandArr), end(tempHandArr));

    //compare
    int straightCount = 0, royalStraightCount = 0, j = 0;
    for (int i = 0; i <=5; i++){
        // 2 if royal straight
        if (tempHandArr[i] == sArr[j]){
            j++;
            royalStraightCount++;
            if (royalStraightCount == 4){ return 2; }
        }
        // 1 if normal straight
        else if (tempHandArr[i] == tempHandArr[i+1]-1){
            straightCount++;
            if (straightCount == 4){ return 1; }
        }
        // 0 if none
        else {
            straightCount = 0;
        }
    }
    return 0;
}

//Of a Kind - iterate through hand if (hand[i] == hand[i+1]).  Check for 3 and 4 of a Kind.  TODO: Save Pair value for tiebreaking.
//Takes a vector<string> of community cards + one player hand and returns true if a match is found
int ofAKind(vector<string> hand){
    // 1 - Pair, 2 - Two Pair, 3 - Three of a Kind, 4 - Four of a Kind
    int tempHandArr[7];

    // convert to ints and handle face cards
    char cArr[5] = {'T','J','Q','K','A'};
	string sArr[5] = {"10","11","12","13","14"};
    
    for (int i = 0; i < hand.size(); i++) {
        tempHandArr[i] = hand[i][0] - '0';
        for (int j = 0; j < 5; j++) {
            if (hand[i][0] == cArr[j]) {
                tempHandArr[i] = stoi(sArr[j]);
            }
        }
    }

    //sort
    sort(begin(tempHandArr), end(tempHandArr));

    unordered_map<int, size_t> count;  // holds count of each encountered number.
    for (int i=0; i<7; i++){count[tempHandArr[i]]++;}  

    int pair = 0, threePair = 0, fourPair = 0;

    for (auto i : count) {
        if (i.second == 2) {
            pair++;
        }
        if (i.second == 3){
            threePair++;
        }
        if (i.second == 4){
            return 4;
        }
    }

    if (pair == 1 && threePair == 1){
        return 5;
    }
    else if (pair == 1){
        return 1;
    }
    else if (pair > 1){
        return 2;
    }
    else if (threePair == 1){
        return 3;
    }
    else if (pair == 1 && threePair == 1){
        return 5;
    }

    return false;
}

int rankHand(vector<string> hand){
    // 10:Royal Flush, 9:Straight Flush, 8:Four Of A Kind, 7:Full House, 6:Flush, 5:Straight, 4:Three of a Kind, 
        //3:Two Pair, 2:Pair, 1:High Card
    
    // Call Each Eval
    bool flush = isFlush(hand);
    int straight = isStraight(hand);
    int pairs = ofAKind(hand);

    if (flush){
        // royal
        if (straight == 2){
            return 10;
        }
        // straight
        if (straight == 1){
            return 9;
        }
        // flush
        return 6;
    }
    else if (straight != 0){
        // straight
        return 5;
    }
    switch (pairs){
        case 1:
            return 2;
        case 2:
            return 3;
        case 3:
            return 4;
        case 4:
            return 8;
        case 5:
            return 7;
    }

    // high card
    return 1;
}
