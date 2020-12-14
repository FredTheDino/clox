#pragma once

#include "object.h"
#include "chunk.h"

ObjFunction* compile(const char* source);
void markCompilerRoots();
