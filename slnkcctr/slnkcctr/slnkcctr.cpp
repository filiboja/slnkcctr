// Exit by pressing Escape

#include "stdafx.h"

// c
#include <ctime> // clock, CLOCKS_PER_SEC

// std::
#include <iostream> // cout, endl
#include <iomanip> // setiosflags, setprecision
#include <string> // string
#include <vector> // vector

// cv::
#include <opencv2/core/core.hpp> // Mat
#include <opencv2/highgui/highgui.hpp> // VideoCapture, namedWindow, imshow
#include <opencv2/imgproc/imgproc.hpp> // resize

#include "CapMode.h" // CapMode
#include "CaptureProperties.h" // CaptureProperties
#include "DetectorSlinky.h" // DetectorColor
#include "program_options.h" // po, loadConfigFile

static int delay = 50;

enum {
	SUCCESS = 0,
	ERROR_SOURCE_OPEN = 1,
	ERROR_UNKNOWN_OPTION = 2,
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<typename T>& t);

int main(int argc, char *argv[]) {
	// Constants
	const char * const configFilename = "slnkcctr.ini";
	const std::string sourceWinname = "Source";
	const std::string controlsWinname = "Controls";
	const double defaultSourceFps = 15.0;
	const int KEY_ESC = 27;
	const int KEY_PAUSE = 32;

	// Format `double` output
	std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3);

	// Basic options
	typedef std::vector<std::string> ConfigFiles;
	ConfigFiles configFiles;

	po::options_description optionsBasic("Basic");
	optionsBasic.add_options()
		("help", "produce help message")
		("config.filename", po::value<ConfigFiles>(&configFiles), "configuration file")
	;

	// Source options
	CapMode sourceCapMode = CAP_MODE_DEVICE;
	int sourceCapDevice = 0;
	std::string sourceCapFilename = "";
	int sourceFrameWidth = 640;
	int sourceFrameHeight = 480;
	double sourceFps = defaultSourceFps;
	bool sourceShow = true;

	po::options_description optionsSource("Source");
	optionsSource.add_options()
		("source.mode,m", po::value<CapMode>(&sourceCapMode)->default_value(sourceCapMode), "mode (d[evice] or f[ile])")
		("source.device,d", po::value<int>(&sourceCapDevice)->default_value(sourceCapDevice), "source capture device (only used in device mode)")
		("source.filename,f", po::value<std::string>(&sourceCapFilename), "source video filename (only used in file mode)")
		("source.width", po::value<int>(&sourceFrameWidth), "frame width (leave empty for original)")
		("source.height", po::value<int>(&sourceFrameHeight), "frame height (leave empty for original)")
		("source.fps", po::value<double>(&sourceFps), "frames per second (leave empty for original)")
		("source.show", po::value<bool>(&sourceShow)->default_value(sourceShow), "show source video stream")
	;

	// Detector options
	int detectorFrameWidth = 640;
	int detectorFrameHeight = 480;
	bool detectorShow = true;
	std::string detectorConfig0;
	std::string detectorConfig1;

	po::options_description optionsDetector("Detector");
	optionsDetector.add_options()
		("detector.width", po::value<int>(&detectorFrameWidth), "frame width (leave empty for original)")
		("detector.height", po::value<int>(&detectorFrameHeight), "frame height (leave empty for original)")
		("detector.show", po::value<bool>(&detectorShow)->default_value(detectorShow), "show detector video stream")
		("detector.config.0", po::value<std::string>(&detectorConfig0), "first detector configuration file")
		("detector.config.1", po::value<std::string>(&detectorConfig1), "second detector configuration file")
	;

	po::options_description options("Options");
	options.add(optionsBasic).add(optionsSource).add(optionsDetector);

	po::variables_map vm;

	// Load options from command line
	try {
		po::store(po::parse_command_line(argc, argv, options), vm);
	} catch (po::unknown_option& e) {
		std::cerr << e.what() << std::endl;
		return ERROR_UNKNOWN_OPTION;
	}

	// Load options from configuration file
	loadConfigFile(configFilename, options, vm);

	po::notify(vm);

	// Load options from configuration files specified in `vm`
	for (ConfigFiles::const_iterator it = configFiles.begin(); it != configFiles.end(); it++) {
		loadConfigFile(*it, options, vm);
	}

	po::notify(vm);

	if (vm.count("help")) {
		std::cout << options << std::endl;
		return SUCCESS;
	}

	// Print instructions
	std::cout << "slnkcctr" << std::endl;
	std::cout << "Press Escape to exit." << std::endl;

	// Open source video stream
	cv::VideoCapture cap;
	std::cout << "Source capture mode: " << sourceCapMode << std::endl;
	switch (sourceCapMode) {
	case CAP_MODE_DEVICE:
		std::cout << "Source device: " << sourceCapDevice << std::endl;
		cap = cv::VideoCapture(sourceCapDevice);
		break;
	case CAP_MODE_FILE:
		std::cout << "Source filename: " << sourceCapFilename << std::endl;
		cap = cv::VideoCapture(sourceCapFilename);
		break;
	}
	if (!cap.isOpened()) {
		std::cerr << "Could not open source capture." << std::endl;
		return ERROR_SOURCE_OPEN;
	}

	// Set source frame dimensions
	if (vm.count("source.width")) {
		if (!cap.set(CV_CAP_PROP_FRAME_WIDTH, (double)sourceFrameWidth)) {
			std::cerr << "Failed to set source frame width." << std::endl;
			std::cerr << "Desired width: " << sourceFrameWidth << std::endl;
			std::cerr << "Actual width: " << (int)cap.get(CV_CAP_PROP_FRAME_WIDTH) << std::endl;
		}
	}
	if (vm.count("source.height")) {
		if (!cap.set(CV_CAP_PROP_FRAME_HEIGHT, (double)sourceFrameHeight)) {
			std::cerr << "Failed to set source frame height." << std::endl;
			std::cerr << "Desired height: " << sourceFrameHeight << std::endl;
			std::cerr << "Actual height: " << (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT) << std::endl;
		}
	}

	// Update source frame dimensions
	sourceFrameWidth = (int)cap.get(CV_CAP_PROP_FRAME_WIDTH);
	sourceFrameHeight = (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	// FPS
	if (!vm.count("source.fps")) {
		sourceFps = cap.get(CV_CAP_PROP_FPS);
		if (sourceFps <= 0.0) {
			std::cerr << "Failed to get FPS from source video stream." << std::endl;
			std::cerr << "Setting source FPS to default value: " << defaultSourceFps << std::endl;
			sourceFps = defaultSourceFps;
		}
	}
	assert(sourceFps > 0.0);
	delay = (int)(1000.0 / sourceFps);
	std::cout << "Desired FPS: " << sourceFps << std::endl;
	std::cout << "Frame delay: " << delay << std::endl;
	std::cout << "Actual FPS: " << 1000.0 / delay << std::endl;

	// Print source capture info
	CaptureProperties capProps;
	std::cout << "Source capture properties:" << std::endl;
	capProps.print(cap);

	// Set detector frame dimensions
	if (!vm.count("detector.width")) {
		detectorFrameWidth = sourceFrameWidth;
	}
	if (!vm.count("detector.height")) {
		detectorFrameHeight = sourceFrameHeight;
	}

	// Initialize detector
	cv::Size detectorFrameSize = cv::Size(detectorFrameWidth, detectorFrameHeight);
	DetectorSlinky detector(detectorFrameSize, detectorConfig0, detectorConfig1);

	// Source window
	if (sourceShow) {
		cv::namedWindow(sourceWinname, cv::WINDOW_NORMAL);
		cv::resizeWindow(sourceWinname, sourceFrameWidth, sourceFrameHeight);
	}

	// Controls window
	cv::namedWindow(controlsWinname, cv::WINDOW_AUTOSIZE);
	cv::createTrackbar("Delay", controlsWinname, &delay, 1000);

	// Main loop
	int key = 0;
	assert(key != KEY_ESC);
	bool pause = false;
	cv::Mat frameSource;
	clock_t clockBegin = clock();
	while (key != KEY_ESC) {
		if (key == KEY_PAUSE) {
			pause = !pause;
		}
		if (!pause) {
			cap >> frameSource;
			if (frameSource.empty()) {
				std::cout << "Empty source frame." << std::endl;
				break;
			}
			if (sourceShow) {
				//cv::imshow(sourceWinname, frameSource);
			}
		}
		FrameAnnotation annotation = detector.detect(frameSource);
		if (sourceShow) {
			//cv::imshow(sourceWinname, frameSource);
			cv::Mat annotationImg = cv::Mat::zeros(frameSource.size(), CV_8UC3);
			annotation.draw(annotationImg);
			cv::Mat frameAnnotated = frameSource + annotationImg;
			cv::imshow(sourceWinname, frameAnnotated);
		}
		clock_t clockEnd = clock();
		assert(clockEnd >= clockBegin);
		clock_t clockDiff = clockEnd - clockBegin;
		int msecDiff = clockDiff * 1000 / CLOCKS_PER_SEC;
		int delayCur = delay - msecDiff;
		if (delayCur <= 0) {
			delayCur = 1;
		}
		clockBegin = clockEnd;
		assert(delayCur > 0); // Don't wait forever
		key = cv::waitKey(delayCur);
	}

	return SUCCESS;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<typename T>& t)
{
	for (std::vector<T>::const_iterator it = t.begin(); it != t.end(); it++) {
		out << *it << std::endl;
	}
	return out;
}
