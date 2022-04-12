#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <map>

class Cluster {

    public :
        Cluster() {};
        Cluster(Cluster& other) {};
        ~Cluster() {};
        Cluster& operator= (const Cluster& other){};
        
        void config(char *file_path);
        void setup();
        int listen();

    private :
        std::map<std::pair<std::string, int>> config_field; 
};

#endif //CLUSTER_HPP