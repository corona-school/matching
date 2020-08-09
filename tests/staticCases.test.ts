import test from "tape"
import { match } from "../src/module";
import * as path from "path";
import { readdirSync, lstatSync } from "fs";
import { Match, Tutor, Tutee } from "../src/module/types";


////////////////////////////////////////////////////////////////////////////////
// Use this file to test the test cases from the testData directory
////////////////////////////////////////////////////////////////////////////////

namespace TEST_CASES_FORMAT {
    export type INPUT = {
        dissolvedMatches: Match[],
        tutors: Tutor[],
        tutees: Tutee[]
    }
    export type OUTPUT = Match[]
}

const TEST_DATA_DIR = path.join(__dirname,  "./testData");
const TEST_DATA_STATIC_INPUT_NAME = "input.json";
const TEST_DATA_EXPECTED_OUTPUT_NAME = "output.json";

const TEST_CASES_DIR_NAMES = readdirSync(TEST_DATA_DIR).filter( f => lstatSync(path.join(TEST_DATA_DIR, f)).isDirectory()); //only directories

for (const testCaseDir of TEST_CASES_DIR_NAMES) {
    const inputPath = path.join(TEST_DATA_DIR, testCaseDir, TEST_DATA_STATIC_INPUT_NAME);
    const outputPath = path.join(TEST_DATA_DIR, testCaseDir, TEST_DATA_EXPECTED_OUTPUT_NAME);

    //read the json
    const input: TEST_CASES_FORMAT.INPUT = require(inputPath);
    const output: TEST_CASES_FORMAT.OUTPUT = require(outputPath);

    test(`Static test case #${testCaseDir}`, t => {
        const result = match(input.tutees, input.tutors, input.dissolvedMatches);

        t.deepEqual(result, output);
        t.end();
    })
}