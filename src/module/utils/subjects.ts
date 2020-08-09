import { Subject } from "../types"

export function getByName(subjects: Subject[], name: string) {
    return subjects.find( s => s.name === name);
}
