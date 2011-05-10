/*
Version: 0.78
Release Date: 5/10/2011
Originally created by Jonathan Annas 3/11/09 and released under GPL v2

Description:
Example main.cpp to show usage of the code profiler.
*/

#include <iostream>
#include "codeprofiler.h"

using namespace std;

/* Static pointer created to Singleton Pattern. */
static CodeProfiler* cp	= CodeProfiler::Inst();

int main(){
	cout  << "\nCreating and destroying memory...";
	
	/* 	The Start() function takes a name which you should make unqique
		To identify your profile.  This will be displayed in the profile.txt
		and is used to index your profile. 
	*/
	cp->Start("Test1");
	
	for(int i=0;i<10000l;i++){
		int* j = new int[10000];
		delete [] j;

				
	}

	/*	Profiles can be started or stopped at any time, 
		including inside other profiled functions.
	*/
	cp->Start("Test 2");
	for(int i=0;i<10;i++){
		int* k = new int[10000];
		delete [] k;
	}
	
	cp->Stop("Test 2");
	cp->Stop("Test1");
	
	cout << "\nPease view the created ouput profile.txt to see results.\n";
	
	return 1;
}
