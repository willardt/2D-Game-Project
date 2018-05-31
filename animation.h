

#ifndef ANIMATION_H
#define ANIMATION_H

class Animation {
private:
public:
	int maxFrame;
	int runFrame;
	int castFrame;
	int specialFrame;
	int currentFrame;

	// Amount of sprites for each direction, ex: minUp = 1, maxUp = 2; Two sprites for direction 'UP'
	int minUp;
	int maxUp;
	int minDown;
	int maxDown;
	int minRight;
	int maxRight;
	int minLeft;
	int maxLeft;
};
#endif