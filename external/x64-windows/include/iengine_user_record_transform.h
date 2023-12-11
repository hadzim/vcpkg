/*******************************************************************************
*
*	Filename		:	IEngine.h
*	Description		:	IEngine user record transform
*
*
*	Company			:	Innovatrics
*	Project			:	IEngine
*	Language		:	C++
*
*
*	Copyright (c)2022   All rights reserved
******************************************************************************/

#include <stdint.h>

#ifndef _INC_IENGINE_USER_RECORD_TRANSFORM_H
#define _INC_IENGINE_USER_RECORD_TRANSFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#define IENGINE_API

enum IEngine_RECORD_FORMAT
{
	VERIFICATION_RECORD,
	SEARCH_RECORD,
	ANSI_RECORD,
	ISO_RECORD,
	ANSI_PLUS_RECORD,
	ISO_PLUS_RECORD
};

// Geometrically transforms minutiae and ridges positions/angles. 
// First flips, then rotates (in clockwise direction).
// Possible values for rotationDegrees are 0, 90, 180, 270.
// Possible values for flipX and flipY are 0, 1.
IENGINE_API int IEngine_TransformUserRecord(uint8_t *userRecord, int flipX, int flipY, int rotationDegrees, uint8_t *outData, int *length, enum IEngine_RECORD_FORMAT format);

#ifdef __cplusplus
}
#endif

#endif
