#include <version.h>

extern "C" {
    
    const char * jags_version()
    {
	const char * version = "@JAGS_VERSION@";
	return version;
    }

}
