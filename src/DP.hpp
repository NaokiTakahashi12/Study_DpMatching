
#ifndef ___DPHPP___
#define ___DPHPP___

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <Eigen/Dense>
#include <Eigen/Geometry>

class DP {
	public :
		DP();
		DP(const DP &);
		virtual ~DP();

		void run();

	private :
		std::string path_name,
					file_extention;

		std::vector<std::string> directory_name_list;

		std::vector<std::string> load_filenames;
		std::vector<std::string> speak_strings;
		std::vector<std::vector<Eigen::MatrixXf>> data_matrixs;
		std::vector<Eigen::MatrixXf> map_matrixs;
		std::vector<Eigen::MatrixXf> result_map;
		std::vector<std::vector<float>> minimums;

		virtual void load();

		void convert_matrix(std::ifstream &file);
		void create_map_matrix(const Eigen::MatrixXf &, const Eigen::MatrixXf &);
		void create_map();
		void matching();
		void minimum_search();
		void write();
};

#endif //___DPHPP___

