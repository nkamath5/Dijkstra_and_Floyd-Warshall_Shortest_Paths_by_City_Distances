/* The dataset that this program uses can be downloaded from the following website:
https://people.sc.fsu.edu/~jburkardt/datasets/cities/cities.html. The name of the dataset is
‘SGB128’, which contains 128 cities in North America. The relevant files are the distance matrix
and the names of each city.*/

#include <climits> // for INT_MAX etc
#include <iostream>
#include <fstream> // file stream
#include <sstream> // for string stream used to extract number
#include <queue> // for priority queue
#include <array>
#include <numeric>

using namespace std;

const int n_cities = 128;
string cities[n_cities];
short distances[n_cities][n_cities];
short adjacency_matrix[n_cities][n_cities] {};
short degree[n_cities] {};

short getNumber (string strg) { // Extracts an integer from a sentence / string.
    stringstream stream;
    stream.str(strg);
    short i {};
    string temp;
    while (!stream.eof()) {
        stream >> temp;
        if (stringstream(temp) >> i)
            return i;
        temp = "";
    }
    return -1; // default return is - since calling function uses return 0 as an indication of something else
}

void get_Data() {
    ifstream file;
    file.open("sgb128_name.txt");
    string str;
    //NodeData node;
    short i =0;
    if (file.is_open()) {
        getline(file, str, '\n');
        getline(file, str, '\n'); // two lines of the file are metadata that we wanna ignore
        while (!file.eof()) {
            getline(file, str, '\n');
            cities[i++] = str;
        }
    }
    file.close();
    /*cout << "Below is the list of all cities read from file: " << endl;
    for (int i = 0; i< n_cities;i++) {  // prints city list
        cout<< cities[i]<< endl;
    }*/
    file.open("sgb128_distance.txt");
    if(file.is_open()) {
        short flag = 0, j = 0; // flag 0 indicates all metadata hasn't been passed thru and ignored yet
        i = 0;
        while(!file.eof()) {
            getline(file, str, ' ');
            short number_extracted = getNumber(str);
            if(number_extracted ==0) flag = 1; // flag 1 means real distance data has been encountered in the file
            if(flag == 1 && number_extracted!=-1) {
                distances[i][j++] = number_extracted;
                if(j == n_cities) {i++; j = 0;}
                if(i == n_cities) break;
            }
        }

    }
    file.close();
    /*cout << "Below is the distance matrix read from file: " << endl;
    for (i = 0; i< n_cities;i++) {  // prints city list
        for(short j=0;j< n_cities;j++) {
            cout << distances[i][j]<< " ";
        }
        cout<<endl;
    }*/
}
void create_network(){
    for(int i = 0; i<n_cities;i++){
        for (int j = 0; j<n_cities;j++){
            if(distances[i][j]<=100 && distances[i][j]>0){
                adjacency_matrix[i][j] = 1; // edge exists
                degree[i]++;
            }
        }
    }
    for(short i = 0; i<n_cities;i++){
        if(degree[i]==0){
            short nearest_node_and_distance[2] = {i, SHRT_MAX};// stores the nearest city (index 0) & its distance (index 1)
            for (short j = 0; j<n_cities;j++){
                if(distances[i][j] < nearest_node_and_distance[1] && i!=j){
                    nearest_node_and_distance[0] = j; // index 0 stores the node
                    nearest_node_and_distance[1] = distances[i][j]; // index 1 stores the distance
                }
            } // here we have found the nearest city
            adjacency_matrix[i][nearest_node_and_distance[0]] = 1; // add an edge to nearest city
            adjacency_matrix[nearest_node_and_distance[0]][i] = 1; // add an edge from nearest city
            degree[i]++;
            degree[nearest_node_and_distance[0]]++;
        }
    }
    /*cout<<"Adjacency matrix: "<<endl;
    for(int i = 0; i<n_cities;i++){
        for (int j = 0; j<n_cities;j++){
            cout<<adjacency_matrix[i][j]<<" ";
        }
        cout<<endl;
    }
    for(short i = 0; i<n_cities;i++) {
        if (degree[i] == 0) cout << "This node has degree 0: " << i << endl;
    }*/
}

/*void DijsktraShortestPathTree() {
    auto comparator = [](vector<int> left, vector<int> right)-> bool {return (left[1] < right[2]);}; // this is called a lambda expression that is a syntactic shortcut for a function object (https://www.walletfox.com/course/sortvectorofcustomobjects11.php)
    priority_queue <vector<int>, vector<vector<int>>, decltype(comparator)> distance_pq (comparator);
    // our priority queue is a vector of vectors with the inner vectors placed in the outer in ascending order of their dist from source.
    // Inner vectors are stored as <node index, distance from source>.
    short permanently_labelled_nodes[n_cities] = {};
    std::array<short, n_cities> temp_labelled_nodes;
    std::iota(temp_labelled_nodes.begin(),temp_labelled_nodes.end(),1); // initializing an array from 1 to n.
    distance_pq.push({0,0});
    for(int i =1; i<n_cities;i++) {
        distance_pq.push({i,INT16_MAX});
    }


}*/

void DijsktraShortestPathTree(int source) {
    short p_labels[n_cities][2] {};
    short p_label_counter =0;
    short temp_labels[n_cities][2] {};
    short increasing_distance_index[n_cities];
    short pred [128] {};
    temp_labels[0][0] = source;
    temp_labels[0][1] = 0;
    temp_labels[source][0]=0;
    temp_labels[source][1]=SHRT_MAX;
    pred[source]=-1;
    for(int i =1; i<n_cities;i++) {
        if(i!=source) {
            temp_labels[i][0] = i;
            temp_labels[i][1] = SHRT_MAX;
            increasing_distance_index[i] = i;
        }
    } // initialization
    int start =0;
    while (p_label_counter<n_cities){
        p_labels[p_label_counter][0] = temp_labels[start][0];
        p_labels[p_label_counter][1] = temp_labels[start][1];
        start++;
        for(int j=start; j<n_cities;j++){
            if(adjacency_matrix[p_labels[p_label_counter][0]][temp_labels[j][0]]>0) {
                if (temp_labels[j][1] > (p_labels[p_label_counter][1] + distances[p_labels[p_label_counter][0]][temp_labels[j][0]])) {
                    temp_labels[j][1] = p_labels[p_label_counter][1] + distances[p_labels[p_label_counter][0]][temp_labels[j][0]];
                    pred[temp_labels[j][0]] = p_labels[p_label_counter][0];
                }
            }
        } // updates distances of all nodes adjacent to the node just popped out of temp_labels
        // auto comparator = [](const array<short,2>& u, const array<short,2>& v) {return (u[1] < v[1]);}; // this is called a lambda expression that is a syntactic shortcut for a function object (https://www.walletfox.com/course/sortvectorofcustomobjects11.php)

        auto comparator = [](const void *arg1, const void *arg2)->int
        {
            int const *lhs = static_cast<int const*>(arg1); int const *rhs = static_cast<int const*>(arg2);
            return (lhs[0] < rhs[0]) ? -1
                :  ((rhs[0] < lhs[0]) ? 1
                :  (lhs[1] < rhs[1] ? -1
                :  ((rhs[1] < lhs[1] ? 1 : 0)))); //https://stackoverflow.com/questions/20931669/sort-a-2d-array-in-c-using-built-in-functionsor-any-other-method
        }; // this is called a lambda expression that is a syntactic shortcut for a function object (https://www.walletfox.com/course/sortvectorofcustomobjects11.php)
        qsort(temp_labels, n_cities, sizeof(*temp_labels), comparator);
        // sort the temp_labels array according to distance

        p_label_counter++;
    }
    //cout<<"Zeroth closest city to "<<cities[p_labels[0][0]]<<" is: "<<cities[p_labels[0][0]]<<" which is "<<p_labels[0][1]<<" miles away."<<endl;
    cout<<"First closest city to "<<cities[p_labels[0][0]]<<" is: "<<cities[p_labels[1][0]]<<" which is "<<p_labels[1][1]<<" miles away."<<endl;
    cout<<"Second closest city to "<<cities[p_labels[0][0]]<<" is: "<<cities[p_labels[2][0]]<<" which is "<<p_labels[2][1]<<" miles away."<<endl;
    cout<<"Third closest city to "<<cities[p_labels[0][0]]<<" is: "<<cities[p_labels[3][0]]<<" which is "<<p_labels[3][1]<<" miles away."<<endl;

    /*for(int i=0;i<50;i++){
       cout<<cities[p_labels[i][0]]<<" which is "<<p_labels[i][1]<<" miles away."<<endl;
    }*/

    // First closest city to Springfield, IL is: Reading, PA which is 72 miles away.
    // Second closest city to Springfield, IL is: Scranton, PA which is 93 miles away.
    // Third closest city to Springfield, IL is: Ravenna, OH which is 106 miles away.

    //for(int i=0;i<n_cities;i++) {cout<<pred[i]<<endl;}
}

void FloydWarshallShortestPaths(){
    short d[n_cities][n_cities] {};
    for(int i=0;i<n_cities;i++){
        for(int j=0;j<n_cities;j++){
            if(i==j) continue;
            if(adjacency_matrix[i][j]>0){
                d[i][j]=distances[i][j];
                continue;
            }
            d[i][j] = SHRT_MAX;
        }
    }
    for(int k=0;k<n_cities;k++){
        for(int i=0;i<n_cities;i++){
            for(int j=0;j<n_cities;j++){
                if(d[i][j] > d[i][k] + d[k][j]){
                    d[i][j] = d[i][k] + d[k][j];
                }
            }
        }
    }
    short max_dist[3] {}; //{source, destination, distance}
    for(int i=0;i<n_cities;i++){
        for(int j=0;j<n_cities;j++){
            if(max_dist[2] < d[i][j] && d[i][j]<SHRT_MAX){
                max_dist[2] = d[i][j];
                max_dist[1] = j;
                max_dist[0] = i;
            }
        }
    }
    cout<<"Longest shortest path city pair is: "<<cities[max_dist[0]]<<" & "<<cities[max_dist[1]]<<". Their distance in miles is: "<<max_dist[2]<<endl;
    //Longest shortest path city pair is: Salt Lake City, UT & Regina, SA. Their distance in miles is: 1052
}

int main() {
    get_Data();
    create_network();
    int source = -1;
    string city = "Springfield, IL"; // Springfield, IL
    for (int i =0; i<n_cities; i++){
        if (cities[i]==city){
            source = i;
            break;
        }
    }
    DijsktraShortestPathTree(source); // label setting algorithm
    FloydWarshallShortestPaths(); // label correcting algorithm
    return 0;
}
