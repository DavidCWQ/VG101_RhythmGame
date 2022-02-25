//
// Created by Binhao on 2020/6/26.
//

#pragma once
#ifndef VG101PROJECT_SONGTRACKDATA_H
#define VG101PROJECT_SONGTRACKDATA_H

//#define POTENTIAL

#include <fstream>
#include <vector>
#include <string>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace RhythmGame {
    namespace Data {

#ifdef POTENTIAL
        class Sound {
        public:
            double wait; //w wait
            double duration; //d positive
            int8_t pitch; //p 0-127, ignore negative value
            int8_t velocity; //v 0-127, ignore negative value
            Sound();
        };
        void to_json(json& soundJson, const Sound& sound);
        void from_json(const json& soundJson, Sound& sound);
#endif

        class Note {
        public:
            unsigned int id; //$id
            float position; //pos
            float size; //size
            double time; //_time

#ifdef POTENTIAL
            std::vector<Sound> sounds;
            constexpr Note();
#endif
            unsigned int trackNumber() const;
        };
        void to_json(json& noteJson, const Note& note);
        void from_json(const json& noteJson, Note& note);

#ifdef POTENTIAL
        class NoteReference {
        public:
            unsigned int referenceId; //$ref
            NoteReference();
        };
        void to_json(json& noteRefJson, const NoteReference& noteRef);
        void from_json(const json& noteRefJson, NoteReference& noteRef);
#endif

        class SongTrackData {
        public:
            static constexpr float version = 1.0F;
            static constexpr size_t typicalNumberOfNotes = 200;
            float speed; //speed
            std::vector<Note> notes; //notes
            static constexpr Note calibrationNote{ 0U,0.0F,1.3F,0.0 };
#ifdef POTENTIAL
            std::vector<std::vector<NoteReference>> links; //links
#endif
            SongTrackData();
            void readFromFile(const char fileName[]);
            void readFromFile(std::string fileName);
            virtual double timeOfFalling() const;
        };
        void to_json(json& trackJson, const SongTrackData& track);
        void from_json(const json& trackJson, SongTrackData& track);
    }
}

#endif //VG101PROJECT_SONGTRACKDATA_H
