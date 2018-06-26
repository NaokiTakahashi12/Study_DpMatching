
#include <cmath>

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>

#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "DP.hpp"

DP::DP() {
	path_name = "city_mcepdata";
	file_extention = "txt";

	directory_name_list.push_back("city011");
	directory_name_list.push_back("city012");
	directory_name_list.push_back("city021");
	directory_name_list.push_back("city022");
}

DP::DP(const DP &dp) {
}

DP::~DP() {
}

void DP::run() {
	auto start_point = std::chrono::high_resolution_clock::now();

	load();
	create_map();
	matching();
	minimum_search();

	auto end_point = std::chrono::high_resolution_clock::now();
	auto interval_time = std::chrono::duration<float, std::ratio<1, 1>>(end_point - start_point).count();

	std::cout << "Loss time is " << interval_time << " [sec]" << std::endl;
}

void DP::load() {
	std::cout << " --------- load ------------ " << std::endl;

	for(auto &dn : directory_name_list) {
		data_matrixs.push_back(std::vector<Eigen::MatrixXf>());

		std::string mather_filename = path_name + "/" + dn + "/" + dn;

		for(int i = 1; i <= 100; i++) {
			std::string filename;
			std::ostringstream filename_settings;

			filename_settings << std::setfill('0') << std::setw(3) << i;
			filename = mather_filename + "_" + filename_settings.str() + "." + file_extention;

			std::ifstream file(filename);

			if(file.fail()) {
				file.close();
				std::cout << "----------- " << filename << " -----------" << std::endl;
				throw std::runtime_error("Cant open file");

			}
			convert_matrix(file);

			file.close();

		}
	}
}

void DP::convert_matrix(std::ifstream &file) {
	std::string read_line;

	int count = 0,
		cols = 0,
		rows = 0;

	while(1) {
		file >> read_line;

		if(file.fail()) {
			break;
		}
		else if(count == 1) {
			speak_strings.push_back(read_line);
		}
		else if(count == 2) {
			data_matrixs.back().push_back(Eigen::MatrixXf(std::atoi(read_line.c_str()), 15));
		}
		else if(count >= 3) {
			if(cols >= 15) {
				rows ++;
				cols = 0;

			}
			data_matrixs.back().back()(rows, cols) = std::atof(read_line.c_str());

			if(cols < 15) {
				cols ++;
			}
		}

		count ++;
	}
}

void DP::create_map_matrix(const Eigen::MatrixXf &a, const Eigen::MatrixXf &b) {
	map_matrixs.push_back(Eigen::MatrixXf(a.rows(), b.rows()));

	const int cols = map_matrixs.back().cols(),
		  	  rows = map_matrixs.back().rows();

	for(int i = 0; i < cols; i++) {
		for(int j = 0; j < rows; j++) {
			float sum = 0;

			for(int k = 0; k < a.cols(); k++) {
				sum += pow(a(i,k) - b(j,k), 2);
			}
			map_matrixs.back()(j,i) = sqrt(sum);

		}
	}
}

void DP::create_map() {
	std::cout << " ----- create map ------- " << std::endl;
	
	for(auto &&template_directory_matrix : data_matrixs) {
		for(auto &&matching_directory_matrix : data_matrixs) {
			int k = 0;

			for(auto &&matching_matrix : matching_directory_matrix) {
				create_map_matrix(template_directory_matrix.at(k), matching_matrix);
				k++;

			}
		}
	}
	std::cout << " ----- Map is " << map_matrixs.size() << " ----" << std::endl;

}

void DP::matching() {
	std::cout << " -------- matching --------- " << std::endl;

	result_map = map_matrixs;

	for(auto &&matrix : result_map) {
		for(int i = 1; i < matrix.cols(); i++) {
			matrix(0, i) += matrix(0, i-1);
		}
		for(int j = 1; j < matrix.rows(); j++) {
			matrix(j, 0) += matrix(j-1, 0);
		}
	}

	for(auto &&matrix : result_map) {
		for(int i = 1; i < matrix.cols(); i++) {
			for(int j = 1; j < matrix.rows(); j++) {
				Eigen::Vector3f min_vector;

				min_vector << matrix(j-1, i), matrix(j, i-1), 2*matrix(j-1, i-1);
				matrix(j, i) += min_vector.minCoeff();

			}
		}
	}
}

void DP::minimum_search() {
	std::cout << " ----------- minimum search --------------- " << std::endl;

	int i = 0,
		j = 0;

	minimums.push_back(std::vector<float>());

	for(auto &&matrix : result_map) {
		const auto last_element = matrix(matrix.rows()-1, matrix.cols()-1) / (matrix.cols() + matrix.rows());

		if((i % 100) == 1 && i != 1) {
			minimums.push_back(std::vector<float>());
			j = 0;

		}

		minimums.back().push_back(last_element);

		j++;
		i++;
	}

	for(auto &&m : minimums) {
		std::sort(m.begin(), m.end());

		float corectt = 1;

		for(auto &&min : m) {
			std::cout << min << " ";
			if(min <= 0) {
				corectt -= 0.01;
			}
		}

		std::cout << "\n" << corectt << std::endl;

		//std::cout << "\n" << std::endl;
		std::cout << std::endl;
	}
}

void DP::write() {
	std::cout << " ----------- write --------------- " << std::endl;

	std::ofstream write_file("dp_output.txt");
	
	if(write_file.fail()) {
		throw std::runtime_error("Cant open file");
	}

	for(auto &&result_matrix : result_map) {
		write_file << result_matrix << "\n" << std::endl;
	}
}

