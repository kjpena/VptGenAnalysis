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
#include "Rivet/Projections/ZFinder.hh"
#include "Rivet/Projections/WFinder.hh"
#include "Rivet/ProjectionApplier.hh"
#include "HepMC/PdfInfo.h"

namespace Rivet {
  
  
  class CMS_Z_Pt : public Analysis {
    
  private:
    
    std::map<string,Histo1DPtr> histos;
    double _sumw;
    
  public:
    
    /// CTOR
    CMS_Z_Pt()  : 
      Analysis("CMS_Z_Pt")
    { 
      //setBeams(PROTON, PROTON);
      _sumw=0;
    }
    
    
    /// Book histograms and initialise projections before the run
    void init() 
    {      
      // Set up projections
      FinalState fs;
      Cut fidcut = Cuts::abseta < 2.4 && Cuts::pT > 20*GeV;

      ZFinder zfinder(fs, fidcut, PID::MUON, 66.0*GeV, 116.0*GeV, 0.1, ZFinder::CLUSTERNODECAY);
      addProjection(zfinder, "ZFinder");
      WFinder wfinder(wminput,-2.5, 2.5, 15*GeV, PID::MUON, 0*GeV, 1000*GeV, 25*GeV, 0.1, WFinder::CLUSTERNODECAY);
      addProjection(wfinder_mu, "WFinde");
      
      addProjection(ChargedFinalState(Cuts::abseta < 2.5 && Cuts::pT > 500*MeV), "Tracks");

      // Book histograms
      std::string ini[]={"","gg_","qq_","qg_"};
      for(size_t i=0; i<sizeof(ini)/sizeof(std::string); i++)
      {
      	histos[ini[i]+"xsec"]    = bookHisto1D(ini[i]+"xsec",1,0,1);
	histos[ini[i]+"vm"]      = bookHisto1D(ini[i]+"vm", 100,0,200);
	histos[ini[i]+"vmt"]      = bookHisto1D(ini[i]+"vmt", 100,0,200);
	histos[ini[i]+"vpt"]     = bookHisto1D("vpt", 50,0,100);
	histos[ini[i]+"vy"]       = bookHisto1D("vy", 50,0,3);
        histos[ini[i]+"phistar"] = bookHisto1D(ini[i]+"phistar", 50,1e-3,10);  
	for(size_t j=0; j<2; j++)
	{
	  std::string ch(j==0 ? "pos" : "neg");
	  histos[ini[i]+ch+"lpt"]      = bookHisto1D(ini[i]+ch+"lpt", 50,0,100);
	  histos[ini[i]+ch+"ly"]       = bookHisto1D(ini[i]+ch+"ly", 50,0,3);
	  histos[ini[i]+ch+"tkmet"]    = bookHisto1D(ini[i]+ch+"tkmet", 50,0,250);
	  histos[ini[i]+ch+"mt_tkmet"] = bookHisto1D(ini[i]+ch+"mt_tkmet", 50,0,250);
	}
    }
    
    ///  the per-event analysis goes here
    void analyze(const Event& event) 
    {

      const double weight = event.weight();
      _sumw += weight;
	    
      //pdf and cross section info
      const HepMC::PdfInfo *pdf=event.genEvent()->pdf_info();
      std::string ini[]={"","qq_"};
      if( pdf->id1()==21 && pdf->id2()==21)     ini[1]="gg_";
      else if(pdf->id1()==21 || pdf->id2()==21) ini[1]="qg_";

      //charged particle balance    
      const Particles &trks = applyProjection<FinalState>(event, "Tracks").particles();
      FourMomentum p_trks(0,0,0,0);
      foreach( const Particle & p, trks ) p_trks += p.momentum(); 
	    
      //determine boson and lepton kinematics
      const ZFinder& zfinder=applyProjection<ZFinder>(event, "ZFinder");
      const WFinder& wfinder=applyProjection<WFinder>(event, "WFinder");
      double vpt(0),vrap(0),vm(0);
      std::map<std::string, const Particle *> chl;
      std::vector<std::string> ch;
	    
      //if a Z is found use Z kinematics
      if (zfinder.bosons().size() == 1 ) 
      {
      	const Particle& Zboson = zfinder.boson();     
	vpt  = Zboson.pT();
	vrap = Zboson.absrap();
        vm   = Zboson.mass(); 
        vmt  = Zboson.mT();
	const ParticleVector& leptons = zfinder.constituents();
	chl["pos"] = &(leptons[0].charge() < 0 ? leptons[0] : leptons[1]);
        chl["neg"] = &(leptons[0].charge() < 0 ? leptons[1] : leptons[0]);
	ch.push_back("pos"); ch.push_back("neg");
      } 
      //else use the W
      else if (wfinder.bosons().size()==1)
      {
         const Particle& Wboson = wfinder.boson();     
	 vpt  = Wboson.pT();
	 vrap= Wboson.absrap();
	 vm  = Wboson.mass();
	 vmT = Wboson.mT();
	 if( wfinder.constituentLeptons()[0].threeCharge()>0 ) 
	 {
		 chl["pos"] = &(wfinder.constituentLeptons()[0]);
		 chl["neg"] = wfinder.constituentNeutrinos()[0].momentum();
		 ch.push_back("pos");
	 }
         else	                                               
	 {
		 chl["neg"] = &(wfinder.constituentLeptons()[0]);
		 chl["pos"] = wfinder.constituentNeutrinos()[0].momentum();
		 ch.push_back("neg");
	 }
      }
      else vetoEvent;
	      
      // Compute phi* (for W use neutrino as if it was measured)            
      const double phi_acop = M_PI - deltaPhi(*chl["neg"], *chl["pos"]);
      const double costhetastar = tanh( 0.5 * (chl["neg"]->eta() - chl["pos"]->eta()) );
      const double sin2thetastar = (costhetastar > 1) ? 0.0 : (1.0 - sqr(costhetastar));
      const double phistar = tan(0.5 * phi_acop) * sqrt(sin2thetastar);

      //fill histos
      for(size_t i=0; i<2; i++)
      {	      
	histos[ini[i]+"xsec"]->fill(0,weight);
	histos[ini[i]+"vm"]->fill(vm,weight);
	histos[ini[i]+"vmt"]->fill(vmt,weight);
	histos[ini[i]+"vpt"]->fill(vpt,weight);
	histos[ini[i]+"vy"]->fill(vy,weight);
        histos[ini[i]+"phistar"]->fill(phistar,weight);
	for(size_t j=0; j<ch.size(); j++)
	{	  
	  histos[ini[i]+ch[j]+"lpt"]->fill(chl[ch[j]]->pT(),weight);
	  histos[ini[i]+ch[j]+"ly"]->fill(chl[ch[j]]->rap(),weight);
		
		
	  FourMomentum p_trks_final( p_trks - *chl[ch[j]]);
          FourMomentum chmet(-p_trks_final.px(),-p_trks_final.py(),0,p_trks_final.pT());
	  histos[ini[i]+ch[j]+"tkmet"]->fill(chmet.pT(),weight);
	  double mt=sqrt(2*chl[ch[j]]->pT()*chmet.pT()*(1-cos(deltaPhi(chmet,*chl[ch[j]]))));
	  histos[ini[i]+ch[j]+"mt_tkmet"]->fill(mt,weight);
	}
      }
    }

    
    /// Normalise histograms by the cross section
    void finalize() 
    {
      double norm = crossSection();
      double totalWgt=sumOfWeights();
      for(std::map<string,Histo1DPtr>::iterator it = histos.begin();
	  it != histos.end();
	  it++)
	{
	  double finalNorm=norm/_sumw;
	  scale(it->second,finalNorm);
	}
    }
  };
  
  
  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_Z_Pt);  
}
