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
    
  private:
    
    std::map<std::string,Histo1DPtr> histos;
    std::map<std::string,Profile1DPtr> profiles;

  public:
    
    /// CTOR
    CMS_TOP_13_007()  : 
      Analysis("CMS_TOP_13_007")    
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

      //book histograms
      for(int i=0; i<4; i++)
	{
	  std::string prodMode("inc");
	  if(i==1) prodMode="qg";
	  if(i==2) prodMode="qq";
	  if(i==3) prodMode="gg";
	  histos[prodMode+"_ptttbar"]    = bookHisto1D(prodMode+"_ptttbar",10,0,250);
	  histos[prodMode+"_dphill"]     = bookHisto1D(prodMode+"_dphill",10,0,180.);
	  histos[prodMode+"_mll"]        = bookHisto1D(prodMode+"_mll",10,0,300);
	}
      profiles["avgpt_mll"]        = bookProfile1D("avgpt_mll",10,0,300.);
      profiles["avgpt_dphill"]     = bookProfile1D("avgpt_dphill",10,0,180.);
      profiles["avgpt_ptttbar"] = bookProfile1D("avgpt_ptttbar",10,0,250.);
      profiles["avgpt_dphi2ttbar"] = bookProfile1D("avgpt_dphi2ttbar",10,0,180.);
      profiles["avgpt_dphi2ll"]    = bookProfile1D("avgpt_dphi2ll",   10,0,180.);
      profiles["avgpt_nch"]        = bookProfile1D("avgpt_nch",       10,0,200);
    }
    
    
    ///  the per-event analysis goes here
    void analyze(const Event& event) 
    {
      const double weight = event.weight();

      //pdf and cross section info                                   
      const HepMC::PdfInfo *pdf=event.genEvent()->pdf_info();
      std::string prodMode("qq");
      if( pdf->id1()==21 && pdf->id2()==21)     prodMode="gg"; 
      else if(pdf->id1()==21 || pdf->id2()==21) prodMode="qg"; 


      //require 2 leptons op. sign
      const std::vector<DressedLepton> &leptons     = applyProjection<DressedLeptons>(event, "Leptons").dressedLeptons();
      if ( leptons.size() < 2) vetoEvent; 
      if( leptons[0].charge() * leptons[1].charge() >=0 ) vetoEvent; 

      const FourMomentum rec_ll=leptons[0].momentum() + leptons[1].momentum();
      float dphill( fabs(deltaPhi(leptons[0].momentum(),leptons[1].momentum()))*180/PI);

      // MET estimate from the balance of all final state particles
      Particles vfs_particles =  applyProjection<VisibleFinalState>(event, "vfs").particles();
      FourMomentum missvec(0.0,0.0,0.0,0.0);
      foreach ( const Particle & p, vfs_particles ) missvec -= p.momentum();
      missvec.setPz(0.0);
      missvec.setE(missvec.perp()); 
      
      // jet multiplicity 
      const FastJets& jetpro = applyProjection<FastJets>(event, "Jets");
      Cut ptcut = Cuts::range(Cuts::pT, 30, MAXDOUBLE);
      Cut etacut = Cuts::range(Cuts::eta, -2.5, 2.5);
      Cut ptetacut = ptcut && etacut;              
      const Jets alljets = jetpro.jetsByPt(ptetacut);
      if (alljets.size() < 2) vetoEvent;

      // b-tagging: this should change to take into account b-tag and mistag rates
      Jets bjets, otherjets;
      foreach (const Jet& jet, alljets) 
	{
	  if (jet.containsBottom() && bjets.size()<2) bjets.push_back(jet); 
	  else                                        otherjets.push_back(jet);
	}
      if(bjets.size()<2) vetoEvent;
      
      // compute the rec_ttbar four vector
      const FourMomentum rec_ttbar = bjets[0].momentum() + bjets[1].momentum() + rec_ll + missvec;
      
      // now loop over all charged particles and select the ones not associated to the hard event
      const FinalState& chfs = applyProjection<FinalState>(event, "chfs");
      std::vector<float> chMult_dphi2ttbar(10,0),  chMult_dphi2ll(10,0);
      std::vector<float> chSumPt_dphi2ttbar(10,0), chSumPt_dphi2ll(10,0);
      foreach (const Particle& p, chfs.particles()) 
	{	
	  // remove the selected leptons
	  float dR = min(deltaR(leptons[0],p.momentum()),deltaR(leptons[1],p.momentum()));
	  if(dR<0.02) continue;

	  // remove particles matching the b-jets
	  if(bjets[0].containsParticle(p) || bjets[1].containsParticle(p)) continue;

	  // pt and/or eta cut on particles
	  if(p.momentum().pT() < 0.5 || abs(p.momentum().eta()) > 2.1) continue;

	  //analyse particle
	  //const int pid = p.pdgId();
	  float dphi2ttbar( fabs(deltaPhi(rec_ttbar,p.momentum())*180./PI) );
	  float dphi2ll( fabs(deltaPhi(rec_ll,p.momentum())*180./PI) );
	  float pt( p.momentum().pT() );
	  
	  int dphi_bin2ttbar(floor(dphi2ttbar/18.));
	  chMult_dphi2ttbar[ dphi_bin2ttbar ] ++;
	  chSumPt_dphi2ttbar[ dphi_bin2ttbar ] += pt;

	  int dphi_bin2ll(floor(dphi2ll/18.));
	  chMult_dphi2ll[ dphi_bin2ll ] ++;
	  chSumPt_dphi2ll[ dphi_bin2ll ] += pt;
	} // loop over particles


      for(size_t ireg=0; ireg<chMult_dphi2ll.size(); ireg++)
	{
	  if(chMult_dphi2ll[ireg]==0) continue;
	  float avgPt_dphi2ll=chSumPt_dphi2ll[ireg]/chMult_dphi2ll[ireg];
	  profiles["avgpt_dphi2ll"]->fill(ireg*18.,avgPt_dphi2ll,weight);
	}

      float nch(0),sumPt(0);
      for(size_t ireg=0; ireg<chMult_dphi2ttbar.size(); ireg++)
	{
	  nch+=chMult_dphi2ttbar[ireg];
	  sumPt+=chSumPt_dphi2ttbar[ireg];
	  if(chMult_dphi2ttbar[ireg]==0) continue;
	  float avgPt_dphi2ttbar=chSumPt_dphi2ttbar[ireg]/chMult_dphi2ttbar[ireg];
	  profiles["avgpt_dphi2ttbar"]->fill(ireg*18.,avgPt_dphi2ttbar,weight);
	}
      float avgPt=(nch>0? sumPt/nch : 0.);

      histos[prodMode+"_ptttbar"]->fill(rec_ttbar.pt(),weight);
      histos["inc_ptttbar"]->fill(rec_ttbar.pt(),weight);
      histos[prodMode+"_dphill"]->fill(dphill,weight);
      histos["inc_dphill"]->fill(dphill,weight);
      histos[prodMode+"_mll"]->fill(rec_ll.mass(),weight);      
      histos["inc_mll"]->fill(rec_ll.mass(),weight);      
      profiles["avgpt_nch"]->fill(nch,avgPt,weight);
      profiles["avgpt_mll"]->fill(rec_ll.mass(),avgPt,weight);
      profiles["avgpt_dphill"]->fill(dphill,avgPt,weight);
      profiles["avgpt_ptttbar"]->fill(rec_ttbar.pt(),avgPt,weight);

    }
        
    /// Normalise histograms etc., after the run
    void finalize() 
    {
      double norm=crossSection();
      double totalWgt=sumOfWeights();
      double finalNorm=norm/totalWgt;
      for(std::map<std::string,Histo1DPtr>::iterator it = histos.begin();
	  it!=histos.end();
	  it++)
	scale(it->second,finalNorm);
    }
  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_TOP_13_007);

}
