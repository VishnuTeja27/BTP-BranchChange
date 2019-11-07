#include <bits/stdc++.h>

using namespace std;

struct stream
{
  string name;
  string ID;
  int san_strength;
  int exis_strength;
  int max_strength;
  int min_strength;
};

struct student
{
  int roll_no;
  string name;
  string current_stream;
  float CGPA;
  int jee_rank;
  vector<string> preferences;
};

bool student_sorter(struct student a, struct student b)
{
  if(a.CGPA!=b.CGPA)return (a.CGPA>b.CGPA);
  return (a.jee_rank<b.jee_rank);
}


pair< vector<stream>,vector<student> >get_data()
{
  ifstream file;
  string w;
  int m,n;
  vector<stream> branch;
  vector<student> stud;


  file.open("test3.txt");
  if (!file.is_open())
  {
    cout<<"error 1 reading file";

  }
  file>>w;
  m=stoi(w);
  file>>w;
  n=stoi(w);

  for(int i=0;i<m;i++)      //  reading branches data
  {
    stream temp;
    file>>w;
    temp.name=w;
    file>>w;
    temp.ID=w;
    file>>w;
    temp.san_strength=stoi(w);
    file>>w;
    temp.exis_strength=stoi(w);
    temp.max_strength=1.1*temp.san_strength-temp.exis_strength;
    temp.min_strength=temp.exis_strength-0.9*temp.exis_strength;
    branch.push_back(temp);
  }

  for(int i=0;i<n;i++)      //reading student data
  {
    student std;
    int count;
    file>>w;
    std.roll_no=stoi(w);
    file>>w;std.name=w;
    file>>w;std.jee_rank=stoi(w);

    file>>w;std.current_stream=w;
    file>>w;std.CGPA=stof(w);
    // file>>w;std.jee_rank=stoi(w);
    file>>w;count=stoi(w);
    for(int k=0;k<count;k++)
    {
      file>>w;
      std.preferences.push_back(w);
    }
    stud.push_back(std);
  }

  // cout<<stud[7].name<<" "<<stud[6].preferences.size();
  // vector<pair<int,pair<int,int>>> res;

  // res=calculate_floor_ceil(branch);
  sort(stud.begin(),stud.end(),student_sorter);

  // cout<<"Students in the descending order of CGPA\n";
  // for(int i=0;i<n;i++)
  // {
  //   cout<<stud[i].roll_no<<" "<<stud[i].name<<" "<<stud[i].CGPA<<" "<<stud[i].current_stream<<"\n";
  // }

  // for(int i=0;i<m;i++)
  // {
  //   cout<<branch[i].ID<<": "<<branch[i].min_strength<<" "<<branch[i].max_strength<<"\n";
  // }
  // cout<<stud[0].name<<stud[1].name<<stud[2].name;
  return (make_pair(branch,stud));
}
