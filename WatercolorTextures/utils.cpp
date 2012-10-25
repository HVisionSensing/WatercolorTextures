#include "utils.h"

void blend(cv::Mat &src1, cv::Mat &src2, cv::Mat &dst, cv::Mat &weight, int itype, double min, double max)
{
	ASSERT(src1.type() == src2.type() && src1.size() == src2.size())

	std::vector<cv::Mat> ch1, ch2, res;
	cv::split(src1, ch1);
	cv::split(src2, ch2);

	cv::Mat w;
	weight.convertTo(w, itype);
	w = (w-min) / (max-min);
	cv::Mat wComp(1.0-w);

	for (int i = 0; i < 3; ++i) {
		cv::Mat tmp1, tmp2, tmpRes;
		ch1[i].convertTo(tmp1, itype);
		ch2[i].convertTo(tmp2, itype);
		cv::multiply(wComp, tmp1, tmp1);
		cv::multiply(w, tmp2, tmp2);
		tmpRes = tmp1 + tmp2;
		res.push_back(tmpRes);
	}

	cv::Mat tmp;
	cv::merge(res, tmp);
	tmp.convertTo(dst, src1.type());
}

std::vector<std::string> explode(const std::string &string, const char &delim)
{
	std::string next("");
	std::vector<std::string> result;

	for (std::string::const_iterator it = string.begin(); it != string.end(); it++) {
		if (*it == delim) {
			if (next.length() > 0) {
				result.push_back(next);
				next = "";
			}
		} else {
			next += *it;
		}
	}

	return result;
}
