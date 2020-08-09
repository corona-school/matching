/// Contains constants the algorithm uses for input and output CSV files
export const ALGO_CONSTANTS = {
    INPUT: {
        FIXED_COLUMN_NAMES: { /// A variable that contains the static column names (the column names that are not dependent on the subject names) from the csv files that the C++ matching algorithm expects to have. 
            tuteeFile: {
                uuid: "uuid"
            }, 
            tutorFiles: {
                uuid: "uuid"
            }, 
            dissolvedMatchesFile: {
                matchUUID: "matchUUID", 
                tutorUUID: "studentID",
                tuteeUUID: "pupilID"
            }
        }
    },
    OUTPUT: {
        COLUMN_NAMES: {
            tutorUUID: "student",
            tuteeUUID: "pupil"
        }
    }
}