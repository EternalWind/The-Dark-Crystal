#ifndef SCREEN_SETTING_H
#define SCREEN_SETTING_H

using namespace std;

class ScreenSetting
{
public:
    ScreenSetting();

    void setResolutionWidth(unsigned width);

    void setResolutionHeight(unsigned height);

    unsigned getResolutionWidth() const;

    unsigned getResolutionHeight() const;

private:
    struct Pair 
    {
        unsigned height;
        unsigned width;
    } mResolution;

};

#endif