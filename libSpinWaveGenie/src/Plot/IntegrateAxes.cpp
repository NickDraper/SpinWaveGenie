//
//  IntegrateAxes.cpp
//  spin_wave_genie
//
//  Created by Hahn, Steven E. on 5/7/14.
//
//
#include "SpinWaveGenie/Plot/IntegrateAxes.h"
#include "SpinWaveGenie/Plot/AdaptiveSimpson.h"

using namespace std;

namespace SpinWaveGenie
{

IntegrateAxes::IntegrateAxes(const IntegrateAxes &other)
{
  resolutionFunction = move(other.resolutionFunction->clone());
  this->maximumEvaluations = other.maximumEvaluations;
  this->tolerance = other.tolerance;
  this->integrationDirections = other.integrationDirections;
}

IntegrateAxes::IntegrateAxes(unique_ptr<SpinWavePlot> resFunction, HKLDirections directions, double tol, int maxEvals)
{
  this->tolerance = tol;
  this->maximumEvaluations = maxEvals;
  this->integrationDirections = directions;
  this->resolutionFunction = move(resFunction);
}

std::vector<double> IntegrateAxes::calculateIntegrand(std::deque<double> &x)
{
  double tmpx = kx, tmpy = ky, tmpz = kz;
  // cout << dim << " dimensions" << endl;
  // cout << " " << tmpx << " " << tmpy << " " << tmpz << endl;

  for (unsigned i = 0; i != x.size(); i++)
  {
    // cout << i << "\t" << x[i] << endl;
    tmpx += x[i] * integrationDirections[i].v0;
    tmpy += x[i] * integrationDirections[i].v1;
    tmpz += x[i] * integrationDirections[i].v2;
  }
  // cout << endl;

  // cout << "** " << x[0] << endl;//<< " " << x[1] << " " << x[2] << endl;
  // cout << " " << tmpx << " " << tmpy << " " << tmpz << endl;

  return resolutionFunction->getCut(tmpx, tmpy, tmpz);
}

struct DivideValue
{
  double value;
  DivideValue(double v) { value = 1.0 / v; }
  void operator()(double &elem) const { elem *= value; }
};

std::vector<double> IntegrateAxes::getCut(double kxIn, double kyIn, double kzIn)
{
  kx = kxIn;
  ky = kyIn;
  kz = kzIn;

  int dim = integrationDirections.size();
  xmin.clear();
  xmin.reserve(dim);
  xmax.clear();
  xmax.reserve(dim);
  for (auto it = integrationDirections.begin(); it != integrationDirections.end(); it++)
  {
    // cout << -1.0*it->delta << " " << it->delta << endl;
    xmin.push_back(-1.0 * it->delta);
    xmax.push_back(it->delta);
  }

  double volume = 1.0;
  for (auto it = integrationDirections.begin(); it != integrationDirections.end(); ++it)
  {
    volume *= 2.0 * it->delta;
  }
  // cout << volume << endl;

  std::function<std::vector<double>(std::deque<double> & x)> funct =
      std::bind<std::vector<double>>(&IntegrateAxes::calculateIntegrand, this, std::placeholders::_1);
  AdaptiveSimpson test;
  test.setFunction(funct);
  test.setInterval(xmin, xmax);
  test.setPrecision(tolerance * volume);
  test.setMaximumDivisions(maximumEvaluations);
  std::vector<double> result = test.integrate();
  std::for_each(result.begin(), result.end(), DivideValue(volume));
  return result;
}

const Cell &IntegrateAxes::getCell() const { return resolutionFunction->getCell(); }

const Energies &IntegrateAxes::getEnergies() { return resolutionFunction->getEnergies(); }

void IntegrateAxes::setEnergies(Energies energiesIn) { resolutionFunction->setEnergies(energiesIn); }

std::unique_ptr<SpinWavePlot> IntegrateAxes::clone() { return unique_ptr<SpinWavePlot>(new IntegrateAxes(*this)); }
}
