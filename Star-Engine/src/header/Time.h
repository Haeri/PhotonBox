#ifndef TIME_H
#define TIME_H

class Time {
public:
	static double deltaTime;
	static double time;
	static double timeScale;

	void setDeltaTime(double deltaTime);
	void setTime(double time);
	void setTimeScale(double timeScale);
	static long now();
};
#endif /* defined(TIME_H) */
