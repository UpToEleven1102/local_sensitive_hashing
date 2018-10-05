
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include "LSH.h"
#include "utilities.h"

double innerProduct(int dim, double *v1, double *v2) {
    double product = 0;
    printf("dim : %d \n", dim);
    for (int i = 0; i < dim; i++) {
        printf("v1: %f - v2: %f \n", v1[i], v2[i]);
        product += v1[i] * v2[i];
    }
    printf("inner product: %f\n", product);
    return product;
}

int *calculateHashValue(int dim, int m, int idx, double w, double *data, double **h) {

    DataPoint element = getElement(dim, idx, data);

    int *hashValue = (int *) malloc(m * sizeof(int));

    for (int i = 0; i < m; i++) {
        double product = innerProduct(dim, element.data, h[i]);
        hashValue[i] = (int) (product / w);
    }

    return hashValue;
}

bool equalHashValue(int m, int *o1, int *o2) {
    bool equal = true;
    for (int i = 0; i < m; ++i) {
        if (o1[i] != o2[i]) {
            equal = false;
            break;
        }
    }

    return equal;
}

int addNewCluster(double *hashValue, HashValue *iterator) {

}


int LSH(int dim, int ndata, double *data, int m, double w, double **h, int **n_cluster_ptr, int **cluster_start,
        int **cluster_size, int ***cluster_hash_val) {
    //cluster_hash_val =
    //initialize outputs:
    *n_cluster_ptr = (int *) malloc(sizeof(int));

    //local variables
    int n_cluster = 0, cluster_idx = 0;
    bool clusterFound = false;

    //Hash Bucket save hash values of each cluster
    ListHashValue *headHashVal = NULL;

    //LInked list represents hash bucket for data point
    ListDataPoint *headDataPoint = NULL;

    //pointers to hash buckets
    HashValue *iterator, *last_hash_val_ptr;
    DataPoint *last_data_point_ptr = headDataPoint;


    int *hashValue;

    for (int i = 0; i < ndata; ++i) {
        iterator = headHashVal;
        last_hash_val_ptr = iterator;
        hashValue = calculateHashValue(dim, m, i, w, data, h);
        clusterFound = false;
        cluster_idx = 0;

        DataPoint dataPoint = getElement(dim, i, data);

        if (headHashVal != NULL) {
            while (iterator != NULL) {
                if (equalHashValue(m, iterator->value, hashValue) == true) {
                    printf("Debug : found an existing cluster\n");

                    clusterFound = true;
                    break;
                }
                if (iterator->next == NULL) {
                    last_hash_val_ptr = iterator;
                }
                iterator = iterator->next;
                cluster_idx+=1;
            }

            if (clusterFound == false) {
                HashValue *tempValue = (HashValue *) malloc(sizeof(HashValue));
                tempValue->next = NULL;
                tempValue->value = hashValue;
                last_hash_val_ptr->next = tempValue;
                n_cluster++;
            }

            //add datapoint into linked list
            last_data_point_ptr->next = (DataPoint*) malloc(sizeof(DataPoint));
            last_data_point_ptr = last_data_point_ptr->next;
            last_data_point_ptr->cluster_number = cluster_idx;
            last_data_point_ptr->next = NULL;
            last_data_point_ptr->data = dataPoint.data;
        } else {
            n_cluster = 1;
            headHashVal = (HashValue *) malloc(sizeof(HashValue));
            headHashVal->value = hashValue;
            headHashVal->next = NULL;

            headDataPoint = (DataPoint*) malloc(sizeof(DataPoint));
            headDataPoint->next = NULL;
            headDataPoint->cluster_number = cluster_idx;
            headDataPoint->data = dataPoint.data;
            last_data_point_ptr = headDataPoint;
        }
    }

    *n_cluster_ptr = &n_cluster;

//    HashValue *iterator2 = headHashVal;
//
//    while(iterator2 != NULL) {
//        printf("debug----------------------\n");
//        for (int j = 0; j < m; ++j) {
//            printf("%d\n", iterator2->value[j]);
//        }
//        iterator2 = iterator2->next;
//    }

    DataPoint *ite = headDataPoint;
    int counter = 0;
    printf("data points linked list: \n");

    while(ite !=NULL) {
        printf("%d ---- cluster number: %d\n", counter, ite->cluster_number);
        for(int i = 0; i< dim; i++) {
            printf("%f \n", ite->data[i]);
        }
        ite= ite->next;
    }


    printf("number of cluster: %d\n", **n_cluster_ptr );


//    int *hashValue = calculateHashValue(dim, m, 0, w, data, h);
//    printf("debug - hash value:\n");
//    for (int i = 0; i < m; i++) {
//        printf("%d\n", hashValue[i]);
//    }
}
