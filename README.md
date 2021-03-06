# Matching Algorithm

The algorithm we use for the Matching Process is based on a graph theoretical approach. It's basically the Edmonds-Karp Algorithm for a network flow applied to the bipartite matching problem. The algorithm finds a weight maximum weight matching with maximum cardinality. This means the first goal of the algorithm is to construct as many matches in the graph as possible and secondly to find a matching with maximum weight among all these matchings with maximum cardinality.

The only difference between our matching algorithm and the usual one given by the Edmonds-Karp Algorithm is that when the Edmonds-Karp Algorithm is looking for a shortest path from s to t, it considers a shortest path given by the number of edges while our algorithm is considering the shortest path given by the edge weights.

In our matching algorithm we prioritize the pupils which are given on top of the list, in that way we're telling the algorithm which pupils to definitely include in a matching (an order of the students could for example be given by the time they're waiting for being matched). The algorithm takes the first (0.9\*num_students) pupils and performs the matching algorithm with them and all students. 10% of the students will be left over at the end of the algorithm. These 10% we're gonna match with all the other pupils who haven't participate in the first matching process. We do this to still guarantee good matching quality while matching the maximum amount of available students. If, for example, we would take the first num_students pupils (instead of 0.9\*num_students) then we would risk that the last matches have very poor quality in that way that pupil and student have very few matches in their subjects.

For the implementation we used a graph class with a given implementation of the Dijkstra Algorithm which we used in our Freshmen year at university. We added some data structures and some functions but overall the implementation is easily to understand.
 
We have four csv input files. The first one contains the information for the pupils, the second and third one information about the student, and the fourth one contains the dissolved matches. We've provided sample files how each input file needs to be structured. The pupils and the two students files have all listed the subjects in the first line. After that each line represents one person. The file structures are basically the same. First in each line we have the pupils or the students id. Then comes a sequence of numbers of the length of the  number of subjects representing the pupils class and whether they need help in one subject or not. Is the i-th number equal to 0 they don't need help for the i-th subject. If they do need help the entry will be equal to the pupil's class. For a student it is very similar. A 0 represents that they don't want to or are not able to offer help for that subject. A number greater than 0 represents that they are willing to help a pupil with that subject and the number shows the highest class for that subject which they would be willing or able to offer help. The other students file is structured the same way but there a number greater than 0 represents the lowest class they would be willing to help. The fourth file shows the dissolved matches. We don't want to connect those people again. The first entry of a line gives the pupils id and the second entry gives the students id of the people which shouldn't be matched once again.


# How to run it
Just compile and call the executable with the appropriate file paths.
```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./matching ../sample_input_data/pupils.csv ../sample_input_data/students_upper_bound.csv ../sample_input_data/students_lower_bound.csv ../sample_input_data/dissolved_matches.csv ../output/matches.csv
```

Make sure, the folder for the output really exists, otherwise it will not work as expected!