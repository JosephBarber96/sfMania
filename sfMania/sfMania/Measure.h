#pragma once

#include <array>
#include <vector>
#include <string>

class Line;

class Measure
{
public:
	Measure();
	~Measure();

	void AddLine(std::string line);

	int LineCount() const { return m_lines.size(); }
	std::vector<Line*> GetLines() const { return m_lines; }

private:
	std::vector<Line*> m_lines;
};

