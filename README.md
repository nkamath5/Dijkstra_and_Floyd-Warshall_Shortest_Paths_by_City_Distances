# Dijkstra_and_Floyd-Warshall_Shortest_Paths_by_City_Distances

main.cpp contains the code to run the Dijkstra (label setting) and Floyd-Warshall (label correcting) graph algorithms on a dataset of 128 cities. Details of problem solved are in the last paragraph of this README file.

sgb128_distance.txt & sgb128_name.txt are source files for the code.

The dataset that this program uses can be downloaded from the following website:
https://people.sc.fsu.edu/~jburkardt/datasets/cities/cities.html. The name of the dataset is
‘SGB128’, which contains 128 cities in North America. The relevant files are the distance matrix
and the names of each city.

# Problem Description: 
An undirected network was connected for cities with distance less than 100 miles. If there are no cities within 100 miles for a city i, it is connected to its nearest city. 

Dijsktra’s Algorithm to finds the shortest paths from Springfield, IL to all the other cities. Reports the SP distance to Saint Johnsbury, VT.

Floyd-Warshall Algorithm finds all-pairs shortest paths between all the cities. Reports the shortest path distance for the (s, t)-pair corresponding to the largest SP distance.
