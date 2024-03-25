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
        {"|A .  |"},
        {"| / \\ |"},
        {"| \\ / |"},
        {"|  v  |"},
        {"|____A|"}},
        {{" _____ "},
        {"|A _  |"},
        {"| ( ) |"},
        {"|(_'_)|"},
        {"|  |  |"},
        {"|____A|"}},
        {{" _____ "},
        {"|A_ _ |"},
        {"|( v )|"},
        {"| \\ / |"},
        {"|  v  |"},
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

void createScreen(vector<string> playerHand, vector<string> communityCards, int screenFlag){
    /*Screen Flag Atlas:
    1 - Main Menu
    2 - In Game
    3 - Settings Menu
    */ 

    if (screenFlag == 1) {
        cout << "\n\n";
        // title
        cout << "$$$$$$$\\            $$\\                TWO CARD\n$$  __$$\\           $$ |\n$$ |  $$ | $$$$$$\\  $$ |  $$\\  $$$$$$\\"   
            << "   $$$$$$\\ \n$$$$$$$  |$$  __$$\\ $$ | $$  |$$  __$$\\ $$  __$$\\ \n$$  ____/ $$ /  $$ |$$$$$$  / $$$$$$$$ |$$ |  \\__|\n"
            << "$$ |      $$ |  $$ |$$  _$$<  $$   ____|$$ |\n$$ |      \\$$$$$$  |$$ | \\$$\\ \\$$$$$$$\\ $$ |\n\\__|       \\______/ \\"
            << "__|  \\__| \\_______|\\__|" << endl;
        cout << "\n                     GROUP 4\n\n		 1. START GAME\n		 2. SETTINGS\n		 3. QUIT\n\n\n\nENTER CHOICE: ";
    }

    if (screenFlag == 2) {
        cout << "CPU:" << endl;
        string cpuHand = " __  __\n|//||//|\n|__||__|";
        cout << cpuHand << endl;

        string spacer = "\n\n\n";
        cout << spacer;

        //community cards
        cout << "Pot Total: " << endl; // add pot total variable
        cout << createCardSprite(communityCards);
        cout << spacer;

        //player hand
        cout << "Your Hand:" << endl;
        cout << createCardSprite(playerHand) << "\n";
        //player chips
        cout << "____________________\n";
        cout << "Your Chips: " << "999,999" << " |\n"; // add player chips total variable

        cout << "You Can: " << "Check, Call, Raise, or Fold.\n";
        cout << "What Will You Do? "; // return string for player input
    }
}

string getPlayerInput(){
    string playerInput;
    cin >> playerInput;
    // Based on input call functions or pass input to driver function

    return playerInput;
}
