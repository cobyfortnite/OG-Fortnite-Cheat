#pragma once
#include <Windows.h>

// ALL POTENTIAL DETECTION VECTORS ARE MARKED WITH A COMMENT!
// IF YOU WANT TO AVOID DETECTION, YOU SHOULD DISABLE THEM!

////////////////////////////////////////////////////////////////
// Settings for different types of injection methods
// 
// Windows Injection (i.e. LoadLibrary, CreateRemoteThread, etc)
/*
* Enable USING_SEH (recommended)
* Disable LOAD_D3DCOMPILER_47 (recommended)
* 
* Properties -> C/C++ -> Code Generation -> Enable C++ Exceptions -> Yes (/EHsc)				(recommended)
* Properties -> C/C++ -> Code Generation -> Security Check -> Enable Security Check (/GS)		(recommended)
* Properties -> C/C++ -> Code Generation -> Runtime Library -> Multi-threaded (/MT)				(recommended)
*/
// 
// 
// 
// Manual Mapping (i.e. VirtualAllocEx, WriteProcessMemory, ZwAllocateVirtualMemory, MmCopyVirtualMemory, etc)
/*
* Disable USING_SEH (REQUIRED)
* Disable INIT_THREAD (recommended)
* Disable UNLOAD_THREAD (REQUIRED)
* Enable LOAD_D3DCOMPILER_47 (REQUIRED if you don't map dependencies)
* 
* Properties -> C/C++ -> Code Generation -> Enable C++ Exceptions -> No						(REQUIRED)
* Properties -> C/C++ -> Code Generation -> Security Check -> Disable Security Check (/GS-)	(REQUIRED)
* Properties -> C/C++ -> Code Generation -> Runtime Library -> Multi-threaded (/MT)			(REQUIRED unless your injector handles and maps ApiSet dependencies)
* Properties -> C/C++ -> Command Line -> Additional Options -> /Zc:threadSafeInit- 			(REQUIRED)
*/
////////////////////////////////////////////////////////////////





#define LOG_NONE 0					// No logs
#define LOG_ERROR 1					// Only error logs
#define LOG_OFFSET 2				// Error logs and offset logs
#define LOG_INFO 3					// Error logs, offset logs and general info logs
#define LOG_ALL 3					// Log everything
#define LOG_LEVEL_MAX 4



// Level of DEBUG_LOG to display
#ifdef _DEBUG
	#define LOG_LEVEL		LOG_ALL
#else
	#define LOG_LEVEL		LOG_NONE// No logs in release mode by default
#endif // _DEBUG

// Only enable this if you are sure your injector supports SEH
#define USING_SEH			TRUE	// Enables the use of SEH (Structured Exception Handler) for verifying if a pointer is valid
#define SEASON_20_PLUS		FALSE	// REQUIRED ON SEASON 20 AND FORWARD! Enables the use of doubles instead of floats on structures like FVector, FRotator etc. Changes a few other structures too

#define NAME_DUMP			FALSE	// Dumps all FNames to the log
#define OBJECT_DUMP			FALSE	// Dumps all UObjects to the log

// RISKY SETTINGS! ADVISED TO KEEP THEM DISABLED!
// Creating a thread can add another major detection vector depending on how you inject, and how the Anti-Cheat scans for suspicous threads
#define INIT_THREAD			TRUE	// Starts the cheat intialization in a new thread
#define UNLOAD_THREAD		TRUE	// Allows the cheat to be unloaded (will only work if DLL is injected as valid module)
#define LOAD_D3DCOMPILER_47 FALSE	// Loads D3DCompiler_47.dll with LoadLibraryA (required for manual mapping, since most mappers don't map dependencies)
// Both of these settings will require a thread to be created, and will be a major detection vector (most of the time)!

inline HMODULE CurrentModule = nullptr;// The current module handle



// Compile-Time Asserts
static_assert(LOG_LEVEL < LOG_LEVEL_MAX && LOG_LEVEL >= LOG_NONE, "Invalid log level");