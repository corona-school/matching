import { FilePath } from "../types";
import { parse as parseJSON } from "json2csv";
import { writeFileSync } from "fs";

export type ColumnCreator<V> = {name: string, cellValue: (value: V) => CellContent };

function getHeadersFromColumnCreators<V>(columnCreators: ColumnCreator<V>[]) {
    return columnCreators.map( cc => cc.name );
}

export type CellContent = string | number | null | undefined;

export type Table = {
    headers: string[]
    rows: object[]
}

export function createTable<T>(columns: ColumnCreator<T>[], rows: T[]): Table {
    //the table
    const table: Table = {
        headers: getHeadersFromColumnCreators(columns),
        rows: []
    };

    //create rows
    for (const row of rows) {
        const rowObject: any = {};
        
        //fill row
        for (const column of columns) {
            rowObject[column.name] = column.cellValue(row);
        }

        //add row to table
        table.rows.push(rowObject)
    }
    
    return table;
}

export function saveTableToCSV(t: Table, path: FilePath) {
    const csvString = parseJSON(t.rows, {header: true, fields: t.headers});
    writeFileSync(path, csvString);
}