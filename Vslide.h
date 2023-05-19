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
-> 만약 하위 오브젝트들을 합한 크기가, 이 슬라이드 오브젝트의 size보다 클 경우
	=> (이 오브젝트.size.x - 그 차이) 길이의 grabber의 visible을 켠다.
	=> 또한 grabber가 밖으로 빠져나가지 않게 하면서 이벤트리스너로 드래그를 구현한다. * 이때 꼭 grabber도 업데이트를 해주자.
-> 아니라면
	=> 아무것도 할 필요 없음
*/