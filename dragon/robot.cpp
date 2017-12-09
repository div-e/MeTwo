#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <memory>
#include <exception>
#include <thread>

#include "controls.hpp"
#include "tcp_client.hpp"
#include "robot.hpp"

using namespace std;

metwo::robot::robot(char* ip) : serverAddr(ip)
{
}

void metwo::robot::run()
{
    try
    {
        init();
        cout << "Initialized\nConnecting...\n";

        auto tcp = make_shared<tcp_client>(serverAddr, 6000);
        cout << "Connected\n";

        thread con(control_thread, tcp);
        //thread cam(webcam_thread, tcp);

        con.join();
        cout << "Control thread started\n";
        //cam.join();
        //cout << "Webcam thread started\n";
    }
    catch (exception& ex)
    {
        cerr << ex.what() << endl;
        return;
    }
}

void metwo::robot::control_thread(shared_ptr<tcp_client> tcp)
{
    void (*controls[10])(void) = { forward, backward, stop, left, right, stop_turning, tiltUp, tiltDown, panRight, panLeft};

    unsigned char buffer[1];
    while (true)
    {
        tcp->read(buffer, 1);
        cout << (buffer[0]+0) << "\n";
        controls[buffer[0] - 1]();
    }
}

void metwo::robot::webcam_thread(shared_ptr<tcp_client> tcp)
{
    cv::Mat frame;
    cv::VideoCapture cap;

    cout << "Trying to open camera..." << endl;
    cap.open("/dev/video0");

    while (!cap.isOpened())
    {
        this_thread::sleep_for(chrono::seconds(10));
        cout << "Retrying to open camera..." << endl;
        cap.open("/dev/video0");
    }
    cout << "Camera is opened" << endl;

    int fourcc = cv::VideoWriter::fourcc('Y', 'U', 'Y', 'V');
    cap.set(cv::CAP_PROP_FPS, 30);
    cap.set(cv::CAP_PROP_FOURCC, fourcc);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

    cout << "FPS: " << cap.get(cv::CAP_PROP_FPS) << endl;
    cout << "Width: " << cap.get(cv::CAP_PROP_FRAME_WIDTH) << endl;
    cout << "Height: " << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << endl;
    cout << "Target Four CC: " << fourcc << endl;
    cout << "Actual Four CC: " << static_cast<int>(cap.get(cv::CAP_PROP_FOURCC)) << endl;
    cout << "To RGB: " << static_cast<bool>(cap.get(cv::CAP_PROP_CONVERT_RGB)) << endl;

    vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(50);
    vector<unsigned char> buf;
    unsigned char size[2];

    while (true)
    {
        cap.read(frame);
        imencode(".jpg", frame, buf, params);
        int s = buf.size();
        size[1] = s;
        size[0] = s >> 8;
        tcp->write(size, 2);
        tcp->write(&buf[0], s);
    }
}


