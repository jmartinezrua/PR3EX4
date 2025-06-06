#include <assert.h>
#include <string.h>
#include "test_data.h"
#include "test.h"
#include "test_pr1.h"
#include "test_pr2.h"
#include "test_pr3.h"

// Write data to file
void save_data(const char* filename, const char* data) {
    FILE *fout;
    fout = fopen(filename, "w");
    assert(fout != NULL);
    fwrite(data, strlen(data), 1, fout);
    fclose(fout);
}


// Run all available tests
void testSuite_run(tTestSuite* test_suite, const char* input, const char* readme) {    
    const char* default_readme = "../README.txt";    
    const char* filename;
    
    assert(test_suite != NULL);
    
    // Load the README.txt file
    if (readme == NULL) {
        testSuite_load_learner(test_suite, default_readme);
    } else {
        testSuite_load_learner(test_suite, readme);
    }
    
    //////////////////////
    // Run tests for PR1
    //////////////////////
    
    // If no file is provided, use default data for PR1
    if (input == NULL) {
        filename = "test_data_pr1.csv";
        save_data(filename, test_data_pr1_str);        
    } else {
        filename = input;
    }
    
    // Run tests
    run_pr1(test_suite, filename);
    
    //////////////////////
    // Run tests for PR2
    //////////////////////
    
    // If no file is provided, use default data for PR2
    if (input == NULL) {
        filename = "test_data_pr2.csv";
        save_data(filename, test_data_pr2_str);        
    } else {
        filename = input;
    }    
    
    run_pr2(test_suite, filename);
    
    //////////////////////
    // Run tests for PR3
    //////////////////////
    
    // If no file is provided, use default data for PR3
    if (input == NULL) {
        filename = "test_data_pr3.csv";
        save_data(filename, test_data_pr3_str);        
    } else {
        filename = input;
    }    
    
    run_pr3(test_suite, filename);
}
