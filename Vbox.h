#pragma once
#include "UI.h"

class Vbox : public UI {
protected:
	float separation;
public:
	Vbox(std::string n, rect r = {}, float sep = 0.0f);
	~Vbox() override;
	void Update(std::vector<SDL_Event>& frameEvent) override;
	void Draw(SDL_Renderer* r, rect src = {}) override;
	void setSeparation(float s);
	float getSeparation();
};