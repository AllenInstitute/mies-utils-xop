#include "XOPStandardHeaders.h" // Include ANSI headers, Mac headers, IgorXOP.h, XOP.h and XOPSupport.h
#include "CustomExceptions.h"
#include "Helpers.h"
#include "MIESUtils.h"
#include "Windows.h"
#include "functions.h"
#include <cmath>
#include <cstdint>
#include <psapi.h>
#include <string>

// variable MU_GetFreeDiskSpace(string path);
extern "C" int MU_GetFreeDiskSpace(MU_GetFreeDiskSpaceParams *p)
{
  ULARGE_INTEGER TotalNumberOfFreeBytes = {};

  BEGIN_OUTER_CATCH
  if(GetDiskFreeSpaceExA(GetStringFromHandleWithDispose(p->path).c_str(),
                         nullptr, nullptr, &TotalNumberOfFreeBytes))
  {
    // note: works up to 8192 TB precisely
    p->result = (double) TotalNumberOfFreeBytes.QuadPart;
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

// variable MU_RunningInMainThread()
extern "C" int MU_RunningInMainThread(MU_RunningInMainThreadParams *p)
{
  p->result = RunningInMainThread();
  return 0;
}

// variable MU_GetDateTimeUTC()
extern "C" int MU_GetDateTimeUTC(MU_GetDateTimeUTCParams *p)
{
  // 100 ns intervals since 1601-01-01 UTC
  constexpr unsigned __int64 INTERVALS_PER_SECOND = 10000000;
  constexpr unsigned __int64 SECONDS_1601_TO_1904 = 9561628800;
  FILETIME fileTime;
  ULARGE_INTEGER intervals;

  GetSystemTimePreciseAsFileTime(&fileTime);
  intervals.LowPart  = fileTime.dwLowDateTime;
  intervals.HighPart = fileTime.dwHighDateTime;

  // convert seconds and fraction separately, as the number of intervals exceeds
  // the exact integer range of double
  const unsigned __int64 seconds   = intervals.QuadPart / INTERVALS_PER_SECOND;
  const unsigned __int64 remainder = intervals.QuadPart % INTERVALS_PER_SECOND;

  p->result = (double) (seconds - SECONDS_1601_TO_1904) +
              ((double) remainder / INTERVALS_PER_SECOND);

  return 0;
}

namespace
{
/// Values of MU_GetMemoryStatistics, see interface.h
enum class MemoryStatisticsType : std::uint8_t
{
  TOTAL_PHYSICAL = 0,
  FREE_PHYSICAL,
  USED_PHYSICAL,
  MEMORY_LOAD,
  COMMIT_LIMIT,
  FREE_COMMIT,
  WORKING_SET,
  PEAK_WORKING_SET,
  PRIVATE_BYTES
};

constexpr MemoryStatisticsType LAST_TYPE = MemoryStatisticsType::PRIVATE_BYTES;
} // namespace

// variable MU_GetMemoryStatistics(variable type)
extern "C" int MU_GetMemoryStatistics(MU_GetMemoryStatisticsParams *p)
{
  p->result = NAN;

  if(!(p->type >= 0 && p->type <= static_cast<int>(LAST_TYPE)) ||
     std::trunc(p->type) != p->type)
  {
    return INVALID_MEMORY_STATISTICS_TYPE;
  }

  const auto type =
      static_cast<MemoryStatisticsType>(static_cast<int>(p->type));

  if(type >= MemoryStatisticsType::WORKING_SET)
  {
    PROCESS_MEMORY_COUNTERS_EX counters;

    if(!GetProcessMemoryInfo(
           GetCurrentProcess(),
           reinterpret_cast<PROCESS_MEMORY_COUNTERS *>(&counters),
           sizeof(counters)))
    {
      return WindowsErrorToIgorError(GetLastError());
    }

    switch(type)
    {
    case MemoryStatisticsType::WORKING_SET:
      p->result = (double) counters.WorkingSetSize;
      break;
    case MemoryStatisticsType::PEAK_WORKING_SET:
      p->result = (double) counters.PeakWorkingSetSize;
      break;
    default:
      p->result = (double) counters.PrivateUsage;
      break;
    }

    return 0;
  }

  MEMORYSTATUSEX status;
  status.dwLength = sizeof(status);

  if(!GlobalMemoryStatusEx(&status))
  {
    return WindowsErrorToIgorError(GetLastError());
  }

  switch(type)
  {
  case MemoryStatisticsType::TOTAL_PHYSICAL:
    p->result = (double) status.ullTotalPhys;
    break;
  case MemoryStatisticsType::FREE_PHYSICAL:
    p->result = (double) status.ullAvailPhys;
    break;
  case MemoryStatisticsType::USED_PHYSICAL:
    p->result = (double) (status.ullTotalPhys - status.ullAvailPhys);
    break;
  case MemoryStatisticsType::MEMORY_LOAD:
    p->result = (double) status.dwMemoryLoad;
    break;
  case MemoryStatisticsType::COMMIT_LIMIT:
    p->result = (double) status.ullTotalPageFile;
    break;
  default:
    p->result = (double) status.ullAvailPageFile;
    break;
  }

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

#if XOP_TOOLKIT_VERSION >= 800
  if(igorVersion < 800)
#else
  if(igorVersion < 700)
#endif
  {
    SetXOPResult(OLD_IGOR);
    return EXIT_FAILURE;
  }

  SetXOPResult(0);
  return EXIT_SUCCESS;
}
