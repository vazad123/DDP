#ifndef __B1MATERIALLIST_H__
#define __B1MATERIALLIST_H__

// Additional Header Files
#include <globals.hh>
#include <map>
#include <vector>

using std::vector;
using std::map;

// Root Header Files
#include "TDirectory.h"
#include "TFile.h"

class DarwinMaterials {
 public:
  DarwinMaterials();
  ~DarwinMaterials();
    
  void DefineMaterials();

 private:
  //G4double pLXeElectricField;
  //G4double pGXeElectricField;
};

#endif