#include <iostream>
#include <fstream>
#include <string>

#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

#include "SurfaceFunction.h"
#include "Cavity.h"

/*

Methods for basic cavity class
written by Krzysztof Mozgawa, 2011

*/

SurfaceFunctionMap Cavity::functions;

void Cavity::writeOutput(string &filename){
    ofstream output;
    output.open(filename.c_str(), fstream::out);
    output << nElements << endl;
    for(int i=0; i < nElements; i++) {
		output << elementCenter(0,i) << " ";
		output << elementCenter(1,i) << " ";
		output << elementCenter(2,i) << " ";
		output << elementArea(i) << " ";
    }
    output.close();
}

double Cavity::compPolarizationEnergy(const std::string & potName, 
									  const std::string & chgName) 
{
	VectorXd & potVec = getFunction(potName).getVector();
	VectorXd & chgVec = getFunction(chgName).getVector();
	return potVec.dot(chgVec);
}
	
double Cavity::compPolarizationEnergy() {
	double ENN = compPolarizationEnergy("NucPot", "NucChg");
	double ENE = compPolarizationEnergy("NucPot", "EleChg");
	double EEN = compPolarizationEnergy("ElePot", "NucChg");
	double EEE = compPolarizationEnergy("ElePot", "EleChg");
//	cout << " E_ee " << EEE << " E_en " << EEN
//		 << " E_ne " << ENE << " E_nn " << ENN << endl;
	printf("E_ee = %.10E, E_en = %.10E, E_ne = %.10E, E_nn = %.10E\n", EEE, EEN, ENE, ENN);
	return 0.5 * (EEE + EEN + ENE + ENN);
}

ostream & operator<<(ostream & os, Cavity & cavity) {
	return cavity.printObject(os);
}

ostream & Cavity::printObject(ostream & os) {
	os << "Molecular cavity" << endl;
	os << "Nr. of tesserae: " << nElements;
    for(int i = 0; i < nElements; i++) {
		os << endl;
		os << i+1 << " ";
		os << elementCenter(0,i) << " ";
		os << elementCenter(1,i) << " ";
		os << elementCenter(2,i) << " ";
		os << elementArea(i) << " ";
    }
	return os;
}

/*
double Cavity::compPolarizationEnergy(std::string pot, std::string chg) {

}
*/
void Cavity::appendNewFunction(const std::string & name) {
	if(not this->isBuilt()) {
		std::cout << "Cavity not yet built!" << std::endl;
		exit(-1);
	}
	if (functions.count(name) == 0) {
		SurfaceFunction * function = new SurfaceFunction(name, nElements);
		pair<SurfaceFunctionMap::iterator, bool> retval;
		retval = functions.insert(SurfaceFunctionPair(name, function));
	}
}

void Cavity::setFunction(const std::string & name, double * values) {
	if(functions.count(name) == 0) {
		appendNewFunction(name);
	}
	SurfaceFunction * func = functions.find(name)->second;
	func->setValues(values);
}

SurfaceFunction & Cavity::getFunction(const std::string & name) {
	if(functions.count(name) == 0) {
		std::cout << "Function " << name << " does not exist" << std::endl;
		exit(-1);
	}
	SurfaceFunction * func = functions.find(name)->second;
	return * func;
}

SurfaceFunctionMap & Cavity::initSurfaceFunctionMap()
{
	return functions;
}

void Cavity::setMode(const string & type) {
	if (type == "Atoms") {
		setMode(Atoms);
	} else if (type == "Implicit") {
		setMode(Implicit);
	} else if (type == "Explicit") {
		setMode(Explicit);
	} else {
		exit(-1);
	}
}

void Cavity::setMode(int type) {
	switch (type) {
	case Atoms :
		mode = Atoms;
		break;
	case Implicit :
		mode = Implicit;
		break;
	case Explicit :
		mode = Explicit;
		break;
	default :
		exit(-1);
	}
}
