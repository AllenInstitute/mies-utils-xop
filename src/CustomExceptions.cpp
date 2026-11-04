#include "CustomExceptions.h"
#include <sstream>
#include <utility>

//--------------------------------------------------------------
// IgorException
//--------------------------------------------------------------

IgorException::IgorException() : ErrorCode(EXIT_FAILURE)
{
}

IgorException::IgorException(int errorCode) : ErrorCode(errorCode)
{
}

IgorException::IgorException(int errorCode, std::string errorMessage)
    : ErrorCode(errorCode), Message(std::move(errorMessage))
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

int HandleException(const std::exception &exception)
{
  /// No Igor error code associated with this.
  std::stringstream sstr;
  sstr << "BUG: Ecountered C++ exception: " << exception.what();
  XOPNotice(sstr.str().c_str());
  return CPP_EXCEPTION;
}
