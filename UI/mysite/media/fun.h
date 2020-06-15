#include <bits/stdc++.h>

using namespace std;
int mode;

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

int get_mode()
{
  return mode;
}


pair< vector<stream>,vector<student> >get_data()
{
  ifstream file;
  string w;
  int m,n;
  vector<stream> branch;
  vector<student> stud;


  file.open("input.txt");
  if (!file.is_open())
  {
    cout<<"error 1 reading file";
  }
  file>>w;
  mode=stoi(w);
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
    temp.min_strength=ceil(0.1*temp.exis_strength);
    cout<<temp.name <<" : max:"<<temp.max_strength<<"  min: "<<temp.min_strength<<"\n";
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


  sort(stud.begin(),stud.end(),student_sorter);


  return (make_pair(branch,stud));
}
