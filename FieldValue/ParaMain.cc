
#include <iostream>
#include "ParameterizedField.hh"

using namespace std;

int main(){

	ParameterizedField Field;
	double p[3] = {0.900, 0.000, 0.500};
	Field.SetCurrentPosition(p[0], p[1], p[2]);
	double* a = Field.GetParaField();

	cout<<"x, y, z = "<<p[0]<<", "<<p[1]<<", "<<p[2]<<endl;
	cout<<"Bx, By, Bz = "<<a[0]<<", "<<a[1]<<", "<<a[2]<<endl;

	return 0;

}
