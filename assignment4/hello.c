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


//CHATGPT
static gboolean check_bus_timeout(gpointer data)
{
  GMainLoop *loop = (GMainLoop *)data;
  g_print("Timeout reached\n");
  g_main_loop_quit(loop);
  return FALSE;  /* Stop the timer after the timeout */
}
//CHATGPT

int
main (int   argc,
      char *argv[])
{
  GMainLoop *loop;

  GstElement *pipeline, *source, *encoder, *decoder, *sink;
  GstBus *bus;
  guint bus_watch_id;
  guint timeout_id;

  /* Initialisation */
  gst_init (&argc, &argv);

  loop = g_main_loop_new (NULL, FALSE);


//  /* Check input arguments */
//  if (argc != 2) {
//    g_printerr ("Usage: %s <Ogg/Vorbis filename>\n", argv[0]);
//    return -1;
//  }


  /* Create gstreamer elements */
  pipeline = gst_pipeline_new ("YUV storing");
  source   = gst_element_factory_make ("v4l2src",       "webcam source");
  encoder  = gst_element_factory_make ("jpegenc",       "jpeg encoder");
  decoder  = gst_element_factory_make ("jpegdec",       "jpeg decoder");
  sink     = gst_element_factory_make ("filesink",      "file sink");

  if (!pipeline || !source || !encoder || !decoder || !sink) {
    g_printerr ("One element could not be created. Exiting.\n");
    return -1;
  }

  /* Set up the pipeline */

// CHATGPT
  /* Set the source device */
  g_object_set(G_OBJECT(source), "device", "/dev/video0", NULL); // Adjust source location.
  g_object_set(G_OBJECT(source), "width", 160, NULL);   // Set width
  g_object_set(G_OBJECT(source), "height", 120, NULL);  // Set height
  g_object_set(G_OBJECT(source), "format", GST_VIDEO_FORMAT_I420, NULL); // Set format, e.g., YUV

  /* Set the output file location */
  g_object_set(G_OBJECT(sink), "location", "epic_media.yuv", NULL);
// CHATGPT

  /* we add a message handler */
  bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
  gst_object_unref (bus);

  /* we add all elements into the pipeline */
  /* file-source | ogg-demuxer | vorbis-decoder | converter | alsa-output */
  gst_bin_add_many (GST_BIN (pipeline),
                    source, encoder, decoder, sink, NULL);

  /* we link the elements together */
  /* file-source -> ogg-demuxer ~> vorbis-decoder -> converter -> alsa-output */
//  gst_element_link (source, demuxer);
  gst_element_link_many (source, encoder, decoder, sink, NULL);
//  g_signal_connect (demuxer, "pad-added", G_CALLBACK (on_pad_added), decoder);

  /* note that the demuxer will be linked to the decoder dynamically.
     The reason is that Ogg may contain various streams (for example
     audio and video). The source pad(s) will be created at run time,
     by the demuxer when it detects the amount and nature of streams.
     Therefore we connect a callback function which will be executed
     when the "pad-added" is emitted.*/


  /* Set the pipeline to "playing" state*/
  g_print ("Now playing");
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

//CHATGPT
  /* Set up a timeout */
  timeout_id = g_timeout_add_seconds(10, check_bus_timeout, loop);
//CHATGPT

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
