/**
 * @file main.cpp
 */

#include <iostream>
#include <string>

#include "ctex.hpp"
#include "detector.hpp"
#define __glogger_implementation__
#include "glogger.hpp"

int main(int argc, char* argv[])
{
	bool interactive = false;
	if (argc == 2 && !strcmp(argv[1], "-i")) {
		interactive = true;
	}

	if (!interactive && argc < 3) {
		std::cout << "Usage:"
			<< "ctex.exe <in_file.c> <out_file.c>" << std::endl;
#ifdef _WIN32
		system("pause");
#endif
		exit(1);
	}
    
    // read from cmd or config file
    GLogger::instance().set_output_mode(GLogger::Output::Both);
    GLogger::instance().set_min_level(GLogger::Output::Console, GLogger::Level::Info);
    GLogger::instance().set_min_level(GLogger::Output::File, GLogger::Level::Trace);
    GLogger::instance().set_log_filename("ctex.log");
    
    LexemeLibrary::add_lexeme("fsign", LexemeLibrary::function, 1);
    std::shared_ptr<CTex> ctex = std::make_shared<CTex>(CTex::default_regex());
    Detector detector(ctex);
    
	if (interactive)
	{
		std::cout << "> Welcome to interactive CTex.\n Type `exit` to exit." << std::endl;
		std::string formula;
		while (true) {
			std::cout << "> type one-line c formula: " << std::endl;
			std::cout << "> ";
			std::getline(std::cin, formula);
			if (!formula.compare("exit"))
				break;
			std::cout << "> latex result:" << std::endl;
			std::cout << ctex->translate(formula) << std::endl;
			std::cout << std::endl;
		}
		std::cout << "> Done!" << std::endl;
	}
	else
	{
		std::ifstream in_file(argv[1]);
		std::ofstream out_file(argv[2]);

		std::cout << "Translating..." << std::endl;
		if (in_file.good() && out_file.good())  {
			detector.perform(in_file, out_file);
		}
		else {
			std::cout << "Bad file!" << std::endl;
		}
		std::cout << "Done!" << std::endl;
	}

#ifdef _WIN32
	system("pause");
#endif
	return 0;
}
