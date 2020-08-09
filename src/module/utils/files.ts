import tmp from "tmp"
import { FilePath } from "../types";
import { unlinkSync } from "fs";

export function createTmpDirectory() {
    try { //this may throw errors...
        return tmp.dirSync();
    }
    catch(e) {
        throw new Error(`Matching Algorithm cannot be executed due to problems with creation and/or access of temporary files! Problem: ${e}`);
    }
}

export function createTmpFile() {
    try {
        return tmp.fileSync();
    }
    catch(e) {
        throw new Error(`Matching Algorithm cannot be executed due to problems with creation and/or access of temporary files! Problem: ${e}`);
    }
}

export function removeFiles(paths: FilePath[]) {
    paths.forEach(unlinkSync);
}