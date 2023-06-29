#pragma once
#include <vector>
#include "mmlib.h"

using namespace std;

enum ANIM_MODES {
	MODE_ONCE, // play once
	MODE_FREEZE, // freeze on the last frame
	MODE_LOOP, // loop betwen start and end frames
	MODE_ILOOP // invert framerate when reaching the start/end frame
};

struct EmotePart {
	int seq;
	int mode;
	float framerate;
	float startFrame;
	float endFrame;

	EmotePart() {}

	EmotePart(int seq, int mode, float framerate, float startFrame, float endFrame) {
		this->seq = seq;
		this->mode = mode;
		this->framerate = framerate;
		this->startFrame = startFrame;
		this->endFrame = endFrame;

		if (framerate == 0) {
			framerate = 0.0000001f;
		}
		if (startFrame <= 0) {
			startFrame = 0.00001f;
		}
		if (endFrame >= 255) {
			endFrame = 254.9999f;
		}
	}
};

struct Emote {
	std::vector<EmotePart> parts;
	bool loop;

	Emote() {}

	Emote(std::vector<EmotePart> parts, bool loop) {
		this->parts = parts;
		this->loop = loop;
	}
};

void doEmote(CBasePlayer* plr, Emote emote, int partIdx);