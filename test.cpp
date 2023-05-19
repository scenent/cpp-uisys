#include "UI.h"
#include "Button.h"
#include "Hbox.h"
#include "Vbox.h"
#include "Label.h"
#include "Vslide.h"
#include "Hslide.h"
#include "Textedit.h"
using namespace std;

SDL_Window* Window;
SDL_Renderer* Renderer;
bool isRunning = true;
int WindowH = 600;
int WindowW = 600;

void change_text(UI* obj) {
	((Label*)obj->getRoot()->getChild("l1"))->getText() == L"This is Label" ? ((Label*)obj->getRoot()->getChild("l1"))->setText(L"Bt Pressed!") : ((Label*)obj->getRoot()->getChild("l1"))->setText(L"This is Label");
}

void add_obj(UI* obj) {
	Button* temp = new Button("temp", { 230, 30, 120, 80 }); temp->setCheckInRect(false);
	obj->getRoot()->getChild("vs1")->addObject(temp);
}


int SDL_main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	Window = SDL_CreateWindow("Simple UI System", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowH, WindowW, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
	Renderer = SDL_CreateRenderer(Window, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	UI* Root = new UI("Root", { 0.0f, 0.0f, 600.0f, 600.0f });

	Button* b1 = new Button("b1", { 30, 30, 70, 40 }); b1->setButtonColor({ 255, 0, 0, 0 }); b1->setCheckInRect(false);
	b1->addEventListener("_on_button_down", change_text);
	Label* b1_l = new Label("b1_l", { 40.0f, 40.0f, 50.0f, 16.0f }, L"Button", "NanumGothic.ttf", 16);
	Label* l1 = new Label("l1", { 120, 30, 100, 40 }, L"This is Label", "NanumGothic.ttf", 16);
	Vslide* vs1 = new Vslide("vs1", { 230, 30, 150, 400 }, 10);
		Button* vs1_b1 = new Button("vs1_b1", { 230, 30, 120, 80 }); vs1_b1->setCheckInRect(false);
		Button* vs1_b2 = new Button("vs1_b2", { 230, 30, 120, 80 }); vs1_b2->setCheckInRect(false);
		Button* vs1_b3 = new Button("vs1_b3", { 230, 30, 120, 80 }); vs1_b3->setCheckInRect(false);
		Button* vs1_b4 = new Button("vs1_b4", { 230, 30, 120, 80 }); vs1_b4->setCheckInRect(false);
		Button* vs1_b5 = new Button("vs1_b5", { 230, 30, 120, 80 }); vs1_b5->setCheckInRect(false);
		Button* vs1_b6 = new Button("vs1_b6", { 230, 30, 120, 80 }); vs1_b6->setCheckInRect(false);
	Button* add_bt = new Button("add_bt", { 230, 440, 150, 40 }); add_bt->setCheckInRect(false);
		Label* add_bt_l = new Label("add_bt_l", { 230, 440, 150, 40 }, L"Add Obj", "NanumGothic.ttf", 16);
		add_bt->addEventListener("_on_button_down", add_obj);
		Textedit* te1 = new Textedit("te1", { 30, 80, 180, 30 }, "NanumGothic.ttf", 16); te1->setText(L"This is Textedit"); te1->setCursorIndex(16);
	Root->addObject(b1);
		b1->addObject(b1_l);
	Root->addObject(l1);
	Root->addObject(vs1);
		vs1->addObject(vs1_b1);
		vs1->addObject(vs1_b2);
		vs1->addObject(vs1_b3);
		vs1->addObject(vs1_b4);
		vs1->addObject(vs1_b5);
		vs1->addObject(vs1_b6);
	Root->addObject(add_bt);
		add_bt->addObject(add_bt_l);
	Root->addObject(te1);
	cout << "Printing Tree.." << "\n";
	Root->printTree();
	while (isRunning) {
		SDL_Event event;
		vector<SDL_Event> frameEvent;
		while (SDL_PollEvent(&event)) {
			frameEvent.push_back(event);
			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			}
		}
		//---------------
		Root->Update(frameEvent);
		//---------------
		SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
		SDL_RenderClear(Renderer);

		//---------------
		Root->Draw(Renderer);
		l1->DrawRect(Renderer);
		add_bt->DrawRect(Renderer);
		te1->DrawRect(Renderer);
		b1->DrawRect(Renderer);
		vs1->DrawRect(Renderer);
		//---------------
		SDL_RenderPresent(Renderer);
	}
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}