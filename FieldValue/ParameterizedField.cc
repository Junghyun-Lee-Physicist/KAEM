#include "ParameterizedField.hh"
#include <cmath>

using namespace std;
//https://blockdmask.tistory.com/307
//cmath에 sqrt와 fabs, exp, cbrt 모두 있음

ParameterizedField::ParameterizedField()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

ParameterizedField::~ParameterizedField()
{
	//https://coding-factory.tistory.com/697 , 소멸자
	//https://boycoding.tistory.com/249
}

void ParameterizedField::SetCurrentPosition( const double Setx, const double Sety, const double Setz)
{
	x = Setx;
	y = Sety;
	z = Setz;
}

double* ParameterizedField::GetParaField() const
{

	double Field[3];
	//static 선언 필요한가?
	//https://zetawiki.com/wiki/C%EC%96%B8%EC%96%B4_%EB%B0%B0%EC%97%B4_%EB%B0%98%ED%99%98%ED%95%98%EA%B8%B0

	double r = 0.0;
	double W_un = 0.0;
	double U_un = 0.0;
	double z0 = 0.1;
	double x0 = 0.1;
	double zeta = (fabs(z)+z0)/z0;
	double xi = (fabs(x)+x0)/x0;

	r = sqrt(x*x + y*y);
	
	double zeta_sqrt = sqrt(zeta);
	double zeta_triple = zeta_sqrt * zeta_sqrt * zeta_sqrt;

	double B0 = 0.006885 * zeta_triple * sqrt(xi);

    double b_B = 2.40 / ((r+1.0) * (r+1.0));
    double m_B = 0.60 * exp(-r/0.2);
    double c_B = (0.80+4.0*r) * exp(-fabs(z)/0.08);

    double f_B = b_B + m_B*(z-1.0) + c_B;
    double B = B0*f_B;


	if(z>=0 && x>=0)
	{
		W_un = cbrt(fabs(z)) / (fabs(x) + x0);
		U_un = -cbrt(fabs(x)) / (fabs(z) + z0);
	}
    
	else if(z<0 && x>=0)
	{
		W_un = -cbrt(fabs(z)) / (fabs(x) + x0);
		U_un = -cbrt(fabs(x)) / (fabs(z) + z0);
	}
    
	else if(z<0 && x<0)
	{
		W_un = -cbrt(fabs(z)) / (fabs(x) + x0);
		U_un = +cbrt(fabs(x)) / (fabs(z) + z0);
	}
    
	else if(z>=0 && x<0)
	{
		W_un = cbrt(fabs(z)) / (fabs(x) + x0);
		U_un = cbrt(fabs(x)) / (fabs(z) + z0);
	}

	double epsilon = 1e-6;
	double alpha = sqrt(W_un*W_un + U_un*U_un+ epsilon);

	double W = W_un / alpha;
	double U = U_un / alpha;
	//normarlized된 방향코사인, direction cosine

	double Bz = W*B;
	double Br = U*B;

    double phi = 0.0;
	//if(x!=0) phi = atan2(y / x);
	if(x!=0) phi = atan2(y, x);
	else phi = M_PI/2.0;

	double Bx = Br*cos(phi);
	double By = Br*sin(phi);

	Field[0] = Bx;
	Field[1] = By;
	Field[2] = Bz;

	return Field;
	//https://zetawiki.com/wiki/C%EC%96%B8%EC%96%B4_%EB%B0%B0%EC%97%B4_%EB%B0%98%ED%99%98%ED%95%98%EA%B8%B0

}

