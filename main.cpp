#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <unistd.h> 
using namespace std;

enum Operations{Match=1, Display =2,Giveup =3,Exit =4};
const int SIZE =4;
const int SHUFFLE_COUNT=15; 


// Function definition
string getName() {
    string username;
    bool validUser;

    do {
        cout << "Enter your username: ";
        getline(cin, username); // Get the entire line including spaces
        validUser = true; // Assume the username is valid

        // Check for invalid characters
        for (int i = 0; i < username.length(); i++) {
            if (!isalpha(username.at(i)) && !isdigit(username.at(i)) && !isspace(username.at(i))) {
                validUser = false; // Mark as invalid
            }
        }

        // If the username is invalid, prompt the user again
        if (!validUser) {
            cout << "Invalid username. Please enter only alphabets, numbers, and whitespaces." << endl;
        }
    } while (!validUser); 

    return username; 
}
// function to generate a random number between 0 and SIZE - 1
int genRandomIndex() {
     
    return rand() % SIZE; 
}


void shuffle(int array[SIZE][SIZE]) {
    for (int i = 0; i < SHUFFLE_COUNT; i++) {
        // Get random indices for the first card
        int x1 = genRandomIndex();
        int y1 = genRandomIndex();
        
        // Get random indices for the second card
        int x2 = genRandomIndex();
        int y2 = genRandomIndex();
        
        // Swap the two cards
        int temp = array[x1][y1]; 
        array[x1][y1] = array[x2][y2]; 
        array[x2][y2] = temp; 
    }
}

void initialize(int cards[SIZE][SIZE], int status[SIZE][SIZE]) {
    int cardValue = 1;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cards[i][j] = (cardValue + j / 2) % 8 + 1;
            status[i][j] = 0;
        }
        cardValue += 2;
    }
    shuffle(cards);
}

void showCards(int cards[SIZE][SIZE], int status[SIZE][SIZE], bool showReal = false) {
    cout << "Current card status:" << endl;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (status[i][j] == 1 || showReal) { // Show real cards or face up
                cout << cards[i][j] << "\t";
            } else {
                cout << "*\t"; // Face down card
            }
        }
        cout << endl; // New line for next row
    }
}

bool allFaceUp(int status[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (status[i][j] == 0) {
                return false; 
            }
        }
    }
    return true; 
}

bool checkInputs(int x, int y, int status[SIZE][SIZE]) {
    if (x >= 0 && x < SIZE && y >= 0 && y < SIZE && status[x][y] == 0) {
        return true; // Valid input
    }
    return false; // Invalid input
}

void match(int cards[SIZE][SIZE], int status[SIZE][SIZE], int &points) {
    int x1, y1, x2, y2;

    // Show current arrangement of cards (only face-up cards will be shown)
    showCards(cards, status);

    // Ask for coordinates of the first card
    cout << "Enter coordinates of the first card (x y): ";
    cin >> x1 >> y1;

    // Validate first coordinates
    if (!checkInputs(x1, y1, status)) {
        cout << "Invalid coordinates or card already faced up.\n";
        return;
    }

    // Ask for coordinates of the second card
    cout << "Enter coordinates of the second card (x y): ";
    cin >> x2 >> y2;

    // Validate second coordinates and check if they are different from the first coordinates
    if ((x1 == x2 && y1 == y2) || !checkInputs(x2, y2, status)) {
        cout << "Invalid coordinates, cards already faced up, or duplicate selection.\n";
        return;
    }

    // Temporarily reveal both selected cards
    status[x1][y1] = status[x2][y2] = 1;
    showCards(cards, status, false); // Show the selected cards temporarily

    // Check if the two cards match
    if (cards[x1][y1] == cards[x2][y2]) {
        cout << "It's a match! You earned 5 points.\n";
        points += 5; // Increase points for a correct match
    } else {
        // Show unmatched cards briefly
        cout << "No match! You lost 1 point.\n";
        points -= 1; // Decrease points for incorrect guess
        sleep(5); // Pause to allow the player to memorize
        status[x1][y1] = status[x2][y2] = 0; // Hide the cards again
        system("clear"); // Clear screen to hide unmatched cards
    }

    // Show the board with updated status (only matched cards remain face up)
    showCards(cards, status);
}

void display(int cards[SIZE][SIZE], int status[SIZE][SIZE], int &points) {
    if (points < 10) {
        cout << "Insufficient points to display identical pairs." << endl;
        return;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (status[i][j] == 0) { // Find the first card that is face down
                int cardValue = cards[i][j];
                // Now find the second matching card
                for (int x = 0; x < SIZE; x++) {
                    for (int y = 0; y < SIZE; y++) {
                        if (status[x][y] == 0 && cards[x][y] == cardValue && !(x == i && y == j)) {
                            // Found matching pair
                            status[i][j] = 1; // Set first card to face up
                            status[x][y] = 1; // Set second card to face up
                            cout << "Displaying a matching pair!" << endl;
                            showCards(cards, status);
                            points -= 10; // Decrease points
                            return;
                        }
                    }
                }
            }
        }
    }

    cout << "No pairs available to display." << endl;
}





int main() {
    srand((unsigned)time(0));
    int points =50;
    int cards[SIZE][SIZE];
    int status[SIZE][SIZE];

    initialize(cards, status);

    string playerName = getName();

    cout << "Welcome, " << playerName << "!" << endl;
    while (points >= 0) { // Loop until the player exits or points go below zero
        cout << "\nMenu:\n";
        cout << "1. Match\n";
        cout << "2. Display identical pairs\n";
        cout << "3. Give up\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        
        int choice;
        cin >> choice;
     Operations menuChoice = static_cast<Operations>(choice); // Use enum type for switch expression
        switch (menuChoice) {
            case Match:
                match(cards, status, points);
                if (allFaceUp(status)) {
                    cout << "Congratulations! You've matched all cards and won the game!\n";
                    char playAgain;
                    cout << "Would you like to play again? (y/n): ";
                    cin >> playAgain;
                    if (playAgain == 'y' || playAgain == 'Y') {
                        points = 50; // Reset points for a new game
                        initialize(cards, status); // Reinitialize the game
                    } else {
                        cout << "Thank you for playing, " << playerName << "!\n";
                        return 0;
                    }
                }
                break;

            case Display:
                display(cards, status, points);
                break;

            case Giveup:
                cout << "You chose to give up. Here is the final arrangement of the cards:\n";
                showCards(cards, status, true); // Show all cards
                char playAgain;
                cout << "Would you like to play again? (y/n): ";
                cin >> playAgain;
                if (playAgain == 'y' || playAgain == 'Y') {
                    points = 50; // Reset points for a new game
                    initialize(cards, status); // Reinitialize the game
                } else {
                    cout << "Thank you for playing, " << playerName << "!\n";
                    return 0;
                }
                break;

            case Exit:
                cout << "Goodbye, " << playerName << "! Your final points balance is: " << points << ".\n";
                return 0;

            default:
                cout << "Invalid choice. Please enter a number between 1 and 4.\n";
                break;
        }

        // Display current points
        cout << "Current points: " << points << endl;

        // Step 19: Check if points are below zero
        if (points < 0) {
            cout << "Game over! Your points balance has fallen below zero.\n";
            break;
        }
    }
    
    return 0;
}