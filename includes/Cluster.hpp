#ifndef INCLUDES_CLUSTER_HPP_
#define INCLUDES_CLUSTER_HPP_

#include <map>
#include <string>

class Cluster {
 public :
    Cluster();
    Cluster(const Cluster& other);
    ~Cluster();
    Cluster& operator= (const Cluster& other);
    void config(const char *file_path);
    // void setup();
    // int listen();

 private :
    void setUser_(const std::string& input);
    void setErrorLogPath(const std::string& input);
    void setWorkerProcesses(size_t input);
    std::string user_;
    std::string error_log_path;
    size_t      worker_process;
};

#endif  // INCLUDES_CLUSTER_HPP_
