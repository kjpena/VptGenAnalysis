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

namespace Rivet {
  
  
  class CMS_TOP_Dileptons : public Analysis {
    
  private:

    Histo1DPtr _h_ptpos;

  public:

    /// CTOR
    CMS_TOP_Dileptons()  : 
      Analysis("CMS_TOP_Dileptons")
    { 
      //setBeams(PROTON, PROTON);
    }


    /// Book histograms and initialise projections before the run
    void init() 
    {
      // define the acceptance of the final state
      const FinalState fs(-2.5,2.5,0.0*GeV);
      addProjection(fs, "FS");

      //photons
      IdentifiedFinalState gammafs(fs);
      gammafs.acceptIdPair(PID::PHOTON);
      
      //charged leptons
      IdentifiedFinalState lep_id(-2.5,2.5,20*GeV);
      lep_id.acceptIdPair(PID::MUON);
      lep_id.acceptIdPair(PID::ELECTRON);
      DressedLeptons ewdressedleptons(gammafs, lep_id, 0.1, true, -2.5, 2.5, 20*GeV, true);
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

      // histogram booking
      double bins_ptpos_gen[]={20,25,30,35,40,45,50,55,60,65,70,80,90,100,125,150,200};
      std::vector<double> bins_ptpos_gen_v(bins_ptpos_gen,bins_ptpos_gen+sizeof(bins_ptpos_gen)/sizeof(double));
      _h_ptpos  = bookHisto1D("ptpos",bins_ptpos_gen_v);

    }
    
    
    ///  the per-event analysis goes here
    void analyze(const Event& event) 
    {

      const double weight = event.weight();

      //require 2 leptons
      const std::vector<DressedLepton> &leptons     = applyProjection<DressedLeptons>(event, "Leptons").clusteredLeptons();
      if ( leptons.size() < 2) vetoEvent; 
      int leadLepIdx(0),trailerLepIdx(1);
      if(leptons[0].momentum().pT()<leptons[1].momentum().pT()) { leadLepIdx=1; trailerLepIdx=0; }
      for(size_t i=2; i<leptons.size(); i++)
	{
	  if(leptons[i].momentum().pT()>leptons[leadLepIdx].momentum().pT())
	    {
	      trailerLepIdx=leadLepIdx;
	      leadLepIdx=i;
	    }
	  else if(leptons[i].momentum().pT()>leptons[trailerLepIdx].momentum().pT())
	    {
	      trailerLepIdx=i;
	    }
	}

      //require opposite sign and op. flavour leptons
      if( leptons[leadLepIdx].charge() * leptons[trailerLepIdx].charge() >=0 ) vetoEvent; 
      if( leptons[leadLepIdx].abspid() * leptons[trailerLepIdx].abspid() != PID::ELECTRON * PID::MUON ) vetoEvent;

      // jet multiplicity 
      const FastJets& jetpro = applyProjection<FastJets>(event, "Jets");
      const Jets alljets = jetpro.jetsByPt(20*GeV,MAXDOUBLE,-2.5,2.5);
      if (alljets.size() < 2) vetoEvent;

      // jets minimum jet pt requirement
      const Jets jets = jetpro.jetsByPt(30*GeV,MAXDOUBLE,-2.5,2.5);
      if (jets.size() < 2)  vetoEvent;

      // b-tagging: this should change to take into account b-tag and mistag rates
      Jets bjets, otherjets;
      foreach (const Jet& jet, alljets) 
	{
	  if (jet.containsBottom() && bjets.size()<2) bjets.push_back(jet); 
	  else                                        otherjets.push_back(jet);
	}
      if(bjets.size()<2) vetoEvent;

      //fill histos
      float ptpos(leptons[leadLepIdx].charge()>0 ? leptons[leadLepIdx].pt() : leptons[trailerLepIdx].pt());
      _h_ptpos->fill(ptpos,weight);
    }
        
    /// Normalise histograms etc., after the run
    void finalize() 
    {
      
      /// @todo Normalise, scale and otherwise manipulate histograms here
      normalize(_h_ptpos);
    }
  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_TOP_Dileptons);

}
