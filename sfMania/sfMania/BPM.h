#pragma once
class BPM
{
public:
	BPM();
	BPM(int _beat, float _bpm);
	~BPM();

	int Beat() const { return m_beat; }
	float Bpm() const { return m_bpm; }

private:
	int m_beat;
	float m_bpm;
};