#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include "opencv2/highgui.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

int main(int arg_c, char** arg_v) {
    if (arg_c == 1) {
        cout << "Usage: ./camera-stream -c n (number of cameras) ... (camera numbers) -r (record) -s (stream)" << endl;
        return 0;
    }

    vector<VideoCapture> caps;
    vector<VideoWriter> writers;
    vector<string> files;
    vector<Mat> frames;

    bool stream = false;
    bool record = false;

    for (int i = 0; i < arg_c; i++) {
        if (strcmp(arg_v[i], "-r") == 0) {
            record = true;
        }
        else if (strcmp(arg_v[i], "-s") == 0) {
            stream = true;
        }
        else if (strcmp(arg_v[i], "-c") == 0 && arg_c > ++i) {
            for (int j = 1; j <= strtol(arg_v[i], nullptr, 0); j++) {
                cout << "Trying Cam " << arg_v[i + j] << "..." << endl;
                VideoCapture cap(strtol(arg_v[i + j], nullptr, 0));

                cap.set(cv::CAP_PROP_FRAME_WIDTH, 850);
                cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

                if (!cap.isOpened()) {
                    cout << "Failed to open camera " << arg_v[i + j] << endl;
                    continue;
                }

                cout << "Camera " << arg_v[i + j] << " opened successfully" << endl;

                caps.push_back(cap);
                files.push_back("save" + to_string(j - 1) + ".avi");
                writers.emplace_back(files[j - 1], VideoWriter::fourcc('a', 'v', 'c', '1'), 20, Size(caps[j - 1].get(cv::CAP_PROP_FRAME_WIDTH), caps[j - 1].get(cv::CAP_PROP_FRAME_HEIGHT)), true);
                frames.emplace_back();
            }
        }
    }

    while (true) {
        for (int i = 0; i < caps.size(); i++) {
            caps[i] >> frames[i];

            if (stream) {
                imshow("Frame" + to_string(i), frames[i]);
            }

            if (record) {
                writers[i].write(frames[i]);
            }
        }

        if (char(waitKey(10)) == 27) {
            break;
        }
    }

    return 0;
}
