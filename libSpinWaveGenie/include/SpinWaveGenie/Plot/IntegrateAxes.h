//
//  IntegrateAxes.h
//  spin_wave_genie
//
//  Created by Hahn, Steven E. on 5/7/14.
//
//

#ifndef __IntegrateAxes__
#define __IntegrateAxes__

#include <iostream>
#include <memory>
#include <vector>
#include <deque>
#include "SpinWaveGenie/Plot/SpinWavePlot.h"
#include "SpinWaveGenie/Containers/HKLDirections.h"
#include "SpinWaveGenie/Containers/Energies.h"

namespace SpinWaveGenie
{

class IntegrateAxes : public SpinWavePlot
{
public:
  IntegrateAxes(const IntegrateAxes &other);
  IntegrateAxes(std::unique_ptr<SpinWavePlot> resFunction, HKLDirections directions, double tol = 0.01,
                int maxEval = 100);
  std::vector<double> getCut(double kx, double ky, double kz);
  std::unique_ptr<SpinWavePlot> clone();
  const Cell &getCell() const;
  const Energies &getEnergies();
  void setEnergies(Energies energies);
  ~IntegrateAxes(){};

private:
  std::unique_ptr<SpinWavePlot> resolutionFunction;
  HKLDirections integrationDirections;
  int maximumEvaluations;
  double tolerance;
  double kx, ky, kz;
  std::vector<double> calculateIntegrand(std::deque<double> &x);
  std::vector<double> xmin, xmax;
};
}
#endif /* defined(__IntegrateAxes__) */
