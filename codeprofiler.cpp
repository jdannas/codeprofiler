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

#include <string.h>
#include "codeprofiler.h"

#ifdef WIN32
	#define snprintf sprintf_s
#endif

CodeProfiler* CodeProfiler::cp_inst = NULL;

CodeProfiler::CodeProfiler(const char* file_name){
	logger.open(file_name);
	Init();
}

/** Singleton pattern to return master instance. */
CodeProfiler* CodeProfiler::Inst(){
	if(cp_inst == NULL){
		cp_inst				= new CodeProfiler("profile.txt");
		cp_inst->master		= true;
#ifdef WIN32
	QueryPerformanceFrequency(&lp);
	cp_inst->freq = lp.QuadPart;
#else		
	//TODO: Insert linux frequency test (always 1 nano second).
#endif
	}

	return cp_inst;
}

void CodeProfiler::Init(){
	master	= false;
}

/** Begin a profile uniquely identified by profile_name. */
void CodeProfiler::Start(const char* profile_name){
	if(master){
		CodeProfile* cp = new CodeProfile;
		snprintf(cp->name, sizeof(char)*100,profile_name);

#ifdef WIN32
		QueryPerformanceCounter(&cp->start);
#else
		clock_gettime(CLOCK_REALTIME, &(cp->start));
#endif
		profiles.push_back(cp);
	}else{
		Inst()->Start(profile_name);
	}
}

/** Find profile "profile_name" in list, caculate stop time, output and then remove it. */
void CodeProfiler::Stop(const char* profile_name){
	if(master){
		//Linear search through profiles to match profile_name.
		for(int i=0;i<(int)profiles.size();i++){			
			CodeProfile* cp = profiles.at(i);

			//Found. Determine profile end time.
			if(strcmp(cp->name,profile_name) == 0){			
#ifdef WIN32
				QueryPerformanceCounter(&cp->end);
				diff = (cp->end.QuadPart - cp->start.QuadPart);
				seconds = (float)diff / freq;
#else
				clock_gettime(CLOCK_REALTIME, &(cp->end));
				delta_sec	= cp->end.tv_sec - cp->start.tv_sec;
				delta_micro	= (cp->end.tv_nsec - cp->start.tv_nsec) / 1000.0f;
				seconds		= delta_sec + (delta_micro / 1000000.0f);		
#endif
				logger << "\n" << cp->name << ": Seconds(" << seconds << ")";
				logger.flush();
				profiles.erase(profiles.begin() + i);
				return;
			}
		}
	}else{
		Inst()->Start(profile_name);
	}
}

/** Default destructor. */
CodeProfiler::~CodeProfiler(){
	for(int i=0;i<(int)profiles.size();i++){
		delete profiles.at(i);
	}

	logger.close();
}
