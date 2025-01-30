#pragma once
#include <string>
class Window
{
public:
	virtual ~Window() = default;
	virtual void ToggleVisibility() = 0;
	virtual void Draw() = 0;
	bool isVisible = true;
	const std::string name = "";
	Window(const std::string& n) : name(n) {}
};

