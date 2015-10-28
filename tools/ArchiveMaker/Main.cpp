
#include <iostream>
#include <filesystem>
#include "../../src/IO/ArchiveMaker.h"
using namespace ln;
namespace sys = std::tr2::sys;

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cerr << "Invalid arguments." << std::endl;
		std::cerr << "ArchiveMaker <directory_path>" << std::endl;
		return 1;
	}

	try
	{
		PathNameA archiveDir(argv[1]);
		if (!archiveDir.IsDirectory()) {
			std::cerr << "Path is not directory." << std::endl;
		}

		// アーカイブファイル名
		PathNameA archivePath = archiveDir.ChangeExtension(".lna");

		printf("Root Dir : %s\n", archiveDir.c_str());
		printf("Output   : %s\n", archivePath.c_str());


		std::string pass;
		std::cout << "Password :";
		std::cin >> pass;

		ArchiveMaker archive;
		archive.Open(archivePath, pass.c_str());

		sys::path dir(archiveDir.c_str());
		sys::recursive_directory_iterator itr = sys::recursive_directory_iterator(dir);
		sys::recursive_directory_iterator end = sys::recursive_directory_iterator();
		for (; itr != end; ++itr)
		{
			PathNameA absPath = itr->path().string().c_str();
			PathNameA relPath = archiveDir.MakeRelative(absPath);

			//if (sys::is_regular_file(*itr)) {
			std::cout << "file :" << absPath.c_str() << std::endl;
			std::cout << "alias:" << relPath.c_str() << std::endl;
			//}
			archive.AddFile(PathName(absPath), relPath.c_str());
		}

		std::cout << "Succeeded.";
	}
	catch (std::exception& e)
	{
		std::cerr << e.what();
	}
	return 0;
}
