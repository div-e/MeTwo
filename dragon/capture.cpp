
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

double ClockToMili(clock_t time)
{
    return static_cast<double>(time) / CLOCKS_PER_SEC * 1000.0;
}

int main(int argc, char **argv)
{
    cv::Mat frame;
    cv::VideoCapture cap;
    cap.open(argv[1]);
    if (!cap.isOpened())
    {
        cerr << "Unable to open camera\n";
        return EXIT_FAILURE;
    }

    int fourcc = cv::VideoWriter::fourcc('Y', 'U', 'Y', 'V');
    cap.set(cv::CAP_PROP_FPS, 30);
    cap.set(cv::CAP_PROP_FOURCC, fourcc);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    //cap.set(CAP_PROP_CONVERT_RGB, false);
    cout << "FPS: " << cap.get(cv::CAP_PROP_FPS) << endl;
    cout << "Width: " << cap.get(cv::CAP_PROP_FRAME_WIDTH) << endl;
    cout << "Height: " << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << endl;
    cout << "Target Four CC: " << fourcc << endl;
    cout << "Actual Four CC: " << static_cast<int>(cap.get(cv::CAP_PROP_FOURCC)) << endl;
    cout << "To RGB: " << cap.get(cv::CAP_PROP_CONVERT_RGB) << endl;

    vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(20);
    vector<uchar> buf;
    //clock_t readTime;
    //clock_t encodeTime;
    clock_t begin;
    clock_t end;
    int count = 1000;

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(6000);
    address.sin_addr.s_addr = htonl((((((127 << 8) | 0) << 8) | 0) << 8) | 1);
    if (connect(sock, (sockaddr *)&address, sizeof(address)) < 0)
    {
        cout << "Failed to connect" << endl;
        return 2;
    }
    uchar stop[1] = { 0 };

    for (int i = 0; i < count; i++)
    {
        begin = clock();
        cap.read(frame);
        end = clock();

        cout << "Read Time: " << ClockToMili(end - begin) << endl;
        //readTime += (end - begin);

        begin = clock();
        imencode(".jpg", frame, buf, params);
        end = clock();
        cout << "Encode Time: " << ClockToMili(end - begin) << endl;
        //encodeTime += (end - begin);

        cout << "Buffer size: " << buf.size() << endl;
        write(sock, &buf[0], buf.size());
        //write(sock, stop, 1);
    }
    //cout << "Avg Read Time: " << ClockToMili(readTime) / count << endl;
    //cout << "Avg Encode Time: " << ClockToMili(encodeTime) / count << endl;


    return 0;
}