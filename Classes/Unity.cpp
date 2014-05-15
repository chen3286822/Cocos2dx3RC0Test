#include "Unity.h"

namespace unity
{
	const char* GetDefaultFontType()
	{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		return "DroidSans";
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		return "Arial";
#endif
	}
}