#pragma once
#include "UI.h"
#include "Button.h"

class Vslide : public UI {
private:
	Button* grabber;
	bool isDragging = false;
	float drag_amount;
	float separation;
	const float grabber_width = 10.0f;
	const float wheel_scale = 0.2f;
public:
	Vslide(std::string n, rect r = {}, float sep = 0.0f);
	void Update(std::vector<SDL_Event>& frameEvent) override;
	void Draw(SDL_Renderer* r, rect src = {}) override;

	void setSeparation(float s);
	float getSeparation();

	bool getIsDragging();
	void setIsDragging(bool b);
};

/*
-> ���� ���� ������Ʈ���� ���� ũ�Ⱑ, �� �����̵� ������Ʈ�� size���� Ŭ ���
	=> (�� ������Ʈ.size.x - �� ����) ������ grabber�� visible�� �Ҵ�.
	=> ���� grabber�� ������ ���������� �ʰ� �ϸ鼭 �̺�Ʈ�����ʷ� �巡�׸� �����Ѵ�. * �̶� �� grabber�� ������Ʈ�� ������.
-> �ƴ϶��
	=> �ƹ��͵� �� �ʿ� ����
*/