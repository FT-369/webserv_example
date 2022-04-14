#include <iostream>
#include "./includes/webserv.hpp"
// #include "./includes/Cluster.hpp"
#include "./includes/ConfigParser.hpp"

int main(int argc, char **argv, char **envp)
{
    // Cluster cluster;
    ConfigParser *c;
    (void)argv;
    (void)envp;

    if (argc == 1) {
        // Cluster.config(DEFAULT_CONFIGFILE_PATH);
        c = new ConfigParser(DEFAULT_CONFIGFILE_PATH);
        c->parseConfigFile();
    }
    // if (argc == 2)
    // {
    //     Cluster.config(argv[1]);
    // }
    else
        std::cerr << ERR_PARAM_MISSING << std::endl;
    // Cluster.setup();
    // Cluster.listen();
    return 0;
}