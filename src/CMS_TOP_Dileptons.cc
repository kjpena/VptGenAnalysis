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

    Histo1DPtr _h_ptpos,_h_enpos,_h_ptsum,_h_ensum,_h_mll,_h_ptll;

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
      double bins_ptpos_gen[]=
	//{20,24,28,32,36,40,44,48,52,60,68,76,84,92,100,120,140,160,180};
	{19.80,27.41,33.03,38.65,44.65,51.43,59.39,69.64,84.30,112.04};
      std::vector<double> bins_ptpos_gen_v(bins_ptpos_gen,bins_ptpos_gen+sizeof(bins_ptpos_gen)/sizeof(double));
      _h_ptpos  = bookHisto1D("ptpos",bins_ptpos_gen_v);


      double bins_enpos_gen[]=
	{24.96,36.20,45.21,54.26,64.11,75.28,89.10,107.18,133.67,183.27};
      std::vector<double> bins_enpos_gen_v(bins_enpos_gen,bins_enpos_gen+sizeof(bins_enpos_gen)/sizeof(double));
      _h_enpos  = bookHisto1D("enpos",bins_enpos_gen_v);

      double bins_ptsum_gen[]=
	//{40,56,64,72,80,88,96,102,107,113,119,127,137,147,163,187,205,235,250};
	{53.08,68.41,78.69,88.05,97.43,107.51,118.85,133.14,153.57,190.53};
      std::vector<double>bins_ptsum_gen_v(bins_ptsum_gen,bins_ptsum_gen+sizeof(bins_ptsum_gen)/sizeof(double));
      _h_ptsum  = bookHisto1D("ptsum",bins_ptsum_gen_v);

      double bins_ensum_gen[]=
	//{55,75,87,99,111,122,131,140,149,160,170,180,190,205,225,250,275,375,475};
	{68.61,92.60,108.79,123.43,138.28,154.27,172.06,199.23,227.96,269.39};
      std::vector<double>bins_ensum_gen_v(bins_ensum_gen,bins_ensum_gen+sizeof(bins_ensum_gen)/sizeof(double));
      _h_ensum  = bookHisto1D("ensum",bins_ensum_gen_v);

      double bins_mll_gen[]=
	//{20,30,40,48,56,68,74,80,86,93,100,115,130,145,165,195,235,275,375};
	{27.39,47.45,61.52,73.87,85.99,98.88,113.90,132.31,157.47,201.49};
      std::vector<double>bins_mll_gen_v(bins_mll_gen,bins_mll_gen+sizeof(bins_mll_gen)/sizeof(double));
      _h_mll    = bookHisto1D("mll",bins_mll_gen_v);
      
      double bins_ptll_gen[]=
      //{15.13,30,41.76,51.57,60.23,70.22,79.93,89.23,104.48,136.92};
	{11.53,28.89,40.52,50.18,58.99,67.74,76.90,87.47,101.13,124.13};
      std::vector<double>bins_ptll_gen_v(bins_ptll_gen,bins_ptll_gen+sizeof(bins_ptll_gen)/sizeof(double));
      _h_ptll   = bookHisto1D("ptll",bins_ptll_gen_v);
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
      if (jets.size() < 1)  vetoEvent;

      // b-tagging: this should change to take into account b-tag and mistag rates
      Jets bjets, otherjets;
      foreach (const Jet& jet, alljets) 
	{
	  if (jet.containsBottom() ) bjets.push_back(jet); 
	  else                       otherjets.push_back(jet);
	}
      if(bjets.size()<1) vetoEvent;

      //fill histos
      float ptpos(leptons[leadLepIdx].charge()>0 ? leptons[leadLepIdx].pt() : leptons[trailerLepIdx].pt());
      float enpos(leptons[leadLepIdx].charge()>0 ? leptons[leadLepIdx].energy() : leptons[trailerLepIdx].energy());
      float ptsum=leptons[leadLepIdx].pt()+leptons[trailerLepIdx].pt();
      float ensum=leptons[leadLepIdx].energy()+leptons[trailerLepIdx].energy();
      _h_ptpos->fill(ptpos,weight);
      _h_enpos->fill(enpos,weight);
      _h_ptsum->fill(ptsum,weight);
      _h_ensum->fill(ensum,weight);

      const FourMomentum rec_dilepton=leptons[leadLepIdx].momentum()+leptons[trailerLepIdx].momentum();
      _h_mll->fill(rec_dilepton.mass(),weight);
      _h_ptll->fill(rec_dilepton.pt(),weight);

    }
        
    /// Normalise histograms etc., after the run
    void finalize() 
    {
      /// @todo Normalise, scale and otherwise manipulate histograms here
      normalize(_h_ptpos);
      normalize(_h_enpos);
      normalize(_h_ptsum);
      normalize(_h_ensum);
      normalize(_h_mll);
      normalize(_h_ptll);
    }
  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_TOP_Dileptons);

}
