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
#include "Rivet/ProjectionApplier.hh"
#include "HepMC/PdfInfo.h"

namespace Rivet {
  
  
  class CMS_Z_Pt : public Analysis {
    
  private:
    
    std::map<string,Histo1DPtr> histos;
    double _sumw_mu_bare,_sumw_mu_dressed;
    bool _usePosChLeptons;
    
  public:
    
    /// CTOR
    CMS_Z_Pt()  : 
      Analysis("CMS_Z_Pt")
    { 
      //setBeams(PROTON, PROTON);
      _sumw_mu_bare = 0;
      _sumw_mu_dressed = 0;
      _usePosChLeptons=true;
    }
    
    
    /// Book histograms and initialise projections before the run
    void init() 
    {      
      // Set up projections
      FinalState fs;
      Cut fidcut = Cuts::abseta < 2.4 && Cuts::pT > 20*GeV;

      ZFinder zfinder_dressed_mu(fs, fidcut, PID::MUON, 66.0*GeV, 116.0*GeV, 0.1, ZFinder::CLUSTERNODECAY);
      addProjection(zfinder_dressed_mu, "ZFinder_dressed_mu");
      ZFinder zfinder_bare_mu(fs, fidcut, PID::MUON, 66.0*GeV, 116.0*GeV, 0.0, ZFinder::NOCLUSTER);
      addProjection(zfinder_bare_mu, "ZFinder_bare_mu");
      
      addProjection(ChargedFinalState(Cuts::abseta < 2.5 && Cuts::pT > 500*MeV), "Tracks");

      // Book histograms
      histos["inistate"]    = bookHisto1D("inistate",3,0,3);
      histos["xsec"]        = bookHisto1D("xsec",1,0,1);
      for(int i=0; i<2; i++)
	{
	  std::string pf(i==0 ? "bare": "dressed");
	  histos["zm_"+pf]       = bookHisto1D("zm_"+pf, 100,66,116);
	  histos["zpt_"+pf]      = bookHisto1D("zpt_"+pf, 100,0,100);
	  histos["zy_"+pf]       = bookHisto1D("zy_"+pf, 100,-6,6);	  
	  histos["z_tkmet_"+pf]  = bookHisto1D("z_tkmet_"+pf, 100,0,100);
	  histos["mt_tkmet_"+pf] = bookHisto1D("mt_tkmet_"+pf, 100,0,100);
	}
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
      
      
      const Particles &trks = applyProjection<FinalState>(event, "Tracks").particles();
      FourMomentum ptrks(0,0,0,0);
      foreach( const Particle & p, trks ) ptrks += p.momentum(); 


      const ZFinder& zfinder_dressed_mu = applyProjection<ZFinder>(event, "ZFinder_dressed_mu");
      if (!zfinder_dressed_mu.bosons().empty()) 
	{
	  _sumw_mu_dressed += weight;
	  const FourMomentum pZ = zfinder_dressed_mu.bosons()[0].momentum();
	  histos["zm_dressed"]->fill(pZ.mass()/GeV, weight);
	  histos["zpt_dressed"]->fill(pZ.pT()/GeV, weight);
	  histos["zy_dressed"]->fill(pZ.rapidity(), weight);
	  FourMomentum ptrks_dressed(ptrks-zfinder_dressed_mu.constituents()[_usePosChLeptons]);
	  FourMomentum chmet_dressed(-ptrks_dressed.px(),-ptrks_dressed.py(),0,ptrks_dressed.pT());
	  histos["z_tkmet_dressed"]->fill(chmet_dressed.pT()/GeV,weight);
	  FourMomentum Wlike(chmet_dressed+zfinder_dressed_mu.constituents()[!_usePosChLeptons]);
	  histos["mt_tkmet_dressed"]->fill(Wlike.mass()/GeV);      
	}
      
      const ZFinder& zfinder_bare_mu = applyProjection<ZFinder>(event, "ZFinder_bare_mu");
      if (!zfinder_bare_mu.bosons().empty()) 
	{
	  _sumw_mu_bare += weight;
	  const FourMomentum pZ = zfinder_bare_mu.bosons()[0].momentum();
	  histos["zm_bare"]->fill(pZ.mass()/GeV, weight);
	  histos["zpt_bare"]->fill(pZ.pT()/GeV, weight);
	  histos["zy_bare"]->fill(pZ.rapidity(), weight);
	  FourMomentum ptrks_bare(ptrks-zfinder_bare_mu.constituents()[_usePosChLeptons]);
	  FourMomentum chmet_bare(-ptrks_bare.px(),-ptrks_bare.py(),0,ptrks_bare.pT());
	  histos["z_tkmet_bare"]->fill(chmet_bare.pT()/GeV,weight);
	  FourMomentum Wlike(chmet_bare+zfinder_bare_mu.constituents()[!_usePosChLeptons]);
	  histos["mt_tkmet_bare"]->fill(Wlike.mass()/GeV);      
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
	  if(it->first.find("bare")!=std::string::npos)    finalNorm=norm/_sumw_mu_bare;
	  if(it->first.find("dressed")!=std::string::npos) finalNorm=norm/_sumw_mu_dressed;
	  scale(it->second,finalNorm);
	}
    }
  };
  
  
  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_Z_Pt);  
}
