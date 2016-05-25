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
#include "Rivet/Tools/RivetMT2.hh"
#include "HepMC/PdfInfo.h"

namespace Rivet {
  
  
  class CMS_TOP_Radius : public Analysis {
    
  private:

    std::map<string,Histo1DPtr> histos;
    

  public:

    /// CTOR
    CMS_TOP_Radius()  : 
      Analysis("CMS_TOP_Radius")
    { 
      //setBeams(PROTON, PROTON);
    }


    /// Book histograms and initialise projections before the run
    void init() 
    {
      // define the acceptance of the final state
      const FinalState fs(-5.0,5.0,0.0*GeV);
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

      // Add MissingMomentum proj to calc MET
      MissingMomentum vismom(fs);
      addProjection(vismom, "MissingET");

      histos["inistate"]  = bookHisto1D("inistate",3,0,3);
      histos["xsec"]      = bookHisto1D("xsec",1,0,1);
      
      //book histograms
      histos["l_ptlead"]  = bookHisto1D("l_ptlead", 20,0,400);
      histos["l_pttrail"] = bookHisto1D("l_pttrail",20,0,400);      
      histos["l_etacen"]  = bookHisto1D("l_etacen", 10,0,5.0);
      histos["l_etafwd"]  = bookHisto1D("l_etafwd", 10,0,5.0);      
      histos["mll"]       = bookHisto1D("mll", 25,0,1000);      
      histos["lptsum"]    = bookHisto1D("lptsum", 25,0,1000);      
      histos["dphill"]    = bookHisto1D("dphill", 10,0,3.16);      
      
      histos["met_pt"]   = bookHisto1D("met_pt", 20,0,400);
      histos["metll_pt"] = bookHisto1D("metll_pt", 20,0,400);
      histos["st"]       = bookHisto1D("st", 20,0,400);
      histos["m_T2"]     = bookHisto1D("m_T2", 20,0,200);

      histos["b_ptlead"]  = bookHisto1D("b_ptlead", 20,0,400);
      histos["b_pttrail"] = bookHisto1D("b_pttrail",20,0,400);      
      histos["b_etacen"]  = bookHisto1D("b_etacen", 10,0,5.0);
      histos["b_etafwd"]  = bookHisto1D("b_etafwd", 10,0,5.0);   
      histos["mbb"]       = bookHisto1D("mbb", 25,0,1000);      
      histos["bptsum"]    = bookHisto1D("bptsum", 25,0,1000);      
      histos["dphibb"]    = bookHisto1D("dphibb", 10,0,3.16);      
      histos["m_T2bb"]    = bookHisto1D("m_T2bb", 20,0,200);

      histos["nb"]        = bookHisto1D("nb", 4,2,6);        
      histos["nj"]         = bookHisto1D("nj", 5,0,5);        

      histos["vispt"]     = bookHisto1D("vispt", 10,0,1000);         
      histos["vismass"]   = bookHisto1D("vismass", 10,0,1000);         
      histos["visht"]     = bookHisto1D("visht", 10,0,1000);       
      histos["ht"]        = bookHisto1D("ht", 20,0,1000);

      histos["j_ptlead"]   = bookHisto1D("j_ptlead", 20,0,400);
      histos["j_etalead"]  = bookHisto1D("j_etalead", 10,0,5.0);
    }
    
    
    ///  the per-event analysis goes here
    void analyze(const Event& event) 
    {

      const double weight = event.weight();

      //pdf info
      const HepMC::PdfInfo *pdf=event.genEvent()->pdf_info();
      int iniStateBin(0);
      if( pdf->id1()==21 && pdf->id2()==21) iniStateBin=2;
      else if(pdf->id1()==21 || pdf->id2()==21) iniStateBin=1;
      histos["inistate"]->fill(iniStateBin,weight);
      histos["xsec"]->fill(0,weight);

      //require 2 leptons
      const std::vector<DressedLepton> &leptons     = applyProjection<DressedLeptons>(event, "Leptons").clusteredLeptons();
      if ( leptons.size() < 2) vetoEvent; 

      //require ee/mm/em
      int ch=leptons[0].abspid() * leptons[1].abspid();
      if( ch != PID::ELECTRON * PID::MUON && ch != PID::ELECTRON*PID::ELECTRON && ch!=PID::MUON*PID::MUON ) vetoEvent;

      //require opposite sign 
      if( leptons[0].charge() * leptons[1].charge() >=0 ) vetoEvent; 
      
      // jet multiplicity 
      const FastJets& jetpro = applyProjection<FastJets>(event, "Jets");
      const Jets alljets = jetpro.jetsByPt(30*GeV,MAXDOUBLE,-4.7,4.7);

      // b-tagging
      Jets bjets,otherjets;
      foreach (const Jet& jet, alljets) 
	{
	  if(fabs(jet.eta())<2.5 && jet.containsBottom()) bjets.push_back(jet);
	  else otherjets.push_back(jet);
	}
      if(bjets.size()<2) vetoEvent;

      //dilepton
      const FourMomentum llP4=leptons[0].momentum()+leptons[1].momentum();
      float lptsum=leptons[0].pt()+leptons[1].pt();

      //met
      const FourMomentum vismom = applyProjection<MissingMomentum>(event, "MissingET").visibleMomentum();
      const FourMomentum missmom(-vismom.px(),-vismom.py(),0,vismom.pt());

      // calculate mT2
      double m_T2 = mT2::mT2( leptons[0].momentum(),leptons[1].momentum(),missmom,0);

      //2b's
      const FourMomentum bbP4=bjets[0].momentum()+bjets[1].momentum();
      float bptsum=bjets[0].pt()+bjets[1].pt();

      //calculate mT2bb
      const FourMomentum metll = missmom-llP4;
      double m_T2bb = mT2::mT2( bjets[0].momentum(),bjets[1].momentum(),metll,0);

      //visible system
      const FourMomentum visP4=llP4+bbP4;
      float visHt=lptsum+bptsum;

      //fill histos
      histos["l_ptlead"]->fill( max(leptons[0].pt(),leptons[1].pt()), weight );
      histos["l_pttrail"]->fill( min(leptons[0].pt(),leptons[1].pt()), weight );
      histos["l_etacen"]->fill( min(fabs(leptons[0].eta()),fabs(leptons[1].eta())), weight );
      histos["l_etafwd"]->fill( max(fabs(leptons[0].eta()),fabs(leptons[1].eta())), weight );
      histos["mll"]->fill(llP4.mass(),weight);
      histos["dphill"]->fill( deltaPhi(leptons[0],leptons[1]),weight);
      histos["lptsum"]->fill(lptsum,weight);
      
      histos["met_pt"]->fill( missmom.pt(), weight );
      histos["metll_pt"]->fill( metll.pt(), weight );
      histos["st"]->fill( llP4.pt()+missmom.pt(), weight );
      histos["m_T2"]->fill( m_T2,weight );
      
      histos["b_ptlead"]->fill( max(bjets[0].pt(),bjets[1].pt()), weight );
      histos["b_pttrail"]->fill( min(bjets[0].pt(),bjets[1].pt()), weight );
      histos["b_etacen"]->fill( min(fabs(bjets[0].eta()),fabs(bjets[1].eta())), weight );
      histos["b_etafwd"]->fill( max(fabs(bjets[0].eta()),fabs(bjets[1].eta())), weight );
      histos["mbb"]->fill(bbP4.mass(),weight);
      histos["dphibb"]->fill( deltaPhi(bjets[0],bjets[1]),weight);
      histos["bptsum"]->fill(bptsum,weight);
      histos["m_T2bb"]->fill( m_T2bb,weight );
      
      histos["nb"]->fill(bjets.size(),weight);
      histos["nj"]->fill(otherjets.size(),weight);
     
      histos["vispt"]->fill(visP4.pt(),weight);
      histos["vismass"]->fill(visP4.mass(),weight);
      histos["visht"]->fill(visHt,weight);
      histos["ht"]->fill(missmom.pt()+visHt,weight);
      
      if(otherjets.size())
	{
	  histos["j_ptlead"]->fill(otherjets[0].pt(),weight);
	  histos["j_etalead"]->fill(fabs(otherjets[0].eta()),weight);
	}
    }
    
    /// Normalise histograms etc., after the run
    void finalize() 
    {
      double norm = crossSection()/sumOfWeights();
      for(std::map<string,Histo1DPtr>::iterator it = histos.begin();
	  it != histos.end();
	  it++)
	scale(it->second,norm);
    }
  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(CMS_TOP_Radius);

}
