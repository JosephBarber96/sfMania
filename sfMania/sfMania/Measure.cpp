#include "Measure.h"

#include "Line.h"


Measure::Measure()
{
	m_lines = std::vector<Line*>();
}


Measure::~Measure()
{
}


void Measure::AddLine(std::string str)
{
	if (str == "") { return; }

	Line* line = new Line(str[0], str[1], str[2], str[3]);
	m_lines.push_back(line);
}