#pragma once
#include "UI.h"

/*
_on_button_down
_on_button_up
*/

enum ButtonState {
	Normal, Hover, Down
};

class Button : public UI {
protected:
	ButtonState buttonState;
	bool checkInRect = true;
	SDL_Color buttonColor = {255, 255, 255, 255};
	rect clipped_rect = {};
public:
	Button(std::string n, rect r = {});
	~Button();
	void Update(std::vector<SDL_Event>& frameEvent) override;
	void Draw(SDL_Renderer* r, rect src = {}) override;
	void DrawOnceIgnore(SDL_Renderer* r);
	int getButtonState();
	void setCheckInRect(bool b = true);

	void setButtonColor(const SDL_Color& c);
	rect getClippedRect();
	SDL_Color getButtonColor();
};