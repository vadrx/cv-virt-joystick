#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;

// Global variables
Mat frame; //current frame

char keyboard; //input from keyboard

int cursor_loc[2]; // [0] - x, [1] - y

double x_center;
double y_center;

void mouseMotion(int event, int x, int y, int flags, void* param)
{
	switch (event)
	{
		case CV_EVENT_MOUSEMOVE:
			//writing coords in global array
			cursor_loc[0] = x,	cursor_loc[1] = y;
			break;
			
		case CV_EVENT_LBUTTONDOWN:
			cout << "you clicked" << endl;
			
			//printing coordinates
			cout << "CURSOR \t x: " << cursor_loc[0] << 
					"\t y: " << cursor_loc[1] << endl; 
			break;

		case CV_EVENT_LBUTTONUP:
			cout << "you unclicked" << endl;
			// here any code
			break;

		default:
			break;
	}
}

void drawRect()
{
	int shiftX = 50;
	int shiftY = 20;
	rectangle(frame, Point(cursor_loc[0] + shiftX, //point 1
				cursor_loc[1] - shiftY ),

				Point(cursor_loc[0] - shiftX, //point 2
				cursor_loc[1] + shiftY), 
				
				Scalar(0,0,255)); // color
}

void getDiffCoords(int x, int y)
{
	double x_diff = x_center - x;
	double y_diff = y_center - y;
	cout << "DIFF:\t X : " << x_diff << "\t Y: " << y_diff << endl;
}

void showFPS(VideoCapture capture)
{
	stringstream ss;
	double fps = capture.get(CAP_PROP_FPS);
	ss << fps;

	string frame_number_string = ss.str();
	putText(frame, frame_number_string.c_str(),Point(15, 15),
			FONT_HERSHEY_SIMPLEX, 0.5 , Scalar(255,0,0));
}

void processVideo(char* video_file_name) 
{
	//create the capture object
	VideoCapture capture(video_file_name);

	double width_w = capture.get(CAP_PROP_FRAME_WIDTH);
	double height_w = capture.get(CAP_PROP_FRAME_HEIGHT);

	x_center = width_w / 2;
	y_center = height_w / 2;

	cout << "Width: " << width_w << "\t Height: " << height_w << endl;
	cout << "X center: " << x_center << "\t Y center: " << y_center << endl;

	if(!capture.isOpened())
	{
		//error in opening the video input
		cerr << "Unable to open video file: " << video_file_name << endl;
		exit(EXIT_FAILURE);
	}
	//read input data. ESC or 'q' for quitting
	keyboard = 0;
	while( keyboard != 'q' && keyboard != 27 )
	{
		//read the current frame
		if(!capture.read(frame)) 
		{
			cerr << "Unable to read next frame.\nExiting..." << endl;
			exit(EXIT_FAILURE);
		}
		//get and print сurrent FPS 
		showFPS(capture);

		//check cursor pos and draw rectangle
		setMouseCallback("Frame", mouseMotion, video_file_name);
		drawRect();

		//show the current frame
		imshow("Frame", frame);

		//get and print diff coords of cursor and center window
		getDiffCoords(cursor_loc[0], cursor_loc[1]);

		//get the input from the keyboard
		keyboard = (char)waitKey( 30 );
	}
	//delete capture object
	capture.release();
}

int main(int argc, char* argv[])
{
	// Run command example:
	// ./build/vidcv test/dog1.mp4
	// or 
	// ./build/vidcv test/dog2.mp4
	if(argc != 2) 
	{
		cerr <<"Incorret input list\nExiting..." << endl;
		return EXIT_FAILURE;
	}
  	//create GUI windows
	namedWindow("Frame");
  	//input data coming from a video
	processVideo(argv[1]);
	//destroy GUI windows
	destroyAllWindows();
	return EXIT_SUCCESS;
}