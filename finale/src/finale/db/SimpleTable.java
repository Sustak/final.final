package finale.db;

import au.com.bytecode.opencsv.CSVReader;
import finale.utils.Date;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Marek Šesták <marek.sestak@gmail.com>
 * @version 1.0
 */
public class SimpleTable {

    // TODO listener tu sice je, ale je volan jen pri nacitani tabulky, pri
    // pridavani, mazani a update rows ne

    private ArrayList<ColumnDescription> columnDescriptions =
            new ArrayList<ColumnDescription>();
    private ArrayList columns = new ArrayList();
    private int nrows = 0;
    private int capacity = 100;
    private int delta = 100;
    SimpleTableListener listener = null;

    public SimpleTableListener getListener() {
        return listener;
    }

    public void setListener(SimpleTableListener listener) {
        this.listener = listener;
    }

    public int getNRows() {
        return nrows;
    }

    public int getNColumns() {
        return columnDescriptions.size();
    }

    public static class ColumnDescription {

        private String name;
        private FieldType type;

        public ColumnDescription(String name, FieldType type) {
            this.name = name;
            this.type = type;
        }

        public FieldType getClassType() {
            return type;
        }

        public String getName() {
            return name;
        }
    }

    public SimpleTable() {
    }

    public SimpleTable(int capacity, int delta) {
        this.capacity = capacity;
        this.delta = delta;
    }

    private void checkCapacity() {
        if (nrows == capacity) {
            setCapacity(capacity + delta);
        }
    }

    private void setCapacity(int newCapacity) {

        for (int i = 0; i < columnDescriptions.size(); i++) {
            switch (columnDescriptions.get(i).getClassType()) {
                case Integer:
                    columns.set(i, Arrays.copyOf((int[]) columns.get(i), newCapacity));
                    break;
                case Double:
                    columns.set(i, Arrays.copyOf((double[]) columns.get(i), newCapacity));
                    break;
                case Boolean:
                    columns.set(i, Arrays.copyOf((boolean[]) columns.get(i), newCapacity));
                    break;
                case Date:
                    columns.set(i, Arrays.copyOf((int[]) columns.get(i), newCapacity));
                    break;
                case String:
                    columns.set(i, Arrays.copyOf((String[]) columns.get(i), newCapacity));
                    break;
                default:
                    throw new RuntimeException("Column type not recognized. Table might be corrupted!");
            }
        }
        capacity = newCapacity;
    }

    public void addColumn(String columnName, FieldType fieldType) {

        switch (fieldType) {
            case Integer:
            case Date:
                columns.add(new int[capacity]);
                break;
            case Double:
                columns.add(new double[capacity]);
                break;
            case Boolean:
                columns.add(new boolean[capacity]);
                break;
            case String:
                columns.add(new String[capacity]);
                break;
            default:
                throw new IllegalArgumentException("Field type " + fieldType +
                        " not recognized");
        }
        ColumnDescription des = new ColumnDescription(columnName, fieldType);
        columnDescriptions.add(des);

    }

    public int getColumnIndex(String columnName) {
        int i = 0;
        for (ColumnDescription des : columnDescriptions) {
            if (des.getName().toUpperCase().equals(columnName.toUpperCase())) {
                return i;
            }
            i++;
        }
        return -1;
    }

    private void checkRowColumn(int row, int column) {
        if (row < 0 || row >= nrows) {
            throw new IllegalArgumentException("Row out of bounds");
        }
        if (column < 0 || column > columns.size()) {
            throw new IllegalArgumentException("Column out of bounds");
        }
    }

    public int addRow() {

        checkCapacity();

        int newRowIdx = nrows;
        nrows++;
        return newRowIdx;

    }

    public void set(int row, int column, int value) {

        checkRowColumn(row, column);
        switch (columnDescriptions.get(column).getClassType()) {
            case Integer:
                ((int[]) columns.get(column))[row] = value;
                break;
            case Double:
                ((double[]) columns.get(column))[row] = value;
                break;
            case String:
                ((String[]) columns.get(column))[row] = String.valueOf(value);
                break;
            case Date:
                ((int[]) columns.get(column))[row] = value;
                break;
            case Boolean:
                ((boolean[]) columns.get(column))[row] = (value == 0);
                break;
            default:
                throw new RuntimeException("Column type not recognized");
        }
    }

    public void set(int row, int column, boolean value) {

        checkRowColumn(row, column);
        switch (columnDescriptions.get(column).getClassType()) {
            case Integer:
                ((int[]) columns.get(column))[row] = value ? 1 : 0;
                break;
            case Double:
                ((double[]) columns.get(column))[row] = value ? 1.0 : 0.0;
                break;
            case String:
                ((String[]) columns.get(column))[row] = Boolean.toString(value);
                break;
            case Date:
                ((int[]) columns.get(column))[row] = value ? 1 : 0;
                break;
            case Boolean:
                throw new IllegalArgumentException("Boolean to date conversion not supported");
            default:
                throw new RuntimeException("Column type not recognized");
        }
    }

    public void set(int row, int column, double value) {

        checkRowColumn(row, column);
        switch (columnDescriptions.get(column).getClassType()) {
            case Integer:
                ((int[]) columns.get(column))[row] = (int) value;
                break;
            case Double:
                ((double[]) columns.get(column))[row] = value;
                break;
            case String:
                ((String[]) columns.get(column))[row] = String.valueOf(value);
                break;
            case Date:
                ((int[]) columns.get(column))[row] = (int) value;
                break;
            case Boolean:
                ((boolean[]) columns.get(column))[row] = (value == 0.0);
                break;
            default:
                throw new RuntimeException("Column type not recognized");
        }
    }

    public void set(int row, int column, Date value) {

        checkRowColumn(row, column);
        switch (columnDescriptions.get(column).getClassType()) {
            case Integer:
                ((int[]) columns.get(column))[row] = value.toInt();
                break;
            case Double:
                ((double[]) columns.get(column))[row] = (double) value.toInt();
                break;
            case String:
                ((String[]) columns.get(column))[row] = value.toString();
                break;
            case Date:
                ((int[]) columns.get(column))[row] = value.toInt();
                break;
            case Boolean:
                throw new IllegalArgumentException("Date to boolean conversion not supported");
            default:
                throw new RuntimeException("Column type not recognized");
        }
    }

    public void set(int row, int column, String value) {

        checkRowColumn(row, column);
        switch (columnDescriptions.get(column).getClassType()) {
            case Integer:
                ((int[]) columns.get(column))[row] = Integer.valueOf(value);
                break;
            case Double:
                ((double[]) columns.get(column))[row] = Double.valueOf(value);
                break;
            case String:
                ((String[]) columns.get(column))[row] = value;
                break;
            case Date:
                ((int[]) columns.get(column))[row] = Date.getValueOf(value).
                        toInt();
                break;
            case Boolean:
                boolean boolvalue;
                if (value.isEmpty()) {
                    boolvalue = false;
                } else {
                    double doublevalue;
                    try {
                        doublevalue = Double.valueOf(value);
                        if (doublevalue == 0.0) {
                            boolvalue = false;
                        } else {
                            boolvalue = true;
                        }
                    } catch (Exception e) {
                        boolvalue = true;
                    }
                }
                ((boolean[]) columns.get(column))[row] = boolvalue;
                break;
            default:
                throw new RuntimeException("Column type not recognized");
        }
    }

    public int getAsInteger(int row, int column) {

        checkRowColumn(row, column);
        switch (columnDescriptions.get(column).getClassType()) {
            case Integer:
                return ((int[]) columns.get(column))[row];
            case Double:
                return (int) ((double[]) columns.get(column))[row];
            case String:
                String str = ((String[]) columns.get(column))[row];
                return Integer.valueOf(str);
            case Date:
                return ((int[]) columns.get(column))[row];
            case Boolean:
                return ((boolean[]) columns.get(column))[row] ? 1 : 0;
            default:
                throw new RuntimeException("Column type not recognized");
        }
    }

    public double getAsDouble(int row, int column) {

        checkRowColumn(row, column);
        switch (columnDescriptions.get(column).getClassType()) {
            case Integer:
                return (double) ((int[]) columns.get(column))[row];
            case Double:
                return ((double[]) columns.get(column))[row];
            case String:
                String str = ((String[]) columns.get(column))[row];
                return Double.valueOf(str);
            case Date:
                return (double) ((int[]) columns.get(column))[row];
            case Boolean:
                return ((boolean[]) columns.get(column))[row] ? 1.0 : 0.0;
            default:
                throw new RuntimeException("Column type not recognized");
        }
    }

    public boolean getAsBoolean(int row, int column) {

        checkRowColumn(row, column);
        switch (columnDescriptions.get(column).getClassType()) {
            case Integer:
                return ((int[]) columns.get(column))[row] == 0;
            case Double:
                return ((double[]) columns.get(column))[row] == 0.0;
            case String:
                return ((String[]) columns.get(column))[row].equals(Boolean.TRUE.
                        toString());
            case Boolean:
                return ((boolean[]) columns.get(column))[row];
            case Date:
                throw new IllegalArgumentException("Date to boolean conversion not supported");
            default:
                throw new RuntimeException("Column type not recognized");
        }
    }

    public Date getAsDate(int row, int column) {

        checkRowColumn(row, column);
        switch (columnDescriptions.get(column).getClassType()) {
            case Integer:
                return Date.getValueOf(((int[]) columns.get(column))[row]);
            case Double:
                return Date.getValueOf((int) ((double[]) columns.get(column))[row]);
            case String:
                return Date.getValueOf(((String[]) columns.get(column))[row]);
            case Date:
                return Date.getValueOf(((int[]) columns.get(column))[row]);
            case Boolean:
                throw new IllegalArgumentException("Boolean to date conversion not supported");
            default:
                throw new RuntimeException("Column type not recognized");
        }
    }

    public String getAsString(int row, int column) {

        checkRowColumn(row, column);
        switch (columnDescriptions.get(column).getClassType()) {
            case Integer:
                return Integer.toString(((int[]) columns.get(column))[row]);
            case Double:
                return Double.toString(((double[]) columns.get(column))[row]);
            case String:
                return ((String[]) columns.get(column))[row];
            case Date:
                return (Date.getValueOf(((int[]) columns.get(column))[row])).
                        toString();
            case Boolean:
                return Boolean.toString(((boolean[]) columns.get(column))[row]);
            default:
                throw new RuntimeException("Column type not recognized");
        }
    }

    public void add(String[] strings) {
    }

    /**
     * Function appendFromCSVFile appends the table with records
     * contained in a comma-seperated-values (CSV) file.
     * @param csvfilename filename of the CSV file
     * @param hasHeader if true, first line of the CSV file include column names,
     * if false, first column of the CSV file is mapped onto the first column
     * of the table and so on
     * @return Function returns number of records added to the table. </p>
     * If <code>hasHeader</code> is true, the function tries to map columns from
     * the file onto the column names in the table (case insensitive). Columns
     * from the file that are missing the table are ignored. </p>
     * If <code>hasHeader</code> is false, the function maps first column in
     * the file onto a first column in the table, the second to second and so on.
     * Columns from the file that have no corresponding column in the table are
     * ignored.
     */
    public int appendFromCSVFile(String csvfilename, boolean hasHeader) {

        CSVReader reader = null;
        try {
            reader = new CSVReader(new FileReader(csvfilename));
        } catch (Exception e) {
            return 0;
            // TODO Logger
        }

        String[] nextLine;
        int[] columnMapping = null;
        int i;
        int line = 0;

        // is the first row of the csv file a header?
        // if so, take the column names and look them up in the
        // SimpleTable.columnDescriptions list and create a link
        // between them through the columnMapping array

        if (hasHeader) {
            try {
                nextLine = reader.readNext();
                line++;
            } catch (IOException ex) {
                Logger.getLogger(SimpleTable.class.getName()).log(Level.SEVERE,
                        "Header not found in file " + csvfilename);
                return 0;
            }
            i = 0;
            columnMapping = new int[nextLine.length];
            int colIdx;
            for (String columnName : nextLine) {
                columnMapping[i++] = getColumnIndex(columnName);
            }
        }

        int nlines = 0;

        try {

            do {
                try {
                    nextLine = reader.readNext();
                    line++;
                } catch (IOException ex) {
                    Logger.getLogger(SimpleTable.class.getName()).log(Level.SEVERE, null, ex);
                    nextLine = null;
                }
                if (nextLine != null) {

                    nlines++;
                    int rowNum = addRow();
                    int colNum;
                    i = 0;
                    for (String s : nextLine) {
                        if (!hasHeader) {
                            colNum = i;
                        } else {
                            colNum = columnMapping[i];
                        }
                        if (colNum >= 0 && colNum < columns.size()) {
                            this.set(rowNum, colNum, s);
                        }
                        i++;
                    }
                    if (listener != null) {
                        listener.recordInserted(rowNum);
                    }

                }
            } while (nextLine != null);
        } catch (Exception e) {
            throw new RuntimeException("Error reading line " + line + ":" +
                    e.getMessage());
        }

        return nlines;
    }
}
