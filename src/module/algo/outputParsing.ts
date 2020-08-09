import { OutputFile, Match } from "../types";
import { parse as parseCSV } from "papaparse"
import { readFileSync } from "fs";
import { ALGO_CONSTANTS } from "./algoColumnNames";

function outputCSVRowToMatch(row: any): Match {
    return {
        tutor: row[ALGO_CONSTANTS.OUTPUT.COLUMN_NAMES.tutorUUID],
        tutee: row[ALGO_CONSTANTS.OUTPUT.COLUMN_NAMES.tuteeUUID]
    }
}

function parseMatches(algoOutputFile: OutputFile): Match[] {
    const matchesCSV = readFileSync(algoOutputFile, 'utf8');

    const csvRows = parseCSV(matchesCSV, {
        header: true,
        skipEmptyLines: true
    }).data as any[];
    
    const matches = csvRows.map(outputCSVRowToMatch);

    return matches;
}

export {
    parseMatches as parseOutputFile
};