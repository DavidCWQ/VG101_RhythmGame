#include "IGame.h"
#include "../FileIO/Config.h"
#include "../FileIO/SongTrackData.h"

namespace RhythmGame {
    using Clock = std::chrono::steady_clock;

	IRhythmGame::IRhythmGame():
		config(),songTrack(),audioPlayer(),
        hitJudger(config,songTrack,audioPlayer), ui(nullptr)
	{
		this->lastEvent = EventType::NO_EVENT;
	}

    IRhythmGame::~IRhythmGame() {
        this->ui->clear();
        delete (this->ui);
    }

    void IRhythmGame::create(std::string configFileName) {
        this->config.configFile = configFileName;
		this->config.readFromFile();
		this->songTrack.readFromFile(config.songTrackFileName);
        this->hitJudger.initialize();
        this->ui->clear();
	}

	void IRhythmGame::calibrate() {
        double standardTime = 0.5034;
        double sumOfOffset = 0.0;
        unsigned int totalCalibrationTime = 0;
        this->ui->promptMessage("Press space as soon as you hear the chord.(synchronously)");
        this->ui->promptMessage("More times you hit, more accurate this calibration will be.");
        Sleep(3000);
        this->ui->promptMessage("Press nothing until the chord end to quit.");
        Sleep(3000);
        this->audioPlayer.setFile(Data::Config::calibrationFileName);
        while (!(this->audioPlayer.sound().isFinished())) {
            this->audioPlayer.play(0);
            while (!(this->audioPlayer.sound().isFinished())) {
                if (_kbhit() && _getch() == ' ') {
                    sumOfOffset += this->audioPlayer.pause();
                    totalCalibrationTime++;
                    break;
                }
            }
        }
        this->ui->clear();
        this->audioPlayer.engine().removeAllSoundSources();
        if (totalCalibrationTime >= 1) {
            this->config.calibration = std::min(sumOfOffset / totalCalibrationTime - standardTime, 0.2);
        }
        this->ui->promptMessage("Calibration: " + std::to_string(this->config.calibration));
        Sleep(1000);
        double refreshInterval = 1.0 / this->songTrack.speed;
        standardTime = 8.0 * refreshInterval;
        sumOfOffset = 0.0; totalCalibrationTime = 0U;
        this->ui->clear();
        this->ui->promptMessage("Press the corresponding key as soon as you see the \'#\' hits the SECOND LAST line.(synchronously)");
        this->ui->promptMessage("More times you hit, more accurate this calibration will be.");
        for (int i = 0; i <= 7; i++) {
            this->ui->promptMessage(" - - - - - - - - -");
        }
        this->ui->promptMessage(" - - - - - - - - - <== SECOND LAST line | Used for calibration only. ", FOREGROUND_INTENSITY | FOREGROUND_BLUE);
        this->ui->promptMessage(" - - - - - - - - - <== Bottom line | Last/Jugement line in the game. ", FOREGROUND_INTENSITY | FOREGROUND_RED);
        std::stringstream strs;
        for (const char& character : this->hitJudger.config.keyCollection) {
            strs << std::setfill(' ') << std::setw(2) << character;
        }
        strs << " <== Key Map";
        this->ui->promptMessage(strs.str(), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        this->ui->promptMessage("The Last line is the line to hit the note while gaming.");
        this->ui->promptMessage("Press nothing until the \'#\' fall through to quit.");
        Sleep(6000);
        this->ui->clear();
        bool hasNextRound = true;
        while (hasNextRound) {
            hasNextRound = false;
            this->ui->getFrame().initialize();
            this->ui->getFrame().addNote(Data::SongTrackData::calibrationNote);
            this->ui->promptMessage("Visual Calibration:\n\n\n");
            this->ui->getFrame().display();
            Clock::time_point start = Clock::now();
            Clock::time_point frameStart = Clock::now();
            while (this->ui->getFrame().hasNote()) {
                if (_kbhit() && _getch() == this->config.keyCollection[4]) {
                    sumOfOffset += std::chrono::duration_cast<std::chrono::duration<double>>(Clock::now() - start).count();
                    totalCalibrationTime++;
                    this->ui->flush();
                    hasNextRound = true;
                    break;
                }
                if (std::chrono::duration_cast<std::chrono::duration<double>>(Clock::now() - frameStart).count() >= refreshInterval) {
                    this->ui->getFrame().shiftCurrentNotes();
                    frameStart = Clock::now();
                    this->ui->flush();
                    this->ui->promptMessage("Visual Calibration:\n\n\n");
                    this->ui->getFrame().display();
                }
            }
        }
        if (totalCalibrationTime >= 1) {
            this->config.visualCalibration = std::min(sumOfOffset / totalCalibrationTime - standardTime, 0.2);
        }
        this->ui->clear();
        this->ui->promptMessage("Visual Calibration: " + std::to_string(this->config.visualCalibration));
        this->config.writeToFile();
        Sleep(1000);
	}

    void IRhythmGame::suspend() {
        this->audioPlayer.pause();
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        std::cout << "Game Stop..." << std::endl;
        std::cout << "Press ESC again to stop, or press Backspace to restart, or press another key to continue." << std::endl;
        switch (int ch = _getch()) {
            case MainLogic::IHitJudge::ESC:
                lastEvent = EventType::GAME_END;
                break;
            case MainLogic::IHitJudge::Backspace:
                lastEvent = EventType::GAME_RESTART;
                this->audioPlayer.engine().removeAllSoundSources();
                this->hitJudger.restart();
                break;
            default:
                this->resume();
                break;
        }
    }
    
    void IRhythmGame::resume() {
        this->ui->promptMessage("Game Resume after 3 seconds...");
        for (int i = 3; i > 0; --i) {
            this->ui->promptMessage(std::to_string(i) + "...");
            Sleep(1000);
        }
        lastEvent = EventType::GAME_RESUME;
        this->ui->clear();
        this->audioPlayer.resume();
    }

    void IRhythmGame::over() {
        this->audioPlayer.engine().removeAllSoundSources();
        this->audioPlayer.setFile(this->config.endingMusicFileName);
        this->audioPlayer.play(0);
        this->ui->displayEndingInterface();
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        this->ui->promptMessage("Press 'Backspace' to replay.");
        this->ui->promptMessage("Press another key to exit...");
        if (_getch() == MainLogic::IHitJudge::Backspace) {
            lastEvent = EventType::GAME_RESTART;
            this->audioPlayer.engine().removeAllSoundSources();
            this->hitJudger.restart();
        }
        else {
            lastEvent = EventType::GAME_EXIT;
        }
    }

    void IRhythmGame::run() {
        this->audioPlayer.setFile(this->config.songFileName);
        this->audioPlayer.play(0);
        this->ui->initialize();
        lastEvent = EventType::GAME_START;
        while (!(this->audioPlayer.sound().isFinished()) && lastEvent != EventType::GAME_END 
            && lastEvent != EventType::GAME_RESTART && lastEvent != EventType::GAME_EXIT) {
            if (_kbhit()){
                lastEvent = this->ui->onUserKeyboardInput(_getch());
            }

            if (lastEvent != EventType::GAME_PAUSE) {
                this->ui->onFrameUpdate();
            }
            else {
                this->suspend();
            }
        }
    }

    RhythmGameCLI::RhythmGameCLI() :IRhythmGame() {
        this->ui = new CmdInterface(this->hitJudger);
    }

#ifdef OPEN_GL_GAME
    GLRhythmGame::GLRhythmGame() : IRhythmGame() {
        this->ui = new GLInterface(this->hitJudger);
    }
#endif

}