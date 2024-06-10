#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <glib.h>

#define WIDTH 640
#define HEIGHT 480

/* The appsink has received a buffer */
static GstFlowReturn new_sample (GstElement *sink) {
  GstSample *sample;

  /* Retrieve the buffer */
  g_signal_emit_by_name (sink, "pull-sample", &sample);
  if (sample) {
    GstBuffer *buffer =  gst_sample_get_buffer(sample);
    GstMapInfo info;
    gst_buffer_map(buffer, &info, GST_MAP_READ);
    
    // Center of mass calculation.
    int total = 0;
    int total_x = 0;
    int total_y = 0;
    
    for(int y = 0; y < HEIGHT; y++)
    {
      for(int x = 0; x < WIDTH; x++)
      {
        int idx = y * WIDTH * 3 + x * 3;
        if(idx < info.size)
        {
          int B = info.data[idx];
          int G = info.data[idx + 1];
          int R = info.data[idx + 2];
          
          // if the current pixel abides these rules then it gets counted to the center of mass calculation.
          int add = ((B > 200) && (B > 0.8 * (G + R))) ? 1 : 0;
          total_x = total_x + add * x;
          total_y = total_y + add * y;
          total   = total   + add;
        }
        else
        {
          g_print("Ran out of info.size: x = %d, y = %d\n", x, y);
          y = WIDTH;
          x = HEIGHT;
        }
      }
    }
    
    // if there were enough data points collected we calculate the centre of mass otherwise just return the centre of the screen.
    if(total > ((HEIGHT * WIDTH) >> 8))
    {
      // should change this the difference from the middle of the screen.
      g_print("%d, %d\n", total_x/total, total_y/total);
    }
    else
    {
      // should change this to 0, 0
      g_print("%d, %d\n", WIDTH/2, HEIGHT/2); 
    }

    gst_sample_unref (sample);
    return GST_FLOW_OK;
  }

  return GST_FLOW_ERROR;
}


static gboolean
bus_call (GstBus     *bus,
          GstMessage *msg,
          gpointer    data)
{
  GMainLoop *loop = (GMainLoop *) data;

  switch (GST_MESSAGE_TYPE (msg)) {

    case GST_MESSAGE_EOS:
      g_print ("End of stream\n");
      g_main_loop_quit (loop);
      break;

    case GST_MESSAGE_ERROR: {
      gchar  *debug;
      GError *error;

      gst_message_parse_error (msg, &error, &debug);
      g_free (debug);

      g_printerr ("Error: %s\n", error->message);
      g_error_free (error);

      g_main_loop_quit (loop);
      break;
    }
    default:
      break;
  }

  return TRUE;
}


int
main (int   argc,
      char *argv[])
{
  GMainLoop *loop;

  GstElement *pipeline, *source, *encoder, *capfilt, *decoder, *sink;
  GstBus *bus;
  guint bus_watch_id;
  guint timeout_id;

  /* Initialisation */
  gst_init (&argc, &argv);

  loop = g_main_loop_new (NULL, FALSE);

  /* Create gstreamer elements */
  pipeline = gst_pipeline_new ("YUV storing");
  source   = gst_element_factory_make ("v4l2src",       "webcam source");
  encoder  = gst_element_factory_make ("videoconvert",  "video encoder");
  capfilt  = gst_element_factory_make ("capsfilter",    "jpeg data format");
  sink     = gst_element_factory_make ("appsink",       "app sink");

  if (!pipeline || !source || !encoder || !capfilt || !sink) {
    g_printerr ("One element could not be created. Exiting.\n");
    return -1;
  }

  /* Set up the pipeline */

  GstCaps *caps = gst_caps_new_simple("video/x-raw",
     "format", G_TYPE_STRING, "BGR",
     "framerate", GST_TYPE_FRACTION, 30, 1,
     "pixel-aspect-ratio", GST_TYPE_FRACTION, 1, 1,
     "width", G_TYPE_INT, WIDTH,
     "height", G_TYPE_INT, HEIGHT,
     NULL);
  g_object_set(G_OBJECT(capfilt), "caps", caps, NULL);

  /* Set the source device */
  g_object_set(G_OBJECT(source), "device", "/dev/video0", NULL);

  /* Configure appsink */
  g_object_set (sink, "emit-signals", TRUE, NULL);
  g_signal_connect (sink, "new-sample", G_CALLBACK (new_sample), NULL);

  /* we add a message handler */
  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
  gst_object_unref (bus);

  /* we add all elements into the pipeline */
  gst_bin_add_many (GST_BIN (pipeline), source, encoder, capfilt, sink, NULL);

  /* we link the elements together */
  gst_element_link_many (source, encoder, capfilt, sink, NULL);

  /* Set the pipeline to "playing" state*/
  g_print ("Now playing\n");
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

  /* Iterate */
  g_print ("Running...\n");
  g_main_loop_run (loop);

  /* Out of the main loop, clean up nicely */
  g_print ("Returned, stopping playback\n");
  gst_element_set_state (pipeline, GST_STATE_NULL);

  g_print ("Deleting pipeline\n");
  gst_object_unref (GST_OBJECT (pipeline));
  g_source_remove (bus_watch_id);
  g_main_loop_unref (loop);

  return 0;
}
