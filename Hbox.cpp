#include "Hbox.h"

Hbox::Hbox(std::string n, rect r, float sep) : UI(n, r), separation(sep) {
	
}
Hbox::~Hbox() {

}
void Hbox::Update(std::vector<SDL_Event>& frameEvent) {
	int center = getObjectPos().x + (getObjectSize().x / 2);
	int children_size = -separation;
	for (int i = 0; i < objectList.size(); i++) {
		children_size += objectList[i]->getObjectSize().x;
		children_size += separation;
	}
	int start_pos = center - (children_size / 2);

	int index = 0;
	int x = start_pos;
	for (;index < objectList.size(); ) {
		objectList[index]->setObjectPos({ (float)x, objectList[index]->getObjectPos().y });
		x += objectList[index]->getObjectSize().x;
		x += separation;
		index++;
	}
	
	for (auto& o : objectList) {
		o->Update(frameEvent);
	}
}
void Hbox::Draw(SDL_Renderer* r, rect src) {
	if (this->visible) {
		for (auto& o : objectList) {
			o->Draw(r, src);
		}
	}
}
void Hbox::setSeparation(float s) {
	this->separation = s;
}
float Hbox::getSeparation() {
	return this->separation;
}