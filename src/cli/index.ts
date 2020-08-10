#!/usr/bin/env node

import { exit } from "process";
import { matchingAlgo } from "../module";

const args = process.argv.slice(2);

//check arguments
if (args.length !== 5) {
    console.log(`This program expects 5 paths as input:
        1. path to CSV with tutee-data
        2. path to CSV with tutor-data (upper bounds)
        3. path to CSV with tutor-data (lower bounds)
        4. path to CSV with dissolved-matches-data
        5. path to where the matching results should be written to
    `);
    
    exit(1);
}

//get arguments
const tuteeInputFile = args[0];
const tutorUpperBoundsInputFile = args[1];
const tutorLowerBoundsInputFile = args[2];
const dissolvedMatchesInputFile = args[3];
const outputFile = args[4];

//perform algorithm
try {
    matchingAlgo(tuteeInputFile, tutorUpperBoundsInputFile, tutorLowerBoundsInputFile, dissolvedMatchesInputFile, outputFile);
    console.log(`Matching results written to ${outputFile}\n`);
}
catch (e) {
    console.log(`🚨 A problem occurred...\n${e}\n`);
    exit(1);
}