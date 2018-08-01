#include "Helpers.h"

std::string GetStringFromHandle(const Handle &strHandle)
{
  // Check for special case of null handle.
  if(strHandle == nullptr)
    return "";
  size_t strLen = GetHandleSize(strHandle);
  return std::string(*strHandle, strLen);
}

std::string GetStringFromHandleWithDispose(const Handle &strHandle)
{
  const auto str = GetStringFromHandle(strHandle);
  DisposeHandle(strHandle);

  return str;
}
