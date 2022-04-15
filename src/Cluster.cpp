  // Copyright [2022] <Two-Jay>

#include "../includes/webserv.hpp"
#include "../includes/Cluster.hpp"
#include "../includes/ConfigParser.hpp"

Cluster::Cluster(void) {}
Cluster::Cluster(const Cluster& other) {}
Cluster::~Cluster() {}
Cluster& Cluster::operator= (const Cluster& other) {}

void Cluster::config(const char *path)
{
    ConfigParser parser(path);
}