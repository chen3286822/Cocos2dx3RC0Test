#ifndef __UNITY_H__
#define __UNITY_H__

#include "cocos2d.h"
#include "Transform.h"

#define TAG "2048Debug"
namespace unity
{
	const char* GetDefaultFontType();
	unsigned long GetTickCountX();
	void Log(const char* tag,const char* format,...);		//CCLog 可以替代，只不过无法自定义标签，但是尽量少用该函数，减少jni传递
}


#endif