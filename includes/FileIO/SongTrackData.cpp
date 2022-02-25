//
// Created by Binhao on 2020/6/26.
//

#include <iostream>
#include "SongTrackData.h"

namespace RhythmGame {
    namespace Data {

#ifdef POTENTIAL
        Sound::Sound() {
            this->wait = 0;
            this->duration = 0;
            this->pitch = 0;
            this->velocity = 0;
        }

        void to_json(json& soundJson, const Sound& sound) {
            soundJson = json{
                {"w",sound.wait},
                {"d",sound.duration},
                {"p",sound.pitch},
                {"v",sound.velocity}
            };
        }
        
        void from_json(const json& soundJson, Sound& sound) {
            soundJson["w"].get_to(sound.wait);
            soundJson["d"].get_to(sound.duration);
            sound.pitch = (int8_t)soundJson["p"].get<int>();
            sound.velocity = (int8_t)soundJson["v"].get<int>();
        }

        constexpr Note::Note() : id(0U), position(0.0F), size(1.30000007F), time(0.0) {}
#endif

        unsigned int Note::trackNumber() const {
            return floorf(2 * (this->position + 2.0F));
        }

        void to_json(json& noteJson, const Note& note) {
            noteJson = json{
                {"$id",std::to_string(note.id)},
                {"pos",note.position},
                {"size",note.size},
                {"time",note.time}
            };
#ifdef POTENTIAL
            for (size_t i = 0U; i < note.sounds.size(); i++) {
                noteJson["sounds"][i] = json{};
                to_json(noteJson["sounds"][i], note.sounds[i]);
            }
#endif
        }

        void from_json(const json& noteJson, Note& note) {
            note.id = stoi(noteJson.at("$id").get<std::string>());
            if (noteJson.count("pos") != 0) {
                noteJson.at("pos").get_to(note.position);
            }
            else {
                note.position = 0.0;
            }
            noteJson.at("size").get_to(note.size);
            noteJson.at("_time").get_to(note.time);
#ifdef POTENTIAL
            note.sounds.reserve(noteJson["sound"].size());
            for (const json& soundJson : noteJson["sounds"]) {
                note.sounds.push_back(soundJson.get<Sound>());
            }
#endif
        }

#ifdef POTENTIAL
        NoteReference::NoteReference() {
            this->referenceId = 0;
        }

        void to_json(json& noteRefJson, const NoteReference& noteRef) {
            noteRefJson = json{ {"$ref",noteRef.referenceId} };
        }

        void from_json(const json& noteRefJson, NoteReference& noteRef) {
            noteRefJson["$ref"].get_to(noteRef.referenceId);
        }
#endif
        constexpr Note SongTrackData::calibrationNote;

        SongTrackData::SongTrackData() :
            speed(10.0F), notes() {}

        void SongTrackData::readFromFile(const char fileName[]) {
            std::ifstream fileInput(fileName, std::ios::in);
            json songJson;
            fileInput >> (songJson);
            fileInput.close();
            songJson.get_to(*this); //from_json
        }

        void SongTrackData::readFromFile(std::string fileName) {
            this->readFromFile(fileName.c_str());
        }

        double SongTrackData::timeOfFalling() const {
            return 9 * (1.0 / this->speed);
        }

        void to_json(json& trackJson, const SongTrackData& track) {
            trackJson = json{
                {"speed",track.speed},
                {"notes",track.notes}
#ifdef POTENTIAL
               ,{"links",track.links}
#endif
            };
        }

        void from_json(const json& trackJson, SongTrackData& track) {
            trackJson.at("speed").get_to(track.speed);
            track.notes.reserve(SongTrackData::typicalNumberOfNotes);
            for (const json& noteJson : trackJson.at("notes")) {
                Note note = noteJson.get<Note>();
                if (note.position >= -2 && note.position <= 2) {
                    track.notes.push_back(note);
                }
            }
            track.notes.shrink_to_fit();
#ifdef POTENTIAL
            track.links.reserve(trackJson["links"].size());
            for (const json& notesJson : trackJson["links"]) {
                std::vector<NoteReference> notes;
                notes.reserve(notesJson["notes"].size());
                for (const json& noteRefJson : notesJson["notes"]) {
                    notes.push_back(noteRefJson.get<NoteReference>());
                }
                track.links.push_back(notes);
            }
#endif
        }
    }
}