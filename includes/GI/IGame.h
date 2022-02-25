#pragma once

#include <conio.h>
#include <chrono>
#include <sstream>
#include "GLUI.hpp"
#include "GameEvent.hpp"
#include "IUserInterface.hpp"
#include "../MainLogic/IHitJudge.h"

#ifdef min
#undef min
#endif

namespace RhythmGame {

	class IRhythmGame {
	public:
		EventType lastEvent;
		Data::Config config;
		Data::SongTrackData songTrack;
		MusicPlayer::MusicPlayer audioPlayer;
		MainLogic::IHitJudge hitJudger;
		IUserInterface* ui;

		IRhythmGame();
		~IRhythmGame();

		virtual void create(std::string configFileName = Data::Config::configFileDirectory);
		virtual void calibrate();
		virtual void suspend();
		virtual void resume();
		virtual void run();
		virtual void over();
	};

	class RhythmGameCLI :public IRhythmGame {
	public:
		RhythmGameCLI();
	};

	class GLRhythmGame :public IRhythmGame {
	public:
		GLRhythmGame();
	};
}


