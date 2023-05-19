#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include <vector>
#include <string>
#include <iostream>
#include <functional>
#include <map>
#include <math.h>
/*
_on_reposed
_on_resized
*/

struct vec2 {
	float x, y;
	vec2(float x, float y) : x(x), y(y) {};
	vec2 operator+(const vec2& v) { return { this->x + v.x, this->y + v.y }; }
	void operator+=(const vec2& v) { this->x += v.x; this->y += v.y; return; }
	vec2 operator-(const vec2& v) { return { this->x - v.x, this->y - v.y }; }
	void operator-=(const vec2& v) { this->x -= v.x; this->y -= v.y; return; }
	vec2 operator*(const int& i) { return { this->x * i, this->y * i }; }
	vec2 operator/(const int& i) { return { this->x / i, this->y / i }; }
};
struct rect {
	vec2 pos, size;
	rect() : pos({ 0.0f, 0.0f }), size({ 0.0f, 0.0f }) { }
	rect(vec2 p, vec2 s) : pos(p), size(s){ }
	rect(float a, float b, float c, float d) : pos({ a, b }), size({ c, d }) { }
	bool operator==(rect r) { return r.pos.x == pos.x && r.pos.y == pos.y && r.size.x == size.x && r.size.y == size.y; }
	bool operator!=(rect r) { return !(r==*this); }
	inline static bool RectVsRect(rect& r1, rect& r2) {
		if (r1.pos.x + r1.size.x >= r2.pos.x && r1.pos.x <= r2.pos.x + r2.size.x && r1.pos.y + r1.size.y >= r2.pos.y && r1.pos.y <= r2.pos.y + r2.size.y) { return true; }
		return false;
	}
	inline static bool PointVsRect(vec2 point, rect rect) {
		return (rect.pos.x <= point.x) && (point.x <= rect.pos.x + rect.size.x) && (rect.pos.y <= point.y) && (point.y <= rect.pos.y + rect.size.y);
	}
	inline bool IsZero() { return pos.x == 0.0f && pos.y == 0.0f && size.x == 0.0f && size.y == 0.0f; }
};


class UI {
private:
	std::string objectName;
	UI* objectOwner = nullptr;
	UI* currentFocused = nullptr;
protected:
	std::vector<UI*> objectList;
	rect objectRect;
	std::map<std::string, std::vector<std::function<void(UI*)>>> objectEventMap;
	bool visible = true;
public:
	UI(std::string n, rect r = {});
	virtual ~UI();
	virtual void Update(std::vector<SDL_Event>& frameEvent);
	virtual void Draw(SDL_Renderer* r, rect src = {});

	void DrawRect(SDL_Renderer* r);
	void setObjectPos(vec2 p);
	void setObjectSize(vec2 s);
	vec2 getObjectPos();
	vec2 getObjectSize();
	vec2 getObjectFullSize();
	void setName(std::string n);
	std::string getName();
	void setVisible(bool b);
	bool getVisible();
	
	void setOwner(UI* o);
	UI* getOwner();
	void addObject(UI* o);
	void removeObject(int index);
	int getObjectIndex(UI* o);
	UI* getChild(std::string name);
	void printTree(int depth = 0);

	void addEventListener(std::string event_name, std::function<void(UI*)> func);

	void setCurrentFocused(UI* o);
	UI* getCurrentFocused();

	UI* getRoot();

};