#include "XOPStandardHeaders.h" // Include ANSI headers, Mac headers, IgorXOP.h, XOP.h and XOPSupport.h
#include "MIESUtils.h"
#include <string>
#include "functions.h"
#include "Helpers.h"
#include "CustomExceptions.h"
#include "Windows.h"

// variable MU_GetFreeDiskSpace(string path);
extern "C" int MU_GetFreeDiskSpace(MU_GetFreeDiskSpaceParams *p)
{
  unsigned __int64 TotalNumberOfFreeBytes;

  BEGIN_OUTER_CATCH
  if(GetDiskFreeSpaceExA(GetStringFromHandleWithDispose(p->path).c_str(),
                         nullptr, nullptr,
                         (PULARGE_INTEGER) &TotalNumberOfFreeBytes))
  {
    // note: works up to 8192 TB precisely
    p->result = (double) TotalNumberOfFreeBytes;
  }
  else
  {
    p->result = NAN;
  }
  return 0;
  END_OUTER_CATCH
}

// variable MU_WaveModCount(WAVE input)
extern "C" int MU_WaveModCount(MU_WaveModCountParams *p)
{
  if(p->input == nullptr)
  {
    return NOWAV;
  }

  p->result = WaveModCount(p->input);

  return 0;
}

/*	XOPEntry()

  This is the entry point from the host application to the XOP for all messages
  after the INIT message.
*/

extern "C" void XOPEntry(void)
{
  XOPIORecResult result = 0;

  switch(GetXOPMessage())
  {
  case FUNCADDRS:
    result = RegisterFunction();
    break;
  }
  SetXOPResult(result);
}

/*	XOPMain(ioRecHandle)

  This is the initial entry point at which the host application calls XOP.
  The message sent by the host must be INIT.

  XOPMain does any necessary initialization and then sets the XOPEntry field of
  the ioRecHandle to the address to be called for future messages.
*/

HOST_IMPORT int
XOPMain(IORecHandle ioRecHandle) // The use of XOPMain rather than main means
                                 // this XOP requires Igor Pro 6.20 or later
{
  XOPInit(ioRecHandle);  // Do standard XOP initialization
  SetXOPEntry(XOPEntry); // Set entry point for future calls

  if(igorVersion < 620)
  {
    SetXOPResult(OLD_IGOR);
    return EXIT_FAILURE;
  }

  SetXOPResult(0);
  return EXIT_SUCCESS;
}
