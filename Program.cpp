#include "stdafx.h"
#include "Program.h"

Program::Program()
{
	r1 = new Rect();
	r2 = new Rect(100, 100, 200, 200);
}

Program::~Program()
{
	SAFE_DELETE(r2);
	SAFE_DELETE(r1);
}

void Program::Update()
{
	bool temp = r1->CheckIntersect(Mouse::Get()->GetPosition());

	if (temp && Mouse::Get()->Press(0))
	{
		r1->SetPosition(Mouse::Get()->GetPosition());
		r1->SetColor(RGB(0, 255, 255));
	}
	else if (r1->CheckIntersect(r2))
	{
		r1->SetColor(RGB(255, 0, 0));
		r2->SetColor(RGB(255, 0, 255));
	}
	else
	{
		r1->SetColor(RGB(127, 127, 127));
		r2->SetColor(RGB(127, 127, 127));
	}

	int keyCount = 0;

	if (Keyboard::Get()->Press('W'))
	{
		keyCount++;
		r1->Move('W');
	}
	if (Keyboard::Get()->Press('S'))
	{
		keyCount++;
		r1->Move('S');
	}
	if (Keyboard::Get()->Press('A'))
	{
		keyCount++;
		r1->Move('A');
	}
	if (Keyboard::Get()->Press('D'))
	{
		keyCount++;
		r1->Move('D');
	}

	if (keyCount == 2)
		r1->SetSpeed(5.0f / sqrtf(2.0f));
	else
		r1->SetSpeed(5.0f);

	r1->Update();
	r2->Update();
}

void Program::Render()
{
	r1->Render();
	r2->Render();
}
