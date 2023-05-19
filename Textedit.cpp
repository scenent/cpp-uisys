#include "Textedit.h"
#include <algorithm>

int RenderCharacter(const wchar_t* text, vec2 pos, TTF_Font* font, SDL_Renderer* Renderer) {
	SDL_Surface* surfaceMessage = TTF_RenderUNICODE_Blended(font, (Uint16*)text, { 0, 0, 0, 255 });
	SDL_Texture* Message = SDL_CreateTextureFromSurface(Renderer, surfaceMessage);
	SDL_Rect src_rect = { 0, 0, surfaceMessage->w, surfaceMessage->h };
	SDL_Rect dst_rect = { pos.x, pos.y, surfaceMessage->w, surfaceMessage->h };
	SDL_RenderCopy(Renderer, Message, &src_rect, &dst_rect);
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
	return dst_rect.x + dst_rect.w;
}



Textedit::Textedit(std::string s, rect r, const char* fp, int fs) : UI(s, r), font_size(fs) {
	font = TTF_OpenFont(fp, fs);
}
Textedit::~Textedit() {
	
}
void Textedit::Update(std::vector<SDL_Event>& frameEvent) {
	for (auto& event : frameEvent) {
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
				int x, y; SDL_GetMouseState(&x, &y);
				if (rect::PointVsRect(vec2(x, y), rect(getObjectPos(), getObjectSize()))) {
					if (getRoot()->getCurrentFocused() == nullptr) {
						this->getRoot()->setCurrentFocused(this);
					}
					Focused = true;
				}
				else {
					if (getRoot()->getCurrentFocused() == this) {
						this->getRoot()->setCurrentFocused(nullptr);
					}
					Focused = false;
				}
			break;
		}
	}
	
	for (auto& event : frameEvent) {
		switch (event.type) {
		case SDL_KEYUP:
			if (Focused && event.key.keysym.scancode == SDL_SCANCODE_LSHIFT || event.key.keysym.scancode == SDL_SCANCODE_RSHIFT) {
				Shifted = false;
				break;
			}
			break;
		case SDL_KEYDOWN:
			if (Focused) {
				if (event.key.keysym.scancode == SDL_SCANCODE_LSHIFT || event.key.keysym.scancode == SDL_SCANCODE_RSHIFT) {
					Shifted = true;
					break;
				}
				std::string temp(SDL_GetScancodeName(event.key.keysym.scancode));
				std::wstring ttemp(temp.begin(), temp.end());
				if (ttemp == L"Backspace") {
					if (cursor_index > 0) {
						charList.erase(charList.begin() + cursor_index-1);
						cursor_index--;
					}
					break;
				}
				else if (ttemp == L"Space") {
					ttemp = L" ";
				}
				else if (ttemp == L"Left") {
					if (cursor_index > 0)
						cursor_index--;
					break;
				}
				else if (ttemp == L"Right") {
					if (cursor_index < charList.size())
						cursor_index++;
					break;
				}
				else if (ttemp == L"Left Alt" ||
					ttemp == L"Right Alt" ||
					ttemp == L"Up" ||
					ttemp == L"Down" ||
					ttemp == L"Left Alt" ||
					ttemp == L"Right Ctrl" ||
					ttemp == L"Left Ctrl" ||
					ttemp == L"Escape" ||
					ttemp == L"Left Shift" ||
					ttemp == L"Right Shift"
					)
				{
					break;
				}
				if (!Shifted) {
					transform(ttemp.begin(), ttemp.end(), ttemp.begin(), towlower);
				}
				charList.insert(charList.begin() + cursor_index , ttemp);
				cursor_index++;
			}
			break;
		}
	}
}
void Textedit::Draw(SDL_Renderer* r, rect src) {
	if (this->visible) {
		if ((src.pos.x <= getObjectPos().x && getObjectPos().x + getObjectSize().x <= src.pos.x + src.size.x &&
			src.pos.y <= getObjectPos().y && getObjectPos().y + getObjectSize().y <= src.pos.y + src.size.y
			) || src.IsZero()) {
			int last_Xpos = getObjectPos().x;
			charEndPosList = { last_Xpos };
			if (cursor_index == 0) {
				SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
				SDL_RenderDrawLine(r, last_Xpos, getObjectPos().y, last_Xpos, getObjectPos().y + getObjectSize().y);
			}
			int i;
			for (i = 0; i < charList.size(); i++) {
				if (getObjectPos().x + getObjectSize().x < last_Xpos) {
					break;
				}
				if (cursor_index == i && i != 0) {
					SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
					SDL_RenderDrawLine(r, last_Xpos, getObjectPos().y, last_Xpos, getObjectPos().y + getObjectSize().y);
				}
				last_Xpos = RenderCharacter(charList[i].c_str(), vec2(last_Xpos, getObjectPos().y), font, r);
				charEndPosList.push_back(last_Xpos);
			}
			if (cursor_index == i) {
				SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
				SDL_RenderDrawLine(r, last_Xpos, getObjectPos().y, last_Xpos, getObjectPos().y + getObjectSize().y);
			}
		}
		for (auto& p : objectList) {
			p->Draw(r, src);
		}
	}
}

void Textedit::setText(std::wstring s) {
	this->charList.clear();
	for (int i = 0; i < s.length(); i++) {
		this->charList.push_back(std::wstring(1, s[i]));
	}
	std::vector<std::function<void(UI*)>> arr = objectEventMap["_on_text_changed"];
	for (auto& func : arr) { func(this); }
}

void Textedit::setCursorIndex(int index) {
	cursor_index = index;
}
