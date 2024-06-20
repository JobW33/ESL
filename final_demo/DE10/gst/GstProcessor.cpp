#include "GstProcessor.h"

GstProcessor::GstProcessor() : pipeline(nullptr), source(nullptr), capfilt(nullptr), sink(nullptr), bus(nullptr) {}

GstProcessor::~GstProcessor() {
    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(GST_OBJECT(pipeline));
    }
    gst_object_unref(bus);
}

bool GstProcessor::initialize(int argc, char *argv[]) {
    gst_init(&argc, &argv);

    pipeline = gst_pipeline_new("YUV processing");
    source = gst_element_factory_make("v4l2src", "webcam source");
    capfilt = gst_element_factory_make("capsfilter", "YUY2 data format");
    sink = gst_element_factory_make("appsink", "app sink");

    if (!pipeline || !source || !capfilt || !sink) {
        g_print("One element could not be created. Exiting.\n");
        return false;
    }

    GstCaps *caps = gst_caps_new_simple("video/x-raw",
                                        "format", G_TYPE_STRING, "YUY2",
                                        "framerate", GST_TYPE_FRACTION, 30, 1,
                                        "pixel-aspect-ratio", GST_TYPE_FRACTION, 1, 1,
                                        "width", G_TYPE_INT, WIDTH,
                                        "height", G_TYPE_INT, HEIGHT,
                                        NULL);
    g_object_set(G_OBJECT(capfilt), "caps", caps, NULL);

    g_object_set(G_OBJECT(source), "device", "/dev/video7", NULL);
    g_object_set(sink, "emit-signals", TRUE, NULL);
    g_signal_connect(sink, "new-sample", G_CALLBACK(new_sample), this);

    gst_bin_add_many(GST_BIN(pipeline), source, capfilt, sink, NULL);
    gst_element_link_many(source, capfilt, sink, NULL);
    
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    
    bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));

    return true;
}

int GstProcessor::get_dx(){
  return dx;
}

int GstProcessor::get_dy(){
  return dy;
}

float GstProcessor::get_radx(){
  return radx;
}

float GstProcessor::get_rady(){
  return rady;
}

void GstProcessor::set_XDistance(float xDist){
  XDistance = xDist;
}

void GstProcessor::set_YDistance(float yDist){
  YDistance = yDist;
}

GstFlowReturn GstProcessor::new_sample(GstElement *sink) {
    GstSample *sample;
    g_signal_emit_by_name(sink, "pull-sample", &sample);
    if (sample) {
        process_sample(sample);
        gst_sample_unref(sample);
        return GST_FLOW_OK;
    }
    return GST_FLOW_ERROR;
}

void GstProcessor::process_sample(GstSample *sample) {
    GstBuffer *buffer = gst_sample_get_buffer(sample);
    GstMapInfo info;
    gst_buffer_map(buffer, &info, GST_MAP_READ);

    int total = 0;
    int total_x = 0;
    int total_y = 0;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x += 2) {
            int idx = y * WIDTH * 2 + x * 2;
            int U = info.data[idx + 1];
            int V = info.data[idx + 3];

            int add = ((U < 140) && (V < 100)) ? 1 : 0;
            total_x += add * x;
            total_y += add * y;
            total += add;
        }
    }

    // I think ternary operators are faster so I use them as much as possible.
    bool enough = (total > ((HEIGHT * WIDTH) >> 8));
    dx = enough ? (HWIDTH - total_x/total) : 0;
    dy = enough ? (HHEIGHT - total_y/total) : 0;
    radx = dx / XDistance;
    rady = dy / YDistance;

    gst_buffer_unmap(buffer, &info);
}

bool GstProcessor::bus_call() {
//CHATGPT
    GstMessage *   msg = gst_bus_timed_pop_filtered(bus, 0, static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
    
    if (msg != NULL) {
        GError *err;
        gchar *debug_info;

        switch (GST_MESSAGE_TYPE(msg)) {
            case GST_MESSAGE_ERROR:
                gst_message_parse_error(msg, &err, &debug_info);
                g_printerr("Error received from element %s: %s\n",
                           GST_OBJECT_NAME(msg->src), err->message);
                g_printerr("Debugging information: %s\n",
                           debug_info ? debug_info : "none");
                g_clear_error(&err);
                g_free(debug_info);
                return false;
            case GST_MESSAGE_EOS:
                g_print("End-Of-Stream reached.\n");
                return false;
            default:
                // We should not reach here because we only asked for ERRORs and EOS
                g_printerr("Unexpected message received.\n");
                break;
        }
        gst_message_unref(msg);
    }
//CHATGPT

    return true;
}

int GstProcessor::dx = 0;
int GstProcessor::dy = 0;
float GstProcessor::radx = 0;
float GstProcessor::rady = 0;
float GstProcessor::XDistance = 600;
float GstProcessor::YDistance = 600;
