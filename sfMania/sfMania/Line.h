#pragma once
class Line
{
public:
	Line(char c0, char c1, char c2, char c3);
	~Line();

	char GetChar(int i) const { return m_chars[i]; }

private:
	char m_chars[4];
};

