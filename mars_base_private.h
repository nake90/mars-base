#ifndef VERSION_H
#define VERSION_H

	//Date Version Types
	static const char INFO_DATE[] = "23";
	static const char INFO_MONTH[] = "09";
	static const char INFO_YEAR[] = "2010";
	static const char INFO_UBUNTU_VERSION_STYLE[] = "10.09";
	
	//Software Status
	static const char INFO_STATUS[] = "Alpha";
	static const char INFO_STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long INFO_MAJOR = 0;
	static const long INFO_MINOR = 11;
	static const long INFO_BUILD = 829;
	static const long INFO_REVISION = 4659;
	
	//Miscellaneous Version Types
	static const long INFO_BUILDS_COUNT = 5777;
	#define INFO_RC_FILEVERSION 0,11,829,4659
	#define INFO_RC_FILEVERSION_STRING "0, 11, 829, 4659\0"
	static const char INFO_FULLVERSION_STRING[] = "0.11.829.4659";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long INFO_BUILD_HISTORY = 22;
	

#endif //VERSION_H
