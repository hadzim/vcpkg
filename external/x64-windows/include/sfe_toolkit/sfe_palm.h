/**
 * @file sfe_palm.h
 * @author Innovatrics - SmartFace Embedded
 * @date 28.10.2024
 * @brief File containing API of sfe_palm library as part of SFE Toolkit
 */

#pragma once

#include "sfe_core.h"

#ifdef __cplusplus
extern "C" {
#endif

// PALM DETECTION

/// @deprecated Use SFEEntity instead, this type will be removed in future
typedef SFEEntity SFEPalmEntity;

/// @deprecated Replace with SFETemplateIdentificationCandidate
typedef SFETemplateIdentificationCandidate
    SFEPalmTemplateIdentificationCandidate;

/// @deprecated Replace with SFEEntityIdentificationCandidate
typedef SFEEntityIdentificationCandidate SFEPalmEntityIdentificationCandidate;

/// @brief Palm keypoint struct
typedef struct SFEPalmKeypoint {
  /// @brief X coordinate of the keypoint relative to source image - range <0,1>
  float x;
  /// @brief Y coordinate of the keypoint relative to source image - range <0,1>
  float y;
  /// @brief Confidence of the keypoint - range <0,1>
  float confidence;
} SFEPalmKeypoint;

/// @brief Palm keypoints count
#define SFE_PALM_KEYPOINT_COUNT 8

/// @brief Palm hand enum
typedef enum SFEHandSide {
  SFE_HAND_UNKNOWN = 0,
  SFE_HAND_LEFT = 1,
  SFE_HAND_RIGHT = 2,
} SFEHandSide;

/// @brief Palm orientation enum
typedef enum SFEHandOrientation {
  SFE_ORIENTATION_UNKNOWN = 0,
  SFE_ORIENTATION_PALMAR = 1,
  SFE_ORIENTATION_DORSAL = 2,
} SFEHandOrientation;


/// @brief Palm detection struct
typedef struct SFEPalmDetect {
  /// @brief Palm bounding box
  SFEBbox bounding_box;
  /// @brief Palm detection confidence - range <0,1>
  float confidence;
  /// @brief Palm hand
  SFEHandSide hand_side;
  /// @brief Palm orientation
  SFEHandOrientation hand_orientation;
  /// @brief Keypoints detected on the palm
  SFEPalmKeypoint keypoints[SFE_PALM_KEYPOINT_COUNT];
} SFEPalmDetect;

/// @brief Detect palm in the source image
/// @param[in] solver palm detection solver
/// @param[in] image source image
/// @param[in] threshold detection threshold - range <0,1>
/// @param[out] out_palm_detection pointer to a storage where for each detected palm
/// @param[in,out] in_out_palm_count IN: Available space in the out_palm_detection OUT: Number
/// of detected palms saved to out_palm_detection
/// @return Error in case of failed detection.
SFEError sfePalmDetect(SFESolver solver, SFEImageView image, float threshold,
                       SFEPalmDetect *out_palm_detection,
                       size_t *in_out_palm_count);

// PALM TEMPLATE EXTRACTION AND MATCHING

/// @brief Palm template size in bytes
#define SFE_PALM_TEMPLATE_SIZE 522

/// @brief Palm template struct
typedef struct SFEPalmTemplate {
  uint8_t data[SFE_PALM_TEMPLATE_SIZE];
} SFEPalmTemplate;

/// @brief Palm template version struct
typedef struct SFEPalmTemplateVersion {
  /// @brief Major template version
  uint8_t major;
  /// @brief Minor template version
  uint8_t minor;
  /// @brief Patch template version
  uint8_t patch;
} SFEPalmTemplateVersion;

/// @brief Extract template from source image and given palm detection
/// @param[in] solver palm extraction solver
/// @param[in] image source image
/// @param[in] in_detection palm detection detected in the source image
/// @param[out] out_palm_template extracted palm template
/// @return Error in case of failed extraction.
SFEError sfePalmTemplateExtract(SFESolver solver, SFEImageView image,
                                const SFEPalmDetect *in_detection,
                                SFEPalmTemplate *out_palm_template);

/// @brief Match two palm templates
/// @param[in] template1 first template to match
/// @param[in] template2 second template to match
/// @param[out] out_matching_score matching score - range <0,1>
/// @return Error in case of failed matching.
SFEError sfePalmTemplateMatch(const SFEPalmTemplate *template1,
                              const SFEPalmTemplate *template2,
                              float *out_matching_score);

/// @brief Get palm template version
/// @param[in] palm_template source palm template
/// @param[out] out_palm_template_version palm template version struct
/// @return Error in case of template checksum mismatch.
SFEError
sfePalmTemplateVersion(const SFEPalmTemplate *palm_template,
                       SFEPalmTemplateVersion *out_palm_template_version);

/// @brief Get an ordered array of SFETemplateIdentificationCandidate from
/// probe's template best matches with templates in the gallery
/// @param[in] probe_palm_template probe template
/// @param[in] templates_gallery pointer to an array of SFPalmTemplate
/// @param[in] gallery_size number of templates in the gallery
/// @param[in] matching_score_threshold threshold for matching - range <0,1>
/// @param[out] out_candidates Pointer to an ordered array of candidates
/// @param[in,out] in_out_candidates_count IN: Expected number of candidates OUT: Number
/// of found candidates with matching score above the matching score threshold
/// @param[in] thread_count Number of threads to use in the identification - 0: use
/// all available cores, 1: use single thread, N: use N number of threads
/// @return Error in case of failed identification.
SFEError
sfePalmTemplateIdentify(const SFEPalmTemplate *probe_palm_template,
                        const SFEPalmTemplate *templates_gallery,
                        size_t gallery_size, float matching_score_threshold,
                        SFETemplateIdentificationCandidate *out_candidates,
                        size_t *in_out_candidates_count, size_t thread_count);

/// @brief Get an ordered array of SFEEntityIdentificationCandidate from
/// probe's template best matches with templates in the gallery.
/// Entity is then used to group the results by entity using the best score of
/// any template associated with the entity.
/// @param[in] probe_palm_template probe template
/// @param[in] templates_gallery pointer to an array of SFEPalmTemplate
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
SFEError sfePalmEntityIdentify(const SFEPalmTemplate *probe_palm_template,
                               const SFEPalmTemplate *templates_gallery,
                               const SFEEntity *entities_gallery,
                               size_t gallery_size,
                               float matching_score_threshold,
                               SFEEntityIdentificationCandidate *out_candidates,
                               size_t *in_out_candidates_count,
                               size_t thread_count);

/// @brief Passive liveness score calculation
/// @param[in] solver passive liveness solver
/// @param[in] image source image
/// @param[in] palm_detect palm detection
/// @param[out] out_liveness_score OUT: normalized score of passive liveness
/// @return Error in case of failed passive liveness score calculation
SFEError sfePalmLivenessPassive(SFESolver solver, SFEImageView image,
                                const SFEPalmDetect *palm_detect,
                                float *out_liveness_score);

/// @brief Tracker is a ByteTrack implementation for palm tracking across
/// multiple frames.
/// @warning Use `sfePalmTrackerFree` to release memory associated with the
/// tracker.
typedef void *SFEPalmTracker;

/// @brief Tracked state
enum SFEPalmTrackedState {
  SFE_PALM_TRACKED_STATE_NEW = 0,
  SFE_PALM_TRACKED_STATE_TRACKED = 1,
  SFE_PALM_TRACKED_STATE_LOST = 2,
  SFE_PALM_TRACKED_STATE_REMOVED = 3,
};

/// @brief Tracked entity struct
typedef struct SFEPalmTracked {
  /// @brief Tracked detection
  SFEPalmDetect tracked;
  /// @brief Tracked ID
  uint64_t id;
  /// @brief UUID
  SFEEntity uuid;
  /// @brief Tracking state
  SFEPalmTrackedState state;
} SFEPalmTracked;

/// @brief Create a new tracker
/// @param[in] track_threshold Tracking threshold
/// @param[in] high_threshold High threshold
/// @param[in] match_threshold Match threshold
/// @param[in] max_time_lost Maximum time lost
/// @param[out] out_tracker OUT: pointer to the created tracker
SFEError sfePalmTrackerCreate(float track_threshold, float high_threshold,
                              float match_threshold, uint64_t max_time_lost,
                              SFEPalmTracker *out_tracker);

/// @brief Free the tracker
/// @param[in] tracker Tracker to free
SFEError sfePalmTrackerFree(SFEPalmTracker tracker);

/// @brief Update the tracker
/// @param[in] tracker Tracker to update
/// @param[in] detections Detections to update the tracker with
/// @param[in] detections_count Number of detections
/// @return Error in case of failed update
SFEError sfePalmTrackerUpdate(SFEPalmTracker tracker,
                              const SFEPalmDetect *detections,
                              size_t detections_count,
                              SFEPalmTracked *out_tracked,
                              size_t *out_tracked_count);

/// @brief Get lost entities after update
/// @param[in] tracker Tracker to get lost entities from
/// @param[out] out_entities OUT: pointer to the array of lost entities
/// @param[in,out] in_out_entities_count OUT: number of lost entities
/// @return Error in case of failed lost entities retrieval
SFEError sfePalmTrackerLost(SFEPalmTracker tracker, SFEEntity *out_entities,
                            size_t *in_out_entities_count);

/// @brief Get removed entities after update
/// @param[in] tracker Tracker to get removed entities from
/// @param[out] out_entities OUT: pointer to the array of removed entities
/// @param[in,out] in_out_entities_count OUT: number of removed entities
/// @return Error in case of failed removed entities retrieval
SFEError sfePalmTrackerRemoved(SFEPalmTracker tracker, SFEEntity *out_entities,
                               size_t *in_out_entities_count);

/// @brief Palm quality attributes struct
typedef struct SFEPalmQualityAttributes {
  /// @brief Sharpness of the palm image
  float sharpness;
  /// @brief Brightness of the palm image
  float brightness;
  /// @brief Hotspots score of the palm image
  float hotspots_score;
} SFEPalmQualityAttributes;

/// @brief Calculate palm image quality attributes from given source image and
/// palm detection
/// @param[in] image source image
/// @param[in] palm_detect palm detection
/// @param[out] out_quality_attributes structure containing `sharpness`, `brightness`, `hotspots_score`
/// @return Error in case of failed quality attributes calculation
SFEError sfePalmQualityAttributes(
    SFEImageView image,
    const SFEPalmDetect *palm_detect,
    SFEPalmQualityAttributes *out_quality_attributes);

#ifdef __cplusplus
} // extern "C"
#endif
