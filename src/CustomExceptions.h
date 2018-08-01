#pragma once
#include "XOPStandardHeaders.h" // Include ANSI headers, Mac headers, IgorXOP.h, XOP.h and XOPSupport.h
#include <MIESUtils.h>
#include <Windows.h>
#include <sstream>
#include <exception>
#include <string>

class IgorException : public std::exception
{
public:
  const int ErrorCode;
  const std::string Message;

  /// Constructors
  // Mark default constructor as deprecated
  // - Allows use of default constructor when a custom error code hasn't been
  // implemented
  // - Compiler warning allows us to find usages later
  __declspec(deprecated("Using default error code.  You should replace this "
                        "with a custom error code")) IgorException();
  IgorException(int errorCode);
  IgorException(int errorCode, const std::string &errorMessage);

  const char *what() const;

  /// Displays the exception if required; gets the return code.
  int HandleException() const;
};

int HandleException(const std::exception e);

#define BEGIN_OUTER_CATCH                                                      \
  try                                                                          \
  {                                                                            \
    p->result = RESULT_ERROR;

#define END_OUTER_CATCH                                                        \
  return 0;                                                                    \
  }                                                                            \
  catch(const IgorException &e)                                                \
  {                                                                            \
    XOPNotice(e.Message.c_str());                                              \
    return e.HandleException();                                                \
  }                                                                            \
  catch(const std::exception &e)                                               \
  {                                                                            \
    return HandleException(e);                                                 \
  }                                                                            \
  catch(...)                                                                   \
  {                                                                            \
    /* Unhandled exception */                                                  \
    return UNHANDLED_CPP_EXCEPTION;                                            \
  }
