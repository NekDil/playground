
#ifndef __WORDS__
#define __WORDS__

#include <string>
#include <iostream>
#include <sstream>
#include <exception>
#include <map>
#include <fstream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Types.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

namespace AnagramPuzzle
{
    class InternetFeed {
    public:
        InternetFeed() {
            url_ = "http://randomword.setgetgo.com/get.php";
        }

        std::string fetchWord(void) {
            // RAII Cleanup
            curlpp::Cleanup myCleanup;
            std::ostringstream os; 
            // Request Object
            curlpp::Easy myRequest;
                
            try {
                // Set the URL
                myRequest.setOpt<curlpp::options::Url>(url_);

                // Send Request and get the result
                myRequest.perform();
            }
            catch (std::exception& e) {
                std::cerr << "Problem getting the word from internet";
                std::cerr << e.what();
                abort();
            }

            os << myRequest;
            return os.str();
        }
    private:
        std::string url_;
    };
    
    const std::string FILE_URL = "./words.txt";

    class LocalFeed {
    public:
        LocalFeed(const std::string& url = FILE_URL) {
            srand(time(0));
            url_ = url;
            int line_no = 0;
            char dummy[128];
            try {
                input_.open(url_.c_str(), std::ifstream::in);

                if (input_.bad() || input_.fail()) {
                    throw std::runtime_error("Failed to open file");
                }

                //index_[line_no] = input_.tellg();

                while (true) {
                    input_.getline(dummy, 128);
                    if (input_.eof()) break;
                    ++line_no;
                    index_[line_no] = input_.tellg();
#ifndef NDEBUG
                    std::cout << "line: " << line_no << " streampos: " << input_.tellg() << std::endl;
#endif
                }
                max_line = line_no;
                input_.clear();
            }
            catch (std::exception& e) {
                std::cerr << "Problem with getting word from file" << std::endl;
                std::cerr << e.what() << std::endl;
                abort();
            }
        }
        
        virtual ~LocalFeed() {
            input_.close();
        }

        std::string fetchWord(void) {
#ifndef NDEBUG
            std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
            return getRandomWord();
        }

    private:
        std::string url_;
        std::ifstream input_;
        std::map <int, std::streampos> index_;
        int max_line;

        std::string getRandomWord(void) {
#ifndef NDEBUG
            std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
            int random_line = rand() % max_line + 1;
            char dummy[128];
            std::streampos pos = index_[random_line];

            if (input_.bad() || input_.fail()) abort();
            input_.seekg(pos, std::ios::beg);

            input_.getline(dummy, 128);
#ifndef NDEBUG            
            std::cout << input_.gcount() << " characters " << std::endl;
            std::cout << std::string(dummy) << " from pos " << pos << std::endl;
#endif
            return std::string(dummy);
        }
    };

    template <typename WordSource=InternetFeed> class Source
    {
    public:
        Source() {
        }

        void fetchWord() {
            word_ = source_.fetchWord();
        }

        const std::string& word(void) {
            return word_;
        }

    private:
        Source(const Source& source);
        void operator=(const Source& source);
        std::string word_;
        WordSource source_;
    };
}

#endif // __WORDS__
