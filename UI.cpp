#include "UI.h"

UI::UI(std::string n, rect r) {
	this->objectName = n;
	this->objectRect = r;
}
UI::~UI() {
	
}
void UI::Update(std::vector<SDL_Event>& frameEvent) {
	for (auto& o : objectList) {
		o->Update(frameEvent);
	}
}
void UI::Draw(SDL_Renderer* r, rect src) {
	if (this->visible) {
		for (auto& o : objectList) {
			o->Draw(r, src);
		}
	}
}
void UI::DrawRect(SDL_Renderer* r) {
	SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
	const SDL_Rect t = { objectRect.pos.x, objectRect.pos.y, objectRect.size.x, objectRect.size.y };
	SDL_RenderDrawRect(r, &t);
}
void UI::setObjectPos(vec2 p) {
	vec2 amount = p - objectRect.pos;
	this->objectRect.pos = p;
	
	for (auto& o : objectList) {
		o->setObjectPos(o->getObjectPos() + amount);
	}

	std::vector<std::function<void(UI*)>> arr = objectEventMap["_on_reposed"];
	for (auto& func : arr) {
		func(this);
	}
}
void UI::setObjectSize(vec2 s) {
	this->objectRect.size = s;
	std::vector<std::function<void(UI*)>> arr = objectEventMap["_on_resized"];
	for (auto& func : arr) {
		func(this);
	}
}
vec2 UI::getObjectPos() {return this->objectRect.pos;}
vec2 UI::getObjectSize() {return this->objectRect.size;}

vec2 UI::getObjectFullSize() {
	float x, y, w, h;
	x = objectRect.pos.x;
	y = objectRect.pos.y;
	w = objectRect.pos.x + objectRect.size.x;
	h = objectRect.pos.y + objectRect.size.y;
	for (auto& p : objectList) {
		if (p->getObjectPos().x < x) { x = p->getObjectPos().x; }
		if (p->getObjectPos().y < y) { y = p->getObjectPos().y; }
		if (p->getObjectPos().x + p->getObjectFullSize().x < w) { w = p->getObjectPos().x + p->getObjectFullSize().x; }
		if (p->getObjectPos().y + p->getObjectFullSize().y < h) { h = p->getObjectPos().y + p->getObjectFullSize().y; }
	}
	return vec2(w - x, h - y);
}

void UI::setVisible(bool b) {
	visible = b;
}
bool UI::getVisible() {
	return visible;
}

void UI::setOwner(UI* o) {
	this->objectOwner = o;
}

UI* UI::getOwner() {
	return this->objectOwner;
}

void UI::setName(std::string n) {
	objectName = n;
}
std::string UI::getName() {
	return objectName;
}


void UI::addObject(UI* o) {
	o->setOwner(this);
	objectList.push_back(o);
}

void UI::removeObject(int index) {
	if (index > objectList.size() - 1) { return; }
	objectList.erase(objectList.begin() + index);
}

int UI::getObjectIndex(UI* o) {
	int result = 0;
	for (result; result < objectList.size(); result++) {
		if (objectList[result] == o) { return result; }
	}
	return -1;
}
UI* UI::getChild(std::string name) {
	for (auto& p : objectList) {
		if (p->getName() == name) {
			return p;
		}
	}
	return nullptr;
}

void UI::printTree(int depth) {
	for (int i = 0; i < depth; i++) { std::cout << " "; }
	std::cout << this->objectName << std::endl;
	for (auto& o : objectList) { o->printTree(depth + 4); }
}

void UI::addEventListener(std::string event_name, std::function<void(UI*)> func) {
	objectEventMap[event_name].push_back(func);
}
void UI::setCurrentFocused(UI* o) {
	currentFocused = o;
}
UI* UI::getCurrentFocused() {
	return currentFocused;
}

UI* UI::getRoot() {
	UI* result = getOwner();
	while (result != nullptr) {
		if (result->getOwner() == nullptr) {
			break;
		}
		result = result->getOwner();
	}
	return result;
}