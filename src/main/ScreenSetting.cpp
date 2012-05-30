#include "ScreenSetting.h"

ScreenSetting::ScreenSetting()
{
    mResolution.width = 1024;
    mResolution.height = 768;
}

unsigned ScreenSetting::getResolutionWidth() const
{
    return mResolution.width;
}

unsigned ScreenSetting::getResolutionHeight() const
{
    return mResolution.height;
}

void ScreenSetting::setResolutionWidth(unsigned width)
{
    mResolution.width = width;
}

void ScreenSetting::setResolutionHeight(unsigned height)
{
    mResolution.height = height;
}