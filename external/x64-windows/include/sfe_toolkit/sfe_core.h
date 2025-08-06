/**
 * @file sfe_core.h
 * @author Innovatrics - SmartFace Embedded
 * @date 16.5.2023
 * @brief File containing API of sfe_core library as part of SFE Toolkit
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Supported methods for Hardware ID (HWID) computation
typedef enum SFEHwidMethod {
  SFE_HWID_METHOD_AUTO = 0,
  SFE_HWID_METHOD_DISK_ID = 1,
  SFE_HWID_METHOD_MAC = 2,
  SFE_HWID_METHOD_SERIAL_NUMBER = 3,
  SFE_HWID_METHOD_IMEI = 4,
  SFE_HWID_METHOD_SM_BIOS = 5,
  SFE_HWID_METHOD_AMAZON = 6,
  SFE_HWID_METHOD_APP_ID = 7,
  SFE_HWID_METHOD_PHY = 8,
  SFE_HWID_METHOD_MAC_ADDRESS = 9,
  SFE_HWID_METHOD_SYS = 10,
  SFE_HWID_METHOD_ANDROID_ID = 11,
} SFEHwidMethod;

/// @brief License sources
typedef enum SFELicenseSource {
  /// @brief License was not read.
  SFE_LICENSE_SOURCE_NONE = 0,
  /// @brief License was read from a file.
  SFE_LICENSE_SOURCE_FILE = 1,
  /// @brief License was read from memory.
  SFE_LICENSE_SOURCE_MEMORY = 2,
  /// @brief License was read from a USB token.
  SFE_LICENSE_SOURCE_TOKEN = 3,
  /// @brief License was read from an environment variable.
  SFE_LICENSE_SOURCE_ENV = 4,
} SFELicenseSource;

/// @brief Supported image formats.
typedef enum SFEImageFormat {
  SFE_IMAGE_FORMAT_PNG = 0,
  SFE_IMAGE_FORMAT_JPEG = 1,
  SFE_IMAGE_FORMAT_GIF = 2,
  SFE_IMAGE_FORMAT_WEBP = 3,
  SFE_IMAGE_FORMAT_PNM = 4,
  SFE_IMAGE_FORMAT_TIFF = 5,
  SFE_IMAGE_FORMAT_TGA = 6,
  SFE_IMAGE_FORMAT_DDS = 7,
  SFE_IMAGE_FORMAT_BMP = 8,
  SFE_IMAGE_FORMAT_ICO = 9,
  SFE_IMAGE_FORMAT_HDR = 10,
  SFE_IMAGE_FORMAT_OPENEXR = 11,
  SFE_IMAGE_FORMAT_FARBFELD = 12,
  SFE_IMAGE_FORMAT_AVIF = 13,
} SFEImageFormat;

/// @brief Raw owned raster image representation, HWC|BGR order.
/// @note For non-owning variant use `SFEImageView`.
/// @warning This type retains ownership of the data, call `sfeImageFree` to
/// free the allocated image data.
typedef struct SFEImage {
  /// @brief Width of the image in pixels
  size_t width;
  /// @brief Height of the image in pixels
  size_t height;
  /// @brief Pointer to raw HWC|BGR data, size is width * height * 3.
  unsigned char *data;
} SFEImage;

/// @brief Raw raster image view, HWC|BGR order.
/// @note For owning variant use `SFEImage`.
typedef SFEImage SFEImageView;

/// @brief Bounding box
typedef struct SFEBbox {
  /// @brief X coordinate of the top left corner of the bounding box relative to
  /// source image size - range <0,1>
  float x;
  /// @brief Y coordinate of the top left corner of the bounding box relative to
  /// source image size - range <0,1>
  float y;
  /// @brief Width of the bounding box relative to source image size - range
  /// <0,1>
  float width;
  /// @brief Height of the bounding box relative to source image size - range
  /// <0,1>
  float height;
} SFEBbox;

/// @brief Error type used to hold optional error message.
/// @note In case of no error NULL is returned.
/// @warning Use `sfeErrorFree` to release memory associated with returned
/// errors.
typedef void *SFEError;

/// @brief Solver provides an abstract interface over inference models and
/// engines.
/// @warning Use `sfeSolverFree` to release memory associated with the solver.
typedef void *SFESolver;

/// @brief Solver parameter used to configure solvers
typedef struct SFESolverParameter {
  /// @brief parameter name
  const char *name;
  /// @brief parameter value
  const char *value;
} SFESolverParameter;

/// @brief Face entity type, used to group templates for entity identification.
/// This type is compatible with uuid_v4 that can be used to generate unique ids
typedef struct SFEEntity {
  uint8_t uuid[16];
} SFEEntity;

/// @brief Candidate for identification by template
typedef struct SFETemplateIdentificationCandidate {
  /// @brief matching score
  float score;
  /// @brief index of the template from the input gallery
  size_t index;
} SFETemplateIdentificationCandidate;

/// @brief Candidate for identification by entity
typedef struct SFEEntityIdentificationCandidate {
  /// @brief matching score
  float score;
  /// @brief candidate entity id
  SFEEntity entity;
} SFEEntityIdentificationCandidate;

/// @brief  Get version of the toolkit library.
/// @note The returned pointer is statically allocated, don't free it.
/// @return NULL terminated C string containing the version.
const char *const sfeVersion();

/// @brief Free memory associated with `SFEError`.
/// @param[in] error Error to free.
void sfeErrorFree(SFEError error);

/// @brief Get error message.
/// @param[in] error Error to get message from.
/// @return NULL terminated C string containing the error message.
/// @warning The returned C string is only valid as long as `sfeErrorFree` was
/// not called for the given error.
const char *sfeErrorMessage(SFEError error);

/// @brief Get Hardware ID (HWID) of the current machine.
/// @note Some methods for HWID computation are not supported on some platforms.
/// If unsure, use `SFE_HWID_METHOD_AUTO` that will choose a default method for
/// the current platform.
/// @param[out] out_hwid Pointer to a buffer where the HWID will be stored.
/// Providing a null pointer will determine the necessary buffer size.
/// @param[in,out] in_out_hwid_len IN: Total size of the out_hwid buffer. OUT: Actual
/// size of the data written to the out_hwid buffer.
/// @param[in] method A method to use for HWID computation.
/// @return Error in case the HWID computation fails or an invalid parameter is
/// provided (e.g. buffer length is smaller than the size of the HWID).
SFEError sfeHwidGet(char *out_hwid, size_t *in_out_hwid_len,
                    SFEHwidMethod method);

/// @brief Validate a license for this product.
/// This process conducts checks to confirm the license's validity for this
/// product and the current machine. Upon successful validation, the product is
/// ready for use and SFELicenseSource will be populated.
/// @note If no license source information is needed, a null pointer can be
/// supplied.
/// @param[in] info Pointer to a `SFELicenseSource`.
/// @return Upon failure, the returned error will contain a descriptive message
/// for the reason behind the failure.
SFEError sfeLicenseValidate(SFELicenseSource *out_license_source);

/// @brief Set the license data via environment variable.
/// The environment variable that will be set: ILICENSE_DATA.
/// The provided data will be converted into a base64 string internally
/// since the ILICENSE_DATA environment variable expects data in this format.
/// @param[in] data License data.
/// @param[in] data_len Length of the license data.
void sfeLicenseSet(const unsigned char *data, const size_t data_len);

/// @brief Create new solver from solver file.
/// @param[in] solver_file Solver file to create the solver from.
/// @param[in] solver_parameters Pointer to collection of solver parameters.
/// @param[in] solver_parameters_count Number of solver parameters.
/// @param[out] out_solver New solver.
/// @return Error in case the solver fails to load.
SFEError sfeSolverCreateWithParameters(
    const char *solver_file, const SFESolverParameter *solver_parameters,
    size_t solver_parameters_count, SFESolver *out_solver);

/// @brief Create new solver from solver file.
/// @param[in] solver_file Solver file to create the solver from.
/// @param[out] out_solver New solver.
/// @return Error in case the solver fails to load.
/// @deprecated Use `sfeSolverCreateWithParameters` instead.
inline SFEError sfeSolverCreate(const char *solver_file,
                                SFESolver *out_solver) {
  return sfeSolverCreateWithParameters(solver_file, NULL, 0, out_solver);
}

/// @brief Free memory associated with `SFESolver`.
/// @param[in] solver Solver to free.
void sfeSolverFree(SFESolver solver);

/// @brief Load `SFEImage` from raw image data of various formats. Eg. PNG, JPEG
/// ..
/// @param[in] data Image data to load image from.
/// @param[in] data_len Size of the image data in bytes.
/// @param[out] out_image Raw raster image.
/// @return Error in case of unknown image type.
/// @warning The returned `SFEImage` retains ownership of the image data, call
/// `sfeImageFree` to release the associated memory.
SFEError sfeImageLoad(const unsigned char *data, size_t data_len,
                      SFEImage *out_image);

/// @brief Save `SFEImage` into a buffer encoding it to specified image_format.
/// @param[in] image Image to save.
/// @param[in] image_format Image format to encode to.
/// @param[out] out_data Image data to save image into.
/// @param[in,out] in_out_data_len IN: Total size of the out_data buffer. OUT: Actual
/// size of the data written to the buffer.
/// @return Error in image format conversion or in case the buffer size is too
/// low.
SFEError sfeImageSave(SFEImageView image, SFEImageFormat image_fromat,
                      unsigned char *out_data, size_t *in_out_data_len);

/// @brief Try to get information from image data without fully decoding it.
/// @param[in] data Image data to load image from.
/// @param[in] data_len Size of the image data in bytes.
/// @param[out] out_width Width of the image data.
/// @param[out] out_height Height of the image data.
/// @param[out] out_image_format Format of the image data.
/// @return Error in case of unknown image type.
SFEError sfeImageInfo(const unsigned char *data, size_t data_len,
                      size_t *out_width, size_t *out_height,
                      SFEImageFormat *out_image_format);

/// @brief Free memory associated with `SFEImage`.
/// @param[in] image Image to free.
void sfeImageFree(SFEImage image);

/// @brief Transpose RGB<->BGR color channel order.
/// @param[in] image Image to color transpose.
/// @return Error
SFEError sfeImageColorTranspose(SFEImageView image);

/// @brief Resize image.
/// @param[in] image Source image view to resize.
/// @param[in] width Target resize width.
/// @param[in] height Target resize height.
/// @param[out] out_image Resized image.
/// @return Error
SFEError sfeImageResize(SFEImageView image, size_t width, size_t height,
                        SFEImage *out_image);

/// @brief Resize image with maintaining aspect ratio. Whole resized image will
/// be fitted and centered inside output image with black edges.
/// @param[in] image Source image view to resize.
/// @param[in] width Target resize width.
/// @param[in] height Target resize height.
/// @param[out] out_image Resized image.
/// @return Error
SFEError sfeImageResizeWithAspectRatio(SFEImageView image, size_t width,
                                       size_t height, SFEImage *out_image);

#ifdef __cplusplus
} // extern "C"
#endif
