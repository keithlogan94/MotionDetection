//
// Title: Motion Detection Software
// Summary: Software that detects motion and displays a reticle around the object detected.
// 
// Last Update: 05/02/2017
// Copyright by Keith Becker 1017 (c)
// 
#include <iostream>
#include <string>
#include <ctime>
#include <BotLibrary.h>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <Windows.h>

int main(void) {
#ifdef _DEBUG
	std::cout << "Application started." << std::endl;
#endif
	std::cout << "Motion Detection Software written by Keith Becker" << 
		std::endl;

	//Get a handle to a window that will be used for screen capture
	//Ask the user at what frames per second will the screen be captured
	//Ask the user if he would rather capture the desktop screen
	//while the application is running...
	//capture the screen

	HWND window = { NULL };
	std::string answer = "";
	bool notCorrectAnswer;
	do {
		std::cout << "Do you want to capture the desktop window? (yes/no): ";
		std::cin >> answer;
		notCorrectAnswer = answer.compare("yes") != NULL && answer.compare("no") != NULL;
	} while (notCorrectAnswer);
	bool desktop = answer.compare("yes") == NULL;
	if (desktop) {//capture the desktop
		window = GetDesktopWindow();
	}
	else {//capture a different window
		std::cout << "captureing another window. Select the window that you want to capture. You have 5 seconds." << std::endl;
		Sleep(5000);
		window = GetForegroundWindow();
		std::cout << "the window has been selected.";
	}

	//Get frames per second to capture images
	bool inputVerifier{ false };
	short int framesPerSecond{ 24 };
	do {
		std::cout << "What fps do you want the stream to capture? (5-100): ";
		std::cin >> framesPerSecond;
		inputVerifier = framesPerSecond < 5 || framesPerSecond > 100;
	} while (inputVerifier);
	std::cout << "fps for captureing is " << (short int)framesPerSecond << std::endl;

	//Window should be selected
	//Start processing the window
	long unsigned int imageSaveNumber { 0 };

	//press escape key to stop
	clock_t lastCycleTime = clock();
	while (!(GetKeyState(VK_ESCAPE) & 0x8000)) {
		float ms = (float(clock() - lastCycleTime) / CLOCKS_PER_SEC) * 1000;
		if (ms * framesPerSecond > 1000) {
			lastCycleTime = clock();
#ifdef _DEBUG
			std::cout << ms << "ms" << " ";
			std::cout << 1000 / ms << "fps" << std::endl;
#endif //_DEBUG
			//record a screen capture and save it to a jpg
			BotLibrary::DetectedObject movingObj = BotLibrary::DetectMovingObject();

			//check to see if image is available
			if (movingObj.mousePosOfObj.x != NULL) {
				std::string fileName{ std::to_string(imageSaveNumber++) + ".jpg" };
				cv::imwrite(fileName.c_str(), movingObj.objectDetectedImage);
			}
		}
	}
	system("pause");
	return 0;
}
