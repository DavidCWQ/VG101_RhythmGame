//
// Created by Binhao
//

#include "MusicPlayer.h"

namespace RhythmGame {
	namespace MusicPlayer {

		MusicPlayer::MusicPlayer() {
			this->pEngine = createIrrKlangDevice();
			this->pSource = nullptr;
			this->pSound = nullptr;
		}

		MusicPlayer::MusicPlayer(const char fileName[]) {
			this->pEngine = createIrrKlangDevice();
			this->pSource = this->pEngine->addSoundSourceFromFile(fileName);
			this->pSound = this->pEngine->play2D(this->pSource, false, true, true);
		}

		MusicPlayer::MusicPlayer(std::string fileName) :MusicPlayer(fileName.c_str()) {}

		ISoundEngine& MusicPlayer::engine() const {
			return (*(this->pEngine));
		}

		ISoundSource& MusicPlayer::source() const {
			return (*(this->pSource));
		}

		ISound& MusicPlayer::sound() const {
			return (*(this->pSound));
		}

		void MusicPlayer::setFile(const char fileName[]) {
			this->pSource = this->pEngine->addSoundSourceFromFile(fileName);
			this->pSound = this->pEngine->play2D(this->pSource, false, true, true);
		}

		void MusicPlayer::setFile(std::string fileName) {
			this->setFile(fileName.c_str());
		}

		void MusicPlayer::play(unsigned int position) {
			this->pSound->setPlayPosition(position);
			this->pSound->setIsPaused(false);
		}

		void MusicPlayer::resume() {
			this->pSound->setIsPaused(false);
		}

		double MusicPlayer::pause() {
			this->pSound->setIsPaused(true);
			return (this->pSound->getPlayPosition()) / (1000.0);
		}

		double MusicPlayer::getCurrentTime() const {
			return (this->pSound->getPlayPosition()) / (1000.0);
		}

		void MusicPlayer::clear() {
			if(this->pSound)
				this->pSound->drop();
			if (this->pSource)
				this->pSource->drop();
			if (this->pEngine)
				this->pEngine->drop();
		}

		MusicPlayer::~MusicPlayer() {
			this->clear();
		}
	}
}
