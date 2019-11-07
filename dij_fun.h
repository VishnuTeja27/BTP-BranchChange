#include <bits/stdc++.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/random.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphviz.hpp>



using namespace std;
using namespace boost;


struct VP
{
  int type=0;// 0 for student,1 for stream,2 for buffer
  int ID;
  string name;
};
struct EP
{
  int priority;
  //int weight;
  int count;    // for duplicates for the buffer node
};

using G = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,VP,EP>;
using V = G::vertex_descriptor;
using E = G::edge_descriptor;

typedef graph_traits<G>::vertex_iterator vertex_iterator;
typedef graph_traits<G>::edge_iterator edge_iterator;
typedef graph_traits < G >::out_edge_iterator out_ei;
typedef graph_traits < G >::in_edge_iterator in_ei;


struct my_visitor : boost::default_dijkstra_visitor {
    using base = boost::default_dijkstra_visitor;
    struct done{};

    my_visitor(V vd, size_t& visited) : destination(vd), visited(visited) {}

    void finish_vertex(V v, G const& g) {
        ++visited;

        if (v == destination)
            throw done{};

        base::finish_vertex(v, g);
    }

  private:
    V destination;
    size_t &visited;
};

G rev_graph(G req_g,G cycle,vertex_iterator v)
{

  clear_vertex(*v,req_g);
  clear_vertex(*v,cycle);

  edge_iterator ei,ei_end;
  print_graph(cycle);
  if(num_edges(cycle)<1)
  {
    return req_g;
  }
  for (boost::tie(ei, ei_end) = edges(cycle); ei != ei_end; ++ei)
  {
    auto source = boost::source ( *ei, cycle );
    auto target = boost::target ( *ei, cycle );
    if(edge(target,source,req_g).second)
    {
      auto ed=edge(target,source,req_g).first;
      req_g[ed].count++;
    }
    else
    {
      auto e=add_edge(target,source,req_g).first;
      auto re =edge(source,target,req_g).first;
      req_g[e].priority=req_g[re].priority;
      req_g[e].count=1;
    }
    auto rem=edge(source,target,req_g).first;
    if(req_g[rem].count>1)req_g[rem].count--;
    else remove_edge(source,target,req_g);
  }


  return req_g;

}


G add_graphs(G cycle,G p_sol)
{
  vertex_iterator v, vend;

  edge_iterator ei,ei_end;

  cout<<"Partial solution(prev): \n";

  print_graph(p_sol);


  for (boost::tie(ei, ei_end) = edges(cycle); ei != ei_end; ++ei)
  {
    auto source = boost::source ( *ei, cycle );
    auto target = boost::target ( *ei, cycle );
    if(edge(target,source,p_sol).second)
    {
      // auto ed =edge(target,source,cycle).first;
      // if(p_sol[ed].count>1)p_sol[ed].count--;
     remove_edge(target,source,p_sol);
    }
    else if(!edge(source,target,p_sol).second)
    {
      auto e=add_edge(source,target,p_sol).first;
      auto re =edge(source,target,cycle).first;
      p_sol[e].priority=cycle[re].priority;
      p_sol[e].count=1;
    }
    else
    {
      auto ed=edge(source,target,p_sol).first;
      p_sol[ed].count++;
    }

  }
  return p_sol;
}


G  dij(G g,G cycle,V start_vertex,V end_vertex,int k)
{

    size_t visited;
    // G cycle(k);
    std::vector<V>                         _pred(num_vertices(g),   g.null_vertex());
    std::vector<size_t>                    _dist(num_vertices(g),   -1ull);



    my_visitor vis { end_vertex, visited };
    auto predmap = _pred.data(); // interior properties: boost::get(boost::vertex_predecessor, g);
    auto distmap = _dist.data(); // interior properties: boost::get(boost::vertex_distance, g);

    try {
        // std::cout << "Searching from #" << start_vertex << " to #" << end_vertex << "...\n";
        boost::dijkstra_shortest_paths(g, start_vertex,
                boost::visitor(vis).
                // color_map(colors.data()).
                distance_map(distmap).
                predecessor_map(predmap).
                weight_map(boost::make_constant_property<E>(1ul))
            );

        std::cout << "No path found\n";
        deque<V> temp_path;
        return cycle;
    } catch(my_visitor::done const&) {
        std::cout << "\n "  ;
    }

    size_t distance = distmap[end_vertex];
    // std::cout << "Distance from #" << start_vertex << " to #" << end_vertex << ": " << distance << "\n";

    if (distance != size_t(-1)) {
        std::deque<V> path;
        // cout<<"start\n";
        auto e=add_edge(end_vertex,start_vertex,cycle).first;
        auto re =edge(end_vertex,start_vertex,g).first;
        cycle[e].priority=g[re].priority;
        cycle[e].count=g[re].count;
        for (V current = end_vertex;
                current != g.null_vertex()
                && predmap[current] != current
                && current != start_vertex;)
        {
            path.push_front(predmap[current]);
            // cout<<current<<" "<<predmap[current]<<" \n";
            auto e=add_edge(predmap[current],current,cycle).first;
            auto re =edge(predmap[current],current,g).first;
            cycle[e].priority=g[re].priority;
            cycle[e].count=g[re].count;
            current = predmap[current];
        }

        // std::cout << "Path from #" << start_vertex << " to #" << end_vertex << ": ";
        // std::copy(path.begin(), path.end(), std::ostream_iterator<V>(std::cout, ", "));
        // std::cout << end_vertex << "\n";
        return cycle;
    }
}
