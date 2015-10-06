// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// TODO: reference additional headers your program requires here
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

#ifndef M_PI
#define M_PI (3.141592654)
#endif

#ifndef TWOPI
#define TWOPI (2.0 * M_PI)
#endif

typedef enum {
	SINE,
	SQUARE,
	SAWD,
	SAWU,
	TRI
} wf_type;
