/**
 * @file sfe_face.h
 * @author Innovatrics - SmartFace Embedded
 * @date 16.5.2023
 * @brief File containing API of sfe_face library as part of SFE Toolkit
 */

#pragma once

#include "sfe_core.h"

#ifdef __cplusplus
extern "C" {
#endif

// FACE DETECTION

/// @deprecated Use SFEEntity instead, this type will be removed in future
typedef SFEEntity SFEFaceEntity;

/// @brief Candidate for identification by template
/// @deprecated Replace with SFETemplateIdentificationCandidate
typedef SFETemplateIdentificationCandidate SFEIdentificationResult;

// @brief Candidate for identification by template
/// @deprecated Replace with SFETemplateIdentificationCandidate
typedef SFETemplateIdentificationCandidate SFEFaceTemplateIdentificationCandidate;

/// @brief Candidate for identification by entity
/// @deprecated Replace with SFEEntityIdentificationCandidate
typedef SFEEntityIdentificationCandidate SFEFaceEntityIdentificationCandidate;

/// @brief Face detect struct
typedef struct SFEFaceDetect {
  /// bounding box
  SFEBbox bbox;
  /// normalised value of detection confidence of detected face - range <0,1>
  float confidence;
  /// raw value of detection confidence of detected face - range <0,1>
  float raw_confidence;
} SFEFaceDetect;

/// @brief Detect faces in the source image
/// @param[in] solver face detection solver
/// @param[in] image source image
/// @param[in] threshold detection confidence threshold - range <0,1>
/// @param[out] out_faces pointer to an array of detected faces with detection
/// confidence above the threshold, detected faces are sorted by detection
/// confidence
/// @param[in,out] in_out_face_count IN: maximum number of faces to detect OUT: real
/// number of detected faces
/// @return Error in case of failed detection.
SFEError sfeFaceDetect(SFESolver solver, SFEImageView image, float threshold,
                       SFEFaceDetect *out_faces, size_t *in_out_face_count);

// FACE LANDMARKS
#define SFE_FACE_LANDMARK_COUNT 23

/// @brief Face landmark types
typedef enum SFEFaceLandmarkType {
  SFE_FACE_LANDMARK_TYPE_RIGHT_EYE_OUTER_CORNER = 0,
  SFE_FACE_LANDMARK_TYPE_RIGHT_EYE_CENTER = 1,
  SFE_FACE_LANDMARK_TYPE_RIGHT_EYE_INNER_CORNER = 2,
  SFE_FACE_LANDMARK_TYPE_LEFT_EYE_INNER_CORNER = 3,
  SFE_FACE_LANDMARK_TYPE_LEFT_EYE_CENTER = 4,
  SFE_FACE_LANDMARK_TYPE_LEFT_EYE_OUTER_CORNER = 5,
  SFE_FACE_LANDMARK_TYPE_NOSE_ROOT = 6,
  SFE_FACE_LANDMARK_TYPE_NOSE_RIGHT_BOTTOM = 7,
  SFE_FACE_LANDMARK_TYPE_NOSE_TIP = 8,
  SFE_FACE_LANDMARK_TYPE_NOSE_LEFT_BOTTOM = 9,
  SFE_FACE_LANDMARK_TYPE_NOSE_BOTTOM = 10,
  SFE_FACE_LANDMARK_TYPE_MOUTH_RIGHT_CORNER = 11,
  SFE_FACE_LANDMARK_TYPE_MOUTH_CENTER = 12,
  SFE_FACE_LANDMARK_TYPE_MOUTH_LEFT_CORNER = 13,
  SFE_FACE_LANDMARK_TYPE_MOUTH_UPPER_EDGE = 14,
  SFE_FACE_LANDMARK_TYPE_MOUTH_LOWER_EDGE = 15,
  SFE_FACE_LANDMARK_TYPE_RIGHT_EYEBROW_OUTER_END = 16,
  SFE_FACE_LANDMARK_TYPE_RIGHT_EYEBROW_INNER_END = 17,
  SFE_FACE_LANDMARK_TYPE_LEFT_EYEBROW_INNER_END = 18,
  SFE_FACE_LANDMARK_TYPE_LEFT_EYEBROW_OUTER_END = 19,
  SFE_FACE_LANDMARK_TYPE_RIGHT_EDGE = 20,
  SFE_FACE_LANDMARK_TYPE_CHIN_TIP = 21,
  SFE_FACE_LANDMARK_TYPE_LEFT_EDGE = 22,
} SFEFaceLandmarkType;

/// @brief Face landmarks struct
typedef struct SFEFaceLandmarks {
  /// @brief Type of the landmark detected on the face
  SFEFaceLandmarkType landmark_type;
  /// @brief Confidence of the landmark detected on the face - range <0,1>
  float confidence;
  /// @brief X coordinate of the landmark relative to source image - range <0,1>
  float x;
  /// @brief Y coordinate of the landmark relative to source image - range <0,1>
  float y;
} SFEFaceLandmarks;

/// @brief Detect 23 landmarks of the face detected in the area of source image
/// marked with bounding box
/// @param[in] solver face landmarks solver
/// @param[in] image source image
/// @param[in] bbox bounding box of the detected face
/// @param[out] out_face_landmarks OUT: pointer to an array of detected landmarks,
/// the size of the array should be 23 which is the number of landmarks detected
/// by the solver
/// @return Error in case of failed landmarks detection.
SFEError
sfeFaceLandmarks(SFESolver solver, SFEImageView image, SFEBbox bbox,
                 SFEFaceLandmarks out_face_landmarks[SFE_FACE_LANDMARK_COUNT]);

/// @brief Get confidence from given landmarks if the face is wearing a face
/// mask
/// @param[in] face_landmarks array of detected landmarks, the size of the array
/// should be 23 which is the number of landmarks detected by the solver
/// @param[out] out_mask_confidence OUT: confidence of wearing a face mask - range
/// <0-1>
SFEError sfeFaceMaskConfidence(
    const SFEFaceLandmarks face_landmarks[SFE_FACE_LANDMARK_COUNT],
    float *out_mask_confidence);

/// @brief Get the face size in pixels from the face landmarks and the source
/// image. Face size is defined as a maximum of values of inter eyes centers
/// distance and distance between center of mouth and center point between eyes
/// (nose root): face_size = max(distance(left_eye_center, right_eye_center),
/// distance(mouth_center, eyes_center))
/// @param[in] image source image
/// @param[in] face_landmarks array of face landmarks
/// @param[out] face_size OUT: size of the face in pixels
/// @return Error
SFEError sfeFaceSize(SFEImageView image,
                     SFEFaceLandmarks face_landmarks[SFE_FACE_LANDMARK_COUNT],
                     float *face_size);

// FACE TEMPLATE
#define SFE_FACE_TEMPLATE_SIZE 522

/// @brief Face template struct
typedef struct SFEFaceTemplate {
  uint8_t data[SFE_FACE_TEMPLATE_SIZE];
} SFEFaceTemplate;

/// @brief Face template version struct
typedef struct SFEFaceTemplateVersion {
  /// @brief major template version
  uint8_t version_major;
  /// @brief minor template version
  uint8_t version_minor;
} SFEFaceTemplateVersion;

/// @brief Extract template from source image and given face landmarks
/// @param[in] solver template extraction solver
/// @param[in] image source image
/// @param[in] face_landmarks face landmarks detected in source image
/// @param[in] raw_detection_confidence raw value of face detection confidence of
/// detected face (used in template quality calculation)
/// @param[out] out_face_template extracted face template
/// @return Error in case of failed extraction.
SFEError
sfeFaceTemplateExtract(SFESolver solver, SFEImageView image,
                       SFEFaceLandmarks face_landmarks[SFE_FACE_LANDMARK_COUNT],
                       float raw_detection_confidence,
                       SFEFaceTemplate *out_face_template);

/// @brief Match two face templates
/// @param[in] template1 first template to match
/// @param[in] template2 second template to match
/// @param[out] out_matching_score OUT: matching score - range <0,1>
/// @return Error in case of failed matching
SFEError sfeFaceTemplateMatch(SFEFaceTemplate *template1,
                              SFEFaceTemplate *template2,
                              float *out_matching_score);

/// @brief Get template quality
/// @param[in] face_template source template
/// @param[out] out_face_template_quality OUT: template quality - range <0,1>
/// @return Error in case of template checksum mismatch
SFEError sfeFaceTemplateQuality(SFEFaceTemplate *face_template,
                                float *out_face_template_quality);

/// @brief Get face template version
/// @param[in] face_template  source template
/// @param[out] out_face_template_version OUT: face template version struct
/// @return Error in case of template checksum mismatch
SFEError
sfeFaceTemplateVersion(SFEFaceTemplate *face_template,
                       SFEFaceTemplateVersion *out_face_template_version);

/// @brief Get an ordered array of SFETemplateIdentificationCandidate from tested template
/// best matches of probe template with the templates in templates_gallery
/// @param[in] probe_face_template probe template
/// @param[in] gallery_size number of templates in gallery
/// @param[in] templates_gallery pointer to an array of SFEFaceTemplate
/// @param[in] matching_score_threshold threshold for matching - range <0,1>
/// Function will return only candidates with score above the threshold
/// @param[out] out_candidates Pointer to an ordered array of candidates
/// @param[in,out] in_out_candidates_count IN: Expected number of candidates
/// OUT: Number of found candidates with matching score above the threshold
/// @param[in] thread_count Number of threads to use in the identification - 0: use
/// all available cores, 1: use single thread, N: use N number of threads
/// @return Error in case of failed identification
SFEError
sfeFaceTemplateIdentify(SFEFaceTemplate *probe_face_template,
                        SFEFaceTemplate *templates_gallery, size_t gallery_size,
                        float matching_score_threshold,
                        SFEFaceTemplateIdentificationCandidate *out_candidates,
                        size_t *in_out_candidates_count, size_t thread_count);

/// @brief Get an ordered array of SFEEntityIdentificationCandidate from tested
/// template best matches of probe template with the templates in
/// templates_gallery Entity is then used to group the results by entity using
/// the best score of any template associated with the entity.
/// @param[in] probe_face_template probe template
/// @param[in] templates_gallery pointer to an array of SFEFaceTemplate
/// @param[in] entities_gallery pointer to an array of SFEEntity that
/// corresponds with templates_gallery
/// @param[in] gallery_size number of templates in gallery
/// @param[in] matching_score_threshold threshold for matching - range <0,1>
/// Function will return only candidates with score above the threshold
/// @param[out] out_candidates Pointer to an ordered array of candidates
/// @param[in,out] in_out_candidates_count IN: Expected number of candidates
/// OUT: Number of found entities with best score above the threshold
/// @param[in] thread_count Number of threads to use in the identification - 0: use
/// all available cores, 1: use single thread, N: use N number of threads
/// @return Error in case of failed identification
SFEError sfeFaceEntityIdentify(SFEFaceTemplate *probe_face_template,
                               SFEFaceTemplate *templates_gallery,
                               SFEEntity *entities_gallery, size_t gallery_size,
                               float matching_score_threshold,
                               SFEEntityIdentificationCandidate *out_candidates,
                               size_t *in_out_candidates_count,
                               size_t thread_count);

/// @brief Passive liveness score calculation
/// @param[in] solver passive liveness solver
/// @param[in] image source image
/// @param[in] face_landmarks face landmarks detected in source image
/// @param[out] out_liveness_score OUT: normalized score of passive liveness
/// @return Error in case of failed passive liveness score calculation
SFEError
sfeFaceLivenessPassive(SFESolver solver, SFEImageView image,
                       SFEFaceLandmarks face_landmarks[SFE_FACE_LANDMARK_COUNT],
                       float *out_liveness_score);

/// @brief Face area struct
typedef struct SFEFaceArea {
  /// @brief absolute face size
  float size;
  /// @brief size of face area relative to the whole image; value in range <0,1>
  float area;
  /// @brief size of face area intersected with the whole image and relative to
  /// the whole image; value in range <0,1>
  float area_in_image;
} SFEFaceArea;

/// @brief Get face area
/// @param[in] image source image
/// @param[in] face_landmarks face landmarks detected in source image
/// @param[out] out_area OUT: structure containing the face size, the face area
/// relative to the source image, the face area intersected with the source
/// image and relative to the source image.
/// @return Error in case of failed area struct calculations
SFEError sfeFaceArea(SFEImageView image,
                     SFEFaceLandmarks face_landmarks[SFE_FACE_LANDMARK_COUNT],
                     SFEFaceArea *out_area);

/// @brief Face head pose struct containing angle rotations of head
typedef struct SFEFaceHeadPose {
  /// @brief Face attribute representing angle rotation of head towards camera
  /// reference frame around X-axis as per DIN9300
  float pitch;
  /// @brief Face attribute representing angle rotation of head towards camera
  /// reference frame around Y-axis as per DIN9300.
  float yaw;
  /// @brief Face attribute representing angle rotation of head towards camera
  /// reference frame around Z-axis as per DIN9300.
  float roll;
} SFEFaceHeadPose;

/// @brief Calculate angle rotations of head towards camera reference frame from
/// given landmarks
/// @param[in] face_landmarks face landmarks detected in source image
/// @param[out] out_head_pose OUT: structure containing the angle rotations - roll,
/// yaw and pitch
/// @return Error in case of failed head pose calculationse
SFEError
sfeFaceHeadPose(SFEImageView image,
                SFEFaceLandmarks face_landmarks[SFE_FACE_LANDMARK_COUNT],
                SFEFaceHeadPose *out_head_pose);

/// @brief Face quality attributes struct
typedef struct SFEFaceQualityAttributes {
  /// @brief Normalized face attribute for evaluating whether an area of face
  /// image is not blurred. Sharpness values are from range <0,inf>.  Values
  /// near 0 indicate 'very blurred', values near (or above) 1 indicate 'very
  /// sharp'. The decision threshold is in range <0.08, 0.7>.
  float sharpness;
  /// @brief Normalized face attribute for evaluating whether an area of face is
  /// correctly exposed. Brightness values are from range <0,1>. Values near 0
  /// indicate 'too dark', values near 1 indicate 'too light', values around 0
  /// indicate OK. The decision thresholds are around 0.25 and 0.75.
  float brightness;
  /// @brief Normalized face attribute for evaluating whether an area of face is
  /// contrast enough.  Contrast values are from range <0,1>. Values near 0
  /// indicate 'very low contrast', values near 1 indicate 'very high
  /// contrast', values around 0 indicate OK. The decision thresholds are
  /// around 0.25 and 0.75.
  float contrast;
  /// @brief Normalized face attribute for evaluating whether an area of face
  /// has appropriate number of unique intensity levels. Unique intensity levels
  /// values are from range <0,1>. Values near 0 indicate 'very
  /// few unique intensity levels', values near 1 indicate 'enough unique
  /// intensity levels'. The decision threshold is around 0.5.
  float unique_intensity_levels;
} SFEFaceQualityAttributes;

/// @brief Calculate face image quality attributes from given source image and
/// landmarks data
/// @param[in] image source image
/// @param[in] face_landmarks face landmarks detected in source image
/// @param[out] out_quality_attributes structure containing normalized `sharpness`,
/// `brightness`, `contrast` and `unique_intensity_levels`
/// @return Error in case of failed quality attributes calculation
SFEError sfeFaceQualityAttributes(
    SFEImageView image,
    SFEFaceLandmarks face_landmarks[SFE_FACE_LANDMARK_COUNT],
    SFEFaceQualityAttributes *out_quality_attributes);

/// @brief Supported 2 detection accuracy types
typedef enum SFEFaceDetectAccuracyType {
  SFE_FACE_DETECT_ACCURACY_TYPE_ACCURATE = 0,
  SFE_FACE_DETECT_ACCURACY_TYPE_BALANCED = 1,
} SFEFaceDetectAccuracyType;

/// @brief Calculation of recommended input image width and height according to
/// desired minimal and maximal size of detected faces.
/// The input combination of required maximal and minimal face sizes is
/// validated against the model constraints. If valid, the recommended width and
/// height of the image are calculated. If invalid, the error is returned. It is
/// recommended to set the max_face_size to `max_face_size < 30 *
/// min_face_size`. Performance could decrease if you do not comply with this
/// requirement.
/// @param[in] image Source image
/// @param[in] detection_mode Accuracy type, it can be accurate or balanced
/// @param[in] min_face_size Desired minimal size of detected face
/// @param[in] max_face_size Desired maximal size of detected face
/// @param[out] out_img_width Recommended image width
/// @param[out] out_img_height Recommended image height
/// @return Error in case of non valid input combination
SFEError sfeFaceDetectInputSize(SFEImageView image,
                                SFEFaceDetectAccuracyType detection_mode,
                                size_t min_face_size, size_t max_face_size,
                                size_t *out_img_width, size_t *out_img_height);

/// @brief Face crop struct
typedef struct SFEFaceCrop {
  /// @brief Defines the size of the crop as an extension of
  /// the detection bounding box.
  uint32_t face_size_extension;
  /// @brief Bounding box of cropped face including the crop extension
  SFEBbox crop_box;
  /// @brief Image of face cropped according the crop_box
  SFEImage crop_image;
} SFEFaceCrop;

/// @brief Crop the face according the given landmarks, the face size
/// extension and the maximal face size. The face size extension defines the
/// size of the crop as an extension of the detection bounding box. The crop
/// will be centered on the detection bounding box and the size will be
/// multiplied by this value. Valid values are 0, 1, 2, 3, 4 and 5.
/// @param[in] image Source image
/// @param[in] face_landmarks Array of face landmarks
/// @param[in] face_size_extension Defines the size of the crop as an extension of
/// the detection bounding box.
/// @param[in] max_face_size Defines the maximum size of the face in the crop area.
/// If the actual face size is larger, the crop image will be downscaled.
/// @param[out] out_crop OUT: Structure containing used crop extension,
/// calculated crop's bounding box and the cropped image of the face
SFEError sfeFaceCrop(SFEImageView image,
                     SFEFaceLandmarks face_landmarks[SFE_FACE_LANDMARK_COUNT],
                     uint32_t face_size_extension, float max_face_size,
                     SFEFaceCrop *out_crop);

/// @brief Tracker is a ByteTrack implementation for face tracking across
/// @warning Use `sfeFaceTrackerFree` to release memory associated with the
/// tracker.
typedef void *SFEFaceTracker;

/// @brief Tracked state
enum SFEFaceTrackedState {
  SFE_FACE_TRACKED_STATE_NEW = 0,
  SFE_FACE_TRACKED_STATE_TRACKED = 1,
  SFE_FACE_TRACKED_STATE_LOST = 2,
  SFE_FACE_TRACKED_STATE_REMOVED = 3,
};

/// @brief Tracked entity struct
typedef struct SFEFaceTracked {
  /// @brief Tracked detection
  SFEFaceDetect tracked;
  /// @brief Tracked ID
  uint64_t id;
  /// @brief UUID
  SFEEntity uuid;
  /// @brief Tracking state
  SFEFaceTrackedState state;
} SFEFaceTracked;

/// @brief Create a new tracker
/// @param[in] track_threshold Tracking threshold
/// @param[in] high_threshold High threshold
/// @param[in] match_threshold Match threshold
/// @param[in] max_time_lost Maximum time lost
/// @param[out] out_tracker OUT: pointer to the created tracker
SFEError sfeFaceTrackerCreate(float track_threshold, float high_threshold,
                              float match_threshold, uint64_t max_time_lost,
                              SFEFaceTracker *out_tracker);

/// @brief Free the tracker
/// @param[in] tracker Tracker to free
SFEError sfeFaceTrackerFree(SFEFaceTracker tracker);

/// @brief Update the tracker
/// @param[in] tracker Tracker to update
/// @param[in] detections Detections to update the tracker with
/// @param[in] detections_count Number of detections
/// @param[out] out_tracked OUT: pointer to the array of tracked entities
/// @param[out] out_tracked_count OUT: number of tracked entities
/// @return Error in case of failed update
SFEError sfeFaceTrackerUpdate(SFEFaceTracker tracker, SFEFaceDetect *detections,
                              size_t detections_count,
                              SFEFaceTracked *out_tracked,
                              size_t *out_tracked_count);

/// @brief Get lost entities after update
/// @param[in] tracker Tracker to get lost entities from
/// @param[out] out_entities OUT: pointer to the array of lost entities
/// @param[in,out] in_out_entities_count OUT: number of lost entities
/// @return Error in case of failed lost entities retrieval
SFEError sfeFaceTrackerLost(SFEFaceTracker tracker, SFEEntity *out_entities,
                            size_t *in_out_entities_count);

/// @brief Get removed entities after update
/// @param[in] tracker Tracker to get removed entities from
/// @param[out] out_entities OUT: pointer to the array of removed entities
/// @param[in,out] in_out_entities_count OUT: number of removed entities
/// @return Error in case of failed removed entities retrieval
SFEError sfeFaceTrackerRemoved(SFEFaceTracker tracker, SFEEntity *out_entities,
                               size_t *in_out_entities_count);

#ifdef __cplusplus
} // extern "C"
#endif
