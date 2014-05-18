#ifndef __UNITY_H__
#define __UNITY_H__

#include "cocos2d.h"
#include "Transform.h"

#define TAG "2048Debug"
namespace unity
{
	const char* GetDefaultFontType();
	unsigned long GetTickCountX();
	void Log(const char* tag,const char* format,...);
}


#endif