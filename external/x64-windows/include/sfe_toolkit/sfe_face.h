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
/// @param solver face detection solver
/// @param image source image
/// @param threshold detection confidence threshold - range <0,1>
/// @param out_faces pointer to an array of detected faces with detection
/// confidence above the threshold, detected faces are sorted by detection
/// confidence
/// @param in_out_face_count IN: maximum number of faces to detect OUT: real
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
/// @param solver face landmarks solver
/// @param image source image
/// @param bbox bounding box of the detected face
/// @param out_face_landmarks OUT: pointer to an array of detected landmarks,
/// the size of the array should be 23 which is the number of landmarks detected
/// by the solver
/// @return Error in case of failed landmarks detection.
SFEError
sfeFaceLandmarks(SFESolver solver, SFEImageView image, SFEBbox bbox,
                 SFEFaceLandmarks out_face_landmarks[SFE_FACE_LANDMARK_COUNT]);

/// @brief Get confidence from given landmarks if the face is wearing a face
/// mask
/// @param face_landmarks array of detected landmarks, the size of the array
/// should be 23 which is the number of landmarks detected by the solver
/// @param out_mask_confidence OUT: confidence of wearing a face mask - range
/// <0-1>
SFEError sfeFaceMaskConfidence(
    const SFEFaceLandmarks face_landmarks[SFE_FACE_LANDMARK_COUNT],
    float *out_mask_confidence);

/// @brief Get the face size in pixels from the face landmarks and the source
/// image. Face size is defined as a maximum of values of inter eyes centers
/// distance and distance between center of mouth and center point between eyes
/// (nose root): face_size = max(distance(left_eye_center, right_eye_center),
/// distance(mouth_center, eyes_center))
/// @param image source image
/// @param face_landmarks array of face landmarks
/// @param face_size OUT: size of the face in pixels
/// @return Error
SFEError sfeFaceSize(SFEImageView image,
                     SFEFaceLandmarks face_landmarks[SFE_FACE_LANDMARK_COUNT],
                     size_t *face_size);

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

/// @brief Candidate for identification by template
typedef struct SFEFaceTemplateIdentificationCandidate {
  /// @brief matching score
  float score;
  /// @brief index of the template from the input gallery
  size_t index;
 } SFEFaceTemplateIdentificationCandidate;

/// @brief Face template identification result
/// @deprecated Replace with SFEFaceTemplateIdentificationCandidate
typedef SFEFaceTemplateIdentificationCandidate SFEIdentificationResult;

/// @brief Face entity type, used to group templates for entity identification.
/// This type is compatible with uuid_v4 that can be used to generate unique ids
typedef struct SFEFaceEntity {
  uint8_t uuid[16];
} SFEFaceEntity;

/// @brief Candidate for identification by entity
typedef struct SFEFaceEntityIdentificationCandidate {
  /// @brief matching score
  float score;
  /// @brief candidate entity id
  SFEFaceEntity entity;
 } SFEFaceEntityIdentificationCandidate;

/// @brief Extract template from source image and given face landmarks
/// @param solver template extraction solver
/// @param image source image
/// @param face_landmarks face landmarks detected in source image
/// @param raw_detection_confidence raw value of face detection confidence of
/// detected face (used in template quality calculation)
/// @param out_face_template extracted face template
/// @return Error in case of failed detection.
SFEError
sfeFaceTemplateExtract(SFESolver solver, SFEImageView image,
                       SFEFaceLandmarks face_landmarks[SFE_FACE_LANDMARK_COUNT],
                       float raw_detection_confidence,
                       SFEFaceTemplate *out_face_template);

/// @brief Match two face templates
/// @param template1 first template to match
/// @param template2 second template to match
/// @param out_matching_score OUT: matching score - range <0,1>
/// @return Error in case of failed matching
SFEError sfeFaceTemplateMatch(SFEFaceTemplate *template1,
                              SFEFaceTemplate *template2,
                              float *out_matching_score);

/// @brief Get template quality
/// @param face_template source template
/// @param out_face_template_quality OUT: template quality - range <0,1>
/// @return Error in case of template checksum mismatch
SFEError sfeFaceTemplateQuality(SFEFaceTemplate *face_template,
                                float *out_face_template_quality);

/// @brief Get face template version
/// @param face_template  source template
/// @param out_face_template_version OUT: face template version struct
/// @return Error in case of template checksum mismatch
SFEError
sfeFaceTemplateVersion(SFEFaceTemplate *face_template,
                       SFEFaceTemplateVersion *out_face_template_version);

/// @brief Get an ordered array of SFEIdentificationResult from tested template
/// best matches of probe template with the templates in templates_gallery
/// @param probe_face_template probe template
/// @param gallery_size number of templates in gallery
/// @param templates_gallery pointer to an array of SFEFaceTemplate
/// @param matching_score_threshold threshold for matching - range <0,1>
/// Function will return only candidates with score above the threshold
/// @param out_candidates Pointer to an ordered array of candidates
/// @param in_out_candidates_count IN: Expected number of candidates
/// OUT: Number of found candidates with matching score above the threshold
/// @param thread_count Number of threads to use in the identification - 0: use
/// all available cores, 1: use single thread, N: use N number of threads
/// @return Error in case of failed identification
SFEError sfeFaceTemplateIdentify(SFEFaceTemplate *probe_face_template,
                                 SFEFaceTemplate *templates_gallery,
                                 size_t gallery_size,
                                 float matching_score_threshold,
                                 SFEFaceTemplateIdentificationCandidate *out_candidates,
                                 size_t *in_out_candidates_count,
                                 size_t thread_count);

/// @brief Get an ordered array of SFEIdentificationEntityResult from tested template
/// best matches of probe template with the templates in templates_gallery
/// Entity is then used to group the results by entity using the best score of any template associated with the entity.
/// @param probe_face_template probe template
/// @param templates_gallery pointer to an array of SFEFaceTemplate
/// @param entities_gallery pointer to an array of SFEFaceEntity that corresponds with templates_gallery
/// @param gallery_size number of templates in gallery
/// @param matching_score_threshold threshold for matching - range <0,1>
/// Function will return only candidates with score above the threshold
/// @param out_candidates Pointer to an ordered array of candidates
/// @param in_out_candidates_count IN: Expected number of candidates
/// OUT: Number of found entities with best score above the threshold
/// @param thread_count Number of threads to use in the identification - 0: use
/// all available cores, 1: use single thread, N: use N number of threads
/// @return Error in case of failed identification
SFEError sfeFaceEntityIdentify(SFEFaceTemplate *probe_face_template,
                                 SFEFaceTemplate *templates_gallery,
                                 SFEFaceEntity *entities_gallery,
                                 size_t gallery_size,
                                 float matching_score_threshold,
                                 SFEFaceEntityIdentificationCandidate *out_candidates,
                                 size_t *in_out_candidates_count,
                                 size_t thread_count);

/// @brief Passive liveness score calculation
/// @param solver passive liveness solver
/// @param image source image
/// @param face_landmarks face landmarks detected in source image
/// @param out_liveness_score OUT: normalized score of passive liveness
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
/// @param image source image
/// @param face_landmarks face landmarks detected in source image
/// @param out_area OUT: structure containing the face size, the face area
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
/// @param face_landmarks face landmarks detected in source image
/// @param out_head_pose OUT: structure containing the angle rotations - roll,
/// yaw and pitch
/// @return Error in case of failed head pose calculationse
SFEError
sfeFaceHeadPose(SFEImageView image,
                SFEFaceLandmarks face_landmarks[SFE_FACE_LANDMARK_COUNT],
                SFEFaceHeadPose *out_head_pose);

/// @brief Face quality attributes struct
typedef struct SFEFaceQualityAttributes {
  /// @brief Normalized face attribute for evaluating whether an area of face
  /// image is not blurred. Sharpness values are from range <0,inf>.  Values near
  /// 0 indicate 'very blurred', values near (or above) 1 indicate 'very sharp'.
  /// The decision threshold is in range <0.08, 0.7>.
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
/// @param image source image
/// @param face_landmarks face landmarks detected in source image
/// @param out_quality_attributes structure containing normalized `sharpness`,
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
/// @param image Source image
/// @param detection_mode Accuracy type, it can be accurate or balanced
/// @param min_face_size Desired minimal size of detected face
/// @param max_face_size Desired maximal size of detected face
/// @param out_img_width Recommended image width
/// @param out_img_height Recommended image height
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
/// @param image Source image
/// @param face_landmarks Array of face landmarks
/// @param face_size_extension Defines the size of the crop as an extension of
/// the detection bounding box.
/// @param max_face_size Defines the maximum size of the face in the crop area.
/// If the actual face size is larger, the crop image will be downscaled.
/// @param out_crop OUT: Structure containing used crop extension,
/// calculated crop's bounding box and the cropped image of the face
SFEError sfeFaceCrop(SFEImageView image,
                     SFEFaceLandmarks face_landmarks[SFE_FACE_LANDMARK_COUNT],
                     uint32_t face_size_extension, float max_face_size,
                     SFEFaceCrop *out_crop);

#ifdef __cplusplus
} // extern "C"
#endif
