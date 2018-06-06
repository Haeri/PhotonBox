#include <fstream>
#include <string>
#include <iostream>
#include <regex>

int main(int argc,  char** argv)
{
	int err = 0;
	int version = 0;
	if(argc < 3)
	{
		std::cerr << "Too few arguments!\n";
		return 1;
	}

	std::string filePath(argv[1]);
	std::string macro(argv[2]);
    std::regex pattern("#define " + macro + " (\\d*)");
	std::smatch sm;
	std::fstream fs;
	
	fs.open (filePath, std::fstream::in | std::fstream::out );

	if(fs.good())
	{
		std::string all((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
		std::regex_search(all, sm, pattern);
	
		if(sm.size() >= 2){
			version = std::stoi(sm[1]);

			std::cout << "Incrementing Build Number from " << version;
			version++;
			std::cout << " to " << version << std::endl;
			
			all = std::regex_replace(all, pattern, "#define PHOTON_BOX_VERSION_BUILD " + std::to_string(version));
	
			fs.clear();
			fs.seekg(0, std::ios::beg);
			fs << all;
			fs.flush();
		}
		else
		{
			err = 1;
		}
		
		fs.close();
	}
	else
	{
		err = 1;
	}

	return err;
}