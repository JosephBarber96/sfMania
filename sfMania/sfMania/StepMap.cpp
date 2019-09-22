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