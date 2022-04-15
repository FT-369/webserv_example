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

static std::string getConfigFileLines(std::ifstream& fs)
{
    std::string buf;
    std::string ret;
    while (std::getline(fs, buf))
    {
        deleteComments(buf);
        ret += buf;
    }
    return ret;
}

static size_t getLastBracketIndex(std::string& line)
{
    size_t idx = 0;
    std::stack<bool> bracket;
    
    while (idx < line.length()) {
        if (line[idx] == '{')
            bracket.push(true);
        else if (line[idx] == '}') {
            bracket.pop();
            if (bracket.empty()) {
                break;
            }
        }
        idx++;
    }
    return idx;
}

static bool isAllofBracketClosed(std::string& line)
{
    std::stack<bool> bracket;
    
    if (line.find("{", 0) == STRING_NPOS)
        return false; // needed throw exception needed http server config
    for (std::string::iterator it = line.begin(); it != line.end(); it++) {
        if (*it == '{')
            bracket.push(true);
        else if (*it == '}') {
            if (bracket.empty())
                return false; // throw exception unclosed bracket;
            bracket.pop();
        }
    }
    return bracket.empty() ? true : false;
}

static std::string getkeywordBlock(std::string& line)
{
    std::string ret;
    std::string::iterator it = line.begin();
    while (!isalpha(*it))
        it++;
    size_t idx = line.find_first_of(" \t\r\n{", 0);
    ret = line.substr(0, idx);
    line.erase(0, idx);
    return ret;
}

void ConfigParser::parseMainContext(std::string& lines)
{
    std::size_t idx_first_block = lines.find("{", 0);
    std::string tmp, key, value;
    for (std::size_t base = 0, found = 0;;)
    {
        while (isspace(lines[base])) base++;
        found = lines.find(";", base);
        if (found > idx_first_block || found >= STRING_NPOS) {
            lines.erase(0, base);
            break;
        }
        tmp = lines.substr(base, found - base);
        std::string::iterator it = tmp.begin();
        while (isalpha(*it) || isSpecialCharactersBlockKeyword(*it)) {
            key += *it;
            it++;
        }
        while (isspace(*it))
            it++;
        while (it != tmp.end()) {
            value += *it;
            it++;
        }
        this->mainDirectives.insert(make_pair(key, value));
        key.clear();
        value.clear();
        base = found + 1;
    }
}

static std::string getvaluesBlock(std::string& lines) {
    size_t bracket_end_idx = getLastBracketIndex(lines);
    std::string ret = lines.substr(lines.find('{', 0), bracket_end_idx);
    lines.erase(0, bracket_end_idx);
    std::string::iterator it = lines.begin();
    while (it != lines.end() && isspace(*it)) {
        it++;
    }
    return ret;
}


// int ConfigParser::parseHTTPBlock(const std::pair<std::string, std::string>& blockContents) {
//     return DONE;
// }

int ConfigParser::parseEventsBlock(const std::pair<std::string, std::string>& blockContents) {
    return DONE;
}

static void parseBlock (std::string& lines) {
    std::pair<std::string, std::string> blockContents = make_pair(getkeywordBlock(lines), getvaluesBlock(lines));
    if (!blockContents.first.compare("events"))
        this->parseEventsBlock(blockContents);
    if (!blockContents.first.compare("http"))
        std::cout << "http" << std::endl;
    if (!blockContents.first.compare("server"))
        std::cout << "server" << std::endl;
    std::cout << blockContents.first << "|" << blockContents.second << "|" << std::endl;
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
    if (isAllofBracketClosed(lines) == false)
    {
        std::cerr << ERR_MSG_HAED << "unclosed bracket" << std::endl;
        return ;
    }
    this->parseMainContext(lines);
    while (lines.find('{', 0) != STRING_NPOS) {
        parseBlock(lines);
    }
}
