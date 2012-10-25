#include <opencv2/imgproc/imgproc.hpp>
#include "patchmatch.h"
#include <time.h>

PatchMatch::PatchMatch(cv::Mat &src, int patchSize, cv::Mat &dst, cv::Mat &mask, int maxRadius, float radiusRatio)
	: src(src.clone()), mask(mask), ps(patchSize), pr(patchSize / 2), pad(pr), iter(0)
{
	this->dst = (dst.data == 0) ? src.clone() : dst.clone();
	CV_Assert(this->src.data != 0 && this->dst.data != 0 && this->src.channels() == 3 && this->src.type() == CV_8UC3 && this->dst.type() == CV_8UC3 && ps % 2 == 1);

#if NDEBUG
	srand(time(NULL));
#endif

	float winSz = (float)(maxRadius < 0 ? std::max(src.cols, src.rows) : maxRadius);
	float alpha = radiusRatio < 0 ? .5f : radiusRatio;

	float tmp = winSz;
	while (tmp > 1.0) {
		R.push_back((int)tmp);
		tmp = winSz * pow(alpha, (int)R.size());
	}
}

PatchMatch::~PatchMatch()
{
}

void PatchMatch::initialize()
{
	nnf.create(dst.size(), CV_32SC3);
	nnf = cv::Scalar(1, 1, INT_MAX); // Boundary fix.

	bounds = cv::Mat::ones(dst.size(), CV_8U);
	bounds(cv::Rect(pad, pad, dst.cols - 2 * pad, dst.rows - 2*pad)) = 0;

	int maxErr = INT_MAX;

	for (int y = pad; y < src.rows - pad; ++y) {
		cv::Vec3i *nnfPtr = nnf.ptr<cv::Vec3i>(y);

		for (int x = pad; x < src.cols - pad; ++x) {
			do {
				nnfPtr[x][0] = rand() % (src.cols - 2 * pad) + pad; // X.
				nnfPtr[x][1] = rand() % (src.rows - 2 * pad) + pad; // Y.

#if EXCLUDE_RADIUS == 0
			} while (FALSE);
#elif EXCLUDE_RADIUS == 1
			} while (nnfPtr[x][0] == x && nnfPtr[x][1] == y);
#else
			} while (euclDist(nnfPtr[x][0], nnfPtr[x][1], x, y) < EXCLUDE_RADIUS);
#endif

			nnfPtr[x][2] = ssdError(src, x, y, dst, nnfPtr[x][0], nnfPtr[x][1], INT_MAX);
		}
	}
}

void PatchMatch::iterate()
{
	// SCANLINE ORDER.
	if (++iter % 2 == 1) {
		for (int y = pad; y < dst.rows - pad; ++y) {
			cv::Vec3i *nnfLinePtr = nnf.ptr<cv::Vec3i>(y);
			cv::Vec3i *nnfPrevLinePtr = nnf.ptr<cv::Vec3i>(y-1);

			for (int x = pad; x < dst.cols - pad; ++x) {
				cv::Point curPt = cv::Point(x, y);

				// PROPAGATION.
				// Find left and top neighbor's offset and shift them accordingly.
				cv::Point neighbor[2];
				neighbor[0] = cv::Point(nnfLinePtr[x-1][0] + 1, nnfLinePtr[x-1][1]); // Left neighbor's best match's right neighbor.
				neighbor[1] = cv::Point(nnfPrevLinePtr[x][0], nnfPrevLinePtr[x][1] + 1); // Top neighbor's best match's bottom neighbor.

				// Test them.
				int neighborErr[2];
				neighborErr[0] = ssdError(src, neighbor[0].x, neighbor[0].y, dst, x, y, nnfLinePtr[x][2]);
				neighborErr[1] = ssdError(src, neighbor[1].x, neighbor[1].y, dst, x, y, nnfLinePtr[x][2]);

				// Select the best one.
#if EXCLUDE_RADIUS == 0
				if (neighborErr[0] < nnfLinePtr[x][2]) {
					nnfLinePtr[x] = cv::Vec3i(neighbor[0].x, neighbor[0].y, neighborErr[0]);
				}
				if (neighborErr[1] < nnfLinePtr[x][2]) {
					nnfLinePtr[x] = cv::Vec3i(neighbor[1].x, neighbor[1].y, neighborErr[1]);
				}
#elif EXCLUDE_RADIUS == 1
				if (neighborErr[0] < nnfLinePtr[x][2] && neighbor[0] != curPt) {
					nnfLinePtr[x] = cv::Vec3i(neighbor[0].x, neighbor[0].y, neighborErr[0]);
				}
				if (neighborErr[1] < nnfLinePtr[x][2] && neighbor[1] != curPt) {
					nnfLinePtr[x] = cv::Vec3i(neighbor[1].x, neighbor[1].y, neighborErr[1]);
				}
#else
				if (neighborErr[0] < nnfLinePtr[x][2] && euclDist(neighbor[0], curPt) >= EXCLUDE_RADIUS) {
					nnfLinePtr[x] = cv::Vec3i(neighbor[0].x, neighbor[0].y, neighborErr[0]);
				}
				if (neighborErr[1] < nnfLinePtr[x][2] && euclDist(neighbor[1], curPt) >= EXCLUDE_RADIUS) {
					nnfLinePtr[x] = cv::Vec3i(neighbor[1].x, neighbor[1].y, neighborErr[1]);
				}
#endif

				// RANDOM SEARCH.
				for (unsigned int i = 0; i < R.size(); ++i) {
					// Determine clamped search window area.
					cv::Rect searchWnd(cv::Point(std::max(pad, nnfLinePtr[x][0] - R[i]), std::max(pad, nnfLinePtr[x][1] - R[i])),
						cv::Point(std::min(src.cols - pad, nnfLinePtr[x][0] + R[i] + 1), std::min(src.rows - pad, nnfLinePtr[x][1] + R[i] + 1)));

					// Find random point in given radius.
					cv::Point randPt(rand() % searchWnd.width + searchWnd.x, rand() % searchWnd.height + searchWnd.y);
					
					// Test its error distance.
					int randPtErr = ssdError(src, randPt.x, randPt.y, dst, x, y, nnfLinePtr[x][2]);

					// Update if this random guess is better.
#if EXCLUDE_RADIUS == 0
					if (randPtErr < nnfLinePtr[x][2]) {
#elif EXCLUDE_RADIUS == 1
					if (randPtErr < nnfLinePtr[x][2] && randPt != curPt) {
#else
					if (randPtErr < nnfLinePtr[x][2] && euclDist(randPt, curPt) >= EXCLUDE_RADIUS) {
#endif
						nnfLinePtr[x] = cv::Vec3i(randPt.x, randPt.y, randPtErr);
					}
				}
			}
		}

	// REVERSE SCANLINE ORDER.
	} else {
		for (int y = dst.rows - pad - 1; y >= pad; --y) {
			cv::Vec3i *nnfLinePtr = nnf.ptr<cv::Vec3i>(y);
			cv::Vec3i *nnfNextLinePtr = nnf.ptr<cv::Vec3i>(y+1);

			for (int x = dst.cols - pad - 1; x >= pad; --x) {
				cv::Point curPt = cv::Point(x, y);

				// PROPAGATION.
				// Find right and bottom neighbor's offset and shift them accordingly.
				cv::Point neighbor[2];
				neighbor[0] = cv::Point(nnfLinePtr[x+1][0] - 1, nnfLinePtr[x+1][1]);
				neighbor[1] = cv::Point(nnfNextLinePtr[x][0], nnfNextLinePtr[x][1] - 1);

				// Test them.
				int neighborErr[2];
				neighborErr[0] = ssdError(src, neighbor[0].x, neighbor[0].y, dst, x, y, nnfLinePtr[x][2]);
				neighborErr[1] = ssdError(src, neighbor[1].x, neighbor[1].y, dst, x, y, nnfLinePtr[x][2]);

				// Select the best one.
#if EXCLUDE_RADIUS == 0
				if (neighborErr[0] < nnfLinePtr[x][2]) {
					nnfLinePtr[x] = cv::Vec3i(neighbor[0].x, neighbor[0].y, neighborErr[0]);
				}
				if (neighborErr[1] < nnfLinePtr[x][2]) {
					nnfLinePtr[x] = cv::Vec3i(neighbor[1].x, neighbor[1].y, neighborErr[1]);
				}
#elif EXCLUDE_RADIUS == 1
				if (neighborErr[0] < nnfLinePtr[x][2] && neighbor[0] != curPt) {
					nnfLinePtr[x] = cv::Vec3i(neighbor[0].x, neighbor[0].y, neighborErr[0]);
				}
				if (neighborErr[1] < nnfLinePtr[x][2] && neighbor[1] != curPt) {
					nnfLinePtr[x] = cv::Vec3i(neighbor[1].x, neighbor[1].y, neighborErr[1]);
				}
#else
				if (neighborErr[0] < nnfLinePtr[x][2] && euclDist(neighbor[0], curPt) >= EXCLUDE_RADIUS) {
					nnfLinePtr[x] = cv::Vec3i(neighbor[0].x, neighbor[0].y, neighborErr[0]);
				}
				if (neighborErr[1] < nnfLinePtr[x][2] && euclDist(neighbor[1], curPt) >= EXCLUDE_RADIUS) {
					nnfLinePtr[x] = cv::Vec3i(neighbor[1].x, neighbor[1].y, neighborErr[1]);
				}
#endif

				// RANDOM SEARCH.
				for (unsigned int i = 0; i < R.size(); ++i) {
					// Determine clamped search window area.
					cv::Rect searchWnd(cv::Point(std::max(pad, nnfLinePtr[x][0] - R[i]), std::max(pad, nnfLinePtr[x][1] - R[i])),
						cv::Point(std::min(src.cols - pad, nnfLinePtr[x][0] + R[i] + 1), std::min(src.rows - pad, nnfLinePtr[x][1] + R[i] + 1)));

					// Find random point in given radius.
					cv::Point randPt(rand() % searchWnd.width + searchWnd.x, rand() % searchWnd.height + searchWnd.y);
					
					// Test its error distance.
					int randPtErr = ssdError(src, randPt.x, randPt.y, dst, x, y, nnfLinePtr[x][2]);

					// Update if this random guess is better.
#if EXCLUDE_RADIUS == 0
					if (randPtErr < nnfLinePtr[x][2]) {
#elif EXCLUDE_RADIUS == 1
					if (randPtErr < nnfLinePtr[x][2] && randPt != curPt) {
#else
					if (randPtErr < nnfLinePtr[x][2] && euclDist(randPt, curPt) >= EXCLUDE_RADIUS) {
#endif
						nnfLinePtr[x] = cv::Vec3i(randPt.x, randPt.y, randPtErr);
					}
				}
			}
		}
	}
}

void PatchMatch::iterate(int count)
{
	for (int i = 0; i < count; ++i) iterate();
}

int PatchMatch::ssdError(cv::Mat &src, int xs, int ys, cv::Mat &dst, int xd, int yd, int prevErr)
{
	if (bounds.ptr<uchar>(ys)[xs]) return INT_MAX;

	xs -= pad;
	ys -= pad;
	xd -= pad;
	yd -= pad;

	int err = 0;

	for (int y = 0; y < ps; ++y, ++ys, ++yd) {
		cv::Vec3b *srcPtr = src.ptr<cv::Vec3b>(ys);
		cv::Vec3b *dstPtr = dst.ptr<cv::Vec3b>(yd);
#if DEST_MASK == 1
		uchar *maskPtr = mask.ptr<uchar>(yd);
#endif

		for (int x = 0; x < ps; ++x, ++xs, ++xd) {
#if DEST_MASK == 1
			if (!maskPtr[xd]) continue; // Dest. pixel unknown.
#endif
			for (int i = 0; i < 3; ++i) {
				int channelErr = (int)srcPtr[xs][i] - (int)dstPtr[xd][i];
				err += channelErr * channelErr;
			}
		}

		if (err >= prevErr) return INT_MAX;
		xs -= ps;
		xd -= ps;
	}

	return err;
}

cv::Mat PatchMatch::getNnfImage()
{
	cv::Mat out = src.clone();

	for (int y = pad; y < src.rows - pad; ++y) {
		cv::Vec3b *outPtr = out.ptr<cv::Vec3b>(y);
		cv::Vec3i *nnfPtr = nnf.ptr<cv::Vec3i>(y);

		for (int x = pad; x < src.cols - pad; ++x) {
			outPtr[x] = src.ptr<cv::Vec3b>(nnfPtr[x][1])[nnfPtr[x][0]];
		}
	}

	return out;
}

cv::Mat PatchMatch::getNnf()
{
	cv::Mat nnf = this->nnf.clone();

	for (int y = 0; y < nnf.rows; ++y) {
		cv::Vec3i *linePtr = nnf.ptr<cv::Vec3i>(y);

		if (y < pad || y >= nnf.rows-1 - pad) {
			for (int x = 0; x < nnf.cols; ++x) {
				linePtr[x] = cv::Vec3i(x, y, 0);
			}
		} else {
			for (int x = 0; x < pad; ++x) {
				linePtr[x] = cv::Vec3i(x, y, 0);
				linePtr[nnf.cols-1 - x] = cv::Vec3i(nnf.cols-1 - x, y, 0);
			}
		}
	}

	return nnf;
}
