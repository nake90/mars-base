#ifndef VERSION_H
#define VERSION_H

	//Date Version Types
	static const char INFO_DATE[] = "23";
	static const char INFO_MONTH[] = "06";
	static const char INFO_YEAR[] = "2010";
	static const char INFO_UBUNTU_VERSION_STYLE[] = "10.06";
	
	//Software Status
	static const char INFO_STATUS[] = "Alpha";
	static const char INFO_STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long INFO_MAJOR = 0;
	static const long INFO_MINOR = 2;
	static const long INFO_BUILD = 25;
	static const long INFO_REVISION = 90;
	
	//Miscellaneous Version Types
	static const long INFO_BUILDS_COUNT = 4254;
	#define INFO_RC_FILEVERSION 0,2,25,90
	#define INFO_RC_FILEVERSION_STRING "0, 2, 25, 90\0"
	static const char INFO_FULLVERSION_STRING[] = "0.2.25.90";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long INFO_BUILD_HISTORY = 18;
	

#endif //VERSION_H
