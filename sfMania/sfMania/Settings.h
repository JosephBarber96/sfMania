#pragma once

enum eResolution : int
{
	res_first = 0,
	x800y600 = 1,
	x1024y786 = 2,
	x1280y720 = 3,
	x1280y960 = 4,
	x1366y768 = 5,
	x1600y900 = 6,
	x1920y1080 = 7,
	res_last = 8
};

class Settings
{
public:

	// Settings
	static eResolution Resolution() { return _resolution; }
	static int WindowX() { return _windowX; }
	static int WindowY() { return _windowY; }
	static float FallTime() { return 0.55f; }

	// Data
	static float PerfectWindow() { return 0.045f; }
	static float GreatWindow() { return 0.09f; }
	static float MissWindow() { return 0.18f; }

private:
	
	friend class SettingsScene;
	friend class GameManager;

	static eResolution _resolution;
	static float _windowX;
	static float _windowY;
	static float _fullscreen;

	static void SetWindowSize(eResolution res);
	static void SetFullscreen(bool fullscreen);

	static void SetDefaultOnStartup(eResolution res, int x, int y, bool fullscreen);
};

