#include "iostream"
#include "string"

#include <cstdlib>
#include <ctime>

int main() {
    std::cout << "Woah this works???" << std::endl << std::endl;
    std::cout << "Guess a number between one and fifty. " << std::endl << std::endl;

    srand(time(0)); //seed rng

    int secret = rand() % 50 + 1;
    int guessCount = 0;

    int playerGuess;
    std::string strGuess;

    do {
        std::cout << "Guess my secret number: ";
        std::getline(std::cin, strGuess);

        try {
            playerGuess = std::stoi(strGuess);
        } catch (...) {
            std::cout << "Could not turn your guess into an integer. Please try again. " << std::endl;
            continue;
        }

        if (playerGuess > secret) {
            std::cout << "Psst! Guess lower!! ";
        } else if (playerGuess < secret) {
            std::cout << "Psst! Guess higher!! ";
        }

        guessCount ++;

    } while (playerGuess != secret);

    std::cout << "You won in " << guessCount << (guessCount == 1 ? " guess!" : " guesses.") << std::endl;

    return 0;
}