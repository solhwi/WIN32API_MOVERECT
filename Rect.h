#pragma once

class Rect
{
public:
	Rect() : Rect(0, 0, 100, 100) {}
	Rect(long left, long top, long right, long bottom);
	~Rect();

	void Update();
	void Render();

	void Move(DWORD key);

	bool CheckIntersect(Rect* rect);
	bool CheckIntersect(Vector2 pos);
	bool CheckIntersect(Vector3 pos);

	void SetPosition(Vector2 pos) { position = pos; }
	void SetPosition(Vector3 pos) { position = Vector2(pos.x, pos.y); }

	float GetSpeed() { return speed; }
	void SetSpeed(float speed) { this->speed = speed; }

	void SetColor(COLORREF color) { brush = CreateSolidBrush(color); }

private:
	RECT rect = { 0 };
	float width = 0.0f;
	float height = 0.0f;

	float speed = 0;

	Vector2 position = { 0.0f, 0.0f };

	HBRUSH brush = CreateSolidBrush(RGB(127, 127, 127));

};