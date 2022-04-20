  // Copyright [2022] <Two-Jay>

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include "../includes/ConfigParser.hpp"
#include "../includes/testFunctions.hpp"
#include "../includes/webserv.hpp"

#define space "\n\t "

ConfigParser::ConfigParser() {}
ConfigParser::ConfigParser(const char *file_path) {
    if (!*file_path) {
        file_path_ = DEFAULT_CONFIGFILE_PATH;
    } else {
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

static bool isSpecialCharactersBlockKeyword(const char c) {
    return (c == '~' || c == '^' || c == '/'
        || c == '(' || c == ')' || c == '_');
}

static std::string getConfigFileLines(std::ifstream& fs) {
    std::string buf;
    std::string ret;
    while (std::getline(fs, buf)) {
        deleteComments(buf);
        ret += buf;
    }
    return ret;
}

static bool isAllofBracketClosed(std::string& line) {
    std::stack<bool> bracket;
    if (line.find("{", 0) == STRING_NPOS) return false;  // needed throw exception needed http server config
    for (std::string::iterator it = line.begin(); it != line.end(); it++) {
        if (*it == '{') {
            bracket.push(true);
        } else if (*it == '}') {
            if (bracket.empty()) return false;  // throw exception unclosed bracket;
            bracket.pop();
        }
    }
    return bracket.empty() ? true : false;
}

static size_t getLastBracketIndex(std::string& line) {
    size_t idx = 0;
    std::stack<bool> bracket;
    while (idx < line.length()) {
        if (line[idx] == '{') {
            bracket.push(true);
        } else if (line[idx] == '}') {
            bracket.pop();
            if (bracket.empty()) break;
        }
        idx++;
    }
    return idx;
}

// 블록 파싱할 때 키워드 체크함수
// 이곳에서 블록 키워드가 제대로 파싱이 안되어 있는 걸 체크합니다.
// 향후 허용되지 않는 키가 들어오는 경우는 여기에서 체크하도록 합시다.
static bool isKeywordBlock(std::string& line) {
    std::string::iterator it = line.begin();
    for (; it != line.end(); it++) if (isalpha(*it)) { break ; }
    if (it == line.end())
        return false;
    return true;
}

static std::string getkeywordBlock(std::string& line) {
    size_t start = 0;
    while (!isalpha(line[start]) || line[start] == '}') start++;
    size_t idx = line.find_first_of(" \t\r\n{", start);
    std::string ret = line.substr(start, idx - start);
    if (!isKeywordBlock(ret)) std::cout << "Error !!!!!!!!" << ret << "!!!!!!!!!!!" << std::endl;  // 키워드블록 파싱 에러
    line.erase(0, ret.length());
    return ret;
}

static std::string getvaluesBlock(std::string& lines) {
    size_t bracket_end_idx = getLastBracketIndex(lines);
    size_t bracket_start_idx = lines.find('{', 0);
    std::string ret = lines.substr(++bracket_start_idx, bracket_end_idx);
    lines.erase(0, ret.length());
    return ret;
}

static void parseDirectives(std::string& lines, std::map<std::string, std::vector<std::string> >& map) {
    std::size_t idx_first_block = lines.find("{", (lines[0] == '{'));
    std::string tmp, key, value;
    for (std::size_t base = 0, found = 0;;) {
        while (isspace(lines[base])) base++;
        found = lines.find(";", base);
        if (found > idx_first_block || found == STRING_NPOS) {
            lines.erase(0, base);
            break;
        }
        tmp = lines.substr(base, found - base);
        std::string::iterator it = tmp.begin();
        while (isalpha(*it) || isSpecialCharactersBlockKeyword(*it)) {
            key += *it;
            it++;
        }
        while (isspace(*it)) it++;
        while (it != tmp.end()) {
            value += *it;
            it++;
        }
        if (map.find(key) == map.end()) {
            std::vector<std::string> bucket;
            bucket.push_back(value);
            map.insert(make_pair(key, bucket));
        } else {
            map.find(key)->second.push_back(value);
        }
        key.clear();
        value.clear();
        base = found + 1;
    }
}

void ConfigParser::parseMainContext(std::string& lines) {
    parseDirectives(lines, this->mainDirectives);
}

void ConfigParser::parseEventsBlock(std::string& lines) {
    parseDirectives(lines, this->eventDirectives);
}

void ConfigParser::parseUpstreamBlock(std::string& lines) {
    parseDirectives(lines, this->upstreamDirectives);
}

void ConfigParser::parseHTTPBlock(std::string& lines) {
    parseDirectives(lines, this->httpDirectives);
    while (lines.find('{', 0) != STRING_NPOS) {
        parseBlock(lines);
    }
}

void parseLocationURLinfo (std::string& lines, std::map<std::string, std::vector<std::string> >& bucket) {
        lines.erase(0, 8);
        size_t cut_start = 0;
        while (isspace(lines[cut_start])) cut_start++;
        size_t first_bracket_appeared_idx = lines.find("{", 0);
        std::string ret = lines.substr(cut_start, --first_bracket_appeared_idx);
        lines.erase(0, first_bracket_appeared_idx);
        std::vector<std::string> v;
        v.push_back(ret);
        bucket.insert(make_pair("location_URL_info", v));
        lines.insert(0, "location ");
}

void ConfigParser::parseServerBlock(std::string& lines,
    std::vector<std::map<std::string, std::vector<std::string> > >&  a) {
    // 두번째 인자는 로직 입증을 위해 임시로 넣어둠
    // 향후 구현 방향에 따라 서버 블록 내의 Directives를 저장할 수 있도록 알맞게 수정해야함.
    std::map<std::string, std::vector<std::string> > t;
    parseDirectives(lines, t);
    while (lines.find('{', 0) != STRING_NPOS) {
        parseLocationURLinfo(lines, t);
        parseBlock(lines);
    }
    if (t.size() != 0) a.push_back(t);
}

void ConfigParser::parseBlock(std::string& lines) {
    std::pair<std::string, std::string> blockContents = make_pair(getkeywordBlock(lines), getvaluesBlock(lines));
    if (blockContents.first.length() == 0) {
        std::cout << "ak!" << std::endl;
        return;
    }
    if (!blockContents.first.compare("events")) {
        parseEventsBlock(blockContents.second);
    }
    if (!blockContents.first.compare("http")) {
        parseHTTPBlock(blockContents.second);
    }
    if (!blockContents.first.compare("server")) {
        parseServerBlock(blockContents.second, this->serverBlocks);
    }
    if (!blockContents.first.compare("upstream")) {
        parseUpstreamBlock(blockContents.second);
    }
    if (blockContents.first.length() >= 8 && blockContents.first.find("location", 0) != STRING_NPOS) {
        parseServerBlock(blockContents.second, this->serverBlocks);
    }
}

void ConfigParser::parseConfigFile(void) {
    std::ifstream   fs;
    std::string     lines;
    fs.open(file_path_, std::ios::in);
    if (!fs.is_open()) {
        std::cerr << ERR_MSG_HAED << "file open err" << std::endl;
        return;
    }
    lines = getConfigFileLines(fs);
    fs.close();
    if (isAllofBracketClosed(lines) == false) {
        std::cerr << ERR_MSG_HAED << "unclosed bracket" << std::endl;
        return;
    }
    parseMainContext(lines);
    while (lines.find('{', 0) != STRING_NPOS) {
        parseBlock(lines);
    }
    test_map_data_print();
}

void test_serverBlock_print(std::vector<std::map<std::string, std::vector<std::string> > >& a) {
    std::vector<std::map<std::string, std::vector<std::string> > >::iterator it = a.begin();
    for (; it != a.end(); it++) {
        test::showParsedDirectives(*it);
    }
}

void ConfigParser::test_map_data_print(void) {
    std::cout << "main -------------" << std::endl;
    test::showParsedDirectives(this->mainDirectives);
    std::cout << "events -------------" << std::endl;
    test::showParsedDirectives(this->eventDirectives);
    std::cout << "http -------------" << std::endl;
    test::showParsedDirectives(this->httpDirectives);
    std::cout << "upstream -------------" << std::endl;
    test::showParsedDirectives(this->upstreamDirectives);
    std::cout << "server -------------" << std::endl;
    test_serverBlock_print(this->serverBlocks);
}
