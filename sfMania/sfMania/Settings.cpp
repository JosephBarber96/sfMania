#include "Settings.h"
#include "GameManager.h"

eResolution Settings::_resolution;
float Settings::_windowX;
float Settings::_windowY;
float Settings::_fullscreen;

void Settings::SetWindowSize(eResolution res)
{
	_resolution = res;

	int x, y;
	switch (res)
	{
	case eResolution::x800y600:
		x = 800;
		y = 600;
		break;

	case eResolution::x1024y786:
		x = 1024;
		y = 768;
		break;

	case eResolution::x1280y720:
		x = 1280;
		y = 720;
		break;

	case eResolution::x1280y960:
		x = 1280;
		y = 960;
		break;

	case eResolution::x1366y768:
		x = 1366;
		y = 768;
		break;

	case eResolution::x1600y900:
		x = 1600;
		y = 900;
		break;

	case eResolution::x1920y1080:
		x = 1920;
		y = 1080;
		break;
	}
	_windowX = x;
	_windowY = y;

	GameManager::instance->ChangeWindowResolution(x, y);
}

void Settings::SetFullscreen(bool fullscreen)
{
	_fullscreen = fullscreen;
	GameManager::instance->SetFullscreen(fullscreen);
}

void Settings::SetDefaultOnStartup(eResolution res, int x, int y, bool fullscreen)
{
	_resolution = res;
	_windowX = x;
	_windowY = y;
	_fullscreen = fullscreen;
}