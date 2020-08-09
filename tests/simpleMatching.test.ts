import test from "tape"
import { match } from "../src/module";

import * as TestData from "./simpleMatching.test.data";


////////////////////////////////////////////////////////////////////////////////
// Use this file to test some simple (edge) cases, primarily edge cases
////////////////////////////////////////////////////////////////////////////////

test('Only one simple possible match combination', t => {
    const resultSingleSubject = match([TestData.tuteeWithOneSubjectA], [TestData.tutorWithOneSubjectA], []);
    t.deepEqual(resultSingleSubject, [
        {
            tutor: TestData.tutorWithOneSubjectA.uuid,
            tutee: TestData.tuteeWithOneSubjectA.uuid
        }
    ]);

    const resultMultipleSubjects = match([TestData.tuteeA], [TestData.tutorA], []);
    t.deepEqual(resultMultipleSubjects, [
        {
            tutor: TestData.tutorA.uuid,
            tutee: TestData.tuteeA.uuid
        }
    ]);

    t.end();
});

test("No one to match", t => {
    const result = match([], [], []);
    t.deepEqual(result, []);

    t.end();
})

test("Empty subject name", t => {
    const result = match([TestData.tuteeA], [TestData.unmatchableTutor1], []);
    t.deepEqual(result, [])

    t.end();
})

test("No tutee or no tutor", t => {
    const resultNoTutor = match([TestData.tuteeA], [], TestData.testDissolvedMatches1);
    t.deepEqual(resultNoTutor, []);
    
    const resultNoTutee = match([], [TestData.tutorA], TestData.testDissolvedMatches1);
    t.deepEqual(resultNoTutee, []);

    t.end();
})

test("A single subject name is empty", t => {
    const result = match([TestData.tuteeWithOneEmptySubjectName1], [TestData.tutorA], []);
    t.deepEqual(result, [
        {
            tutor: TestData.tutorA.uuid,
            tutee: TestData.tuteeWithOneEmptySubjectName1.uuid
        }
    ]);

    t.end();
});

test("No matches due to dissolvedMatches", t => {
    const result = match([TestData.tuteeA], [TestData.tutorA], TestData.testDissolvedMatches2);
    t.deepEqual(result, []);

    t.end();
})

test("Two with disjunct subjects, i.e. unmatchable", t => {
    const result = match([TestData.tuteeC], [TestData.tutorB], TestData.testDissolvedMatches1);
    t.deepEqual(result, []);

    t.end();
})