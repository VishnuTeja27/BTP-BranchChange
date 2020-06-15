#include"fun.h"
#include"dij_fun.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[])
{
  vector<stream> streams;     // Details of all the branches
  vector<student> students;   // Details of all the students
  int m;                      // Number of streams
  int n;                      // Number of Students
  int mode;

  std::unordered_map<string, int> stream_nodes;   //map for branch and respective node
  pair<vector<stream>,vector<student>> Temp;
  // string file_name(argv[0]);
  std::string filename(argv[1]);
  Temp=get_data(filename);      // gets all the details from a file
  streams=Temp.first;
  students=Temp.second;
  mode=get_mode();
  // cout<<"Rule - 5.6:"<<mode<<"\n";
  m=streams.size();
  n=students.size();

  for(int i=0;i<m;i++)
  {
    stream_nodes[streams[i].ID]=i+1;
  }
  // cout<<"Details of the Streams in the institute:\n";
  // for(int i=0;i<m;i++)
  // {
  //   cout<<streams[i].name<<" "<<streams[i].ID<<": "<<streams[i].min_strength<<" "<<streams[i].max_strength<<"\n";
  // }
  //
  // cout<<"Students in the descending order of CGPA\n";
  // for(int i=0;i<n;i++)
  // {
  //   cout<<students[i].roll_no<<" "<<students[i].name<<" "<<students[i].CGPA<<" "<<students[i].current_stream<<"\n";
  // }

  G g(m+n+1);
  G p_sol(m+n+1);

  vertex_iterator v, vend;

  int i=0,student_count=0;

  for (boost::tie(v, vend) = vertices(g); v != vend; ++v)
  {
    if(i==0)//buffer
    {
      g[*v].type=2;     //add the edges to streams from this buffer
      g[*v].name="Buffer";
      p_sol[*v].type=2;
      p_sol[*v].name="Buffer";
      // cycle[*v].type=2;
      // cycle[*v].name="Buffer";

      for(int j=0;j<m;j++)
      {
        auto e=add_edge(0,j+1,g).first;
      //  auto e=egde(0,j+1,g).first;
        g[e].count=streams[j].min_strength;
        g[e].priority=-1;
      }
    }
    else if(i>0 && i<=m)//streams
    {
      g[*v].type=1;
      g[*v].name=streams[i-1].name;
      g[*v].locked=0;
      p_sol[*v].type=1;
      p_sol[*v].name=streams[i-1].name;
      // cycle[*v].type=1;
      // cycle[*v].name=streams[i-1].name;

      auto e = add_edge(i,0,g).first;
      g[e].count=streams[i-1].max_strength;
      g[e].priority=-1;


    }
    else  //students
    {
      g[*v].type=0;
      g[*v].name=students[i-m-1].name;
      p_sol[*v].type=0;
      p_sol[*v].name=students[i-m-1].name;
      // cycle[*v].type=0;
      // cycle[*v].name=students[i-m-1].name;
      auto pref=students[i-m-1].preferences;

      for(int k=0;k<pref.size();k++)
      {
        int temp_node=stream_nodes.at(pref[k]);
        auto e=add_edge(i,temp_node,g).first;
        g[e].priority=k+1;
        g[e].count=1;
      }
      string current_stream=students[i-m-1].current_stream;
      auto e = add_edge(stream_nodes.at(current_stream),i,g).first;
      g[e].priority=m;
      g[e].count=1;
    }
    i++;
  }

  // std::cout << "\nInitial Graph: " << std::endl;
  // boost::print_graph(g);
  // std::cout << std::endl;

  // auto w_map = boost::get(&VP::type, g);
  // ofstream dot_file("original_graph.dot");
  // write_graphviz(dot_file, g, boost::make_label_writer(get(&VP::name,g)),boost::make_label_writer(boost::get(&EP::priority, g)));
//
//---------------------------------------------------------------///

i=0;
int flag=0;

for (boost::tie(v, vend) = vertices(g); v != vend; ++v)   // loop for every student in request graph
{
  if(i<m+1)//buffer
  {
    i++;
    continue;
  }
  // cout<<"Current Student: "<<g[*v].name<<"\n";
  // auto pref=students[i-m-1].preferences;
  out_ei ei,ei_end;
  pair <in_ei,in_ei> in_e;
  in_e = in_edges(*v, g);
  int curr_prio;
  curr_prio=g[*(in_e.first)].priority;
  // cout<<"Current priority:"<<g[*(in_e.first)].priority<<"\n";
  G cycle(m+n+1);
  V target;

  for (boost::tie(ei, ei_end) = out_edges(*v, g); ei != ei_end; ++ei)   // every student's priority check
  {
    auto source = boost::source ( *ei, g );
     target = boost::target ( *ei, g );
    if(g[*ei].priority >= curr_prio)
    {
      continue;
    }
    if(mode==1 && g[target].locked==1)
    {
      continue;
    }
    // cout<<"Request_graph";
    // print_graph(g);
    // std::cout << "There is an edge from " << source <<  " to " << target << " priority "<<g[*ei].priority;

    cycle=dij(g,cycle,target,source,m+n+1);
    if(num_edges(cycle)==0)continue;
    // std::cout << "\nCycle :" << std::endl;
    // print_graph(cycle);
    // std::cout << std::endl;
    // w_map = boost::get(&VP::type, cycle);
    // ofstream dot_file1("cycle.dot");
    // write_graphviz(dot_file1, cycle, boost::make_label_writer(get(&VP::name,cycle)),boost::make_label_writer(boost::get(&EP::priority, cycle)));

    break;
  }
  // flag=0;
  if(num_edges(cycle)==0)
  {
    g[target].locked=1;
  }

  // copy this cycle to partial solution
  p_sol=add_graphs(cycle,p_sol);
  // std::cout <<"\nPartial solution(current)"<< std::endl;

  // print_graph(p_sol);
  // std::cout <<"\noriginal:"<< std::endl;

  // w_map = boost::get(&VP::type, p_sol);
  // ofstream dot_file1("p_sol.dot");
  // write_graphviz(dot_file1, p_sol, boost::make_label_writer(get(&VP::name,p_sol)),boost::make_label_writer(boost::get(&EP::priority, p_sol)));
  // print_graph(g);

  g=rev_graph(g,cycle,v);
  // std::cout <<"\nResidual Graph:"<< std::endl;

  // print_graph(g);
  // w_map = boost::get(&VP::type, g);
  // ofstream dot_file("residual_graph.dot");
  // write_graphviz(dot_file, g, boost::make_label_writer(get(&VP::name,g)),boost::make_label_writer(boost::get(&EP::priority, g)));

  // break;


  // remove current student and reverse the edges in the request graph that are present in cycle

 }
 // w_map = boost::get(&VP::type, p_sol);
 // ofstream dot_file1("solution.dot");
 // write_graphviz(dot_file1, p_sol, boost::make_label_writer(get(&VP::name,p_sol)),boost::make_label_writer(boost::get(&EP::priority, p_sol)));

 print_solution(p_sol,m);
}
