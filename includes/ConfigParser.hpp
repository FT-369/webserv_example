#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <map>

#define DEFAULT_CONFIGFILE_PATH "./config./default.config"

class ConfigParser
{
    public :
        ~ConfigParser();
        ConfigParser(const char *file_path);
        ConfigParser& operator= (const ConfigParser& other);

        const char *getFilePath(void);
        std::map<std::string, std::string>& getAttribute(void);
        void parseConfigFile(const char *file_path);

    private :
        ConfigParser();
        ConfigParser(const ConfigParser& other);

        const char *file_path_;
        std::map<std::string, std::string> attributes_;
};

#endif //CONFIGPARSER_HPP