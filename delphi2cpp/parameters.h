#ifndef PARAMETERS_H
#define PARAMETERS_H

using namespace std;

struct Climate {
  double TMax;
  double TMin;
  double TMoy;
  double HMax;
  double HMin;
  double HMoy;
  double Vt;
  double Ins;
  double Rg;
  double ETP;
  double Rain;

  Climate(double TMax, double TMin, double TMoy, double HMax, double HMin,
          double HMoy, double Vt, double Ins, double Rg,
          double Rain) :
    TMax(TMax), TMin(TMin), TMoy(TMoy), HMax(HMax), HMin(HMin),
    HMoy(HMoy), Vt(Vt), Ins(Ins), Rg(Rg), ETP(-999), Rain(Rain)
  { }
};

class SamaraParameters {
 public:
    double getDouble(string s) {return doubles[s].first;}
    string getString(string s) {return strings[s].first;}
    Climate getTemp(int t){return climatics[t];}
    map < string, pair < double, string > > doubles;
    map < string, pair < string, string > > strings;
    vector < Climate > climatics;
};


#endif // PARAMETERS_H
