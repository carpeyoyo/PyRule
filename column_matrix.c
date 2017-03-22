// Joshua Mazur (carpeyoyo.github.io)
// Last Edited: Mar. 21, 2017
// Functions for working with column matrices
// See included License file for license

#include "column_matrix.h"
#include <math.h>

void FourByFour_FourByFour(float *x, float *y, float *z){
  float x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15;
  float y0,y1,y2,y3,y4,y5,y6,y7,y8,y9,y10,y11,y12,y13,y14,y15;
  if ((x != NULL) && (y != NULL) && (z != NULL)){
    // values
    x0 = x[0];
    x1 = x[1];
    x2 = x[2];
    x3 = x[3];
    x4 = x[4];
    x5 = x[5];
    x6 = x[6];
    x7 = x[7];
    x8 = x[8];
    x9 = x[9];
    x10 = x[10];
    x11 = x[11];
    x12 = x[12];
    x13 = x[13];
    x14 = x[14];
    x15 = x[15];

    y0 = y[0];
    y1 = y[1];
    y2 = y[2];
    y3 = y[3];
    y4 = y[4];
    y5 = y[5];
    y6 = y[6];
    y7 = y[7];
    y8 = y[8];
    y9 = y[9];
    y10 = y[10];
    y11 = y[11];
    y12 = y[12];
    y13 = y[13];
    y14 = y[14];
    y15 = y[15];

    // Multiplication
    z[0] = x0*y0 + x4*y1 + x8*y2 + x12*y3;
    z[1] = x1*y0 + x5*y1 + x9*y2 + x13*y3;
    z[2] = x2*y0 + x6*y1 + x10*y2 + x14*y3;
    z[3] = x3*y0 + x7*y1 + x11*y2 + x15*y3;

    z[4] = x0*y4 + x4*y5 + x8*y6 + x12*y7;
    z[5] = x1*y4 + x5*y5 + x9*y6 + x13*y7;
    z[6] = x2*y4 + x6*y5 + x10*y6 + x14*y7;
    z[7] = x3*y4 + x7*y5 + x11*y6 + x15*y7;

    z[8] = x0*y8 + x4*y9 + x8*y10 + x12*y11;
    z[9] = x1*y8 + x5*y9 + x9*y10 + x13*y11;
    z[10] = x2*y8 + x6*y9 + x10*y10 + x14*y11;
    z[11] = x3*y8 + x7*y9 + x11*y10 + x15*y11;

    z[12] = x0*y12 + x4*y13 + x8*y14 + x12*y15;
    z[13] = x1*y12 + x5*y13 + x9*y14 + x13*y15;
    z[14] = x2*y12 + x6*y13 + x10*y14 + x14*y15;
    z[15] = x3*y12 + x7*y13 + x11*y14 + x15*y15;
  }
}

float *MultipleMatrices(float **array, size_t size){
  // Multiples all matrices in array to size specified.
  // Pre: Pointer to array of matrixes, size of that array.
  // Post: Will return NULL if size is less then two, otherwise pointer to 
  size_t current;
  float *temp1;
  float *temp2;
  float *answer;
  float *to_delete;
  int use_temp1;

  if (size < 2){
    answer = NULL;
  }
  else{
    temp1 = (float *) calloc(sizeof(float),16);
    // First two matrices
    FourByFour_FourByFour(array[1],array[0],temp1);
    current = 2;
    use_temp1 = 1;
    // Rest
    if (current < size){
      temp2 = (float *) calloc(sizeof(float),16);
      while (current < size){
	if (use_temp1){
	  FourByFour_FourByFour(array[current],temp1,temp2);
	  answer = temp2;
	  use_temp1 = 0;
	}
	else{
	  FourByFour_FourByFour(array[current],temp2,temp1);
	  answer = temp1;
	  use_temp1 = 1;
	}
	current++;
      }
      // Deleting whichever matrix is no longer needed. 
      if (use_temp1 == 1){
	free(temp2);
      }
      else{
	free(temp1);
      }
    }
    else{
      answer = temp1;
    }
  }
  return answer;
}

void FourByFour_FourByOne(float *x, float *y, float *z){
  float x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15;
  float y0,y1,y2,y3;
  if ((x != NULL) && (y != NULL) && (z != NULL)){
    // values
    x0 = x[0];
    x1 = x[1];
    x2 = x[2];
    x3 = x[3];
    x4 = x[4];
    x5 = x[5];
    x6 = x[6];
    x7 = x[7];
    x8 = x[8];
    x9 = x[9];
    x10 = x[10];
    x11 = x[11];
    x12 = x[12];
    x13 = x[13];
    x14 = x[14];
    x15 = x[15];
    y0 = y[0];
    y1 = y[1];
    y2 = y[2];
    y3 = y[3];
    
    // Multiplication
    z[0] = x0*y0 + x4*y1 + x8*y2 + x12*y3;
    z[1] = x1*y0 + x5*y1 + x9*y2 + x13*y3;
    z[2] = x2*y0 + x6*y1 + x10*y2 + x14*y3;
    z[3] = x3*y0 + x7*y1 + x11*y2 + x15*y3;    
  }
}


void FourDimensionalTranslation(float *matrix, float x, float y, float z, float *return_matrix){
  // Translates the given matrix by the vector transpose([x,y,z]) and returns values in return_matrix
  // Pre: Both matrices must already be size 16
  // Post: return_matrix now contains values, or is left alone if matrix and return_matrix are equal to NULL.
  float *t;

  if ( (matrix != NULL) && (return_matrix != NULL) ){
  
    // Translation matrix
    t = (float *) malloc(sizeof(float)*16);
    
    // First column
    t[0] = 1.0;
    t[1] = 0.0;
    t[2] = 0.0;
    t[3] = 0.0;
    // Second column
    t[4] = 0.0;
    t[5] = 1.0;
    t[6] = 0.0;
    t[7] = 0.0;
    // Third Column
    t[8] = 0.0;
    t[9] = 0.0;
    t[10] = 1.0;
    t[11] = 0.0;
    // Fourth Column
    t[12] = x;
    t[13] = y;
    t[14] = z;
    t[15] = 1.0;

    // Multiplication
    FourByFour_FourByFour(t,matrix,return_matrix);

    free(t);
  }
}

float *OrthoReplacement(float left, float right, float bottom, float top, float nearVal, float farVal){
  // Replacement for glOrtho. Method found on opengl's website.
  float *r;
  float tx,ty,tz;

  r = (float *) calloc(sizeof(float),16);

  tx = -1.0 * ((right + left) / (right - left));
  ty = -1.0 * ((top + bottom) / (top - bottom));
  tz = -1.0 * ((farVal + nearVal) / (farVal-nearVal));

  // First Column
  r[0] = 2.0 / (right -left);
  r[1] = 0.0;
  r[2] = 0.0;
  r[3] = 0.0;
  // Second Column
  r[4] = 0.0;
  r[5] = 2.0 / (top - bottom);
  r[6] = 0.0;
  r[7] = 0.0;
  // Third Column
  r[8] = 0.0;
  r[9] = 0.0;
  r[10] = -2.0 / (farVal - nearVal);
  r[11] = 0.0;
  // Fourth Column
  r[12] = tx;
  r[13] = ty;
  r[14] = tz;
  r[15] = 1.0;

  return r;
}

void ThreeByOne_OneByThree(float *a, float *b, float *answer){
  // Cross product
  if ((a != NULL) && (b != NULL) && (answer != NULL)){
    answer[0] = (a[1] * b[2] ) - (a[2] * b[1]);
    answer[1] = (a[2] * b[0]) - (a[0] * b[2]);
    answer[2] = (a[0] * b[1]) - (a[1] * b[0]);
  }
}

float *LookAtReplacement(float eyeX,
			 float eyeY,
			 float eyeZ,
			 float centerX,
			 float centerY,
			 float centerZ,
			 float upX,
			 float upY,
			 float upZ){
  // Replaces gluLookAt. Method found on opengl's website.
  float *r;
  float *f;
  float *up;
  float *s;
  float *ss;
  float *u;
  float upn,fn,sn;

  r = (float *) calloc(sizeof(float),16);
  f = (float *) calloc(sizeof(float),3);
  up = (float *) calloc(sizeof(float),3);
  s = (float *) calloc(sizeof(float),3);
  ss = (float *) calloc(sizeof(float),3);
  u = (float *) calloc(sizeof(float),3);
  
  f[0] = centerX - eyeX;
  f[1] = centerY - eyeY;
  f[2] = centerZ - eyeZ;
  fn = sqrt((f[0] * f[0]) + (f[1] *f[1]) + (f[2] * f[2]));
  f[0] = f[0] / fn;
  f[1] = f[1] / fn;
  f[2] = f[2] / fn;

  upn = sqrt((upX*upX) + (upY*upY) + (upZ*upZ));
  up[0] = upX / upn;
  up[1] = upY / upn;
  up[2] = upZ / upn;

  ThreeByOne_OneByThree(f,up,s);

  sn = sqrt((s[0] * s[0]) + (s[1] * s[1]) + (s[2] * s[2]));
  ss[0] = s[0] / sn;
  ss[1] = s[1] / sn;
  ss[2] = s[2] / sn;

  ThreeByOne_OneByThree(ss,f,u);

  free(up);
  free(ss);

  // Return Matrix
  // First Column
  r[0] = s[0];
  r[1] = u[0];
  r[2] = -1.0 * f[0];
  r[3] = 0.0;
  // Second Column
  r[4] = s[1];
  r[5] = u[1];
  r[6] = -1.0 * f[1];
  r[7] = 0.0;
  // Third Column
  r[8] = s[2];
  r[9] = u[2];
  r[10] = -1.0 * f[2];
  r[11] = 0.0;
  // Fourth Column
  r[12] = 0.0;
  r[13] = 0.0;
  r[14] = 0.0;
  r[15] = 1.0;

  free(f);
  free(u);
  free(s);
  
  return r;
}

void Normalize(float *array, size_t size){
  // Normalizes array
  float dd;
  float value;
  int i;

  dd = 0;
  for (i=0; i<size; i++){
    value = array[i];
    dd += value * value;
  }

  dd = sqrt(dd);

  for (i=0; i<size; i++){
    value = array[i];
    array[i] = value / dd;
  }
  
}

float DotProduct(float *a, float *b, size_t length){
  // Returns Dot product.
  float answer;
  size_t i;
  
  answer = 0;

  for (i=0; i<length; i++){
    answer += a[i] * b[i];
  }
  
  return answer;
}

float *CrossProduct3D(float *a, float *b){
  // Finds cross product of given 3D vectors
  // Pre: Valid pointers to two three element float arrays
  // Post: Returns pointer to 3 element float array if pointers are valid.
  //       Array will need to be freed by caller.
  float *answer;

  answer = NULL;

  if ((a != NULL) && (b != NULL)){
    answer = (float *) calloc(sizeof(float),3);

    answer[0] = (a[1] * b[2]) - (a[2] * b[1]);
    answer[1] = (a[2] * b[0]) - (a[0] * b[2]);
    answer[2] = (a[0] * b[1]) - (a[1] * b[0]);
  }

  return answer;
}

float *SurfaceNormalVector3D(float *a, float *b, float *c){
  // Normalized Surface vector normal for given points
  // Pre: Three valid pointers to three element float arrays
  // Post: Returns normalized 3 element surface normal vector if pointers are valid,
  //       NULL othewise. Remember right hand rule when determining point order.
  float *answer;
  float u[3];
  float v[3];
  int i;

  answer = NULL;

  if ((a != NULL) && (b != NULL) && (c != NULL)){

    for (i=0; i<3; i++){
      u[i] = c[i] - a[i];
      v[i] = b[i] - a[i];
    }

    answer = CrossProduct3D(u,v);

    if (answer != NULL){
      Normalize(answer,3);
    }
  }

  return answer;
}

float clamp(float value, float low, float high){
  // Returns value if between low and high.
  // Returns high if value is greater than high
  // Returns low if value is less than low
  float answer;
  if (value < low){
    answer = low;
  }
  else if (value > high){
    answer = high;
  }
  else{
    answer = value;
  }
  return answer;
}
  
// Matrices

float *ScaleMatrix(float x, float y, float z){
  // Produces Scale matrix
  // Pre: The x, y, and z values for scaling.
  // Post: Creates 16 element matrix that needs to be freed using free.
  float *r;

  r = (float *) calloc(sizeof(float),16);

  // First Column
  r[0] = x;
  r[1] = 0.0;
  r[2] = 0.0;
  r[3] = 0.0;
  // Second Column
  r[4] = 0.0;
  r[5] = y;
  r[6] = 0.0;
  r[7] = 0.0;
  // Third Column
  r[8] = 0.0;
  r[9] = 0.0;
  r[10] = z;
  r[11] = 0.0;
  // Fourth Column
  r[12] = 0.0;
  r[13] = 0.0;
  r[14] = 0.0;
  r[15] = 1.0;

  return r;
}

float *TranslationMatrix(float x, float y, float z){
  // Produces Translation Matrix
  // Pre: The x, y, and z values for translation
  // Post: Creates 16 element matrix that needs to be freed during free.
  float *r;
  
  r = (float *) calloc(sizeof(float),16);

  // First Column
  r[0] = 1.0;
  r[1] = 0.0;
  r[2] = 0.0;
  r[3] = 0.0;
  // Second Column
  r[4] = 0.0;
  r[5] = 1.0;
  r[6] = 0.0;
  r[7] = 0.0;
  // Third Column
  r[8] = 0.0;
  r[9] = 0.0;
  r[10] = 1.0;
  r[11] = 0.0;
  // Fourth Column
  r[12] = x;
  r[13] = y;
  r[14] = z;
  r[15] = 1.0;

  return r;
}

float *XRotationMatrix(float angle){
  // Produces Rotation Matrix around x axis
  // Pre: The angle to rotate around x axis in radians.
  // Post: Returns 16 element matrix that needs freed using free.
  float sin_value,cos_value;
  float *r;

  sin_value = sin(angle);
  cos_value = cos(angle);
  
  r = (float *) calloc(sizeof(float),16);
  
  // First Column
  r[0] = 1.0;
  r[1] = 0.0;
  r[2] = 0.0;
  r[3] = 0.0;
  // Second Column
  r[4] = 0.0;
  r[5] = cos_value;
  r[6] = sin_value;
  r[7] = 0.0;
  // Third Column
  r[8] = 0.0;
  r[9] = -1.0 * sin_value;
  r[10] = cos_value;
  r[11] = 0.0;
  // Fourth Column
  r[12] = 0.0;
  r[13] = 0.0;
  r[14] = 0.0;
  r[15] = 1.0;

  return r;
}

float *YRotationMatrix(float angle){
  // Produces Rotation Matrix around y axis
  // Pre: The angle to rotate around y axis in radians
  // Post: Returns 16 element matrix that needs freed using free.
  float sin_value,cos_value;
  float *r;

  sin_value = sin(angle);
  cos_value = cos(angle);
  
  r = (float *) calloc(sizeof(float),16);
  
  // First Column
  r[0] = cos_value;
  r[1] = 0.0;
  r[2] = -1.0 * sin_value;
  r[3] = 0.0;
  // Second Column
  r[4] = 0.0;
  r[5] = 1.0;
  r[6] = 0.0;
  r[7] = 0.0;
  // Third Column
  r[8] = sin_value;
  r[9] = 0.0;
  r[10] = cos_value;
  r[11] = 0.0;
  // Fourth Column
  r[12] = 0.0;
  r[13] = 0.0;
  r[14] = 0.0;
  r[15] = 1.0;

  return r;
}

float *ZRotationMatrix(float angle){
  // Produces Rotation Matrix around z axis.
  // Pre: Angle value to rotate around z axis in radians.
  // Post: Returns 16 element array that needs freed using free.
  float sin_value,cos_value;
  float *r;

  sin_value = sin(angle);
  cos_value = cos(angle);
  
  r = (float *) calloc(sizeof(float),16);
  
  // First Column
  r[0] = cos_value;
  r[1] = sin_value;
  r[2] = 0.0;
  r[3] = 0.0;
  // Second Column
  r[4] = -1.0 * sin_value;
  r[5] = cos_value;
  r[6] = 0.0;
  r[7] = 0.0;
  // Third Column
  r[8] = 0.0;
  r[9] = 0.0;
  r[10] = 1.0;
  r[11] = 0.0;
  // Fourth Column
  r[12] = 0.0;
  r[13] = 0.0;
  r[14] = 0.0;
  r[15] = 1.0;

  return r;
}  

float *IdentityMatrix(void){
  // Produces identity matrix
  // Pre: None.
  // Post: returns 16 element matrix that needs to be freed using free.
  float *r;
  
  r = (float *) calloc(sizeof(float),16);
  
  // First Column
  r[0] = 1.0;
  r[1] = 0.0;
  r[2] = 0.0;
  r[3] = 0.0;
  // Second Column
  r[4] = 0.0;
  r[5] = 1.0;
  r[6] = 0.0;
  r[7] = 0.0;
  // Third Column
  r[8] = 0.0;
  r[9] = 0.0;
  r[10] = 1.0;
  r[11] = 0.0;
  // Fourth Column
  r[12] = 0.0;
  r[13] = 0.0;
  r[14] = 0.0;
  r[15] = 1.0;

  return r;
}

float *OrthographicMatrix(float right, float left, float top, float bottom, float far, float near){
  // Produces Orthographic Matrix
  // Pre: The square coordinates for the matrix.
  // Post: Returns 16 element list that must be freed using free.
  float *r;
  float zdifference,ydifference,xdifference;

  zdifference = far - near;
  ydifference = top - bottom;
  xdifference = right - left;
  
  r = (float *) calloc(sizeof(float),16);
  
  // First Column
  r[0] = 2.0 / xdifference;
  r[1] = 0.0;
  r[2] = 0.0;
  r[3] = 0.0;
  // Second Column
  r[4] = 0.0;
  r[5] = 2.0 / ydifference;
  r[6] = 0.0;
  r[7] = 0.0;
  // Third Column
  r[8] = 0.0;
  r[9] = 0.0;
  r[10] = -2.0 / zdifference;
  r[11] = 0.0;
  // Fourth Column
  r[12] = (-1.0 * (right + left)) / xdifference;
  r[13] = (-1.0 * (top + bottom)) / ydifference;
  r[14] = (-1.0 * (far + near)) / zdifference;
  r[15] = 1.0;

  return r;
}

float *PerspectiveMatrix(float right, float left, float top, float bottom, float far, float near){
  // Produces Prospective matrix
  // Pre: The right and left clipping planes, the top and bottom clipping planes, and the near and far clipping planes
  //      (both of which must be positive)
  // Post: Returns pointer to 16 element column array that will need to be freed using free.
  float *r;
  float zdifference,ydifference,xdifference;
  float double_near;

  r = (float *) calloc(sizeof(float),16);

  zdifference = far - near;
  ydifference = top - bottom;
  xdifference = right - left;
  double_near = 2.0 * near;
  
  // First Column
  r[0] = double_near / xdifference;
  r[1] = 0.0;
  r[2] = 0.0;
  r[3] = 0.0;
  // Second Column
  r[4] = 0.0;
  r[5] = double_near / ydifference;
  r[6] = 0.0;
  r[7] = 0.0;
  // Third Column
  r[8] = (right + left) / xdifference;
  r[9] = (top + bottom) / ydifference;
  r[10] = -1.0 * ((far + near) / zdifference);
  r[11] = -1.0;
  // Fourth Column
  r[12] = 0.0;
  r[13] = 0.0;
  r[14] = (-2.0 * far * near) / zdifference;
  r[15] = 0.0;
  
  return r;
}

float *ReturnCopyMatrix(float *matrix){
  // Returns copy of given matrix
  // Pre: Non-NULL sixteen element float array
  // Post: Returns pointer to sixteen element float array with the same values
  //       as matrix if matrix is not equal to NULL. Otherwise returns NULL.
  //       Will need to be freed using free.
  float *answer;
  int i;
  answer = NULL;
  if (matrix != NULL){
    answer = (float *) calloc(sizeof(float),16);
    for (i=0; i<16; i++){
      answer[i] = matrix[i];
    }
  }
  return answer;
}

// Angle from matrices
float XRotationAngle(float *matrix){
  // Retrieves roll angle from matrix
  // Pre: Column matrix to check
  // Post: Returns x angle if matrix is not NULL, NAN otherwise.
  float answer;
  answer = NAN;
  if (matrix != NULL){
    answer = atanf(matrix[6] / matrix[10]);
  }
  return answer;
}

float YRotationAngle(float *matrix){
  // Retrieves pitch angle from matrix
  // Pre: Column matrix to check
  // Post: Returns y angle if matrix is not NULL, NAN otherwise.
  float answer;
  float num;
  float den;
  answer = NAN;
  if (matrix != NULL){
    num = -1.0 * matrix[2];
    den = sqrtf(powf(matrix[6],2.0) + powf(matrix[10],2.0));
    answer = atanf(num / den);
  }
  return answer;
}

float ZRotationAngle(float *matrix){
  // Retrieves yaw angle from matrix
  // Pre: Column matrix to check
  // Post: Returns z angle if matrix is not NULL, NAN otherwise.
  float answer;
  answer = NAN;
  if (matrix != NULL){
    answer = atanf(matrix[6] / matrix[10]);
  }
  return answer;
}
