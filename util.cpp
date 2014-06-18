#include "util.hpp"

using namespace std;
using namespace cv;
static double t1;
void timeMeasureStart(void)
{
    t1 = (double) getTickCount();
}

void timeMeasureOver(const std::string& str)
{
    t1 = ((double)getTickCount() - t1) / getTickFrequency();
    cout << str << "cost time: " << t1 * 1000 << " ms" << endl;
}
