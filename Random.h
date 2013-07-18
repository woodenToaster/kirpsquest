/** @file Random.h */

#ifndef KQ_RANDOM_H
#define KQ_RANDOM_H

#include "Common.h"

/** @brief Finished */

class Random
{
private:
	Random();

public:
	static void initialize();
	static void quit();

	static int get_number(unsigned int x);
	static int get_number(unsigned int x, unsigned int y);
};
#endif