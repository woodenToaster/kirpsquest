/** @file Random.cpp */

/** @brief Finished */

#include "Random.h"
#include <ctime>
#include <cstdlib>

Random::Random()
{
}

void Random::initialize()
{
	srand((int) time(NULL));
}

void Random::quit()
{
}

/**
* @brief Returns a random integer number in [0, x[ with a uniform distribution.
*
* This is equivalent to: Random::get_number(0, x).
*
* @param x the superior bound
* @return a random integer number in [0, x[
*/
int Random::get_number(unsigned int x)
{
	return (int) ((double) x * rand() / (RAND_MAX + 1.0));
}

/**
* @brief Returns a random integer number in [x, y[ with a uniform distribution.
* @param x the inferior bound
* @param y the superior bound
* @return a random integer number in [x, y[
*/
int Random::get_number(unsigned int x, unsigned int y)
{
	return x + get_number(y - x);
}

