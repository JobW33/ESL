#include "GstProcessor.h"

GstProcessor::GstProcessor() : pipeline(nullptr), source(nullptr), capfilt(nullptr), sink(nullptr), bus(nullptr) {}

GstProcessor::~GstProcessor() {
    if (pipeline) { // Destroy the pipeline if it's still there.
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(GST_OBJECT(pipeline));
    }
    // Destroy the bus.
    gst_object_unref(bus);
}

bool GstProcessor::initialize(int argc, char *argv[]) {
    gst_init(&argc, &argv);

    // Make a pipeline with three elements
    pipeline = gst_pipeline_new("YUV processing");
    source = gst_element_factory_make("v4l2src", "webcam source");        // A Source element.
    capfilt = gst_element_factory_make("capsfilter", "YUY2 data format"); // A filter element to get the correct data format.
    sink = gst_element_factory_make("appsink", "app sink");               // A sink element to process the frames.

    if (!pipeline || !source || !capfilt || !sink) {
        g_print("One element could not be created. Exiting.\n");
        return false;
    }

    // Set the data format we expect.
    GstCaps *caps = gst_caps_new_simple("video/x-raw",
                                        "format", G_TYPE_STRING, "YUY2",
                                        "framerate", GST_TYPE_FRACTION, 30, 1,
                                        "pixel-aspect-ratio", GST_TYPE_FRACTION, 1, 1,
                                        "width", G_TYPE_INT, WIDTH,
                                        "height", G_TYPE_INT, HEIGHT,
                                        NULL);
    g_object_set(G_OBJECT(capfilt), "caps", caps, NULL);

    // Set the source element to the camera.
    g_object_set(G_OBJECT(source), "device", "/dev/video7", NULL);
    g_object_set(sink, "emit-signals", TRUE, NULL);

    // Set the callback function for the sink.
    g_signal_connect(sink, "new-sample", G_CALLBACK(new_sample), NULL); 

    // Connect all elements of the pipeline.
    gst_bin_add_many(GST_BIN(pipeline), source, capfilt, sink, NULL);
    gst_element_link_many(source, capfilt, sink, NULL);
    
    // Start the pipeline.
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    
    // Get a bus element to listen to.
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
    // Navigate Gstreamers data structures in order to get to a frame.
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

    int total = 0;    // The total amount of green pixels there are.
    int total_x = 0;  // The summed x coordinates of all green pixels.
    int total_y = 0;  // The summed y coordinates of all green pixels.

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x += 2) {  // Loop through the entire frame. x += 2 because |Y||U| |Y||V|.
            int idx = y * WIDTH * 2 + x * 2;  // Get the current pixel position in the array.
            int U = info.data[idx + 1];       // Get the U component (skip the y component).
            int V = info.data[idx + 3];       // Get the V component (skip the y u y components).

            int add = ((U < 140) && (V < 100)) ? 1 : 0;   // This checks has been found to work in the python script.
            total_x += add * x;
            total_y += add * y;
            total += add;
        }
    }
    
    // If there are enough pixels green (i.e. there is a sufficiently large green object on screen) calculate the center of mass.
    bool enough = (total > ((HEIGHT * WIDTH) >> 8));
    
    // Note: here we use H-WIDTH and H-HEIGTH. Which are half of the width and height respectively.
    dx = enough ? (HWIDTH - total_x/total) : 0;     // The centre of mass x component in pixels from the centre of the frame.
    dy = enough ? (HHEIGHT - total_y/total) : 0;    // The centre of massi y component in pixels from the centre of the frame.

    // Update the radx and rady class members.
    radx = dx / XDistance;  // This is an approximation for the arctan(dx / some_const).
    rady = dy / YDistance;  // This is an approximation for the arctan(dy / some_const).

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

// Initialize the class members.
int GstProcessor::dx = 0;
int GstProcessor::dy = 0;
float GstProcessor::radx = 0;
float GstProcessor::rady = 0;
float GstProcessor::XDistance = 600;
float GstProcessor::YDistance = 600;
