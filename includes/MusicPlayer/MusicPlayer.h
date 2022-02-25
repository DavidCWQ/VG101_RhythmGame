//
// Created by Binhao
//

#pragma once

#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <ctime>
#include <string>
#include "irrKlang.h"
using namespace irrklang;

namespace RhythmGame {
	namespace MusicPlayer {

		class MusicPlayer {
		private:
			ISoundEngine* pEngine;
			ISoundSource* pSource;
			ISound* pSound;
		public:
			MusicPlayer();
			MusicPlayer(const char fileName[]);
			MusicPlayer(std::string fileName);
			~MusicPlayer();
			ISoundEngine& engine() const;
			ISoundSource& source() const;
			ISound& sound() const;
			void setFile(const char fileName[]);
			void setFile(std::string fileName);
			void play(unsigned int position);
			void resume();
			double pause();
			double getCurrentTime() const;
			void clear();
		};
	}
}

#endif //MUSIC_PLAYER_H