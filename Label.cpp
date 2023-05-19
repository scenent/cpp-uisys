#include "Label.h"

void RenderText(const wchar_t* text, UI* obj, TTF_Font* font, SDL_Renderer* Renderer, int H_Align = 1, int V_Align = 1, rect src = {}) {
	SDL_Surface* surfaceMessage = TTF_RenderUNICODE_Blended(font, (Uint16*)text, {0, 0, 0, 255});
	SDL_Texture* Message = SDL_CreateTextureFromSurface(Renderer, surfaceMessage);
	SDL_Rect src_rect = { 0, 0, surfaceMessage->w, surfaceMessage->h };
	SDL_Rect dst_rect = { obj->getObjectPos().x, obj->getObjectPos().y, surfaceMessage->w, surfaceMessage->h};
	if (H_Align == 1) {
		dst_rect.x = (obj->getObjectPos().x + obj->getObjectSize().x/2) - surfaceMessage->w / 2;
	}
	else if (H_Align == 2) {
		dst_rect.x = (obj->getObjectPos().x + obj->getObjectSize().x) - surfaceMessage->w;
	}
	if (V_Align == 1) {
		dst_rect.y = (obj->getObjectPos().y + obj->getObjectSize().y / 2) - surfaceMessage->h / 2;
	}
	else if (V_Align == 2) {
		dst_rect.y = (obj->getObjectPos().y + obj->getObjectSize().y) - surfaceMessage->h;
	}
	if (!(src.pos.x <= dst_rect.x && dst_rect.x + dst_rect.w <= src.pos.x + src.size.x && 
		src.pos.y <= dst_rect.y && dst_rect.y + dst_rect.h <= src.pos.y + src.size.y
		) && !src.IsZero()) {
		return;
	}
	SDL_RenderCopy(Renderer, Message, &src_rect, &dst_rect);
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}

Label::Label(std::string n, rect r, std::wstring t, const char* p, int ps) : UI(n, r), text(t), font_path(p), font_size(ps){
	font = TTF_OpenFont(p, ps);
}
void Label::Update(std::vector<SDL_Event>& frameEvent) {

}
void Label::Draw(SDL_Renderer* r, rect src) {
	if (this->visible) {
		RenderText(text.c_str(), this, font, r, (int)H_Align, (int)V_Align, src);
		for (auto& o : objectList) {
			o->Draw(r, src);
		}
	}
}

void Label::setText(std::wstring s) {
	text = s;
}
std::wstring Label::getText() {
	return text;
}

void Label::setH_Align(HorizontalAlign i) {
	H_Align = i;
	std::vector<std::function<void(UI*)>> arr = objectEventMap["_on_realigned_h"];
	for (auto& func : arr) { func(this); }
}
void Label::setV_Align(VerticalAlign i) {
	V_Align = i;
	std::vector<std::function<void(UI*)>> arr = objectEventMap["_on_realigned_v"];
	for (auto& func : arr) { func(this); }
}