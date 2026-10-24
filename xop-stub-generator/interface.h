/// @file interface.h Available functions in the MIESUtils XOP

/// @brief Return the wave modification count from the XOPToolkit
///
/// Builtin into Igor Pro 8.
variable MU_WaveModCount(WAVE input);

/// @brief Returns the free disk space at path in bytes, returns nan for invalid path
///
/// @param[in] path filesystem or network location
variable MU_GetFreeDiskSpace(string path);

THREADSAFE variable MU_RunningInMainThread();

/// @brief Return the current UTC time in seconds since 1904-01-01 with sub-microsecond resolution
///
/// In contrast to DateTime the resolution is not limited by the system timer tick.
THREADSAFE variable MU_GetDateTimeUTC();

/// @brief Return a memory statistics value of the system or of the Igor Pro process
///
/// @param type 0: total physical memory, 1: free physical memory, 2: used physical memory [bytes],
///             3: memory load [%], 4: commit limit, 5: free commit [bytes],
///             6: working set, 7: peak working set, 8: private bytes of Igor Pro [bytes]
THREADSAFE variable MU_GetMemoryStatistics(variable type);
