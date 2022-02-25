//
// Created by wenqi.
//
#include "IHitJudge.h"

namespace RhythmGame {
	namespace MainLogic {
		////Initialize objects with constructor
		IHitJudge::IHitJudge(const Data::Config& config, const Data::SongTrackData& track, const MusicPlayer::MusicPlayer& player)
			: config(config), songTrack(track), songPlayer(player)
		{
			keyHit = '\0';
			keyHitTime = 0;
			currentNote = 0;
			currentScore = 0;
			currentCombo = 0;
			additionalComboScore = 0;
			basicNoteScore = 0;
			comboNoteScore = 0;
			fullComboScore = 0;
			maxComboHits = 0;
			charmingHits = 0;
		}

		void IHitJudge::initialize() {
			size_t totalNotes = this->songTrack.notes.size();
			this->fullComboScore = totalNotes * (totalNotes + 1U) / 2U;
		}

		void IHitJudge::restart()
		{
			keyHit = '\0';
			keyHitTime = 0;
			currentNote = 0;
			currentScore = 0;
			currentCombo = 0;
			additionalComboScore = 0;
			basicNoteScore = 0;
			comboNoteScore = 0;
			maxComboHits = 0;
			charmingHits = 0;
		}

		////Destruct object with destructor
		IHitJudge::~IHitJudge() = default;

		EventType IHitJudge::keyJudge(char key) {
			if (key == ESC) {
				return EventType::GAME_PAUSE;
			}
			else {
				this->keyHitTime = songPlayer.getCurrentTime();
				if (this->keyHitTime - config.calibration <=
					songTrack.notes.back().time + MISS_HIT_BOUNDARY) {
					// Deal with the case that player misses too much notes.
					while (this->currentNote < songTrack.notes.size() &&
						this->keyHitTime - config.calibration > songTrack.notes[this->currentNote].time + MISS_HIT_BOUNDARY)
					{
						this->currentNote++;
					}
					// Prevent overflow caused by extra hits and early hits during the game play.
					if (this->currentNote < songTrack.notes.size() && this->keyHitTime - config.calibration >=
						songTrack.notes[this->currentNote].time - this->songTrack.timeOfFalling() * 0.4) {
						this->keyHit = key;
						// Start key judging and score calculating only if all previous conditions are met.
						if (this->IsKeyCorrect(this->currentNote)) {
							this->currentCombo++;
							this->additionalComboScore++;
							if (this->ScoreCalc() == 0) { 
								this->currentCombo = 0;
							}
						}
						else {
							this->currentCombo = 0;
							this->additionalComboScore *= 0.8;
							this->comboNoteScore += this->additionalComboScore;
						}
						if (this->maxComboHits < this->currentCombo) {
							this->maxComboHits = this->currentCombo;
						}
						if (this->currentNote < songTrack.notes.size()) {
							this->currentNote++;
						}
					}
				}
				return EventType::GAME_START;
			}
		}

		// Judge whether the key(s) you hit is(are) correct or not.
		bool IHitJudge::IsKeyCorrect(unsigned int i) { //"i" here represents currentNote.
			bool result = false;
			int doubleHit = 0;
			unsigned int lastHit = songTrack.notes.size() - 1U; // Record the total hit of notes.
			// Implement the judgement of double hit notes in the game.
			if (i != lastHit && songTrack.notes[this->currentNote + 1U].time - songTrack.notes[this->currentNote].time <= 1.0 / songTrack.speed)
				doubleHit = 1;
			else if (i != 0 && songTrack.notes[this->currentNote].time - songTrack.notes[this->currentNote - 1U].time <= 1.0 / songTrack.speed)
				doubleHit = -1;
			if (doubleHit != 0)
				result = (this->SingleKeyCompare(this->currentNote) || this->SingleKeyCompare(this->currentNote + doubleHit));
			else
				result = this->SingleKeyCompare(this->currentNote);
			return result;
		}

		// Judge whether the single key is correct or not.
		bool IHitJudge::SingleKeyCompare(unsigned int i) { //"i" here represents the key to compare.
			if (this->config.isFuzzyOn)
				return FuzzySingleKeyCompare(i);
			else
				return this->keyHit == config.keyCollection[songTrack.notes[i].trackNumber()];
		}

		// Judge whether the single key is almost correct or not.
		// This funciton can be used to improve game experience.
		bool IHitJudge::FuzzySingleKeyCompare(unsigned int i) { //"i" here represents the key to compare.
			int trackNumber = songTrack.notes[i].trackNumber();
			int leftIndex = std::max(0, trackNumber - 1);
			int rightIndex = std::min((int)(config.keyCollection.size() - 1), trackNumber + 1);
			return (this->keyHit == config.keyCollection[leftIndex] || 
				this->keyHit == config.keyCollection[trackNumber] || 
				this->keyHit == config.keyCollection[rightIndex]);
		}

		// Calculate current score if IsKeyCorrect == true.
		double IHitJudge::ScoreCalc() {
			double hitTimeError, tempNoteScore = 0;
			hitTimeError = abs(this->keyHitTime - config.calibration - songTrack.notes[this->currentNote].time);
			if (hitTimeError <= MISS_HIT_BOUNDARY) {
				tempNoteScore = (MISS_HIT_BOUNDARY - hitTimeError) / (MISS_HIT_BOUNDARY - CHARMING_HIT_BOUNDARY);
			}
			if (hitTimeError <= CHARMING_HIT_BOUNDARY) {
				this->charmingHits++;
			}
			if (tempNoteScore > 0) {
				// If the hitTimeError is within the range of CHARMING_HIT_BOUNDARY, you'll get full basicNoteScore.
				this->basicNoteScore += (0.5 + 0.5 * std::min(1.0, tempNoteScore));
				this->comboNoteScore += this->additionalComboScore;
				currentScore = (this->basicNoteScore / songTrack.notes.size()) * 0.8
					+ (this->comboNoteScore / this->fullComboScore) * 0.2;
			}
			else {
				this->additionalComboScore = (this->additionalComboScore - 1.0) * 0.8;
				this->comboNoteScore += this->additionalComboScore;
			}
			return tempNoteScore;
		}

		double IHitJudge::getScore() const {
			return this->currentScore * 100;
		}

		unsigned int IHitJudge::getCurrentCombo() const {
			return this->currentCombo;
		}

		unsigned int IHitJudge::getMaxComboHits() const {
			return this->maxComboHits;
		}

		unsigned int IHitJudge::getCharmingHits() const {
			return this->charmingHits;
		}

		double IHitJudge::getCurrentTime() const {
			return this->songPlayer.getCurrentTime();
		}
	}
}