#pragma once
#include "XOPStandardHeaders.h" // Include ANSI headers, Mac headers, IgorXOP.h, XOP.h and XOPSupport.h
#include <string>

std::string GetStringFromHandle(const Handle &strHandle);
std::string GetStringFromHandleWithDispose(const Handle &strHandle);
