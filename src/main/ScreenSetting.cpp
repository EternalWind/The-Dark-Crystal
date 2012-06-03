#include "ScreenSetting.h"

ScreenSetting::ScreenSetting()
{
    mFullScreen = false;
}


bool ScreenSetting::getFullScreen() const
{
    return mFullScreen;
}

void ScreenSetting::setFullScreen(bool is_full_Scrren)
{
    mFullScreen = is_full_Scrren;
}