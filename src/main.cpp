#include "graph.h"
#include "dijkstra.h"
#include "pupil.h"
#include "student.h"
#include "matching_preparation.h"
#include <iomanip>
#include <math.h>
#include <numeric>

void perform_matching_algorithm(Graph &M, std::vector<Pupil> &pupil_vector, std::vector<Student> &student_vector, std::vector<bool> &pupil_available_for_matching, std::vector<bool> &student_available_for_matching, char const* filename_dissolved_matches){
    Graph g = create_matching_graph(filename_dissolved_matches, pupil_vector, student_vector, pupil_available_for_matching, student_available_for_matching);
    int num_pupils = pupil_vector.size();
    int num_students = student_vector.size();

    Graph::NodeId start_nodeid = num_pupils + num_students;
    Graph::NodeId end_nodeid = num_pupils + num_students + 1;
    std::vector<Graph::NodeId> shortest_path = dijkstra(g, start_nodeid, end_nodeid);

    while (shortest_path.size() > 0) {
        for(size_t i = 0; i < shortest_path.size() - 1; i++){
            g.delete_edge(shortest_path[i], shortest_path[i+1]);    //to call the delete_edge function we need that the graph is simple (no edges join same vertices)
        }
        for(int nodeid = 0; nodeid < g.num_nodes(); nodeid++) {
            for(size_t i = 0; i < g.get_node(nodeid).adjacent_nodes().size(); i++){
                double old_edge_weight = g.get_node(nodeid).adjacent_nodes()[i].get_edge_weight();
                double potential_tail = g.get_node(nodeid).get_potential();
                double potential_head = g.get_node(g.get_node(nodeid).adjacent_nodes()[i].id()).get_potential();
                g.get_node(nodeid).adjacent_nodes()[i].set_edge_weight(old_edge_weight + potential_tail - potential_head);
            }
        }
        for(size_t i = 1; i < shortest_path.size() - 2; i++){
            g.add_edge(shortest_path[i+1], shortest_path[i], 0);
        }
        shortest_path = dijkstra(g, start_nodeid, end_nodeid);
    }

    //We save the computed Matches in M. The graph M is undirected. The graph g will be changed in the second run of this function,
    //hence we save the information about the found matches in the graph M. If at the end of the algorithm there is edge in
    //g directed from a student to a pupil. Then by the way how the algorithm works, the student is matched with that pupil.
    for(Graph::NodeId i = num_pupils; i < num_pupils + num_students; i++) {
        if(g.get_node(i).adjacent_nodes().size() >= 1){
            if(g.get_node(i).adjacent_nodes()[0].id() < num_pupils + num_students){ //the graph g might still contain edges between the students and the sink node. Obviously this doesn't count as a match.
                M.add_edge(i, g.get_node(i).adjacent_nodes()[0].id());
                student_available_for_matching[i - num_pupils] = false;
                pupil_available_for_matching[g.get_node(i).adjacent_nodes()[0].id()] = false;
            }
        }
    }
}

void evaluate_matching(Graph &M, std::vector<Pupil> &pupil_vector, std::vector<Student> &student_vector, std::vector<std::string> &subjects){
    int num_pupils = pupil_vector.size();
    int num_students = student_vector.size();
    double share_of_pupils_perfectly_matched;
    std::vector<int> subjects_not_matched(pupil_vector[0]._subjects.size(), 0); //represents the number how often subject i wasn't for a pupil
    std::vector<int> num_pupils_with_i_subjects_unmatched(pupil_vector[0]._subjects.size(), 0); //represents the number of pupils with i unmatched subjects (at the i-th entry of the vector)
    std::vector<int> num_pupils_percentage_quantiles(6, 0);    //represents the num pupils of which the subjects are matched by 0%-19%, by 20%-39%, ..., by 80%-99.9%, by 100%
    std::vector<double> average_subjects_if_nonperfect(pupil_vector[0]._subjects.size(), 0);   //will represent the average number of subjects a pupil requested if the matching wasn't perfect

    for (auto nodeid = num_pupils; nodeid < num_pupils + num_students; ++nodeid) {  //we iterate over the students since the students vector is usually smaller than the pupils vector
        int num_subjects_matched = 0, num_subjects_unmatched = 0;   //counts for a single pupil how many subjects are matched and how many aren't.
        for (auto neighbor: M.get_node(nodeid).adjacent_nodes()) {  //the node has only one neighbor
            for(size_t subject = 0; subject < pupil_vector[0]._subjects.size(); subject++){
                if(pupil_vector[neighbor.id()]._subjects[subject] > 0){
                    if(student_vector[nodeid - num_pupils]._subjects_upper_bound[subject] >= pupil_vector[neighbor.id()]._subjects[subject] && student_vector[nodeid - num_pupils]._subjects_lower_bound[subject] <= pupil_vector[neighbor.id()]._subjects[subject]){
                        num_subjects_matched +=1;
                    }
                    else{
                        num_subjects_unmatched += 1;
                        subjects_not_matched[subject] += 1;
                    }
                }
            }
        }
        if(M.get_node(nodeid).adjacent_nodes().size() > 0){
            num_pupils_with_i_subjects_unmatched[num_subjects_unmatched] += 1;
            num_pupils_percentage_quantiles[(size_t) (5.0*num_subjects_matched/(num_subjects_matched + num_subjects_unmatched))] += 1;
            average_subjects_if_nonperfect[num_subjects_unmatched] += num_subjects_matched + num_subjects_unmatched;
        }
    }

    for(size_t i = 0; i < average_subjects_if_nonperfect.size(); i++){
        if(num_pupils_with_i_subjects_unmatched[i] != 0){
            average_subjects_if_nonperfect[i] = average_subjects_if_nonperfect[i]/num_pupils_with_i_subjects_unmatched[i];
        }
    }
    share_of_pupils_perfectly_matched = 1.0*num_pupils_with_i_subjects_unmatched[0]/std::accumulate(num_pupils_with_i_subjects_unmatched.begin(), num_pupils_with_i_subjects_unmatched.end(), 0);

// ////////////////////////////////////////////////////////////////////////////////
    //We print the evaluation of the matching on the console
    int TAB = 6;
    std::cout << "How often have how many requested subjects of a student not been matched?\n";
    std::cout << "num subjects not matched:" << std::setw(6) << "0";
    for(size_t i = 1; i < num_pupils_with_i_subjects_unmatched.size(); i++){
        std::cout << std::setw(TAB) << i;
    }
    std::cout << "\n";
    std::cout << "num pupils:" << std::setw(20) << num_pupils_with_i_subjects_unmatched[0];
    for(size_t i = 1; i < num_pupils_with_i_subjects_unmatched.size(); i++){
        std::cout << std::setw(TAB) << num_pupils_with_i_subjects_unmatched[i];
    }
    std::cout << "\n";

    std::cout << "on avg requested subjects:" << std::setw(5) << truncf(average_subjects_if_nonperfect[0]*10)/10;
    for(size_t i = 1; i < num_pupils_with_i_subjects_unmatched.size(); i++){
        std::cout << std::setw(TAB) << truncf(average_subjects_if_nonperfect[i]*10)/10;
    }
    std::cout << "\n \n";

    TAB = 10;
    std::cout << "How many percent of the subjects a pupil requested have been matched?\n";
    std::cout << "percentage:" << std::setw(TAB) << "100%" << std::setw(TAB) << "99%-80%" << std::setw(TAB) << "79%-60%" << std::setw(TAB) << "59%-40%" << std::setw(TAB) << "39%-20%" << std::setw(TAB) << "19%-0%\n";
    std::cout << "num pupils:" << std::setw(TAB) << num_pupils_percentage_quantiles.back();
    for(int i = (int) num_pupils_percentage_quantiles.size() - 2; i >=0 ; i--){
        std::cout << std::setw(TAB) << num_pupils_percentage_quantiles[i];
    }
    std::cout << "\n \n";

    TAB = 4;
    std::cout << "Which subjects requested by a pupil couldn't be matched?\n";
    std::cout << subjects[0] << "(" << subjects_not_matched[0] << ") ";
    for(size_t i = 1; i < subjects.size(); i++){
        std::cout << std::setw(TAB) << subjects[i] << "(" << subjects_not_matched[i] << ") ";
    }
    std::cout << "\n \n";

    std::cout << "Number of Matches: " << std::accumulate(num_pupils_with_i_subjects_unmatched.begin(), num_pupils_with_i_subjects_unmatched.end(), 0) << "\n";
    std::cout << "Share of pupils whose requested subjects have all been matched: " << truncf(share_of_pupils_perfectly_matched*100) << "%\n \n";

}

int main(int argc, char *argv[]){
    if(argc != 6){
        throw std::runtime_error("The algorithm needs 5 input files. A file with the informaiont about the pupils, two files for the students, one file about the dissolved matches and the location for the output file."
                                 "For further information see the readme file.");
    }
    char const* filename_pupils = argv[1];
    char const* filename_students = argv[2];
    char const* filename_students_lb = argv[3];
    char const* filename_dissolved_matches = argv[4];
    char const* filename_output = argv[5];

    std::vector<std::string> subjects;

    std::vector<Pupil> pupil_vector = create_pupils_vector(filename_pupils, subjects);
    std::vector<Student> student_vector = create_students_vector(filename_students, filename_students_lb);
    int num_pupils = pupil_vector.size(), num_students = student_vector.size();

    //We split the matching operation in two steps. In the first step we only want to match pupils which have highest priority, that means pupils who are
    //positioned first in the input file. We take the first 0.9*num_students pupils and match them to all students. In the second step we take all pupils
    //and all students who haven't been matched in the first step and start the matching algorithm once again for these people.
    std::vector<bool> pupil_available_for_matching(pupil_vector.size(), false);
    std::vector<bool> student_available_for_matching(student_vector.size(), true);
    for(size_t i = 0 ; i < std::min(pupil_vector.size(), (size_t) (0.9*student_vector.size())); i++){
        pupil_available_for_matching[i] = true;
    }
    Graph M(num_pupils + num_students + 2, Graph::undirected);

    perform_matching_algorithm(M, pupil_vector, student_vector, pupil_available_for_matching, student_available_for_matching, filename_dissolved_matches);

    for(size_t i = std::min(pupil_vector.size(), (size_t) (0.9*student_vector.size())); i < pupil_vector.size(); i++){
        pupil_available_for_matching[i] = true;
    }

    perform_matching_algorithm(M, pupil_vector, student_vector, pupil_available_for_matching, student_available_for_matching, filename_dissolved_matches);

    evaluate_matching(M, pupil_vector, student_vector, subjects);

    M.print_matching(pupil_vector, student_vector, filename_output);

}
