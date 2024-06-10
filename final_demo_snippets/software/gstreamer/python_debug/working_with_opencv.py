import cv2
import numpy as np
import gi
gi.require_version('Gst', '1.0')
from gi.repository import Gst

# Initialize GStreamer
Gst.init(None)

# Create GStreamer pipeline
pipeline = Gst.parse_launch("v4l2src device=/dev/video0 ! videoconvert ! video/x-raw,format=BGR ! appsink name=sink")

# Get the appsink
appsink = pipeline.get_by_name("sink")
appsink.set_property("emit-signals", True)
appsink.set_property("sync", False)

# Start the pipeline
pipeline.set_state(Gst.State.PLAYING)

# Define video dimensions
width = 640
height = 480

# Create video writers
fourcc = cv2.VideoWriter_fourcc(*'XVID')
writer_frame = cv2.VideoWriter('output_frame.avi', fourcc, 20.0, (width, height))
writer_mask = cv2.VideoWriter('output_mask.avi', fourcc, 20.0, (width, height))

# Function to calculate the center of mass for custom green regions
def calculate_center_of_mass(frame):
    # Define the structuring element for 24 neighbors
    kernel_size = 8
    kernel = np.ones((kernel_size, kernel_size), np.uint8)
    
    # Perform dilation on the blue channel
#    frame = cv2.dilate(frame, kernel)

    # Extract color channels
    B = frame[:, :, 0]
    G = frame[:, :, 1]
    R = frame[:, :, 2]
    
    # Custom condition: B > 2 * (G + R)
    mask = (((B > 200) & (B.astype(int) > 0.8 * (G.astype(int) + R.astype(int))))).astype(np.uint8)
#    mask = ((B.astype(int) > 200) * 255).astype(np.uint8)
 
#    print(int(G[0,0]) + int(R[0,0]))
#    print(B[0,0] > (G[0,0] + R[0,0]))#((B[0,0] > 2*(G[0,0] + R[0,0])) * B[0,0]))
    # Calculate image moments of the mask
    moments = cv2.moments(mask)
    if moments["m00"] > ((height*width)/2**8):
        cx = int(moments["m10"] / moments["m00"])
        cy = int(moments["m01"] / moments["m00"])
    else:
        cx, cy = int(width/2), int(height/2)
    
    mask = mask * 255
    print(str(cx) + ", " + str(cy))
    return cx, cy, mask

# Main loop to process frames
while True:
    sample = appsink.emit("pull-sample")
    if sample:
        buf = sample.get_buffer()
        caps = sample.get_caps()
        arr = np.ndarray(
            (caps.get_structure(0).get_value("height"), caps.get_structure(0).get_value("width"), 3),
            buffer=buf.extract_dup(0, buf.get_size()),
            dtype=np.uint8
        )
        
        # Make the array writable
        writable_arr = arr.copy()
        
        # Calculate the center of mass for custom green regions
        cx, cy, mask = calculate_center_of_mass(writable_arr)
        
        # Draw the center of mass on the frame
        cv2.circle(writable_arr, (cx, cy), 5, (0, 0, 255), -1)
        
        # Write the frame with the center of mass to video file
        writer_frame.write(writable_arr)
        
        # Write the mask to video file
        writer_mask.write(mask)
        
        # Display the frame and the mask
        cv2.imshow("Frame", writable_arr)
        cv2.imshow("Mask", mask)
        
        # Break the loop if the 'q' key is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    else:
        break

# Clean up
pipeline.set_state(Gst.State.NULL)
cv2.destroyAllWindows()

# Release video writers
writer_frame.release()
writer_mask.release()
