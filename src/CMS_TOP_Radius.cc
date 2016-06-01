// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Tools/ParticleIdUtils.hh"
#include "Rivet/Projections/ChargedLeptons.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/DressedLeptons.hh"
#include "Rivet/Projections/MissingMomentum.hh"
#include "Rivet/Tools/ParticleIdUtils.hh"
#include "Rivet/Projections/VetoedFinalState.hh"
#include "Rivet/Tools/RivetMT2.hh"
#include "HepMC/PdfInfo.h"

namespace Rivet {
  
  
  class CMS_TOP_Radius : public Analysis {
    
  private:

    std::map<string,Histo1DPtr> histos;
    

  public:

    /// CTOR
    CMS_TOP_Radius()  : 
      Analysis("CMS_TOP_Radius")
    { 
      //setBeams(PROTON, PROTON);
    }


    /// Book histograms and initialise projections before the run
    void init() 
    {
      // define the acceptance of the final state
      const FinalState fs(-5.0,5.0,0.0*GeV);
      addProjection(fs, "FS");

      //photons
      IdentifiedFinalState gammafs(fs);
      gammafs.acceptIdPair(PID::PHOTON);
      
      //charged leptons
      Cut ptcut = Cuts::range(Cuts::pT, 20, MAXDOUBLE);  
      Cut etacut = Cuts::range(Cuts::eta, -2.5, 2.5);
      Cut ptetacut = ptcut && etacut;  
      IdentifiedFinalState lep_id(ptetacut);
      lep_id.acceptIdPair(PID::MUON);
      lep_id.acceptIdPair(PID::ELECTRON);
      DressedLeptons ewdressedleptons(gammafs, lep_id, 0.1, ptetacut, true, true);
      addProjection(ewdressedleptons, "Leptons");

      // neutrinos
      IdentifiedFinalState nu_id;
      nu_id.acceptNeutrinos();
      addProjection(nu_id, "neutrinos");

      // remove neutrinos, electrons and muons and get the jets
      VetoedFinalState vfs(fs);
      vfs.addVetoOnThisFinalState(ewdressedleptons);
      vfs.addVetoOnThisFinalState(nu_id);
      FastJets jets(vfs, FastJets::ANTIKT, 0.5);
      addProjection(jets, "Jets");

      //for pTmiss use visible final state particles with |eta|<5 
      addProjection(VisibleFinalState(-5,5),"vfs");

      // get the charged particles
      ChargedFinalState chfs(fs);             
      addProjection(chfs, "chfs"); 

      // Add MissingMomentum proj to calc MET
      MissingMomentum vismom(fs);
      addProjection(vismom, "MissingET");
      
      //book histograms
      histos["inistate"]  = bookHisto1D("inistate",3,0,3);
      histos["xsec"]      = bookHisto1D("xsec",1,0,1);
      histos["dphill"]    = bookHisto1D("dphill", 20,0,2*M_PI);      
      histos["ht"]        = bookHisto1D("ht", 20,0,1000);
    }
    
    
    ///  the per-event analysis goes here
    void analyze(const Event& event) 
    {

      const double weight = event.weight();

      //pdf and cross section info
      const HepMC::PdfInfo *pdf=event.genEvent()->pdf_info();
      int iniStateBin(0);
      if( pdf->id1()==21 && pdf->id2()==21) iniStateBin=2;
      else if(pdf->id1()==21 || pdf->id2()==21) iniStateBin=1;
      histos["inistate"]->fill(iniStateBin,weight);
      histos["xsec"]->fill(0,weight);

      //require 2 leptons
      const std::vector<DressedLepton> &leptons     = applyProjection<DressedLeptons>(event, "Leptons").dressedLeptons();
      if ( leptons.size() < 2) vetoEvent; 

      //require ee/mm/em
      int ch=leptons[0].abspid() * leptons[1].abspid();
      if( ch != PID::ELECTRON * PID::MUON && ch != PID::ELECTRON*PID::ELECTRON && ch!=PID::MUON*PID::MUON ) vetoEvent;

      //require opposite sign 
      if( leptons[0].charge() * leptons[1].charge() >=0 ) vetoEvent; 
      
      // jets
      Jets bjets,otherjets;
      const FastJets& jetpro = applyProjection<FastJets>(event, "Jets");
      Cut ptcut = Cuts::range(Cuts::pT, 30, MAXDOUBLE);
      Cut etacut = Cuts::range(Cuts::eta, -4.7, 4.7);
      Cut ptetacut = ptcut && etacut; 
      const Jets alljets = jetpro.jetsByPt(ptetacut);
      foreach (const Jet& jet, alljets) 
	{
	  if(fabs(jet.eta())<2.5 && jet.containsBottom()) bjets.push_back(jet);
	  else otherjets.push_back(jet);
	}
      if(bjets.size()<2) vetoEvent;

      //missing transverse energy
      const FourMomentum vismom = applyProjection<MissingMomentum>(event, "MissingET").visibleMomentum();
      const FourMomentum missmom(-vismom.px(),-vismom.py(),0,vismom.pt());

      //variables to plot
      float ht=leptons[0].pt()+leptons[1].pt()+bjets[0].pt()+bjets[1].pt()+missmom.pt();
      float dphill=deltaPhi(leptons[0],leptons[1]);
      if(ht>500) dphill += M_PI;

      //fill histograms
      histos["dphill"]->fill(dphill,weight);
      histos["ht"]->fill(ht,weight);      
    }
    
    /// Normalise histograms by the cross section
    void finalize() 
    {
      double norm = crossSection()/sumOfWeights();
      for(std::map<string,Histo1DPtr>::iterator it = histos.begin();
	  it != histos.end();
	  it++)
	scale(it->second,norm);
    }
  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_TOP_Radius);

}
