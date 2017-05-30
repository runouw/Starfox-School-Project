/*=======================================================================
   Copyright 2010 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# include <math.h>
# include <gsim/gs_vec.h>

//===========================================================================

const GsVec GsVec::null ( 0, 0, 0 );
const GsVec GsVec::minusone ( -1.0f, -1.0f, -1.0f );
const GsVec GsVec::one ( 1.0f, 1.0f, 1.0f );
const GsVec GsVec::i ( 1.0f, 0.0f, 0.0f );
const GsVec GsVec::j ( 0.0f, 1.0f, 0.0f );
const GsVec GsVec::k ( 0.0f, 0.0f, 1.0f );

//============================== GsVec ====================================

void GsVec::rotx ( float radians )
 {
   rotx ( sinf(radians), cosf(radians) );
 }

void GsVec::roty ( float radians )
 {
   roty ( sinf(radians), cosf(radians) );
 }

void GsVec::rotz ( float radians )
 {
   rotz ( sinf(radians), cosf(radians) );
 }

void GsVec::normalize ()
 {
   float f = sqrtf(x*x + y*y + z*z);
   if ( f>0 ) { x/=f; y/=f; z/=f; }
 }

float GsVec::len ( float n )
 {
   float f = sqrtf (x*x + y*y + z*z);
   if ( f>0 ) { n/=f; x*=n; y*=n; z*=n; }
   return f;
 }

float GsVec::norm () const
 {
   return sqrtf ( x*x + y*y + z*z );
 }

float GsVec::normax () const
 {
   float a = GS_ABS(x);
   float b = GS_ABS(y);
   float c = GS_ABS(z);
   return GS_MAX3 ( a, b, c );
 }

void GsVec::cross ( const GsVec& v1, const GsVec& v2 )
 {
   x = v1.y*v2.z - v1.z*v2.y;
   y = v1.z*v2.x - v1.x*v2.z;
   z = v1.x*v2.y - v1.y*v2.x;
 }

//=================================== Friend Functions ===================================

GsVec cross ( const GsVec& v1, const GsVec& v2 )
 {
   return GsVec ( v1.y*v2.z - v1.z*v2.y,
                  v1.z*v2.x - v1.x*v2.z,
                  v1.x*v2.y - v1.y*v2.x  );
 }

void swap ( GsVec& v1, GsVec& v2 )
 {
   float tmp;
   GS_SWAP(v1.x,v2.x);
   GS_SWAP(v1.y,v2.y);
   GS_SWAP(v1.z,v2.z);
 }

float distmax ( const GsVec& v1, const GsVec& v2 )
 {
   float a = v1.x-v2.x;
   float b = v1.y-v2.y;
   float c = v1.z-v2.z;
   a = GS_ABS(a);
   b = GS_ABS(b);
   c = GS_ABS(c);
   return a>b? (a>c? a:c) : (b>c? b:c);
 }

float dist ( const GsVec& v1, const GsVec& v2 )
 {
   float a = v1.x-v2.x;
   float b = v1.y-v2.y;
   float c = v1.z-v2.z;
   return sqrtf (a*a + b*b + c*c);
 }

float dist2 ( const GsVec& v1, const GsVec& v2 )
 {
   float a = v1.x-v2.x;
   float b = v1.y-v2.y;
   float c = v1.z-v2.z;
   return a*a + b*b + c*c;
 }

float angle ( const GsVec& v1, const GsVec& v2 )
 {
   double n = sqrt(v1.x*v1.x+v1.y*v1.y+v1.z*v1.z) * sqrt(v2.x*v2.x+v2.y*v2.y+v2.z*v2.z);
   // n will be 0 if a given vector is null, so check that here:
   if ( n==0 ) return 0;
   // get the cosine of the angle:
   n = double(v1.x*v2.x + v1.y*v2.y + v1.z*v2.z) / n;
   // n has to be in [-1,1] and numerical aproximations may result in values
   // slightly outside the interval; so the test below is really needed:
   if ( n<-1.0 ) n=-1.0; else if ( n>1.0 ) n=1.0;
   return (float) acos ( n );
 }

float anglenorm ( const GsVec& v1, const GsVec& v2 )
 {
   return acosf ( dot(v1,v2) );
   double n = double(v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
   if ( n<-1.0 ) n=-1.0; else if ( n>1.0 ) n=1.0;
   return (float) acos ( n );
 }

GsVec normal ( const GsPnt& a, const GsPnt& b, const GsPnt& c )
 { 
   GsVec n; 
   n.cross ( b-a, c-a ); 
   n.normalize(); 
   return n; 
 }

static void matinverse ( float M[9], const GsVec &l1, const GsVec &l2, const GsVec &l3 )
 {
   M[0]=l2.y*l3.z-l2.z*l3.y; M[3]=l2.z*l3.x-l2.x*l3.z; M[6]=l2.x*l3.y-l2.y*l3.x;
   M[1]=l1.z*l3.y-l1.y*l3.z; M[4]=l1.x*l3.z-l1.z*l3.x; M[7]=l1.y*l3.x-l1.x*l3.y;
   M[2]=l1.y*l2.z-l1.z*l2.y; M[5]=l1.z*l2.x-l1.x*l2.z; M[8]=l1.x*l2.y-l1.y*l2.x;

   float d = l1.x*M[0] + l1.y*M[3] + l1.z*M[6];
   if ( GS_NEXTZ(d,gstiny) ) std::cout<<"Singular Matrix in gs_vec.cpp matinverse!\n";
   d = 1.0f/d;

   M[0]*=d; M[1]*=d; M[2]*=d; M[3]*=d; M[4]*=d; M[5]*=d; M[6]*=d; M[7]*=d; M[8]*=d;
 }

static void matposmult ( const GsVec& v, const float M[9], GsVec &r ) // r = v M
 {
   r.x = v.x*M[0] + v.y*M[3] + v.z*M[6];
   r.y = v.x*M[1] + v.y*M[4] + v.z*M[7];
   r.z = v.x*M[2] + v.y*M[5] + v.z*M[8];
 }

GsVec barycentric ( const GsPnt& a, const GsPnt& b, const GsPnt& c, const GsVec &p )
 {
   float m[9];
   GsVec k;
   matinverse ( m, a, b, c );
   matposmult ( m, p, k );
   return k;
 }

void transbarycentric ( GsPnt& a, GsPnt& b, GsPnt& c, const GsVec &k, const GsVec& v )
 {
   float k2 = k.x*k.x + k.y*k.y + k.z*k.z;

   a += (k.x+1.0f-k2)*v;
   b += (k.y+1.0f-k2)*v;
   c += (k.z+1.0f-k2)*v;
 }

int GsVec::compare ( const GsVec& v1, const GsVec& v2 )
 {
   if ( v1.x > v2.x ) return  1;
   if ( v1.x < v2.x ) return -1;
   if ( v1.y > v2.y ) return  1;
   if ( v1.y < v2.y ) return -1;
   if ( v1.z > v2.z ) return  1;
   if ( v1.z < v2.z ) return -1;
   return 0;
 }

//================================== End of File ===========================================

