#include "CustomExceptions.h"
#include <sstream>

//--------------------------------------------------------------
// IgorException
//--------------------------------------------------------------

IgorException::IgorException() : ErrorCode(EXIT_FAILURE)
{
}

IgorException::IgorException(int errorCode) : ErrorCode(errorCode)
{
}

IgorException::IgorException(int errorCode, const std::string &errorMessage)
    : ErrorCode(errorCode), Message(errorMessage)
{
}

const char *IgorException::what() const
{
  return Message.c_str();
}

int IgorException::HandleException() const
{
  return ErrorCode;
}

//--------------------------------------------------------------
// std::exception
//--------------------------------------------------------------

int HandleException(const std::exception e)
{
  /// No Igor error code associated with this.
  std::stringstream sstr;
  sstr << "BUG: Ecountered C++ exception: " << e.what();
  XOPNotice(sstr.str().c_str());
  return CPP_EXCEPTION;
}
