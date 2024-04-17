// Written by JHoulihan
#include "Poker_Graphics.h"
#include "iostream"
#include "vector"
#include "string"

using namespace std;

// Graphics functions for displaying information and "sprites" during gameplay.

// Card Sprite Creation
string createCardSprite(vector<string> handInfo){
    string cardBases[4][6] = {
        {{" _____ "},
        {"|A .  |"},
        {"| /.\\ |"},
        {"|(_._)|"},
        {"|  |  |"},
        {"|____A|"}},
        {{" _____ "},
        {"|A \033[1;33m.\033[0m  |"},
        {"| \033[1;33m/ \\\033[0m |"},
        {"| \033[1;33m\\ /\033[0m |"},
        {"|  \033[1;33mv\033[0m  |"},
        {"|____A|"}},
        {{" _____ "},
        {"|A \033[1;32m_\033[0m  |"},
        {"| \033[1;32m( )\033[0m |"},
        {"|\033[1;32m(_'_)\033[0m|"},
        {"|  \033[1;32m|\033[0m  |"},
        {"|____A|"}},
        {{" _____ "},
        {"|A\033[1;31m_ _\033[0m |"},
        {"|\033[1;31m( v )\033[0m|"},
        {"| \033[1;31m\\ /\033[0m |"},
        {"|  \033[1;31mv\033[0m  |"},
        {"|____A|"}}
    };
    

    string cardSprite = "", cardSuitIndex = "SDCH";
    int cardBaseIndex;

    cardSprite += "\n";
    for (int i = 0; i < 6; i++){ // each line
        for (int j = 0; j < handInfo.size(); j++){ // each card
            char cardSuit = handInfo[j][1];
            string cardValue = ""; cardValue.push_back(handInfo[j][0]);
            cardBaseIndex = cardSuitIndex.find(cardSuit);

            string cardLine = cardBases[cardBaseIndex][i];
            if (i == 1 | i == 5) {cardLine.replace(i, 1, cardValue);}
            cardSprite.append(cardLine);
        }  
        cardSprite += "\n";
    }  

    return cardSprite;
}

// clears screen, for display later.
void clearScreen(int numOfLines){
    for (int i = 0; i < numOfLines; i++){
        // VT100 Escape Codes: "[2k":Clear line "[A":Move up a line
        cout << "\33[2K\r\033[A"; // delete line, move up a line
    }
    cout << "\x1b[2K"; // delete current line
}

string createStringFromInt(int input){
    string result = to_string(input);
    if (result.size() > 3)
        result.insert(result.size() - 3, ",");
    for (int i = result.size(); i < 7; i++)
        result.insert(0, " ");
    return result;
}

void createScreen(player *player, vector<cpuOpponent*> cpuInPlay, vector<string> communityCards, int screenFlag){
    /*Screen Flag Atlas:
    1 - In Game
    2 - In Game, CPU Hand shown
    */ 
    if (screenFlag == 1) { // In Game
        int numOfCpu = cpuInPlay.size();
        string cpuHand = "";
        switch(numOfCpu){
            case 1:
                cpuHand = "CPU 1:\n __  __\n|//||//|\n|__||__|\n[" + cpuInPlay[0]->getBlindBet() + "]";
                break;
            case 2:
                cpuHand = " CPU 1:    CPU 2:\n __  __    __  __\n|//||//|  |//||//|\n|__||__|  |__||__|\n"
                "[" + cpuInPlay[0]->getBlindBet() + "]     [" + cpuInPlay[1]->getBlindBet() + "]";
                break;
            case 3:
                cpuHand = " CPU 1:    CPU 2:    CPU 3:\n __  __    __  __    __  __\n|//||//|  |//||//|  "
                "|//||//|\n|__||__|  |__||__|  |__||__|\n[" + cpuInPlay[0]->getBlindBet() + "]     "
                "[" + cpuInPlay[1]->getBlindBet() + "]     [" + cpuInPlay[2]->getBlindBet() + "]";
                break;
        }
        
        cout << cpuHand << endl;

        cout << "\n";
        for (int i = 0; i < numOfCpu; i++){
                cout << "CPU" << i + 1 <<": " << cpuInPlay[i]->getChipTotal() << " - " << cpuInPlay[i]->getLastActionTaken() << endl; // previous action;
        }
        cout << "\n";

        //community cards
        cout << "Pot Total: " << player->getPot(); // add pot total variable
        cout << createCardSprite(communityCards);
        cout << "\n";

        //player hand
        cout << "Your Hand:";
        cout << createCardSprite(player->getHand());
        //player chips
        cout << "____________________\n";
        cout << "Your Chips: " << createStringFromInt(player->getChipTotal()) << " | [" + player->getBlindBet() + "]\n"; // add player chips total variable

        cout << "You Can: " << "Check, Call, Raise, or Fold. What Will You Do?" << endl;
    }

    if (screenFlag == 2) { // End of Hand / CPU Card Show
        int numOfCpu = cpuInPlay.size();
        string cpuHand = "";
        switch(numOfCpu){
            case 1:
                cpuHand = "CPU 1:\n __  __\n|" + cpuInPlay[0]->getHand()[0] + "||" + cpuInPlay[0]->getHand()[1] + "|\n|__||__|\n[   ]";
                break;
            case 2:
                cpuHand = " CPU 1:    CPU 2:\n __  __    __  __\n|" + cpuInPlay[0]->getHand()[0] + "||" + cpuInPlay[0]->getHand()[1] 
                + "|  |" + cpuInPlay[1]->getHand()[0] + "||" + cpuInPlay[1]->getHand()[1] + "|\n|__||__|  |__||__|\n[   ]     [   ]";
                break;
            case 3:
                cpuHand = " CPU 1:    CPU 2:    CPU 3:\n __  __    __  __    __  __\n|" + cpuInPlay[0]->getHand()[0] + "||" + cpuInPlay[0]->getHand()[1] 
                + "|  |" + cpuInPlay[1]->getHand()[0] + "||" + cpuInPlay[1]->getHand()[1] + "|  |" + cpuInPlay[2]->getHand()[0] + "||" 
                + cpuInPlay[2]->getHand()[1] + "|\n|__||__|  |__||__|  |__||__|\n[   ]     [   ]     [   ]";
                break;
        }
        
        cout << cpuHand << endl;

        cout << "\n";
        for (int i = 0; i < numOfCpu; i++){
                cout << "CPU" << i + 1 <<": " << cpuInPlay[i]->getChipTotal() << " - " << cpuInPlay[i]->getLastActionTaken() << endl; // previous action;
        }
        cout << "\n";

        //community cards
        cout << "Pot Total: " << player->getPot(); // add pot total variable
        cout << createCardSprite(communityCards);
        cout << "\n";

        //player hand
        cout << "Your Hand:";
        cout << createCardSprite(player->getHand());
        //player chips
        cout << "____________________\n";
        cout << "Your Chips: " << createStringFromInt(player->getChipTotal()) << " | [   ]\n"; // add player chips total variable

        cout << "You Can: " << "Check, Call, Raise, or Fold. What Will You Do?" << endl;
    }
}

void createMenuScreen(settingsHolder *settings, int menuFlag){
    // Title
    if (menuFlag == 1) { 
        cout << "\n\n";
        cout << "				        TWO CARD\n\033[1;33m$$$$$$$\\            $$\\            \033[0mTEXAS HOLD'EM\033[1;33m\n$$  "
            "__$$\\           $$ |\n$$ |  $$ | $$$$$$\\  $$ |  $$\\  $$$$$$\\   $$$$$$\\ \n$$$$$$$  |$$  __$$\\ $$ | $$  |$$  __$$\\ "
            "$$  __$$\\ \n$$  ____/ $$ /  $$ |$$$$$$  / $$$$$$$$ |$$ |  \\__|\n$$ |      $$ |  $$ |$$  _$$<  $$   ____|$$ |\n$$ |   "
            "   \\$$$$$$  |$$ | \\$$\\ \\$$$$$$$\\ $$ |\n\\__|       \\______/ \\__|  \\__| \\_______|\\__|\033[0m" << endl;
        cout << "\n                     GROUP 4\n\n		 1. START GAME\n		 2. SETTINGS\n		 3. QUIT\n\n\n\nENTER CHOICE: ";
    }
    // Settings
    if (menuFlag == 2){
        cout << "\nCURRENT SETTINGS:";
        cout << "\n\n1: Number Of CPU Opponents(Max 3): " << settings->getNumberOfCpu();
        cout << "\n\n2: Starting Chips(Max 200,000): " << settings->getStartingChips();
        cout << "\n\n3: Small Blind Value(Max 2,000): " << settings->getSmallBlindValue();
        
        for (int i = 0; i < 8; i++)
            cout << "\n";
        
        cout << "Enter the number of a setting you'd like to modify, or back to return to menu: " << endl;
    }
}

string getPlayerInput(){
    string playerInput;
    cin >> playerInput;

    return playerInput;
}
