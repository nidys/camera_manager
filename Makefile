camera_manager: recording_detector.cpp
	g++ -o camera_manager recording_detector.cpp -I.

clean:
	rm camera_manager
