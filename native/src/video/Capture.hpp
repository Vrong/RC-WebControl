#ifndef CAPTURE_H
#define CAPTURE_H

#include "opencv2/opencv.hpp"


#include <unistd.h>
#include <string>

using namespace cv;

class Capture
{



private:
	int m_camera;
	int m_quality = 85;
	bool m_blackandwhite = false;
	bool m_equalization = false;
	Mat cur_image, prec_image, iframe;
	std::vector<uchar> buf;

	int m_height, m_width;

	VideoCapture cap;

	std::vector<uchar> encodeImage(std::string ext, int source);
	void applyFilters(cv::Mat &src, cv::Mat &dst);
	std::vector<int> getJpegParams();

public:

	enum Type { FRAME, IFRAME };

	Capture();
	Capture(int camera);
	Capture(int camera, int width, int height);

	void setSize(int width, int height);
	void setJpegQuality(int quality);
	void setBW(bool bw);
	void setEqualization(bool equ);

	void updateImage();
	void calculateIFrame();

	Mat getImageMat();
	Mat getIFrameMat();
	void saveImage(char dest[]);
	void saveIFrame(char dest[]);

	std::vector<uchar> getJpeg();
	std::vector<uchar> getPng();
	std::vector<uchar> getJpeg(int source);
	std::vector<uchar> getPng(int source);

	void show();
	void show(int source);


};


#endif
