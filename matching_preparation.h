#ifndef MATCHING_PREPARATION_H
#define MATCHING_PREPARATION_H

#include <fstream>
#include <sstream>
#include <vector>
#include <graph.h>
#include <pupil.h>
#include <student.h>

std::vector<Pupil> create_pupils_vector(char const *filename_pupils, std::vector<std::string> &subjects){

    std::ifstream file_pupils(filename_pupils);
    if (not file_pupils) {
        throw std::runtime_error("Cannot open file");
    }

    std::vector<Pupil> pupil_vector;
    std::string line;
    std::string token;
    std::getline(file_pupils, line);
    std::istringstream ss(line);
    while(getline(ss, token, ',')){
        subjects.push_back(token);
    }

    while(std::getline(file_pupils, line)){
        std::istringstream ss(line);
        std::vector<int> subjects_grade;
        int grade;

        Pupil pupil;
        std::string token;
        getline(ss, token, ',');
        pupil._id = token;
        pupil_vector.push_back(pupil);
        while (getline(ss, token, ',')){
            grade = std::stoi(token);
            subjects_grade.push_back(grade);
        }
        pupil_vector.back()._subjects = subjects_grade;
    }

    return pupil_vector;
}

std::vector<Student> create_students_vector(char const *filename_students, char const *filename_students_lb){

    std::ifstream file_students(filename_students);
    if (not file_students) {
        throw std::runtime_error("Cannot open file");
    }

    std::string line;
    std::getline(file_students, line);
    std::istringstream ss(line);

    std::vector<Student> student_vector;

    while(std::getline(file_students, line)){
        std::istringstream ss(line);
        std::vector<int> subjects_grade;
        int grade;

        Student student;
        std::string token;
        getline(ss, token, ',');
        student._id = token;
        student_vector.push_back(student);
        while (getline(ss, token, ',')) {
            grade = std::stoi(token);
            subjects_grade.push_back(grade);
        }
        student_vector.back()._subjects_upper_bound = subjects_grade;
    }

    // get the student's lower bounds for classes they wanna teach
    std::ifstream file_students_lb(filename_students_lb);
    if (not file_students_lb){
        throw std::runtime_error("Cannot open file");
    }

    std::getline(file_students_lb, line);
    std::istringstream ss2(line);
    std::vector<int> subjects_grade;
    int grade;

    for(size_t i = 0; i < student_vector.size(); i++){
        std::getline(file_students_lb, line);
        std::istringstream ss2(line);
        std::string token;
        getline(ss2, token, ',');
        while(getline(ss2, token, ',')){
            grade = std::stoi(token);
            student_vector[i]._subjects_lower_bound.push_back(grade);
        }
    }

    return student_vector;
}

Graph create_matching_graph(char const *filename_dissolved_matches, std::vector<Pupil> const &pupil_vector, std::vector<Student> const &student_vector, std::vector<bool> &pupil_available_for_matching, std::vector<bool> &student_available_for_matching) {
    //Creates the graph on which we are performing our matching algorithm. We draw an edge between a pupil and a student if they have at least
    //one subject in common and both are still available for a match (we run the matching part of the algorithm twice on different groups, hence we
    //need to pass this information to the algorithm). We add nodes s and t and draw an edge from s to every pupil and an edge from every student to t.
    int num_pupils = pupil_vector.size();
    int num_students = student_vector.size();
    int num_subjects = 0;
    if(num_pupils > 0 && num_students > 0){
        num_subjects = student_vector[0]._subjects_upper_bound.size();
    }
    else{
        throw std::runtime_error("Number of Pupils or Students is equal to 0. We can't find any matches.");
    }

    Graph g(num_pupils + num_students + 2, Graph::directed);
    for (Graph::NodeId i = 0; i < num_pupils; i ++){
        g.add_edge(num_pupils + num_students, i, 0);
    }
    for (Graph::NodeId i = num_pupils; i < num_pupils + num_students; i++) {
        g.add_edge(i, num_pupils + num_students + 1, 0);
    }

    for(int pupil = 0; pupil < num_pupils; pupil++) {
        if(pupil_available_for_matching[pupil]){
            for(int student = 0; student < num_students; student++) {
                if(student_available_for_matching[student]){
                    double num_matches = 0;
                    for (size_t k = 0; k < pupil_vector[pupil]._subjects.size(); k++){
                        if(pupil_vector[pupil]._subjects[k] > 0 && pupil_vector[pupil]._subjects[k] <= student_vector[student]._subjects_upper_bound[k] && pupil_vector[pupil]._subjects[k] >= student_vector[student]._subjects_lower_bound[k]){
                            num_matches += 1;
                        }
                    }
                    if(num_matches > 0) {
                        double weight = num_subjects - num_matches;
                        g.add_edge(pupil, num_pupils + student, weight);
                    }
                }
            }
        }
    }

    //If a match between a pupil and a student got dissolved, we don't want to match this pair again, hence in this step we
    //delete possible edges between these pairs.
    std::ifstream file_dissolved_matches(filename_dissolved_matches);
    if (not file_dissolved_matches) {
        throw std::runtime_error("Cannot open file");
    }

    std::string line;
    std::getline(file_dissolved_matches, line);
    std::istringstream ss(line);

    while(std::getline(file_dissolved_matches, line)){
        std::istringstream ss(line);
        std::string token, pupil_id, student_id;
        getline(ss, token, ',');
        while (getline(ss, token, ',')) {
            student_id = "\"" + token + "\"";
            getline(ss, token, ',');
            pupil_id = "\"" + token +"\"";
            for(size_t node = 0; node < pupil_vector.size(); node++){
                if(pupil_id == pupil_vector[node]._id){
                    for(size_t neighbor = 0; neighbor < g.get_node(node).adjacent_nodes().size(); neighbor++){
                        if(student_id == student_vector[g.get_node(node).adjacent_nodes()[neighbor].id() - pupil_vector.size()]._id){
                            g.get_node(node).adjacent_nodes().erase(g.get_node(node).adjacent_nodes().begin() + neighbor);
                            break;
                        }
                    }
                }
            }
        }
    }
    return g;
}


#endif // MATCHING_PREPARATION_H
