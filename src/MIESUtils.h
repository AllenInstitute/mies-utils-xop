#pragma once

// clang-format off
#define OLD_IGOR 1 + FIRST_XOP_ERR
#define UNHANDLED_CPP_EXCEPTION 2 + FIRST_XOP_ERR
#define CPP_EXCEPTION 3 + FIRST_XOP_ERR
// clang-format on

// Defined return values
const int RESULT_ERROR = 0;

/* Prototypes */
HOST_IMPORT int XOPMain(IORecHandle ioRecHandle);
