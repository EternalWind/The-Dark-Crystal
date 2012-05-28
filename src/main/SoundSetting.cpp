#include "SoundSetting.h"

SoundSetting::SoundSetting()
{
    mSoundEffect = 1;
    mMusic = 2;
    mMainVolume = 4;

}

unsigned SoundSetting::getSoundEffect() const
{
    return mSoundEffect;
}

void SoundSetting::setSoundEffect(unsigned sound_effect)
{
    mSoundEffect = sound_effect;
}

void SoundSetting::setMusic(unsigned music)
{
    mMusic = music;
}

unsigned SoundSetting::getMusic() const
{
    return mMusic;
}

void SoundSetting::setMainVolume(unsigned main_volume)
{
    mMainVolume = main_volume;
}

unsigned SoundSetting::getMainVolume() const
{
    return mMainVolume;
}