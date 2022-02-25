#pragma once

#include <string>
#include <fstream>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace RhythmGame {
	namespace Data {
		class Config {
		public:
			static constexpr char configFileDirectory[] = "Resources/Json/config.json";
			static constexpr char defaultKeySetup[] = "1234 7890";
			static constexpr char calibrationFileName[] = "Resources/Audio/Calibration.wav";

			std::string configFile; // Json file directory.
			std::string difficulty;
			std::string songFileName;
			std::string songTrackFileName;
			std::string endingMusicFileName;
			std::string songName;
            std::string keyCollection;
			double calibration;
			double visualCalibration;
			bool isFuzzyOn;

			Config();
			void readFromFile();
			void writeToFile() const;
		};

		void to_json(json& configJson, const Config& config);
		void from_json(const json& configJson, Config& config);
	}
}
