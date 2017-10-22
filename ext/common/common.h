#ifndef COMMON_H
#define COMMON_H

#include <string.h>
#include <stdio.h>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>

// Define colors for printing to terminal
static const char* COL_NORM    = "\x1B[0m";   // Normal
static const char* COL_FATAL   = "\x1B[31m";  // Red
static const char* COL_ERROR   = "\x1B[91m";  // Light Red
static const char* COL_INFO    = "\x1B[37m";  // White
static const char* COL_WARN    = "\x1B[33m";  // Yellow
static const char* COL_DEBUG   = "\x1B[94m";  // Light Blue
static const char* COL_VERBOSE = "\x1B[36m";  // Cyan
static const char* COL_LOGGER  = "\x1B[90m";  // Dark Grey
static const char* COL_TRACE   = "\x1B[95m";  // Light Magenta
static const char* COL_END     = "\033[0m";   // Color End

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

//#define logd(msg, arg...) printf("%s:%s(%d): " msg, __FILENAME__, __FUNCTION__, __LINE__, ##arg)
const char* log_timestamp();
#define logd(msg, arg...) do {printf("%s [d] %s:%s(%d): \x1B[94m" msg, log_timestamp(), __FILENAME__, __FUNCTION__, __LINE__, ##arg); printf("\033[0m\n");} while(0)

#define logw(msg, arg...) do {printf("%s [d] %s:%s(%d): \x1B[33m" msg, log_timestamp(), __FILENAME__, __FUNCTION__, __LINE__, ##arg); printf("\033[0m\n");} while(0)

#define loge(msg, arg...) do {printf("%s [d] %s:%s(%d): \x1B[91m" msg, log_timestamp(), __FILENAME__, __FUNCTION__, __LINE__, ##arg); printf("\033[0m\n");} while(0)

#include "buffer.h"

#endif // COMMON_H