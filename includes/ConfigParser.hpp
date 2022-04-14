#ifndef INCLUDES_CONFIGPARSER_HPP_
#define INCLUDES_CONFIGPARSER_HPP_

#include <map>
#include <string>

class ConfigParser {
 public :
    ~ConfigParser();
    explicit ConfigParser(const char *file_path);
    ConfigParser& operator= (const ConfigParser& other);

    const char *getFilePath(void);
    std::map<std::string, std::string>& getAttribute(void);
    void parseConfigFile(void);
 private :
    ConfigParser();
    ConfigParser(const ConfigParser& other);
    void parseDerectivelineInMain(std::string line);

    const char *file_path_;
    std::map<std::string, std::string> mainDirectives;
    std::multimap<std::string, std::map<std::string, std::string> > d;
    
};

#endif  // INCLUDES_CONFIGPARSER_HPP_
