#include <gst/gst.h>
#include <glib.h>

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


static void
on_pad_added (GstElement *element,
              GstPad     *pad,
              gpointer    data)
{
  GstPad *sinkpad;
  GstElement *decoder = (GstElement *) data;

  /* We can now link this pad with the vorbis-decoder sink pad */
  g_print ("Dynamic pad created, linking demuxer/decoder\n");

  sinkpad = gst_element_get_static_pad (decoder, "sink");

  gst_pad_link (pad, sinkpad);

  gst_object_unref (sinkpad);
}

/* The appsink has received a buffer */
static GstFlowReturn new_sample (GstElement *sink) {
  GstSample *sample;

  /* Retrieve the buffer */
  g_signal_emit_by_name (sink, "pull-sample", &sample);
  if (sample) {
    /* The only thing we do in this example is print a * to indicate a received buffer */
    GstBuffer *buffer =  gst_sample_get_buffer(sample);
//    GstCaps *caps =  gst_sample_get_caps(sample);
//    GstSegment *segment = gst_sample_get_segment(sample);
    g_print("%ld\n", buffer->pts);
//    g_print("%ld\n", buffer->offset);
//    g_print("%ld\n", buffer->duration);
    
//    GstMapInfo info;
//    gst_buffer_map (buffer, &info, GST_MAP_READ);
//    gst_buffer_unmap(buffer, &info);

//    g_print("Data received (%" G_GSIZE_FORMAT " bytes):\n", info.size);
//    g_print(info);

//    gst_caps_unref(caps);
//    gst_buffer_unref(buffer);
    gst_sample_unref (sample);
    return GST_FLOW_OK;
  }

  return GST_FLOW_ERROR;
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
  encoder  = gst_element_factory_make ("jpegenc",       "jpeg encoder");
  capfilt  = gst_element_factory_make ("capsfilter",    "jpeg data format");
  decoder  = gst_element_factory_make ("jpegdec",       "jpeg decoder");
  sink     = gst_element_factory_make ("appsink",       "app sink");

  if (!pipeline || !source || !encoder || !capfilt || !decoder || !sink) {
    g_printerr ("One element could not be created. Exiting.\n");
    return -1;
  }

  /* Set up the pipeline */

  GstCaps *caps = gst_caps_from_string ("image/jpeg,width=160,height=120,framerate=30/1");
  g_object_set(G_OBJECT(capfilt), "caps", caps, NULL);   // Set width

// CHATGPT
  /* Set the source device */
  g_object_set(G_OBJECT(source), "device", "/dev/video0", NULL); // Adjust source location.
// CHATGPT

  /* Configure appsink */
  g_object_set (sink, "emit-signals", TRUE, NULL);
  g_signal_connect (sink, "new-sample", G_CALLBACK (new_sample), NULL);


  /* we add a message handler */
  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
  gst_object_unref (bus);

  /* we add all elements into the pipeline */
  /* webcam source | jpeg encoder | jpeg data format | jpeg decoder | file sink */
  gst_bin_add_many (GST_BIN (pipeline),
                    source, encoder, capfilt, decoder, sink, NULL);

  /* we link the elements together */
  /* webcam source -> jpeg encoder -> jpeg data format -> jpeg decoder -> file sink */
  gst_element_link_many (source, encoder, capfilt, decoder, sink, NULL);

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
