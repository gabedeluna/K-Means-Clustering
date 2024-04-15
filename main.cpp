#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <string>

using namespace std;

struct point {
    int x;
    int y;
    int cluster;
    struct point* next;//pointer to next point
    struct point* nextData;//pointer to next point in cluster list
};

struct cluster {
    int clusterNum;
    float centroidX;
    float centroidY;
    struct cluster* next;//list of clusters
    struct point* dataHead;//head of list of points under each cluster

};

void findCentroid(int k, int pointCount);
bool findMeans(int k, int pointCount);

//Global Variables////
struct point* head;
struct point* iter;
struct point* clusterPointIter;
struct cluster* cHead;
struct cluster* cIter;
bool initial = true;

int main(int argc, char* argv[]) {
    ifstream inFile;
    ofstream outFile ("output.txt");
    srand(time(0));
    int x;
    int y;
    point *p;
    cluster *c;
    int pointCount = 0;
    int k = atoi(argv[1]);
    bool updateCluster = true;
    string fileName = argv[2];


    //creating cluster nodes based on input k
    for (int i = 0; i < k; i++) {
        c = new cluster();
        c->clusterNum = i+1;

        cIter = cHead;//set cluster iterator to cluster linked list head
        if (cIter == NULL) {
            c->next = cIter;
            cHead = c;
        }
        else {
            while (cIter->next != NULL) {
                cIter = cIter->next;
            }
            cIter->next = c;
        }
    }

    inFile.open(fileName);
    if (!inFile) {
        cout << "Unable to open file";
        exit(1);
    }

    while (inFile >> x) {
        p = new point();
        cout << x << " ";
        p->x = x;
        inFile >> y;
        cout << y << endl;
        p->y = y;

        iter = head;
        if (iter == NULL) {
            p->next = iter;
            head = p;
        }
        else {
            while (iter->next != NULL) {
                iter = iter->next;
            }
            iter->next = p;
        }
        pointCount++;
    }
    //debug
    iter = head;
    cout << "\n" << endl;
    while (iter != NULL) {
        cout << iter->x << " " << iter->y << endl;
        iter = iter->next;
    }
    while (updateCluster) {
        findCentroid(k, pointCount);
        updateCluster = findMeans(k, pointCount);
    }

    if (outFile.is_open()) {
        iter = head;
        while (iter != NULL) {
            outFile << iter->x << "  " << iter->y << "  " << iter->cluster << endl;
            iter = iter->next;
        }
    }
    else {
        cout << "Unable to open output file" << endl;
    }





    inFile.close();
    return 0;
}


void findCentroid(int k, int pointCount) {
    float xTotal = 0;
    float yTotal = 0;
    int numPoints = 0;

    if (initial) {//First initialization of centroids.

        //The following code produces non-repeating random order in which to
        //pick several points based on k to be the initial centroids
        int *values = new int[pointCount];
        for (int i = 0; i < pointCount; i++) {
            values[i] = i;
        }
        for (int i = 0; i < (pointCount * 20); i++) {
            int randvalue1 = rand() % pointCount;
            int randvalue2 = rand() % pointCount;
            int temp = values[randvalue1];
            values[randvalue1] = values[randvalue2];
            values[randvalue2] = temp;
        }

        //Sets each cluster centroid to a random point coordinate
        iter = head;
        cIter = cHead;
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < values[i]; j++) {
                iter = iter->next;
            }
            cIter->centroidX = iter->x;
            cIter->centroidY = iter->y;
            cIter = cIter->next;
            iter = head;
        }
        cIter = cHead;
        cout << "\n" << endl;
        cout << "centriod values" << endl;
        while (cIter != NULL) {
            cout << cIter->centroidX << " " << cIter->centroidY << endl;
            cIter = cIter->next;
        }
        initial = false;
    }
    else {//After initial update, centroids will be updated through this branch of code
        cIter = cHead;

        while (cIter != NULL) {
            clusterPointIter = cIter->dataHead;//iterator to coordinate list under cluster
            xTotal = 0;//total counters
            yTotal = 0;

            while (clusterPointIter != NULL) {
                xTotal += clusterPointIter->x;
                yTotal += clusterPointIter->y;
                numPoints++;//gets total number of coordinates to calculate cluster means
                clusterPointIter = clusterPointIter->nextData;
            }
            cIter->centroidX = (xTotal / numPoints);
            cIter->centroidY = (yTotal / numPoints);
            cIter = cIter->next;
            numPoints = 0;
        }
        cIter = cHead;//debug code
        cout << "\n" << endl;
        cout << "centriod values" << endl;
        while (cIter != NULL) {
            cout << cIter->centroidX << " " << cIter->centroidY << endl;
            cIter = cIter->next;
        }
    }

}

bool findMeans(int k, int pointCount) {
    float *arr = new float(k);//array to hold distance mean from each point to each centroid
    float x;//variable to hold value from subtracting x point from centroid point
    float y;//variable to hold value from subtraction y point from centroid point
    bool updateCluster = true;//false when no update to clusters
    int matches = 0;//counter increments when cluster is not updated

    //clear old coordinate data from previous means calculation
    cIter = cHead;
    while (cIter != NULL) {
        cIter->dataHead = NULL;
        cIter = cIter->next;
    }
    //clearing previous means calculation from each point
    iter = head;
    while (iter != NULL) {
        iter->nextData = NULL;
        iter = iter->next;
    }

    //reset iterators
    iter = head;
    cIter = cHead;
    while (iter != NULL) {
        cIter = cHead;
        for (int i = 0; i < k; i++) {
            x = abs(iter->x - cIter->centroidX);
            y = abs(iter->y - cIter->centroidY);
            arr[i] = (x + y);
            cout << "debug " << arr[i] << endl;
            cIter = cIter->next;
        }
        float *min = min_element(arr, arr + k );//find minimum distance to cluster
        size_t index = distance(arr, min);//find which cluster is closest
        if (iter->cluster == (index + 1)) {
            matches++;
        }
        else {
            iter->cluster = index + 1;//update cluster number for specific point
        }
        cout << "Matches are " << matches << endl;
        //implement counter to tick when cluster is the same. if counter equals pointCounter
        //then raise flag updateCluster to false
        cout << index << " is the lowest index and the value is" << *min << endl;

        cIter = cHead;
        while (index != 0) {
            cIter = cIter->next;
            index--;
        }
        if (cIter->dataHead == NULL) {
            cIter->dataHead = iter;
        }
        else {
            clusterPointIter = cIter->dataHead;

            while (clusterPointIter->nextData != NULL) {
                clusterPointIter = clusterPointIter->nextData;
            }
            clusterPointIter->nextData = iter;
        }



        iter = iter->next;
    }
    if (matches == pointCount) {
        updateCluster = false;
    }
    cout << "DEBUG: Showing points in clusters." << endl;

    iter = head;
    cout << "\n" << endl;
    while (iter != NULL) {
        cout << iter->x << " " << iter->y << endl;
        cout << "Cluster: " << iter->cluster << endl;
        iter = iter->next;
    }

    cout << "Cluster 1" << endl;
    cIter = cHead;
    cout << "\n" << endl;
    while (cIter != NULL) {
        clusterPointIter = cIter->dataHead;
        cout << "Points under cluster" << endl;
        while (clusterPointIter != NULL) {
            cout << clusterPointIter->x << " " << clusterPointIter->y << endl;
            clusterPointIter = clusterPointIter->nextData;
        }
        cIter = cIter->next;
        cout << "Next cluster" << endl;
    }
    return updateCluster;//return false if no more cluster groupings are updated

}
