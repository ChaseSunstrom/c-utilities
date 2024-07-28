#ifndef CUTIL_PCH_H
#define CUTIL_PCH_H

#include <assert.h>
#include <ctype.h>
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
#include "types.h"

#endif
