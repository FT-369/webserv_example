#include <iostream>
#include "./includes/webserv.hpp"
#include "./includes/Cluster.hpp"

int main(int argc, char **argv char **envp)
{
    Cluster cluster;

    if (argc == 2)
    {
        Cluster.config(argv[1]);
        Cluster.setup();
        Cluster.listen();
    }
    else
        std::cerr << ERR_PARAM_MISSING << std::endl;
    return 0;
}