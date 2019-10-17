//Tracking the pmt hits

void XebraAnalysisManager::BeginOfRun(const G4Run *)
{
  m_pTree->Branch("ntpmthits", &m_pEventData->m_iNbTopPmtHits, "ntpmthits/I"); 
  m_pTree->Branch("nbpmthits", &m_pEventData->m_iNbBottomPmtHits, "nbpmthits/I");
  m_pTree->Branch("pmthits", "vector<int>", &m_pEventData->m_pPmtHits);
}

void XebraAnalysisManager::BeginOfEvent(const G4Event *)
{

 if(m_iPmtHitsCollectionID == -1)
  {
    G4SDManager *pSDManager = G4SDManager::GetSDMpointer();
    m_iPmtHitsCollectionID = pSDManager->GetCollectionID("PmtHitsCollection");
  }
  
}

void XebraAnalysisManager::EndOfEvent(const G4Event *pEvent)
{
  G4HCofThisEvent* pHCofThisEvent = pEvent->GetHCofThisEvent();
  
  XebraPmtHitsCollection* pPmtHitsCollection = 0;
  
  G4int iNbLXeHits = 0, iNbPmtHits = 0, iNbLScintHits = 0;
	
  if(pHCofThisEvent)
  {
      if(m_iPmtHitsCollectionID != -1)
	  {
        pPmtHitsCollection = (XebraPmtHitsCollection *)(pHCofThisEvent->GetHC(m_iPmtHitsCollectionID));
        iNbPmtHits = (pPmtHitsCollection)?(pPmtHitsCollection->entries()):(0);
	  }
      
  }
  
  G4int iNbTopPmts = 7;
  G4int iNbBottomPmts = 1;

  m_pEventData->m_pPmtHits->resize(iNbTopPmts+iNbBottomPmts, 0);
  
  
      for(G4int i=0; i<iNbPmtHits; i++)
    {
        (*(m_pEventData->m_pPmtHits))[(*pPmtHitsCollection)[i]->GetPmtNb()]++;

    } 
  
    m_pEventData->m_iNbBottomPmtHits = accumulate(m_pEventData->m_pPmtHits->begin(), m_pEventData->m_pPmtHits->begin()+iNbBottomPmts, 0);
    m_pEventData->m_iNbTopPmtHits = accumulate(m_pEventData->m_pPmtHits->begin()+iNbBottomPmts, m_pEventData->m_pPmtHits->begin()+iNbBottomPmts+iNbTopPmts, 0);
    
      // save only energy depositing events
  if(writeEmptyEvents)
  {
    m_pTree->Fill(); // write all events to the tree
  }
  else
  {
  // if(fTotalEnergyDeposited > 0.) m_pTree->Fill();
  if(fTotalEnergyDeposited > 0. || iNbPmtHits > 0) m_pTree->Fill(); // only events with some activity are written to the tree
  }
  
}
  
  
void XebraPmtSensitiveDetector::Initialize(G4HCofThisEvent* pHitsCollectionOfThisEvent)
{
	m_pPmtHitsCollection = new XebraPmtHitsCollection(SensitiveDetectorName, collectionName[0]);

	static G4int iHitsCollectionID = -1;

	if(iHitsCollectionID < 0)
		iHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	
	pHitsCollectionOfThisEvent->AddHitsCollection(iHitsCollectionID, m_pPmtHitsCollection); 
}




G4bool XebraPmtSensitiveDetector::ProcessHits(G4Step* pStep, G4TouchableHistory *)
{
	G4Track *pTrack = pStep->GetTrack();

	if(pTrack->GetDefinition()->GetParticleName() == "opticalphoton")
	{
		XebraPmtHit* pHit = new XebraPmtHit();
		//G4cout << "PMT hit" << pTrack->GetTouchable()->GetVolume(1)->GetCopyNo() << G4endl;// DR 20160720 Testing PMTs sensitivity

		pHit->SetPosition(pStep->GetPreStepPoint()->GetPosition());
		pHit->SetTime(pTrack->GetGlobalTime());
		pHit->SetPmtNb(pTrack->GetTouchable()->GetVolume(2)->GetCopyNo()); //TPC PMTs 
                //pHit->SetPmtNb(pTrack->GetTouchable()->GetVolume(1)->GetCopyNo()); //WaterTank PMTs

		m_pPmtHitsCollection->insert(pHit);
		


		return true;
	}
	else
	{
		return false;
	}
}



  

