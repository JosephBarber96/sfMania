#pragma once

#include <string>
#include <vector>

class Measure;

class StepMap
{
public:
	StepMap();
	~StepMap();

	std::string m_chartType;
	std::string m_description;
	std::string m_difficultyName;
	int m_numericalDifficulty;

	void AddMeasure(Measure* measure);

	std::vector<Measure*> GetMeasures() const { return m_measures; }

private:
	std::vector<Measure*> m_measures;
};