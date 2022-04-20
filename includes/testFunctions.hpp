#ifndef TESTFUNCTIONS_HPP
#define TESTFUNCTIONS_HPP

#include <iostream>
namespace test {
    void showSizeDirectives(const std::map<std::string, std::string>& m) {
        std::cout << "size : " << m.size() << std::endl;
    };
    void showParsedDirectives(std::map<std::string, std::vector<std::string> >& a) {
    std::cout << "size : " << a.size() << std::endl;
    for (std::map<std::string, std::vector<std::string> >::iterator it = a.begin(); it != a.end(); it++) {
        std:: cout << it->first << " : [ ";
        for (std::vector<std::string>::iterator itv = it->second.begin(); itv != it->second.end(); itv++) {
            std::cout << *itv << ", ";
        }
        std::cout << "]"<< std::endl;
    }
};

}

#endif // TESTFUNCTIONS_HPP