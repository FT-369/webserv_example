#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <map>

class Cluster {

    public :
        Cluster();
        Cluster(const Cluster& other);
        ~Cluster();
        Cluster& operator= (const Cluster& other);
        
        void config(char *file_path);
        // void setup();
        // int listen();
};

#endif //CLUSTER_HPP