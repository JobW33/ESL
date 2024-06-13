
When the RPI is used, compile with: hardware/spidev/spidev_lib.c

When the controller is used, compile with: controller/tilt/PositionControllerTilt.cpp controller/tilt/common/xxinteg.cpp controller/pan/PositionControllerPan.cpp

rm gst_hello; clear; g++ main.cpp gst/GstProcessor.cpp controller/tilt/PositionControllerTilt.cpp controller/tilt/common/xxinteg.cpp controller/pan/PositionControllerPan.cpp -o gst_hello $(pkg-config --cflags --libs gstreamer-1.0); ./gst_hello
