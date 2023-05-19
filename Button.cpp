#include "Button.h"

Button::Button(std::string n, rect r) : UI(n, r){
	buttonState = ButtonState::Normal;
}

Button::~Button() {

}

void Button::Update(std::vector<SDL_Event>& frameEvent){
	for (auto& event : frameEvent) {
		switch (event.type) {
		case SDL_MOUSEMOTION:
			if (getRoot()->getCurrentFocused() != nullptr) {
				break;
			}
			int x, y;
			SDL_GetMouseState(&x, &y);
			if (rect::PointVsRect({ (float)x, (float)y }, clipped_rect.IsZero() ? this->objectRect : this->clipped_rect)) {
				if (buttonState != ButtonState::Down) {
					buttonState = ButtonState::Hover;
				}
			}
			else {
				if (buttonState != ButtonState::Down) {
					buttonState = ButtonState::Normal;
					if (getRoot()->getCurrentFocused() == this) {
						getRoot()->setCurrentFocused(nullptr);
					}
				}
			}
			break;
		}
	}

	for (auto& event : frameEvent) {
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			if (this->getRoot()->getCurrentFocused() == nullptr && rect::PointVsRect({(float)event.button.x, (float)event.button.y}, clipped_rect.IsZero() ? this->objectRect : this->clipped_rect)) {
				this->getRoot()->setCurrentFocused(this);
				buttonState = ButtonState::Down;
				std::vector<std::function<void(UI*)>> arr = objectEventMap["_on_button_down"];
				for (auto& func : arr) { func(this); }
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (buttonState == ButtonState::Down) {
				if (checkInRect) {
					if (rect::PointVsRect({ (float)event.button.x, (float)event.button.y }, clipped_rect.IsZero() ? this->objectRect : this->clipped_rect)) {
						std::vector<std::function<void(UI*)>> arr = objectEventMap["_on_button_up"];
						for (auto& func : arr) { func(this); }
						buttonState = ButtonState::Hover;
						this->getRoot()->setCurrentFocused(nullptr);
					}
					else {
						getRoot()->setCurrentFocused(nullptr);
					}
				}
				else {
					std::vector<std::function<void(UI*)>> arr = objectEventMap["_on_button_up"];
					for (auto& func : arr) { func(this); }
					buttonState = ButtonState::Hover;
					this->getRoot()->setCurrentFocused(nullptr);
				}
			}
			break;
		}
	}
	switch (getButtonState()) {
	case ButtonState::Normal:
		setButtonColor({ 193, 193, 193, 255 });
		break;
	case ButtonState::Hover:
		setButtonColor({ 168, 168, 168, 255 });
		break;
	case ButtonState::Down:
		setButtonColor({ 120, 120, 120, 255 });
		break;
	}
	for (auto& o : objectList) {
		o->Update(frameEvent);
	}
}

void Button::Draw(SDL_Renderer* r, rect src) {
	if (this->visible) {
		SDL_SetRenderDrawColor(r, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
		SDL_Rect dst_rect = { this->getObjectPos().x, this->getObjectPos().y, this->getObjectSize().x, this->getObjectSize().y };
		if (not src.IsZero()) {
			if (src.pos.x > dst_rect.x) {
				dst_rect.w = getObjectSize().x - (src.pos.x - dst_rect.x);
				dst_rect.x = src.pos.x;
				if (dst_rect.w < 0) {
					dst_rect.w = 0;
				}
			}

			if (src.pos.y > dst_rect.y) {
				dst_rect.h = getObjectSize().y - (src.pos.y - dst_rect.y);
				dst_rect.y = src.pos.y;
				if (dst_rect.h < 0) {
					dst_rect.h = 0;
				}
			}

			if (src.pos.x + src.size.x < dst_rect.x + dst_rect.w) {
				dst_rect.w = src.pos.x + src.size.x - dst_rect.x;
				if (dst_rect.w < 0) {
					dst_rect.w = 0;
				}
			}

			if (src.pos.y + src.size.y < dst_rect.y + dst_rect.h) {
				dst_rect.h = src.pos.y + src.size.y - dst_rect.y;
				if (dst_rect.h < 0) {
					dst_rect.h = 0;
				}
			}
			clipped_rect = { (float)dst_rect.x, (float)dst_rect.y, (float)dst_rect.w, (float)dst_rect.h };
		}
		const SDL_Rect dr(dst_rect);
		SDL_RenderFillRect(r, &dr);

		for (auto& o : objectList) {
			o->Draw(r, src);
		}
	}
}

void Button::DrawOnceIgnore(SDL_Renderer* r) {
	if (this->visible) {
		SDL_SetRenderDrawColor(r, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
		SDL_Rect dst_rect = { this->getObjectPos().x, this->getObjectPos().y, this->getObjectSize().x, this->getObjectSize().y };
		SDL_RenderFillRect(r, &dst_rect);
	}
}

int Button::getButtonState() {
	return buttonState;
}

void Button::setCheckInRect(bool b) {
	checkInRect = b;
}

void Button::setButtonColor(const SDL_Color& c){
	this->buttonColor = c;
}

SDL_Color Button::getButtonColor() {
	return this->buttonColor;
}

rect Button::getClippedRect() {
	return this->clipped_rect;
}