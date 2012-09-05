
#include "player.hpp"

using AnagramPuzzle::Player;
using AnagramPuzzle::QuizMode;
using AnagramPuzzle::HintMode;
using AnagramPuzzle::HangmanMode;

int main(void)
{
    Player<HangmanMode> p;
    std::cout << p.play() << std::endl;
    return 0;
}
