#pragma once
#include "UI.h"

class Hbox : public UI {
protected:
	float separation;
public:
	Hbox(std::string n, rect r = {}, float sep = 0.0f);
	~Hbox() override;
	void Update(std::vector<SDL_Event>& frameEvent) override;
	void Draw(SDL_Renderer* r, rect src = {}) override;
	void setSeparation(float s);
	float getSeparation();
};