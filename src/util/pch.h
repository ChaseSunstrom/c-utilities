#ifndef CUTIL_PCH_H
#define CUTIL_PCH_H

#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __unix
#include <sys/mman.h>
#include <unistd.h>
#endif

#include "macros.h"
#include "system.h"
#include "types.h"

#endif
