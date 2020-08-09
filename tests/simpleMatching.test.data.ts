import { Tutor, Tutee } from "../src/module/types";

//////////////////////////// convention for the test data in this file ////////////////////////////
// 1. `tutorA` and `tuteeA` form a match (use the *letter* to recognize that)
// 2. `unmatchableTutor1` is the first unmatchable tutor (use the *number* to recognize the number)
///////////////////////////////////////////////////////////////////////////////////////////////////

export const tutorA: Tutor = {
    uuid: "TutorA",
    openMatchRequestCount: 1,
    subjects: [
        {
            name: "Deutsch",
            gradeInformation: {
                min: 1,
                max: 8
            }
        },
        { 
            name: "Mathematik",
            gradeInformation: {
                min: 3,
                max: 10
            }
        }
    ]
}
export const tuteeA: Tutee = {
    uuid: "TuteeA",
    openMatchRequestCount: 1,
    grade: 4,
    subjects: [
        {
            name: "Deutsch"
        },
        {
            name: "Mathematik"
        }
    ]
}

export const tutorB: Tutor = {
    uuid: "TutorB",
    openMatchRequestCount: 1,
    subjects: [
        {
            name: "Englisch",
            gradeInformation: {
                min: 7,
                max: 13
            }
        },
        { 
            name: "Physik",
            gradeInformation: {
                min: 3,
                max: 7
            }
        }
    ]
}
export const tuteeC: Tutee = {
    uuid: "TuteeC",
    openMatchRequestCount: 1,
    grade: 8,
    subjects: [
        {
            name: "Deutsch"
        },
        {
            name: "Mathematik"
        }
    ]
}

export const tutorWithOneSubjectA: Tutor = {
    uuid: "tutorWithOneSubjectA",
    openMatchRequestCount: 1,
    subjects: [
        {
            name: "Deutsch",
            gradeInformation: {
                min: 1,
                max: 8
            }
        }
    ]
}
export const tuteeWithOneSubjectA: Tutee = {
    uuid: "tuteeWithOneSubjectA",
    openMatchRequestCount: 1,
    grade: 1,
    subjects: [
        {
            name: "Deutsch"
        }
    ]
}



export const unmatchableTutor1: Tutor = { //is unmatchable due to subjects with empty names
    uuid: "unmatchableTutor1",
    openMatchRequestCount: 1,
    subjects: [
        {
            name: "",
            gradeInformation: {
                min: 1,
                max: 8
            }
        },
        { 
            name: "",
            gradeInformation: {
                min: 3,
                max: 10
            }
        }
    ]
}

export const unmatchableTutee1: Tutee = { //in this test file, no match for that tutee can be found
    uuid: "unmatchableTutee1",
    openMatchRequestCount: 1,
    grade: 4,
    subjects: [
        {
            name: "Altgriechisch"
        },
        {
            name: "Französisch"
        }
    ]
}

export const tuteeWithOneEmptySubjectName1: Tutee = {
    uuid: "tuteeWithOneEmptySubjectName1",
    openMatchRequestCount: 1,
    grade: 4,
    subjects: [
        {
            name: ""
        },
        {
            name: "Mathematik"
        }
    ]
}

export const testDissolvedMatches1 = [{tutee: "testA", tutor: "testB"}];
export const testDissolvedMatches2 = [{tutee: tuteeA.uuid, tutor: tutorA.uuid}];
