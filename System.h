/** @file System.h */

#ifndef KQ_SYSTEM_H
#define KQ_SYSTEM_H

#include "Common.h"
#include <cstdint>

/** @brief Initializes all low level functions */

class System
{
private:
	static uint32_t ticks;

public:
	static void initialize(int argc, char** argv);
	static void quit();
	static void update();

	static uint32_t now();
	static void sleep(uint32_t duration);
};

#endif