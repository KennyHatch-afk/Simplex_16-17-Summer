/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2017/05
----------------------------------------------*/
#ifndef __BASICXSYSTEM_H_
#define __BASICXSYSTEM_H_

#include "BasicX\System\Folder.h"
#include <random>

namespace BasicX
{

//System Class
class BasicXDLL BasicXSystem
{
	bool m_bWindowFullscreen = false; // Window Fullscreen
	bool m_bWindowBorderless = false; // Window Borderless
	bool m_bMultithreaded = false; // Experimental: mutithreaded system
	bool m_bConsoleWindow = false; // Experimental: Console window init

	int m_nWindowWidth = 1280; // Window width
	int m_nWindowHeight = 720; // Window height
	int m_nWindowX = 0; // Window Position X
	int m_nWindowY = 0; // Window Position Y

	int m_nFPS = 60; //Frames per Second
	uint m_uFrameCount = 0; //Frames Passed

	static BasicXSystem* m_pInstance; // Singleton

	String m_sWindowName = "BasicX"; // Window Name
	String m_sAppName = "Application";//Name of the Application

	DWORD m_dTimerStart = 0; //Start time of the program
	DWORD m_dStartingTime = 0; //Start time of the program
	DWORD m_dLastFPS = 0; //Last time the time was called

	std::vector<DWORD> m_lClock;//clocks list
	
public:
	Folder* m_pFolder;
	BTO_OPTIONS m_RenderingContext;
	/*
	USAGE: Singleton accessor
	ARGUMENTS:
	OUTPUT:
	*/
	static BasicXSystem* GetInstance();
	
	/*
	USAGE: Singleton Release
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	static void ReleaseInstance(void); 

	/*
	USAGE:
	ARGUMENTS: ---
	OUTPUT:
	*/
	String GetAppName(void);

	/*
	USAGE:
	ARGUMENTS:
	OUTPUT: ---
	*/
	void SetWindowBorderless(bool a_bBorderless = true);
	
	/*
	USAGE:
	ARGUMENTS: ---
	OUTPUT:
	*/
	bool IsWindowBorderless(void);

	/*
	USAGE: Will set the window to fullscreen mode in the desired resolution
	ARGUMENTS:
		BTO_RESOLUTIONS a_Resolution = BTO_RESOLUTIONS::HD_1280X720 -> resolution of the screen
	OUTPUT: ---
	*/
	void SetWindowFullscreen(BTO_RESOLUTIONS a_Resolution = BTO_RESOLUTIONS::RES_C_1280x720_16x9_HD);
	
	/*
	USAGE:
	ARGUMENTS: ---
	OUTPUT:
	*/
	bool IsWindowFullscreen(void);

	/*
	USAGE:
	ARGUMENTS:
	OUTPUT: ---
	*/
	void SetWindowResolution(BTO_RESOLUTIONS a_Resolution = BTO_RESOLUTIONS::RES_C_1280x720_16x9_HD);
	
	/*
	USAGE:
	ARGUMENTS:
	OUTPUT: ---
	*/
	void SetWindowWidth(int a_nWidth);
	
	/*
	USAGE:
	ARGUMENTS: ---
	OUTPUT:
	*/
	int GetWindowWidth(void);

	/*
	USAGE:
	ARGUMENTS:
	OUTPUT: ---
	*/
	void SetWindowHeight(int a_nHeight);
	
	/*
	USAGE:
	ARGUMENTS: ---
	OUTPUT:
	*/
	int GetWindowHeight(void);
	
	/*
	USAGE:
	ARGUMENTS:
	OUTPUT: ---
	*/
	void SetWindowX(int a_nX);
	
	/*
	USAGE:
	ARGUMENTS: ---
	OUTPUT:
	*/
	int GetWindowX(void);
	
	/*
	USAGE:
	ARGUMENTS:
	OUTPUT: ---
	*/
	void SetWindowY(int a_nY);
	
	/*
	USAGE:
	ARGUMENTS: ---
	OUTPUT:
	*/
	int GetWindowY(void);

	/*
	USAGE: Calculates the render area in the gl window
	ARGUMENTS:
	int* a_nPosX -> output; Gets the position x
	int* a_nPosY -> output; Gets the position y
	OUTPUT: ---
	*/
	void GetWindowPosition(int* a_nPosX, int* a_nPosY);

	/*
	USAGE: Calculates the size of the window
	ARGUMENTS:
	int* a_nWidth -> output; Gets the width
	int* a_nHeight -> output; Gets the height
	OUTPUT: ---
	*/
	void GetWindowSize(int* a_nWidth, int* a_nHeight);
		
	/*
	USAGE:
	ARGUMENTS:
	OUTPUT:
	*/
	void SetWindowName(String a_sWindowName);
	
	/*
	USAGE:
	ARGUMENTS: ---
	OUTPUT:
	*/
	String GetWindowName(void);
	
	/*
	USAGE:Sets the system to be multi-threaded <<<EXPERIMENTAL>>>
	ARGUMENTS:
	OUTPUT: ---
	*/
	void SetThreaded(bool a_bMultithreaded);
	
	/*
	USAGE: Asks the system if its multi-threaded
	ARGUMENTS: ---
	OUTPUT:
	*/
	bool GetThreaded(void);
	
	/*
	USAGE: Asks for the value of m_bConsole
	ARGUMENTS: ---
	OUTPUT:
	*/
	bool GetUsingConsole(void);

	/*
	USAGE: Asks for the value of m_bConsole
	ARGUMENTS:
	OUTPUT: ---
	*/
	void SetUsingConsole(bool a_bUsing);

	/*
	USAGE: This will return the proportions of the window size in the system, from bottom to top of the window the value is 1,
		from left to right the value is the output of this function.
	ARGUMENTS: ---
	OUTPUT:
	*/
	float GetWindowRatio(void);

	/*
	USAGE:
	ARGUMENTS: ---
	OUTPUT:
	*/
	int GetFPS(void);
	
	/*
	USAGE: Updates the system
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Update(void);

	/*
	USAGE: Checks the time difference between start clock and current call
	ARGUMENTS:
	OUTPUT: ---
	*/
	void StartClock(unsigned int a_nClock = 0);//Starts a time count for the specified clock
	
	/*
	USAGE: Gets the time difference between the last time this method was called for this particular clock
	ARGUMENTS:
	OUTPUT:
	*/
	double GetDeltaTime(unsigned int a_nClock = 0);

	/*
	USAGE: Adds a clock to the list and return said clock's index
	ARGUMENTS: ---
	OUTPUT: uint -> index of the new clock;
	*/
	uint GenClock(void);

	/*
	USAGE: returns true when the count is up, only one clock available
	ARGUMENTS:
		float a_fTime -> total time to countdown
		bool a_bRepeat = false -> repeat the count once its done?
	OUTPUT:
	*/
	bool CountDown(float a_fTime, bool a_bRepeat = false);

private:
	/*
	USAGE: Constructor
	ARGUMENTS: ---
	OUTPUT:
	*/
	BasicXSystem(void);

	/*
	USAGE: Copy Constructor
	ARGUMENTS:
	OUTPUT:
	*/
	BasicXSystem(BasicXSystem const& other);
	
	/*
	USAGE: Copy Assignment Operator
	ARGUMENTS:
	OUTPUT:
	*/
	BasicXSystem& operator=(BasicXSystem const& other);
	
	/*
	USAGE: Destructor
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	~BasicXSystem(void);

	/*
	USAGE: Release Memory
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Release(void);
	
	/*
	USAGE: Initialize variables
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Init(void);
};

}
#include "BasicX\System\GLSystem.h"

#endif //__BasicXSystem_H__