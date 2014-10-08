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
    
  private:

    Histo1DPtr _h_deltaPhi;
    Histo1DPtr _h_chmult;
    Histo1DPtr _h_chmult_toward;
    Histo1DPtr _h_chmult_transverse;
    Histo1DPtr _h_chmult_away;
    Histo1DPtr _h_sumPt;
    Histo1DPtr _h_sumPt_toward;
    Histo1DPtr _h_sumPt_transverse;
    Histo1DPtr _h_sumPt_away;
    Histo1DPtr _h_vecSumPt;
    Histo1DPtr _h_vecSumPt_toward;
    Histo1DPtr _h_vecSumPt_transverse;
    Histo1DPtr _h_vecSumPt_away;
    Histo1DPtr _h_vecDeltaPhi;

    // profiles
    // AIDA::IProfile1D* _p_chmult_ttpt;
    // AIDA::IProfile1D* _p_chmult_ttpt_toward;
    // AIDA::IProfile1D* _p_chmult_ttpt_transverse;
    // AIDA::IProfile1D* _p_chmult_ttpt_away;
    // AIDA::IProfile1D* _p_vecSumPt_ttpt;
    // AIDA::IProfile1D* _p_vecSumPt_ttpt_toward;
    // AIDA::IProfile1D* _p_vecSumPt_ttpt_transverse;
    // AIDA::IProfile1D* _p_vecSumPt_ttpt_away;
    // AIDA::IProfile1D* _p_avgPt_ttpt;
    // AIDA::IProfile1D* _p_avgPt_ttpt_toward;
    // AIDA::IProfile1D* _p_avgPt_ttpt_transverse;
    // AIDA::IProfile1D* _p_avgPt_ttpt_away;
    
    // various contol distributions
    Histo1DPtr _h_pt_tt;
    Histo1DPtr _h_pt_mu;
    Histo1DPtr _h_pt_ele;
    Histo1DPtr _h_eta_tt;
    Histo1DPtr _h_eta_mu;
    Histo1DPtr _h_eta_ele;
    Histo1DPtr _h_pt_jets;
    Histo1DPtr _h_eta_jets;
    Histo1DPtr _h_jetmult;

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
      _h_deltaPhi	     = bookHisto1D(15, 0, 1);
      _h_chmult		     = bookHisto1D(15, 0, 1);
      _h_chmult_toward	     = bookHisto1D(15, 0, 1);
      _h_chmult_transverse   = bookHisto1D(15, 0, 1);
      _h_chmult_away	     = bookHisto1D(15, 0, 1);
      _h_sumPt		     = bookHisto1D(15, 0, 1);
      _h_sumPt_toward	     = bookHisto1D(15, 0, 1);
      _h_sumPt_transverse    = bookHisto1D(15, 0, 1);
      _h_sumPt_away	     = bookHisto1D(15, 0, 1);
      _h_vecSumPt	     = bookHisto1D(15, 0, 1);
      _h_vecSumPt_toward     = bookHisto1D(15, 0, 1);
      _h_vecSumPt_transverse = bookHisto1D(15, 0, 1);
      _h_vecSumPt_away       = bookHisto1D(15, 0, 1);
      _h_vecDeltaPhi         = bookHisto1D(15, 0, 1);
      _h_pt_tt	             = bookHisto1D(15, 0, 1);
      _h_pt_mu	             = bookHisto1D(15, 0, 1);
      _h_pt_ele	             = bookHisto1D(15, 0, 1);
      _h_eta_tt	             = bookHisto1D(15, 0, 1);
      _h_eta_mu	             = bookHisto1D(15, 0, 1);
      _h_eta_ele             = bookHisto1D(15, 0, 1);
      _h_jetmult             = bookHisto1D(15, 0, 1);

      // _p_chmult_ttpt		  = bookProfile1D("d02-x01-y01", 15, 0., 300.);
      // _p_chmult_ttpt_toward	  = bookProfile1D("d02-x01-y02", 15, 0., 300.);
      // _p_chmult_ttpt_transverse   = bookProfile1D("d02-x01-y03", 15, 0., 300.);
      // _p_chmult_ttpt_away         = bookProfile1D("d02-x01-y04", 15, 0., 300.);
      // _p_vecSumPt_ttpt            = bookProfile1D("d02-x01-y05", 15, 0., 300.);
      // _p_vecSumPt_ttpt_toward     = bookProfile1D("d02-x01-y06", 15, 0., 300.);
      // _p_vecSumPt_ttpt_transverse = bookProfile1D("d02-x01-y07", 15, 0., 300.);
      // _p_vecSumPt_ttpt_away	  = bookProfile1D("d02-x01-y08", 15, 0., 300.);
      // _p_avgPt_ttpt		  = bookProfile1D("d02-x01-y09", 15, 0., 300.);
      // _p_avgPt_ttpt_toward	  = bookProfile1D("d02-x01-y10", 15, 0., 300.);
      // _p_avgPt_ttpt_transverse	  = bookProfile1D("d02-x01-y11", 15, 0., 300.);
      // _p_avgPt_ttpt_away	  = bookProfile1D("d02-x01-y12", 15, 0., 300.);
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
      //const ParticleVector sortedMuonNus     = applyProjection<IdentifiedFinalState>(event, "MuonNus").particlesByPt();
      //const ParticleVector sortedElectronNus = applyProjection<IdentifiedFinalState>(event, "ElectronNus").particlesByPt();
      
      // loop over the muons
      MSG_DEBUG("Muon multiplicity = " << sortedMuons.size());
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
      MSG_DEBUG("Electron multiplicity = " << sortedElectrons.size());
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
      _h_pt_tt->fill(ttbar.pT(),weight);
      _h_eta_tt->fill(ttbar.eta(),weight);
      _h_pt_mu->fill(sortedMuons[0].momentum().pT(),weight);
      _h_eta_mu->fill(sortedMuons[0].momentum().eta(),weight);
      _h_pt_ele->fill(sortedElectrons[0].momentum().pT(),weight);
      _h_eta_ele->fill(sortedElectrons[0].momentum().eta(),weight);
      _h_jetmult->fill(jets.size(),weight);
      
      // now loop over all charged particles
      int count_chrg = 0;
      int count_chrg_toward = 0;
      int count_chrg_transverse = 0;
      int count_chrg_away = 0;

      float sum_pt = 0;
      float sum_pt_toward = 0;
      float sum_pt_transverse = 0;
      float sum_pt_away = 0;

      FourMomentum particleFlux(0.,0.,0.,0.);
      FourMomentum particleFlux_toward(0.,0.,0.,0.);
      FourMomentum particleFlux_transverse(0.,0.,0.,0.);
      FourMomentum particleFlux_away(0.,0.,0.,0.);

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
	
	  sum_pt += pt;
	  particleFlux+=p.momentum();
	  count_chrg++;
	  float pi = 3.14159;
	  // toward region
	  if(dphi < 1./3.*pi || dphi>5./3.*pi) 
	    {
	      count_chrg_toward++;
	      sum_pt_toward += pt;
	      particleFlux_toward+=p.momentum();
	    }
	  // away region
	  if(dphi > 2./3.*pi && dphi<4./3.*pi) 
	    {
	      count_chrg_away++;
	      sum_pt_away += pt;
	      particleFlux_away+=p.momentum();	     
	    }
	  // transverse region
	  if((dphi > 1./3.*pi && dphi<2./3.*pi) || (dphi > 4./3.*pi && dphi<5./3.*pi)) 
	    {
	      count_chrg_transverse++;
	      sum_pt_transverse += pt;
	      particleFlux_transverse+=p.momentum();
	    }
	  _h_deltaPhi->fill(dphi/3.14159*180., weight);
      } // loop over particles

      
      _h_chmult->fill(count_chrg,weight);
      _h_chmult_toward->fill(count_chrg_toward,weight);
      _h_chmult_transverse->fill(count_chrg_transverse,weight);
      _h_chmult_away->fill(count_chrg_away,weight);
      
      _h_sumPt->fill(sum_pt, weight);
      _h_sumPt_toward->fill(sum_pt_toward, weight);
      _h_sumPt_transverse->fill(sum_pt_transverse, weight);
      _h_sumPt_away->fill(sum_pt_away, weight);
      
      _h_vecSumPt->fill(particleFlux.pT(), weight);
      _h_vecSumPt_toward->fill(particleFlux_toward.pT(), weight);
      _h_vecSumPt_transverse->fill(particleFlux_transverse.pT(), weight);
      _h_vecSumPt_away->fill(particleFlux_away.pT(), weight);
      _h_vecDeltaPhi->fill(deltaPhi(particleFlux,ttbar)/3.14159*180., weight);

      // fill the profiles
      // _p_chmult_ttpt->fill(ttbar.pT(), count_chrg, weight);
      // _p_chmult_ttpt_toward->fill(ttbar.pT(), count_chrg_toward, weight);
      // _p_chmult_ttpt_transverse->fill(ttbar.pT(), count_chrg_transverse, weight);
      // _p_chmult_ttpt_away->fill(ttbar.pT(), count_chrg_away, weight);
      
      // _p_vecSumPt_ttpt->fill(ttbar.pT(), particleFlux.pT(), weight);
      // _p_vecSumPt_ttpt_toward->fill(ttbar.pT(), particleFlux_toward.pT(), weight);
      // _p_vecSumPt_ttpt_transverse->fill(ttbar.pT(), particleFlux_transverse.pT(), weight);
      // _p_vecSumPt_ttpt_away->fill(ttbar.pT(), particleFlux_away.pT(), weight);
      // if(count_chrg>0)            {_p_avgPt_ttpt           ->fill(ttbar.pT(), particleFlux.pT()/count_chrg, weight);}
      // if(count_chrg_toward>0)     {_p_avgPt_ttpt_toward    ->fill(ttbar.pT(), particleFlux_toward.pT()/count_chrg_toward, weight);}
      // if(count_chrg_transverse>0) {_p_avgPt_ttpt_transverse->fill(ttbar.pT(), particleFlux_transverse.pT()/count_chrg_transverse, weight);}
      // if(count_chrg_away>0)       {_p_avgPt_ttpt_away      ->fill(ttbar.pT(), particleFlux_away.pT()/count_chrg_away, weight);}
    }
    
    
    /// Normalise histograms etc., after the run
    void finalize() 
    {
      
      /// @todo Normalise, scale and otherwise manipulate histograms here

      // scale(_h_YYYY, crossSection()/sumOfWeights()); # norm to cross section
      //normalize(_h_YYYY); # normalize to unity
      normalize(_h_chmult);
      normalize(_h_deltaPhi);
      normalize(_h_chmult);
      normalize(_h_chmult_toward);
      normalize(_h_chmult_transverse);
      normalize(_h_chmult_away);
      normalize(_h_sumPt);
      normalize(_h_sumPt_toward);
      normalize(_h_sumPt_transverse);
      normalize(_h_sumPt_away);
      normalize(_h_vecSumPt);
      normalize(_h_vecSumPt_toward);
      normalize(_h_vecSumPt_transverse);
      normalize(_h_vecSumPt_away);
      normalize(_h_vecDeltaPhi);
      normalize(_h_pt_tt);
      normalize(_h_pt_mu);
      normalize(_h_pt_ele);
      normalize(_h_eta_tt);
      normalize(_h_eta_mu);
      normalize(_h_eta_ele);
    }

    //@}




  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_TOP_13_007);

}
