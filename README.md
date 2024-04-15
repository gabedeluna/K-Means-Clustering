# K-Means-Clustering

This program implements the K-means clustering algorithm, a popular method used in data mining and machine learning to partition n observations into k clusters in which each observation belongs to the cluster with the nearest mean. The program is designed to read points from a file, perform clustering, and then output the results with cluster assignments for each point.

Features
Reads input data points from a specified file.
Implements the K-means algorithm to cluster the points.
Outputs the clustered points and their corresponding cluster number to a file.
Utilizes a linked-list structure for managing points and clusters dynamically.
Requirements
C++11 compatible compiler
Standard Template Library (STL)
Usage
The program requires two command line arguments:

k: Number of desired clusters.
filename: Path to the input file containing the data points.
Command Line Syntax
./kmeans <k> <filename>
Input File Format
The input file should contain one data point per line, with x and y coordinates separated by a space. For example:

34 22
15 31
54 12
...

Output File
The output will be written to a file named output.txt in the same directory as the executable. It contains the x and y coordinates of each point followed by the cluster number to which it has been assigned. For example:
34 22 1
15 31 2
54 12 1
...
Building the Program
You can compile the program using any C++ compiler like g++. Here is an example of how to compile it:
g++ -o kmeans kmeans.cpp -std=c++11
Internal Structures
point: Represents a data point with x, y coordinates, and cluster assignment.
cluster: Represents a cluster, storing the centroid coordinates and a linked list of points belonging to this cluster.
Global linked lists are used for storing all points and clusters.
Functions
void findCentroid(int k, int pointCount): Calculates or updates centroids of clusters.
bool findMeans(int k, int pointCount): Assigns each point to the nearest centroid and updates clusters.
Notes
Ensure the input file is properly formatted and accessible at the specified location.
The number of clusters k should be appropriate for the dataset and not exceed the number of data points.
