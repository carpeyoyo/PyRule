// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 29, 2017
// Functions for working with column matrices
// See included License file for license

#ifndef COLUMN_MATRIX_H
#define COLUMN_MATRIX_H

#include <stdlib.h>
#include <math.h>

void FourByFour_FourByFour(float *x, float *y, float *z);

float *MultipleMatrices(float **array, size_t size);

void FourByFour_FourByOne(float *x, float *y, float *z);

void FourDimensionalTranslation(float *matrix, float x, float y, float z, float *return_matrix);

float *OrthoReplacement(float left, float right, float bottom, float top, float nearVal, float farVal);

void ThreeByOne_OneByThree(float *a, float *b, float *answer);

float *LookAtReplacement(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);

void Normalize(float *array, size_t size);

float DotProduct(float *a, float *b, size_t length);

float *CrossProduct3D(float *a, float *b);

float *SurfaceNormalVector3D(float *a, float *b, float *c);

float clamp(float value, float low, float high);

// Matrices
float *ScaleMatrix(float x, float y, float z);

float *TranslationMatrix(float x, float y, float z);

float *XRotationMatrix(float angle);

float *YRotationMatrix(float angle);

float *ZRotationMatrix(float angle);

float *IdentityMatrix(void);

float *OrthographicMatrix(float right, float left, float top, float bottom, float far, float near);

float *PerspectiveMatrix(float right, float left, float top, float bottom, float far, float near);

float *ReturnCopyMatrix(float *matrix);

#endif
