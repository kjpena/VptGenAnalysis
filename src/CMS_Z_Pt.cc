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
    
  public:
    
    /// CTOR
    CMS_Z_Pt()  : 
      Analysis("CMS_Z_Pt")
    { 
      //setBeams(PROTON, PROTON);
    }
    
    
    /// Book histograms and initialise projections before the run
    void init() 
    {      
      // Set up projections
      FinalState fs;
      Cut fidcut = Cuts::abseta < 2.4 && Cuts::pT > 20*GeV;

      ZFinder zfinder(fs, fidcut, PID::MUON, 66.0*GeV, 116.0*GeV, 0.1, ZFinder::CLUSTERNODECAY);
      addProjection(zfinder, "ZFinder");
      WFinder wfinder(fs, fidcut, PID::MUON, 0*GeV, 1000*GeV, 25*GeV, 0.1, WFinder::CLUSTERNODECAY);
      addProjection(wfinder, "WFinder");
      
      addProjection(ChargedFinalState(Cuts::abseta < 2.5 && Cuts::pT > 500*MeV), "Tracks");


      //these binnings are taken from ATLAS_2015_I1408516_MU
      std::vector<double> phistarBins={2.000000e-03,6.000000e-03,1.000000e-02,1.400000e-02,1.800000e-02,2.200000e-02,2.650000e-02,3.150000e-02,3.650000e-02,4.200000e-02,4.800000e-02,5.400000e-02,6.050000e-02,6.800000e-02,7.650000e-02,8.600000e-02,9.650000e-02,1.080000e-01,1.210000e-01,1.365000e-01,1.550000e-01,1.770000e-01,2.040000e-01,2.385000e-01,2.850000e-01,3.515000e-01,4.575000e-01,6.095000e-01,8.065000e-01,1.035500e+00,1.324500e+00,1.721500e+00,2.234500e+00,2.899500e+00,4.138500e+00,7.500000e+00};

      std::vector<double> ptllBins={1.000000e+00,3.000000e+00,5.000000e+00,7.000000e+00,9.000000e+00,1.100000e+01,1.300000e+01,1.500000e+01,1.700000e+01,1.900000e+01,2.125000e+01,2.375000e+01,2.625000e+01,2.875000e+01,3.150000e+01,3.450000e+01,3.750000e+01,4.050000e+01,4.350000e+01,4.650000e+01,4.950000e+01,5.250000e+01,5.550000e+01,5.900000e+01,6.300000e+01,6.750000e+01,7.250000e+01,7.750000e+01,8.250000e+01,9.000000e+01,1.000000e+02,1.150000e+02,1.375000e+02,1.625000e+02,1.875000e+02,2.250000e+02,2.750000e+02,3.250000e+02,3.750000e+02,4.350000e+02,5.100000e+02,6.000000e+02,7.750000e+02};

      // Book histograms
      std::string ini[]={"","gg_","qq_","qg_"};
      for(size_t i=0; i<sizeof(ini)/sizeof(std::string); i++)
      {
      	histos[ini[i]+"xsec"]    = bookHisto1D(ini[i]+"xsec",1,0,1);
	histos[ini[i]+"vm"]      = bookHisto1D(ini[i]+"vm", 100,0,200);
	histos[ini[i]+"vmt"]     = bookHisto1D(ini[i]+"vmt", 100,0,200);
	std::string name(ini[i]+"vpt");
	histos[ini[i]+"vpt"]     = bookHisto1D(name, ptllBins);
	histos[ini[i]+"vy"]      = bookHisto1D(ini[i]+"vy", 50,-3,3);
	name=ini[i]+"phistar";
        histos[name] = bookHisto1D(name, phistarBins);
	for(size_t j=0; j<2; j++)
	{
	  std::string ch(j==0 ? "pos" : "neg");
	  histos[ini[i]+ch+"lpt"]      = bookHisto1D(ini[i]+ch+"lpt", 50,20,145);
	  histos[ini[i]+ch+"ly"]       = bookHisto1D(ini[i]+ch+"ly", 50,-3,3);
	  histos[ini[i]+ch+"tkmet"]    = bookHisto1D(ini[i]+ch+"tkmet", 50,0,125);
	  histos[ini[i]+ch+"mt_tkmet"] = bookHisto1D(ini[i]+ch+"mt_tkmet", 50,0,125);
	}
      }
    }
    
    ///  the per-event analysis goes here
    void analyze(const Event& event) 
    {

      const double weight = event.weight();
	    
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
      double vpt(0),vrap(0),vm(0),vmt(0);
      std::map<std::string, const Particle *> chl;
      std::vector<std::string> ch;
    
      //if a Z is found use Z kinematics
      if (zfinder.bosons().size() == 1 ) 
      {
      	const Particle& Zboson = zfinder.boson();     
	vpt  = Zboson.pT();
	vrap = Zboson.rapidity();
        vm   = Zboson.mass(); 
        vmt  = sqrt(vpt*vpt+vm*vm);
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
	 vrap = Wboson.rapidity();
	 vm   = Wboson.mass();
	 vmt  = sqrt(vpt*vpt+vm*vm);
	 if( wfinder.constituentLeptons()[0].threeCharge()>0 ) 
	 {
		 chl["pos"] = &(wfinder.constituentLeptons()[0]);
		 chl["neg"] = &(wfinder.constituentNeutrinos()[0]);
		 ch.push_back("pos");
	 }
         else	                                               
	 {
		 chl["neg"] = &(wfinder.constituentLeptons()[0]);
		 chl["pos"] = &(wfinder.constituentNeutrinos()[0]);
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
	histos[ini[i]+"vy"]->fill(vrap,weight);
        histos[ini[i]+"phistar"]->fill(phistar,weight);
	for(size_t j=0; j<ch.size(); j++)
	{	  
	  histos[ini[i]+ch[j]+"lpt"]->fill(chl[ch[j]]->pT(),weight);
	  histos[ini[i]+ch[j]+"ly"]->fill(chl[ch[j]]->rapidity(),weight);
		
		
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
	  double finalNorm=norm/totalWgt;
	  scale(it->second,finalNorm);
	}
    }
  };
  
  
  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_Z_Pt);  
}
