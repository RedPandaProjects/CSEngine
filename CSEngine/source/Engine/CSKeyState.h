#pragma once
class CSKeyState
{
	BEAR_CLASS_WITHOUT_COPY(CSKeyState);
public:
	CSKeyState();
	~CSKeyState();
	void ClearState();
	void SetBinding(int key, BearStringConteniarAnsi Binding);
	BearStringConteniarAnsi GetBinding(int key);
	void Event(BearInput::Key Key, bool Down);
	bool IsDown(int key);
	void FunctionToC();
private:
	struct SKey
	{
		SKey() :Down(false) {};
		bool Down;
		BearStringConteniarAnsi Binding;
	};
	SKey m_Keys[256];
};
extern CSKeyState *GKeyState;