#pragma once

class Program
{
public:
	Program();
	~Program();

public:
	void Update();
	void Render();

private:
	class Rect* r1;
	Rect* r2;
};