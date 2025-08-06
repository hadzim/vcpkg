/**
 * @file sfe_iris.h
 * @author Innovatrics - SmartFace Embedded
 * @date 2.1.2024
 * @brief File containing API of sfe_iris library as part of SFE Toolkit
 */

#pragma once

#include "sfe_core.h"

#ifdef __cplusplus
extern "C" {
#endif

// IRIS DETECTION

/// @deprecated Use SFEEntity instead, this type will be removed in future
typedef SFEEntity SFEIrisEntity;

/// @deprecated Replace with SFETemplateIdentificationCandidate
typedef SFETemplateIdentificationCandidate SFEIrisTemplateIdentificationCandidate;

/// @deprecated Replace with SFEEntityIdentificationCandidate
typedef SFEEntityIdentificationCandidate SFEIrisEntityIdentificationCandidate;

/// @brief Iris keypoint types
typedef enum SFEIrisKeypointType {
  SFE_IRIS_KEYPOINT_TYPE_TOP_LEFT = 0,
  SFE_IRIS_KEYPOINT_TYPE_BOTTOM_LEFT = 1,
  SFE_IRIS_KEYPOINT_TYPE_TOP_INNER_LEFT = 2,
  SFE_IRIS_KEYPOINT_TYPE_BOTTOM_INNER_LEFT = 3,
  SFE_IRIS_KEYPOINT_TYPE_TOP_INNER_RIGHT = 4,
  SFE_IRIS_KEYPOINT_TYPE_BOTTOM_INNER_RIGHT = 5,
  SFE_IRIS_KEYPOINT_TYPE_TOP_RIGHT = 6,
  SFE_IRIS_KEYPOINT_TYPE_BOTTOM_RIGHT = 7,
} SFEIrisKeypointType;

/// @brief Iris keypoint struct
typedef struct SFEIrisKeypoint {
  /// @brief Type of the keypoint
  SFEIrisKeypointType keypoint_type;
  /// @brief Confidence of the keypoint - range <0,1>
  float confidence;
  /// @brief X coordinate of the keypoint relative to source image - range <0,1>
  float x;
  /// @brief Y coordinate of the keypoint relative to source image - range <0,1>
  float y;
} SFEIrisKeypoint;

/// @brief Iris keypoints count
#define SFE_IRIS_KEYPOINT_COUNT 8

/// @brief Iris detection struct
typedef struct SFEIrisDetect {
  /// @brief Iris bounding box
  SFEBbox iris_bounding_box;
  /// @brief Pupil bounding box
  SFEBbox pupil_bounding_box;
  /// @brief Iris detection confidence - range <0,1>
  float confidence;
  /// @brief Keypoints detected on the eye
  SFEIrisKeypoint keypoints[SFE_IRIS_KEYPOINT_COUNT];
} SFEIrisDetect;

/// @brief Detect iris in the source image
/// @param[in] solver iris detection solver
/// @param[in] image source image
/// @param[out] out_iris_detection pointer to the iris detection struct where a
/// detected iris will be saved
/// @return Error in case of failed detection.
SFEError sfeIrisDetect(SFESolver solver, SFEImageView image,
                       SFEIrisDetect *out_iris_detection);

// IRIS TEMPLATE EXTRACTION AND MATCHING

/// @brief Iris template size in bytes
#define SFE_IRIS_TEMPLATE_SIZE 522

/// @brief Iris template struct
typedef struct SFEIrisTemplate {
  uint8_t data[SFE_IRIS_TEMPLATE_SIZE];
} SFEIrisTemplate;

/// @brief Iris template version struct
typedef struct SFEIrisTemplateVersion {
  /// @brief Major template version
  uint8_t version_major;
  /// @brief Minor template version
  uint8_t version_minor;
} SFEIrisTemplateVersion;

/// @brief Extract template from source image and given iris detection
/// @param[in] solver iris template extraction solver
/// @param[in] image source image
/// @param[in] detection iris detection detected in the source image
/// @param[out] out_iris_template extracted iris template
/// @return Error in case of failed extraction.
SFEError sfeIrisTemplateExtract(SFESolver solver, SFEImageView image,
                                const SFEIrisDetect *in_detection,
                                SFEIrisTemplate *out_iris_template);

/// @brief Match two iris templates
/// @param[in] template1 first template to match
/// @param[in] template2 second template to match
/// @param[out] out_matching_score matching score - range <0,1>
/// @return Error in case of failed matching.
SFEError sfeIrisTemplateMatch(const SFEIrisTemplate *template1,
                              const SFEIrisTemplate *template2,
                              float *out_matching_score);

/// @brief Get iris template version
/// @param[in] iris_template source iris template
/// @param[out] out_iris_template_version iris template version struct
/// @return Error in case of template checksum mismatch.
SFEError
sfeIrisTemplateVersion(const SFEIrisTemplate *iris_template,
                       SFEIrisTemplateVersion *out_iris_template_version);

/// @brief Get iris template quality
/// @param[in] iris_template source iris template
/// @param[out] out_iris_template_quality template quality - range <0,1>
/// @return Error in case of template checksum mismatch.
SFEError sfeIrisTemplateQuality(const SFEIrisTemplate *iris_template,
                                float *out_iris_template_quality);

/// @brief Get an ordered array of SFETemplateIdentificationCandidate from
/// probe's template best matches with templates in the gallery
/// @param[in] probe_iris_template probe template
/// @param[in] templates_gallery pointer to an array of SFIrisTemplate
/// @param[in] gallery_size number of templates in the gallery
/// @param[in] matching_score_threshold threshold for matching - range <0,1>
/// @param[out] out_candidates Pointer to an ordered array of candidates
/// @param[in,out] in_out_candidates_count IN: Expected number of candidates OUT: Number
/// of found candidates with matching score above the matching score threshold
/// @param[in] thread_count Number of threads to use in the identification - 0: use
/// all available cores, 1: use single thread, N: use N number of threads
/// @return Error in case of failed identification.
SFEError
sfeIrisTemplateIdentify(const SFEIrisTemplate *probe_iris_template,
                        const SFEIrisTemplate *templates_gallery,
                        size_t gallery_size, float matching_score_threshold,
                        SFETemplateIdentificationCandidate *out_candidates,
                        size_t *in_out_candidates_count, size_t thread_count);

/// @brief Get an ordered array of SFEEntityIdentificationCandidate from
/// probe's template best matches with templates in the gallery.
/// Entity is then used to group the results by entity using the best score of
/// any template associated with the entity.
/// @param[in] probe_iris_template probe template
/// @param[in] templates_gallery pointer to an array of SFEIrisTemplate
/// @param[in] entities_gallery pointer to an array of SFEEntity that
/// corresponds with templates_gallery
/// @param[in] gallery_size number of templates in gallery
/// @param[in] matching_score_threshold threshold for matching - range <0,1>
/// @param[out] out_candidates Pointer to an ordered array of candidates
/// @param[in,out] in_out_candidates_count IN: Expected number of candidates OUT: Number
/// of found candidates with matching score above the matching score threshold
/// @param[in] thread_count Number of threads to use in the identification - 0: use
/// all available cores, 1: use single thread, N: use N number of threads
/// @return Error in case of failed identification.
SFEError sfeIrisEntityIdentify(const SFEIrisTemplate *probe_iris_template,
                               const SFEIrisTemplate *templates_gallery,
                               const SFEEntity *entities_gallery,
                               size_t gallery_size,
                               float matching_score_threshold,
                               SFEEntityIdentificationCandidate *out_candidates,
                               size_t *in_out_candidates_count,
                               size_t thread_count);

/// @brief Tracker is a ByteTrack implementation for iris tracking across
/// multiple frames.
/// @warning Use `sfeIrisTrackerFree` to release memory associated with the
/// tracker.
typedef void *SFEIrisTracker;

/// @brief Tracked state
enum SFEIrisTrackedState {
  SFE_IRIS_TRACKED_STATE_NEW = 0,
  SFE_IRIS_TRACKED_STATE_TRACKED = 1,
  SFE_IRIS_TRACKED_STATE_LOST = 2,
  SFE_IRIS_TRACKED_STATE_REMOVED = 3,
};

/// @brief Tracked entity struct
typedef struct SFEIrisTracked {
  /// @brief Tracked detection
  SFEIrisDetect tracked;
  /// @brief Entity
  uint64_t id;
  /// @brief UUID
  SFEEntity uuid;
  /// @brief Tracking state
  SFEIrisTrackedState state;
} SFEIrisTracked;

/// @brief Create a new tracker
/// @param[in] track_threshold Tracking threshold
/// @param[in] high_threshold High threshold
/// @param[in] match_threshold Match threshold
/// @param[in] max_time_lost Maximum time lost
/// @param[out] out_tracker OUT: pointer to the created tracker
SFEError sfeIrisTrackerCreate(float track_threshold, float high_threshold,
                              float match_threshold, uint64_t max_time_lost,
                              SFEIrisTracker *out_tracker);

/// @brief Free the tracker
/// @param[in] tracker Tracker to free
SFEError sfeIrisTrackerFree(SFEIrisTracker tracker);

/// @brief Update the tracker
/// @param[in] tracker Tracker to update
/// @param[in] detections Detections to update the tracker with
/// @param[in] detections_count Number of detections
/// @return Error in case of failed update
SFEError sfeIrisTrackerUpdate(SFEIrisTracker tracker, SFEIrisDetect *detections,
                              size_t detections_count,
                              SFEIrisTracked *out_tracked,
                              size_t *out_tracked_count);

/// @brief Get lost entities after update
/// @param[in] tracker Tracker to get lost entities from
/// @param[out] out_entities OUT: pointer to the array of lost entities
/// @param[in,out] in_out_entities_count OUT: number of lost entities
/// @return Error in case of failed lost entities retrieval
SFEError sfeIrisTrackerLost(SFEIrisTracker tracker, SFEEntity *out_entities,
                            size_t *in_out_entities_count);

/// @brief Get removed entities after update
/// @param[in] tracker Tracker to get removed entities from
/// @param[out] out_entities OUT: pointer to the array of removed entities
/// @param[in,out] in_out_entities_count OUT: number of removed entities
/// @return Error in case of failed removed entities retrieval
SFEError sfeIrisTrackerRemoved(SFEIrisTracker tracker, SFEEntity *out_entities,
                               size_t *in_out_entities_count);

#ifdef __cplusplus
} // extern "C"
#endif
