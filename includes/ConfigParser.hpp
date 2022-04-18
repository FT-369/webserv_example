#ifndef INCLUDES_CONFIGPARSER_HPP_
#define INCLUDES_CONFIGPARSER_HPP_

#include <map>
#include <vector>
#include <string>
#include "./HTTPmoduler.hpp"

class ConfigParser {
 public :
    ~ConfigParser();
    explicit ConfigParser(const char *file_path);
    ConfigParser& operator= (const ConfigParser& other);

    const char *getFilePath(void);
    void parseConfigFile(void);

 private :
    ConfigParser();
    ConfigParser(const ConfigParser& other);
    void parseBlock (std::string& lines);
    void parseMainContext(std::string& lines);
    void parseEventsBlock(std::string& lines);
    void parseHTTPBlock(std::string& lines);
    void parseServerBlock(std::string& lines);
    void showSizeDirectives(void);

    const char *file_path_;
    std::map<std::string, std::string>  mainDirectives;
    std::map<std::string, std::string>  eventDirectives;
    std::multimap<std::string, std::string> httpDirectives;
};

#endif  // INCLUDES_CONFIGPARSER_HPP_
