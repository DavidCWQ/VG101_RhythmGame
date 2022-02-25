//
// Created by wenqi.
//

#ifndef COMPOSE_IHITJUDGE_H
#define COMPOSE_IHITJUDGE_H

#include <algorithm>
#include <iostream>
#include "../FileIO/Config.h"
#include "../FileIO/SongTrackData.h"
#include "../GI/GameEvent.hpp"
#include "../MusicPlayer/MusicPlayer.h"


#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

namespace RhythmGame {
    namespace MainLogic {

        class IHitJudge {
        private:

            char keyHit;
            double keyHitTime;
            double basicNoteScore;
            double comboNoteScore;
            unsigned int fullComboScore; // Calculated by first-order arithmetic sequence.
            double additionalComboScore; //Used to add to the comboNoteScore.
            size_t currentNote;
            double currentScore;
            unsigned int currentCombo;
            unsigned int maxComboHits;
            unsigned int charmingHits;

            bool IsKeyCorrect(unsigned int);

            bool SingleKeyCompare(unsigned int);

            bool FuzzySingleKeyCompare(unsigned int);

            double ScoreCalc();

        public:
            
            static constexpr double MISS_HIT_BOUNDARY = 0.4;
            static constexpr double CHARMING_HIT_BOUNDARY = 0.2;
            static constexpr int ESC = 27;
            static constexpr int Backspace = 8;

            const Data::Config& config;
            const Data::SongTrackData& songTrack;
            const MusicPlayer::MusicPlayer& songPlayer;

            IHitJudge(const Data::Config& config, const Data::SongTrackData& track, const MusicPlayer::MusicPlayer& player);

            ~IHitJudge();

            void initialize();

            void restart();

            EventType keyJudge(char key);

            double getScore() const;

            unsigned int getCurrentCombo() const;

            unsigned int getMaxComboHits() const;

            unsigned int getCharmingHits() const;

            double getCurrentTime() const;
        };
        
    }
}

#endif //COMPOSE_IHITJUDGE_H