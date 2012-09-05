
#include <iostream>
#include <boost/cgi/cgi.hpp>

namespace cgi = boost::cgi;

int main(void)
{
    try {
        cgi::request req;
        cgi::response resp;
        resp << "Hello World" << cgi::cookie("test", "check")
             << cgi::charset("ascii");
        return cgi::commit(req,resp);
    }
    catch (std::exception& e) {
        std::cout << "Content-type: text/plain\r\n\r\n"
                  << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cout << "Content-type: text/plain\r\n\r\n"
                  << "Unexpected Exception" << std::endl;
        return 1;
    }
};
