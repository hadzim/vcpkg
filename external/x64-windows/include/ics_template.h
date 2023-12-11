/**
 * @file ics_template.h
 *
 * @copyright Copyright (c) 2020 Innovatrics s.r.o. All rights reserved.
 *
 * @maintainer Peter Tatrai <peter.tatrai@innovatrics.com>
 * @created 16.10.2020
 */

#ifndef INNOVATRICS_UPGRADE_TOOL_H
#define INNOVATRICS_UPGRADE_TOOL_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ICT_API_EXPORT
#   ifdef _WIN32
#       define ICT_API __declspec(dllexport)
#   else
#       define ICT_API  __attribute__ ((visibility ("default")))
#   endif
#else
#   ifdef _WIN32
#       define ICT_API __declspec(dllimport)
#   else
#       define ICT_API
#   endif
#endif

/* Summary
   Upgrades user template
   Parameters
   inputTemplateData :     [in] Pointer to user template data to be upgraded
   outputTemplateData :    [out] Pointer to the memory space where
                           upgraded template raw data will be saved
   outputTemplateLength :  [in/out] On input, length parameter is
                           interpreted as total size of allocated memory
                           pointed by templateData. On return, this
                           parameter will be equal to the total length of
                           user template.
   Return Value
   0 :             No error occurred.
   non null :      User template format is not supported or recognized.
   Remarks
   This function takes user template raw data and upgrades it into the newest
   template format version. User template consists of all fingerprint,
   face and iris templates contained within the user structure. User template
   does not contain images, custom data and tags. It contains only
   biometric templates with its positions (fingerprint and iris position).
   This function uses two-phase buffer retrieval algorithm.                                          */

ICT_API int IEngine_UpgradeUserTemplateVersion(const unsigned char *inputTemplateData, unsigned char *outputTemplateData, int *outputTemplateLength);

#ifdef __cplusplus
} //extern "C"
#endif

#endif //INNOVATRICS_UPGRADE_TOOL_H
