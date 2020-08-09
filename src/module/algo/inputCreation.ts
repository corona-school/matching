import { Tutee, Match, Tutor, Matchable, InputFiles } from "../types";
import { createTable, CellContent, ColumnCreator, saveTableToCSV } from "../utils/table"
import { getByName } from "../utils/subjects"
import { createTmpFile } from "../utils/files";
import { ALGO_CONSTANTS } from "./algoColumnNames";


function multiplyAccordingToOpenMatchRequestCount<T extends Matchable>(matchable: T): T[] {
    const multiplied = [];
    for (let i = 0; i < matchable.openMatchRequestCount; i++) {
        multiplied.push(matchable);
    }

    return multiplied;
}

function findAllSubjectNames(tutees: Tutee[], tutors: Tutor[]) {
    const all: Matchable[] = [...tutees, ...tutors];
    const allSubjectsWithDuplicates = all.flatMap( p => p.subjects.map(s => s.name) );

    return [...new Set(allSubjectsWithDuplicates)]
}

function getColumnCreatorsForSubjectNames<V>(subjectNames: string[], subjectNameToCellContent: (sn: string, v: V) => CellContent) {
    return subjectNames.map( sn => {
        return {
            name: sn,
            cellValue: (v: V) => subjectNameToCellContent(sn, v)
        }
    })
}

// create the table column creators
function getTableColumnCreators(subjectNames: string[]) {
    return {
        tutees: [
            {
                name: ALGO_CONSTANTS.INPUT.FIXED_COLUMN_NAMES.tuteeFile.uuid,
                cellValue: (t: Tutee) => t.uuid
            },
            ...getColumnCreatorsForSubjectNames(subjectNames, (sn: string, v: Tutee) => {
                return getByName(v.subjects, sn) ? v.grade : 0
            })
        ],
        tutorsLowerBoundGrade: [
            {
                name: ALGO_CONSTANTS.INPUT.FIXED_COLUMN_NAMES.tutorFiles.uuid,
                cellValue: (t: Tutor) => t.uuid
            },
            ...getColumnCreatorsForSubjectNames(subjectNames, (sn: string, v: Tutor) => {
                return getByName(v.subjects, sn)?.gradeInformation?.min ?? 0
            })
        ],
        tutorsUpperBoundGrade: [
            {
                name: ALGO_CONSTANTS.INPUT.FIXED_COLUMN_NAMES.tutorFiles.uuid,
                cellValue: (t: Tutor) => t.uuid
            },
            ...getColumnCreatorsForSubjectNames(subjectNames, (sn: string, v: Tutor) => {
                return getByName(v.subjects, sn)?.gradeInformation?.max ?? 0
            })
        ],
        dissolvedMatches: [
            {
                name:ALGO_CONSTANTS.INPUT.FIXED_COLUMN_NAMES.dissolvedMatchesFile.matchUUID,
                cellValue: (m: Match) => null //just a dummy at the moment
            },
            {
                name: ALGO_CONSTANTS.INPUT.FIXED_COLUMN_NAMES.dissolvedMatchesFile.tutorUUID,
                cellValue: (m: Match) => m.tutor
            },
            {
                name: ALGO_CONSTANTS.INPUT.FIXED_COLUMN_NAMES.dissolvedMatchesFile.tuteeUUID,
                cellValue: (m: Match) => m.tutee
            }
        ]
    }
}



function createAndSaveTableToTmpCSV<T>(columnCreators: ColumnCreator<T>[], values: T[]) {
    const table = createTable(columnCreators, values);

    const tmpFile = createTmpFile();

    saveTableToCSV(table, tmpFile.name);

    return tmpFile;
}

export function createInputFiles(tutees: Tutee[], tutors: Tutor[] , dissolvedMatches: Match[]): InputFiles {
    //Respect the open match request count of tutors and tutees
    const tuteesMultiplied = tutees.flatMap(multiplyAccordingToOpenMatchRequestCount);
    const tutorsMultiplied = tutors.flatMap(multiplyAccordingToOpenMatchRequestCount);

    //find all subject names
    const subjectNames = findAllSubjectNames(tuteesMultiplied, tutorsMultiplied);

    //create the function to create the columns (-> "column creators")
    const columnCreators = getTableColumnCreators(subjectNames);

    //save tutees
    const tuteesCSVFile = createAndSaveTableToTmpCSV(columnCreators.tutees, tuteesMultiplied);

    //save tutors
    // ... lower grade bound
    const tutorsLowerGradeCSVFile = createAndSaveTableToTmpCSV(columnCreators.tutorsLowerBoundGrade, tutorsMultiplied);
    // ... upper grade bound
    const tutorsUpperGradeCSVFile = createAndSaveTableToTmpCSV(columnCreators.tutorsUpperBoundGrade, tutorsMultiplied);

    //save dissolved matches
    const dissolvedMatchesCSVFile = createAndSaveTableToTmpCSV(columnCreators.dissolvedMatches, dissolvedMatches);

    return {
        tutees: tuteesCSVFile.name,
        tutors: {
            lowerGradeBound: tutorsLowerGradeCSVFile.name,
            upperGradeBound: tutorsUpperGradeCSVFile.name
        },
        dissolvedMatches: dissolvedMatchesCSVFile.name
    }
}