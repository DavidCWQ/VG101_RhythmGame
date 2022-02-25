#include "Config.h"

namespace RhythmGame {
	namespace Data {

        constexpr char Config::configFileDirectory[];
        constexpr char Config::defaultKeySetup[];
        constexpr char Config::calibrationFileName[];

		Config::Config() :difficulty("EASY"), songFileName(), songTrackFileName(),
		    calibration(0.0), isFuzzyOn(false)
		{
			keyCollection = defaultKeySetup;
			visualCalibration = 0.0;
		}

		void to_json(json& configJson, const Config& config) {
			configJson = json{
				{"configFile",config.configFile},
				{"difficulty",config.difficulty},
				{"songFileName",config.songFileName},
				{"songTrackFileName",config.songTrackFileName},
				{"endingMusicFileName",config.endingMusicFileName},
				{"songName",config.songName},
				{"calibration",config.calibration},
				{"visualCalibration",config.visualCalibration},
				{"isFuzzyOn",config.isFuzzyOn},
				{"keyMap",config.keyCollection}
			};
		}

		void from_json(const json& configJson, Config& config) {
			configJson.at("difficulty").get_to(config.difficulty);
			configJson.at("songFileName").get_to(config.songFileName);
			configJson.at("songTrackFileName").get_to(config.songTrackFileName);
			configJson.at("endingMusicFileName").get_to(config.endingMusicFileName);
			configJson.at("songName").get_to(config.songName);
			configJson.at("calibration").get_to(config.calibration);
			configJson.at("isFuzzyOn").get_to(config.isFuzzyOn);
			configJson.at("keyMap").get_to(config.keyCollection);
			configJson.at("visualCalibration").get_to(config.visualCalibration);
			configJson.at("configFile").get_to(config.configFile);
		}

		void Config::readFromFile() {
			std::ifstream fileInput(this->configFile, std::ios::in);
			json configJson = json{};
			fileInput >> configJson; // Operator Overload.
			fileInput.close();
			configJson.get_to(*this); //to_json
		}

		void Config::writeToFile() const {
			json configJson;
			to_json(configJson, (*this));
			std::ofstream fileOutput(this->configFile, std::ios::out);
			fileOutput << configJson;
			fileOutput.close();
		}
	}
}
