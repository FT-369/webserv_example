#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <vector>

# define GET_LINE_BUF 1024
# define ERROR -1
# define SUCCESS 0

std::vector<std::string> ft_split(std::string const line, std::string const delimiter);
std::string ft_fgets_line(FILE* fp);

#endif