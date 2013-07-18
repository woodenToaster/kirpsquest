/** @file Main.cc */

#include "MainLoop.h"
#include <iostream>
#include "SDL.h"

/** Done unless I want a --help option at the command line */

int main(int argc, char** argv)
{
	std::cout << "Kirp's Quest " << KQ_VERSION << std::endl;
	MainLoop(argc, argv).run();
	return 0;
}