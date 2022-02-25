#pragma once

namespace RhythmGame {
	enum class EventType :int {
		NO_EVENT,
		GAME_START,
		GAME_RESUME,
		GAME_PAUSE,
		GAME_END,
		GAME_EXIT,
		GAME_RESTART
	};
}