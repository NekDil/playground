
#ifndef __PUZZLE__
#define __PUZZLE__

#include "words.hpp"

using AnagramPuzzle::InternetFeed;
using AnagramPuzzle::LocalFeed;
using AnagramPuzzle::Source;

namespace AnagramPuzzle
{
    const bool INTERNET_MODE = true;
    const bool LOCAL_MODE = false;
    class Puzzle
    {
    public:
        Puzzle(bool mode = LOCAL_MODE) :
            mode_(mode) {
        }
        
        std::string& getAnswer() {
            return answer_;
        }

        std::string& getPuzzle() {
            if (mode_ == INTERNET_MODE) {
                Source<InternetFeed> feed_;
                feed_.fetchWord();
                answer_ = feed_.word();
                anagram_ = answer_;
            }
            else {
                Source<LocalFeed> feed_;
                feed_.fetchWord();
                answer_ = feed_.word();
                anagram_ = answer_;
            }
            return shuffle();
        }
        
        bool checkPuzzle(const std::string& sol) {
            if (answer_ == sol) {
                return true;
            }
            return false;
        }

    private:
        std::string answer_;
        std::string anagram_;
        int mode_;

        std::string& shuffle() {
            srand(time(0));
            int len = anagram_.length();
            int shuffle_loc = 0;
            for (int i = 0; i < len; i++) {
                shuffle_loc = rand() % len;
                char temp = anagram_[i];
                anagram_[i] = anagram_[shuffle_loc];
                anagram_[shuffle_loc] = temp;
            }
            return anagram_;
        }
    };
}

#endif // __PUZZLE__
