#include "Unity.h"
#include "JNIFunc.h"

namespace unity
{
	const char* GetDefaultFontType()
	{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		return "fonts/American Typewriter.ttf";
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		return "fonts/American Typewriter.ttf";
#endif
	}

	unsigned long GetTickCountX()
	{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		return getTickCount();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		return GetTickCount();
#endif
	}

	void Log(const char* tag, const char* format,...)
	{
		char temp[250] = {0};
		va_list pArgs;
		va_start (pArgs, format);
		vsprintf(temp, format, pArgs);
		va_end(pArgs);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		debugLog(tag,temp);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
// 		wchar_t temp2[250] = {0};
// 		LPCWSTR LPformat = LPCWSTR("%s : %s");
// 		wsprintf(temp2, LPformat, tag, temp);
// 		OutputDebugString(temp2);
#endif
		

	}

	int GetDigits(int a)
	{
		int digits = 1;
		while (a >= 10)
		{
			a = a / 10;
			digits++;
		}
		return digits;
	}
}