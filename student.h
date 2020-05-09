#ifndef STUDENT_H
#define STUDENT_H

#include <vector>
#include <string>

class Student{
public:
    std::string _id;
    std::vector<int> _subjects_upper_bound;
    std::vector<int> _subjects_lower_bound;
};

#endif // STUDENT_H
