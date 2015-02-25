#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TChain.h"
#include "TMath.h"
#include "TString.h"

#include "SusyAnaTools/Tools/NTupleReader.h"
#include "TauResponse.h"
#include "utils.h"

class BaseHistgram
{
 public:
  void BookHistgram(const char *);
  TFile *oFile;
  std::vector<TH1*> hTauResp(TauResponse::nBins());
  std::vector<TH1*> hTauvisible(TauResponse::nBins());
  std::vector<TH1*> hTaugenerated(TauResponse::nBins());
  TString Title1(int i);
  TString Title2(int j);
};

void BaseHistgram::BookHistgram(const char *outFileName)
{
  oFile = new TFile(outFileName, "recreate");
  for(unsigned int i = 0; i < TauResponse::nBins(); ++i) {
    hTauResp.at(i) = new TH1D(TauResponse::name(i),";p_{T}(visible) / p_{T}(generated);Count",50,0.,2.5);
    hTauResp.at(i)->Sumw2();

    hTauvisible.at(i) = new TH1D(Title1(i),";p_{T}(visible)[GeV];Events",50,0.,500.);
    hTauvisible.at(i)->Sumw2();

    hTaugenerated.at(i) = new TH1D(Title2(i),";p_{T}(generated)[GeV];Events",50,0.,500.);
    hTaugenerated.at(i)->Sumw2();

  }
}

TString BaseHistgram::Title1(int i){
  TString title = "htauvisible_";
  title += i;
  return title;
}
TString BaseHistgram::Title2(int j){
  TString title = "htaugenerated_";
  title += j;
  return title;
}

bool FillChain(TChain *chain, const TString &inputFileList)
{
  ifstream infile(inputFileList, ifstream::in);
  std::string buffer;
  if(!infile.is_open())
    {
      std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
      return false;
    }
  std::cout << "TreeUtilities : FillChain " << std::endl;
  while(1)
    {
      infile >> buffer;
      if(!infile.good()) break;
      //std::cout << "Adding tree from " << buffer.c_str() << std::endl;
      chain->Add(buffer.c_str());
    }
  std::cout << "No. of Entries in this tree : " << chain->GetEntries() << std::endl;
  return true;
}
