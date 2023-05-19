#include "Vslide.h"

void clipGrabber(Button* grabber, Vslide* slide) {
	if (grabber->getObjectPos().y < slide->getObjectPos().y) {
		grabber->setObjectPos({ grabber->getObjectPos().x, slide->getObjectPos().y });
	}
	else if ((grabber->getObjectPos().y + grabber->getObjectSize().y) > slide->getObjectPos().y + slide->getObjectSize().y) {
		grabber->setObjectPos({ grabber->getObjectPos().x, slide->getObjectPos().y + slide->getObjectSize().y - grabber->getObjectSize().y });
	}
}

void _on_v_grabber_down(UI* obj) {
	((Vslide*)obj->getOwner())->setIsDragging(true);
}
void _on_v_grabber_up(UI* obj) {
	((Vslide*)obj->getOwner())->setIsDragging(false);
}

Vslide::Vslide(std::string n, rect r, float sep) : UI(n, r), separation(sep) {
	grabber = new Button("Grabber", {(r.pos.x + r.size.x) - grabber_width, r.pos.y, grabber_width, r.size.y});
	grabber->setCheckInRect(false);
	grabber->setVisible(false);
	grabber->setOwner(this);
	grabber->setButtonColor({193, 193, 193, 255});
	grabber->addEventListener("_on_button_down", _on_v_grabber_down);
	grabber->addEventListener("_on_button_up", _on_v_grabber_up);
}
int mouseOldPosY = 0;
void Vslide::Update(std::vector<SDL_Event>& frameEvent) {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	
	int children_size = -separation;
	for (int i = 0; i < objectList.size(); i++) {
		children_size += objectList[i]->getObjectSize().y;
		children_size += separation;
	}
	if (children_size - getObjectSize().y > 0) {
		grabber->setVisible(true);
		float wheel_amount = 0.0f;
		for (auto& event : frameEvent) {
			switch (event.type) {
			case SDL_MOUSEWHEEL:
				wheel_amount = event.wheel.y;
			}
		}
		
		grabber->setObjectSize({ grabber->getObjectSize().x, getObjectSize().y * getObjectSize().y / children_size});
		if (grabber->getObjectSize().y >= getObjectSize().y) {
			isDragging = false;
			grabber->setVisible(false);
		}
		if (isDragging) {
			if ((grabber->getObjectPos().y + (mouseY - mouseOldPosY) >= getObjectPos().y) && (grabber->getObjectPos().y + grabber->getObjectSize().y +  (mouseY - mouseOldPosY) <= getObjectPos().y + getObjectSize().y)) {
				grabber->setObjectPos({ grabber->getObjectPos().x, grabber->getObjectPos().y + (mouseY - mouseOldPosY) });
				float grabber_center = grabber->getObjectPos().y;
				float start = getObjectPos().y;
				float end = getObjectPos().y + getObjectSize().y - grabber->getObjectSize().y;
				drag_amount = grabber_center / (end - start) - start / (end-start);
			}
		}
		if (wheel_amount != 0.0f) {
			grabber->setObjectPos(grabber->getObjectPos() + vec2(0, -wheel_amount * wheel_scale * (getObjectSize().y - grabber->getObjectSize().y)));
			drag_amount -= wheel_amount * wheel_scale;
			if (drag_amount > 1.0f) { drag_amount = 1.0f;}
			else if (drag_amount < 0.0f) {drag_amount = 0.0f;}
		}
		clipGrabber(grabber, this);
	}
	else {
		grabber->setVisible(false);
	}

	for (auto& event : frameEvent) {
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			if (rect::PointVsRect(vec2(mouseX, mouseY), { getObjectPos() + vec2(getObjectSize().x - grabber_width, 0), getObjectSize() }) && not rect::PointVsRect(vec2(mouseX, mouseY), { grabber->getObjectPos(), grabber->getObjectSize() })) {
				float amount = 0.0f;
				if (mouseY > grabber->getObjectPos().y + grabber->getObjectSize().y / 2) {
					amount = float(mouseY - grabber->getObjectSize().y);
				}
				else {
					amount = mouseY;
				}
				grabber->setObjectPos({ grabber->getObjectPos().x, amount });
				clipGrabber(grabber, this);
			}
		}
	}
	int start_pos = getObjectPos().y + (getObjectSize().y - children_size) * drag_amount;//bottom - (children_size / 2);
	
	int index = 0;int y = start_pos;
	for (; index < objectList.size(); ) { objectList[index]->setObjectPos({ objectList[index]->getObjectPos().x, (float)y });y += objectList[index]->getObjectSize().y;y += separation;index++;}
	
	grabber->Update(frameEvent);
	for (auto& o : objectList) {
		o->Update(frameEvent);
	}
	mouseOldPosY = mouseY;
}

void Vslide::Draw(SDL_Renderer* r, rect src) {
	if (this->visible) {
		grabber->DrawOnceIgnore(r);
		for (auto& o : objectList) {
			o->Draw(r, { getObjectPos(), getObjectSize() });
		}
	}
}

void Vslide::setSeparation(float s) {
	this->separation = s;
}
float Vslide::getSeparation() {
	return this->separation;
}

bool Vslide::getIsDragging() {
	return isDragging;
}
void Vslide::setIsDragging(bool b) {
	isDragging = b;
}

