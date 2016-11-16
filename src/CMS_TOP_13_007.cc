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

#include "TopMonteCarlo/RivetTop/interface/CMSGenParticle.hh"
#include "TopMonteCarlo/RivetTop/interface/PartonTop.hh"

namespace Rivet {
  
  
  class CMS_TOP_13_007 : public Analysis {
    
  private:
    
    std::map<std::string,Histo1DPtr> histos;
    std::map<std::string,Histo2DPtr> histos2D;
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

      // Parton level top quarks
      PartonTop ttbarState;
      addProjection(ttbarState, "ttbar");

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

      std::vector<double> phistarBins={2.000000e-03,6.000000e-03,1.000000e-02,1.400000e-02,1.800000e-02,2.200000e-02,2.650000e-02,3.150000e-02,3.650000e-02,4.200000e-02,4.800000e-02,5.400000e-02,6.050000e-02,6.800000e-02,7.650000e-02,8.600000e-02,9.650000e-02,1.080000e-01,1.210000e-01,1.365000e-01,1.550000e-01,1.770000e-01,2.040000e-01,2.385000e-01,2.850000e-01,3.515000e-01,4.575000e-01,6.095000e-01,8.065000e-01,1.035500e+00,1.324500e+00,1.721500e+00,2.234500e+00,2.899500e+00,4.138500e+00,7.500000e+00};
      std::vector<double> ptBins={0,25,50,75,100,125,150,175,200,225,250};

      //book histograms
      for(int i=0; i<4; i++)
	{
	  std::string prodMode("inc");
	  if(i==1) prodMode="qg";
	  if(i==2) prodMode="qq";
	  if(i==3) prodMode="gg";
	  histos[prodMode+"_ptttbar"]    = bookHisto1D(prodMode+"_ptttbar",10,0,250);
	  for(int j=0; j<5; j++)
	    {
	      std::string fs("ll");
	      double maxMass(200);
	      if(j==1) { fs="bb"; maxMass=300; }
	      if(j==2) { fs="llbb"; maxMass=500; }
	      if(j==3) { fs="llbbmet"; maxMass=1000; }
	      if(j==4) { fs="ttbar"; maxMass=1000; }
	      histos[prodMode+"_m"+fs]        = bookHisto1D(prodMode+"_m"+fs,10,0,maxMass);
	      histos[prodMode+"_pt"+fs]        = bookHisto1D(prodMode+"_pt"+fs,ptBins);
	      if(j>1) continue;
	      histos[prodMode+"_phistar"+fs]  = bookHisto1D(prodMode+"_phistar"+fs,phistarBins);
	      histos[prodMode+"_dphi"+fs]     = bookHisto1D(prodMode+"_dphi"+fs,10,0,180.);
	      histos[prodMode+"_deta"+fs]     = bookHisto1D(prodMode+"_deta"+fs,10,0,5.);
	    }
	}
      
      //profiles and correlations
      for(int j=0; j<5; j++)
	{
	  double maxMass(200);
	  std::string fs("ll");
	  if(j==1) { fs="bb"; maxMass=300; }
	  if(j==2) { fs="llbb"; maxMass=500; }
	  if(j==3) { fs="llbbmet"; maxMass=1000; }
	  if(j==4) { fs="ttbar"; maxMass=1000; }
	  profiles["avgpt_m"+fs]        = bookProfile1D("avgpt_m"+fs,      10,0,maxMass);
	  profiles["avgpt_pt"+fs]       = bookProfile1D("avgpt_pt"+fs,     10,0,250.);
	  profiles["avgpt_dphi2"+fs]    = bookProfile1D("avgpt_dphi2"+fs,  10,0,180.);

	  if(j>3) continue;
	  histos2D["m"+fs+"vsptttbar"]  = bookHisto2D("m"+fs+"vspttbar",   10,0,maxMass,10,0,250.);
	  histos2D["pt"+fs+"vsptttbar"] = bookHisto2D("pt"+fs+"vspttbar",  10,0,300,10,0,250.);

	  if(j>1) continue;
	  profiles["avgpt_dphi"+fs]          =  bookProfile1D("avgpt_dphi"+fs,     10,0,180.);
	  profiles["avgpt_deta"+fs]          = bookProfile1D("avgpt_deta"+fs,     10,0,5.);	 
	  profiles["avgpt_phistar"+fs]       = bookProfile1D("avgpt_phistar"+fs,  phistarBins);
	  histos2D["deta"+fs+"vsptttbar"]    = bookHisto2D("deta"+fs+"vspttbar",10,0,5,10,0,250.);
	  histos2D["dphi"+fs+"vsptttbar"]    = bookHisto2D("dphi"+fs+"vspttbar",10,0,180,10,0,250.);
	  histos2D["phistar"+fs+"vsptttbar"] = bookHisto2D("phistar"+fs+"vspttbar",phistarBins,ptBins);
	}

      profiles["avgpt_nch"]        = bookProfile1D("avgpt_nch",        10,0,200);
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

      //true MC ttbar
      const PartonTop& ttbarState = applyProjection<PartonTop>(event, "ttbar");
      const FourMomentum& gen_ttbar = ttbarState.t1().momentum()+ttbarState.t2().momentum();

      //require 2 leptons op. sign
      const std::vector<DressedLepton> &leptons     = applyProjection<DressedLeptons>(event, "Leptons").dressedLeptons();
      if ( leptons.size() < 2) vetoEvent; 
      if( leptons[0].charge() * leptons[1].charge() >=0 ) vetoEvent; 

      const FourMomentum rec_ll=leptons[0].momentum() + leptons[1].momentum();
      float dphill( fabs(deltaPhi(leptons[0].momentum(),leptons[1].momentum()))*180/PI);
      float detall( fabs(leptons[0].eta()-leptons[1].eta()) );
      const float phi_acop = M_PI - deltaPhi(leptons[0].momentum(),leptons[1].momentum());
      const float costhetastar = tanh( 0.5 * (leptons[0].eta() - leptons[1].eta()) );
      const float sin2thetastar = (costhetastar > 1) ? 0.0 : (1.0 - sqr(costhetastar));
      const float phistar = tan(0.5 * phi_acop) * sqrt(sin2thetastar);

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
      
      //angular variables with b-jets
      const FourMomentum rec_bb=bjets[0].momentum() + bjets[1].momentum();
      float dphibb( fabs(deltaPhi(bjets[0].momentum(),bjets[1].momentum()))*180/PI);
      float detabb( fabs(bjets[0].eta()-bjets[1].eta()) );
      const float phi_acop_bb = M_PI - deltaPhi(bjets[0].momentum(),bjets[1].momentum());
      const float costhetastar_bb = tanh( 0.5 * (bjets[0].eta() - bjets[1].eta()) );
      const float sin2thetastar_bb = (costhetastar_bb > 1) ? 0.0 : (1.0 - sqr(costhetastar_bb));
      const float phistar_bb = tan(0.5 * phi_acop_bb) * sqrt(sin2thetastar_bb);

      // compute the rec_ttbar four vector
      const FourMomentum rec_llbb    = rec_bb + rec_ll;
      const FourMomentum rec_llbbmet = rec_llbb  + missvec;
      
      // now loop over all charged particles and select the ones not associated to the hard event
      const FinalState& chfs = applyProjection<FinalState>(event, "chfs");
      std::vector<float> chMult_dphi2ttbar(10,0),  chMult_dphi2ll(10,0),  chMult_dphi2bb(10,0),  chMult_dphi2llbb(10,0),  chMult_dphi2llbbmet(10,0);
      std::vector<float> chSumPt_dphi2ttbar(10,0), chSumPt_dphi2ll(10,0), chSumPt_dphi2bb(10,0), chSumPt_dphi2llbb(10,0), chSumPt_dphi2llbbmet(10,0);
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
	  float dphi2ttbar( fabs(deltaPhi(gen_ttbar,p.momentum())*180./PI) );
	  float dphi2ll( fabs(deltaPhi(rec_ll,p.momentum())*180./PI) );
	  float dphi2bb( fabs(deltaPhi(rec_bb,p.momentum())*180./PI) );
	  float dphi2llbb( fabs(deltaPhi(rec_llbb,p.momentum())*180./PI) );
	  float dphi2llbbmet( fabs(deltaPhi(rec_llbbmet,p.momentum())*180./PI) );
	  float pt( p.momentum().pT() );
	  
	  int dphi_bin2ttbar(floor(dphi2ttbar/18.));
	  chMult_dphi2ttbar[ dphi_bin2ttbar ] ++;
	  chSumPt_dphi2ttbar[ dphi_bin2ttbar ] += pt;

	  int dphi_bin2ll(floor(dphi2ll/18.));
	  chMult_dphi2ll[ dphi_bin2ll ] ++;
	  chSumPt_dphi2ll[ dphi_bin2ll ] += pt;

	  int dphi_bin2bb(floor(dphi2bb/18.));
	  chMult_dphi2bb[ dphi_bin2bb ] ++;
	  chSumPt_dphi2bb[ dphi_bin2bb ] += pt;

	  int dphi_bin2llbb(floor(dphi2llbb/18.));
	  chMult_dphi2llbb[ dphi_bin2llbb ] ++;
	  chSumPt_dphi2llbb[ dphi_bin2llbb ] += pt;

	  int dphi_bin2llbbmet(floor(dphi2llbbmet/18.));
	  chMult_dphi2llbbmet[ dphi_bin2llbbmet ] ++;
	  chSumPt_dphi2llbbmet[ dphi_bin2llbbmet ] += pt;
	} // loop over particles


      //trace multiplicities as function of the angles
      float nch(0),sumPt(0);
      for(size_t ireg=0; ireg<10; ireg++)
	{
	  nch+=chMult_dphi2ttbar[ireg];
	  sumPt+=chSumPt_dphi2ttbar[ireg];

	  if(chMult_dphi2ttbar[ireg]!=0)
	    {
	      float avgPt_dphi2ttbar=chSumPt_dphi2ttbar[ireg]/chMult_dphi2ttbar[ireg];
	      profiles["avgpt_dphi2ttbar"]->fill(ireg*18.,avgPt_dphi2ttbar,weight);
	    }
	  if(chMult_dphi2ll[ireg]!=0)
	    {
	      float avgPt_dphi2ll=chSumPt_dphi2ll[ireg]/chMult_dphi2ll[ireg];
	      profiles["avgpt_dphi2ll"]->fill(ireg*18.,avgPt_dphi2ll,weight);
	    }
	  if(chMult_dphi2bb[ireg]!=0)
	    {
	      float avgPt_dphi2bb=chSumPt_dphi2bb[ireg]/chMult_dphi2bb[ireg];
	      profiles["avgpt_dphi2bb"]->fill(ireg*18.,avgPt_dphi2bb,weight);
	    }
	  if(chMult_dphi2llbb[ireg]!=0)
	    {
	      float avgPt_dphi2llbb=chSumPt_dphi2llbb[ireg]/chMult_dphi2llbb[ireg];
	      profiles["avgpt_dphi2llbb"]->fill(ireg*18.,avgPt_dphi2llbb,weight);
	    }
	  if(chMult_dphi2llbbmet[ireg]!=0)
	    {
	      float avgPt_dphi2llbbmet=chSumPt_dphi2llbbmet[ireg]/chMult_dphi2llbbmet[ireg];
	      profiles["avgpt_dphi2llbbmet"]->fill(ireg*18.,avgPt_dphi2llbbmet,weight);
	    }
	}
      float avgPt=(nch>0? sumPt/nch : 0.);

      //production mode sensitivity
      for(size_t i=0; i<2; i++)
	{
	  std::string pf(prodMode);
	  if(i==1) pf="inc";
	  histos[pf+"_mttbar"]->fill(gen_ttbar.pt(),weight);
	  histos[pf+"_ptttbar"]->fill(gen_ttbar.pt(),weight);
	  histos[pf+"_mll"]->fill(rec_ll.mass(),weight);
	  histos[pf+"_mbb"]->fill(rec_bb.mass(),weight);
	  histos[pf+"_mllbb"]->fill(rec_llbb.mass(),weight);
	  histos[pf+"_mllbbmet"]->fill(rec_llbbmet.mass(),weight);
	  histos[pf+"_ptll"]->fill(rec_ll.pt(),weight);
	  histos[pf+"_ptbb"]->fill(rec_bb.pt(),weight);
	  histos[pf+"_ptllbb"]->fill(rec_llbb.pt(),weight);
	  histos[pf+"_ptllbbmet"]->fill(rec_llbbmet.pt(),weight);
	  histos[pf+"_phistarll"]->fill(phistar,weight);
	  histos[pf+"_phistarbb"]->fill(phistar_bb,weight);
	  histos[pf+"_dphill"]->fill(dphill,weight);
	  histos[pf+"_dphibb"]->fill(dphibb,weight);
	  histos[pf+"_detall"]->fill(detall,weight);
	  histos[pf+"_detabb"]->fill(detabb,weight);
	}

      //profiles
      profiles["avgpt_nch"]->fill(nch,avgPt,weight);
      profiles["avgpt_mll"]->fill(rec_ll.mass(),avgPt,weight);
      profiles["avgpt_mbb"]->fill(rec_bb.mass(),avgPt,weight);
      profiles["avgpt_mllbb"]->fill(rec_llbb.mass(),avgPt,weight);
      profiles["avgpt_mllbbmet"]->fill(rec_llbbmet.mass(),avgPt,weight);
      profiles["avgpt_mttbar"]->fill(gen_ttbar.mass(),avgPt,weight);
      profiles["avgpt_ptll"]->fill(rec_ll.pt(),avgPt,weight);
      profiles["avgpt_ptbb"]->fill(rec_bb.pt(),avgPt,weight);
      profiles["avgpt_ptllbb"]->fill(rec_llbb.pt(),avgPt,weight);
      profiles["avgpt_ptllbbmet"]->fill(rec_llbbmet.pt(),avgPt,weight);
      profiles["avgpt_ptttbar"]->fill(gen_ttbar.pt(),avgPt,weight);
      profiles["avgpt_dphill"]->fill(dphill,avgPt,weight);
      profiles["avgpt_dphibb"]->fill(dphibb,avgPt,weight);
      profiles["avgpt_detall"]->fill(detall,avgPt,weight);
      profiles["avgpt_detabb"]->fill(detabb,avgPt,weight);
      profiles["avgpt_phistarll"]->fill(phistar,avgPt,weight);
      profiles["avgpt_phistarbb"]->fill(phistar_bb,avgPt,weight);

      //correlations with the recoil
      histos2D["mllvsptttbar"]->fill(rec_ll.mass(),gen_ttbar.pt(),weight);
      histos2D["mbbvsptttbar"]->fill(rec_bb.mass(),gen_ttbar.pt(),weight);
      histos2D["mllbbvsptttbar"]->fill(rec_llbb.mass(),gen_ttbar.pt(),weight);
      histos2D["mllbbmetvsptttbar"]->fill(rec_llbbmet.mass(),gen_ttbar.pt(),weight);
      histos2D["ptllvsptttbar"]->fill(rec_ll.pt(),gen_ttbar.pt(),weight);
      histos2D["ptbbvsptttbar"]->fill(rec_bb.pt(),gen_ttbar.pt(),weight);
      histos2D["ptllbbvsptttbar"]->fill(rec_llbb.pt(),gen_ttbar.pt(),weight);
      histos2D["ptllbbmetvsptttbar"]->fill(rec_llbbmet.pt(),gen_ttbar.pt(),weight);
      histos2D["detallvsptttbar"]->fill(detall,gen_ttbar.pt(),weight);
      histos2D["dphillvsptttbar"]->fill(dphill,gen_ttbar.pt(),weight);
      histos2D["phistarllvsptttbar"]->fill(phistar,gen_ttbar.pt(),weight);
      histos2D["detabbvsptttbar"]->fill(detabb,gen_ttbar.pt(),weight);
      histos2D["dphibbvsptttbar"]->fill(dphibb,gen_ttbar.pt(),weight);
      histos2D["phistarbbvsptttbar"]->fill(phistar_bb,gen_ttbar.pt(),weight);
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
