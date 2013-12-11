#include <vector>
#include <cmath>

#include "Config.hpp"

// Disable obnoxious warnings from Eigen headers
#if defined (__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall" 
#pragma GCC diagnostic ignored "-Weffc++" 
#pragma GCC diagnostic ignored "-Wextra"
#include <Eigen/Dense>
#pragma GCC diagnostic pop
#elif (__INTEL_COMPILER)
#pragma warning push
#pragma warning disable "-Wall"
#include <Eigen/Dense>
#pragma warning pop
#endif

#include "PWCSolver.hpp"
#include "Vacuum.hpp"
#include "UniformDielectric.hpp"
#include "WaveletCavity.hpp"

// Disable obnoxious warnings from Google Test headers
#if defined (__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall" 
#pragma GCC diagnostic ignored "-Weffc++" 
#pragma GCC diagnostic ignored "-Wextra"
#include "gtest/gtest.h"
#pragma GCC diagnostic pop
#endif

class WaveletCavityTest : public ::testing::Test
{
	protected:
		WaveletCavity cavity;
		virtual void SetUp()
		{
			Eigen::Vector3d N(0.0, 0.0, 0.0);
			std::vector<Sphere> spheres;
			Sphere sph1(N, 1.0);
			spheres.push_back(sph1);
			double probeRadius = 1.385; // Probe Radius for water
	        int patchLevel = 2;
        	double coarsity = 0.5;
			cavity = WaveletCavity(spheres, probeRadius, patchLevel, coarsity);
			cavity.readCavity("molec_dyadic.dat");
			double permittivity = 78.39;
			Vacuum * gfInside = new Vacuum(2); // Automatic directional derivative
			UniformDielectric * gfOutside = new UniformDielectric(2, permittivity);
			int firstKind = 0;
			PWCSolver solver(gfInside, gfOutside, firstKind);
			solver.buildSystemMatrix(cavity);
			cavity.uploadPoints(solver.getQuadratureLevel(), solver.getT_(), false);
		}
};

TEST_F(WaveletCavityTest, size)
{
	int size = 4864;
	int actualSize = cavity.size();
	EXPECT_EQ(size, actualSize);
}

TEST_F(WaveletCavityTest, area)
{
	double area = 4.0 * M_PI * pow(1.0, 2);
 	double actualArea = cavity.getElementArea().sum();
	EXPECT_DOUBLE_EQ(area, actualArea);
//	EXPECT_NEAR(area, actualArea, 1.0e-12);
}

TEST_F(WaveletCavityTest, volume)
{
	double volume = 4.0 * M_PI * pow(1.0, 3) / 3.0;
	Eigen::Matrix3Xd elementCenter = cavity.getElementCenter();
	Eigen::Matrix3Xd elementNormal = cavity.getElementNormal();
	double actualVolume = 0;
        for ( int i = 0; i < cavity.size(); ++i )
	{
		actualVolume += cavity.getElementArea(i) * elementCenter.col(i).dot(elementNormal.col(i));
	}
	actualVolume /= 3;
	EXPECT_DOUBLE_EQ(volume, actualVolume);
//	EXPECT_NEAR(volume, actualVolume, 1.0e-12);
}