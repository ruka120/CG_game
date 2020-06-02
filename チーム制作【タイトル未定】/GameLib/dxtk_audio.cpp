//******************************************************************************
//
//
//      DirectXTKAudio
//
//
//******************************************************************************

//------< インクルード >---------------------------------------------------------
#include "dxtk_audio.h"

namespace GameLib
{
    namespace audio
    {
        using DirectX::AUDIO_ENGINE_FLAGS;
        using DirectX::AudioEngine;
        using DirectX::SoundEffect;
        using DirectX::WaveBank;

        using DirectX::AudioEngine_Default;
        using DirectX::AudioEngine_Debug;

        //--------------------------------
        //  コンストラクタ
        //--------------------------------
        DXTKAudio::DXTKAudio()
        {
            // Create DirectXTK for Audio objects
            AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
            eflags = eflags | AudioEngine_Debug;
#endif
            audioEngine.reset(new AudioEngine(eflags));
            for (auto &p : musicVolume) p = 1.0f;
            for (auto &p : soundVolume) p = 1.0f;
        }

        //--------------------------------
        //  デストラクタ
        //--------------------------------
        DXTKAudio::~DXTKAudio()
        {
            audioEngine.reset();
        }

        //--------------------------------
        //  オーディオエンジンのリセット
        //--------------------------------
        bool DXTKAudio::reset()
        {
            return audioEngine->Reset();
        }

        //--------------------------------
        //  音楽読込
        //--------------------------------
        void DXTKAudio::musicLoad(int trackNo, const wchar_t* waveFileName, float volume)
        {
            if (musicInst[trackNo]) musicInst[trackNo].reset();
            music[trackNo].reset(new SoundEffect(audioEngine.get(), waveFileName));
            musicVolume[trackNo] = volume;
        }

        //--------------------------------
        //  音楽解放
        //--------------------------------
        void DXTKAudio::musicUnload(int trackNo)
        {
            if (musicInst[trackNo]) musicInst[trackNo].reset();
            music[trackNo].reset(nullptr);
        }

        //--------------------------------
        //  音楽再生
        //--------------------------------
        void DXTKAudio::musicPlay(int trackNo, bool isLoop)
        {
            if (!music[trackNo]) return;
            musicInst[trackNo] = music[trackNo]->CreateInstance();
            musicInst[trackNo]->Play(isLoop);
            musicInst[trackNo]->SetVolume(musicVolume[trackNo]);
        }

        //--------------------------------
        //  音楽停止
        //--------------------------------
        void DXTKAudio::musicStop(int trackNo)
        {
            if (!musicInst[trackNo]) return;
            musicInst[trackNo]->Stop();
        }

        //--------------------------------
        //  音楽一時停止
        //--------------------------------
        void DXTKAudio::musicPause(int trackNo)
        {
            if (!musicInst[trackNo]) return;
            musicInst[trackNo]->Pause();
        }

        //--------------------------------
        //  音楽再開
        //--------------------------------
        void DXTKAudio::musicResume(int trackNo)
        {
            if (!musicInst[trackNo]) return;
            musicInst[trackNo]->Resume();
        }

        //--------------------------------
        //  音楽ボリューム設定
        //--------------------------------
        void DXTKAudio::musicSetVolume(int trackNo, float volume)
        {
            if (!musicInst[trackNo]) return;
            musicInst[trackNo]->SetVolume(volume);
        }

        //--------------------------------
        //  効果音の読み込み（xwbファイル）
        //--------------------------------
        void DXTKAudio::soundLoad(const wchar_t* xwbFileName, float volume)
        {
            for (auto &p : soundInst) if (p) p.reset();
            waveBank.reset(new WaveBank(audioEngine.get(), xwbFileName));

            for (auto &p : soundVolume) p = volume;
        }

        //--------------------------------
        //  効果音の再生
        //--------------------------------
        void DXTKAudio::soundPlay(int trackNo)
        {
            soundInst[trackNo] = waveBank->CreateInstance(trackNo);
            if (soundInst[trackNo])
            {
                soundInst[trackNo]->SetVolume(soundVolume[trackNo]);
                soundInst[trackNo]->Play();
            }
        }

        //--------------------------------
        //  効果音のボリューム設定
        //--------------------------------
        void DXTKAudio::soundSetVolume(int trackNo, float vol)
        {
            soundVolume[trackNo] = vol;
        }
    }
}

//******************************************************************************