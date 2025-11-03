/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fileUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trahanta <trahanta@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 08:11:46 by barandri          #+#    #+#             */
/*   Updated: 2025/11/03 23:43:50 by trahanta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	printFileContent(std::string const & path)
{
	std::string		line;
	std::ifstream	file;

	file.open(path.c_str());
	if (!file.is_open())
		return ;
	while (!file.eof())
	{
		std::getline(file, line);
		std::cout << line << std::endl;
	}
}

void    checkFileExtension(std::string const & fileName, std::string extension)
{
	if (fileName.compare(fileName.size() - extension.size(), extension.size(), extension) != 0)
		throw std::runtime_error("Extension file problem");
}

std::string readFileToString(std::string filePath)
{
	std::fstream file;

	file.open(filePath.c_str());
	if (!file.is_open())
		throw std::runtime_error("ReadFileToString : File can't be opened");
	std::string fileString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return (fileString);
}

stringMap splitFileToMapString(std::string const &filePath, std::string const & sep)
{
	std::fstream	file;
	std::string		line;
	stringPair		pair;
	stringVect		vect;
	stringMap		result;

	file.open(filePath.c_str());
	if (!file.is_open())
		throw std::runtime_error("splitFileToMapString : File is not available");
	while (getline(file, line))
	{
		vect = strictSplit(line, sep);
		pair.first = vect[0];
		pair.second = vect[1];
		result.insert(pair);
		vect.clear();
		line.clear();
	}
	file.close();
	return (result);
}

std::string	getFileExtension(std::string const & fileName)
{
	std::string ext_file;
	int 		pe;

	ext_file = fileName;
	pe = fileName.find_last_of(".");
	ext_file.erase(0, pe + 1);
	return ext_file;
}

std::vector<std::string> getContentFile(std::string _path)
{
	std::fstream fluxFile;
	std::vector<std::string> lines;
	fluxFile.open(_path.c_str(), std::ios::in);
	if (fluxFile.is_open())
	{
		std::string line;
		while (std::getline(fluxFile, line))
		{
			line += "\n";
			lines.push_back(line);
		}
		fluxFile.close();
	}
	else
		std::cout << "Error : to open the file" << std::endl;
	return (lines);
}

std::vector<int> findLinesContain(const std::vector<std::string> &lines, const std::string &word)
{
	std::vector<int> Index;
	for (int i = 0; i < (int)lines.size(); ++i)
	{
		if (lines[i].find(word) != std::string::npos)
			Index.push_back(i);
	}
	return (Index);
}
std::string cleanLine(const std::string &s)
{
	std::string result = s;
	while (!result.empty() && (result[result.size() - 1] == '\r' || result[result.size() - 1] == '\n'))
		result.erase(result.size() - 1);
	return result;
}

std::vector<int> containsExactWordInVector(const std::vector<std::string> &lines, const std::string &word)
{
	std::vector<int> index;
	int i = 0;
	for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it, i++)
	{
		const std::string &line = *it;

		std::istringstream iss(line);
		std::string w;
		while (iss >> w)
		{
			if (w == word)
			{
				index.push_back(i);
			}
		}
	}
	return (index);
}

bool checkWriteConfig(const std::vector<std::string> &lines, const std::string &word, const std::string &writeLine)
{
	bool foundWord = false;

	std::vector<int> index = containsExactWordInVector(lines, "server_names");
	for (size_t i = 0; i < lines.size(); ++i)
	{
		std::string line = cleanLine(lines[i]);
		bool skypIndex = false;
		for (int j = 0; j < (int)index.size(); j++)
		{
			if ((int)i == index[j])
			{
				skypIndex = true;
				break;
			}
		}
		if (skypIndex)
			continue;
		if (line.find(word) != std::string::npos)
		{
			foundWord = true;
			if (line != writeLine)
			{
				std::cout << i << std::endl;
				std::cout << line << std::endl;
				std::cerr << "Syntax Error\n"
						  << std::endl;
				return false;
			}
		}
	}
	if (!foundWord)
	{
		std::cerr << "Syntax Error\n";
		return false;
	}

	return true;
}

bool checkCountWord(const std::vector<std::string> &lines, const std::string &word, const int &count)
{
	std::vector<int> indexW = containsExactWordInVector(lines, word);
	int countW = 0;
	std::vector<int> countWord;
	for (size_t i = 0; i < lines.size(); i++)
	{
		std::string word;
		for (size_t j = 0; j < indexW.size(); j++)
		{
			if ((int)i == indexW[j])
			{
				countW = 0;
				std::istringstream iss(lines[indexW[j]]);
				while (iss >> word)
				{
					countW++;
				}
				countWord.push_back(countW);
			}
		}
	}
	for (size_t i = 0; i < countWord.size(); i++)
	{
		if (countWord[i] != count)
		{
			std::cerr << "Error : Syntax error ( " << word <<")"   << std::endl;
			return (false);
		}
	}
	return (true);
}

int	openFile(std::string const & filePath)
{
	int			fd;
	DIR*		dir;

	dir = opendir(filePath.c_str());
	if (dir)
		throw std::runtime_error("File is a directory");	
	fd = open(filePath.c_str(), O_RDONLY);
	if (fd == -1)
	{
		perror("Open file");
		throw std::runtime_error("File error");
	}
	return fd;
}

size_t	getFileSize(std::string const & path)
{
	struct stat	sf;

	if (stat(path.c_str(), &sf) == -1)
		throw std::runtime_error("Stat function error");
	std::cout << "\e[0;35mFile size = " << sf.st_size  << "\e[0m" << std::endl;
	return sf.st_size;
}

DIR*	openDir(std::string const & dirPath)
{
	DIR	*dir;

	dir = opendir(dirPath.c_str());
	if (!dir)
		throw std::runtime_error("Opendir error");
	return dir;
}

bool checkCountWord2(const std::vector<std::string> &lines, const std::string &word, const int &count)
{
	std::vector<int> indexW = containsExactWordInVector(lines, word);
	int countW = 0;
	std::vector<int> countWord;
	for (size_t i = 0; i < lines.size(); i++)
	{
		std::string word;
		for (size_t j = 0; j < indexW.size(); j++)
		{
			if ((int)i == indexW[j])
			{
				countW = 0;
				std::istringstream iss(lines[indexW[j]]);
				while (iss >> word)
				{
					countW++;
				}
				countWord.push_back(countW);
			}
		}
	}
	for (size_t i = 0; i < countWord.size(); i++)
	{
		if (countWord[i] <= 1 || countWord[i] > count)
		{
			std::cerr << "Error : Syntax error ( " << word << ")" << std::endl;
			return (false);
		}
	}
	return (true);
}

bool checkNumberAllContent(std::vector<std::string> &file)
{
	if (!checkCountWord(file, "host", 2) || !checkCountWord(file, "size", 3) || !checkCountWord(file, "redirect", 3) || !checkCountWord(file, "location", 3) || !checkCountWord(file, "root", 2) || !checkCountWord(file, "redirect", 3) || !checkCountWord(file, "autoindex", 2))
		return (false);
	if (!checkCountWord2(file, "method", 4) || !checkCountWord2(file, "error_page", 6 )|| !checkCountWord2(file, "port", 4) || !checkCountWord(file, "cgi", 3))
	{
		return (false);
	}
	return (true);
}

bool checkDuplicate(stringVect file, std::string name, size_t &start, size_t &end)
{
	int count = 0;

	for (size_t i = start; i < end; ++i)
	{
		std::istringstream iss(file[i]);
		std::string word;
		while (iss >> word)
		{
			if (name == word)
				count++;
		}
	}
	if (count != 1)
	{
		std::cout << "Name of duplicate => " << name << std::endl;
		return (true);
	}
	return (false);
}

bool checkDuplicate3(stringVect file, std::string name, size_t &start, size_t &end)
{
	int count = 0;

	for (size_t i = start; i < end; ++i)
	{
		std::istringstream iss(file[i]);
		std::string word;
		while (iss >> word)
		{
			if (name == word)
				count++;
		}
	}
	if (count > 1)
	{
		std::cout << "Name of duplicate => " << name << std::endl;
		return (true);
	}
	return (false);
}

bool checkConfigDupli(stringVect file, size_t &start, size_t &end)
{
	if (checkDuplicate(file, "host", start, end) || checkDuplicate(file, "port", start, end) || checkDuplicate(file, "server_names", start, end) || checkDuplicate(file, "size", start, end))
	{
		std::cerr << "Error : duplicate setting find or not find setting" << std::endl;
		return (true);
	}
	return (false);
}

bool checkDuplicate2(stringVect file, std::string name, size_t &start, size_t &end)
{
	int count = 0;

	for (size_t i = start; i < end; ++i)
	{
		std::istringstream iss(file[i]);
		std::string word;
		while (iss >> word)
		{
			if (name == word)
				count++;
		}
	}
	if( count < 0 || count >1)
	{
		std::cout << "Name of duplicate => " << name << std::endl;
		return (true);
	}
	return (false);
}
