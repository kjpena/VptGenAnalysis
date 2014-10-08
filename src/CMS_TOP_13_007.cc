// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Tools/ParticleIdUtils.hh"
#include "Rivet/Projections/ChargedLeptons.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/MissingMomentum.hh"
#include "Rivet/Tools/ParticleIdUtils.hh"
#include "Rivet/Projections/VetoedFinalState.hh"

namespace Rivet {
  
  
  class CMS_TOP_13_007 : public Analysis {
    
    enum UERegions{TT_INC,TT_TOWARD, TT_TRANS, TT_AWAY};

  private:

    Histo1DPtr _h_chMult[4], _h_chSumPt[4], _h_chAvgPt[4];

  public:

    /// CTOR
    CMS_TOP_13_007()  : Analysis("CMS_TOP_13_007")
    { 
      //setBeams(PROTON, PROTON);
    }


    /// Book histograms and initialise projections before the run
    void init() 
    {
      // define the acceptance of the final state
      const FinalState fs(-2.5,2.5,0.0*GeV);
      addProjection(fs, "FS");
      
      // muons
      IdentifiedFinalState mufs(-2.5,2.5,20*GeV);
      mufs.acceptIdPair(PID::MUON);
      addProjection(mufs, "Muons");

      // electrons
      IdentifiedFinalState elfs(-2.5,2.5,20*GeV);
      elfs.acceptIdPair(PID::ELECTRON);
      addProjection(elfs, "Electrons");

      // muon neutrinos
      //IdentifiedFinalState munufs(-5.0,5.0,0*GeV);
      //munufs.acceptIdPair(PID::NU_MU);
      //addProjection(munufs, "MuonNus");

      // electron neutrinos
      //IdentifiedFinalState elnufs(-5.0,5.0,0*GeV);
      //elnufs.acceptIdPair(PID::NU_E);
      //addProjection(elnufs, "ElectronNus");

      // remove neutrinos, electrons and muons and get the jets
      VetoedFinalState vfs(fs);
      vfs.vetoNeutrinos();
      vfs.addVetoPairDetail(PID::MUON, 20.0*GeV, MAXDOUBLE);
      vfs.addVetoPairDetail(PID::ELECTRON, 20.0*GeV, MAXDOUBLE);
      addProjection(vfs, "VFS");
      addProjection(FastJets(vfs, FastJets::ANTIKT, 0.5), "Jets");
      addProjection(MissingMomentum(vfs), "MissingET");
      addProjection(ChargedLeptons(FinalState(-2.5, 2.5, 20*GeV)), "LFS");

      // get the charged particles
      ChargedFinalState chfs(fs);             
      addProjection(chfs, "chfs"); 

      // histogram booking
      for(int ireg=0; ireg<4; ireg++)
	{
	  char buf[50];
	  sprintf(buf,"chMult-r%d",ireg);  _h_chMult[ireg]  = bookHisto1D(buf,200,0,200);
	  sprintf(buf,"chSumPt-r%d",ireg); _h_chSumPt[ireg] = bookHisto1D(buf,50,0,500);
	  sprintf(buf,"chAvgPt-r%d",ireg); _h_chAvgPt[ireg] = bookHisto1D(buf,25,0,25);
	}
    }
    
    
    ///  the per-event analysis goes here
    void analyze(const Event& event) 
    {

      const double weight = event.weight();

      // loop over the charged leptons
      const ChargedLeptons& lfs = applyProjection<ChargedLeptons>(event, "LFS");
      MSG_DEBUG("Charged lepton multiplicity = " << lfs.chargedLeptons().size());
      foreach (Particle lepton, lfs.chargedLeptons()) {
	MSG_DEBUG("Lepton pT = " << lepton.momentum().pT());
      }
      if (lfs.chargedLeptons().size() < 2) {
	MSG_DEBUG("Event failed lepton multiplicity cut");
        vetoEvent;
      }

      const ParticleVector sortedMuons       = applyProjection<IdentifiedFinalState>(event, "Muons").particlesByPt();
      const ParticleVector sortedElectrons   = applyProjection<IdentifiedFinalState>(event, "Electrons").particlesByPt();
      
      // loop over the muons
      //MSG_DEBUG("Muon multiplicity = " << sortedMuons.size());
      //foreach (Particle mu, sortedMuons ) 
      //{
      //MSG_DEBUG("Muon pT = " << mu.momentum().pT());
      //}
      if ( sortedMuons.size() < 1) 
	{
	  MSG_DEBUG("Event failed muon multiplicity cut");
	  vetoEvent;
	}

      // loop over the electrons
      //MSG_DEBUG("Electron multiplicity = " << sortedElectrons.size());
      //foreach (Particle ele, sortedElectrons ) 
      //{
      //MSG_DEBUG("Electron pT = " << ele.momentum().pT());
      //}
      if ( sortedElectrons.size() < 1) 
	{
	  MSG_DEBUG("Event failed electron multiplicity cut");
	  vetoEvent;
	}

      // check opposite sign lepton charges
      int chlep1 = PID::charge(sortedMuons[0]) ;
      int chlep2 = PID::charge(sortedElectrons[0]) ;
      int chlep  = chlep1 * chlep2;
      if(chlep>=0) 
	{
	  MSG_DEBUG("Event failed opposite sign cut");
	  vetoEvent;
	}

      // MET cut (not applied)
      const MissingMomentum& met = applyProjection<MissingMomentum>(event, "MissingET");
      //MSG_DEBUG("Vector ET  =" << met.visibleMomentum().perp() << " GeV");
      //if ( met.visibleMomentum().perp() < 30*GeV) 
      //{
      //MSG_DEBUG("Event failed missing ET cut");
      //vetoEvent;
      //}

      FourMomentum missvec(0.0,0.0,0.0,0.0);
      missvec = -met.visibleMomentum(); 
      missvec.setPz(0.0); // no information about Pz
      missvec.setE(missvec.perp()); // assume neutrinos are massless

      // jet multiplicity requirement
      const FastJets& jetpro = applyProjection<FastJets>(event, "Jets");
      const Jets alljets = jetpro.jetsByPt();
      if (alljets.size() < 2) 
	{
	  MSG_DEBUG("Event failed jet multiplicity cut");
	  vetoEvent;
	}

      // jets minimum jet pt requirement
      const Jets jets = jetpro.jetsByPt(30*GeV);
      //foreach (const Jet& jet, jets) 
      //{
      //MSG_DEBUG("Jet pT = " << jet.momentum().pT()/GeV << " GeV");
      //}
      if (jets.size() < 2) 
	{
	  MSG_DEBUG("Event failed jet pT cut");
	  vetoEvent;
	}

      // b-tagging
      //this should change to take into account b-tag and mistag rates
      Jets bjets;
      foreach (const Jet& jet, jets) 
	{
	  if (jet.containsBottom()) 
	    {
	      bjets.push_back(jet);
	    } 
	}
      
      //MSG_DEBUG("Number of b-jets = " << bjets.size());
      if (bjets.size() != 2) 
	{
	  MSG_DEBUG("Event failed b-tagging cut");
	  vetoEvent;
	}
      // end event selection
      
      // get the ttbar four vector
      const FourMomentum ttbar = bjets[0].momentum() + bjets[1].momentum() + sortedMuons[0].momentum() + sortedElectrons[0].momentum() + missvec ;
      
      // now loop over all charged particles
      float pi = 3.14159;
      std::vector<int> chMult(4,0);
      std::vector<float> chSumPt(4,0);
      const FinalState& chfs = applyProjection<FinalState>(event, "chfs");
      foreach (const Particle& p, chfs.particles()) 
	{	
	  // remove the selected leptons
	  float dR = min(deltaR(sortedMuons[0],p.momentum()),deltaR(sortedElectrons[0],p.momentum()));
	  if(dR==0.) continue;

	  // remove particles matching the b-jets
	  if(bjets[0].containsParticle(p) || bjets[1].containsParticle(p)) continue;

	  // pt and/or eta cut on particels
	  if(p.momentum().pT() < 0.5 || abs(p.momentum().eta()) > 2.1) continue;

	  //analyse particle
	  //const int pid = p.pdgId();
	  float dphi = deltaPhi(ttbar,p.momentum());
	  float pt = p.momentum().pT();
	  
	  int tt_reg(TT_INC);
	  if(dphi < 1./3.*pi || dphi>5./3.*pi) tt_reg=TT_TOWARD;
	  if(dphi > 2./3.*pi && dphi<4./3.*pi) tt_reg=TT_AWAY;
	  if((dphi > 1./3.*pi && dphi<2./3.*pi) || (dphi > 4./3.*pi && dphi<5./3.*pi))  tt_reg=TT_TRANS;
	  chMult[0]  ++;    chMult[tt_reg]  ++;
	  chSumPt[0] += pt; chSumPt[tt_reg] += pt;
	} // loop over particles

      //fill histograms
      for(size_t ireg=0; ireg<chMult.size(); ireg++)
	{
	  _h_chMult[ireg]->fill(chMult[ireg],weight);
	  _h_chSumPt[ireg]->fill(chSumPt[ireg], weight);
	  if(chMult[ireg]==0) continue;
	  _h_chAvgPt[ireg]->fill(chSumPt[ireg]/chMult[ireg],weight);
	}
    }
    
    
    /// Normalise histograms etc., after the run
    void finalize() 
    {
      
      /// @todo Normalise, scale and otherwise manipulate histograms here

      // scale(_h_YYYY, crossSection()/sumOfWeights()); # norm to cross section
      //normalize(_h_YYYY); # normalize to unity
      for(size_t ireg=0; ireg<4; ireg++)
	{
	  normalize(_h_chMult[ireg] );
	  normalize(_h_chSumPt[ireg] );
	  normalize(_h_chAvgPt[ireg] );
	}
    }

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_TOP_13_007);

}
