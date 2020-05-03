#include <bcm2835.h>
#include <stdint.h>

extern "C"
{
#include "tft.h"
}
#include "ST7789.h"
#include "bmp.h"

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;


int main(int argc, char **argv) {
	if(argc != 1) {
		cout << "Usage: " << argv[0] << endl;
		return 0;
	}

	VideoCapture capture(0, cv::CAP_ANY);
	if(!capture.isOpened()) {
		cerr << "Error: Unable to open camera" << endl;
		return 1;
	}

	capture.set(cv::CAP_PROP_FRAME_WIDTH, 640.0);
	capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480.0);

	if (!bcm2835_init()) {
		cerr << "Error: bcm2835 initialization failed" << endl;
		return 1;
	}

	TFT_init_board();
	TFT_hard_reset();
	STcontroller_init();
	TFT_SetBacklightPWMValue(200);

	Mat frame, scaledFrame;
	uint16_t picture[1][PICTURE_PIXELS];
	while(1) {
		capture.read(frame);
		if(frame.empty()) {
			cerr << "Error: Frame is empty" << endl;
			break;
		}

//		cout << "channels: " << frame.channels() << endl;
//		cout << "depth: " << frame.depth() << endl;
//		cout << "size: " << frame.size << endl;
//		cout << "total: " << frame.total() << endl;

		resize(frame, scaledFrame, Size(320, 240));

		for(int i = 0; i < PICTURE_PIXELS; i++) {
//			cout << scaledFrame.data[i*3+0] << "," << scaledFrame.data[i*3+1] << "," << scaledFrame.data[i*3+2] << " ";
			picture[0][i]  = (uint16_t) ((scaledFrame.data[i*3+2] / 8) << 11);
			picture[0][i] |= (uint16_t) ((scaledFrame.data[i*3+1] / 4) << 5);
			picture[0][i] |= (uint16_t) ((scaledFrame.data[i*3+0] / 8));
		}

		STcontroller_Write_Picture(&picture[0][0], PICTURE_PIXELS);
	}

	TFT_SetBacklightPWMValue(0);
	bcm2835_close();
 	return 1;
}
