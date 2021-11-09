void face_Detection_init(){
  
  mtmn_config.min_face = 80;
  mtmn_config.pyramid = 0.7;
  mtmn_config.p_threshold.score = 0.6;
  mtmn_config.p_threshold.nms = 0.7;
  mtmn_config.r_threshold.score = 0.7;
  mtmn_config.r_threshold.nms = 0.7;
  mtmn_config.r_threshold.candidate_number = 4;
  mtmn_config.o_threshold.score = 0.7;
  mtmn_config.o_threshold.nms = 0.4;
  mtmn_config.o_threshold.candidate_number = 1;
  mtmn_config = mtmn_init_config();
  Serial.println("[FD] Face detection init OK");
}

boolean face_detection(){

  boolean detected = false;
  
  camera_fb_t * fb = NULL;
  dl_matrix3du_t *image_matrix = NULL;
  box_array_t *net_boxes = NULL;

  // Frame capture
  fb = esp_camera_fb_get();

  // Check if frame was captured successfully
  if (!fb) {
    Serial.println("[FD] Error getting frame");
  }
  else {
    // Frame was captured successfully

    // Create image matrix
    image_matrix = dl_matrix3du_alloc(1, fb->width, fb->height, 3);
  
    if (!image_matrix) {
      Serial.println("[FD] dl_matrix3du_alloc failed");
    }
    else {
      // Image matrix creation successful

      // Convert matrix to RGB888
      if(!fmt2rgb888(fb->buf, fb->len, fb->format, image_matrix->item)){
        Serial.println("[FD] fmt2rgb888 failed");
      }
      else {
        // Conversion to RGB888 succeeded

        // Perform the actual face detection
        if(face_detect(image_matrix, &mtmn_config)){
          Serial.println("[FD] Face detected");
          detected = true;
        }
      } // End of RGB888 conversion check

      // Free up resources taken by the image matrix
      dl_matrix3du_free(image_matrix);
      image_matrix = NULL;
    } // End of image matrix check


    // Freeing up frame buffer resources
    if(fb) {
      esp_camera_fb_return(fb);
      fb = NULL;
    }

  } // end of FB check

  return detected;
  
}
