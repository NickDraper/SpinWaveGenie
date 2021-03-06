#include "SpinWaveGenie/Interactions/Interaction.h"
using namespace std;

namespace SpinWaveGenie
{

bool Interaction::operator<(const Interaction &other) const
{
  vector<string> sl1 = this->sublattices();
  vector<string> sl2 = other.sublattices();
  if (sl1.size() <= sl2.size())
  {
    for (size_t index = 0; index < sl1.size(); index++)
    {
      if (sl1[index] < sl2[index])
        return true;
      else if (sl1[index] > sl2[index])
        return false;
    }
    return true;
  }
  else // sl1.size() > sl2.size()
  {
    for (size_t index = 0; index < sl2.size(); index++)
    {
      if (sl1[index] < sl2[index])
        return true;
      else if (sl1[index] > sl2[index])
        return false;
    }
    return false;
  }
}

bool Interaction::operator==(const Interaction &other) const
{
  vector<string> sl1 = this->sublattices();
  vector<string> sl2 = other.sublattices();

  if (sl1.size() != sl2.size())
    return false;
  else if (sl1.size() == 1)
  {
    if (sl1[0].compare(sl2[0]) == 0)
      return true;
    else
      return false;
  }
  else // sl1.size() == 2
  {
    if (sl1[0].compare(sl2[0]) == 0 && sl1[1].compare(sl2[1]) == 0)
      return true;
    else
      return false;
  }
}
}
