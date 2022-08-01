
void Fill_d_Option_Histogram(){
	
	string SampleName	 = "CMS-SUS-19-005";

	string BaseFileName	 = "histoOut-"+SampleName+"_CutLang.root";
	string NewFileName	 = "histoOut-"+SampleName+"_deps_CutLang.root";
	string TextFileName	 =			   SampleName+"_dependencies.txt";
	//[ BaseFileName root file ] comes from CutLang with no option
	//[ NewFileName root file ]  comes from CutLang with -d option

	bool FileNameChecking = false;
	if(FileNameChecking == true){
		cout<<"Now, File Name checking..."<<endl;
		cout<<"No option root file name = "<<BaseFileName<<endl;
		cout<<"-d option root file name = "<<NewFileName<<endl;
		cout<<"Text file name = "<<TextFileName<<endl;
	}

	TFile* fBase = new TFile(BaseFileName.c_str(), "READ");
	TFile* fNew	 = new TFile(NewFileName.c_str() , "UPDATE");
	ifstream DirNameFile; DirNameFile.open(TextFileName);

	if( (!fBase)||(!fNew)||(!DirNameFile) )
	{
		cerr << endl;
		cerr << "====================================================" << endl;
		cerr << "Error, There are no necessary file" << endl;
		cerr << "You need 3 file for [ Fill_d_Option_Histogram ] function" << endl;
		cerr << "====================================================" << endl;
		cerr << endl;
		exit(100);
	}
	
	string CardName;
	string DirName;
	int iter = 0;

//=====================================================================
	while(!DirNameFile.eof()){

		if(iter == 0) DirNameFile >> CardName;
		//First line of Textfile is no need for this process
		
		else if(iter >= 1){
		
			DirNameFile >> CardName >> DirName;
			//cout<<"DirName = "<<DirName<<endl;
	
			fBase->cd();
			TDirectory* Base = (TDirectory*)fBase->Get(DirName.c_str());
			Base->cd();
			TH1D* Histo_Base = (TH1D*)Base->Get("cutflow")->Clone("Histo_Base");
		
			fNew->cd();
			TDirectory* New = (TDirectory*)fNew->Get(DirName.c_str());
			New->cd();
			TH1D* Histo_New = (TH1D*)New->Get("cutflow");

			int Nbins = Histo_Base->GetNbinsX();
			for (int i_bin = 1; i_bin < Nbins; i_bin++){
				double BaseBinContent =	 Histo_Base -> GetBinContent(i_bin);
				double NewBinContent  =	 Histo_New  -> GetBinContent(i_bin);
				if( NewBinContent== 0.0) Histo_New->SetBinContent(i_bin, BaseBinContent);
			
			}
			New->Write("",TObject::kOverwrite);

		}//End of else if(iter >=1)

	iter++;

	if(iter>=100000) break;
	//Break for iteration infinite error
	//If ROOT file has [ directories >= 100000 ], then delete this line

	}//End of While
//=====================================================================

	fBase->Close();
	fNew->Close();
	DirNameFile.close();
	//Close Opening file

}//End of Main function


