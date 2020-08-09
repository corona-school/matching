
//////////////////////////
// Files
//////////////////////////
type FilePath = string;

type InputFiles = {
    tutees: FilePath,
    tutors: {
        upperGradeBound: FilePath,
        lowerGradeBound: FilePath
    }
    dissolvedMatches: FilePath
};
type OutputFile = FilePath;

//////////////////////////
// Input / Output Formats
//////////////////////////
type PersonUUID = string
type Grade = number;

type Subject = {
    name: string
    gradeInformation?: {
        min: Grade
        max: Grade
    }
}
type Tutor = {
    uuid: PersonUUID
    openMatchRequestCount: number
    subjects: Subject[]
}
type Tutee = {
    uuid: PersonUUID
    grade: Grade
    openMatchRequestCount: number
    subjects: Subject[]
}
type Match = {
    tutor: PersonUUID
    tutee: PersonUUID
}
interface Matchable { //defines what criteria are
    subjects: Subject[]
    openMatchRequestCount: number
}

//////////////////////////
// Exports
//////////////////////////
export {
    FilePath,
    InputFiles,
    OutputFile,
    Tutor, 
    Tutee,
    Match,
    Subject, 
    Matchable
}