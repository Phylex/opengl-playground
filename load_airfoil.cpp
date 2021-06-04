#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstddef>

std::vector<std::string> split_string_at_delim(std::string string, std::string delim) {
	std::vector<std::string> tokens;
	size_t found = 0;
	size_t current_start_pos = 0;
	found = string.find(delim);
	while (found != std::string::npos) {
		if (found - current_start_pos > 1)
			tokens.push_back(string.substr(current_start_pos, found));
		current_start_pos = found + delim.length();
		found = string.find(delim, current_start_pos);
	}
	if (current_start_pos < string.length()) {
		tokens.push_back(string.substr(current_start_pos));
	}
	return tokens;
}

std::vector<float> load_airfoil(std::string airfoil_data_filepath) {
	std::fstream airfoil_file;
	std::string line;
	int line_number = 0;
	airfoil_file.open(airfoil_data_filepath, std::fstream::in);
	std::vector<float> coordinates;
	std::getline(airfoil_file, line);
	std::string airfoil_name = line;
	std::cout << "Loading airfoil: " << airfoil_name << std::endl;
	while(std::getline(airfoil_file, line)) {
		line_number ++;
		std::vector<std::string> tokens_from_line = split_string_at_delim(line, " ");
		if (tokens_from_line.size() < 2) {
			std::cerr << "Invalid input in "
				<< airfoil_data_filepath
				<< " at line: "
				<< line_number
				<< std::endl;
			exit(1);
		}
		for(std::string num_str: tokens_from_line) {
			size_t pos = 0;
			float coordinate_num = std::stof(num_str, &pos);
			if (pos == 0) {
				std::cerr << "Number could not be parsed in file: "
					<< airfoil_data_filepath
					<< " at line: "
					<< line_number
					<< std::endl;
				exit(1);
			}
			coordinates.push_back(coordinate_num);
		}
	}
	return coordinates;
}
