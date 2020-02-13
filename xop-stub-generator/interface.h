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
