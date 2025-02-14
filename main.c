#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Org_struct {
    char name[20];
    int* prey; //dynamic array of indices  
    int numPrey;
} Org;


void buildWeb(Org* web, int numOrg, int predInd, int preyInd) {
    //TODO: build the web by adding the predator-prey relation to the food web.
    //      Inputs: 
    //          web - a dynamically allocated array of Orgs 
    //          numOrgs - number of organisms = size of web[]
    //          predInd - predator index in web[]; an entry to its prey[] subitem will be added
    //          preyInd - prey index in web[]; will be added to predator's prey[] subitem
    //      Outputs:
    //          web - array is updated and implicitly returned (previously allocated on heap)
    //        
    //      For the predator-prey relation...
    //      (1) if the predator's prey[] array is empty, allocate memory for one index
    //          otherwise, reallocate predator's prey[] array to allow one more index
    //      (2) append the prey index as the last element of the predator's prey[] array 
    //      (3) update the numPrey subitem for the predator appropriately 
    if (predInd < 0 || predInd >= numOrg || preyInd < 0 || preyInd >= numOrg) {
        printf("Invalid indices. Predator and prey indices must be within [0, %d).\n", numOrg);
        return;
    }

    // Predator's prey array is empty, allocate memory for one index
    if (web[predInd].prey == NULL) {
        web[predInd].prey = (int*)malloc(sizeof(int));
        web[predInd].prey[0] = preyInd;
        web[predInd].numPrey = 1;
    } else { // Predator's prey array is not empty, reallocate to add one more index
        web[predInd].prey = (int*)realloc(web[predInd].prey, (web[predInd].numPrey + 1) * sizeof(int));
        web[predInd].prey[web[predInd].numPrey] = preyInd;
        web[predInd].numPrey++;
    }
}


void extinction(Org** web, int* numOrgs, int index) {
    //TODO: remove the organism associated with [index] from web.
    //      Inputs: 
    //          web - a dynamically allocated array of Orgs 
    //          numOrgs - number of organisms = size of web[]
    //          index - organism index in web[] to remove
    //      Outputs:
    //          web - pointer passed-by-pointer; memory address of web array changes due to reallocation
    //          numOrgs - passed-by-pointer; must be decremented since web[] loses an organism
    //
    //      Remember to do the following:
    //      1. remove organism at index from web[] - DO NOT use realloc(), instead...
    //          (a) free any malloc'd memory associated with organism at index; i.e. its prey[] subitem
    //          (b) malloc new space for the array with the new number of Orgs
    //          (c) copy all but one of the old array elements to the new array, 
    //              some require shifting forward to overwrite the organism at index
    //          (d) free the old array
    //          (e) update the array pointer to the new array
    //          (f) update numOrgs
    //      2. remove index from all organsisms' prey[] array subitems - DO NOT use realloc(), instead...
    //          (a) search for index in all organisms' prey[] arrays; when index is found:
    //                [i] malloc new space for the array with the new number of ints
    //               [ii] copy all but one of the old array elements to the new array, 
    //                    keeping the same order some require shifting forward
    //              [iii] free the old array
    //               [iv] update the array pointer to the new array
    //                [v] update the numPrey subitem accordingly
    //          (b) update all organisms' prey[] elements that are greater than index, 
    //              which have been shifted forward in the web array
    //
    //          Edge case: check the size array being malloc'ed; 
    //                     for a web with only one organism and 
    //                     that orgranism goes extinct, 
    //                     instead of malloc'ing an empty array, 
    //                     explicitly set the pointer to NULL;
    //                     see the web[] allocation in main() as an example
    if (index < 0 || index >= *numOrgs) {
        printf("Invalid index for extinction. Index must be within [0, %d).\n", *numOrgs);
        return;
    }

    // Free any malloc'd memory associated with the organism at index
    free((*web)[index].prey);

    // Remove organism at index from web[]
    for (int i = index; i < (*numOrgs) - 1; i++) {
        (*web)[i] = (*web)[i + 1];
    }

    // Decrement numOrgs
    (*numOrgs)--;

    // Remove index from all organisms' prey arrays and update prey elements greater than index
    for (int i = 0; i < *numOrgs; i++) {
        int* updatedPrey = NULL;
        int updatedNumPrey = 0;
        for (int j = 0; j < (*web)[i].numPrey; j++) {
            if ((*web)[i].prey[j] == index) {
                continue; // Skip prey index to be removed
            }
            if ((*web)[i].prey[j] > index) {
                (*web)[i].prey[j]--; // Update prey index
            }
            // Copy other prey indices to updatedPrey array
            updatedPrey = (int*)realloc(updatedPrey, (updatedNumPrey + 1) * sizeof(int));
            updatedPrey[updatedNumPrey] = (*web)[i].prey[j];
            updatedNumPrey++;
        }
        free((*web)[i].prey);
        (*web)[i].prey = updatedPrey;
        (*web)[i].numPrey = updatedNumPrey;
    }

}

// Helper function to print an organism's prey indices
void printPrey(Org* web, int index) {
    printf("%s preys on: ", web[index].name);
    for (int i = 0; i < web[index].numPrey; i++) {
        printf("%s ", web[web[index].prey[i]].name);
    }
    printf("\n");
}

int main(void) {

    int numOrgs;
    printf("Welcome to the Food Web Application\n");
    printf("--------------------------------\n");
    printf("Enter number of organisms:\n");
    scanf("%d",&numOrgs);

    Org* web = NULL;
    if(numOrgs > 0) { //Do not malloc an empty array, leave it pointing to NULL
        web = (Org*)malloc(numOrgs*sizeof(Org));
    }
    
    printf("Enter names for %d organisms:\n", numOrgs);
    for (int i = 0; i < numOrgs; ++i) {
        scanf("%s",web[i].name);
        web[i].prey = NULL;
        web[i].numPrey = 0;
    }

    printf("Enter number of predator/prey relations:\n");
    int numRels;
    scanf("%d",&numRels);

    printf("Enter the pair of indices for the %d predator/prey relations\n",numRels);
    printf("the format is [predator index] [prey index]:\n");
    
    int predInd, preyInd;
    for (int i = 0; i < numRels; ++i) {
        scanf("%d %d",&predInd, &preyInd);
        buildWeb(web,numOrgs,predInd,preyInd);
    }
    printf("--------------------------------\n\n");

    printf("Food Web Predators & Prey:\n");
    //TODO: print the Food Web Organisms with what they eat (i.e. prey)
    for (int i = 0; i < numOrgs; i++) {
        printPrey(web, i);
    }

    printf("\n");

    printf("Apex Predators:\n");
    //TODO: identify and print the organisms not eaten by any others
    printf("\n");

    printf("Producers:\n");
    //TODO: identify and print the organisms that eat no other organisms
    printf("\n");

    printf("Most Flexible Eaters:\n");
    //TODO: identity and print the organism(s) with the most prey
    printf("\n");

    printf("Tastiest Food:\n");
    //TODO: identity and print organism(s) eaten by the most other organisms
    printf("\n");

    printf("Food Web Heights:\n");
    //TODO: calculate and print the length of the longest chain from a 
    //      producer to each organism
    printf("\n");

    printf("Vore Types:\n");
    //TODO: classify all organisms and print each group
    //      (producers, herbivores, omnivores, & carnivores)
    printf("\n");

    printf("--------------------------------\n");
    int extInd;
    printf("Enter extinct species index:\n");
    scanf("%d",&extInd);
    printf("Species Extinction: %s\n", web[extInd].name);
    extinction(&web,&numOrgs,extInd);
    printf("--------------------------------\n\n");


    printf("UPDATED Food Web Predators & Prey:\n");
    //TODO: print the UPDATED Food Web Organisms with what they eat (i.e. prey), AFTER THE EXTINCTION
    printf("\n");

    printf("UPDATED Apex Predators:\n");
    //TODO: AFTER THE EXTINCTION, identify and print the organisms not eaten by any other
    printf("\n");

    printf("UPDATED Producers:\n");
    //TODO: AFTER THE EXTINCTION, identify and print the organisms that eat no other organisms
    printf("\n");

    printf("UPDATED Most Flexible Eaters:\n");
    //TODO: AFTER THE EXTINCTION, identity and print the organism(s) with the most prey
    printf("\n");

    printf("UPDATED Tastiest Food:\n");
    //TODO: AFTER THE EXTINCTION, identity and print organism(s) eaten by the most other organisms
    printf("\n");

    printf("UPDATED Food Web Heights:\n");
    //TODO: AFTER THE EXTINCTION, calculate and print the length of the longest chain from a 
    //      producer to each organism
    printf("\n");

    printf("UPDATED Vore Types:\n");
    //TODO: AFTER THE EXTINCTION, classify all organisms and print each group
    //      (producers, herbivores, omnivores, & carnivores)
    printf("\n");
    printf("--------------------------------\n");

    //TODO: make sure to free all malloc'd memory to prevent potential leaks
    for (int i = 0; i < numOrgs; i++) {
        free(web[i].prey);
    }
    free(web);

    return 0;
}
