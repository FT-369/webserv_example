  // Copyright [2022] <Two-Jay>

#include "../includes/ConfigParser.hpp"
#include "../includes/webserv.hpp"
#include <iostream>
#include <map>
#include <string>
#include <fstream>

#define space "\n\t "

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

static bool isSpecialCharactersBlockKeyword(const char c)
{
    return (c == '~' || c == '^' || c == '/' || c == '(' || c == ')' || c == '_');
}

void ConfigParser::parseDerectivelineInMain(std::string line) {
    std::string::iterator it = line.begin();
    std::string key;
    std::string value;

    while (isalpha(*it) || isSpecialCharactersBlockKeyword(*it)) {
        key += *it;
        it++;
    }
    while (isspace(*it))
        it++;
    while (it != line.end()) {
        value += *it;
        it++;
    }
    // std::cout << "[" << line << "]" << std::endl;
    // std::cout << "[" << key << "]" << std::endl;
    // std::cout << "[" << value << "]" << std::endl;
    this->mainDirectives.insert(make_pair(key, value));
}

// int ConfigParser::parseHTTPBlock(const std::string& keyword) {
//     return DONE;
// }

// int ConfigParser::parseEventsBlock(const std::string& keyword) {
//     return DONE;
// }

static std::string getConfigFileLines(std::ifstream& fs)
{
    std::string  buf;
    std::string  ret;
    while (std::getline(fs, buf))
    {
        deleteComments(buf);
        ret += buf;
    }
    return ret;
}

void ConfigParser::parseConfigFile(void) {
    std::ifstream   fs;
    std::string     lines;
    
    fs.open(file_path_, std::ios::in);
    if (!fs.is_open())
    {
        std::cerr << ERR_MSG_HAED << "file open err" << std::endl;
        return ;
    }
    lines = getConfigFileLines(fs);
    fs.close();
    std::size_t idx_first_block = lines.find("{", 0);
    for (std::size_t base = 0, found = 0;;)
    {
        while (isspace(lines[base])) base++;
        found = lines.find(";", base);
        if (found > idx_first_block || found >= STRING_NPOS) {
            lines.erase(0, base);
            break;
        }
        parseDerectivelineInMain(lines.substr(base, found - base));
        base = found + 1;
    }
    std::cout << lines << std::endl;
}

