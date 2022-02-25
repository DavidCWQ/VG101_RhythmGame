#include "GI/IGame.h"

#ifdef OPEN_GL_GAME
#include "GL/freeglut.h"
#endif

using namespace RhythmGame;

#ifdef OPEN_GL_GAME // OpenGL part NOT implemented yet.
using Game = RhythmGame::GLRhythmGame;

void onKeyboardEvent(unsigned char key, int x, int y) {
	game.ui->onUserKeyboardInput(key);
}

void onDisplayUpdate() {
	game.ui->onFrameUpdate();
}
#else
using Game = RhythmGame::RhythmGameCLI;
#endif


int main(int argc, char* argv[]) {
#ifdef OPEN_GL_GAME
	glutInit(&argc, argv);
#endif
	Game game;

	if (argc >= 3 && strcmp(argv[1], "--config") == 0) {
		game.create(argv[2]);
	}
	else {
		game.create();
	}
#ifdef OPEN_GL_GAME
	glutDisplayFunc(onDisplayUpdate);
	glutKeyboardFunc(onKeyboardEvent);
#endif
	game.calibrate();
	while (game.lastEvent != EventType::GAME_EXIT) {
		game.run();
		if (game.lastEvent != EventType::GAME_RESTART) {
			game.over();
		}
	}
	return 0;
}
