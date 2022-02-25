#pragma once
#include <iostream>
#include <iomanip>
#include ".././MainLogic/IHitJudge.h"

namespace RhythmGame {
	class IFrame {
	public:
		MainLogic::IHitJudge& hitJudger;

		IFrame(MainLogic::IHitJudge& judger)
			: hitJudger(judger) {}
		virtual bool hasNext() const = 0;
		virtual bool hasNote() const = 0;
		virtual bool shouldUpdate() const = 0;
		virtual void renderNext() = 0;
		virtual void addNote(Data::Note note) = 0;
		virtual void shiftCurrentNotes() = 0;
		virtual void display() const = 0;
		virtual void initialize() = 0;
	};

	class FrameCLI :public IFrame {
	protected:
		char frame[10][9];
		size_t currentNote;
		double previousFrameTime;
		double refreshInterval;
	public:
		FrameCLI(MainLogic::IHitJudge& judger)
			:IFrame(judger), frame{ '\0' }
		{
			this->currentNote = 0U;
			this->previousFrameTime = 0.0;
			this->refreshInterval = 0.0;
			for (size_t i = 0; i < 10; i++) {
				for (size_t j = 0; j < 9; j++) {
					this->frame[i][j] = '-';
				}
			}
		}

		virtual void initialize() override {
			this->currentNote = 0U;
			this->refreshInterval = 1.0 / this->hitJudger.songTrack.speed;
			this->previousFrameTime = this->hitJudger.getCurrentTime();
			for (size_t i = 0; i < 10; i++) {
				for (size_t j = 0; j < 9; j++) {
					this->frame[i][j] = '-';
				}
			}
		}

		virtual bool hasNext() const override {
			return this->currentNote < this->hitJudger.songTrack.notes.size();
		}

		virtual bool hasNote() const override {
			for (int i = 9; i >= 0; i--)
			{
				for (int j = 0; j < 9; j++)
				{
					if (this->frame[i][j] == '#')
					{
						return true;
					}
				}
			}
			return false;
		}

		virtual bool shouldUpdate() const override {
			return this->hitJudger.getCurrentTime() - this->previousFrameTime >= this->refreshInterval;
		}

		virtual void shiftCurrentNotes() override {
			for (int i = 9; i >= 0; i--)
			{
				for (int j = 0; j < 9; j++)
				{
					if (this->frame[i][j] == '#')
					{
						this->frame[i][j] = '-';
						if (i != 9) {
							this->frame[i + 1][j] = '#';
						}
					}
				}
			}
		}

		virtual void renderNext() override {
			this->shiftCurrentNotes();
			while (this->hasNext() &&
				this->hitJudger.songTrack.notes[this->currentNote].time - this->hitJudger.songTrack.timeOfFalling() -
				this->hitJudger.config.calibration - this->hitJudger.config.visualCalibration - 
				this->hitJudger.getCurrentTime() < this->refreshInterval)
			{
				this->addNote(this->hitJudger.songTrack.notes[this->currentNote]);
				this->currentNote++;
			}
			this->previousFrameTime = this->hitJudger.getCurrentTime();
		}

		virtual void addNote(Data::Note note) override {
			this->frame[0][note.trackNumber()] = '#';
		}

		virtual void display() const override {
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					std::cout << std::setfill(' ') << std::setw(2) << this->frame[i][j];
				}
				std::cout << std::endl;
			}
			for (const char& character : this->hitJudger.config.keyCollection) {
				std::cout << std::setfill(' ') << std::setw(2) << character;
			}
			std::cout << std::endl;
		}

	};
	
}