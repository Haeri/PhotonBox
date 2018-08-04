#ifndef TIME_H
#define TIME_H

class Time
{
public:
	static double deltaTime;
	static float deltaTimef;
	static double time;
	static float timeScale;

	void setDeltaTime(double deltaTime);
	void setTime(double time);
	void setTimeScale(float timeScale);
	static double now();
};
#endif // TIME_H
