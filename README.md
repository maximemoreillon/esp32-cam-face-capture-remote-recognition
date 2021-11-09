# ESP32-cam remote face recognition
Although the ESP32-CAM module is capable of face recognition, I don't relly trust its abilities to do a robust job at it.
Thus, with this project, the ESP32-cam simply detects faces, takes a snapshot of said faces and uploads the snapshots via HTTP to a specific server which can perform a more advanced face recognition.