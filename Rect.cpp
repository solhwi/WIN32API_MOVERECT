#include "stdafx.h"
#include "Rect.h"

Rect::Rect(long left, long top, long right, long bottom)
{
	rect = { left, top, right, bottom };
	width = static_cast<float>(right) - left;
	height = static_cast<float>(bottom) - top;
	speed = 5.0f;
	position = Vector2(left + width / 2, top + height / 2);
}

Rect::~Rect()
{
	DeleteObject(brush);
}

void Rect::Update()
{
	rect.left = static_cast<LONG>(position.x - width / 2);
	rect.right = static_cast<LONG>(position.x + width / 2);
	rect.top = static_cast<LONG>(position.y - width / 2);
	rect.bottom = static_cast<LONG>(position.y + width / 2);
}

void Rect::Render()
{
	APIFuncs::DrawRect(hdc, rect);

	FillRect(hdc, &rect, brush);

}

void Rect::Move(DWORD key)
{
	if (key == 'W')
	{
		position.y -= speed;
	}
	else if (key == 'S')
	{
		position.y += speed;
	}
	else if (key == 'A')
	{
		position.x -= speed;
	}
	else if (key == 'D')
	{
		position.x += speed;
	}
}

bool Rect::CheckIntersect(Rect * rect)
{
	if (rect == nullptr) return false;

	if (this->rect.right >= rect->rect.left
		&& this->rect.left <= rect->rect.right
		&& this->rect.top <= rect->rect.bottom
		&& this->rect.bottom >= rect->rect.top)
		return true;

	return false;
}

bool Rect::CheckIntersect(Vector2 pos)
{
	if (this->rect.left < pos.x
		&& this->rect.right > pos.x
		&& this->rect.top < pos.y
		&& this->rect.bottom > pos.y)
		return true;

	return false;
}

bool Rect::CheckIntersect(Vector3 pos)
{
	if (this->rect.left < pos.x
		&& this->rect.right > pos.x
		&& this->rect.top < pos.y
		&& this->rect.bottom > pos.y)
		return true;

	return false;
}