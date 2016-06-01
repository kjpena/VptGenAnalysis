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
  
  
  class CMS_TOP_13_007 : public Analysis {
    
    enum JetBins   {JET_INC, JET_0,     JET_1,    JET_2  };
    enum UERegions {TT_INC,  TT_TOWARD, TT_TRANS, TT_AWAY};

  private:

    Histo1DPtr _h_ptttbar[4];
    Histo1DPtr   _h_chMult[4*4], _h_chSumPt[4*4], _h_chAvgPt[4*4];
    Profile1DPtr _p_ptttbar_chMult[4*4],    _p_ptttbar_chSumPt[4*4],    _p_ptttbar_chAvgPt[4*4];
    size_t _n_dphiProfileBins;
    Profile1DPtr _p_dphi2ttbar_chMult[4], _p_dphi2ttbar_chSumPt[4], _p_dphi2ttbar_chAvgPt[4], _p_chMult_chAvgPt[4];
    //Profile1DPtr                            _p_chMult_chSumPt[4*4],     _p_chMult_chAvgPt[4*4];

  public:

    /// CTOR
    CMS_TOP_13_007()  : 
      Analysis("CMS_TOP_13_007"),
      _n_dphiProfileBins(18)
    { 
     
      //setBeams(PROTON, PROTON);
    }


    /// Book histograms and initialise projections before the run
    void init() 
    {
      // define the acceptance of the final state
      Cut ptcut = Cuts::range(Cuts::pT, 0.0, MAXDOUBLE);
      Cut etacut = Cuts::range(Cuts::eta, -2.5, 2.5);
      Cut ptetacut = ptcut && etacut;
      const FinalState fs(ptetacut);
      addProjection(fs, "FS");

      //photons
      IdentifiedFinalState gammafs(fs);
      gammafs.acceptIdPair(PID::PHOTON);
      
      //charged leptons
      Cut lepptcut = Cuts::range(Cuts::pT, 20.0, MAXDOUBLE);
      Cut lepetacut = Cuts::range(Cuts::eta, -2.5, 2.5);
      Cut lepptetacut = lepptcut && lepetacut;
      IdentifiedFinalState lep_id(lepptetacut);
      lep_id.acceptIdPair(PID::MUON);
      lep_id.acceptIdPair(PID::ELECTRON);
      DressedLeptons ewdressedleptons(gammafs, lep_id, 0.1, lepptetacut, true, true);
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
      char buf[50];
      for(int ijet=-1; ijet<=2; ijet++)
	{
	  if(ijet>=0) sprintf(buf,"ptttbar-j%d",ijet);
	  else        sprintf(buf,"ptttbar");
	  _h_ptttbar[ijet+1]  = bookHisto1D(buf,10,0,250);

	  for(int ireg=0; ireg<4; ireg++)
	    {
	      int idx(ireg+(ijet+1)*4);
	      if(ijet>=0) sprintf(buf,"chMult-r%d-j%d",ireg,ijet);  
	      else        sprintf(buf,"chMult-r%d",ireg);  
	      _h_chMult[idx]  = bookHisto1D(buf,50,0,100);
	      std::string name("ptttbar_"); name += buf;
	      _p_ptttbar_chMult[idx] = bookProfile1D(name,10,0,250);
	      if(ireg==0){
		name="dphi2ttbar_chMult";
		if(ijet>=0) {
		  sprintf(buf,"-j%d",ijet);
		  name += buf;
		}
		_p_dphi2ttbar_chMult[ijet+1] = bookProfile1D(name,_n_dphiProfileBins,0,180);
	      }

	      if(ijet>=0) sprintf(buf,"chSumPt-r%d-j%d",ireg,ijet);
	      else        sprintf(buf,"chSumPt-r%d",ireg);
	      _h_chSumPt[idx] = bookHisto1D(buf,20,0,100);
	      name="ptttbar_"; name += buf;
	      _p_ptttbar_chSumPt[idx] = bookProfile1D(name,10,0,250);
	      if(ireg==0){
		name="dphi2ttbar_chSumPt"; 
		if(ijet>=0) {
		  sprintf(buf,"-j%d",ijet);
		  name += buf;
		}
		_p_dphi2ttbar_chSumPt[ijet+1] = bookProfile1D(name,_n_dphiProfileBins,0,180);
	      }

	      if(ijet>=0) sprintf(buf,"chAvgPt-r%d-j%d",ireg,ijet);
	      else        sprintf(buf,"chAvgPt-r%d",ireg);
	      _h_chAvgPt[idx] = bookHisto1D(buf,20,0,10);
	      name="ptttbar_"; name += buf;
	      _p_ptttbar_chAvgPt[idx] = bookProfile1D(name,10,0,250);
	      if(ireg==0){
		name="dphi2ttbar_chAvgPt"; 
		if(ijet>=0) {
		  sprintf(buf,"-j%d",ijet);
		  name += buf;
		}
		_p_dphi2ttbar_chAvgPt[ijet+1] = bookProfile1D(name,_n_dphiProfileBins,0,180);

		name="chMult_chAvgPt";
		if(ijet>=0) name+=buf;
		_p_chMult_chAvgPt[ijet+1] =  bookProfile1D(name,50,0,100);
	      }
	    }
	}
    }
    
    
    ///  the per-event analysis goes here
    void analyze(const Event& event) 
    {

      const double weight = event.weight();

      //require 2 leptons
      const std::vector<DressedLepton> &leptons     = applyProjection<DressedLeptons>(event, "Leptons").dressedLeptons();
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

      // MET estimate from the balance of all final state particles
      Particles vfs_particles =  applyProjection<VisibleFinalState>(event, "vfs").particles();
      FourMomentum missvec(0.0,0.0,0.0,0.0);
      foreach ( const Particle & p, vfs_particles ) missvec -= p.momentum();
      missvec.setPz(0.0);
      missvec.setE(missvec.perp()); 

      /*
      Particles neutrinos =  applyProjection<IdentifiedFinalState>(event, "neutrinos").particles();
      FourMomentum truemissvec(0.0,0.0,0.0,0.0);
      foreach ( const Particle & p, neutrinos) truemissvec += p.momentum();
      truemissvec.setPz(0.0);
      truemissvec.setE(truemissvec.perp());
      */

      // jet multiplicity 
      const FastJets& jetpro = applyProjection<FastJets>(event, "Jets");
      Cut ptcut = Cuts::range(Cuts::pT, 20, MAXDOUBLE);
      Cut etacut = Cuts::range(Cuts::eta, -2.5, 2.5);
      Cut ptetacut = ptcut && etacut;              
      const Jets alljets = jetpro.jetsByPt(ptetacut);
      if (alljets.size() < 2) vetoEvent;

      // jets minimum jet pt requirement
      Cut tightptcut = Cuts::range(Cuts::pT, 30, MAXDOUBLE); 
      Cut tightptetacut = tightptcut && etacut;  
      const Jets jets = jetpro.jetsByPt(tightptetacut);
      if (jets.size() < 2)  vetoEvent;

      // b-tagging: this should change to take into account b-tag and mistag rates
      Jets bjets, otherjets;
      foreach (const Jet& jet, alljets) 
	{
	  if (jet.containsBottom() && bjets.size()<2) bjets.push_back(jet); 
	  else                                        otherjets.push_back(jet);
	}
      if(bjets.size()<2) vetoEvent;

      //assign jet bin
      std::vector<int> jet_bin(2,JET_INC);
      if(otherjets.size()==0)      jet_bin[1]=JET_0;
      else if(otherjets.size()==1) jet_bin[1]=JET_1;
      else                         jet_bin[1]=JET_2;
      
      // compute the rec_ttbar four vector
      const FourMomentum rec_ttbar = bjets[0].momentum() + bjets[1].momentum() 
	+ leptons[leadLepIdx].momentum() + leptons[trailerLepIdx].momentum() 
	+ missvec ;
      
      // now loop over all charged particles and select the ones not associated to the hard event
      std::vector<int> chMult(4,0),    chMult_dphi(_n_dphiProfileBins,0);
      std::vector<float> chSumPt(4,0), chSumPt_dphi(_n_dphiProfileBins,0);
      const FinalState& chfs = applyProjection<FinalState>(event, "chfs");
      foreach (const Particle& p, chfs.particles()) 
	{	
	  // remove the selected leptons
	  float dR = min(deltaR(leptons[leadLepIdx],p.momentum()),deltaR(leptons[trailerLepIdx],p.momentum()));
	  if(dR<0.02) continue;

	  // remove particles matching the b-jets
	  if(bjets[0].containsParticle(p) || bjets[1].containsParticle(p)) continue;

	  // pt and/or eta cut on particles
	  if(p.momentum().pT() < 0.5 || abs(p.momentum().eta()) > 2.1) continue;

	  //analyse particle
	  //const int pid = p.pdgId();
	  float dphi( fabs(deltaPhi(rec_ttbar,p.momentum())*180./PI) );
	  float pt( p.momentum().pT() );
	  
	  int tt_reg(TT_TRANS);
	  if(dphi>120) tt_reg=TT_AWAY;
	  if(dphi<60)  tt_reg=TT_TOWARD;
	  int dphi_bin(floor(dphi*_n_dphiProfileBins/180.));
	  chMult[0]  ++;    chMult[tt_reg]  ++;    chMult_dphi[ dphi_bin ] ++;
	  chSumPt[0] += pt; chSumPt[tt_reg] += pt; chSumPt_dphi[ dphi_bin ] += pt;

	} // loop over particles

      //fill histogram
      for(size_t ijbin=0; ijbin<jet_bin.size(); ijbin++)
	{
	  _h_ptttbar[jet_bin[ijbin]]->fill(rec_ttbar.pT(),weight);

	  float totalSumPt(0),totalChMult(0);
	  for(size_t ireg=0; ireg<chMult.size(); ireg++)
	    {
	      int idx(ireg+jet_bin[ijbin]*4);
	      _h_chMult[idx]->fill(chMult[ireg],weight);
	      _p_ptttbar_chMult[idx]->fill(rec_ttbar.pT(),chMult[ireg],weight);

	      _h_chSumPt[idx]->fill(chSumPt[ireg], weight);
	      _p_ptttbar_chSumPt[idx]->fill(rec_ttbar.pT(),chSumPt[ireg],weight);

	      //dphi profiles
	      if(ireg==0){
		for(size_t idphibin=0; idphibin<_n_dphiProfileBins; idphibin++)
		  {
		    //bin centered
		    float dphi=(idphibin+0.5)*180./_n_dphiProfileBins;
		    _p_dphi2ttbar_chMult[jet_bin[ijbin]]->fill(dphi,chMult_dphi[idphibin],weight);
		    _p_dphi2ttbar_chSumPt[jet_bin[ijbin]]->fill(dphi,chSumPt_dphi[idphibin],weight);
		    if(chMult_dphi[idphibin]==0) continue;
		    _p_dphi2ttbar_chAvgPt[jet_bin[ijbin]]->fill(dphi,chSumPt_dphi[idphibin]/chMult_dphi[idphibin],weight);
		  }
	      }

	      if(chMult[ireg]==0) continue;
	      totalSumPt+=chSumPt[ireg];
	      totalChMult+=chMult[ireg];
	      float avgPtPerParticle(chSumPt[ireg]/chMult[ireg]);
	      _h_chAvgPt[idx]->fill(avgPtPerParticle,weight);
	      _p_ptttbar_chAvgPt[idx]->fill(rec_ttbar.pT(),avgPtPerParticle,weight);
	    }
	  if(totalChMult==0) continue;
	  _p_chMult_chAvgPt[jet_bin[ijbin]]->fill(totalChMult,totalSumPt/totalChMult);
	}
    }
        
    /// Normalise histograms etc., after the run
    void finalize() 
    {
      
      /// @todo Normalise, scale and otherwise manipulate histograms here

      // scale(_h_YYYY, crossSection()/sumOfWeights()); # norm to cross section
      //normalize(_h_YYYY); # normalize to unity
      for(size_t ijreg=0; ijreg<4; ijreg++)
	{
	  normalize(_h_ptttbar[ijreg]);
	  for(size_t ireg=0; ireg<4; ireg++)
	    {
	      int idx(ireg+ijreg*4);
	      normalize(_h_chMult[idx] );
	      normalize(_h_chSumPt[idx] );
	      normalize(_h_chAvgPt[idx] );
	    }
	}
    }
  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_TOP_13_007);

}
