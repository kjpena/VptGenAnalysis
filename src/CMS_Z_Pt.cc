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
    std::map<string,double> _sumw;
    bool _usePosChLeptons;
    
  public:
    
    /// CTOR
    CMS_Z_Pt()  : 
      Analysis("CMS_Z_Pt")
    { 
      //setBeams(PROTON, PROTON);
      _sumw["bare"]=0;
      _sumw["dressed"]=0;
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
	  histos["phistar_"+pf]       = bookHisto1D("phistar_"+pf, 200,0,10);
	  histos["zm_"+pf]       = bookHisto1D("zm_"+pf, 100,66,116);
	  histos["zpt_"+pf]      = bookHisto1D("zpt_"+pf, 100,0,100);
	  histos["zy_"+pf]       = bookHisto1D("zy_"+pf, 100,-3,3);
	  histos["lpt_"+pf]      = bookHisto1D("lpt_"+pf, 100,0,100);	  
	  histos["tkmet_"+pf]    = bookHisto1D("tkmet_"+pf, 100,0,250);
	  histos["mt_tkmet_"+pf] = bookHisto1D("mt_tkmet_"+pf, 100,0,250);
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
      FourMomentum p_trks(0,0,0,0);
      foreach( const Particle & p, trks ) p_trks += p.momentum(); 


      analyzeZ(applyProjection<ZFinder>(event, "ZFinder_dressed_mu"),
	       p_trks,
	       weight,
	       "dressed");
      
      analyzeZ(applyProjection<ZFinder>(event, "ZFinder_bare_mu"),
	       p_trks,
	       weight,
	       "bare");
    }

    //
    void analyzeZ(const ZFinder& zfinder, FourMomentum p_trks, double weight, std::string tag)
    {
      if (zfinder.bosons().size() != 1 ) return;

      //Z kinematics
      const Particle& Zboson = zfinder.boson();
      const double zpt   = Zboson.pT();
      const double zrap  = Zboson.absrap();
      const double zmass = Zboson.mass(); 
      if (zrap > 2.4) return;
      
      //leptons
      const ParticleVector& leptons = zfinder.constituents();
      if (leptons.size() != 2 || 
	  leptons[0].threeCharge() * leptons[1].threeCharge() > 0) return;

      _sumw[tag] += weight;
            
      // Compute phi*
      const Particle& lminus = leptons[0].charge() < 0 ? leptons[0] : leptons[1];
      const Particle& lplus  = leptons[0].charge() < 0 ? leptons[1] : leptons[0];
      const double phi_acop = M_PI - deltaPhi(lminus, lplus);
      const double costhetastar = tanh( 0.5 * (lminus.eta() - lplus.eta()) );
      const double sin2thetastar = (costhetastar > 1) ? 0.0 : (1.0 - sqr(costhetastar));
      const double phistar = tan(0.5 * phi_acop) * sqrt(sin2thetastar);
      histos["phistar_"+tag]->fill(phistar, weight);      


      //compute Zpt
      const FourMomentum pZ = zfinder.bosons()[0].momentum();
      histos["zm_"+tag]->fill(zmass/GeV, weight);
      histos["zpt_"+tag]->fill(zpt/GeV, weight);
      histos["zy_"+tag]->fill(zrap, weight);
      
      Particle visLepton( _usePosChLeptons ? lplus : lminus );
      histos["lpt_"+tag]->fill(visLepton.pT()/GeV,weight);
      
      FourMomentum p_trks_final( p_trks - (_usePosChLeptons ? lminus : lplus ));
      FourMomentum chmet(-p_trks_final.px(),-p_trks_final.py(),0,p_trks_final.pT());
      histos["tkmet_"+tag]->fill(chmet.pT()/GeV,weight);
      
      double mt=sqrt(2*visLepton.pT()*chmet.pT()*(1-cos(deltaPhi(chmet,visLepton))));
      histos["mt_tkmet_"+tag]->fill(mt/GeV);      
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
	  if(it->first.find("bare")!=std::string::npos)    finalNorm=norm/_sumw["bare"];
	  if(it->first.find("dressed")!=std::string::npos) finalNorm=norm/_sumw["dressed"];
	  scale(it->second,finalNorm);
	}
    }
  };
  
  
  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_Z_Pt);  
}
