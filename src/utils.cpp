#include "utils.hpp"

std::vector<std::string> ft_split(std::string const line, std::string const delimiter)
{
	std::vector<std::string> words;
	std::string new_line = line;
	size_t delimiter_len = delimiter.length();

	size_t pos;
	while ((pos = new_line.find(delimiter)) != std::string::npos) {
		words.push_back(new_line.substr(0, pos));
		new_line = new_line.substr(pos + delimiter_len);
	}
	words.push_back(new_line);

	return words;
}

std::string ft_fgets_line(FILE* fp)
{
	char line[GET_LINE_BUF] = { 0, };
	std::string getline;
	size_t len;

	if (!fgets(line, GET_LINE_BUF, fp))
		return getline;
	getline = std::string(line);
	
	while ((len = std::strlen(line)) && line[len - 1] != '\n')
	{
		if (!fgets(line, GET_LINE_BUF, fp))
			return getline;
		getline += std::string(line);
	}
	return getline;
}
