cm: recording_detector.cpp
	g++ --std=c++0x -o camera_manager recording_detector.cpp -I.

camman: recording_detector.cpp
	g++ --std=c++0x -o camman recording_detector.cpp -I.

cd: change_detector.cpp
	g++ --std=c++0x -o change_detector change_detector.cpp -I.

clean_camman:
	rm camman

clean:
	rm camera_manager camman change_detector
