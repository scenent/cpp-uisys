#include "Vbox.h"

Vbox::Vbox(std::string n, rect r, float sep) : UI(n, r), separation(sep) {

}
Vbox::~Vbox() {

}
void Vbox::Update(std::vector<SDL_Event>& frameEvent) {
	int center = getObjectPos().y + (getObjectSize().y / 2);
	int children_size = -separation;
	for (int i = 0; i < objectList.size(); i++) {
		children_size += objectList[i]->getObjectSize().y;
		children_size += separation;
	}
	int start_pos = center - (children_size / 2);

	int index = 0;
	int y = start_pos;
	for (; index < objectList.size(); ) {
		objectList[index]->setObjectPos({ objectList[index]->getObjectPos().x, (float)y});
		y += objectList[index]->getObjectSize().y;
		y += separation;
		index++;
	}

	for (auto& o : objectList) {
		o->Update(frameEvent);
	}
}
void Vbox::Draw(SDL_Renderer* r, rect src) {
	if (this->visible) {
		for (auto& o : objectList) {
			o->Draw(r, src);
		}
	}
}
void Vbox::setSeparation(float s) {
	this->separation = s;
}
float Vbox::getSeparation() {
	return this->separation;
}