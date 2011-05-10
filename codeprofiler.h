/*
Version: 0.78
Release Date: 5/10/2011

Created by Jonathan Annas 3/11/09 and released under GPL v2.

Description:
The code profiler is meant to be used to time functions or operations
at runtime with the best possible precision.  It uses high precision
timers for both Windows and Linux to provide time profiles for any
statement.

Notes: 
The output times should be considered relative to each other.  The
absolute values of the runtimes may be affected by the OS command
process timer which may add some constant time to your profile.
*/

#ifndef CODEPROFILER_H
#define CODEPROFILER_H

#include <time.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#ifdef WIN32
#include <windows.h>

struct CodeProfile{
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	char name[100];
};
#else
struct CodeProfile{
	struct timespec start;
	struct timespec end;
	char name[100];
};
#endif

using std::ofstream;
using std::vector;

class CodeProfiler{
	protected:
		CodeProfiler(const char* file_name);
		bool master;

	public:
		
		static	CodeProfiler* Inst();
		~CodeProfiler();

		void Start(const char* profile_name);
		void Stop(const char* profile_name);
		void Init();

	private:
		static CodeProfiler* cp_inst;
		ofstream logger;
		vector<CodeProfile*> profiles;
		float seconds;

		/* Stores the clock rate of the high precision 
		   timer provided by the OS.
		*/
#ifdef WIN32
		LARGE_INTEGER lp;
		LONGLONG freq;
		LONGLONG diff;
#else
		float freq;
		float delta_sec;
		float delta_micro;
#endif
};
#endif