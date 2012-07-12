package finale;

/**
 *
 * @author Marek Šesták
 * @version 1.0
 */
public enum PaymentFrequency {

    IRREGULAR, // 0 <> 0
    ANNUAL, // 1 <> 1
    SEMIANNUAL, // 2 <> 2
    TRIANNUAL, // 3 <> 3
    QUATERLY, // 4 <> 4
    BIMONTHLY, // 5 <> 6
    MONTHLY, // 6 <> 12
    WEEKLY, // 7 <> 52
    DAILY; // 8 <> 365
    private static final int[][] translationTable =
            new int[][]{{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 6}, {6, 12}, {7, 52}, {8, 365}};

    public int toInt() {
        return translationTable[ordinal()][1];
    }

    public static PaymentFrequency toPaymentFrequency(int code) {

        for (int i = 0; i < translationTable.length; i++) {
            if (translationTable[i][1] == code) {
                return values()[i];
            }
        }

        throw new IllegalArgumentException("Illegal numerical value (" +
                code + ") representing payment frequency.");

    }
}
