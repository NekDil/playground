
#ifndef __PLAYER__
#define __PLAYER__

#include <vector>
#include "puzzle.hpp"

namespace AnagramPuzzle
{
    class QuizMode
    {
    public:
        QuizMode() {
        }

        int play(Puzzle& puzzle_) {
            score_ = 0;
            for (int i = 0; i <= 10; i++) {
                std::cout << puzzle_.getPuzzle() << std::endl;
                std::string buf;
                std::cout << "Enter your answer: ";
    
                std::cin >> buf;
                if (puzzle_.checkPuzzle(buf)) {
                    std::cout << "CORRECT !!!" << std::endl;
                    score_ += puzzle_.getAnswer().length();
                }
                else {
                    std::cout << "WRONG !!!" << std::endl;
                    score_ -= puzzle_.getAnswer().length();
                }
            }
            return score_;
        }

        int score(void) {
            return score_;
        }

        ~QuizMode() {
        }

    private:
        int score_;
    };

    class HintMode
    {
    public:
        int play(Puzzle& puzzle_) {
            score_ = 0;
            srand(time(0));
            for (int i = 0; i <= 10; i++) {
                std::cout << puzzle_.getPuzzle() << std::endl;
                std::string buf;
                std::string ans = puzzle_.getAnswer();
                int pos = rand() % ans.length();
                std::cout << "Enter your answer (Hint: Position "
                          << pos + 1<< " is \'" << ans[pos] << "\'): ";
    
                std::cin >> buf;
                if (puzzle_.checkPuzzle(buf)) {
                    std::cout << "CORRECT !!!" << std::endl;
                    score_ += puzzle_.getAnswer().length();
                }
                else {
                    std::cout << "WRONG !!!" << std::endl;
                    score_ -= puzzle_.getAnswer().length();
                    std::cout << "ANSWER: " << ans << std::endl;
                }
            }
            return score_;
        }

        int score(void) {
            return score_;
        }
    private:
        int score_;
    };

    class HangmanMode
    {
    public:
        int play(Puzzle& puzzle_) {
            srand(time(0));
            std::string puz = puzzle_.getPuzzle();
            score_ = puz.length() / 2;

            std::cout << puz << std::endl;
            std::cout << std::endl;
            std::string buf;
            std::string ans = puzzle_.getAnswer();
            int pos;
            std::vector<char> marked;
            marked.reserve(ans.length());
            for(int i = 0; i < ans.length(); i++) {
                marked[i] = '_';
            }
            for (int i = 0; i < puz.length(); i++) std::cout << marked[i];
            std::cout << std::endl;
            while (true) {
                std::cout << "Attempt (a) / anything else gives hint" 
                          << std::endl;
                std::string c;
                std::cin >> c;
                if (c == "a" || c == "A") {
                    std::cout << "Enter Answer: ";
                    std::cin >> buf;
                    if (puzzle_.checkPuzzle(buf)) {
                        std::cout << "CORRECT !!!" << std::endl;
                    }
                    else {
                        std::cout << "WRONG !!!" << std::endl;
                        std::cout << "ANSWER: " << ans << std::endl;
                        score_ = 0;
                    }
                    return score_;
                } else {
                    if (score_ <= 0) {
                        std::cerr << "No hints left" << std::endl;
                    } else {
                        --score_;
                        do {
                            pos = rand() % ans.length();
                        } while (marked[pos] != '_');

                        marked[pos] = ans[pos];
                        std::cout << puz << std::endl;
                        for (int j = 0; j < puz.length(); j++) {
                            std::cout << marked[j];
                        }
                        std::cout << std::endl;
                    }
                    continue;
                } 
            }
        }

        int score(void) {
            return score_;
        }
    private:
        int score_;
    };

    template <typename Mode> class Player
    {
    public:
        int play() {
            mode_.play(puzzle_);
            return mode_.score();
        }

    private:
        Puzzle puzzle_;
        Mode mode_;
    };
}
#endif // __PLAYER__
