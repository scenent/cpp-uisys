#pragma once
#include "UI.h"

/*
_on_realigned_h
_on_realigned_v
*/

enum class HorizontalAlign {
	Left, Center, Right
};
enum class VerticalAlign {
	Top, Center, Bottom
};

class Label : public UI {
protected:
	std::wstring text;
	const char* font_path;
	int font_size;
	TTF_Font* font;
	HorizontalAlign H_Align = HorizontalAlign::Center;
	VerticalAlign V_Align = VerticalAlign::Center;
public:
	Label(std::string n, rect r = {}, std::wstring t = L"", const char* p = "", int ps = 16);
	void Update(std::vector<SDL_Event>& frameEvent) override;
	void Draw(SDL_Renderer* r, rect src = {}) override;

	void setText(std::wstring s);
	std::wstring getText();

	void setH_Align(HorizontalAlign i);
	void setV_Align(VerticalAlign i);
};