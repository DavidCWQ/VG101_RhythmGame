#pragma once

#include <array>
#include <list>
#include "IFrame.hpp"
#include "IUserInterface.hpp"
#include "../MainLogic/IHitJudge.h"

#ifdef OPEN_GL_GAME
#include "GL/freeglut.h"
#endif

namespace RhythmGame {
#ifdef OPEN_GL_GAME
	class GLPoint {
	public:
		GLdouble x;
		GLdouble y;
	};

	class Block {
	public:
		std::array<GLPoint, 4> vertices; 

		void setTrack(unsigned int trackNumber) {
			vertices[0].x = -0.1 * (9 - trackNumber);
			vertices[1].x = vertices[0].x;
			vertices[2].x = vertices[1].x + 0.1;
			vertices[3].x = vertices[2].x;
			for (size_t i = 0; i < 4;i++) {
				vertices[i].y = (i % 3) ? (0.9) : (1.0);
			}
		}

		void fallToNext() {
			for (GLPoint point : vertices) {
				point.y -= 0.2;
			}
		}

		void draw() const {
			glBegin(GL_POLYGON);
			for (GLPoint point : vertices) {
				glVertex2d(point.x, point.y);
			}
			glEnd();
			glFlush();
		}

		bool isOnScreen() const {
			bool result = true;
			for (GLPoint point : vertices) {
				result = result && (point.y > 0);
			}
			return result;
		}
	};

	class GLFrame :public IFrame {
	protected:
		std::list<Block> frame;
		size_t currentNote;
		double previousFrameTime;
		double refreshInterval;
	public:
		GLFrame(MainLogic::IHitJudge& judger)
			:IFrame(judger), frame()
		{
			this->currentNote = 0U;
			this->previousFrameTime = 0.0;
			this->refreshInterval = 0.0;
		}

		virtual void initialize() override {
			this->currentNote = 0U;
			this->refreshInterval = 1.0 / this->hitJudger.songTrack.speed;
			this->previousFrameTime = this->hitJudger.getCurrentTime();
			this->frame.clear();
		}

		virtual bool hasNext() const override {
			return this->currentNote < this->hitJudger.songTrack.notes.size();
		}

		virtual bool hasNote() const override {
			return !(this->frame.empty());
		}

		virtual bool shouldUpdate() const override {
			return this->hitJudger.getCurrentTime() - this->previousFrameTime >= this->refreshInterval;
		}

		virtual void shiftCurrentNotes() override {
			for (Block& block : this->frame) {
				block.fallToNext();
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
			while (!(this->frame.begin()->isOnScreen())) {
				this->frame.pop_front();
			}
			this->previousFrameTime = this->hitJudger.getCurrentTime();
		}

		virtual void addNote(Data::Note note) override {
			Block noteBlock;
			noteBlock.setTrack(note.trackNumber());
			this->frame.push_back(noteBlock);
		}

		virtual void display() const override {
			glClear(GL_COLOR_BUFFER_BIT);
			for (Block block : this->frame) {
				block.draw();
			}
			glutSwapBuffers();
		}
	};

	class GLInterface :public IUserInterface {
	protected:
		GLFrame frame;

	public:
		//static constexpr int refreshTime = 2400;
		//static constexpr int resultWindowSize = 50;

		GLInterface(MainLogic::IHitJudge& judger) :
			frame(judger) {}

		virtual void displayFrame() const override {
			this->flush();
			this->frame.display();
		}

		virtual IFrame& getFrame() override {
			return this->frame;
		}

		virtual void initialize() override {
			glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
			glutInitWindowPosition(200, 150);
			glutInitWindowSize(900, 600);
			glutCreateWindow("Rhythm Game");
			this->clear();
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
			glFlush();
		}
		virtual void clear() const override {
			glFlush();
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		}

		virtual EventType onUserKeyboardInput(char key) override {
			return this->frame.hitJudger.keyJudge(key);
		}

		virtual void promptMessage(std::string message, int color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) override {
			// Not implemented yet
		}

		virtual void displayEndingInterface() override {
			this->clear();
			// Not implemented yet
		}
	};
#endif
}
