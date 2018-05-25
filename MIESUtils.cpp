/*	MIESUtils.c -- illustrates Igor external functions.

	HR, 091021
		Updated for 64-bit compatibility.

	HR, 2013-02-08
		Updated for Xcode 4 compatibility. Changed to use XOPMain instead of main.
		As a result the XOP now requires Igor Pro 6.20 or later.
*/

#include "XOPStandardHeaders.h"			// Include ANSI headers, Mac headers, IgorXOP.h, XOP.h and XOPSupport.h
#include "MIESUtils.h"
#include <string>
#include <sstream>

/* Global Variables (none) */

#pragma pack(2)		// All structures passed to Igor are two-byte aligned.
struct MIESUtilsTestParams  {
	double result;
};
typedef struct MIESUtilsTestParams MIESUtilsTestParams;
#pragma pack()		// Reset structure alignment to default.

extern "C" int
MIESUtilsTest(MIESUtilsTestParams* p)
{
  int ret;

  p->result = 0;

  Handle h = FetchStrHandle("func");

  if(!h)
  {
    return USING_NULL_STRVAR;
  }

  std::string str(*h, WMGetHandleSize(h));

  FunctionInfo fi = {};
  if(ret = GetFunctionInfo(str.c_str(), &fi))
  {
    if(ret == NAME_TOO_LONG_FOR_OLD_XOP)
      XOPNotice("name too long" CR_STR);

    XOPNotice("error getting function info" CR_STR);
    return ret;
  }

  std::stringstream sstr;

  sstr << "name: " << fi.name << std::endl;
  sstr << "returnType: " << fi.returnType << std::endl;
  sstr << "numOptionalParameters: " << fi.numOptionalParameters << std::endl;
  sstr << "numRequiredParameters: " << fi.numRequiredParameters << std::endl;
  sstr << "totalNumParameters: " << fi.totalNumParameters<< std::endl;
  sstr << CR_CHAR;

  XOPNotice(sstr.str().c_str());

	return(0);					/* XFunc error code */
}

static XOPIORecResult
RegisterFunction()
{
	int funcIndex;

	funcIndex = (int)GetXOPItem(0);	/* which function invoked ? */
	switch (funcIndex) {
		case 0:						/* MIESUtilsAdd(p1, p2) */
			return (XOPIORecResult)MIESUtilsTest;
			break;
	}
	return 0;
}

/*	XOPEntry()

	This is the entry point from the host application to the XOP for all messages after the
	INIT message.
*/

extern "C" void
XOPEntry(void)
{
	XOPIORecResult result = 0;

	switch (GetXOPMessage()) {
		case FUNCADDRS:
			result = RegisterFunction();
			break;
	}
	SetXOPResult(result);
}

/*	XOPMain(ioRecHandle)

	This is the initial entry point at which the host application calls XOP.
	The message sent by the host must be INIT.

	XOPMain does any necessary initialization and then sets the XOPEntry field of the
	ioRecHandle to the address to be called for future messages.
*/

HOST_IMPORT int
XOPMain(IORecHandle ioRecHandle)			// The use of XOPMain rather than main means this XOP requires Igor Pro 6.20 or later
{
	XOPInit(ioRecHandle);					// Do standard XOP initialization
	SetXOPEntry(XOPEntry);					// Set entry point for future calls

	//if (igorVersion < 800) {
	//	SetXOPResult(OLD_IGOR);
	//	return EXIT_FAILURE;
	//}

	SetXOPResult(0);
	return EXIT_SUCCESS;
}
