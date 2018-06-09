#ifndef PROFILER_H
#define PROFILER_H

class Profiler
{
public:
	static const int WARMUP_SECONDS = 6;
	static void addFps(int fps);
	static int getMinFps() { return _minFPS; }
	static int getMaxFps() { return _maxFPS; }
	static int getAvgFps() { return (int)_avgFPS; }
	
	void reset();
private:
	static int _minFPS;
	static int _maxFPS;
	static float _avgFPS;
	static unsigned long int _allFrames;
	static unsigned long int _allSeconds;

	static void startProfiling();
};

#endif // PROFILER_H
