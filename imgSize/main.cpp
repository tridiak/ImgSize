//
//  main.cpp
//  imgSize
//
//  Created by tridiak on 20/04/22.
//


#include <iostream>
#include "ImgSize.hpp"
#include <vector>

using namespace std;

#define VERSION "0.1.3"

void BlabHelp() {
	std::cout << "imgSize";
	std::cout << "\tv0.1.3\n";
	std::cout << "--version\tWill output version and exit. Must be the first argument.\n";
	std::cout << "\n";
	std::cout << "\t(--simple) (--complex) file...\n";
	std::cout << "\n";
	std::cout << "\tReturns image size for passed image files. Important, the extension is ignored. The tool searches for magic numbers in the file.\n";
	std::cout << "\t 'simple' & 'complex' can be used in any order and multiple times. They simply toggle what is output before each file.\n";
	std::cout << "\t 'simple' outputs {XXX,XXX}({XXX,XXX}...)\n";
	std::cout << "\t 'complex' outputs <file path>:<type> {XXX,XXX}({XXX,XXX}...)\n";
	std::cout << "\t Some image files may return multiple sizes. JPEG, for example, has a thumbnail option and its size will be returned if found.\n";
	std::cout << "\n";
	std::cout << "\tv0.1.1\n";
	std::cout << "\t\tBug Fix: large jpeg files may return {-1,-1}\n";
	std::cout << "\tv0.1.2\n";
	std::cout << "\t\tBug Fix: some jpeg files may return no size result\n";
	std::cout << "\tv0.1.3\n";
	std::cout << "\t\tBug Fix: invalid jpeg files may not be recognised\n";
	std::cout << "\t\tAdded --version option\n";
}

extern int FileType(const char* file);

int ParseArg(const char* arg) {
	if (strncmp(arg, "--simple", strlen("--simple")) == 0) { return -2; }
	if (strncmp(arg, "--complex", strlen("--complex")) == 0) { return -3; }
	return FileType(arg);
}

int main(int argc, const char * argv[]) {
	if (argc == 1) {
		BlabHelp();
		return 0;
	}
	
	if (strncmp("--version", argv[1], strlen("--version")) == 0) {
		std::cout << VERSION << "\n";
		return 0;
	}
	
	ImgSize sz;
	std::vector<ImgSize> arysz;
	bool simpleOut = true;
	for (int t = 1; t < argc; t++) {
		try {
			int typ = ParseArg(argv[t]);
			if (typ == -2) {
				simpleOut = true;
				continue;
			}
			else if (typ == -3) {
				simpleOut = false;
				continue;
			}
			
			if (!simpleOut) { std::cout << argv[t] << ":"; }
			switch (typ) {
				case 0:
					sz = PngImageSize(argv[t]);
					if (!simpleOut) { std::cout << "PNG "; }
					PrintImgStruct(std::cout, sz);
					std::cout << "\n";
					
					break;
				case 1:
					sz = GifImageSize(argv[t]);
					if (!simpleOut) { std::cout << "GIF "; }
					PrintImgStruct(std::cout, sz);
					std::cout << "\n";
					break;
				case 2:
					arysz = JpgImageSize(argv[t]);
					if (!simpleOut) { std::cout << "JPG "; }
					for (auto itr = arysz.begin(); itr != arysz.end(); itr++) {
						PrintImgStruct(std::cout, *itr);
					}
					std::cout << "\n";
					break;
				default:
					if (!simpleOut) { std::cout << "Not a known image file\n"; }
					else { std::cout << "{-1,-1}\n"; }
			}
		}
		catch (std::ios::failure& ex) {
			if (!simpleOut) { std::cout << "Error IO: " << ex.what() << " : " << argv[t] << "\n"; }
			else { std::cout << "{-1,-1}\n"; }
		}
		catch (std::invalid_argument& ex2) {
			if (!simpleOut) { std::cout << "Error invalid argument: " << ex2.what() << " : " << argv[t]  << "\n"; }
			else { std::cout << "{-1,-1}\n"; }
		}
		catch (...) {
			if (!simpleOut) { std::cout << "Error invalid image file: "  << argv[t]  << "\n"; }
			else { std::cout << "{-1,-1}\n"; }
		}
	}
	return 0;
}
