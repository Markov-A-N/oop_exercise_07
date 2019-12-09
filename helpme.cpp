#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

#include "editor.h"
Editor editor;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int BUTTON_WIDTH = 80;
const int BUTTON_HEIGHT = 20;
const int TOTAL_BUTTONS = 7;

const int COORDINATE_SYSTEM_WIDTH = SCREEN_WIDTH - BUTTON_WIDTH - 60;
const int COORDINATE_SYSTEM_HEIGHT = SCREEN_HEIGHT - 20;
const int COORDINATE_SYSTEM_X = BUTTON_WIDTH + 50;
const int COORDINATE_SYSTEM_Y = 10;

void create(Editor &editor) {
	std::string cmd;
	if (editor.DocumentExist()) {
		std::cout << "Save old document or don't save? Yes/No\n";
		std::cin >> cmd;
		if (cmd == "Yes") {
			std::string filename;
			std::cout << "Enter name of file\n";
			std::cin >> filename;
			try {
				editor.SaveDocument(filename);
				std::cout << "Document save in file " << filename << "\n";
			} catch (std::runtime_error &err) {
				std::cout << err.what() << "\n";
			}
		}
		std::cout << "Enter name of new project\n";
	}
	std::cin >> cmd;
	editor.CreateDocument(cmd);
	std::cout << "Document " << cmd << " is created\n";
}

void save(Editor &editor) {
	std::string filename;
	std::cin >> filename;

	try {
		editor.SaveDocument(filename);
		std::cout << "Document save if file " << filename << "\n";
	} catch (std::runtime_error &err) {
		std::cout << err.what() << "\n";
	}
}

void load(Editor &editor) {
	std::string cmd;
	if (editor.DocumentExist()) {
		std::cout << "Save old document or don't save? Yes/No\n";
		std::cin >> cmd;
		if (cmd == "Yes") {
			std::string filename;
			std::cout << "Enter name of file\n";
			std::cin >> filename;
			try {
				editor.SaveDocument(filename);
				std::cout << "Document save in file " << filename << "\n";
			} catch (std::runtime_error& err) {
				std::cout << err.what() << "\n";
			}
		}
	}
	std::string filename;
	std::cin >> filename;
	try {
		editor.LoadDocument(filename);
		std::cout << "Document loaded from file " << filename << "\n";
	} catch (std::runtime_error& err) {
		std::cout << err.what() << "\n";
	}
}

void add(Editor &editor) {
	char type;
	std::cin >> type;
	if (type == 'T') {
		std::pair<double, double> *vertices = new std::pair<double, double>[3];
		for (int i = 0; i < 3; i++) {
			std::cin >> vertices[i].first >> vertices[i].second;
		}
		editor.InsertPrimitive(TRIANGLE, vertices);
	}
	else if (type == 'S') {
		std::pair<double, double> *vertices = new std::pair<double, double>[4];
		for (int i = 0; i < 4; i++) {
			std::cin >> vertices[i].first >> vertices[i].second;
		}
		editor.InsertPrimitive(SQUARE, vertices);
	}
	else if (type == 'R') {
		std::pair<double, double> *vertices = new std::pair<double, double>[4];
		for (int i = 0; i < 4; i++) {
			std::cin >> vertices[i].first >> vertices[i].second;
		}
		editor.InsertPrimitive(RECTANGLE, vertices);
	}
	else {
		std::cout << "Primitive isn't added\n";
		std::cin.clear();
		std::cin.ignore(30000, '\n');
		return;
	}
	std::cout << "Primitive is added\n";
}

void remove(Editor &editor) {
	int id;
	std::cin >> id;

	try {
		editor.RemovePrimitive(id);
		std::cout << "Primitive with " << id << " is removed\n";
	} catch (std::logic_error &err) {
		std::cout << err.what() << "\n";
	}
}


enum WordsCode {
	CREATE,
	LOAD,
	SAVE,
	ADD,
	REMOVE,
	UNDO,
	PRINT
};

class Texture {
public:
	Texture() {
		mTexture = nullptr;
		mWidth = 0;
		mHeight = 0;
	}

	~Texture() {
		free();
	}

	void free() {
		if( mTexture != nullptr )
		{
			SDL_DestroyTexture( mTexture );
			mTexture = nullptr;
			mWidth = 0;
			mHeight = 0;
		}
	}

	bool CreateTexture(SDL_Renderer *&gRenderer, SDL_Color *color) {
		SDL_Texture* newTexture = nullptr;
		SDL_Surface *s = SDL_CreateRGBSurface(0, BUTTON_WIDTH, BUTTON_HEIGHT, 32, 0, 0, 0, 0);
		if (SDL_FillRect(s, nullptr, SDL_MapRGB(s->format, color->r, color->g, color->b)) != 0) {
			std::cout << "CreateTexture error: " << SDL_GetError() << "\n";
			return false;
		}
		newTexture = SDL_CreateTextureFromSurface(gRenderer, s);
		if( newTexture == nullptr ) {
			std::cout << "SDL_Error: " << SDL_GetError() << "\n";
			return false;
		}
		else {
			mWidth = s->w;
			mHeight = s->h;
		}
		SDL_FreeSurface( s );
		mTexture = newTexture;
		return true;
	}

	void Render(SDL_Renderer *&gRenderer, int x, int y/*, SDL_Rect *clip*/) {
		SDL_Rect renderQuad = { x, y, mWidth, mHeight };

		/*if( clip != nullptr ) {
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}*/

		SDL_RenderCopy(gRenderer, mTexture, /*clip, */nullptr, &renderQuad);
	}

	int getWidth() {
		return mWidth;
	}

	int getHeight() {
		return mHeight;
	}

private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};

class TextureText {
public:
	TextureText() {
		mTexture = nullptr;
		mWidth = 0;
		mHeight = 0;
	}

	~TextureText() {
		free();
	}

	void free() {
		if( mTexture != nullptr ) {
			SDL_DestroyTexture( mTexture );
			mTexture = nullptr;
			mWidth = 0;
			mHeight = 0;
		}
	}

	bool loadFromRenderedText( SDL_Renderer *&gRenderer, std::string textureText, SDL_Color textColor, TTF_Font *&gFont ) {
		free();

		SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
		if( textSurface == nullptr ) {
			std::cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << "\n";
		} else {
	        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
			if( mTexture == nullptr ) {
				std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
			} else {
				//Get image dimensions
				mWidth = textSurface->w;
				mHeight = textSurface->h;
			}

			//Get rid of old surface
			SDL_FreeSurface( textSurface );
		}
		
		//Return success
		return mTexture != NULL;
	}

	bool CreateTextToButton(SDL_Renderer *&gRenderer, char *str, TTF_Font *&gFont) {
		SDL_Color textColor = {0, 0, 0};
		if (!loadFromRenderedText(gRenderer, str, textColor, gFont)) {
			std::cout << "Failed to render text texture!\n";
			return false;
		}
		return true;
	}

	void Render(SDL_Renderer *&gRenderer, int x, int y/*, SDL_Rect *clip*/) {
		SDL_Rect renderQuad = { x, y, mWidth, mHeight };

		/*if( clip != nullptr ) {
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}*/

		SDL_RenderCopy(gRenderer, mTexture, /*clip, */nullptr, &renderQuad);
	}

	int getWidth() {
		return mWidth;
	}

	int getHeight() {
		return mHeight;
	}

private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};

class Button {
public:
	Button() : x_{0}, y_{0} {};

	void setPosition(int x, int y) {
		x_ = x;
		y_ = y;
	}

	void handleEvent(SDL_Event* e, int id) {
		if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP ) {
			//Get mouse position
			int x, y;
			SDL_GetMouseState( &x, &y );

			//Check if mouse is in button
			bool inside = true;

			//Mouse is left of the button
			if( x < x_ ) {
				inside = false;
			}
			//Mouse is right of the button
			else if( x > x_ + BUTTON_WIDTH )
			{
				inside = false;
			}
			//Mouse above the button
			else if( y < y_ )
			{
				inside = false;
			}
			//Mouse below the button
			else if( y > y_ + BUTTON_HEIGHT )
			{
				inside = false;
			}

			//Mouse is outside button
			if( !inside )
			{
				//std::cout << "not inside\n";
			}
			//Mouse is inside button
			else
			{
				//Set mouse over sprite
				switch( e->type )
				{
					case SDL_MOUSEMOTION:
						//std::cout << "inside\n";
						break;
				
					case SDL_MOUSEBUTTONDOWN:
						switch (id) {
							case CREATE:
								create(editor);
								break;
							case LOAD:
								load(editor);
								break;
							case SAVE:
								save(editor);
								break;
							case ADD:
								add(editor);
								break;
							case REMOVE:
								remove(editor);
								break;
							case UNDO:
								editor.Undo();
								break;
							case PRINT:
								editor.PrintDocument();
								break;
						}
						std::cin.clear();
						std::cin.ignore(30000, '\n');
						fflush(stdin);
						//std::cout << (id == CREATE) << "\n";
						//std::cout << "down\n";
						break;
					
					case SDL_MOUSEBUTTONUP:
						//std::cout << "up\n";
						break;
				}
			}
		}
	}

	void render(SDL_Renderer *&gRenderer, Texture &gButtonTexture, TextureText &gTextTexture, char *word, TTF_Font *&gFont) {
		gButtonTexture.Render( gRenderer, x_, y_ );
		if (!gTextTexture.CreateTextToButton(gRenderer, word, gFont)) {
			std::cout << "Failed create text to button\n";
		} else {
			gTextTexture.Render( gRenderer, x_, y_ );
		}
	}

private:
	int x_;
	int y_;
};

bool loadMedia(SDL_Renderer *&gRenderer, Texture &gButtonTexture, Button *gButtons, TTF_Font *&gFont)
{
	bool success = true;
	SDL_Color background = {50, 100, 50};
	if (!gButtonTexture.CreateTexture(gRenderer, &background)) {
		std::cout << "Failed to load button sprite texture!\n";
		success = false;
	} else {
		gButtons[ 0 ].setPosition( 30, 70 );
		gButtons[ 1 ].setPosition( 30, 100 );
		gButtons[ 2 ].setPosition( 30, 130 );
		gButtons[ 3 ].setPosition( 30, 160 );
		gButtons[ 4 ].setPosition( 30, 190 );
		gButtons[ 5 ].setPosition( 30, 220 );
		gButtons[ 6 ].setPosition( 30, 250 );
	}

	gFont = TTF_OpenFont("OpenSans-Regular.ttf", 12);
	if (gFont == nullptr) {
		std::cout << "Failed to load lazy font! SDL_ttf Error: " << TTF_GetError() << "\n";
		success = false;
	} 

	return success;
}

bool init(SDL_Window *gWindow, SDL_Renderer *&Renderer)
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			std::cout << "Warning: Linear texture filtering not enabled!\n";
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == nullptr )
		{
			std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			Renderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( Renderer == nullptr )
			{
				std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << "\n";
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( Renderer, 0xFF, 0xFF, 0xFF, 0xFF );

				/*//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}*/
				if( TTF_Init() == -1 )
				{
					std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << "\n";
					success = false;
				}
			}
		}
	}

	return success;
}

void close(SDL_Window *gWindow, SDL_Renderer *gRenderer, Texture &gButtonTexture, TTF_Font *&gFont, TextureText &gTextTexture)
{
	//Free loaded images
	gButtonTexture.free();
	gTextTexture.free();

	TTF_CloseFont(gFont);
	gFont = nullptr;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = nullptr;
	gRenderer = nullptr;

	//Quit SDL subsystems
	//IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	SDL_Window *gWindow = nullptr;
	SDL_Renderer *gRenderer = nullptr;
	TTF_Font *gFont = nullptr;
	Texture gButtonTexture;
	Button gButtons[TOTAL_BUTTONS];
	TextureText gTextTexture;
	char **words = new char*[7];
	words[0] = "Create";
	words[1] = "Load";
	words[2] = "Save";
	words[3] = "Add";
	words[4] = "Remove";
	words[5] = "Undo";
	words[6] = "Print";

	if( !init(gWindow, gRenderer) ){
		std::cout << "Failed to initialize!\n";
	}
	else {
		if( !loadMedia(gRenderer, gButtonTexture, gButtons, gFont) ) {
			std::cout << "Failed to load media!\n";
		}
		else {	
			bool quit = false;
			SDL_Event e;
			while( !quit ) {
				while( SDL_PollEvent( &e ) != 0 ) {
					if( e.type == SDL_QUIT ) {
						quit = true;
					}
					for( int i = 0; i < TOTAL_BUTTONS; ++i ) {
						gButtons[ i ].handleEvent( &e, i );
					}
				}
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );
				for( int i = 0; i < TOTAL_BUTTONS; ++i )
				{
					gButtons[ i ].render(gRenderer, gButtonTexture, gTextTexture, words[i], gFont);
				}

				SDL_Rect menuInsideRect = {20, 20, 100, 25};
				SDL_SetRenderDrawColor(gRenderer, 0x57, 0x63, 0x5A, 0x00);
				SDL_RenderFillRect(gRenderer, &menuInsideRect);

				SDL_Rect menuRect = {20, 20, 100, SCREEN_HEIGHT - 40};
				SDL_SetRenderDrawColor(gRenderer, 0x33, 0x00, 0x00, 0x00);
				SDL_RenderDrawRect(gRenderer, &menuRect);

				SDL_Rect headerMenuRect = {20, 20, 100, 25};
				SDL_SetRenderDrawColor(gRenderer, 0x33, 0x00, 0x00, 0x00);
				SDL_RenderDrawRect(gRenderer, &headerMenuRect);

				SDL_Rect frame = {COORDINATE_SYSTEM_X, COORDINATE_SYSTEM_Y, COORDINATE_SYSTEM_WIDTH, COORDINATE_SYSTEM_HEIGHT};
				SDL_RenderDrawRect(gRenderer, &frame);

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
				for( int i = 0; i < COORDINATE_SYSTEM_HEIGHT; i += 4 )
				{
					SDL_RenderDrawPoint( gRenderer, COORDINATE_SYSTEM_WIDTH / 2 + COORDINATE_SYSTEM_X, i + COORDINATE_SYSTEM_Y );
				}

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
				for( int i = 0; i < COORDINATE_SYSTEM_WIDTH; i += 4 )
				{
					SDL_RenderDrawPoint( gRenderer, i + COORDINATE_SYSTEM_X, COORDINATE_SYSTEM_HEIGHT / 2 + COORDINATE_SYSTEM_Y);
				}

				SDL_RenderPresent( gRenderer );
			}
		}
	}
	close(gWindow, gRenderer, gButtonTexture, gFont, gTextTexture);

	return 0;
}