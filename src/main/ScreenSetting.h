#ifndef SCREEN_SETTING_H
#define SCREEN_SETTING_H

using namespace std;

class ScreenSetting
{
public:
    ScreenSetting();

    void setFullScreen(bool is_full_screen);

    bool getFullScreen() const;

private:
      bool mFullScreen;
};

#endif