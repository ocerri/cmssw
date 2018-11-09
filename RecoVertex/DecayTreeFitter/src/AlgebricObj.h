#ifndef _ALGEBRIC_OBJ_
#define _ALGEBRIC_OBJ_

#include <Math/SVector.h>
#include <Math/SMatrix.h>

typedef ROOT::Math::SVector<double,3> AlgebraicVector3;
typedef ROOT::Math::SMatrix<double,3,3,ROOT::Math::MatRepSym<double,3> > AlgebraicSymMatrix33;
// typedef ROOT::Math::SMatrix<double,7,7,ROOT::Math::MatRepStd<double,7,7> > AlgebraicMatrix77;
// typedef ROOT::Math::SMatrix<double,6,7,ROOT::Math::MatRepStd<double,6,7> > AlgebraicMatrix67;
// typedef ROOT::Math::SMatrix<double,7,6,ROOT::Math::MatRepStd<double,7,6> > AlgebraicMatrix76;

#endif
