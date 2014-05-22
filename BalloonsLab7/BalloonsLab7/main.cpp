/*
=================
main.cpp
Main entry point for the Card application
=================
*/

#include "GameConstants.h"
#include "GameResources.h"
#include "cD3DManager.h"
#include "cD3DXSpriteMgr.h"
#include "cD3DXTexture.h"
#include "cPigeon.h"
#include "cSprite.h"
#include "cExplosion.h"
#include "cXAudio.h"
#include "cD3DXFont.h"

using namespace std;

HINSTANCE hInst; // global handle to hold the application instance
HWND wndHandle; // global variable to hold the window handle

// Get a reference to the DirectX Manager
static cD3DManager* d3dMgr = cD3DManager::getInstance();

// Get a reference to the DirectX Sprite renderer Manager 
static cD3DXSpriteMgr* d3dxSRMgr = cD3DXSpriteMgr::getInstance();

//Vector containing pigeons
vector<cPigeon*> aPigeon;
vector<cPigeon*>::iterator iter;
vector<cPigeon*>::iterator index;

//Sets the values of the spawn variables
float pigeonSpawnTimer = 0.0f;
float pigeonSpawnDelay = 1.5f;

//Sets the values of the timer variables
float timer = 30.0f;
char timerStr[100];

//Sets the conditions of the game scenes
bool mainMenu = true;
bool mainGame = false;
bool endMenu = false;

//The images used for the buttons
cPigeon playButton;
cPigeon startexitButton;
cPigeon endexitButton;
cPigeon replayButton;
cPigeon menuButton;

//Stores the text for the menus
char gMainMenuStr[100];
char gEndMenuStr[100];

//Initial number of pigeons
int numPigeons = 0;

//The bounds of the screen
RECT clientBounds;

TCHAR szTempOutput[30];

//Sets the initial values for pigeons hit
bool gHit = false;
int gPigeonsHit = 0;
char gPigeonsHitStr[50];


D3DXVECTOR3 expPos;
list<cExplosion*> gExplode;

//The sounds used in the game
cXAudio gExplodeSound;
cXAudio gBackgroundSound;

//The textures used for the pigeon sprites
cD3DXTexture* pigeonTextures[3];
char* pigeonTxtres[] = {"Images\\Pigeon2.png","Images\\Pigeon3.png","Images\\explosion.png"};

//The textures used for the start screen button images
cD3DXTexture* startTextures[3];
char* startTxtres[] = {"Images\\PlayButtonV3.png", "Images\\ExitButtonV3.png", "Images\\explosion.png"};

//The textures used for the end screen button images
cD3DXTexture* endTextures[4];
char* endTxtres[] = {"Images\\ReplayButtonV3.png", "Images\\MenuButtonV3.png", "Images\\ExitButtonV3.png", "Images\\explosion.png"};

/*
==================================================================
* LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
* LPARAM lParam)
* The window procedure
==================================================================
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Check any available messages from the queue
	switch (message)
	{
		case WM_LBUTTONDOWN:
			{
				POINT mouseXY;
				mouseXY.x = LOWORD(lParam);
				mouseXY.y = HIWORD(lParam);
				
				expPos = D3DXVECTOR3((float)mouseXY.x,(float)mouseXY.y, 0.0f);

				//Game Level
				if(mainGame == true && (mainMenu == false && endMenu == false))
				{
					iter = aPigeon.begin();
					while (iter != aPigeon.end() && !gHit)
					{
						if ( (*iter)->insideRect((*iter)->getBoundingRect(),mouseXY))
						{
							//Get explosion
							OutputDebugString("Hit!\n");
							gHit = true;
							expPos = (*iter)->getSpritePos();
							gExplode.push_back(new cExplosion(expPos,pigeonTextures[2]));
						
							//Play explosion sound and delete pigeon, decrement number of pigeons and increment pigeons hit
							gExplodeSound.playSound(L"Sounds\\explosion.wav",false);
							iter = aPigeon.erase(iter);
							gPigeonsHit++;
							numPigeons--;
							sprintf_s( gPigeonsHitStr, 50, "Pigeons Hit : %d", gPigeonsHit);
						}
						else
						{
							++iter;
						}
					}

					gHit = false;
					return 0;
				}

				//Main Menu
				if(mainMenu == true && (mainGame == false && endMenu == false))
				{
					//Play Button
					if(playButton.insideRect(playButton.getBoundingRect(),mouseXY))
					{						
						//Resets all values
						timer = 30.0f;
						pigeonSpawnTimer = 0.0f;
						gPigeonsHit = 0;
						numPigeons = 0;

						//Changes scene
						mainGame = true;
						mainMenu = false;
						return 0;
					}

					//Exit Button
					if(startexitButton.insideRect(startexitButton.getBoundingRect(),mouseXY))
					{
						//Exits the game and closes window
						PostQuitMessage(0);
						return 0;
					}
				}
				
				//End Menu
				if(endMenu == true && (mainGame == false && mainMenu == false))
				{
					//Replay Button
					if(replayButton.insideRect(replayButton.getBoundingRect(),mouseXY))
					{						
						//Resets the values
						timer = 30.0f;
						pigeonSpawnTimer = 0.0f;
						gPigeonsHit = 0;
						numPigeons = 0;
						
						//Changes scene
						mainGame = true;
						endMenu = false;
						return 0;
					}

					//Menu Button
					if(menuButton.insideRect(menuButton.getBoundingRect(),mouseXY))
					{		
						//Changes scene
						mainMenu = true;
						endMenu = false;
						return 0;
					}

					//Exit Button
					if(endexitButton.insideRect(endexitButton.getBoundingRect(),mouseXY))
					{
						//Exits the game and closes window
						PostQuitMessage(0);
						return 0;
					}
				}
				
			}

		case WM_KEYDOWN:
			{
				//Return Key
				if (wParam == VK_RETURN)
				{
					//Main Menu
					if(mainMenu == true && (mainGame == false && endMenu == false))
					{
						//Resets all values
						timer = 30.0f;
						pigeonSpawnTimer = 0.0f;
						gPigeonsHit = 0;
						numPigeons = 0;

						//Changes scene
						mainGame = true;
						mainMenu = false;
						return 0;
					}

					//End Menu
					if(endMenu == true && (mainGame == false && mainMenu == false))
					{
						//Resets all values
						timer = 30.0f;
						pigeonSpawnTimer = 0.0f;
						gPigeonsHit = 0;
						numPigeons = 0;
						
						//Changes screen
						mainGame = true;
						endMenu = false;
						return 0;
					}
				}

				//"M" Key
				if(wParam == 'M')
				{
					//End Menu
					if(endMenu == true && (mainGame == false && mainMenu == false))
					{
						//Changes scene
						mainMenu = true;
						endMenu = false;
						return 0;
					}
				}

				//Escape Key
				if(wParam == VK_ESCAPE)
				{
					//Exit the game and close application
					PostQuitMessage(0);
					return 0;
				}
				return 0;
			}

		case WM_CLOSE:
			{
				//Exit the Game and close applictaion
				PostQuitMessage(0);
				return 0;
			}

		case WM_DESTROY:
			{
				//Exit the Game and close applictaion
				PostQuitMessage(0);
				return 0;
			}
	}
	// Always return the message to the default window
	// procedure for further processing
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/*
==================================================================
* bool initWindow( HINSTANCE hInstance )
* initWindow registers the window class for the application, creates the window
==================================================================
*/
bool initWindow( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;
	// Fill in the WNDCLASSEX structure. This describes how the window
	// will look to the system
	wcex.cbSize = sizeof(WNDCLASSEX); // the size of the structure
	wcex.style = CS_HREDRAW | CS_VREDRAW; // the class style
	wcex.lpfnWndProc = (WNDPROC)WndProc; // the window procedure callback
	wcex.cbClsExtra = 0; // extra bytes to allocate for this class
	wcex.cbWndExtra = 0; // extra bytes to allocate for this instance
	wcex.hInstance = hInstance; // handle to the application instance
	wcex.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_MyWindowIcon)); // icon to associate with the application
	wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_GUNSIGHT));// the default cursor
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); // the background color
	wcex.lpszMenuName = NULL; // the resource name for the menu
	wcex.lpszClassName = "Pigeons"; // the class name being created
	wcex.hIconSm = LoadIcon(hInstance,"PigeonIcon.ico"); // the handle to the small icon

	RegisterClassEx(&wcex);
	// Create the window
	wndHandle = CreateWindow("Pigeons",			// the window class to use
							 "Pesky Pigeons",			// the title bar text
							WS_OVERLAPPEDWINDOW,	// the window style
							CW_USEDEFAULT, // the starting x coordinate
							CW_USEDEFAULT, // the starting y coordinate
							800, // the pixel width of the window
							600, // the pixel height of the window
							NULL, // the parent window; NULL for desktop
							NULL, // the menu for the application; NULL for none
							hInstance, // the handle to the application instance
							NULL); // no values passed to the window
	// Make sure that the window handle that is created is valid
	if (!wndHandle)
		return false;
	// Display the window on the screen
	ShowWindow(wndHandle, SW_SHOW);
	UpdateWindow(wndHandle);
	return true;
}

/*
==================================================================
// This is winmain, the main entry point for Windows applications
==================================================================
*/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	// Initialize the window
	if ( !initWindow( hInstance ) )
		return false;
	// called after creating the window
	if ( !d3dMgr->initD3DManager(wndHandle) )
		return false;
	if ( !d3dxSRMgr->initD3DXSpriteMgr(d3dMgr->getTheD3DDevice()))
		return false;

	// Grab the frequency of the high def timer
	__int64 freq = 0;				// measured in counts per second;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	float sPC = 1.0f / (float)freq;			// number of seconds per count

	__int64 currentTime = 0;				// current time measured in counts per second;
	__int64 previousTime = 0;				// previous time measured in counts per second;

	float numFrames   = 0.0f;				// Used to hold the number of frames
	float timeElapsed = 0.0f;				// cumulative elapsed time

	GetClientRect(wndHandle,&clientBounds);

	float fpsRate = 1.0f/25.0f;

	D3DXVECTOR3 apigeonPos;

	//====================================================================
	// Load three textures for the pigeons; left, right, & explosion
	//====================================================================
	for (int txture = 0; txture < 3; txture++)
	{
		pigeonTextures[txture] = new cD3DXTexture(d3dMgr->getTheD3DDevice(), pigeonTxtres[txture]);
	}

	//Load main menu image textures
	for (int txture = 0; txture < 3; txture++)
	{
		startTextures[txture] = new cD3DXTexture(d3dMgr->getTheD3DDevice(), startTxtres[txture]);
	}

	//Load end menu image textures
	for (int txture = 0; txture < 4; txture++)
	{
		endTextures[txture] = new cD3DXTexture(d3dMgr->getTheD3DDevice(), endTxtres[txture]);
	}

	// Initial starting position for pigeon
	D3DXVECTOR3 pigeonPos;

	//Position of menu buttons
	D3DXVECTOR3 playPos;
	D3DXVECTOR3 startexitPos;
	D3DXVECTOR3 endexitPos;
	D3DXVECTOR3 replayPos;
	D3DXVECTOR3 menuPos;

	//Setting the button positions and textures
	playPos = D3DXVECTOR3(340, 490, 0);
	playButton.setSpritePos(playPos);
	playButton.setTexture(startTextures[0]);
	
	startexitPos = D3DXVECTOR3(580, 490, 0);
	startexitButton.setSpritePos(startexitPos);
	startexitButton.setTexture(startTextures[1]);
	
	endexitPos = D3DXVECTOR3(590, 490, 0);
	endexitButton.setSpritePos(endexitPos);
	endexitButton.setTexture(endTextures[2]);
	
	replayPos = D3DXVECTOR3(170, 490, 0);
	replayButton.setSpritePos(replayPos);
	replayButton.setTexture(endTextures[0]);
	
	menuPos = D3DXVECTOR3(380, 490, 0);
	menuButton.setSpritePos(menuPos);
	menuButton.setTexture(endTextures[1]);
	
	/* initialize random seed: */
	srand ( (unsigned int)time(NULL) );

	//Arrays which contain the spawn point coordinates
	float xspawnPoints[2] = {0.0f, 700.0f};
	float yspawnPoints[5] = {100.0f, 200.0f, 300.0f, 400.0f, 500.0f};

	
	LPDIRECT3DSURFACE9 aSurface;				// the Direct3D surface
	LPDIRECT3DSURFACE9 startSurface;
	LPDIRECT3DSURFACE9 endSurface;
	LPDIRECT3DSURFACE9 theBackbuffer = NULL;  // This will hold the back buffer
	
	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );

	// Create the background surface
	aSurface = d3dMgr->getD3DSurfaceFromFile("Images\\Field1.png");

	//Create the start and end surfaces
	startSurface = d3dMgr->getD3DSurfaceFromFile("Images\\StartScreen.png");
	endSurface = d3dMgr->getD3DSurfaceFromFile("Images\\EndScreen.png");

	// load custom font
	cD3DXFont* pigeonFont = new cD3DXFont(d3dMgr->getTheD3DDevice(),hInstance, "biondi");

	//Background Sound
	gBackgroundSound.playSound(L"Sounds\\bird_chirping_1.wav",true);

	//Sets the positions of the text 
	RECT hitPos;
	SetRect(&hitPos, 50, 10, 550, 100);

	RECT timerPos;
	SetRect(&timerPos, 450, 10, 750, 100);

	RECT mainTextPos;
	SetRect(&mainTextPos, 250, 330, 750, 480);

	RECT endTextPos;
	SetRect(&endTextPos, 230, 380, 750, 480);


	QueryPerformanceCounter((LARGE_INTEGER*)&previousTime);

	while( msg.message!=WM_QUIT )
	{
		// Check the message queue
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{

			// Game code goes here
			QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
			float dt = (currentTime - previousTime)*sPC;

			// Accumulate how much time has passed.
			timeElapsed += dt;
				
			/*
			==============================================================
			| Update the postion of the pigeons and check for collisions
			==============================================================
			*/
			if(timeElapsed > fpsRate)
			{
				//Game Level
				if(mainGame == true && (mainMenu == false && endMenu == false))
				{
					//Generate random number of pigeons to spawn between 1-3
					int numSpawn = (rand() % 3 + 1);
					
					//Check if should spawn
					if(pigeonSpawnTimer >= pigeonSpawnDelay)
					{
						//For the number of pigeons to spawn
						for(int loop = 0; loop < numSpawn; loop++)
						{
							//Choose random spawn point and set sprite position
							pigeonPos = D3DXVECTOR3((xspawnPoints[(rand() % 2)]), (yspawnPoints[(rand() % 5)]),0);
							aPigeon.push_back(new cPigeon());
							aPigeon[numPigeons]->setSpritePos(pigeonPos);
														
							//Get sprite X coordinates
							aPigeon[numPigeons]->getSpritePos().x;
							float sPos = aPigeon[numPigeons]->getSpritePos().x;
							
							//If coordinate is 0 or 700, set correct translation and correct texture
							if(sPos == 0.0f)
							{
								aPigeon[numPigeons]->setTranslation(D3DXVECTOR2(200.0f,0.0f));
								aPigeon[numPigeons]->setTexture(pigeonTextures[1]);
							}
							else if(sPos == 700.0f)
							{
								aPigeon[numPigeons]->setTranslation(D3DXVECTOR2(-200.0f,0.0f));
								aPigeon[numPigeons]->setTexture(pigeonTextures[0]);
							}
						
							//Reset the spawn timer and increment number of pigeons 
							pigeonSpawnTimer = 0.0f;
							numPigeons++;
						}
					}	

					for(iter = aPigeon.begin(); iter != aPigeon.end(); ++iter)
					{
						(*iter)->update(timeElapsed);			// update pigeon
						apigeonPos = (*iter)->getSpritePos();  // get the position of the current pigeon
						/*
						==============================================================
						| Check for boundry collision and change pigeon direction
						==============================================================
						*/
						if (apigeonPos.x>(clientBounds.right - 60) || apigeonPos.x < 2)
						{
							(*iter)->setTranslation((*iter)->getTranslation()*(-1));
						}
						/*
						==============================================================
						| Check each pigeon against each other for collisions
						==============================================================
						*/				
						for(index = aPigeon.begin(); index != aPigeon.end(); ++index)
						{
							if ((*iter)->collidedWith((*iter)->getBoundingRect(),(*index)->getBoundingRect()))
							{
								// if a collision occurs change the direction of each pigeon that has collided
								OutputDebugString("Collision!!");
								(*iter)->setTranslation((*iter)->getTranslation()*(-1));
								(*index)->setTranslation((*index)->getTranslation()*(-1));
							}
						}
					}
					//Begin the renderer and update the surface 
					d3dMgr->beginRender();
					theBackbuffer = d3dMgr->getTheBackBuffer();
					d3dMgr->updateTheSurface(aSurface, theBackbuffer);
					d3dMgr->releaseTheBackbuffer(theBackbuffer);
				
					//Draw the pigeons
					d3dxSRMgr->beginDraw();
					vector<cPigeon*>::iterator iterB = aPigeon.begin();
					for(iterB = aPigeon.begin(); iterB != aPigeon.end(); ++iterB)
					{
						d3dxSRMgr->setTheTransform((*iterB)->getSpriteTransformMatrix());  
						d3dxSRMgr->drawSprite((*iterB)->getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
					}
					
					//Draw the explosion
					list<cExplosion*>::iterator iter = gExplode.begin();
					while(iter != gExplode.end())
					{
						if((*iter)->isActive() == false)
						{
							iter = gExplode.erase(iter);
						}
						else
						{
							(*iter)->update(timeElapsed);
							d3dxSRMgr->setTheTransform((*iter)->getSpriteTransformMatrix());  
							d3dxSRMgr->drawSprite((*iter)->getTexture(),&((*iter)->getSourceRect()),NULL,NULL,0xFFFFFFFF);
							++iter;
						}
					}
					
					//Update the spawn timer and timer values
					pigeonSpawnTimer += 0.04f;
					timer -= 0.04f;

					//Change scene
					if(timer <= 0)
					{
						mainGame = false;
						mainMenu = false;
						endMenu = true;
					}
					
					//Display and print Text
					sprintf_s( gPigeonsHitStr, 50, "Pigeons Hit : %d", gPigeonsHit);
					sprintf_s( timerStr, 100, "Time : %.02f", timer);
					pigeonFont->printText(gPigeonsHitStr,hitPos);
					pigeonFont->printText(timerStr,timerPos);

					//End Draw and render
					d3dxSRMgr->endDraw();
					d3dMgr->endRender();
					timeElapsed = 0.0f;
				}


				//Main Menu
				if(mainMenu == true && (mainGame == false && endMenu == false))
				{
					//Update the menu buttons
					playButton.update(timeElapsed);
					startexitButton.update(timeElapsed);
					menuButton.update(timeElapsed);

					//Begin the renderer and update the surface 
					d3dMgr->beginRender();
					theBackbuffer = d3dMgr->getTheBackBuffer();
					d3dMgr->updateTheSurface(startSurface, theBackbuffer);
					d3dMgr->releaseTheBackbuffer(theBackbuffer);
				
					//Draw the buttons
					d3dxSRMgr->beginDraw();

					d3dxSRMgr->setTheTransform(playButton.getSpriteTransformMatrix());
					d3dxSRMgr->drawSprite(playButton.getTexture(), NULL, NULL, NULL, 0xFFFFFFFF);

					d3dxSRMgr->setTheTransform(startexitButton.getSpriteTransformMatrix());
					d3dxSRMgr->drawSprite(startexitButton.getTexture(), NULL, NULL, NULL, 0xFFFFFFFF);
										
					//Display and print text
					sprintf_s( gMainMenuStr, 100, "Use the mouse to aim\nand press the left\nmouse button to fire.");
					pigeonFont->printText(gMainMenuStr,mainTextPos);
					
					//End draw and render
					d3dxSRMgr->endDraw();
					d3dMgr->endRender();
					timeElapsed = 0.0f;
				}

				//End Menu
				if(endMenu == true && (mainGame == false && mainMenu == false))
				{
					//Clear the pigeons and set the number of pigeons to 0
					aPigeon.clear();
					numPigeons = 0;

					//Update the buttons
					endexitButton.update(timeElapsed);
					replayButton.update(timeElapsed);

					//Begin the renderer and update the surface 
					d3dMgr->beginRender();
					theBackbuffer = d3dMgr->getTheBackBuffer();
					d3dMgr->updateTheSurface(endSurface, theBackbuffer);
					d3dMgr->releaseTheBackbuffer(theBackbuffer);
				
					//Draw the menu buttons
					d3dxSRMgr->beginDraw();

					d3dxSRMgr->setTheTransform(replayButton.getSpriteTransformMatrix());
					d3dxSRMgr->drawSprite(replayButton.getTexture(), NULL, NULL, NULL, 0xFFFFFFFF);

					d3dxSRMgr->setTheTransform(menuButton.getSpriteTransformMatrix());
					d3dxSRMgr->drawSprite(menuButton.getTexture(), NULL, NULL, NULL, 0xFFFFFFFF);

					d3dxSRMgr->setTheTransform(endexitButton.getSpriteTransformMatrix());
					d3dxSRMgr->drawSprite(endexitButton.getTexture(), NULL, NULL, NULL, 0xFFFFFFFF);
											
					//Print and display the text
					sprintf_s( gEndMenuStr, 100, "You killed: %d" " pigeons", gPigeonsHit);
					pigeonFont->printText(gEndMenuStr,endTextPos);

					//End draw and render
					d3dxSRMgr->endDraw();
					d3dMgr->endRender();
					timeElapsed = 0.0f;
				}

			}

			previousTime = currentTime;
		}
	}
	d3dxSRMgr->cleanUp();
	d3dMgr->clean();
	return (int) msg.wParam;
}
