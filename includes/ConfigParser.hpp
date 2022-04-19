#ifndef INCLUDES_CONFIGPARSER_HPP_
#define INCLUDES_CONFIGPARSER_HPP_

#include <map>
#include <vector>
#include <string>

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
    void parseServerBlock(std::string& lines, std::vector<std::map<std::string, std::vector<std::string> > >& a);
    void test_map_data_print(void);

    const char *file_path_;
    std::map<std::string, std::vector<std::string> >  mainDirectives;
    std::map<std::string, std::vector<std::string> >  eventDirectives;
    std::map<std::string, std::vector<std::string> > httpDirectives;
    std::vector<std::map<std::string, std::vector<std::string> > > serverBlocks;
};

#endif  // INCLUDES_CONFIGPARSER_HPP_
