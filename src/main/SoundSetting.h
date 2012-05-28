#ifndef _SOUND_SETTING_H
#define _SOUND_SETTING_H

using namespace std;

class SoundSetting
{
public:
    SoundSetting();

    void setSoundEffect(unsigned sound_effect);

    unsigned getSoundEffect() const;

    void setMusic(unsigned music);

    unsigned getMusic() const;

    void setMainVolume(unsigned main_volume);

    unsigned getMainVolume() const;

private:
    unsigned mSoundEffect;
    unsigned mMusic;
    unsigned mMainVolume;
};

#endif