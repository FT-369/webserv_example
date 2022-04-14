  // Copyright [2022] <Two-Jay>

#include "../includes/ConfigParser.hpp"
#include "../includes/webserv.hpp"
#include <iostream>
#include <string>
#include <fstream>

ConfigParser::ConfigParser() {}
ConfigParser::ConfigParser(const char *file_path) {
    if (!*file_path) {
        file_path_ = DEFAULT_CONFIGFILE_PATH;
    }
    else {
        file_path_ = file_path;
    }
}
ConfigParser::ConfigParser(const ConfigParser& other) { (void)other; }
ConfigParser::~ConfigParser() {}
ConfigParser& ConfigParser::operator= (const ConfigParser& other) {
    (void)other;
    return *this;
}

static void deleteComments(std::string& line) {
    size_t idx = line.find("#");
    if (idx != STRING_NPOS) {
        line.erase(idx, line.length() - idx);
    }
}

void ConfigParser::parseConfigFile(void) {
    std::ifstream   fs;
    std::string     buf;
    std::string     lines;
    
    fs.open(file_path_, std::ios::in);
    if (!fs.is_open())
    {
        std::cerr << ERR_MSG_HAED << "file open err" << std::endl;
        return ;
    }
    while (std::getline(fs, buf))
    {
        deleteComments(buf);
        lines += buf;
        lines += '\n';
    }
    std::cout << lines << std::endl;
}