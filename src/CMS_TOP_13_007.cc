// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/RivetAIDA.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Projections/FinalState.hh"
/// @todo Include more projections as required, e.g. ChargedFinalState, FastJets, ZFinder...
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/ChargedFinalState.hh"
#include "Rivet/Tools/ParticleIdUtils.hh"
#include "Rivet/Projections/ChargedLeptons.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/MissingMomentum.hh"
#include "Rivet/Tools/ParticleIdUtils.hh"
#include "Rivet/Projections/VetoedFinalState.hh"


// plots to implement
// charge multiplicities
// charge multiplicities as function of pt(ttbar)
// average pt per charge
// sum pt = flux



namespace Rivet {


  class CMS_TOP_13_007 : public Analysis {
    
  private:
    // Histograms

    // basic distributions
    AIDA::IHistogram1D* _h_deltaPhi;

    // charge multiplicities
    AIDA::IHistogram1D* _h_chmult;
    AIDA::IHistogram1D* _h_chmult_toward;
    AIDA::IHistogram1D* _h_chmult_transverse;
    AIDA::IHistogram1D* _h_chmult_away;

    // charged particle fluxes
    AIDA::IHistogram1D* _h_sumPt;
    AIDA::IHistogram1D* _h_sumPt_toward;
    AIDA::IHistogram1D* _h_sumPt_transverse;
    AIDA::IHistogram1D* _h_sumPt_away;
    AIDA::IHistogram1D* _h_vecSumPt;
    AIDA::IHistogram1D* _h_vecSumPt_toward;
    AIDA::IHistogram1D* _h_vecSumPt_transverse;
    AIDA::IHistogram1D* _h_vecSumPt_away;
    AIDA::IHistogram1D* _h_vecDeltaPhi;
    AIDA::IHistogram1D* _h_theta1;
    AIDA::IHistogram1D* _h_theta2;

    // profiles
    AIDA::IProfile1D* _p_chmult_ttpt;
    AIDA::IProfile1D* _p_chmult_ttpt_toward;
    AIDA::IProfile1D* _p_chmult_ttpt_transverse;
    AIDA::IProfile1D* _p_chmult_ttpt_away;
    AIDA::IProfile1D* _p_vecSumPt_ttpt;
    AIDA::IProfile1D* _p_vecSumPt_ttpt_toward;
    AIDA::IProfile1D* _p_vecSumPt_ttpt_transverse;
    AIDA::IProfile1D* _p_vecSumPt_ttpt_away;
    AIDA::IProfile1D* _p_avgPt_ttpt;
    AIDA::IProfile1D* _p_avgPt_ttpt_toward;
    AIDA::IProfile1D* _p_avgPt_ttpt_transverse;
    AIDA::IProfile1D* _p_avgPt_ttpt_away;

    
    
    // various contol distributions
    AIDA::IHistogram1D* _h_pt_tt;
    AIDA::IHistogram1D* _h_pt_mu;
    AIDA::IHistogram1D* _h_pt_ele;
    AIDA::IHistogram1D* _h_eta_tt;
    AIDA::IHistogram1D* _h_eta_mu;
    AIDA::IHistogram1D* _h_eta_ele;
    AIDA::IHistogram1D* _h_pt_jets;
    AIDA::IHistogram1D* _h_eta_jets;
    AIDA::IHistogram1D* _h_jetmult;

  public:


    CMS_TOP_13_007() 
      : Analysis("CMS_TOP_13_007")
    { 
      setBeams(PROTON, PROTON);
    }


    /// Book histograms and initialise projections before the run
    void init() {

      // define the acceptance of the final state
      const FinalState fs(-2.5,2.5,0.0*GeV);
      addProjection(fs, "FS");

      // muons
      IdentifiedFinalState mufs(-2.5,2.5,20*GeV);
      mufs.acceptIdPair(MUON);
      addProjection(mufs, "Muons");

      // electrons
      IdentifiedFinalState elfs(-2.5,2.5,20*GeV);
      elfs.acceptIdPair(ELECTRON);
      addProjection(elfs, "Electrons");

      // muon neutrinos
      IdentifiedFinalState munufs(-5.0,5.0,0*GeV);
      munufs.acceptIdPair(NU_MU);
      addProjection(munufs, "MuonNus");

      // electron neutrinos
      IdentifiedFinalState elnufs(-5.0,5.0,0*GeV);
      elnufs.acceptIdPair(NU_E);
      addProjection(elnufs, "ElectronNus");


      // remove neutrinos, electrons and muons and get the jets
      VetoedFinalState vfs(fs);
      vfs.vetoNeutrinos();
      vfs.addVetoPairDetail(MUON, 20.0*GeV, MAXDOUBLE);
      vfs.addVetoPairDetail(ELECTRON, 20.0*GeV, MAXDOUBLE);
      addProjection(vfs, "VFS");
      addProjection(FastJets(vfs, FastJets::ANTIKT, 0.5), "Jets");
      addProjection(MissingMomentum(vfs), "MissingET");

      addProjection(ChargedLeptons(FinalState(-2.5, 2.5, 20*GeV)), "LFS");

      // get the charged particles
      ChargedFinalState chfs(fs);             
      addProjection(chfs, "chfs"); 



      // histogram booking goes here
      // for these histograms one needs a corresponding data reference *.aida file from which the binning is taken...
      _h_deltaPhi	     = bookHistogram1D("d01-x01-y01");
      _h_chmult		     = bookHistogram1D("d01-x01-y02");
      _h_chmult_toward	     = bookHistogram1D("d01-x01-y03");
      _h_chmult_transverse   = bookHistogram1D("d01-x01-y04");
      _h_chmult_away	     = bookHistogram1D("d01-x01-y05");
      _h_sumPt		     = bookHistogram1D("d01-x01-y06");
      _h_sumPt_toward	     = bookHistogram1D("d01-x01-y07");
      _h_sumPt_transverse    = bookHistogram1D("d01-x01-y08");
      _h_sumPt_away	     = bookHistogram1D("d01-x01-y09");
      _h_vecSumPt	     = bookHistogram1D("d01-x02-y01");
      _h_vecSumPt_toward     = bookHistogram1D("d01-x02-y02");
      _h_vecSumPt_transverse = bookHistogram1D("d01-x02-y03");
      _h_vecSumPt_away       = bookHistogram1D("d01-x02-y04");
      _h_vecDeltaPhi         = bookHistogram1D("d01-x02-y05");


      // control plots (binning hardcoded here)
      _h_pt_tt	     = bookHistogram1D("d00-x00-y00", 15, 0., 300.);
      _h_pt_mu	     = bookHistogram1D("d00-x00-y01", 15, 0., 300.);
      _h_pt_ele	     = bookHistogram1D("d00-x00-y02", 15, 0., 300.);
      // _h_pt_jets  = bookHistogram1D("d00-x00-y03", 15, 0., 300.);
      _h_eta_tt	     = bookHistogram1D("d00-x00-y04", 25, -5.0, 5.0);
      _h_eta_mu	     = bookHistogram1D("d00-x00-y05", 15, -3.0, 3.0);
      _h_eta_ele     = bookHistogram1D("d00-x00-y06", 15, -3.0, 3.0);
      // _h_eta_jets = bookHistogram1D("d00-x00-y07", 15, -3.0, 3.0);
      _h_jetmult     = bookHistogram1D("d00-x00-y08",  9, -0.5,	8.5);
      _h_theta1      = bookHistogram1D("d01-x03-y00",10,0.,3.14159);
      _h_theta2      = bookHistogram1D("d01-x03-y01",10,0.,3.14159);


      _p_chmult_ttpt		  = bookProfile1D("d02-x01-y01", 15, 0., 300.);
      _p_chmult_ttpt_toward	  = bookProfile1D("d02-x01-y02", 15, 0., 300.);
      _p_chmult_ttpt_transverse   = bookProfile1D("d02-x01-y03", 15, 0., 300.);
      _p_chmult_ttpt_away         = bookProfile1D("d02-x01-y04", 15, 0., 300.);
      _p_vecSumPt_ttpt            = bookProfile1D("d02-x01-y05", 15, 0., 300.);
      _p_vecSumPt_ttpt_toward     = bookProfile1D("d02-x01-y06", 15, 0., 300.);
      _p_vecSumPt_ttpt_transverse = bookProfile1D("d02-x01-y07", 15, 0., 300.);
      _p_vecSumPt_ttpt_away	  = bookProfile1D("d02-x01-y08", 15, 0., 300.);
      _p_avgPt_ttpt		  = bookProfile1D("d02-x01-y09", 15, 0., 300.);
      _p_avgPt_ttpt_toward	  = bookProfile1D("d02-x01-y10", 15, 0., 300.);
      _p_avgPt_ttpt_transverse	  = bookProfile1D("d02-x01-y11", 15, 0., 300.);
      _p_avgPt_ttpt_away	  = bookProfile1D("d02-x01-y12", 15, 0., 300.);
    }


    ///  the per-event analysis goes here
    void analyze(const Event& event) {

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

      const ParticleVector sortedMuons = applyProjection<IdentifiedFinalState>(event, "Muons").particlesByPt();
      const ParticleVector sortedElectrons = applyProjection<IdentifiedFinalState>(event, "Electrons").particlesByPt();
      const ParticleVector sortedMuonNus = applyProjection<IdentifiedFinalState>(event, "MuonNus").particlesByPt();
      const ParticleVector sortedElectronNus = applyProjection<IdentifiedFinalState>(event, "ElectronNus").particlesByPt();

      // loop over the muons
      MSG_DEBUG("Muon multiplicity = " << sortedMuons.size());
      foreach (Particle mu, sortedMuons ) {
	MSG_DEBUG("Muon pT = " << mu.momentum().pT());
      }
      if ( sortedMuons.size() < 1) {
	MSG_DEBUG("Event failed muon multiplicity cut");
        vetoEvent;
      }

      // loop over the electrons
      MSG_DEBUG("Electron multiplicity = " << sortedElectrons.size());
      foreach (Particle ele, sortedElectrons ) {
	MSG_DEBUG("Electron pT = " << ele.momentum().pT());
      }
      if ( sortedElectrons.size() < 1) {
	MSG_DEBUG("Event failed electron multiplicity cut");
        vetoEvent;
      }

      // check opposite sign lepton charges
      int chlep1 = PID::charge(sortedMuons[0]) ;
      int chlep2 = PID::charge(sortedElectrons[0]) ;
      int chlep = chlep1 * chlep2;

      // std::cout << chlep1 << " " << chlep2 << std::endl;

      if(chlep>=0) {
	MSG_DEBUG("Event failed opposite sign cut");
	vetoEvent;
      }

      // MET cut
      const MissingMomentum& met = applyProjection<MissingMomentum>(event, "MissingET");
      MSG_DEBUG("Vector ET  =" << met.visibleMomentum().perp() << " GeV");
      if ( met.visibleMomentum().perp() < 30*GeV) {
        MSG_DEBUG("Event failed missing ET cut");
        vetoEvent;
      }

      FourMomentum missvec(0.0,0.0,0.0,0.0);
      missvec = -met.visibleMomentum(); 
      missvec.setPz(0.0); // no information about Pz
      missvec.setE(missvec.perp()); // assume neutrinos are massless

      

      // jet multiplicity requirement
      const FastJets& jetpro = applyProjection<FastJets>(event, "Jets");
      const Jets alljets = jetpro.jetsByPt();
      if (alljets.size() < 2) {
        MSG_DEBUG("Event failed jet multiplicity cut");
        vetoEvent;
      }

      // jets minimum jet pt requirement
      const Jets jets = jetpro.jetsByPt(30*GeV);
      foreach (const Jet& jet, jets) {
        MSG_DEBUG("Jet pT = " << jet.momentum().pT()/GeV << " GeV");
      }
      if (jets.size() < 2) {
        MSG_DEBUG("Event failed jet pT cut");
        vetoEvent;
      }

      // b-tagging
      Jets bjets;
      foreach (const Jet& jet, jets) {
        if (jet.containsBottom()) {
          bjets.push_back(jet);
        } 
      }

      MSG_DEBUG("Number of b-jets = " << bjets.size());
      if (bjets.size() != 2) {
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

      // std::cout << "***************" << std::endl;
      // std::cout << "ttbar four vector: " << ttbar << std::endl;
      
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
      foreach (const Particle& p, chfs.particles()) {
      	const int pid = p.pdgId();
	// remove the selected leptons
	float dR = min(deltaR(sortedMuons[0],p.momentum()),deltaR(sortedElectrons[0],p.momentum()));
	if(dR==0.) {
	  continue;
	}
	// remove particles matching the b-jets
	if(bjets[0].containsParticle(p) || bjets[1].containsParticle(p)) {
	  continue;
	}
	// pt and/or eta cut on particels
	if(p.momentum().pT() < 0.5 || abs(p.momentum().eta()) > 2.1) {
	  continue;
	}

	float dphi = deltaPhi(ttbar,p.momentum());
	float pt = p.momentum().pT();
	
	sum_pt += pt;
	particleFlux+=p.momentum();

	count_chrg++;
	float pi = 3.14159;
	// toward region
	if(dphi < 1./3.*pi || dphi>5./3.*pi) {
	  count_chrg_toward++;
	  sum_pt_toward += pt;
	  particleFlux_toward+=p.momentum();
	}
	// away region
	if(dphi > 2./3.*pi && dphi<4./3.*pi) {
	  count_chrg_away++;
	  sum_pt_away += pt;
	  particleFlux_away+=p.momentum();	     
	}
	// transverse region
	if((dphi > 1./3.*pi && dphi<2./3.*pi) || (dphi > 4./3.*pi && dphi<5./3.*pi)) {
	  count_chrg_transverse++;
	  sum_pt_transverse += pt;
	  particleFlux_transverse+=p.momentum();
	}
	
	_h_deltaPhi->fill(dphi/3.14159*180., weight);
	
	  
      } // loop over particles


      // calculate jet pulls
      // loop over the b-jets
      // std::vector<FourMomentum> ts;
      std::vector<Vector<2> > ts;
      std::vector<FourMomentum> Js_all;


      for(int i=0; i<2;i++) {
	Vector<2> t;
	t.set(0,0.);
	t.set(1,0.);
	// std::cout <<  "t: " << t << std::endl;

	// calculate jet thrust
	FourMomentum J_all(0.,0.,0.,0.);
	FourMomentum J_ch(0.,0.,0.,0.);
	FourMomentum J_neut(0.,0.,0.,0.);
	foreach (const Particle& p, bjets[i].particles()) {
	  J_all+=p.momentum();
	  if(PID::charge(p)!=0) {J_ch+=p.momentum();}
	  if(PID::charge(p)==0) {J_neut+=p.momentum();}
	}
	std::cout << "jet thrust (all)  : " << J_all << std::endl;
	std::cout << "jet thrust (ch)   : " << J_ch << std::endl;
	std::cout << "jet thrust (neut) : " << J_neut << std::endl;
	std::cout << "jet momentum      : " << bjets[i].momentum() << std::endl;

	Js_all.push_back(J_all);

	// loop over the constituents of the jets
	foreach (const Particle& p, bjets[i].particles()) {
	  // const FinalState& vfs = applyProjection<FinalState>(event, "VFS");
	  // foreach (const Particle& p, vfs.particles()) {
	  Vector<2> r;
	  r.set(0, p.momentum().eta()-J_all.eta());
	  r.set(1, deltaPhi(p.momentum(),J_all));

	  std::cout << r << std::endl;
	  std::cout << "mod() " << r.mod() << std::endl;

	  float c = (p.momentum().pT()/J_all.pT()) * r.mod();
	  t.set(0,t[0]+(c * r[0]));
	  t.set(1,t[1]+(c * r[1]));
	}
	// ts.push_back(FourMomentum(t.mod(),t[0],t[1],0.));
	ts.push_back(t);
      }
      // calculate delta theta
      float dyjj = Js_all[1].rapidity() - Js_all[0].rapidity();
      float dphijj = deltaPhi(Js_all[0],Js_all[1]);
      // FourMomentum t1_all(ts[0].mod(), ts[0][0], ts[0][1], 0.);
      // FourMomentum t2_all(ts[1].mod(), ts[1][0], ts[1][1], 0.);

      float deltaTheta1 = deltaPhi(ts[0][1],3.14159+atan2(dyjj,dphijj));
      float deltaTheta2 = deltaPhi(ts[1][1],3.14159+atan2(dyjj,dphijj));
      std::vector<float> theta;
      theta.push_back(deltaTheta1);
      theta.push_back(deltaTheta2);
      std::cout << theta << std::endl;
      _h_theta1->fill(theta[0], weight);
      _h_theta2->fill(theta[1], weight);




      
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
      _p_chmult_ttpt->fill(ttbar.pT(), count_chrg, weight);
      _p_chmult_ttpt_toward->fill(ttbar.pT(), count_chrg_toward, weight);
      _p_chmult_ttpt_transverse->fill(ttbar.pT(), count_chrg_transverse, weight);
      _p_chmult_ttpt_away->fill(ttbar.pT(), count_chrg_away, weight);

      _p_vecSumPt_ttpt->fill(ttbar.pT(), particleFlux.pT(), weight);
      _p_vecSumPt_ttpt_toward->fill(ttbar.pT(), particleFlux_toward.pT(), weight);
      _p_vecSumPt_ttpt_transverse->fill(ttbar.pT(), particleFlux_transverse.pT(), weight);
      _p_vecSumPt_ttpt_away->fill(ttbar.pT(), particleFlux_away.pT(), weight);

      if(count_chrg>0)            {_p_avgPt_ttpt           ->fill(ttbar.pT(), particleFlux.pT()/count_chrg, weight);}
      if(count_chrg_toward>0)     {_p_avgPt_ttpt_toward    ->fill(ttbar.pT(), particleFlux_toward.pT()/count_chrg_toward, weight);}
      if(count_chrg_transverse>0) {_p_avgPt_ttpt_transverse->fill(ttbar.pT(), particleFlux_transverse.pT()/count_chrg_transverse, weight);}
      if(count_chrg_away>0)       {_p_avgPt_ttpt_away      ->fill(ttbar.pT(), particleFlux_away.pT()/count_chrg_away, weight);}








    }


    /// Normalise histograms etc., after the run
    void finalize() {

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
      // normalize(_h_pt_jets);
      // normalize(_h_eta_jets);
      normalize(_h_jetmult);
      normalize(_h_theta1);
      normalize(_h_theta2); 
      
      




    }

    //@}




  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_TOP_13_007);

}
