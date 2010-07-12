#ifndef VERSION_H
#define VERSION_H

	//Date Version Types
	static const char INFO_DATE[] = "12";
	static const char INFO_MONTH[] = "07";
	static const char INFO_YEAR[] = "2010";
	static const char INFO_UBUNTU_VERSION_STYLE[] = "10.07";
	
	//Software Status
	static const char INFO_STATUS[] = "Alpha";
	static const char INFO_STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long INFO_MAJOR = 0;
	static const long INFO_MINOR = 2;
	static const long INFO_BUILD = 94;
	static const long INFO_REVISION = 433;
	
	//Miscellaneous Version Types
	static const long INFO_BUILDS_COUNT = 4381;
	#define INFO_RC_FILEVERSION 0,2,94,433
	#define INFO_RC_FILEVERSION_STRING "0, 2, 94, 433\0"
	static const char INFO_FULLVERSION_STRING[] = "0.2.94.433";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long INFO_BUILD_HISTORY = 87;
	

#endif //VERSION_H
