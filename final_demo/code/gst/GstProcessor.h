//CHATGPT I asked chat gpt to make a class for me. Almost all function definitions are written by me. With help of https://gstreamer.freedesktop.org/documentation/?gi-language=c
#ifndef GSTPROCESSOR_H
#define GSTPROCESSOR_H

#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <glib.h>

class GstProcessor {
public:
    GstProcessor();
    ~GstProcessor();

    bool initialize(int argc, char *argv[]);
    bool bus_call();
    int get_dx();
    int get_dy();
    float get_radx();
    float get_rady();
    void set_XDistance(float xDist);
    void set_YDistance(float yDist);

private:
    static GstFlowReturn new_sample(GstElement *sink);

    static void process_sample(GstSample *sample);

    static const int WIDTH = 640;
    static const int HEIGHT = 480;
    static const int HWIDTH = WIDTH/2;
    static const int HHEIGHT = HEIGHT/2;

    GstElement *pipeline, *source, *encoder, *capfilt, *sink;
    GstBus *bus;
    
    static int dx;
    static int dy;
    static float radx;
    static float rady;
    static float XDistance; // for radians calculation
    static float YDistance; // for radians calculation
};

#endif // GSTPROCESSOR_H
//CHATGPT
