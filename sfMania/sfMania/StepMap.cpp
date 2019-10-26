#include "StepMap.h"



StepMap::StepMap()
{
	m_measures = std::vector<Measure*>();
}


StepMap::~StepMap()
{
}


void StepMap::AddMeasure(Measure* measure)
{
	m_measures.push_back(measure);
}

Measure* StepMap::GetMeasure(int i) const
{
	if (i < 0 || i > m_measures.size() - 1)
		return nullptr;
	return m_measures[i];
}