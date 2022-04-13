  // Copyright [2022] <Two-Jay>

#include "../includes/ConfigParser.hpp"
#include "../includes/webserv.hpp"
#include <iostream>
#include <fstream>

ConfigParser::ConfigParser() {}
ConfigParser::ConfigParser(const ConfigParser& other) {}
ConfigParser::~ConfigParser() {}
ConfigParser& ConfigParser::operator= (const ConfigParser& other) {}

void ConfigParser::parseConfigFile(const char *file_path) {
    std::ifstream   fs;
    std::string     lines;
    
    if (!*file_path) {
        file_path = DEFAULT_CONFIGFILE_PATH;
    }
    else {
        file_path_ = file_path;
    }
    fs.open(file_path, std::ios::in);
    if (!fs.is_open())
    {
        std::cerr << ERR_MSG_HAED << "file open err" << std::endl;
        return ;
    }
    while (!fs.eof())
    {
        fs >> lines;
    }
    std::cout << lines << std::endl;
}