#pragma once
#include "UI.h"
#include "SDL/SDL_ttf.h"
/*
_on_text_changed
*/



class Textedit : public UI {
private:
	bool Focused = false;
	bool Shifted = false;
	std::vector<std::wstring> charList;
	std::vector<int> charEndPosList;
	TTF_Font* font;
	int font_size;
	int cursor_index = 0;
public:
	Textedit(std::string s, rect r = {}, const char* fp = "", int ps = 16);
	~Textedit();
	void Update(std::vector<SDL_Event>& frameEvent) override;
	void Draw(SDL_Renderer* r, rect src = {}) override;

	void setText(std::wstring s);
	void setCursorIndex(int index);
};