#pragma once
#include <Windows.h>
#include <conio.h>
#include "IFrame.hpp"

namespace RhythmGame {
	class IUserInterface {
	public:
		virtual IFrame& getFrame() = 0;
		virtual void initialize() = 0;
		virtual void flush() const = 0;
		virtual void clear() const = 0;
		virtual void displayFrame() const = 0;
		virtual void onFrameUpdate() = 0;
		virtual EventType onUserKeyboardInput(char) = 0;
		virtual void promptMessage(std::string, int color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) = 0;
		virtual void displayEndingInterface() = 0;
	};

	class CmdInterface :public IUserInterface {
	protected:
		FrameCLI frame;

		void setCursor(short x = 0, short y = 0) const {
			HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
			COORD pos = { x, y };
			SetConsoleCursorPosition(hOut, pos);
		}

		void showScoreResult(unsigned int score) const {
			unsigned int interval = refreshTime / (score + 1);
			for (size_t i = 1; i <= score; i++)
			{
				std::cout << std::setfill(' ') << std::setw(3) << i;
				std::cout << "\b\b\b";
				Sleep(interval);
			}
		}

		void showPercentageScoreResult(double score) const {
			for (double i = 0.01; i <= score; i += 0.01)
			{
				std::cout << std::setfill(' ') << std::setw(6) << std::setprecision(2) << i << '%';
				std::cout << "\b\b\b\b\b\b\b";
			}
		}

		virtual void stringPrint(std::string str) const {
			for (const char& character : str)
			{
				std::cout << character;
				Sleep(40); //(8/6)
			}
			Sleep(2000);
			std::cout << std::endl;
		}

	public:
		static constexpr int refreshTime = 2400;
		static constexpr int resultWindowSize = 50;

		CmdInterface(MainLogic::IHitJudge& judger) :
			frame(judger) {}

		virtual void displayFrame() const override {
			this->flush();
			this->frame.display();
			std::cout << "Scores: " << std::setprecision(5) << this->frame.hitJudger.getScore() << '%' << std::endl;
			std::cout << "Current Combo: " << std::setfill('0') << std::setw(3) << 
				this->frame.hitJudger.getCurrentCombo() << std::endl;
			std::cout << "Charming Hits: " << std::setfill('0') << std::setw(3) << 
				this->frame.hitJudger.getCharmingHits() << std::endl;
		}

		virtual IFrame& getFrame() override {
			return this->frame;
		}

		virtual void initialize() override {
			this->clear();
			std::cout << std::fixed << std::showpoint;
			this->frame.initialize();
			this->frame.display();
		}

		virtual void onFrameUpdate() override {
			if (this->frame.shouldUpdate()) {
				this->frame.renderNext();
				this->displayFrame();
			}
		}

		virtual void flush() const override {
			this->setCursor(0,0);
		}

		virtual void clear() const override {
#ifdef WIN32
			system("cls");
#else
			system("clear");
#endif
		}

		virtual EventType onUserKeyboardInput(char key) override {
			return this->frame.hitJudger.keyJudge(key);
		}

		virtual void promptMessage(std::string message, int color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) override {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
			std::cout << message << std::endl;
		}

		virtual void displayEndingInterface() override {
			this->clear();
			std::cout << "Calculating your score ";
			Sleep(200);
			for (size_t i = 0; i < 3; i++)
			{
				for (size_t j = 0; j < 3; j++)
				{
					Sleep(500);
					std::cout << ". ";
				}
				Sleep(500);
				std::cout << "\b\b\b\b\b\b      ";
				std::cout << "\b\b\b\b\b\b";
			}
			this->clear();

			//Score showing animation
			for (size_t i = 0; i < resultWindowSize; i++)
			{
				std::cout << "_";
			}
			this->setCursor(0, 8);
			for (size_t i = 0; i < resultWindowSize; i++)
			{
				std::cout << "_";
			}

			this->setCursor(0, 2);
			this->setCursor(0, 2);
			std::cout << "\t" << "Song Name:           ";
			stringPrint(this->frame.hitJudger.config.songName);
			std::cout << "\t" << "Difficulty:          ";
			stringPrint(this->frame.hitJudger.config.difficulty);

			std::cout << "\t" << "Charming Hits:           /   " << this->frame.hitJudger.songTrack.notes.size();
			this->setCursor(25, 4);
			this->showScoreResult(this->frame.hitJudger.getCharmingHits());
			std::cout << std::endl;

			std::cout << "\t" << "MaxCombo Hits:           /   " << this->frame.hitJudger.songTrack.notes.size();
			this->setCursor(25, 5);
			this->showScoreResult(this->frame.hitJudger.getMaxComboHits());
			std::cout << std::endl << std::endl;

			std::cout << "\t" << "Your Score:     ";
			this->setCursor(35, 7);
			this->showPercentageScoreResult(this->frame.hitJudger.getScore());

			this->setCursor(0, 10);
		}

	};

}