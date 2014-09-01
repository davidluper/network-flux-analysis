#include "RuntimeContext.h"
#include "DecompositionPlanExecutor.h"
#include "Util.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <stdlib.h>

using namespace std;

void print_time() {
    time_t now = time(0);
    char* dt = ctime(&now);

    cout << dt;
}

void output_help() {

}

void parse_command_line_arguments(int argc, char** args, nfa::RuntimeContext& context) {
    for (int i = 0; i < argc; ++i) {
        if (strcmp(args[i], "--help") == 0) {
            cout << "help" << endl;
            exit(0);
        }
        else if (strcmp(args[i], "--verbose") == 0) {
            context.set_verbose_mode(true);
        }
        else if (strcmp(args[i], "--path-factory") == 0) {
            char* path_factory = args[++i];
            if (strcmp(path_factory, "econet") == 0) {
                context.set_path_factory(nfa::RuntimeContext::ECONET);
            }
        }
        else if (strcmp(args[i], "--directory") == 0) {
            string directory(args[++i]);
            directory += "/";

            if (!nfa::Util::directory_exists(directory)) {
                cout << "specified directory doesn't exist" << endl;
                exit(0);
            }

            context.set_directory(directory);
        }
        else if (strcmp(args[i], "--paths-file") == 0) {
            string paths_file_name(args[++i]);
            context.set_paths_file_name(paths_file_name);
        }
        else if (strcmp(args[i], "--import-adjacency-matrix") == 0) {
            context.set_do_build_adjacency_matrix(false);
        }
        else if (strcmp(args[i], "--adjacency-matrix-file") == 0) {
            string adjacency_matrix_file(args[++i]);
            context.set_adjacency_matrix_file(adjacency_matrix_file);
        }
    }
}

int main(int argc, char** args) {
    nfa::RuntimeContext context(nfa::RuntimeContext::ECONET, string("./"), string("paths.dat"));
    
    parse_command_line_arguments(argc, args, context);

    cout << "START ";  
    print_time();
    
    nfa::DecompositionPlanExecutor plan(context);
    plan.execute_plan();

    cout << "STOP "; 
    print_time();
    
    return 1;
}
