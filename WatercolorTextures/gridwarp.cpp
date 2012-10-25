#include "warp/imgwarp_mls.h"
#include "warp/imgwarp_mls_rigid.h"
#include "warp/imgwarp_mls_similarity.h"
#include "warp/imgwarp_piecewiseaffine.h"
#include "gridwarp.h"

void GridWarp::setType(int type)
{
	switch(type) {
	case RIGID:
	case SIMILARITY:
	case AFFINE:
		this->type = type;
		break;
	default:
		THROW(E_InvParVal)
	}
}

void GridWarp::setBaseImage(cv::Mat *image)
{
	src = image->clone();
}

void GridWarp::setPoints(const std::vector<std::vector<cv::Point>> *pointsHistory, int firstPointIndex)
{
	points = pointsHistory;
	this->firstPointIndex = firstPointIndex;
}

bool GridWarp::keyListener(int key)
{
	bool response = false;
	
	switch (key) {
	// W
	case 87:
	case 119:
		warp();
		response = true;
		*mask = opacity; // always show after the warp
		break;
	}

	response = response || StaticImage::keyListener(key);
	if (response) invalidate();

	return response;
}

void GridWarp::warp()
{
	ASSERT(type != -1 && src.data != 0 && points != 0)

	ImgWarp_MLS *method;

	switch(type) {
	case RIGID:
		method = new ImgWarp_MLS_Rigid();
		break;
	case SIMILARITY:
		method = new ImgWarp_MLS_Similarity();
		break;
	case AFFINE:
		method = new ImgWarp_PieceWiseAffine();
		((ImgWarp_PieceWiseAffine*)method)->backGroundFillAlg = ImgWarp_PieceWiseAffine::BGMLS;
		break;
	}

	int t = static_cast<int>(points->size())-1;
	std::vector<cv::Point> srcPts((*points)[t-1].begin()+firstPointIndex, (*points)[t-1].end());
	std::vector<cv::Point> dstPts((*points)[t].begin()+firstPointIndex, (*points)[t].end());

	// !!! TODO - prostudovat dany clanek a doladit nastaveni techto hodnot !!!
	method->alpha = 1.0;
	method->gridSize = 5;

	cv::Mat tmp = method->setAllAndGenerate(src, srcPts, dstPts, src.cols, src.rows);
	tmp.copyTo(*target);

	delete method;
}