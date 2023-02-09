


#include"../../include.h"
using namespace std;

mms::mms(string ifn_dat)
{
  mms::read(ifn_dat.c_str());
}

void mms::set(vector<mm> mms)
{
  this->_mms=mms;
}

void mms::set(Eigen::VectorXd volume_vec)
{
  int n_shim=this->_mms.size();
  int size_eigen=volume_vec.rows();
  if(n_shim!=size_eigen)
    {
      cout<<"You are trying to substitute a different number of magnetic moments"<<endl;
      return;
    }
  double volume;
  for(int i_shim=0;i_shim<n_shim;i_shim++)
    {
      volume=volume_vec(i_shim);
      this->_mms[i_shim].set_volume(volume);
    }
}

void mms::set(info_SVD information)
{
  auto volume_eigen=information.info_svd_eigen();
  mms::set(volume_eigen);
}

void mms::set_volume(int i_shim,double volume)
{
  int n_shim=this->_mms.size();
  if(n_shim<=i_shim)
    {
      cout<<"The nember of the shims is "<<n_shim<<endl;
      cout<<"But you somehow want to know "<<i_shim<<"th volume."<<endl;
      return;
    }
  this->_mms[i_shim].set_volume(volume);
}

Eigen::VectorXd mms::mms_eigen()
{
  int n_shim=this->_mms.size();
  Eigen::VectorXd volume_eigen(n_shim);
  for(int i_shim=0;i_shim<n_shim;i_shim++)
    {
      volume_eigen(i_shim)=this->_mms[i_shim].volume();
    }
  return volume_eigen;
}

void mms::push_back(mm mm)
{
  this->_mms.push_back(mm);
}

void mms::Push_Back(mms mms)
{
  auto mms_vec=(vector<mm>)mms;
  int n_mm=mms_vec.size();
  for(int i_mm=0;i_mm<n_mm;i_mm++)
    {
      this->push_back(mms_vec[i_mm]);
    }
}

mms::operator vector<mm>()
{
  return this->_mms;
}

int mms::n_shim()
{
  int _n_shim=this->_mms.size();
  return _n_shim;
}

mm mms::at(int index)
{
  mm mm=this->_mms.at(index);
  return mm;
}

double mms::volume(int index)
{
  double volume=at(index).volume();
  return volume;
}

TVector3 mms::position(int index)
{
  TVector3 _position;
  _position=at(index).pos();
  return _position;
}

TVector3 mms::B(TVector3 MFEP)
{
  vector<mm> mms=this->_mms;
  TVector3 _B;
  for(int i_mm=0;i_mm<mms.size();i_mm++)
    {
      _B+=mms[i_mm].b(MFEP);
    }
  return _B;
}

double mms::Flux(TVector3 MFEP)
{
  vector<mm> mms=this->_mms;
  double _flux_mm=0.;
  for(int i_mm=0;i_mm<mms.size();i_mm++)
    {
      _flux_mm+=mms[i_mm].flux(MFEP);
    }
  return _flux_mm;
}

double mms::TotalVolume(string unit)
{
  double volume;
  vector<double> volume_vec;
  vector<mm> mms=this->_mms;
  for(int i_mm=0;i_mm<mms.size();i_mm++)
    {
      volume=mms[i_mm].volume();
      volume_vec.push_back(volume);
    }
  double total_volume=accumulate(volume_vec.begin(),volume_vec.end(),0.);
  if(unit=="m3")
    {
      return total_volume;
    }
  else if(unit=="cc")
    {
      return total_volume*1.e6;
    }
  cout<<unit<<" is an invalid argument for mms::TotalVolume()"<<endl;
  return nan("");
}

double mms::AveragedVolume()
{
  double _TotalVolume=mms::TotalVolume();
  int number_pocket=this->_mms.size();
  double _AveragedVolume=_TotalVolume/number_pocket;
  return _AveragedVolume;
}

void mms::tilt(double theta,double phi)
{
  for(int i_flc=0;i_flc< this->_mms.size();i_flc++)
    {
      this->_mms[i_flc].settilt(theta,phi);
    }
}

void mms::dis(double x_dis,double y_dis,double z_dis)
{
  TVector3 pos_dis(x_dis,y_dis,z_dis);
  for(int i_mm=0;i_mm< this->_mms.size();i_mm++)
    {
      this->_mms[i_mm].setdis(pos_dis);
    }
}

void mms::read(const char* path_to_filename)
{
  if(_mms.size())
    {
      return;
    }
  ifstream ifn(path_to_filename);
  double x,y,z;
  mm mm_tmp;
  string line;
  TVector3 position;
  for(int row=0;getline(ifn,line);++row)
    {
      istringstream stream(line);
      for(int col=0;stream>>x >>y >>z ;++col)
        {
          position.SetXYZ(x,y,z);
          mm_tmp.set(position);
          push_back(mm_tmp);
        }
    }
}

void mms::draw(string ofn_root)
{
  vector<mm> _mm=this->_mms;
  int n_theta=number_shim_tray;
  int n_z=number_shim_pocket;
  double range_theta=1.+2./(double)n_theta;
  double range_z=length_shim_tray*(1.+1./(double)n_z);
  TH2D* h=new TH2D("h",";#theta (rad/#pi);z (m)",
                   n_theta,-1.+1./(double)n_theta,1.+1./(double)n_theta,
                   n_z,-range_z/2.,range_z/2.);
  for(mm mm:_mm)
    {
      double x=mm.pos().x();
      double y=mm.pos().y();
      double theta=atan2(y,x);
      double z=mm.pos().z();
      double volume=mm.volume()*1.e6;//[cc]
      volume=round(volume*1.e2)*1.e-2;
      h->Fill(theta/TMath::Pi(),z,volume);
    }
  ArrangeRange(h);
  auto ax=h->GetXaxis();
  ax->SetNdivisions(202);
  ax->SetLabelSize(.07);
  ax->SetTitleSize(.07);
  ax->CenterTitle(true);
  ax->ChangeLabel(1,-1,-1,-1,-1,-1,"-1");
  ax->ChangeLabel(-1,-1,-1,-1,-1,-1,"1");

  auto ay=h->GetYaxis();
  ay->SetNdivisions(202);
  ay->SetLabelSize(.07);
  ay->SetTitleSize(.07);
  ay->CenterTitle(true);
  ay->ChangeLabel(1,-1,-1,-1,-1,-1,"- 0.7");
  ay->ChangeLabel(-1,-1,-1,-1,-1,-1,"0.7");

  h->GetZaxis()->SetLabelSize(.07);
  h->GetZaxis()->SetRangeUser(0.,20.);//volume_maximum_iron_shim*1.e6);
  //  h->GetZaxis()->SetTitle("Iron volume (cm^{3}/pocket)");
  h->GetZaxis()->SetTitleSize(.07);
  h->GetZaxis()->SetNdivisions(1);
  h->GetZaxis()->CenterTitle(true);
  h->GetZaxis()->SetTitleOffset(.4);

  auto *c=new TCanvas("c","c");
  const char* option_TH2D="colz";
  int kGreyScale=53;
  gStyle->SetPalette(kGreyScale);
  TColor::InvertPalette();
  auto* st_h=(TPaveStats*)h->FindObject("stats");
  h->SetStats(0);
  h->SetMarkerSize(0.7);
  h->Draw(option_TH2D);
  h->SetMarkerSize(.8);
  c->SetBottomMargin(.15);
  c->SetLeftMargin(.11);
  TFile* f=new TFile(ofn_root.c_str(),"recreate");
  c->Write();
  f->Close();
  delete c;
  delete f;
  delete h;
}


void mms::check_constraints(double v_min,double v_max,double unit_shim)
{
  double volume;
  for(mm& shim:this->_mms)
    {
      //      shim.VolumeRounding(unit_shim);
      volume=shim.volume();
      if(volume<v_min)
        {
          shim.set_volume(v_min);
        }
      else if(v_max<volume)
        {
          shim.set_volume(v_max);
        }
    }
}

int mms::count(string range,double volume_min,double volume_max)
{
  int count_low=0,count_OK=0,count_high=0;
  double volume;
  for(int i_shim=0;i_shim< this->_mms.size();i_shim++)
    {
      volume=this->_mms[i_shim].volume();
      if(volume<volume_min)
        {
          count_low++;
        }
      else if(volume_max<volume)
        {
          count_high++;
        }
      else
        {
          if(volume<volume_min || volume_max<volume)
            {
              cout<<volume<<endl;
            }
          count_OK++;
        }
    }
  if(range=="low")
    {
      return count_low;
    }
  if(range=="ok")
    {
      return count_OK;
    }
  if(range=="high")
    {
      return count_high;
    }
  return 0;
}

vector<double> mms::mms_vec()
{
  vector<double> _mms_vec;
  double volume;
  for(mm mm:this->_mms)
    {
      volume=mm.volume();
      _mms_vec.push_back(volume);
    }
  return _mms_vec;
}

void mms::add_mms(mms mms1,mms mms2)
{
  int n_shim_1=((vector<mm>)mms1).size();
  int n_shim_2=((vector<mm>)mms2).size();
  if(n_shim_1!=n_shim_2)
    {
      puts("size of 2 mms is different");
      return;
    }
  int n_shim=n_shim_1;
  
  for(int i_shim=0;i_shim<n_shim;i_shim++)
    {
      double volume_1=mms1.volume(i_shim);
      double volume_2=mms2.volume(i_shim);
      double summedvolume=volume_1+volume_2;
      mms::set_volume(i_shim,summedvolume);
    }
}

void mms::clear()
{
  for(mm& shim:this->_mms)
    {
      shim.clear();
    }
}

double mms::UseRate()
{
  int size_shim=this->_mms.size();
  double v_total_max=volume_maximum_iron_shim*size_shim;
  double v_total=mms::TotalVolume();
  double _UseRate=v_total/v_total_max;
  return _UseRate;
}

void mms::load(string ifn_dat)
{
  ifstream InputFile(ifn_dat.c_str());
  string line;
  int n_shim=this->_mms.size();
  Eigen::VectorXd volumes(n_shim);
  for(int i_shim=0;getline(InputFile,line);i_shim++)
    {
      double volume=stod(line);
      volumes(i_shim)=volume;
    }
  mms::set(volumes);
}

int mms::n_stuck(string HighOrLow)
{
  int count=0;
  double tolerance=1.e-12;
  for(auto piece:this->_mms)
    {
      double v=piece.volume();
      if(HighOrLow=="low")
        {
          count+=(v<tolerance);
        }
      if(HighOrLow=="high")
        {
          count+=(volume_maximum_iron_shim-tolerance<v);
        }
    }
  return count;
}











